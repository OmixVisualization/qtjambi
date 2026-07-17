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

#if !defined(QTJAMBI_FUTUREAPI_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_FUTUREAPI_H

#include <QtCore/QLoggingCategory>
#include <QtCore/QPromise>
#include <QtCore/QFutureWatcher>
#include "coreapi.h"
#include "javautils.h"
#include "jnienvironment.h"
#include "jobjectwrapper.h"

Q_DECLARE_EXPORTED_LOGGING_CATEGORY(FUTURE_CATEGORY, QTJAMBI_EXPORT)

#if (defined(Q_OS_ANDROID) /*|| defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD) || defined(Q_OS_OPENBSD) || defined(Q_OS_SOLARIS)*/)
#define HANDLE_EXCEPTION
#define QFUTURE_POINTER_DECL()\
QSharedPointer<QFutureInterfaceBase> futurePointer(new QFutureInterfaceBase);\

#define QFUTURE_POINTER_INIT(in)\
*futurePointer = CoreAPI::futureInterface(in);\

#define QFUTURE_POINTER_ARG ,futurePointer
#define EXCEPTION_HANDLER_ARG(in) ,in
#else
#define QFUTURE_POINTER_DECL()
#define QFUTURE_POINTER_INIT(in)
#define QFUTURE_POINTER_ARG
#define EXCEPTION_HANDLER_ARG(in)
#endif

#define QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(type_name, members) QTJAMBI_REPOSITORY_DECLARE_CLASS_IMPL(QTJAMBI_EXPORT,type_name, members)

namespace Java{
namespace QtCore{
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Runnable,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Runnable1,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Runnable2,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Runnable3,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Runnable4,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Runnable5,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Runnable6,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Runnable7,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Runnable8,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Runnable9,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Callable,
                                 QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Callable1,
                                 QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Callable2,
                                 QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Callable3,
                                 QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Callable4,
                                 QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Callable5,
                                 QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Callable6,
                                 QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Callable7,
                                 QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Callable8,
                                 QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Callable9,
                                 QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(call))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Predicate,
                                 QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(test))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Predicate1,
                                 QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(test))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Predicate2,
                                 QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(test))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Predicate3,
                                 QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(test))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Predicate4,
                                 QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(test))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Predicate5,
                                 QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(test))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Predicate6,
                                 QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(test))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Predicate7,
                                 QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(test))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Predicate8,
                                 QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(test))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$Predicate9,
                                 QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(test))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$RunnableWithPromise,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$RunnableWithPromise1,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$RunnableWithPromise2,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$RunnableWithPromise3,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$RunnableWithPromise4,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$RunnableWithPromise5,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$RunnableWithPromise6,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$RunnableWithPromise7,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$RunnableWithPromise8,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$RunnableWithPromise9,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$RunnableWithVoidPromise,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$RunnableWithVoidPromise1,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$RunnableWithVoidPromise2,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$RunnableWithVoidPromise3,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$RunnableWithVoidPromise4,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$RunnableWithVoidPromise5,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$RunnableWithVoidPromise6,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$RunnableWithVoidPromise7,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$RunnableWithVoidPromise8,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS(QtFuture$RunnableWithVoidPromise9,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(run))
}
}

