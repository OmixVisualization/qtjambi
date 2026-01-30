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

#ifndef QTJAMBI_CAST_IMPL_ARRAY_H
#define QTJAMBI_CAST_IMPL_ARRAY_H

#include "qtjambi_cast_impl.h"

namespace QtJambiPrivate {

template<typename JArray,
         typename T, bool t_is_const,
         typename I, typename... Args>
struct qtjambi_native_to_jarray_cast;

template<typename O,
         bool is_const,
         typename JArray, typename I, bool fixSize, typename... Args>
struct qtjambi_jarray_to_native_cast;

template<typename O,
         typename T,
         typename I, bool fixSize, typename... Args>
static constexpr auto qtjambi_array_cast_impl() {
    if constexpr(QtJambiPrivate::is_jni_array_type<O>::value || std::is_same_v<O, jobject> || std::is_same_v<O, jcoreobject>){
        return qtjambi_native_to_jarray_cast<O, typename std::remove_const<typename std::remove_pointer<T>::type>::type, std::is_const<typename std::remove_pointer<T>::type>::value, I, Args...>{};
    }else{
        Q_STATIC_ASSERT_X((QtJambiPrivate::is_jni_array_type<T>::value || std::is_same<T, jobject>::value || std::is_same<T, jcoreobject>::value), "Cannot cast types");
        return qtjambi_jarray_to_native_cast<typename std::remove_const<typename std::remove_pointer<O>::type>::type, std::is_const<typename std::remove_pointer<O>::type>::value, T, I, fixSize, Args...>{};
    }
}

template<typename O, typename T, typename I, bool fixSize, typename... Args>
struct qtjambi_array_cast : decltype(qtjambi_array_cast_impl<O, T, I, fixSize, Args...>()) {
};

template<typename JArray, typename T, bool t_is_const, typename I, typename... Args>
struct qtjambi_native_to_jarray_cast{
    typedef typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type T_in;

