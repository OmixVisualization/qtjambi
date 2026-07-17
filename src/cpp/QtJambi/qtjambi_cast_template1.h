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

#ifndef QTJAMBI_CAST_TEMPLATE1_H
#define QTJAMBI_CAST_TEMPLATE1_H

#include "qtjambi_cast.h"
#include "qtjambiapi.h"

QT_WARNING_DISABLE_GCC("-Wstrict-aliasing")
QT_WARNING_DISABLE_DEPRECATED

namespace QtJambiPrivate {

template<bool forward,
         typename JniType,
         template<typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast : decltype(qtjambi_jobject_template_plain_cast<forward, JniType, NativeType<T>, is_pointer, is_const, is_reference, is_rvalue, Args...>()){
};

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_qmllist_cast;

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      QQmlListProperty, is_pointer, is_const, is_reference, is_rvalue,
                                      T, Args...> : qtjambi_jobject_qmllist_cast<forward, is_pointer, is_const, is_reference, is_rvalue, T, Args...>{};

//template from any QFlags to jobject
template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      QFlags, is_pointer, is_const, is_reference, is_rvalue,
                                      T, Args...>{
    typedef QFlags<T> NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, jobject> In;
    typedef std::conditional_t<forward, jobject, NativeType_out> Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            return QtJambiAPI::convertQFlagsToJavaObject<T>(env, deref_ptr<is_pointer,NativeType_c>::deref(in));
        }else{
            if constexpr(is_pointer || is_reference){
                Q_STATIC_ASSERT_X(!is_reference || cast_var_args<Args...>::hasScope, "Cannot cast to QFlags<T>& without scope");
                Q_STATIC_ASSERT_X(!is_pointer || cast_var_args<Args...>::hasScope, "Cannot cast to QFlags<T>* without scope");
                NativeType* result = create<NativeType>(0);
                if(!QtJambiAPI::convertJavaToNative(env, in, result, typeid(NativeType))){
                    delete result;
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null")).arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                }
                if constexpr(is_const){
                    cast_var_args<Args...>::scope(args...).addDeletion(result);
                }else{
#if defined(QTJAMBI_JOBJECTWRAPPER_H)
                    cast_var_args<Args...>::scope(args...).addFinalAction([result, wrapper = JObjectWrapper(env, in)](){
                        if(JniEnvironment env{128}){
                            jobject o = wrapper.object(env);
                            if constexpr(sizeof(NativeType::Int)==sizeof(jlong))
                                QtJambiAPI::setFlagsValue(env, o, jlong(result->toInt()));
                            else
                                QtJambiAPI::setFlagsValue(env, o, jint(result->toInt()));
                        }
                        delete result;
                    });
#else
                    cast_var_args<Args...>::scope(args...).addFinalAction([result, env, in](){
                        if constexpr(sizeof(NativeType::Int)==sizeof(jlong))
                            QtJambiAPI::setFlagsValue(env, in, jlong(result->toInt()));
                        else
                            QtJambiAPI::setFlagsValue(env, in, jint(result->toInt()));
                        delete result;
                    });
#endif //defined(QTJAMBI_JOBJECTWRAPPER_H)
                }

                if constexpr(is_pointer){
                    return result;
                }else{
                    return qtjambi_deref_value<NativeType, is_default_constructible_v<NativeType>, is_copy_constructible_v<NativeType>, is_const, is_reference>::deref(env, result);
                }
            }else{
                NativeType result;
                if(!QtJambiAPI::convertJavaToNative(env, in, &result, typeid(NativeType))){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null")).arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                }
                return result;
            }
        }
    }
};

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      QBindable, is_pointer, is_const, is_reference, is_rvalue,
                                      T, Args...>{
    typedef QBindable<T> NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, jobject> In;
    typedef std::conditional_t<forward, jobject, NativeType_out> Out;

    static Out cast(In in, Args...args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            NativeType_c* _in = ref_ptr<is_pointer, NativeType_c>::ref(in);
            return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, _in, qtjambi_type<NativeType>::id());
        }else{
            return QtJambiAPI::convertJavaObjectToNative<NativeType>(env, in);
        }
    }
};

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      QPropertyBinding, is_pointer, is_const, is_reference, is_rvalue,
                                      T, Args...>{
    typedef QPropertyBinding<T> NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, jobject> In;
    typedef std::conditional_t<forward, jobject, NativeType_out> Out;

    static Out cast(In in, Args...args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            NativeType_c* _in = ref_ptr<is_pointer, NativeType_c>::ref(in);
            return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, _in, qtjambi_type<NativeType>::id());
        }else{
            return QtJambiAPI::convertJavaObjectToNative<NativeType>(env, in);
        }
    }
};

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      QPropertyChangeHandler, is_pointer, is_const, is_reference, is_rvalue,
                                      T, Args...>{
    typedef QPropertyChangeHandler<T> NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, jobject> In;
    typedef std::conditional_t<forward, jobject, NativeType_out> Out;

    static Out cast(In in, Args...args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            NativeType_c* _in = ref_ptr<is_pointer, NativeType_c>::ref(in);
            return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, _in, qtjambi_type<NativeType>::id());
        }else{
            return QtJambiAPI::convertJavaObjectToNative<NativeType>(env, in);
        }
    }
};

