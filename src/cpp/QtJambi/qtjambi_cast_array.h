/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#ifndef QTJAMBI_CAST_ARRAY_H
#define QTJAMBI_CAST_ARRAY_H

#include <array>
#include "javaarrays.h"
#include "containeraccess_sequential.h"
#include "qtjambi_cast_util.h"
#include "qtjambiapi_array.h"
#include "qtjambiapi_string.h"

namespace QtJambiPrivate {

QT_WARNING_DISABLE_GCC("-Winit-list-lifetime")

template<typename NativeType, typename Output>
struct convert_jstring_to_chars;
template<typename NativeType, typename Output>
struct convert_jstring_to_qchars;

template<typename Out>
struct qtjambi_construct_from_argument{
    typedef Out create;
};

template<typename T>
struct qtjambi_construct_from_argument<std::initializer_list<T>>{
    static std::initializer_list<T> create(const QVector<T>& pointer){
        return QtJambiAPI::initializer_list<T>(pointer.data(), pointer.size());
    }
};

template<typename T>
struct ArrayFactory{
};

template<>
struct ArrayFactory<jintArray>{
    static inline auto converterFunction = QtJambiAPI::toJIntArray;
    template<bool persistent, typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jint), "T* cannot be cast to jint*");
        using type = std::conditional_t<persistent, std::conditional_t<isConst, PersistentJConstIntArrayPointer, PersistentJIntArrayPointer>, std::conditional_t<isConst, JConstIntArrayPointer, JIntArrayPointer>>;
    };
};

template<>
struct ArrayFactory<jbyteArray>{
    static inline auto converterFunction = QtJambiAPI::toJByteArray;
    template<bool persistent, typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jbyte), "T* cannot be cast to jbyte*");
        using type = std::conditional_t<persistent, std::conditional_t<isConst, PersistentJConstByteArrayPointer, PersistentJByteArrayPointer>, std::conditional_t<isConst, JConstByteArrayPointer, JByteArrayPointer>>;
    };
};

template<>
struct ArrayFactory<jshortArray>{
    static inline auto converterFunction = QtJambiAPI::toJShortArray;
    template<bool persistent, typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jshort), "T* cannot be cast to jshort*");
        using type = std::conditional_t<persistent, std::conditional_t<isConst, PersistentJConstShortArrayPointer, PersistentJShortArrayPointer>, std::conditional_t<isConst, JConstShortArrayPointer, JShortArrayPointer>>;
    };
};

template<>
struct ArrayFactory<jlongArray>{
    static inline auto converterFunction = QtJambiAPI::toJLongArray;
    template<bool persistent, typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jlong), "T* cannot be cast to jlong*");
        using type = std::conditional_t<persistent, std::conditional_t<isConst, PersistentJConstLongArrayPointer, PersistentJLongArrayPointer>, std::conditional_t<isConst, JConstLongArrayPointer, JLongArrayPointer>>;
    };
};

template<>
struct ArrayFactory<jcharArray>{
    static inline auto converterFunction = QtJambiAPI::toJCharArray;
    template<bool persistent, typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jchar), "T* cannot be cast to jchar*");
        using type = std::conditional_t<persistent, std::conditional_t<isConst, PersistentJConstCharArrayPointer, PersistentJCharArrayPointer>, std::conditional_t<isConst, JConstCharArrayPointer, JCharArrayPointer>>;
    };
};

template<>
struct ArrayFactory<jbooleanArray>{
    static inline auto converterFunction = QtJambiAPI::toJBooleanArray;
    template<bool persistent, typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jboolean) || sizeof(T)==sizeof(bool), "T* cannot be cast to jboolean*");
        using type = std::conditional_t<persistent, std::conditional_t<isConst, PersistentJConstBooleanArrayPointer, PersistentJBooleanArrayPointer>, std::conditional_t<isConst, JConstBooleanArrayPointer, JBooleanArrayPointer>>;
    };
};

template<>
struct ArrayFactory<jdoubleArray>{
    static inline auto converterFunction = QtJambiAPI::toJDoubleArray;
    template<bool persistent, typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jdouble), "T* cannot be cast to jdouble*");
        using type = std::conditional_t<persistent, std::conditional_t<isConst, PersistentJConstDoubleArrayPointer, PersistentJDoubleArrayPointer>, std::conditional_t<isConst, JConstDoubleArrayPointer, JDoubleArrayPointer>>;
    };
};

template<>
struct ArrayFactory<jfloatArray>{
    static inline auto converterFunction = QtJambiAPI::toJFloatArray;
    typedef jfloat ElementType;

    template<bool persistent, typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jfloat), "T* cannot be cast to jfloat*");
        using type = std::conditional_t<persistent, std::conditional_t<isConst, PersistentJConstFloatArrayPointer, PersistentJFloatArrayPointer>, std::conditional_t<isConst, JConstFloatArrayPointer, JFloatArrayPointer>>;
    };
};

template<typename JArray, bool persistent, typename T, bool is_const = std::is_const<T>::value>
using jni_native_to_java_array_converter_t = typename ArrayFactory<JArray>::template NativeFactory<persistent,T,is_const>::type;

template<typename NativeType, typename T, bool is_const, typename JniType, typename... Args>
struct qtjambi_collection_to_container_impl{

    static NativeType convert(JniType in, Args... args){
        if constexpr(is_complete_v<value_range_converter_from_java_buffer<is_const, JniType, NativeType, Args...>>){
            if(auto data = value_range_converter_from_java_buffer<is_const, JniType, NativeType, Args...>::JBufferPointer(in, args...)){
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope || !value_range_requires_scope_v<NativeType>, "Cannot cast without scope.");
                if constexpr(value_range_requires_scope_v<NativeType>)
                    cast_var_args<Args...>::scope(args...).addDeletion(data);
                return data->operator NativeType();
            }
        }
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(cast_var_args<Args...>::hasScope){
            QVector<T>* pointer{nullptr};
            if (!ContainerAPI::getAsQList<T>(env, in, pointer)) {
                pointer = new QVector<T>();
                cast_var_args<Args...>::scope(args...).addDeletion(pointer);
                jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, in);
                if(iterator){
                    while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                        jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                        pointer->append(qtjambi_cast_with_args<T>(element, std::forward<Args>(args)...));
                    }
                }
            }
            if(pointer)
                return qtjambi_construct_from_argument<NativeType>::create(*pointer);
        }else{
            QVector<T>* pointer{nullptr};
            if (ContainerAPI::getAsQList<T>(env, in, pointer)) {
                if(pointer)
                    return qtjambi_construct_from_argument<NativeType>::create(*pointer);
            }
            QVector<T> container;
            jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, in);
            if(iterator){
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                    container.append(qtjambi_cast_with_args<T>(element, std::forward<Args>(args)...));
                }
            }
            return qtjambi_construct_from_argument<NativeType>::create(container);
        }
        return NativeType();
    }
};

template<typename NativeType, typename T, bool is_const, typename JniType, bool isArrayType, typename... Args>
struct qtjambi_collection_to_container{
    static NativeType convert(JniType, Args...){
        return NativeType();
    }
};

template<typename NativeType, typename T, bool is_const, typename JniType, typename... Args>
struct qtjambi_collection_to_container<NativeType, T, is_const, JniType, false, Args...>
    : qtjambi_collection_to_container_impl<NativeType, T, is_const, JniType, Args...>{
    using qtjambi_collection_to_container_impl<NativeType, T, is_const, JniType, Args...>::convert;
};

template<typename TArray, typename T, bool is_const, typename JniType, typename NativeType, typename... Args>
struct value_range_converter_base{};