namespace QtJambiPrivate{
template<typename V>
struct JavaValue{
    static V create(JNIEnv *env, jobject value){
        return ::qtjambi_cast<V>(env, value);
    }
    static void assign(V& v, JNIEnv *env, jobject value){
        v = ::qtjambi_cast<V>(env, value);
    }
    static V create(){
        return V();
    }
};

template<>
struct JavaValue<JObjectWrapper>{
    static JObjectWrapper create(JNIEnv *env, jobject value){
        return JObjectWrapper(env, value);
    }
    static void assign(JObjectWrapper& v, JNIEnv *env, jobject value){
        v.assign(env, value);
    }
    static JObjectWrapper create(){
        return JObjectWrapper();
    }
};

template<size_t>
struct ThreadFunctionCall{
    using Callable = Java::QtCore::QtFuture$Callable;
    using RunnableWithPromise = Java::QtCore::QtFuture$RunnableWithPromise;
    using Runnable = Java::QtCore::QtFuture$Runnable;
    using RunnableWithVoidPromise = Java::QtCore::QtFuture$RunnableWithVoidPromise;
    using Predicate = Java::QtCore::QtFuture$Predicate;
};

template<>
struct ThreadFunctionCall<1>{
    using Callable = Java::QtCore::QtFuture$Callable1;
    using RunnableWithPromise = Java::QtCore::QtFuture$RunnableWithPromise1;
    using Runnable = Java::QtCore::QtFuture$Runnable1;
    using RunnableWithVoidPromise = Java::QtCore::QtFuture$RunnableWithVoidPromise1;
    using Predicate = Java::QtCore::QtFuture$Predicate1;
};

template<>
struct ThreadFunctionCall<2>{
    using Callable = Java::QtCore::QtFuture$Callable2;
    using RunnableWithPromise = Java::QtCore::QtFuture$RunnableWithPromise2;
    using Runnable = Java::QtCore::QtFuture$Runnable2;
    using RunnableWithVoidPromise = Java::QtCore::QtFuture$RunnableWithVoidPromise2;
    using Predicate = Java::QtCore::QtFuture$Predicate2;
};

template<>
struct ThreadFunctionCall<3>{
    using Callable = Java::QtCore::QtFuture$Callable3;
    using RunnableWithPromise = Java::QtCore::QtFuture$RunnableWithPromise3;
    using Runnable = Java::QtCore::QtFuture$Runnable3;
    using RunnableWithVoidPromise = Java::QtCore::QtFuture$RunnableWithVoidPromise3;
    using Predicate = Java::QtCore::QtFuture$Predicate3;
};

template<>
struct ThreadFunctionCall<4>{
    using Callable = Java::QtCore::QtFuture$Callable4;
    using RunnableWithPromise = Java::QtCore::QtFuture$RunnableWithPromise4;
    using Runnable = Java::QtCore::QtFuture$Runnable4;
    using RunnableWithVoidPromise = Java::QtCore::QtFuture$RunnableWithVoidPromise4;
    using Predicate = Java::QtCore::QtFuture$Predicate4;
};

template<>
struct ThreadFunctionCall<5>{
    using Callable = Java::QtCore::QtFuture$Callable5;
    using RunnableWithPromise = Java::QtCore::QtFuture$RunnableWithPromise5;
    using Runnable = Java::QtCore::QtFuture$Runnable5;
    using RunnableWithVoidPromise = Java::QtCore::QtFuture$RunnableWithVoidPromise5;
    using Predicate = Java::QtCore::QtFuture$Predicate5;
};

template<>
struct ThreadFunctionCall<6>{
    using Callable = Java::QtCore::QtFuture$Callable6;
    using RunnableWithPromise = Java::QtCore::QtFuture$RunnableWithPromise6;
    using Runnable = Java::QtCore::QtFuture$Runnable6;
    using RunnableWithVoidPromise = Java::QtCore::QtFuture$RunnableWithVoidPromise6;
    using Predicate = Java::QtCore::QtFuture$Predicate6;
};

template<>
struct ThreadFunctionCall<7>{
    using Callable = Java::QtCore::QtFuture$Callable7;
    using RunnableWithPromise = Java::QtCore::QtFuture$RunnableWithPromise7;
    using Runnable = Java::QtCore::QtFuture$Runnable7;
    using RunnableWithVoidPromise = Java::QtCore::QtFuture$RunnableWithVoidPromise7;
    using Predicate = Java::QtCore::QtFuture$Predicate7;
};

template<>
struct ThreadFunctionCall<8>{
    using Callable = Java::QtCore::QtFuture$Callable8;
    using RunnableWithPromise = Java::QtCore::QtFuture$RunnableWithPromise8;
    using Runnable = Java::QtCore::QtFuture$Runnable8;
    using RunnableWithVoidPromise = Java::QtCore::QtFuture$RunnableWithVoidPromise8;
    using Predicate = Java::QtCore::QtFuture$Predicate8;
};

template<>
struct ThreadFunctionCall<9>{
    using Callable = Java::QtCore::QtFuture$Callable9;
    using RunnableWithPromise = Java::QtCore::QtFuture$RunnableWithPromise9;
    using Runnable = Java::QtCore::QtFuture$Runnable9;
    using RunnableWithVoidPromise = Java::QtCore::QtFuture$RunnableWithVoidPromise9;
    using Predicate = Java::QtCore::QtFuture$Predicate9;
};

template<typename ...Args>
struct ExceptionHandler{
#if defined(HANDLE_EXCEPTION)
    static void handleException(const QSharedPointer<QFutureInterfaceBase>& future, std::exception_ptr exception, Args...){
        while(!future->isValid())
            QThread::msleep(50);
        future->reportException(exception);
    }
#endif
};

template<typename ...Args>
struct ExceptionHandler<QPromise<void>&, Args...>{
#if defined(HANDLE_EXCEPTION)
    static void handleException(const QSharedPointer<QFutureInterfaceBase>&, std::exception_ptr exception, QPromise<void>& promise, Args...){
        QFutureInterfaceBase& future = reinterpret_cast<QFutureInterfaceBase&>(promise);
        future.reportException(exception);
    }
    static void handleException(std::exception_ptr exception, QPromise<void>& promise, Args...){
        QFutureInterfaceBase& future = reinterpret_cast<QFutureInterfaceBase&>(promise);
        future.reportException(exception);
    }
#endif
};

template<typename T, typename ...Args>
struct ExceptionHandler<QPromise<T>&, Args...>{
#if defined(HANDLE_EXCEPTION)
    static void handleException(const QSharedPointer<QFutureInterfaceBase>&, std::exception_ptr exception, QPromise<T>& promise, Args...){
        QFutureInterfaceBase& future = reinterpret_cast<QFutureInterfaceBase&>(promise);
        future.reportException(exception);
    }
    static void handleException(std::exception_ptr exception, QPromise<T>& promise, Args...){
        QFutureInterfaceBase& future = reinterpret_cast<QFutureInterfaceBase&>(promise);
        future.reportException(exception);
    }
#endif
};

template<typename Fun>
struct FunConverter{
    Q_STATIC_ASSERT_X(std::is_const_v<Fun>, "Cannot convert function to given type.");
};

template<typename... Args>
struct first_is_reference : std::false_type{

};

template<typename A, typename... Args>
struct first_is_reference_impl : std::true_type{
    static constexpr void call(JNIEnv* env, jobject functor, A a, Args... args){
        jobject value = ThreadFunctionCall<sizeof...(Args)+1>::Callable::call(env, functor, ::qtjambi_cast<jobject>(env, std::as_const(a)), ::qtjambi_cast<jobject>(env, std::forward<Args>(args))...);
        a = ::qtjambi_cast<std::remove_reference_t<A>>(env, value);
    }
};

template<typename A, typename... Args>
struct first_is_reference<A,Args...> : std::conditional_t<std::is_lvalue_reference_v<A> && !std::is_const_v<std::remove_reference_t<A>>,first_is_reference_impl<A,Args...>,std::false_type>{

};

template<typename R, typename... Args>
struct FunConverter<R(Args...)>{
    static R call(JNIEnv* env, jobject functor, Args... args){
        if(functor){
            if constexpr(std::is_same_v<R,void>){
                if constexpr(first_is_reference<Args...>::value){
                    first_is_reference<Args...>::call(env, functor, std::forward<Args>(args)...);
                }else{
                    ThreadFunctionCall<sizeof...(Args)>::Runnable::run(env, functor, ::qtjambi_cast<jobject>(env, std::forward<Args>(args))...);
                }
            }else if constexpr(std::is_same_v<R,bool>){
                return ThreadFunctionCall<sizeof...(Args)>::Predicate::test(env, functor, ::qtjambi_cast<jobject>(env, std::forward<Args>(args))...);
            }else{
                jobject value = ThreadFunctionCall<sizeof...(Args)>::Callable::call(env, functor, ::qtjambi_cast<jobject>(env, std::forward<Args>(args))...);
                return ::qtjambi_cast<R>(env, value);
            }
        }else
            qCWarning(FUTURE_CATEGORY) << "Run functor called with invalid data. JNI Environment == " << env << ", java functor object == null";
        if constexpr(!std::is_same_v<R,void>){
            return R{};
        }
    }
    static constexpr auto convertNullable(JNIEnv* env, jobject function){
        std::function<R(Args...)> __qt_function;
        if(function){
            __qt_function = [wrapper = JObjectWrapper(env, function)](Args... args){
                if(JniEnvironment env{200}){
                    return call(env, wrapper.object(env), std::forward<Args>(args)...);
                }
                if constexpr(!std::is_same_v<R,void>){
                    return R{};
                }
            };
        }
        return __qt_function;
    }
    static constexpr auto convert(JNIEnv* env, jobject function){
        Q_ASSERT(function);
        return [wrapper = JObjectWrapper(env, function)](Args... args){
            if(JniEnvironment env{200}){
                return call(env, wrapper.object(env), std::forward<Args>(args)...);
            }
            if constexpr(!std::is_same_v<R,void>){
                return R{};
            }
        };
    }
    template<typename Data>
    static constexpr auto convert(JNIEnv* env, jobject function, std::shared_ptr<Data>&& data){
        Q_ASSERT(function);
        return [wrapper = JObjectWrapper(env, function), data = std::move(data)](Args... args){
            if(JniEnvironment env{200}){
                Q_UNUSED(data)
                return call(env, wrapper.object(env), std::forward<Args>(args)...);
            }
            if constexpr(!std::is_same_v<R,void>){
                return R{};
            }
        };
    }
    static constexpr auto convert(JNIEnv* env, jobject function, const QSharedPointer<QFutureInterfaceBase>& futurePointer){
        Q_ASSERT(function);
        return [wrapper = JObjectWrapper(env, function), futurePointer](Args... args){
            if(JniEnvironment env{200}){
#if defined(HANDLE_EXCEPTION)
                QTJAMBI_TRY{
                    return call(env, wrapper.object(env), std::forward<Args>(args)...);
                }QTJAMBI_CATCH(const JavaException& exn) {
                    ExceptionHandler<Args...>::handleException(futurePointer, std::make_exception_ptr(QUnhandledException(std::make_exception_ptr(exn))), args...);
                }QTJAMBI_CATCH(...) {
                    ExceptionHandler<Args...>::handleException(futurePointer, std::current_exception(), args...);
                }QTJAMBI_TRY_END
#else
                Q_UNUSED(futurePointer)
                return call(env, wrapper.object(env), std::forward<Args>(args)...);
#endif
            }
            if constexpr(!std::is_same_v<R,void>){
                return R{};
            }
        };
    }
    template<typename ExceptionHandler>
    static constexpr auto convert(JNIEnv* env, jobject function, QWeakPointer<ExceptionHandler>&& exceptionHandler){
        Q_ASSERT(function);
        return [wrapper = JObjectWrapper(env, function), exceptionHandler](Args... args){
            if(JniEnvironment env{200}){
#if defined(HANDLE_EXCEPTION)
                QTJAMBI_TRY{
                    return call(env, wrapper.object(env), std::forward<Args>(args)...);
                }QTJAMBI_CATCH(const JavaException& exn) {
                    if(QSharedPointer<ExceptionHandler> eh = exceptionHandler){
                        eh->reportException(std::make_exception_ptr(QUnhandledException(std::make_exception_ptr(exn))));
                    }else{
                        qCWarning(FUTURE_CATEGORY) << exn.what();
                    }
                }QTJAMBI_CATCH(const std::exception& e) {
                    if(QSharedPointer<ExceptionHandler> eh = exceptionHandler){
                        eh->reportException(std::make_exception_ptr(e));
                    }else{
                        qCWarning(FUTURE_CATEGORY) << e.what();
                    }
                }QTJAMBI_CATCH(...) {
                    if(QSharedPointer<ExceptionHandler> eh = exceptionHandler){
                        eh->reportException(std::current_exception());
                    }
                }QTJAMBI_TRY_END
#else
                Q_UNUSED(exceptionHandler)
                return call(env, wrapper.object(env), std::forward<Args>(args)...);
#endif
            }
            if constexpr(!std::is_same_v<R,void>){
                return R{};
            }
        };
    }
    template<typename ExceptionHandler>
    static constexpr auto convert(JNIEnv* env, jobject function, const ExceptionHandler& exceptionHandler){
        Q_ASSERT(function);
        return [wrapper = JObjectWrapper(env, function), exceptionHandler](Args... args){
            if(JniEnvironment env{200}){
#if defined(HANDLE_EXCEPTION)
                QTJAMBI_TRY{
                    return call(env, wrapper.object(env), std::forward<Args>(args)...);
                }QTJAMBI_CATCH(const JavaException& exn) {
                    exceptionHandler.reportException(std::make_exception_ptr(QUnhandledException(std::make_exception_ptr(exn))));
                }QTJAMBI_CATCH(const std::exception& e) {
                    exceptionHandler.reportException(std::make_exception_ptr(e));
                }QTJAMBI_CATCH(...) {
                    exceptionHandler.reportException(std::current_exception());
                }QTJAMBI_TRY_END
#else
                Q_UNUSED(exceptionHandler)
                return call(env, wrapper.object(env), std::forward<Args>(args)...);
#endif
            }
            if constexpr(!std::is_same_v<R,void>){
                return R{};
            }
        };
    }
    template<typename T>
    static constexpr auto convert(JNIEnv* env, jobject function, QPointer<QFutureWatcher<T>>&& watcher){
        Q_ASSERT(function);
        return [wrapper = JObjectWrapper(env, function), watcher = std::move(watcher)](Args... args){
            if(JniEnvironment env{200}){
#if defined(HANDLE_EXCEPTION)
                QTJAMBI_TRY{
                    return call(env, wrapper.object(env), std::forward<Args>(args)...);
                }QTJAMBI_CATCH(const JavaException& exn) {
                    if(watcher){
                        QFuture<T> future = watcher->future();
                        static_cast<QFutureInterface<T>&>(CoreAPI::futureInterface(future)).reportException(std::make_exception_ptr(QUnhandledException(std::make_exception_ptr(exn))));
                    }else{
                        qCWarning(FUTURE_CATEGORY) << exn.what();
                    }
                }QTJAMBI_CATCH(const std::exception& e) {
                    if(watcher){
                        QFuture<T> future = watcher->future();
                        static_cast<QFutureInterface<T>&>(CoreAPI::futureInterface(future)).reportException(std::make_exception_ptr(e));
                    }else{
                        qCWarning(FUTURE_CATEGORY) << e.what();
                    }
                }QTJAMBI_CATCH(...) {
                    if(watcher){
                        QFuture<T> future = watcher->future();
                        static_cast<QFutureInterface<T>&>(CoreAPI::futureInterface(future)).reportException(std::current_exception());
                    }
                }QTJAMBI_TRY_END
#else
                Q_UNUSED(watcher)
                return call(env, wrapper.object(env), std::forward<Args>(args)...);
#endif
            }
            if constexpr(!std::is_same_v<R,void>){
                return R{};
            }
        };
    }