template<bool forward,
         template<typename T> class Future, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_future_cast;

template<bool forward,
         template<typename T> class Future, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
constexpr auto qtjambi_jobject_future_cast_decider(){
    QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/FutureCast, is_complete_v<qtjambi_jobject_future_cast<forward,Future,is_pointer,is_const,is_reference,is_rvalue,T,Args...>>);
    return qtjambi_jobject_future_cast<forward,Future,is_pointer,is_const,is_reference,is_rvalue,T,Args...>{};
};

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      QFutureInterface, is_pointer, is_const, is_reference, is_rvalue,
                                      T, Args...> : decltype(qtjambi_jobject_future_cast_decider<forward,QFutureInterface,is_pointer,is_const,is_reference,is_rvalue,T,Args...>()){};

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      QFutureWatcher, is_pointer, is_const, is_reference, is_rvalue,
                                      T, Args...> : decltype(qtjambi_jobject_future_cast_decider<forward,QFutureWatcher,is_pointer,is_const,is_reference,is_rvalue,T,Args...>()){};

template<bool forward, typename T,
             bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      QPromise, is_pointer, is_const, is_reference, is_rvalue,
                                      T, Args...> : decltype(qtjambi_jobject_future_cast_decider<forward,QPromise,is_pointer,is_const,is_reference,is_rvalue,T,Args...>()){};

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      QFuture, is_pointer, is_const, is_reference, is_rvalue,
                                      T, Args...> : decltype(qtjambi_jobject_future_cast_decider<forward,QFuture,is_pointer,is_const,is_reference,is_rvalue,T,Args...>()){};

#if defined(_OPTIONAL_) || defined(_OPTIONAL) || (defined(_LIBCPP_OPTIONAL) && _LIBCPP_STD_VER > 14) || defined(_GLIBCXX_OPTIONAL)

template<typename ElementType>
struct qtjambi_optional{
    template<typename Optional, typename... Args>
    static ElementType value(Optional& optional, Args...){
        return optional.has_value() ? ElementType(optional.value()) : ElementType(0);
    }
    template<typename T, typename... Args>
    static std::optional<T> create(ElementType value, Args...){
        return std::optional<T>(value);
    }
};

template<>
struct qtjambi_optional<jobject>{
    template<typename Optional, typename... Args>
    static jobject value(Optional& optional, Args... args){
        return optional.has_value() ? qtjambi_cast_with_args<jobject>(optional.value(), std::forward<Args>(args)...) : nullptr;
    }
    template<typename T, typename... Args>
    static std::optional<T> create(jobject value, Args... args){
        return std::optional<T>(qtjambi_cast_with_args<T>(value, std::forward<Args>(args)...));
    }
};

template<bool forward, class JniType, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue, typename T, typename... Args>
struct qtjambi_smart_pointer_cast;