template<typename TArray, typename T, bool is_const, typename JniType, typename NativeType, typename... Args>
struct value_range_converter : value_range_converter_base<TArray, T, is_const, JniType, NativeType, Args...>{
    using value_range_converter_base<TArray, T, is_const, JniType, NativeType, Args...>::JArrayPointer;
    static NativeType toNativeContainer(JniType in, jsize expectedSize, arg_pointer_t<Args>... args){
        Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope || !value_range_requires_scope<NativeType>::value, "Cannot cast without scope.");
        if(in){
            if(auto data = value_range_converter_base<TArray, T, is_const, JniType, NativeType, Args...>::JArrayPointer(in, args...)){
                if constexpr(value_range_requires_scope<NativeType>::value)
                    cast_var_args<Args...>::scope(args...).addDeletion(data);
                if(expectedSize>=0 && data->size()>0 && data->size()<expectedSize){
                    JavaException::raiseIllegalArgumentException(cast_var_args<arg_pointer_t<Args>...>::env(args...), QStringLiteral("Cannot cast array of size %1. Expected size: %2").arg(QString::number(data->size()), QString::number(expectedSize)) QTJAMBI_STACKTRACEINFO );
                }
                return data->operator NativeType();
            }else{
                return qtjambi_collection_to_container<NativeType, T, is_const, JniType, is_jni_array_type_v<JniType>, Args...>::convert(in, arg_pointer<Args>::deref(args)...);
            }
        }
        return NativeType();
    }
};

template<typename TArray, typename T, bool is_const, typename JniType, typename NativeType, typename... Args>
struct value_range_converter_from_java_array{
    static auto JArrayPointer(JniType in, arg_pointer_t<Args>...args){
        typedef jni_native_to_java_array_converter_t<TArray,true,T,is_const> PersistentConverter;
        PersistentConverter * data = nullptr;
        auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
        if (PersistentConverter::isValidArray(env, in)) {
            data = new PersistentConverter(env, TArray(in));
        }
        return data;
    }

    static auto NativePointerArray(NativeType* container, arg_pointer_t<Args>...args){
        auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
        return new PointerArray<true, TArray, is_const,T>(env, container->begin(), jsize(container->size()));
    }
};

template<typename TArray, typename T, typename JniType, typename NativeType, typename... Args>
struct value_range_converter_base<TArray, T, true, JniType, NativeType, Args...>
    : value_range_converter_from_java_array<TArray, T, true, JniType, NativeType, Args...>{
    using value_range_converter_from_java_array<TArray, T, true, JniType, NativeType, Args...>::JArrayPointer;
    using value_range_converter_from_java_array<TArray, T, true, JniType, NativeType, Args...>::NativePointerArray;

    static TArray toJavaArray(const NativeType* container, arg_pointer_t<Args>...args){
        auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
        return ArrayFactory<TArray>::converterFunction(env, reinterpret_cast<const jni_array_element_type_t<TArray>*>(container->begin()), jsize(container->size()));
    }
};

template<typename TArray, typename T, typename JniType, typename NativeType, typename... Args>
struct value_range_converter_base<TArray, T, false, JniType, NativeType, Args...>
    : value_range_converter_from_java_array<TArray, T, false, JniType, NativeType, Args...>{
    using value_range_converter_from_java_array<TArray, T, false, JniType, NativeType, Args...>::JArrayPointer;
    using value_range_converter_from_java_array<TArray, T, false, JniType, NativeType, Args...>::NativePointerArray;

    static TArray toJavaArray(const NativeType* container, arg_pointer_t<Args>... args){
        if(!container)
            return nullptr;
        if constexpr(cast_var_args<Args...>::hasScope){
            auto converter = value_range_converter_from_java_array<TArray, T, false, JniType, const NativeType, Args...>::NativePointerArray(container, args...);
            cast_var_args<Args...>::scope(args...).addDeletion(converter);
            auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
            return converter->array(env);
        }
        return value_range_converter<TArray, T, true, JniType, NativeType, Args...>::toJavaArray(container, args...);
    }
};

template<typename T, typename JniType, typename NativeType, typename... Args>
struct value_range_converter_base<jobjectArray, T, true, JniType, NativeType, Args...>{

    static jobjectArray toJavaArray(const NativeType* container, arg_pointer_t<Args>... args){
        if(!container)
            return nullptr;
        auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
        return QtJambiAPI::toJObjectArray(env, qtjambi_type<T>::id(), container, jsize(container->size()),
                                          [args...](JNIEnv * env,const void* in, jsize index)->jobject{
                                              const auto& element = reinterpret_cast<const NativeType*>(in)->begin()[index];
                                              return qtjambi_cast_with_args<jobject>(element, arg_pointer<Args>::deref(args, env)...);
                                          }
                                          );
    }

    static auto NativePointerArray(const NativeType* container, arg_pointer_t<Args>... args){
        auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
        return new ObjectPointerArray<true,true,T>(env, container->begin(), jsize(container->size()),
                                                        [args...](JNIEnv * env,const T& in)->jobject{
                                                            return qtjambi_cast_with_args<jobject>(in, arg_pointer<Args>::deref(args, env)...);
                                                        });
    }

    static auto JArrayPointer(JniType in, arg_pointer_t<Args>... args){
        auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
        PersistentJConstObjectArrayPointer<T>* converter = nullptr;
        if (QtJambiAPI::isValidArray(env, in, qtjambi_type<T>::id())) {
            converter = new PersistentJConstObjectArrayPointer<T>(env, jobjectArray(in), [args...](T& d,JNIEnv * env, jobject obj){
                d = qtjambi_cast_with_args<T>(obj, arg_pointer<Args>::deref(args, env)...);
            });
        }
        return converter;
    }
};

template<typename T, typename JniType, typename NativeType, typename... Args>
struct value_range_converter_base<jobjectArray, T, false, JniType, NativeType, Args...>{

    static jobjectArray toJavaArray(const NativeType* container, arg_pointer_t<Args>... args){
        if(!container)
            return nullptr;
        if constexpr(cast_var_args<Args...>::hasScope){
            auto converter = NativePointerArray(container, args...);
            cast_var_args<Args...>::scope(args...).addDeletion(converter);
            auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
            return converter->array(env);
        }
        return value_range_converter<jobjectArray, T, true, JniType, NativeType, Args...>::toJavaArray(container, args...);
    }

    static auto NativePointerArray(NativeType* container, arg_pointer_t<Args>... args){
        auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
        return new ObjectPointerArray<true,false,T>(env, container->begin(), jsize(container->size()),
                                                   [args...](JNIEnv * env,const T& in)->jobject{
                                                       return qtjambi_cast_with_args<jobject>(in, arg_pointer<Args>::deref(args, env)...);
                                                   },
                                                   [args...](T& d,JNIEnv * env, jobject obj){
                                                       d = qtjambi_cast_with_args<T>(obj, arg_pointer<Args>::deref(args, env)...);
                                                   });
    }

    static auto JArrayPointer(JniType in, arg_pointer_t<Args>... args){
        auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
        PersistentJObjectArrayPointer<T>* converter = nullptr;
        if (QtJambiAPI::isValidArray(env, in, qtjambi_type<T>::id())) {
            converter = new PersistentJObjectArrayPointer<T>(env, jobjectArray(in), [args...](T& d,JNIEnv * env, jobject obj){
                d = qtjambi_cast_with_args<T>(obj, arg_pointer<Args>::deref(args, env)...);
            },
                                                        [args...](JNIEnv * env,const T& in)->jobject{
                                                            return qtjambi_cast_with_args<jobject>(in, arg_pointer<Args>::deref(args, env)...);
                                                        });
        }
        return converter;
    }
};