    static void run(JNIEnv* env, jobject functor, Args... args){
        if(functor){
            if constexpr(std::is_same_v<R,void>){
                if constexpr(first_is_reference<Args...>::value){
                    first_is_reference<Args...>::call(env, functor, std::forward<Args>(args)...);
                }else{
                    ThreadFunctionCall<sizeof...(Args)>::Runnable::run(env, functor, ::qtjambi_cast<jobject>(env, std::forward<Args>(args))...);
                }
            }else if constexpr(std::is_same_v<R,bool>){
                return ThreadFunctionCall<sizeof...(Args)>::Predicate::test(env, functor, ::qtjambi_cast<jobject>(env, std::forward<Args>(args))...);
            }else{
                jobject value = ThreadFunctionCall<sizeof...(Args)>::Callable::call(env, functor, ::qtjambi_cast<jobject>(env, std::forward<Args>(args))...);
                return ::qtjambi_cast<R>(env, value);
            }
        }else
            qCWarning(FUTURE_CATEGORY) << "Run functor called with invalid data. JNI Environment == " << env << ", java functor object == null";
        if constexpr(!std::is_same_v<R,void>){
            return R{};
        }
    }
    static constexpr auto convertRunnableNullable(JNIEnv* env, jobject function){
        std::function<R(Args...)> __qt_function;
        if(function){
            __qt_function = [wrapper = JObjectWrapper(env, function)](Args... args) {
                if(JniEnvironment env{200}){
                    run(env, wrapper.object(env), std::forward<Args>(args)...);
                }
            };
        }
        return __qt_function;
    }
    static constexpr auto convertRunnable(JNIEnv* env, jobject function){
        Q_ASSERT(function);
        return [wrapper = JObjectWrapper(env, function)](Args... args) {
            if(JniEnvironment env{200}){
                run(env, wrapper.object(env), std::forward<Args>(args)...);
            }
        };
    }
    template<typename Data>
    static constexpr auto convertRunnable(JNIEnv* env, jobject function, std::shared_ptr<Data>&& data){
        Q_ASSERT(function);
        return [wrapper = JObjectWrapper(env, function), data = std::move(data)](Args... args) {
            if(JniEnvironment env{200}){
                Q_UNUSED(data)
                run(env, wrapper.object(env), std::forward<Args>(args)...);
            }
        };
    }
    static constexpr auto convertRunnable(JNIEnv* env, jobject function, const QSharedPointer<QFutureInterfaceBase>& futurePointer){
        Q_ASSERT(function);
        return [wrapper = JObjectWrapper(env, function), futurePointer](Args... args) {
            if(JniEnvironment env{200}){
#if defined(HANDLE_EXCEPTION)
                QTJAMBI_TRY{
                    run(env, wrapper.object(env), std::forward<Args>(args)...);
                }QTJAMBI_CATCH(const JavaException& exn) {
                    ExceptionHandler<Args...>::handleException(futurePointer, std::make_exception_ptr(QUnhandledException(std::make_exception_ptr(exn))), args...);
                }QTJAMBI_CATCH(...) {
                    ExceptionHandler<Args...>::handleException(futurePointer, std::current_exception(), args...);
                }QTJAMBI_TRY_END
#else
                Q_UNUSED(futurePointer)
                run(env, wrapper.object(env), std::forward<Args>(args)...);
#endif
            }
        };
    }
    template<typename ExceptionHandler>
    static constexpr auto convertRunnable(JNIEnv* env, jobject function, QWeakPointer<ExceptionHandler>&& exceptionHandler){
        Q_ASSERT(function);
        return [wrapper = JObjectWrapper(env, function), exceptionHandler](Args... args) {
            if(JniEnvironment env{200}){
#if defined(HANDLE_EXCEPTION)
                QTJAMBI_TRY{
                    run(env, wrapper.object(env), std::forward<Args>(args)...);
                }QTJAMBI_CATCH(const JavaException& exn) {
                    if(QSharedPointer<ExceptionHandler> eh = exceptionHandler){
                        eh->reportException(std::make_exception_ptr(QUnhandledException(std::make_exception_ptr(exn))));
                    }else{
                        qCWarning(FUTURE_CATEGORY) << exn.what();
                    }
                }QTJAMBI_CATCH(const std::exception& e) {
                    if(QSharedPointer<ExceptionHandler> eh = exceptionHandler){
                        eh->reportException(std::make_exception_ptr(e));
                    }else{
                        qCWarning(FUTURE_CATEGORY) << e.what();
                    }
                }QTJAMBI_CATCH(...) {
                    if(QSharedPointer<ExceptionHandler> eh = exceptionHandler){
                        eh->reportException(std::current_exception());
                    }
                }QTJAMBI_TRY_END
#else
                Q_UNUSED(exceptionHandler)
                run(env, wrapper.object(env), std::forward<Args>(args)...);
#endif
            }
        };
    }
    template<typename ExceptionHandler>
    static constexpr auto convertRunnable(JNIEnv* env, jobject function, const ExceptionHandler& exceptionHandler){
        Q_ASSERT(function);
        return [wrapper = JObjectWrapper(env, function), exceptionHandler](Args... args) {
            if(JniEnvironment env{200}){
#if defined(HANDLE_EXCEPTION)
                QTJAMBI_TRY{
                    run(env, wrapper.object(env), std::forward<Args>(args)...);
                }QTJAMBI_CATCH(const JavaException& exn) {
                    exceptionHandler.reportException(std::make_exception_ptr(QUnhandledException(std::make_exception_ptr(exn))));
                }QTJAMBI_CATCH(const std::exception& e) {
                    exceptionHandler.reportException(std::make_exception_ptr(e));
                }QTJAMBI_CATCH(...) {
                    exceptionHandler.reportException(std::current_exception());
                }QTJAMBI_TRY_END
#else
                Q_UNUSED(exceptionHandler)
                run(env, wrapper.object(env), std::forward<Args>(args)...);
#endif
            }
        };
    }
    template<typename T>
    static constexpr auto convertRunnable(JNIEnv* env, jobject function, QPointer<QFutureWatcher<T>>&& watcher){
        Q_ASSERT(function);
        return [wrapper = JObjectWrapper(env, function), watcher = std::move(watcher)](Args... args) {
            if(JniEnvironment env{200}){
#if defined(HANDLE_EXCEPTION)
                QTJAMBI_TRY{
                    run(env, wrapper.object(env), std::forward<Args>(args)...);
                }QTJAMBI_CATCH(const JavaException& exn) {
                    if(watcher){
                        QFuture<T> future = watcher->future();
                        static_cast<QFutureInterface<T>&>(CoreAPI::futureInterface(future)).reportException(std::make_exception_ptr(QUnhandledException(std::make_exception_ptr(exn))));
                    }else{
                        qCWarning(FUTURE_CATEGORY) << exn.what();
                    }
                }QTJAMBI_CATCH(const std::exception& e) {
                    if(watcher){
                        QFuture<T> future = watcher->future();
                        static_cast<QFutureInterface<T>&>(CoreAPI::futureInterface(future)).reportException(std::make_exception_ptr(e));
                    }else{
                        qCWarning(FUTURE_CATEGORY) << e.what();
                    }
                }QTJAMBI_CATCH(...) {
                    if(watcher){
                        QFuture<T> future = watcher->future();
                        static_cast<QFutureInterface<T>&>(CoreAPI::futureInterface(future)).reportException(std::current_exception());
                    }
                }QTJAMBI_TRY_END
#else
                Q_UNUSED(watcher)
                run(env, wrapper.object(env), std::forward<Args>(args)...);
#endif
            }
        };
    }
};

template<typename R, typename... Args>
struct FunConverter<R(&)(Args...)> : FunConverter<R(Args...)>{
};

template<typename R, typename... Args>
struct FunConverter<R(*)(Args...)> : FunConverter<R(Args...)>{
};

template<typename R, typename... Args>
struct FunConverter<std::function<R(Args...)>> : FunConverter<R(Args...)>{
};

template<typename R, typename... Args>
struct FunConverter<void(QPromise<R>&,Args...)>{
    static auto convertNullable(JNIEnv* env, jobject function){
        std::function<void(QPromise<R>&,Args...)> __qt_function;
        if(function){
            __qt_function = [wrapper = JObjectWrapper(env, function)](QPromise<R>& promise,Args... args) {
                if(JniEnvironment env{200}){
                    QTJAMBI_TRY{
                        using Runnable = std::conditional_t<std::is_same_v<R,void>,
                                                            typename ThreadFunctionCall<sizeof...(Args)>::RunnableWithVoidPromise,
                                                            typename ThreadFunctionCall<sizeof...(Args)>::RunnableWithPromise>;
                        if(jobject functor = wrapper.object(env))
                            Runnable::run(env, functor, ::qtjambi_cast<jobject>(env, promise), ::qtjambi_cast<jobject>(env, std::forward<Args>(args))...);
                        else
                            qCWarning(FUTURE_CATEGORY) << "Run functor called with invalid data. JNI Environment == " << env.environment() << ", java functor object == null";
                    }QTJAMBI_CATCH(const JavaException& exn){
                        promise.setException(exn);
                    }QTJAMBI_TRY_END
                }
            };
        }
        return __qt_function;
    }
    static auto convert(JNIEnv* env, jobject function){
        return [wrapper = JObjectWrapper(env, function)](QPromise<R>& promise,Args... args) {
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    using Runnable = std::conditional_t<std::is_same_v<R,void>,
                                                        typename ThreadFunctionCall<sizeof...(Args)>::RunnableWithVoidPromise,
                                                        typename ThreadFunctionCall<sizeof...(Args)>::RunnableWithPromise>;
                    if(jobject functor = wrapper.object(env))
                        Runnable::run(env, functor, ::qtjambi_cast<jobject>(env, promise), ::qtjambi_cast<jobject>(env, std::forward<Args>(args))...);
                    else
                        qCWarning(FUTURE_CATEGORY) << "Run functor called with invalid data. JNI Environment == " << env.environment() << ", java functor object == null";
                }QTJAMBI_CATCH(const JavaException& exn){
                    promise.setException(exn);
                }QTJAMBI_TRY_END
            }
        };
    }
    template<typename Data>
    static constexpr auto convert(JNIEnv* env, jobject function, Data&& data){
        return [wrapper = JObjectWrapper(env, function), data = std::move(data)](QPromise<R>& promise,Args... args) {
            Q_UNUSED(data)
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    using Runnable = std::conditional_t<std::is_same_v<R,void>,
                                                        typename ThreadFunctionCall<sizeof...(Args)>::RunnableWithVoidPromise,
                                                        typename ThreadFunctionCall<sizeof...(Args)>::RunnableWithPromise>;
                    if(jobject functor = wrapper.object(env))
                        Runnable::run(env, functor, ::qtjambi_cast<jobject>(env, promise), ::qtjambi_cast<jobject>(env, std::forward<Args>(args))...);
                    else
                        qCWarning(FUTURE_CATEGORY) << "Run functor called with invalid data. JNI Environment == " << env.environment() << ", java functor object == null";
                }QTJAMBI_CATCH(const JavaException& exn){
                    promise.setException(exn);
                }QTJAMBI_TRY_END
            }
        };
    }
    template<typename Data>
    static constexpr auto convert(JNIEnv* env, jobject function, const Data& data){
        return [wrapper = JObjectWrapper(env, function), data](QPromise<R>& promise,Args... args) {
            Q_UNUSED(data)
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    using Runnable = std::conditional_t<std::is_same_v<R,void>,
                                                        typename ThreadFunctionCall<sizeof...(Args)>::RunnableWithVoidPromise,
                                                        typename ThreadFunctionCall<sizeof...(Args)>::RunnableWithPromise>;
                    if(jobject functor = wrapper.object(env))
                        Runnable::run(env, functor, ::qtjambi_cast<jobject>(env, promise), ::qtjambi_cast<jobject>(env, std::forward<Args>(args))...);
                    else
                        qCWarning(FUTURE_CATEGORY) << "Run functor called with invalid data. JNI Environment == " << env.environment() << ", java functor object == null";
                }QTJAMBI_CATCH(const JavaException& exn){
                    promise.setException(exn);
                }QTJAMBI_TRY_END
            }
        };
    }
};