    static JArray cast(T_in* in, I size, Args...args){
        Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
        if constexpr(std::is_same_v<JArray,jobjectArray>){
            return castImpl(in, jsize(size), arg_pointer<Args>::ref(args)...);
        }else if constexpr(std::is_arithmetic_v<T>
                      || std::is_same_v<T,QChar>
                      || std::is_same_v<T,QLatin1Char>
                      || std::is_same_v<T,std::byte>){
            if constexpr(std::is_same_v<JArray,jobject>){
                typedef typename jni_type_decider<T>::JArrayType ArrayType;
                if constexpr(std::is_same_v<ArrayType,jobjectArray>){
                    return castImpl(in, jsize(size), arg_pointer<Args>::ref(args)...);
                }else{
                    typedef typename jni_type<ArrayType>::template JavaFactory<T,t_is_const>::PersistentConverter PersistentConverter;
                    typedef typename jni_type<ArrayType>::template JavaFactory<T,t_is_const>::Converter Converter;
                    auto env = cast_var_args<Args...>::env(args...);
                    if constexpr(cast_var_args<Args...>::hasScope){
                        PersistentConverter* array = new PersistentConverter(env, in, jsize(size));
                        cast_var_args<Args...>::scope(args...).addDeletion(array);
                        return array->array(env);
                    }else{
                        Converter array(env, in, jsize(size));
                        return array.array();
                    }
                }
            }else{
                typedef typename jni_type<JArray>::template JavaFactory<T,t_is_const>::PersistentConverter PersistentConverter;
                typedef typename jni_type<JArray>::template JavaFactory<T,t_is_const>::Converter Converter;
                auto env = cast_var_args<Args...>::env(args...);
                if constexpr(cast_var_args<Args...>::hasScope){
                    PersistentConverter* array = new PersistentConverter(env, in, jsize(size));
                    cast_var_args<Args...>::scope(args...).addDeletion(array);
                    return array->array(env);
                }else{
                    Converter array(env, in, jsize(size));
                    return array.array();
                }
            }
        }else{
            Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
            Q_STATIC_ASSERT_X(false && !t_is_const, "Cannot cast types");
        }
    }

private:
    static JArray castImpl(T_in* in, jsize size, typename arg_pointer<Args>::type... args){
        auto env = cast_var_args<typename arg_pointer<Args>::type...>::env(args...);
        if constexpr(t_is_const){
            if constexpr(cast_var_args<Args...>::hasScope){
                PersistentConstObjectPointerArray<T>* array = new PersistentConstObjectPointerArray<T>(env, in, size,
                                                                                                       [args...](JNIEnv * env,const T& in)->jobject{
                                                                                                           return qtjambi_cast<jobject,const T&, Args...>::cast(in, arg_pointer<Args>::deref(args, env)...);
                                                                                                       });
                cast_var_args<Args...>::scope(args...).addDeletion(array);
                return array->array(env);
            }else{
                ConstObjectPointerArray<T> array(env, in, size,
                                                 [args...](JNIEnv * env,const T& in)->jobject{
                                                     return qtjambi_cast<jobject,const T&, Args...>::cast(in, arg_pointer<Args>::deref(args, env)...);
                                                 });
                return array.array();
            }
        }else{
            if constexpr(cast_var_args<Args...>::hasScope){
                PersistentObjectPointerArray<T>* array = new PersistentObjectPointerArray<T>(env, in, size,
                                                                                             [args...](JNIEnv * env,const T& in)->jobject{
                                                                                                 return qtjambi_cast<jobject,const T&, Args...>::cast(in, arg_pointer<Args>::deref(args, env)...);
                                                                                             },
                                                                                             [args...](T& out,JNIEnv * env,jobject in){
                                                                                                 out = qtjambi_cast< T, jobject, Args...>::cast(in, arg_pointer<Args>::deref(args, env)...);
                                                                                             });
                cast_var_args<Args...>::scope(args...).addDeletion(array);
                return array->array(env);
            }else{
                ObjectPointerArray<T> array(env, in, size,
                                            [args...](JNIEnv * env,const T& in)->jobject{
                                                return qtjambi_cast<jobject,const T&, Args...>::cast(in, arg_pointer<Args>::deref(args, env)...);
                                            },
                                            [args...](T& out,JNIEnv * env,jobject in){
                                                out = qtjambi_cast< T, jobject, Args...>::cast(in, arg_pointer<Args>::deref(args, env)...);
                                            });
                return array.array();
            }
        }
    }
};

template<typename O, bool is_const, typename JArray, typename Int, bool fixSize, typename... Args>
struct qtjambi_jarray_to_native_cast{
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type Out;
    typedef std::conditional_t<fixSize, Int, Int&&> I;