#if defined(_INITIALIZER_LIST_) || defined(_INITIALIZER_LIST) || defined(INITIALIZER_LIST) || defined(_LIBCPP_INITIALIZER_LIST) || defined(_GLIBCXX_INITIALIZER_LIST)

//template from any std::initializer_list to jobject

template<bool forward, typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename... Args>
struct qtjambi_jobject_initializer_list_cast{
    typedef std::initializer_list<T> NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, JniType> In;
    typedef std::conditional_t<forward, JniType, NativeType_out> Out;
    typedef std::remove_cv_t<T> T_noconst;
    typedef typename jni_type_decider<T_noconst>::JArrayType TArray;
    typedef jni_array_element_type_t<TArray> ElementType;

    static Out cast(In in, Args... args){
        if constexpr(forward){
            return value_range_converter<TArray, T, true, JniType, NativeType_c, Args...>::toJavaArray(ref_ptr<is_pointer, NativeType_c>::ref(in), arg_pointer<Args>::ref(args)...);
        }else{
            Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to std::initializer_list<T> &");
            Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to std::initializer_list<T> *");
            return value_range_converter<TArray, T, true, JniType, NativeType, Args...>::toNativeContainer(in, -1, arg_pointer<Args>::ref(args)...);
        }
    }
};

#endif //defined(_INITIALIZER_LIST_)

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)

//template from any span to jobject

template<typename Container>
class Result{
private:
    Container container;
    bool success = false;
public:
    template<typename V>
    Result& operator=(V& v) {
        container = v;
        success = true;
        return *this;
    }
    Container& operator*(){return container;}
    operator bool(){return success;}
};

template<bool forward, typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, std::size_t E, template<typename,std::size_t> typename Span, bool t_is_const, typename... Args>
struct qtjambi_jobject_span_cast{
    typedef std::conditional_t<t_is_const, std::add_const_t<T>, T> Tconst;
    typedef Span<Tconst,E> NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::conditional_t<std::is_pointer_v<T>, T, std::add_const_t<T>> T_const;
    typedef jni_array_type_t<T> TArray;
    typedef jni_array_element_type_t<TArray> ElementType;
    typedef std::conditional_t<forward, NativeType_in, JniType> In;
    typedef std::conditional_t<forward, JniType, NativeType_out> Out;