template<bool forward, class JniType, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue, typename T, typename... Args>
static constexpr auto find_qtjambi_smart_pointer_cast() {
    QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/SmartPointerCast, is_complete_v< qtjambi_smart_pointer_cast<forward, JniType, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, T, Args...> >);
    return qtjambi_smart_pointer_cast<forward, JniType, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, T, Args...>{};
}

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      std::optional, is_pointer, is_const, is_reference, is_rvalue, T, Args...>{
    typedef std::optional<T> NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, jobject> In;
    typedef std::conditional_t<forward, jobject, NativeType_out> Out;
    typedef result_of_t<decltype(jni_type_decider<T>::readJavaOptional)> ElementType;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
            return jni_type_decider<T>::newJavaOptional(env, _in.has_value(), qtjambi_optional<ElementType>::template value<NativeType_c, Args...>(_in, args...));
        }else{
            bool isPresent = false;
            auto value = jni_type_decider<T>::readJavaOptional(env, in, isPresent);
            if(isPresent){
                return qtjambi_optional<decltype(value)>::template create<T, Args...>(value, args...);
            }else{
                return std::nullopt;
            }
        }
    }
};
#endif

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      QPointer, is_pointer, is_const, is_reference, is_rvalue, T, Args...>{
    typedef QPointer<T> NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, jobject> In;
    typedef std::conditional_t<forward, jobject, NativeType_out> Out;

    static Out cast(In in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(forward){
            NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
            return qtjambi_cast_with_args<jobject>(_in.data(), std::forward<Args>(args)...);
        }else{
            T* object = qtjambi_cast_with_args<T*>(in, std::forward<Args>(args)...);
            NativeType ptr{object};
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(ptr), args...);
        }
    }
};

//template from any type's QWeakPointer<T> to jobject
template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      QWeakPointer, is_pointer, is_const, is_reference, is_rvalue,
                                      T, Args...>{
    typedef QWeakPointer<T> NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, jobject> In;
    typedef std::conditional_t<forward, jobject, NativeType_out> Out;

    static Out cast(In in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(forward){
            if(QSharedPointer<T> ptr = deref_ptr<is_pointer,NativeType_c>::deref(in).toStrongRef()){
                return qtjambi_cast_with_args<jobject>(ptr, std::forward<Args>(args)...);
            }else
                return nullptr;
        }else{
            QSharedPointer<T> pointer = qtjambi_cast_with_args<QSharedPointer<T>>(in, std::forward<Args>(args)...);
            NativeType ptr = pointer.toWeakRef();
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(ptr), args...);
        }
    }
};

//template from any type's QSharedPointer<T> to jobject
template<bool forward,
         typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      JniType,
                                      QSharedPointer, is_pointer, is_const, is_reference, is_rvalue,
                                      T, Args...>
    : decltype(find_qtjambi_smart_pointer_cast<forward, JniType, QSharedPointer, is_pointer, is_const, is_reference, is_rvalue, T, Args...>()){
};

#if defined(_MEMORY_) || defined(_LIBCPP_MEMORY) || defined(_GLIBCXX_MEMORY)
//template from any type's std::weak_ptr<T> to jobject

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      std::weak_ptr, is_pointer, is_const, is_reference, is_rvalue, T, Args...>{
    typedef std::weak_ptr<T> NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, jobject> In;
    typedef std::conditional_t<forward, jobject, NativeType_out> Out;

    static Out cast(In in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(forward){
            if(std::shared_ptr<T> ptr = std::shared_ptr<T>(deref_ptr<is_pointer,NativeType_c>::deref(in))){
                return qtjambi_cast_with_args<jobject>(ptr, std::forward<Args>(args)...);
            }else
                return nullptr;
        }else{
            std::shared_ptr<T> pointer = qtjambi_cast_with_args<std::shared_ptr<T>>(in, std::forward<Args>(args)...);
            NativeType ptr(pointer);
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(ptr), args...);
        }
    }
};

//template from any type's std::shared_ptr<T> to jobject
template<bool forward,
         typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      JniType,
                                      std::shared_ptr, is_pointer, is_const, is_reference, is_rvalue,
                                      T, Args...>
    : decltype(find_qtjambi_smart_pointer_cast<forward, JniType, std::shared_ptr, is_pointer, is_const, is_reference, is_rvalue, T, Args...>()){
};
#endif //defined(_MEMORY_)

#if defined(_FUNCTIONAL_) || defined(_FUNCTIONAL) || defined(_LIBCPP_FUNCTIONAL) || defined(_GLIBCXX_FUNCTIONAL)
//template from any std::function to jobject