template<typename R, typename... Args>
struct FunConverter<void(&)(QPromise<R>&,Args...)> : FunConverter<void(QPromise<R>&,Args...)>{
};

template<typename R, typename... Args>
struct FunConverter<void(*)(QPromise<R>&,Args...)> : FunConverter<void(QPromise<R>&,Args...)>{
};

template<typename R, typename... Args>
struct FunConverter<std::function<void(QPromise<R>&,Args...)>> : FunConverter<void(QPromise<R>&,Args...)>{
};

}

namespace FutureAPI{

template<typename Fun>
auto convert(JNIEnv* env, jobject function, const QSharedPointer<QFutureInterfaceBase>& futurePointer){
    return QtJambiPrivate::FunConverter<Fun>::convert(env, function, futurePointer);
}

template<typename Fun, typename ExceptionHandler>
auto convert(JNIEnv* env, jobject function, QWeakPointer<ExceptionHandler>&& exceptionHandler){
    return QtJambiPrivate::FunConverter<Fun>::convert(env, function, exceptionHandler);
}

template<typename Fun, typename ExceptionHandler>
auto convert(JNIEnv* env, jobject function, const QSharedPointer<ExceptionHandler>& exceptionHandler){
    return QtJambiPrivate::FunConverter<Fun>::convert(env, function, exceptionHandler.toWeakRef());
}

template<typename Fun, typename ExceptionHandler>
auto convert(JNIEnv* env, jobject function, const ExceptionHandler& exceptionHandler){
    return QtJambiPrivate::FunConverter<Fun>::convert(env, function, exceptionHandler);
}

template<typename Fun, typename Data>
auto convert(JNIEnv* env, jobject function, std::shared_ptr<Data>&& data){
    return QtJambiPrivate::FunConverter<Fun>::convert(env, function, std::move(data));
}

template<typename Fun, typename T>
auto convert(JNIEnv* env, jobject function, QFutureWatcher<T>* watcher){
    return QtJambiPrivate::FunConverter<Fun>::convert(env, function, QPointer<QFutureWatcher<T>>(watcher));
}

template<typename Fun>
auto convert(JNIEnv* env, jobject function){
    return QtJambiPrivate::FunConverter<Fun>::convert(env, function);
}

template<typename Fun>
auto convertNullable(JNIEnv* env, jobject function){
    return QtJambiPrivate::FunConverter<Fun>::convertNullable(env, function);
}

}

#undef QTJAMBI_REPOSITORY_DECLARE_EXPORTED_CLASS

#endif // QTJAMBI_FUTUREAPI_H