    static Out cast(In in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(forward){
            if constexpr(is_jni_array_type_v<JniType>){
                return value_range_converter<TArray, T, t_is_const, JniType, NativeType_c, Args...>::toJavaArray(ref_ptr<is_pointer, NativeType_c>::ref(in), arg_pointer<Args>::ref(args)...);
            }else{
                auto env = cast_var_args<Args...>::env(args...);
                if constexpr(t_is_const){
                    const void * spanPtr;
                    if constexpr(is_pointer){
                        if(!in)
                            return nullptr;
                        spanPtr = in;
                    }else
                        spanPtr = &in;
                    return QtJambiAPI::convertQSpanFromQListToJavaObject(env,
                                                                         spanPtr,
                                                                         [](const void* ptr) -> void* {
                                                                             const Span<Tconst,E>& _span = *reinterpret_cast<const Span<Tconst,E>*>(ptr);
                                                                             return new QList<T>(_span.begin(), _span.end());
                                                                         },
                                                                         &QtJambiAPI::deletePointer<QList<T>>,
                                                                         QListAccess<T>::newInstance(),
                                                                         t_is_const);
                }else{
                    QtJambiNativeID owner{InvalidNativeID};
                    if constexpr(cast_var_args<Args...>::hasScope){
                        owner = cast_var_args<Args...>::scope(args...).relatedNativeID();
                    }
                    if constexpr(is_pointer)
                        return in ? QtJambiAPI::convertQSpanToJavaObject(env,
                                                                         owner,
                                                                         QSpanAccess<Tconst,E>::newInstance(),
                                                                         in->begin(),
                                                                         jlong(in->size())
                                                                         ) : nullptr;
                    else
                        return QtJambiAPI::convertQSpanToJavaObject(env,
                                                                    owner,
                                                                    QSpanAccess<Tconst,E>::newInstance(),
                                                                    in.begin(),
                                                                    jlong(in.size())
                                                                    );
                }
            }
        }else{
            if constexpr(is_pointer){
                if(!in)
                    return nullptr;
            }
            if constexpr(is_jni_array_type_v<JniType>){
                typedef Span<Tconst,q20::dynamic_extent> UnlimitedSpan;
                if constexpr(E==q20::dynamic_extent){
                    UnlimitedSpan span = value_range_converter<TArray, T, t_is_const, JniType, UnlimitedSpan, Args...>::toNativeContainer(in, -1, arg_pointer<Args>::ref(args)...);
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(span), args...);
                }else{
                    UnlimitedSpan span = value_range_converter<TArray, T, t_is_const, JniType, UnlimitedSpan, Args...>::toNativeContainer(in, jsize(E), arg_pointer<Args>::ref(args)...);
                    if constexpr(is_pointer){
                        Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast span pointer without scope.");
                        NativeType* result = new NativeType(span.template first<E>());
                        cast_var_args<Args...>::scope(args...).addDeletion(result);
                        return result;
                    }else if constexpr(is_reference){
                        Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast span reference without scope.");
                        NativeType* result = new NativeType(span.template first<E>());
                        cast_var_args<Args...>::scope(args...).addDeletion(result);
                        return *result;
                    }else{
                        return NativeType(span.template first<E>());
                    }
                }
            }else{
                auto env = cast_var_args<Args...>::env(args...);
                if constexpr(E==q20::dynamic_extent){
                    NativeType result;
                    if(QtJambiAPI::isQSpanObject(env, in)){
                        QPair<void*,jlong> data = QtJambiAPI::fromQSpanObject(env, in, t_is_const, QMetaType::fromType<T>());
                        if(data.first && data.second>0){
                            if constexpr (cast_var_args<Args...>::hasScope && !t_is_const){
                                if constexpr (is_copy_constructible_v<decltype(env)>){
                                    cast_var_args<Args...>::scope(args...).addFinalAction([env, in](){QtJambiAPI::commitQSpanObject(env, in);});
                                }else{
#if defined(QTJAMBI_JOBJECTWRAPPER_H)
                                    cast_var_args<Args...>::scope(args...).addFinalAction([in = JObjectWrapper(env, in)](){
                                        JniEnvironment env{500};
                                        QtJambiAPI::commitQSpanObject(env, in.object(env));
                                    });
#else
                                    cast_var_args<Args...>::scope(args...).addFinalAction([in](){
                                        JniEnvironment env{500};
                                        QtJambiAPI::commitQSpanObject(env, in);
                                    });
#endif
                                }
                            }
                            result = NativeType(reinterpret_cast<typename NativeType::iterator>(data.first), qsizetype(data.second));
                        }
                    }else if(in){
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(QtJambiAPI::getObjectClassName(env, in), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    }
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(result), args...);
                }else{
                    if(QtJambiAPI::isQSpanObject(env, in)){
                        QPair<void*,jlong> data = QtJambiAPI::fromQSpanObject(env, in, t_is_const, QMetaType::fromType<T>());
                        if(data.first && data.second>0){
                            if constexpr (cast_var_args<Args...>::hasScope && !t_is_const){
                                if constexpr (is_copy_constructible_v<decltype(env)>){
                                    cast_var_args<Args...>::scope(args...).addFinalAction([env, in](){QtJambiAPI::commitQSpanObject(env, in);});
                                }else{
#if defined(QTJAMBI_JOBJECTWRAPPER_H)
                                    cast_var_args<Args...>::scope(args...).addFinalAction([in = JObjectWrapper(env, in)](){
                                        JniEnvironment env{500};
                                        QtJambiAPI::commitQSpanObject(env, in.object(env));
                                    });
#else
                                    cast_var_args<Args...>::scope(args...).addFinalAction([in](){
                                        JniEnvironment env{500};
                                        QtJambiAPI::commitQSpanObject(env, in);
                                    });
#endif
                                }
                            }
                            if(data.second<E){
                                JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast span of size %1 to %2").arg(QString::number(data.second), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                            }
                            if constexpr(is_pointer){
                                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast span pointer without scope.");
                                NativeType* result = new NativeType(reinterpret_cast<typename NativeType::iterator>(data.first), E);
                                cast_var_args<Args...>::scope(args...).addDeletion(result);
                                return result;
                            }else if constexpr(is_reference){
                                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast span reference without scope.");
                                NativeType* result = new NativeType(reinterpret_cast<typename NativeType::iterator>(data.first), E);
                                cast_var_args<Args...>::scope(args...).addDeletion(result);
                                return *result;
                            }else{
                                return NativeType(reinterpret_cast<typename NativeType::iterator>(data.first), E);
                            }
                        }else{
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast span of size 0 to %1").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                }
            }
        }
    }
};

#endif // QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)

template<typename JArray,
         typename T, bool t_is_const,
         typename I, typename... Args>
struct qtjambi_native_to_jarray_cast;

template<typename O,
         bool is_const,
         typename JArray, typename I, bool fixSize, typename... Args>
struct qtjambi_jarray_to_native_cast;

template<typename Out,
         typename O, size_t size,
         bool is_const, bool is_reference,
         typename JArray, typename... Args>
struct qtjambi_jarray_to_native_array_cast;

template<typename O,
         typename T,
         typename I, bool fixSize, typename... Args>
static constexpr auto qtjambi_array_cast_impl() {
    if constexpr(is_jni_array_type_v<O> || std::is_same_v<O, jobject> || std::is_same_v<O, jcoreobject>){
        if constexpr(std::is_same_v<I,void>){
            Q_STATIC_ASSERT_X(std::is_array_v<std::remove_reference_t<T>>, "Cannot cast array pointer without size argument");
            return qtjambi_native_to_jarray_cast<O, T, false, I, Args...>{};
        }else{
            if constexpr(std::is_array_v<std::remove_reference_t<T>>){
                return qtjambi_native_to_jarray_cast<O, std::remove_const_t<std::remove_pointer_t<std::decay_t<T>>>, std::is_const_v<std::remove_pointer_t<std::decay_t<T>>>, I, Args...>{};
            }else{
                return qtjambi_native_to_jarray_cast<O, std::remove_const_t<std::remove_pointer_t<T>>, std::is_const_v<std::remove_pointer_t<T>>, I, Args...>{};
            }
        }
    }else if constexpr(std::is_array_v<std::remove_reference_t<O>> && std::extent_v<std::remove_reference_t<O>> > 0){
        Q_STATIC_ASSERT_X((is_jni_array_type_v<std::remove_reference_t<T>> || std::is_same_v<std::remove_reference_t<T>, jobject> || std::is_same_v<std::remove_reference_t<T>, jstring>), "Cannot cast types");
        return qtjambi_jarray_to_native_array_cast<std::conditional_t<std::is_array_v<O>, std::decay_t<O>, O>,
                                                   std::remove_const_t<std::remove_extent_t<std::remove_reference_t<O>>>,
                                                   std::extent_v<std::remove_reference_t<O>>,
                                                   std::is_const_v<std::remove_extent_t<std::remove_reference_t<O>>>,
                                                   std::is_reference_v<O>,
                                                   T, Args...>{};
    }else{
        Q_STATIC_ASSERT_X((is_jni_array_type_v<std::remove_reference_t<T>> || std::is_same_v<std::remove_reference_t<T>, jobject> || std::is_same_v<std::remove_reference_t<T>, jstring>), "Cannot cast types");
        if constexpr(std::is_array_v<std::remove_reference_t<O>>){
            return qtjambi_jarray_to_native_cast<std::remove_const_t<std::remove_pointer_t<std::decay_t<O>>>, std::is_const_v<std::remove_pointer_t<std::decay_t<O>>>, std::remove_reference_t<T>, I, fixSize, Args...>{};
        }else{
            return qtjambi_jarray_to_native_cast<std::remove_const_t<std::remove_pointer_t<O>>, std::is_const_v<std::remove_pointer_t<O>>, std::remove_reference_t<T>, I, fixSize, Args...>{};
        }
    }
}

template<typename O, typename T, typename I, bool fixSize, typename... Args>
struct qtjambi_array_cast : decltype(qtjambi_array_cast_impl<O, T, I, fixSize, Args...>()) {
};

template<typename JArray, typename T, bool t_is_const, typename I, typename... Args>
struct qtjambi_native_to_jarray_cast{
    typedef std::conditional_t<t_is_const, std::add_const_t<T>, T> T_in;

    static JArray cast(T_in* in, I size, Args...args){
        Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
        if constexpr(std::is_same_v<JArray,jobjectArray>){
            return castImpl(in, jsize(size), arg_pointer<Args>::ref(args)...);
        }else if constexpr(std::is_arithmetic_v<T>
                      || std::is_same_v<T,QChar>
                      || std::is_same_v<T,QLatin1Char>
                      || std::is_same_v<T,std::byte>){
            if constexpr(std::is_same_v<JArray,jobject>){
                typedef jni_array_type_t<T> ArrayType;
                if constexpr(std::is_same_v<ArrayType,jobjectArray>){
                    return castImpl(in, jsize(size), arg_pointer<Args>::ref(args)...);
                }else{
                    typedef PointerArray<true, ArrayType, t_is_const> PersistentConverter;
                    typedef PointerArray<false, ArrayType, t_is_const> Converter;
                    auto env = cast_var_args<Args...>::env(args...);
                    if constexpr(cast_var_args<Args...>::hasScope){
                        PersistentConverter* converter = new PersistentConverter(env, in, jsize(size));
                        cast_var_args<Args...>::scope(args...).addDeletion(converter);
                        return converter->array(env);
                    }else{
                        Converter converter(env, in, jsize(size));
                        return converter.array();
                    }
                }
            }else{
                auto env = cast_var_args<Args...>::env(args...);
                using O = jni_array_element_type_t<std::remove_reference_t<JArray>>;
                using T_ = std::conditional_t<sizeof(O)==sizeof(T),O,T>;
                if constexpr(cast_var_args<Args...>::hasScope){
                    typedef PointerArray<true, std::remove_reference_t<JArray>, t_is_const, T_> PersistentConverter;
                    PersistentConverter* converter = new PersistentConverter(env, in, jsize(size));
                    cast_var_args<Args...>::scope(args...).addDeletion(converter);
                    return converter->array(env);
                }else{
                    PointerArray<false, std::remove_reference_t<JArray>, t_is_const, T_> converter(env, in, jsize(size));
                    return converter.array();
                }
            }
        }else{
            Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
            Q_STATIC_ASSERT_X(false && !t_is_const, "Cannot cast types");
        }
    }

private:
    static JArray castImpl(T_in* in, jsize size, arg_pointer_t<Args>... args){
        auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
        if constexpr(t_is_const){
            if constexpr(cast_var_args<arg_pointer_t<Args>...>::hasScope){
                ObjectPointerArray<true,true,T>* converter = new ObjectPointerArray<true,true,T>(env, in, size,
                                                                                                       [args...](JNIEnv * env,const T& in)->jobject{
                                                                                                           return qtjambi_cast_with_args<jobject>(in, arg_pointer<Args>::deref(args, env)...);
                                                                                                       });
                cast_var_args<arg_pointer_t<Args>...>::scope(args...).addDeletion(converter);
                return converter->array(env);
            }else{
                ObjectPointerArray<false,true,T> converter(env, in, size,
                                                 [args...](JNIEnv * env,const T& in)->jobject{
                                                     return qtjambi_cast_with_args<jobject>(in, arg_pointer<Args>::deref(args, env)...);
                                                 });
                return converter.array();
            }
        }else{
            if constexpr(cast_var_args<arg_pointer_t<Args>...>::hasScope){
                ObjectPointerArray<true,false,T>* converter = new ObjectPointerArray<true,false,T>(env, in, size,
                                                                                             [args...](JNIEnv * env,const T& in)->jobject{
                                                                                                 return qtjambi_cast_with_args<jobject>(in, arg_pointer<Args>::deref(args, env)...);
                                                                                             },
                                                                                             [args...](T& out,JNIEnv * env,jobject in){
                                                                                                 out = qtjambi_cast_with_args<T>(in, arg_pointer<Args>::deref(args, env)...);
                                                                                             });
                cast_var_args<arg_pointer_t<Args>...>::scope(args...).addDeletion(converter);
                return converter->array(env);
            }else{
                ObjectPointerArray<false,false,T> converter(env, in, size,
                                            [args...](JNIEnv * env,const T& in)->jobject{
                                                return qtjambi_cast_with_args<jobject>(in, arg_pointer<Args>::deref(args, env)...);
                                            },
                                            [args...](T& out,JNIEnv * env,jobject in){
                                                out = qtjambi_cast_with_args<T>(in, arg_pointer<Args>::deref(args, env)...);
                                            });
                return converter.array();
            }
        }
    }
};

template<typename JArray, typename T, typename... Args>
struct qtjambi_native_to_jarray_cast<JArray, T, false, void, Args...>{
    static constexpr JArray cast(T& in, Args...args){
        return qtjambi_native_to_jarray_cast<JArray, std::remove_pointer_t<std::decay_t<std::remove_reference_t<T>>>, std::is_const_v<std::remove_reference_t<T>>, jsize, Args...>::cast(in, std::extent_v<std::remove_reference_t<T>>, args...);
    }
};

template<typename O, bool is_const, typename JArray, typename Int, bool fixSize, typename... Args>
struct qtjambi_jarray_to_native_cast{
    typedef std::conditional_t<is_const, std::add_const_t<O>, O> Out;
    typedef std::conditional_t<fixSize, Int, Int&&> I;

    static Out* cast(JArray in, I size, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if(env->IsSameObject(in, nullptr)){
            return reinterpret_cast<Out*>(0);
        }
        if constexpr(std::is_same_v<std::remove_reference_t<JArray>,jobjectArray>
                      || std::is_same_v<std::remove_reference_t<JArray>,jstring>
                      || std::is_same_v<std::remove_reference_t<JArray>,jobject>){
            return castImpl(in, std::forward<I>(size), arg_pointer<Args>::ref(args)...);
        }else if constexpr(std::is_arithmetic_v<O>
                      || std::is_same_v<O,QChar>
                      || std::is_same_v<O,QLatin1Char>
                      || std::is_same_v<O,std::byte>){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast array without scope.");
            using T = jni_array_element_type_t<std::remove_reference_t<JArray>>;
            if constexpr(sizeof(O)!=sizeof(T)){
                if constexpr(is_const){
                    typedef jni_native_to_java_array_converter_t<std::remove_reference_t<JArray>,false,T,is_const> Converter;
                    auto nativeArray = std::make_unique<O[]>(size);
                    O* result = nativeArray.get();
                    Converter converter(env, in);
                    for(size_t i=0; i<size; ++i){
                        result[i] = O(converter.pointer()[i]);
                    }
                    cast_var_args<Args...>::scope(args...).addDeletion(std::move(nativeArray));
                    return reinterpret_cast<Out*>(result);
                }else{
                    using PersistentConverter = jni_native_to_java_array_converter_t<std::remove_reference_t<JArray>,true,T,is_const>;
                    auto nativeArray = std::make_unique<O[]>(size);
                    O* result = nativeArray.get();
                    auto converter = std::make_unique<PersistentConverter>(env, in);
                    for(Int i=0; i<size; ++i){
                        result[i] = O(converter->pointer()[i]);
                    }
                    cast_var_args<Args...>::scope(args...).addFinalAction([converter = std::move(converter), nativeArray = std::move(nativeArray), size]{
                        for(Int i=0; i<size; ++i){
                            converter->pointer()[i] = T(nativeArray[i]);
                        }
                    });
                    return reinterpret_cast<Out*>(result);
                }
            }else{
                using PersistentConverter = jni_native_to_java_array_converter_t<std::remove_reference_t<JArray>,true,O,is_const>;
                PersistentConverter* converter = new PersistentConverter(env, in);
                cast_var_args<Args...>::scope(args...).addDeletion(converter);
                if constexpr(fixSize){
                    if(size>=0 && converter->size()>0 && Int(converter->size())<size){
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast array of size %1. Expected size: %2").arg(QString::number(converter->size()), QString::number(size)) QTJAMBI_STACKTRACEINFO );
                    }
                }else{
                    size = std::remove_reference_t<Int>(converter->size());
                }
                return reinterpret_cast<Out*>(converter->pointer());
            }
        }else{
            Q_STATIC_ASSERT_X(false && !is_const, "Cannot cast types");
        }
    }

private:
    static Out* castImpl(JArray in, I size, arg_pointer_t<Args>... args){
        if constexpr(std::is_same_v<std::remove_reference_t<JArray>,jobject>){
            auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
            if constexpr(std::is_arithmetic_v<O>
                          || std::is_same_v<O,QChar>
                          || std::is_same_v<O,QLatin1Char>
                          || std::is_same_v<O,std::byte>){
                typedef typename jni_type_decider<O>::JArrayType ArrayType;
                if constexpr(!std::is_same_v<ArrayType,jobjectArray>){
                    using Converter = jni_native_to_java_array_converter_t<std::remove_reference_t<JArray>,false,O,is_const>;
                    if(Converter::isValidArray(env, in)){
                        return cast(ArrayType(in), std::move(size), arg_pointer<Args>::ref(args)...);
                    }
                }
            }
            if(QtJambiAPI::isValidArray(env, in, qtjambi_type<O>::id())) {
                return castImpl(jobjectArray(in), std::move(size), arg_pointer<Args>::ref(args)...);
            }else{
                return nullptr;
            }
        }else if constexpr(std::is_same_v<std::remove_reference_t<JArray>,jstring>){
            if constexpr(std::is_same_v<O,char>
                          || std::is_same_v<O,QLatin1Char>
                          || std::is_same_v<O,std::byte>){
                QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/StringAPI, is_complete_v<convert_jstring_to_chars<O,const O*>>);
                auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
                const O* array = convert_jstring_to_chars<O,const O*>::template convert<I,fixSize>(env, cast_var_args<arg_pointer_t<Args>...>::scope(args...), size, in);
                return reinterpret_cast<Out*>(array);
            }else if constexpr(std::is_same_v<O,char16_t>
                                 || std::is_same_v<O,wchar_t>
                                 || std::is_same_v<O,QChar>){
                QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/StringAPI, is_complete_v<convert_jstring_to_qchars<O,const O*>>);
                auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
                const O* array = convert_jstring_to_qchars<O,const O*>::template convert<I,fixSize>(env, cast_var_args<arg_pointer_t<Args>...>::scope(args...), size, in);
                return reinterpret_cast<Out*>(array);
            }else{
                Q_STATIC_ASSERT_X(false && !is_const, "Cannot cast types");
                return {};
            }
        }else{
            Q_STATIC_ASSERT_X(cast_var_args<arg_pointer_t<Args>...>::hasScope, "Cannot cast array without scope.");
            auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
            if constexpr(is_const){
                PersistentJConstObjectArrayPointer<O>* converter = new PersistentJConstObjectArrayPointer<O>(env, in, [args...](O& out,JNIEnv * env,jobject in){
                    out = qtjambi_cast_with_args<O>(in, arg_pointer<Args>::deref(args, env)...);
                });
                cast_var_args<arg_pointer_t<Args>...>::scope(args...).addDeletion(converter);
                if constexpr(fixSize){
                    if(size>=0 && converter->size()>0 && Int(converter->size())<size){
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast array of size %1. Expected size: %2").arg(QString::number(converter->size()), QString::number(size)) QTJAMBI_STACKTRACEINFO );
                    }
                }else{
                    size = std::remove_reference_t<Int>(converter->size());
                }
                return converter->pointer();
            }else{
                PersistentJObjectArrayPointer<O>* converter = new PersistentJObjectArrayPointer<O>(env, in, [args...](O& out,JNIEnv * env,jobject in){
                    out = qtjambi_cast_with_args<O>(in, arg_pointer<Args>::deref(args, env)...);
                }, [args...](JNIEnv * env,const O& in)->jobject{
                    return qtjambi_cast_with_args<jobject>(in, arg_pointer<Args>::deref(args, env)...);
                });
                cast_var_args<arg_pointer_t<Args>...>::scope(args...).addDeletion(converter);
                if constexpr(fixSize){
                    if(size>=0 && converter->size()>0 && Int(converter->size())<size){
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast array of size %1. Expected size: %2").arg(QString::number(converter->size()), QString::number(size)) QTJAMBI_STACKTRACEINFO );
                    }
                }else{
                    size = std::remove_reference_t<Int>(converter->size());
                }
                return converter->pointer();
            }
        }
    }
};

template<typename O, bool is_const, typename JArray, typename... Args>
struct qtjambi_jarray_to_native_cast<O,is_const,JArray,void,true,Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<O>, O> Out;