    static Out* cast(JArray in, I size, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(std::is_same_v<JArray,jobjectArray> || std::is_same_v<JArray,jobject>){
            return castImpl(in, std::move(size), arg_pointer<Args>::ref(args)...);
        }else if constexpr(std::is_arithmetic_v<O>
                      || std::is_same_v<O,QChar>
                      || std::is_same_v<O,QLatin1Char>
                      || std::is_same_v<O,std::byte>){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast array without scope.");
            typedef typename jni_type<JArray>::template NativeFactory<O,is_const>::PersistentConverter JArrayPointer;
            JArrayPointer* array = new JArrayPointer(env, in);
            cast_var_args<Args...>::scope(args...).addDeletion(array);
            if constexpr(fixSize){
                if(size>=0 && array->size()>0 && Int(array->size())<size){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast array of size %1. Expected size: %2").arg(QString::number(array->size()), QString::number(size)) QTJAMBI_STACKTRACEINFO );
                }
            }else{
                size = Int(array->size());
            }
            return reinterpret_cast<Out*>(array->pointer());
        }else{
            Q_STATIC_ASSERT_X(false && !is_const, "Cannot cast types");
        }
    }

private:
    static Out* castImpl(JArray in, I size, typename arg_pointer<Args>::type... args){
        Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast array without scope.");
        auto env = cast_var_args<typename arg_pointer<Args>::type...>::env(args...);
        if constexpr(is_const){
            PersistentJConstObjectArrayPointer<O>* array = new PersistentJConstObjectArrayPointer<O>(env, in, [args...](O& out,JNIEnv * env,jobject in){
                out = qtjambi_cast< O, jobject, Args...>::cast(in, arg_pointer<Args>::deref(args, env)...);
            });
            cast_var_args<Args...>::scope(args...).addDeletion(array);
            if constexpr(fixSize){
                if(size>=0 && array->size()>0 && Int(array->size())<size){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast array of size %1. Expected size: %2").arg(QString::number(array->size()), QString::number(size)) QTJAMBI_STACKTRACEINFO );
                }
            }else{
                size = Int(array->size());
            }
            return array->pointer();
        }else{
            PersistentJObjectArrayPointer<O>* array = new PersistentJObjectArrayPointer<O>(env, in, [args...](O& out,JNIEnv * env,jobject in){
                out = qtjambi_cast< O, jobject, Args...>::cast(in, arg_pointer<Args>::deref(args, env)...);
            }, [args...](JNIEnv * env,const O& in)->jobject{
                return qtjambi_cast< jobject, const O&, Args...>::cast(in, arg_pointer<Args>::deref(args, env)...);
            });
            cast_var_args<Args...>::scope(args...).addDeletion(array);
            if constexpr(fixSize){
                if(size>=0 && array->size()>0 && Int(array->size())<size){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast array of size %1. Expected size: %2").arg(QString::number(array->size()), QString::number(size)) QTJAMBI_STACKTRACEINFO );
                }
            }else{
                size = Int(array->size());
            }
            return array->pointer();
        }
    }
    static Out* castImpl(jobject in, I size, typename arg_pointer<Args>::type... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(std::is_arithmetic_v<O>
                      || std::is_same_v<O,QChar>
                      || std::is_same_v<O,QLatin1Char>
                      || std::is_same_v<O,std::byte>){
            typedef typename jni_type_decider<O>::JArrayType ArrayType;
            if constexpr(!std::is_same_v<ArrayType,jobjectArray>){
                typedef typename jni_type<ArrayType>::template NativeFactory<O,is_const>::PersistentConverter JArrayPointer;
                if(JArrayPointer::isValidArray(env, in)){
                    return cast(ArrayType(in), std::move(size), arg_pointer<Args>::ref(args)...);
                }
            }
        }
        if(QtJambiAPI::isValidArray(env, in, qtjambi_type<O>::id())) {
            return castImpl(jobjectArray(in), std::move(size), arg_pointer<Args>::ref(args)...);
        }else{
            return nullptr;
        }
    }
};

template<bool forward, typename JniType, typename T, size_t N,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_plain_cast<forward, JniType, std::array<T,N>, is_pointer, is_const, is_reference, is_rvalue, Args...>
{
    typedef std::array<T,N> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    typedef typename std::conditional<forward, NativeType_in, JniType>::type In;
    typedef typename std::conditional<forward, JniType, NativeType_out>::type Out;
    typedef typename std::conditional<std::is_same_v<JniType, jobject> || std::is_same_v<JniType, jarray>, typename jni_type_decider<T>::JArrayType, JniType>::type JArrayType;

    static Out cast(In in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(forward){
            jsize size = N;
            if constexpr (is_pointer){
                return QtJambiPrivate::qtjambi_array_cast<
                    JArrayType,
                    const T*,
                    jsize, true, Args...>::cast(in->data(), size, args...);
            }else{
                return QtJambiPrivate::qtjambi_array_cast<
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
                        typedef typename jni_type<JniType>::template NativeFactory<T,is_const>::PersistentConverter JArrayPointer;
                        auto env = cast_var_args<Args...>::env(args...);
                        JArrayPointer* ap = new JArrayPointer(env, JniType(in));
                        std::memcpy(result->data(), ap->pointer(), sizeof(T) * qMin<size_t,size_t>(N, ap->size()));
                        cast_var_args<Args...>::scope(args...).addFinalAction([ap, result]() mutable {
                            std::memcpy(ap->pointer(), result->data(), sizeof(T) * qMin<size_t,size_t>(N, ap->size()));
                            delete ap;
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
                    typedef typename jni_type<JniType>::template NativeFactory<T,is_const>::Converter JArrayPointer;
                    auto env = cast_var_args<Args...>::env(args...);
                    JArrayPointer ap(env, JniType(in));
                    std::memcpy(result.data(), ap.pointer(), sizeof(T) * qMin<size_t,size_t>(N, ap.size()));
                }else{
                    copyArray(in, result, arg_pointer<Args>::ref(args)...);
                }
                return result;
            }
        }
    }
private:
    static void copyArray(jobject in, std::array<T,N>* result, typename arg_pointer<Args>::type... args){
        auto env = cast_var_args<typename arg_pointer<Args>::type...>::env(args...);
        if constexpr(std::is_arithmetic_v<T>
                      || std::is_same_v<T,QChar>
                      || std::is_same_v<T,QLatin1Char>
                      || std::is_same_v<T,std::byte>){
            typedef typename jni_type_decider<T>::JArrayType ArrayType;
            if constexpr(!std::is_same_v<ArrayType,jobjectArray>){
                typedef typename jni_type<ArrayType>::template NativeFactory<T,is_const>::PersistentConverter JArrayPointer;
                if(JArrayPointer::isValidArray(env, in)){
                    JArrayPointer* ap = new JArrayPointer(env, ArrayType(in));
                    std::memcpy(result->data(), ap->pointer(), sizeof(T) * qMin<size_t,size_t>(N, ap->size()));
                    cast_var_args<Args...>::scope(args...).addFinalAction([ap, result]() mutable {
                        std::memcpy(ap->pointer(), result->data(), sizeof(T) * qMin<size_t,size_t>(N, ap->size()));
                        delete ap;
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
    static void copyArray(jobject in, std::array<T,N>& result, typename arg_pointer<Args>::type... args){
        auto env = cast_var_args<typename arg_pointer<Args>::type...>::env(args...);
        if constexpr(std::is_arithmetic_v<T>
                      || std::is_same_v<T,QChar>
                      || std::is_same_v<T,QLatin1Char>
                      || std::is_same_v<T,std::byte>){
            typedef typename jni_type_decider<T>::JArrayType ArrayType;
            if constexpr(!std::is_same_v<ArrayType,jobjectArray>){
                typedef typename jni_type<ArrayType>::template NativeFactory<T,is_const>::Converter JArrayPointer;
                if(JArrayPointer::isValidArray(env, in)){
                    JArrayPointer ap(env, ArrayType(in));
                    std::memcpy(result.data(), ap.pointer(), sizeof(T) * qMin<size_t,size_t>(N, ap.size()));
                    return;
                }
            }
        }
        if(QtJambiAPI::isValidArray(env, in, qtjambi_type<T>::id())) {
            copyArray(jobjectArray(in), result, args...);
        }
    }
    static void copyArray(jobjectArray array, std::array<T,N>* result, typename arg_pointer<Args>::type... args){
        Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast array without scope.");
        auto env = cast_var_args<typename arg_pointer<Args>::type...>::env(args...);
        auto ap = new PersistentJObjectArrayPointer<T>(env, array,
                                                       [args...](T& out,JNIEnv * env,jobject in){
                                                           out = qtjambi_cast<T, jobject, Args...>::cast(in, arg_pointer<Args>::deref(args, env)...);
                                                       }, [args...](JNIEnv * env,const T& in)->jobject{
                                                           return qtjambi_cast<jobject, const T&, Args...>::cast(in, arg_pointer<Args>::deref(args, env)...);
                                                       });
        for(size_t l = qMin<size_t,size_t>(N, ap->size()), i = 0; i<l; i++){
            (*result)[i] = (*ap)[jsize(i)];
        }
        cast_var_args<Args...>::scope(args...).addFinalAction([ap, result]() mutable {
            for(size_t l = qMin<size_t,size_t>(N, ap->size()), i = 0; i<l; i++){
                (*ap)[jsize(i)] = (*result)[i];
            }
            delete ap;
            delete result;
        });
    }
    static void copyArray(jobjectArray array, std::array<T,N>& result, typename arg_pointer<Args>::type... args){
        auto env = cast_var_args<typename arg_pointer<Args>::type...>::env(args...);
        JConstObjectArrayPointer<T> ap(env, array, [args...](T& out,JNIEnv * env,jobject in){
            out = qtjambi_cast< T, jobject, Args...>::cast(in, arg_pointer<Args>::deref(args, env)...);
        });
        for(size_t l = qMin<size_t,size_t>(N, ap.length()), i = 0; i<l; i++){
            result[i] = ap[jsize(i)];
        }
    }
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_IMPL_ARRAY_H