template<bool forward, typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      JniType,
                                      std::function, is_pointer, is_const, is_reference, is_rvalue, T, Args...>{
    typedef std::function<T> NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::add_pointer_t<NativeType> NativeType_ptr;
    typedef std::conditional_t<forward, NativeType_in, JniType> In;
    typedef std::conditional_t<forward, JniType, NativeType_out> Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(!std::is_same_v<JniType, jobject>){
            Q_STATIC_ASSERT_X(false && !cast_var_args<Args...>::hasScope, "Cannot cast types");
        }else if constexpr(forward){
            if constexpr(is_pointer){
                if constexpr(cast_var_args<Args...>::hasScope)
                    return QtJambiAPI::convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), in, typeid(NativeType), cast_var_args<Args...>::nativeTypeName(args...));
                else
                    return QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, typeid(NativeType), cast_var_args<Args...>::nativeTypeName(args...));
            }else{
                return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &in, typeid(NativeType), cast_var_args<Args...>::nativeTypeName(args...));
            }
        }else{
            Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast to non-const std::function<R(Args...)>*");
            Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast to non-const std::function<R(Args...)>&");
            if constexpr(is_pointer){
                if(!in)
                    return nullptr;
            }
            NativeType result;
            if(!QtJambiAPI::convertJavaToNative(env, in, &result, typeid(NativeType), cast_var_args<Args...>::nativeTypeName(args...))){
                if constexpr(cast_var_args<Args...>::hasNativeTypeName){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), cast_var_args<Args...>::nativeTypeName(args...)) QTJAMBI_STACKTRACEINFO );
                }else{
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                }
            }
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(result), args...);
        }
    }
};

#endif // defined(_FUNCTIONAL_)

#if defined(_INITIALIZER_LIST_) || defined(_INITIALIZER_LIST) || defined(INITIALIZER_LIST) || defined(_LIBCPP_INITIALIZER_LIST) || defined(_GLIBCXX_INITIALIZER_LIST)

//template from any std::initializer_list to jobject
template<bool forward, typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename... Args>
struct qtjambi_jobject_initializer_list_cast;

template<bool forward, typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename... Args>
constexpr auto qtjambi_jobject_initializer_list_cast_decider(){
    QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/ArrayCast, is_complete_v<qtjambi_jobject_initializer_list_cast<forward, JniType, is_pointer, is_const, is_reference, is_rvalue, T, Args...>>);
    return qtjambi_jobject_initializer_list_cast<forward, JniType, is_pointer, is_const, is_reference, is_rvalue, T, Args...>{};
};

template<bool forward, typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      JniType,
                                      std::initializer_list, is_pointer, is_const, is_reference, is_rvalue, T, Args...>
    : decltype(qtjambi_jobject_initializer_list_cast_decider<forward, JniType, is_pointer, is_const, is_reference, is_rvalue, T, Args...>()) {
};

#endif //defined(_INITIALIZER_LIST_)

template<bool forward,
         template<typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_sequential_container_cast;

template<bool forward,
         template<typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
static constexpr auto find_qtjambi_jobject_sequential_container_cast() {
    QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/ContainerCast, is_complete_v< qtjambi_jobject_sequential_container_cast<forward, NativeType, is_pointer, is_const, is_reference, is_rvalue, T, Args...> >);
    return qtjambi_jobject_sequential_container_cast<forward, NativeType, is_pointer, is_const, is_reference, is_rvalue, T, Args...>{};
}

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward, jobject, QList, is_pointer, is_const, is_reference, is_rvalue, T, Args...>
    : decltype( find_qtjambi_jobject_sequential_container_cast<forward, QList, is_pointer, is_const, is_reference, is_rvalue, T, Args...>() ){
};

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward, jobject, QQueue, is_pointer, is_const, is_reference, is_rvalue, T, Args...>
    : decltype( find_qtjambi_jobject_sequential_container_cast<forward, QQueue, is_pointer, is_const, is_reference, is_rvalue, T, Args...>() ){
};

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward, jobject, QStack, is_pointer, is_const, is_reference, is_rvalue, T, Args...>
    : decltype( find_qtjambi_jobject_sequential_container_cast<forward, QStack, is_pointer, is_const, is_reference, is_rvalue, T, Args...>() ){
};

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward, jobject, QSet, is_pointer, is_const, is_reference, is_rvalue, T, Args...>
    : decltype( find_qtjambi_jobject_sequential_container_cast<forward, QSet, is_pointer, is_const, is_reference, is_rvalue, T, Args...>() ){
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_TEMPLATE1_H