    static Out* cast(JArray in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if(env->IsSameObject(in, nullptr)){
            return reinterpret_cast<Out*>(0);
        }
        if constexpr(std::is_same_v<std::remove_reference_t<JArray>,jobjectArray>
                      || std::is_same_v<std::remove_reference_t<JArray>,jstring>
                      || std::is_same_v<std::remove_reference_t<JArray>,jobject>){
            return castImpl(in, arg_pointer<Args>::ref(args)...);
        }else if constexpr(std::is_arithmetic_v<O>
                             || std::is_same_v<O,QChar>
                             || std::is_same_v<O,QLatin1Char>
                             || std::is_same_v<O,std::byte>){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast array without scope.");
            using PersistentConverter = jni_native_to_java_array_converter_t<std::remove_reference_t<JArray>,true,O,is_const>;
            PersistentConverter* converter = new PersistentConverter(env, in);
            cast_var_args<Args...>::scope(args...).addDeletion(converter);
            return reinterpret_cast<Out*>(converter->pointer());
        }else{
            Q_STATIC_ASSERT_X(false && !is_const, "Cannot cast types");
        }
    }

private:
    static Out* castImpl(JArray in, arg_pointer_t<Args>... args){
        if constexpr(std::is_same_v<std::remove_reference_t<JArray>,jobject>){
            auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
            if constexpr(std::is_arithmetic_v<O>
                          || std::is_same_v<O,QChar>
                          || std::is_same_v<O,QLatin1Char>
                          || std::is_same_v<O,std::byte>){
                typedef typename jni_type_decider<O>::JArrayType ArrayType;
                if constexpr(!std::is_same_v<ArrayType,jobjectArray>){
                    using Converter = jni_native_to_java_array_converter_t<std::remove_reference_t<JArray>,false,O,is_const>;
                    if(Converter::isValidArray(env, in)){
                        return cast(ArrayType(in), arg_pointer<Args>::ref(args)...);
                    }
                }
            }
            if(QtJambiAPI::isValidArray(env, in, qtjambi_type<O>::id())) {
                return castImpl(jobjectArray(in), arg_pointer<Args>::ref(args)...);
            }else{
                return nullptr;
            }
        }else if constexpr(std::is_same_v<std::remove_reference_t<JArray>,jstring>){
            if constexpr(std::is_same_v<O,char>
                          || std::is_same_v<O,QLatin1Char>
                          || std::is_same_v<O,std::byte>){
                QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/StringAPI, is_complete_v<convert_jstring_to_chars<O,const O*>>);
                auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
                const O* array = convert_jstring_to_chars<O,const O*>::convert(env, cast_var_args<arg_pointer_t<Args>...>::scope(args...), in);
                return reinterpret_cast<Out*>(array);
            }else if constexpr(std::is_same_v<O,char16_t>
                                 || std::is_same_v<O,wchar_t>
                                 || std::is_same_v<O,QChar>){
                QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/StringAPI, is_complete_v<convert_jstring_to_qchars<O,const O*>>);
                auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
                const O* array = convert_jstring_to_qchars<O,const O*>::convert(env, cast_var_args<arg_pointer_t<Args>...>::scope(args...), in);
                return reinterpret_cast<Out*>(array);
            }else{
                Q_STATIC_ASSERT_X(false && !is_const, "Cannot cast types");
                return {};
            }
        }else{
            Q_STATIC_ASSERT_X(cast_var_args<arg_pointer_t<Args>...>::hasScope, "Cannot cast array without scope.");
            auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
            if constexpr(is_const){
                PersistentJConstObjectArrayPointer<O>* converter = new PersistentJConstObjectArrayPointer<O>(env, in, [args...](O& out,JNIEnv * env,jobject in){
                    out = qtjambi_cast_with_args<O>(in, arg_pointer<Args>::deref(args, env)...);
                });
                cast_var_args<arg_pointer_t<Args>...>::scope(args...).addDeletion(converter);
                return converter->pointer();
            }else{
                PersistentJObjectArrayPointer<O>* converter = new PersistentJObjectArrayPointer<O>(env, in, [args...](O& out,JNIEnv * env,jobject in){
                    out = qtjambi_cast_with_args<O>(in, arg_pointer<Args>::deref(args, env)...);
                }, [args...](JNIEnv * env,const O& in)->jobject{
                    return qtjambi_cast_with_args<jobject>(in, arg_pointer<Args>::deref(args, env)...);
                });
                cast_var_args<arg_pointer_t<Args>...>::scope(args...).addDeletion(converter);
                return converter->pointer();
            }
        }
    }
};

template<typename Out,
         typename O, size_t size,
         bool is_const, bool is_reference,
         typename JArray, typename... Args>
struct qtjambi_jarray_to_native_array_cast{
    typedef std::conditional_t<is_const, std::add_const_t<O>, O> Element;

    static Out cast(JArray in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if(env->IsSameObject(in, nullptr)){
            if constexpr(std::is_array_v<std::remove_reference_t<Out>>){
                JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast null to native array reference of size %1").arg(QString::number(size)) QTJAMBI_STACKTRACEINFO );
            }else{
                return nullptr;
            }
        }
        if constexpr(std::is_same_v<std::remove_reference_t<JArray>,jobjectArray>
                      || std::is_same_v<std::remove_reference_t<JArray>,jstring>
                      || std::is_same_v<std::remove_reference_t<JArray>,jobject>){
            return castImpl(in, arg_pointer<Args>::ref(args)...);
        }else if constexpr(std::is_arithmetic_v<O>
                             || std::is_same_v<O,QChar>
                             || std::is_same_v<O,QLatin1Char>
                             || std::is_same_v<O,std::byte>){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast array without scope.");
            using T = jni_array_element_type_t<std::remove_reference_t<JArray>>;
            if constexpr(sizeof(O)!=sizeof(T)){
                if constexpr(is_const){
                    using Converter = jni_native_to_java_array_converter_t<std::remove_reference_t<JArray>,false,T,is_const>;
                    auto nativeArray = std::make_unique<O[]>(size);
                    O* result = nativeArray.get();
                    Converter converter(env, in);
                    for(size_t i=0; i<size; ++i){
                        result[i] = O(converter.pointer()[i]);
                    }
                    cast_var_args<Args...>::scope(args...).addDeletion(std::move(nativeArray));
                    if constexpr(std::is_array_v<std::remove_reference_t<Out>>){
                        return reinterpret_cast<Out>(*result);
                    }else{
                        return reinterpret_cast<Out>(result);
                    }
                }else{
                    using PersistentConverter = jni_native_to_java_array_converter_t<std::remove_reference_t<JArray>,true,T,is_const>;
                    auto nativeArray = std::make_unique<O[]>(size);
                    O* result = nativeArray.get();
                    auto converter = std::make_unique<PersistentConverter>(env, in);
                    for(size_t i=0; i<size; ++i){
                        result[i] = O(converter->pointer()[i]);
                    }
                    cast_var_args<Args...>::scope(args...).addFinalAction([converter = std::move(converter), nativeArray = std::move(nativeArray)]{
                        for(size_t i=0; i<size; ++i){
                            converter->pointer()[i] = T(nativeArray[i]);
                        }
                    });
                    if constexpr(std::is_array_v<std::remove_reference_t<Out>>){
                        return reinterpret_cast<Out>(*result);
                    }else{
                        return reinterpret_cast<Out>(result);
                    }
                }
            }else{
                using PersistentConverter = jni_native_to_java_array_converter_t<std::remove_reference_t<JArray>,true,O,is_const>;
                PersistentConverter* converter = new PersistentConverter(env, in);
                cast_var_args<Args...>::scope(args...).addDeletion(converter);
                if constexpr(std::is_array_v<std::remove_reference_t<Out>>){
                    if(size_t(converter->size())<size){
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast array of size %1. Expected size: %2").arg(QString::number(converter->size()), QString::number(size)) QTJAMBI_STACKTRACEINFO );
                    }
                    return reinterpret_cast<Out>(*converter->pointer());
                }else{
                    if(converter->size()>0 && size_t(converter->size())<size){
                        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast array of size %1. Expected size: %2").arg(QString::number(converter->size()), QString::number(size)) QTJAMBI_STACKTRACEINFO );
                    }
                    return reinterpret_cast<Out>(converter->pointer());
                }
            }
        }else{
            Q_STATIC_ASSERT_X(false && !is_const, "Cannot cast types");
        }
    }

private:
    static Out castImpl(JArray in, arg_pointer_t<Args>... args){
        if constexpr(std::is_same_v<std::remove_reference_t<JArray>,jobject>){
            auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
            if constexpr(std::is_arithmetic_v<O>
                          || std::is_same_v<O,QChar>
                          || std::is_same_v<O,QLatin1Char>
                          || std::is_same_v<O,std::byte>){
                typedef typename jni_type_decider<O>::JArrayType ArrayType;
                if constexpr(!std::is_same_v<ArrayType,jobjectArray>){
                    using PersistentConverter = jni_native_to_java_array_converter_t<ArrayType,true,O,is_const>;
                    if(PersistentConverter::isValidArray(env, in)){
                        return cast(ArrayType(in), std::move(size), arg_pointer<Args>::ref(args)...);
                    }
                }
            }
            if(QtJambiAPI::isValidArray(env, in, qtjambi_type<O>::id())) {
                return castImpl(jobjectArray(in), std::move(size), arg_pointer<Args>::ref(args)...);
            }else{
                return nullptr;
            }
        }else if constexpr(std::is_same_v<std::remove_reference_t<JArray>,jstring>){
            if constexpr(std::is_same_v<O,char>
                          || std::is_same_v<O,QLatin1Char>
                          || std::is_same_v<O,std::byte>){
                QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/StringAPI, is_complete_v<convert_jstring_to_chars<O,const O*>>);
                auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
                const O* array = convert_jstring_to_chars<O,const O*>::template convert<size_t,true,false>(env, cast_var_args<arg_pointer_t<Args>...>::scope(args...), size-1, in);
                if constexpr(std::is_array_v<std::remove_reference_t<Out>>){
                    return reinterpret_cast<Out>(*array);
                }else{
                    return reinterpret_cast<Out>(array);
                }
            }else if constexpr(std::is_same_v<O,char16_t>
                                 || std::is_same_v<O,wchar_t>
                                 || std::is_same_v<O,QChar>){
                QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/StringAPI, is_complete_v<convert_jstring_to_qchars<O,const O*>>);
                auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
                const O* array = convert_jstring_to_qchars<O,const O*>::template convert<size_t,true,false>(env, cast_var_args<arg_pointer_t<Args>...>::scope(args...), size-1, in);
                if constexpr(std::is_array_v<std::remove_reference_t<Out>>){
                    return reinterpret_cast<Out>(*array);
                }else{
                    return reinterpret_cast<Out>(array);
                }
            }else{
                Q_STATIC_ASSERT_X(false && !is_const, "Cannot cast types");
                return {};
            }
        }else{
            Q_STATIC_ASSERT_X(cast_var_args<arg_pointer_t<Args>...>::hasScope, "Cannot cast array without scope.");
            auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
            if constexpr(is_const){
                PersistentJConstObjectArrayPointer<O>* converter = new PersistentJConstObjectArrayPointer<O>(env, in, [args...](O& out,JNIEnv * env,jobject in){
                    out = qtjambi_cast_with_args<O>(in, arg_pointer<Args>::deref(args, env)...);
                });
                cast_var_args<arg_pointer_t<Args>...>::scope(args...).addDeletion(converter);
                if(size>=0 && converter->size()>0 && size_t(converter->size())<size){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast array of size %1. Expected size: %2").arg(QString::number(converter->size()), QString::number(size)) QTJAMBI_STACKTRACEINFO );
                }
                if constexpr(std::is_array_v<std::remove_reference_t<Out>>){
                    return reinterpret_cast<Out>(*converter->pointer());
                }else{
                    return reinterpret_cast<Out>(converter->pointer());
                }
            }else{
                PersistentJObjectArrayPointer<O>* converter = new PersistentJObjectArrayPointer<O>(env, in, [args...](O& out,JNIEnv * env,jobject in){
                    out = qtjambi_cast_with_args<O>(in, arg_pointer<Args>::deref(args, env)...);
                }, [args...](JNIEnv * env,const O& in)->jobject{
                                                                                                   return qtjambi_cast_with_args<jobject>(in, arg_pointer<Args>::deref(args, env)...);
                                                                                               });
                cast_var_args<arg_pointer_t<Args>...>::scope(args...).addDeletion(converter);
                if(size>=0 && converter->size()>0 && size_t(converter->size())<size){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast array of size %1. Expected size: %2").arg(QString::number(converter->size()), QString::number(size)) QTJAMBI_STACKTRACEINFO );
                }
                if constexpr(std::is_array_v<std::remove_reference_t<Out>>){
                    return reinterpret_cast<Out>(*converter->pointer());
                }else{
                    return reinterpret_cast<Out>(converter->pointer());
                }
            }
        }
    }
};

template<bool forward, typename JniType, typename T, size_t N,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_std_array_cast{
    typedef std::array<T,N> NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::add_pointer_t<NativeType> NativeType_ptr;
    typedef std::conditional_t<forward, NativeType_in, JniType> In;
    typedef std::conditional_t<forward, JniType, NativeType_out> Out;
    typedef std::conditional_t<std::is_same_v<JniType, jobject> || std::is_same_v<JniType, jarray>, jni_array_type_t<T>, JniType> JArrayType;

    static Out cast(In in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(forward){
            jsize size = N;
            if constexpr (is_pointer){
                return qtjambi_array_cast<
                    JArrayType,
                    const T*,
                    jsize, true, Args...>::cast(in->data(), size, args...);
            }else{
                return qtjambi_array_cast<
                    JArrayType,
                    const T*,
                    jsize, true, Args...>::cast(in.data(), size, args...);
            }
        }else{
            if constexpr (is_pointer || is_reference){
                Q_STATIC_ASSERT_X(!is_reference || cast_var_args<Args...>::hasScope, "Cannot cast to std::array<T,N> &");
                Q_STATIC_ASSERT_X(!is_pointer || cast_var_args<Args...>::hasScope, "Cannot cast to std::array<T,N> *");
                NativeType* result;
                if constexpr(is_const){
                    result = new NativeType(qtjambi_jobject_plain_cast<forward, JniType, NativeType, false, false, false, false, Args...>::cast(in, args...));
                    cast_var_args<Args...>::scope(args...).addDeletion(result);
                }else{
                    result = new NativeType();
                    if constexpr(jni_type<JniType>::isArray
                                  && (std::is_arithmetic_v<T>
                                      || std::is_same_v<T,QChar>
                                      || std::is_same_v<T,QLatin1Char>
                                      || std::is_same_v<T,std::byte>)){
                        using PersistentConverter = jni_native_to_java_array_converter_t<JniType,true,T,is_const>;
                        auto env = cast_var_args<Args...>::env(args...);
                        PersistentConverter* converter = new PersistentConverter(env, JniType(in));
                        std::memcpy(result->data(), converter->pointer(), sizeof(T) * qMin<size_t,size_t>(N, converter->size()));
                        cast_var_args<Args...>::scope(args...).addFinalAction([converter, result]() mutable {
                            std::memcpy(converter->pointer(), result->data(), sizeof(T) * qMin<size_t,size_t>(N, converter->size()));
                            delete converter;
                            delete result;
                        });
                    }else{
                        copyArray(in, result, arg_pointer<Args>::ref(args)...);
                    }
                }
                if constexpr(is_pointer)
                    return result;
                else
                    return *result;
            }else{
                std::array<T,N> result;
                if constexpr(jni_type<JniType>::isArray
                              && (std::is_arithmetic_v<T>
                              || std::is_same_v<T,QChar>
                              || std::is_same_v<T,QLatin1Char>
                              || std::is_same_v<T,std::byte>)){
                    using Converter = jni_native_to_java_array_converter_t<JniType,false,T,is_const>;
                    auto env = cast_var_args<Args...>::env(args...);
                    Converter converter(env, JniType(in));
                    std::memcpy(result.data(), converter.pointer(), sizeof(T) * qMin<size_t,size_t>(N, converter.size()));
                }else{
                    copyArray(in, result, arg_pointer<Args>::ref(args)...);
                }
                return result;
            }
        }
    }
private:
    static void copyArray(jobject in, std::array<T,N>* result, arg_pointer_t<Args>... args){
        auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
        if constexpr(std::is_arithmetic_v<T>
                      || std::is_same_v<T,QChar>
                      || std::is_same_v<T,QLatin1Char>
                      || std::is_same_v<T,std::byte>){
            typedef jni_array_type_t<T> ArrayType;
            if constexpr(!std::is_same_v<ArrayType,jobjectArray>){
                using PersistentConverter = jni_native_to_java_array_converter_t<ArrayType,true,T,is_const>;
                if(PersistentConverter::isValidArray(env, in)){
                    PersistentConverter* converter = new PersistentConverter(env, ArrayType(in));
                    std::memcpy(result->data(), converter->pointer(), sizeof(T) * qMin<size_t,size_t>(N, converter->size()));
                    cast_var_args<Args...>::scope(args...).addFinalAction([converter, result]() mutable {
                        std::memcpy(converter->pointer(), result->data(), sizeof(T) * qMin<size_t,size_t>(N, converter->size()));
                        delete converter;
                        delete result;
                    });
                    return;
                }
            }
        }
        if(QtJambiAPI::isValidArray(env, in, qtjambi_type<T>::id())) {
            copyArray(jobjectArray(in), result, args...);
        }
    }
    static void copyArray(jobject in, std::array<T,N>& result, arg_pointer_t<Args>... args){
        auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
        if constexpr(std::is_arithmetic_v<T>
                      || std::is_same_v<T,QChar>
                      || std::is_same_v<T,QLatin1Char>
                      || std::is_same_v<T,std::byte>){
            typedef jni_array_type_t<T> ArrayType;
            if constexpr(!std::is_same_v<ArrayType,jobjectArray>){
                using Converter = jni_native_to_java_array_converter_t<ArrayType,false,T,is_const>;
                if(Converter::isValidArray(env, in)){
                    Converter converter(env, ArrayType(in));
                    std::memcpy(result.data(), converter.pointer(), sizeof(T) * qMin<size_t,size_t>(N, converter.size()));
                    return;
                }
            }
        }
        if(QtJambiAPI::isValidArray(env, in, qtjambi_type<T>::id())) {
            copyArray(jobjectArray(in), result, args...);
        }
    }
    static void copyArray(jobjectArray array, std::array<T,N>* result, arg_pointer_t<Args>... args){
        Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast array without scope.");
        auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
        auto converter = new PersistentJObjectArrayPointer<T>(env, array,
                                                       [args...](T& out,JNIEnv * env,jobject in){
                                                           out = qtjambi_cast_with_args<T>(in, arg_pointer<Args>::deref(args, env)...);
                                                       }, [args...](JNIEnv * env,const T& in)->jobject{
                                                           return qtjambi_cast_with_args<jobject>(in, arg_pointer<Args>::deref(args, env)...);
                                                       });
        for(size_t l = qMin<size_t,size_t>(N, converter->size()), i = 0; i<l; i++){
            (*result)[i] = (*converter)[jsize(i)];
        }
        cast_var_args<Args...>::scope(args...).addFinalAction([converter, result]() mutable {
            for(size_t l = qMin<size_t,size_t>(N, converter->size()), i = 0; i<l; i++){
                (*converter)[jsize(i)] = (*result)[i];
            }
            delete converter;
            delete result;
        });
    }
    static void copyArray(jobjectArray array, std::array<T,N>& result, arg_pointer_t<Args>... args){
        auto env = cast_var_args<arg_pointer_t<Args>...>::env(args...);
        JConstObjectArrayPointer<T> converter(env, array, [args...](T& out,JNIEnv * env,jobject in){
            out = qtjambi_cast_with_args<T>(in, arg_pointer<Args>::deref(args, env)...);
        });
        for(size_t l = qMin<size_t,size_t>(N, converter.length()), i = 0; i<l; i++){
            result[i] = converter[jsize(i)];
        }
    }
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_ARRAY_H
