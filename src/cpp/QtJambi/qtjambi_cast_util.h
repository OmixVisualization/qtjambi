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

#ifndef QTJAMBI_CAST_UTIL_H
#define QTJAMBI_CAST_UTIL_H

#include <type_traits>
#include <utility>
#include "jnienvironment.h"
#include "typetests.h"
#include "qtjambiapi_name.h"
#include "qtjambiapi_nativeid.h"
#include "qtjambiapi_optional.h"
#include "qtjambiapi_ownership.h"

class AbstractContainerAccess;

enum class jcoreobject{};

namespace QtJambiPrivate {

template<typename O>
struct qtjambi_cast_result;

template<>
struct qtjambi_cast_result<jcoreobject>{
    using type = jobject;
};

template<typename O>
struct jni_type;

template<>
struct jni_type<jcoreobject>{
    static constexpr bool isObject = true;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = false;
};

template<typename T, typename...Args>
auto create(Args&&... args){
    if constexpr(is_complete_v<T>){
        return new T(std::forward<Args>(args)...);
    }else{
        Q_STATIC_ASSERT_X(is_complete_v<T>, "Cannot create unknown type");
    }
}

template<typename... Args>
static constexpr bool unuseArgs(Args...){return true;}

template<typename T>
struct qtjambi_cast_types{
    typedef std::remove_reference_t<T> T_noref;
    typedef std::remove_cv_t<T_noref> T_noconst;
    typedef std::remove_cv_t<std::remove_pointer_t<T_noconst>> T_plain;
};

template<class O, typename... Args>
constexpr typename QtJambiPrivate::qtjambi_cast_result<O>::type qtjambi_cast_with_args(Args&&... args){
    return qtjambi_cast_impl<O, Args...>::cast(std::forward<Args>(args)...);
}

template<typename TArray, typename T, bool is_const, typename JniType, typename NativeType, typename... Args>
struct value_range_converter;

template<typename T, bool isIntegral = std::is_integral_v<T>, bool isFP = std::is_floating_point_v<T>, size_t size = sizeof(T)>
struct jni_type_decider_impl{
    static constexpr bool isPrimitive = false;
    typedef jobject JType;
    typedef jobjectArray JArrayType;
    static inline auto readJavaOptional = QtJambiAPI::readJavaOptional;
    static inline auto newJavaOptional = QtJambiAPI::newJavaOptional;
};

template<typename T>
struct jni_type_decider : jni_type_decider_impl<std::remove_cv_t<T>>{
};

template<typename O>
using jni_array_type_t = typename jni_type_decider<O>::JArrayType;

template<>
struct jni_type_decider_impl<bool, std::is_integral_v<bool>, std::is_floating_point_v<bool>, sizeof(bool)>{
    static constexpr bool isPrimitive = true;
    typedef jboolean JType;
    typedef jbooleanArray JArrayType;
    static inline auto readJavaOptional = QtJambiAPI::readJavaOptional;
    static inline auto newJavaOptional = QtJambiAPI::newJavaOptional;
};

template<>
struct jni_type_decider_impl<QChar, std::is_integral_v<QChar>, std::is_floating_point_v<QChar>, sizeof(jchar)>{
    static constexpr bool isPrimitive = true;
    typedef jchar JType;
    typedef jcharArray JArrayType;
    static inline auto readJavaOptional = QtJambiAPI::readJavaOptional;
    static inline auto newJavaOptional = QtJambiAPI::newJavaOptional;
};

template<>
struct jni_type_decider_impl<wchar_t, std::is_integral_v<wchar_t>, std::is_floating_point_v<wchar_t>, sizeof(jchar)>{
    static constexpr bool isPrimitive = true;
    typedef jchar JType;
    typedef jcharArray JArrayType;
    static inline auto readJavaOptional = QtJambiAPI::readJavaOptional;
    static inline auto newJavaOptional = QtJambiAPI::newJavaOptional;
};

template<>
struct jni_type_decider_impl<char16_t, std::is_integral_v<char16_t>, std::is_floating_point_v<char16_t>, sizeof(jchar)>{
    static constexpr bool isPrimitive = true;
    typedef jchar JType;
    typedef jcharArray JArrayType;
    static inline auto readJavaOptional = QtJambiAPI::readJavaOptional;
    static inline auto newJavaOptional = QtJambiAPI::newJavaOptional;
};

template<typename T>
struct jni_type_decider_impl<T, true, false, sizeof(jbyte)>{
    static constexpr bool isPrimitive = true;
    typedef jbyte JType;
    typedef jbyteArray JArrayType;
    static inline auto readJavaOptional = QtJambiAPI::readJavaOptional;
    static inline auto newJavaOptional = QtJambiAPI::newJavaOptional;
};

template<typename T>
struct jni_type_decider_impl<T, true, false, sizeof(jshort)>{
    static constexpr bool isPrimitive = true;
    typedef jshort JType;
    typedef jshortArray JArrayType;
    static inline auto readJavaOptional = QtJambiAPI::readJavaOptional;
    static inline auto newJavaOptional = QtJambiAPI::newJavaOptional;
};

template<typename T>
struct jni_type_decider_impl<T, true, false, sizeof(jint)>{
    static constexpr bool isPrimitive = true;
    typedef jint JType;
    typedef jintArray JArrayType;
    static inline auto readJavaOptional = QtJambiAPI::readJavaOptionalInt;
    static inline auto newJavaOptional = QtJambiAPI::newJavaOptionalInt;
};

template<typename T>
struct jni_type_decider_impl<T, true, false, sizeof(jlong)>{
    typedef jlong JType;
    typedef jlongArray JArrayType;
    static inline auto readJavaOptional = QtJambiAPI::readJavaOptionalLong;
    static inline auto newJavaOptional = QtJambiAPI::newJavaOptionalLong;
};

template<typename T>
struct jni_type_decider_impl<T, false, true, sizeof(jdouble)>{
    static constexpr bool isPrimitive = true;
    typedef jdouble JType;
    typedef jdoubleArray JArrayType;
    static inline auto readJavaOptional = QtJambiAPI::readJavaOptionalDouble;
    static inline auto newJavaOptional = QtJambiAPI::newJavaOptionalDouble;
};

template<typename T>
struct jni_type_decider_impl<T, false, true, sizeof(jfloat)>{
    static constexpr bool isPrimitive = true;
    typedef jfloat JType;
    typedef jfloatArray JArrayType;
    static inline auto readJavaOptional = QtJambiAPI::readJavaOptional;
    static inline auto newJavaOptional = QtJambiAPI::newJavaOptional;
};

template<bool is_reference,typename O>
struct ptr2ref{
};

template<typename O>
struct ptr2ref<false,O>{
    static constexpr O* value(JNIEnv *, O* o){
        return o;
    }
};

template<typename O>
struct ptr2ref<true,O>{
    static inline auto value = &QtJambiAPI::checkedAddressOf<O>;
};

template<bool is_pointer,typename O>
struct deref_ptr{
};

template<typename O>
struct deref_ptr<false,O>{
    static constexpr O& deref(O& o){
        return o;
    }
};

template<typename O>
struct deref_ptr<true,O>{
    static O& deref(O* o){
        Q_ASSERT(o);
        return *o;
    }
};

template<bool is_pointer,typename O>
struct ref_ptr{
};

template<typename O>
struct ref_ptr<true,O>{
    static constexpr O* ref(O* o){
        return o;
    }
};

template<typename O>
struct ref_ptr<false,O>{
    static constexpr O* ref(O& o){
        return &o;
    }
};

template<bool is_pointer,typename O>
struct deref_ptr_or_default{
};

template<typename O>
struct deref_ptr_or_default<false,O>{
    static constexpr O& deref(O& o){
        return o;
    }
};

template<typename O>
struct deref_ptr_or_default<true,O>{
    typedef std::add_const_t<O> O_const;
    static O_const& deref(O_const* o){
        return o ? *o : QtJambiAPI::getDefaultValue<std::remove_const_t<O>>();
    }
};

template<typename O, bool has_std_constructor, bool has_copy_constructor, bool is_const, bool o_is_reference>
struct qtjambi_deref_value{
    Q_STATIC_ASSERT_X(!has_std_constructor && false, "Cannot deref type");
    typedef std::conditional_t<is_const, std::add_const_t<O>, O> O_const;
    static O_const& deref(JNIEnv *env, O* o){
        return QtJambiAPI::checkedAddressOf(env, o);
    }
};

template<typename O, bool is_const>
struct qtjambi_deref_value<O,false, true, is_const, false>{
    typedef std::conditional_t<is_const, std::add_const_t<O>, O> O_const;
    static O_const deref(JNIEnv *env, O* o){
        return O(QtJambiAPI::checkedAddressOf(env, o));
    }
};

template<typename O, bool has_copy_constructor>
struct qtjambi_deref_value<O,false/*has_std_constructor*/, has_copy_constructor, false/*is_const*/, true>{
    static constexpr auto deref = &QtJambiAPI::checkedAddressOf<O>;
};

template<typename O, bool has_copy_constructor>
struct qtjambi_deref_value<O,false/*has_std_constructor*/, has_copy_constructor, true/*is_const*/, true>{
    typedef std::add_const_t<O> O_const;
    static O_const& deref(JNIEnv *env, O* o){
        return QtJambiAPI::checkedAddressOf(env, o);
    }
};

template<typename O, bool has_copy_constructor>
struct qtjambi_deref_value<O,true/*has_std_constructor*/, has_copy_constructor,true/*is_const*/,true>{
    typedef std::add_const_t<O> O_const;
    static O_const& deref(JNIEnv *, O_const* o){
        return o ? *o : QtJambiAPI::getDefaultValue<std::remove_const_t<O>>();
    }
};

template<typename O, bool has_copy_constructor>
struct qtjambi_deref_value<O,true/*has_std_constructor*/,has_copy_constructor,false/*is_const*/,true>{
    static constexpr auto deref = &QtJambiAPI::checkedAddressOf<O>;
};

template<typename O, bool is_const>
struct qtjambi_deref_value<O,true,true,is_const,false>{
    typedef std::conditional_t<is_const, std::add_const_t<O>, O> O_const;
    static O deref(JNIEnv *, O_const* o){
        return o ? O(*o) : O();
    }
};

template<typename... Args>
struct cast_var_args;

template<bool is_pointer, bool is_const, bool is_reference, class Type, bool has_default_ctor = is_default_constructible_v<Type>, bool has_copy_ctor = is_copy_constructible_v<Type>, bool has_move_ctor = is_move_constructible_v<Type>, typename... Args>
struct pointer_ref_or_clone_decider_impl{

    template<typename T>
    static Type convert(T, Args...){
        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
    }
};

template<bool is_pointer, bool is_const, bool is_reference, class Type, typename... Args>
struct pointer_ref_or_clone_decider : pointer_ref_or_clone_decider_impl<is_pointer, is_const, is_reference, Type,
                                                                        is_default_constructible_v<Type>,
                                                                        is_copy_constructible_v<Type>,
                                                                        is_move_constructible_v<Type>, Args...>{

};

template<bool is_const, class Type, bool has_default_ctor, bool has_copy_ctor, bool has_move_ctor, typename... Args>
struct pointer_ref_or_clone_decider_impl</*is_pointer=*/true, is_const, /*is_reference=*/false, Type, has_default_ctor, has_copy_ctor, has_move_ctor, Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<Type>, Type> Type_const;

    static Type* convert(std::nullptr_t, Args...){
        return nullptr;
    }
    static Type* convert(Type&& t, Args...){
        return &t;
    }
    static Type* convert(Type* t, Args...){
        return t;
    }
    static Type_const* convert(const Type* t, Args...){
        return t;
    }
};

template<bool is_const, class Type, bool has_default_ctor, bool has_move_ctor, typename... Args>
struct pointer_ref_or_clone_decider_impl</*is_pointer=*/true, is_const, /*is_reference=*/false, Type, has_default_ctor, /*has_copy_ctor=*/true, has_move_ctor, Args...>{

    static Type* convert(std::nullptr_t, Args...){
        return nullptr;
    }

    static Type* convert(Type&& t, Args...args){
        if constexpr(!cast_var_args<Args...>::hasScope){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to %1* without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }
        Type* result = create<Type>(std::move(t));
        cast_var_args<Args...>::scope(args...).addDeletion(result);
        return result;
    }

    static Type* convert(Type* t, Args...){
        return t;
    }

    static Type* convert(const Type* t, Args...args){
        if(t){
            if constexpr(!cast_var_args<Args...>::hasScope){
                auto env = cast_var_args<Args...>::env(args...);
                JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to %1* without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
            }
            Type* result = create<Type>(*t);
            cast_var_args<Args...>::scope(args...).addDeletion(result);
            return result;
        }else{
            return nullptr;
        }
    }
};

template<bool is_const, class Type, bool has_default_ctor, bool has_move_ctor, typename... Args>
struct pointer_ref_or_clone_decider_impl</*is_pointer=*/true, is_const, /*is_reference=*/false, Type, has_default_ctor, /*has_copy_ctor=*/false, has_move_ctor, Args...>{

    static Type* convert(std::nullptr_t, Args...){
        return nullptr;
    }

    static Type* convert(Type&& t, Args...args){
        if constexpr(!cast_var_args<Args...>::hasScope){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to %1* without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }
        Type* result = create<Type>(std::move(t));
        cast_var_args<Args...>::scope(args...).addDeletion(result);
        return result;
    }

    static Type* convert(Type* t, Args...){
        return t;
    }

    static Type* convert(const Type*, Args...args){
        auto env = cast_var_args<Args...>::env(args...);
        JavaException::raiseError(env, QStringLiteral("Cannot cast to %1*.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<bool is_const, class Type, bool has_move_ctor, typename... Args>
struct pointer_ref_or_clone_decider_impl</*is_pointer=*/false, is_const, /*is_reference=*/false, Type, /*has_default_ctor=*/true, /*has_copy_ctor=*/true, has_move_ctor, Args...>{

    static Type convert(Type&& t, Args...){
        return std::move(t);
    }

    static Type convert(const Type* t, Args...){
        return t ? *t : Type();
    }
};

template<bool is_const, class Type, typename... Args>
struct pointer_ref_or_clone_decider_impl</*is_pointer=*/false, is_const, false, Type, /*has_default_ctor=*/true, false, true, Args...>{

    static Type convert(Type&& t, Args...){
        return Type(std::move(t));
    }

    static Type convert(Type* t, Args...){
        return t ? std::move(*t) : Type();
    }
};

template<class Type, bool has_copy_ctor, bool has_move_ctor, typename... Args>
struct pointer_ref_or_clone_decider_impl</*is_pointer=*/false, /*is_const=*/true, /*is_reference=*/true, Type, /*has_default_ctor=*/true, has_copy_ctor, has_move_ctor, Args...>{

    static const Type& convert(std::nullptr_t, Args...){
        return QtJambiAPI::getDefaultValue<Type>();
    }

    static const Type& convert(Type&& t, Args...args){
        if constexpr(!cast_var_args<Args...>::hasScope){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to const %1& without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }
        Type* result = create<Type>(std::move(t));
        cast_var_args<Args...>::scope(args...).addDeletion(result);
        return *result;
    }

    static const Type& convert(const Type* t, Args...){
        return t ? *t : QtJambiAPI::getDefaultValue<Type>();
    }
};

template<class Type, bool has_copy_ctor, bool has_move_ctor, typename... Args>
struct pointer_ref_or_clone_decider_impl</*is_pointer=*/false, /*is_const=*/true, /*is_reference=*/true, Type, /*has_default_ctor=*/false, has_copy_ctor, has_move_ctor, Args...>{

    static const Type& convert(Type&& t, Args...args){
        if constexpr(!cast_var_args<Args...>::hasScope){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to const %1& without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }
        Type* result = create<Type>(std::move(t));
        cast_var_args<Args...>::scope(args...).addDeletion(result);
        return *result;
    }

    static const Type& convert(const Type* t, Args...args){
        if(!t){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast nullptr to %1&.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }
        return *t;
    }
};

template<class Type, bool has_copy_ctor, bool has_move_ctor, typename... Args>
struct pointer_ref_or_clone_decider_impl</*is_pointer=*/false, /*is_const=*/false, /*is_reference=*/true, Type, /*has_default_ctor*/true, has_copy_ctor, has_move_ctor, Args...>{

    static Type& convert(Type&& t, Args...args){
        if constexpr(!cast_var_args<Args...>::hasScope){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to %1& without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }
        Type* result = create<Type>(std::move(t));
        cast_var_args<Args...>::scope(args...).addDeletion(result);
        return *result;
    }

    static Type& convert(std::nullptr_t, Args...args){
        if constexpr(!cast_var_args<Args...>::hasScope){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to %1& without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }
        Type* result = create<Type>();
        cast_var_args<Args...>::scope(args...).addDeletion(result);
        return *result;
    }

    static Type& convert(Type* t, Args...args){
        if(t){
            return *t;
        }else{
            if constexpr(!cast_var_args<Args...>::hasScope){
                auto env = cast_var_args<Args...>::env(args...);
                JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to %1& without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
            }
            Type* result = create<Type>();
            cast_var_args<Args...>::scope(args...).addDeletion(result);
            return *result;
        }
    }
};


template<class Type, bool has_copy_ctor, bool has_move_ctor, typename... Args>
struct pointer_ref_or_clone_decider_impl</*is_pointer=*/false, /*is_const=*/false, /*is_reference=*/true, Type, /*has_default_ctor=*/false, has_copy_ctor, has_move_ctor, Args...>{

    static Type& convert(Type&& t, Args...args){
        if constexpr(!cast_var_args<Args...>::hasScope){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to %1& without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }

        Type* result = create<Type>(std::move(t));
        cast_var_args<Args...>::scope(args...).addDeletion(result);
        return *result;
    }


    static Type& convert(std::nullptr_t, Args...args){
        auto env = cast_var_args<Args...>::env(args...);
        JavaException::raiseError(env, QStringLiteral("Cannot cast nullptr to %1&.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        throw "Cannot cast nullptr";
    }

    static Type& convert(Type* t, Args...args){
        if(!t){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast nullptr to %1&.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }
        return *t;
    }


    static const Type& convert(const Type* t, Args...args){
        if(!t){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast nullptr to %1&.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }
        return *t;
    }
};

template<class Type, bool has_default_ctor, bool has_copy_ctor, bool has_move_ctor, typename... Args>
struct pointer_ref_or_clone_decider_impl</*is_pointer=*/false, /*is_const=*/false, /*is_reference=*/true, Type, has_default_ctor, has_copy_ctor, has_move_ctor, Args...>{
    typedef Type Type_KT;

    static Type& convert(Type&& t, Args...){
        return t;
    }

    static Type& convert(Type* t, Args...args){
        if(!t){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast null to %1&.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }
        return *t;
    }
};

template<typename K, typename... Args>
struct qtjambi_ownership_decider{
    static void setJavaOwnership(jobject o, K* qo, Args...args)
    {
        if constexpr(std::is_arithmetic<K>::value || std::is_enum<K>::value){
            Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
            Q_UNUSED(o)
            Q_UNUSED(qo)
        }else if constexpr(std::is_base_of<QObject, K>::value){
            auto env = cast_var_args<Args...>::env(args...);
            QtJambiAPI::setJavaOwnershipForTopLevelObject(env, qo);
        }else{
            auto env = cast_var_args<Args...>::env(args...);
            Q_UNUSED(qo)
            QtJambiAPI::setJavaOwnership(env, o);
        }
    }
    static void setCppOwnershipAndInvalidate(jobject o, K* qo, Args...args)
    {
        if constexpr(std::is_arithmetic<K>::value || std::is_enum<K>::value){
            Q_UNUSED(o)
            Q_UNUSED(qo)
            Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        }else if constexpr(std::is_base_of<QObject, K>::value){
            auto env = cast_var_args<Args...>::env(args...);
            QtJambiAPI::setCppOwnershipForTopLevelObject(env, qo);
        }else{
            auto env = cast_var_args<Args...>::env(args...);
            Q_UNUSED(qo)
            if constexpr(cast_var_args<Args...>::hasScope){
                QtJambiAPI::setCppOwnership(env, o);
                cast_var_args<Args...>::scope(args...).addObjectInvalidation(env, o);
            }else{
                Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
                QtJambiAPI::setCppOwnershipAndInvalidate(env, o);
            }
        }
    }
};

template<typename NativeType, typename Output>
struct convert_jstring_to_chars;

template<typename NativeType, typename Output>
struct convert_jstring_to_qchars;

template<bool forward,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jnitype_function_cast;

template<bool forward,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jnitype_qobject_cast;

template<bool forward,
         typename JniType,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_plain_cast;

template<bool forward, typename JniType, typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_arithmetic_cast;

template<bool forward, typename JniType, typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
static constexpr auto find_qtjambi_jobject_arithmetic_cast() {
    QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/ArithmeticCast, is_complete_v<qtjambi_jobject_arithmetic_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>>);
    return qtjambi_jobject_arithmetic_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{};
}

template<bool forward,
         typename JniType, typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_function_cast{
    Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
    static void cast(...){}
};

template<bool forward, class JniType, typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_string_cast;

template<bool forward,
         typename JniType,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
static constexpr auto qtjambi_jobject_template_plain_cast() {
    if constexpr(std::is_arithmetic<NativeType>::value
                  || std::is_same_v<NativeType, QChar>
                  || std::is_same_v<NativeType, QLatin1Char>
                  || std::is_same_v<NativeType, std::byte>){
        return qtjambi_jobject_arithmetic_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{};
    }else if constexpr(std::is_function_v<NativeType>){
        return qtjambi_jobject_function_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{};
#ifdef QOBJECT_H
    }else if constexpr(std::is_base_of_v<QObject, NativeType>){
        return qtjambi_jnitype_qobject_cast<forward, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{};
#endif
    }else if constexpr(std::is_same_v<JniType, jstring>
                         || std::is_same_v<NativeType, QString>
                         || std::is_same_v<NativeType, QStringView>
                         || std::is_same_v<NativeType, QAnyStringView>
                         || std::is_same_v<NativeType, QUtf8StringView>
                         || std::is_same_v<NativeType, QLatin1String>
#if defined(__cpp_char8_t)
                         || std::is_same_v<NativeType, std::u8string>
                         || std::is_same_v<NativeType, std::u8string_view>
#endif
                         || std::is_same_v<NativeType, std::string>
                         || std::is_same_v<NativeType, std::u16string>
                         || std::is_same_v<NativeType, std::string_view>
                         || std::is_same_v<NativeType, std::u16string_view>){
        return qtjambi_string_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{};
    }else{
        return qtjambi_jobject_plain_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{};
    }
}

template<typename T, typename... Args>
struct index_of;

template<typename T>
struct index_of<T> {
    static constexpr qint64 value = -1; // not found
};

template<typename T, typename U, typename... Args>
struct index_of<T, U, Args...> {
    static constexpr qint64 sub = index_of<T, Args...>::value;
    static constexpr qint64 value = std::is_same_v<std::conditional_t<std::is_array_v<std::remove_reference_t<T>>, std::decay<T>, T>, U> ? 0 : (sub == -1 ? -1 : 1 + sub);
};

template<size_t n>
struct argument_getter{
    template<typename T, typename... Args>
    static constexpr decltype(auto) get(T first, Args... rest) {
        if constexpr (n == 0)
            return first;
        else
            return argument_getter<n - 1>::get(rest...);
    }
};

template<typename... Args>
struct cast_var_args_name{
private:
    static constexpr qint64 index = index_of<const char*,Args...>::value;
public:
    static constexpr const char* nativeTypeName(Args... args) {
        if constexpr(index>=0){
            return argument_getter<index>::get(args...);
        }else{
            Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
            return nullptr;
        }
    }
    static constexpr bool hasNativeTypeName = index>=0;
};

template<size_t N,typename... Args>
struct cast_var_args_name<const char(&)[N],Args...>{
    static constexpr const char* nativeTypeName(const char(&nativeTypeName)[N], Args...) { return nativeTypeName; }
    static constexpr bool hasNativeTypeName = true;
};

template<typename A, size_t N,typename... Args>
struct cast_var_args_name<A,const char(&)[N],Args...>{
    static constexpr const char* nativeTypeName(A, const char(&nativeTypeName)[N], Args...) { return nativeTypeName; }
    static constexpr bool hasNativeTypeName = true;
};

template<typename A, typename B, size_t N,typename... Args>
struct cast_var_args_name<A,B,const char(&)[N],Args...>{
    static constexpr const char* nativeTypeName(A, B, const char(&nativeTypeName)[N], Args...) { return nativeTypeName; }
    static constexpr bool hasNativeTypeName = true;
};

template<typename... Args>
struct cast_var_args_scope{
private:
    static constexpr qint64 index = index_of<QtJambiScope&,Args...>::value;
    static constexpr qint64 ptr_index = index_of<QtJambiScope*,Args...>::value;
public:
    static constexpr QtJambiScope* scopePointer(Args... args) {
        if constexpr(ptr_index>=0){
            return argument_getter<ptr_index>::get(args...);
        }else if constexpr(index>=0){
            return &argument_getter<index>::get(args...);
        }else{
            Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
            return nullptr;
        }
    }
    static constexpr QtJambiScope& scope(Args... args) {
        if constexpr(ptr_index>=0){
            return *argument_getter<ptr_index>::get(args...);
        }else if constexpr(index>=0){
            return argument_getter<index>::get(args...);
        }else{
            return *scopePointer(args...);
        }
    }
    static constexpr QtJambiNativeID relatedNativeID(Args... args) {
        if constexpr(ptr_index>=0){
            QtJambiScope* scope = argument_getter<ptr_index>::get(args...);
            return scope ? scope->relatedNativeID() : InvalidNativeID;
        }else if constexpr(index>=0){
            return argument_getter<index>::get(args...).relatedNativeID();
        }else{
            Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
            return InvalidNativeID;
        }
    }
    static constexpr bool hasScope = ptr_index>=0 || index>=0;
};

template<typename... Args>
struct cast_var_args_scope<QtJambiScope&,Args...>{
    static constexpr QtJambiScope* scopePointer(QtJambiScope& _scope, Args...) { return &_scope; }
    static constexpr QtJambiScope& scope(QtJambiScope& _scope, Args...) { return _scope; }
    static constexpr QtJambiScope* scopePointer(QtJambiScope* _scope, Args...) { return _scope; }
    static constexpr QtJambiScope& scope(QtJambiScope* _scope, Args...) { return *_scope; }
    static QtJambiNativeID relatedNativeID(QtJambiScope* _scope, Args...) { return _scope ? _scope->relatedNativeID() : InvalidNativeID; }
    static QtJambiNativeID relatedNativeID(QtJambiScope& _scope, Args...) { return _scope.relatedNativeID(); }
    static constexpr bool hasScope = true;
};

template<typename A, typename... Args>
struct cast_var_args_scope<A,QtJambiScope&,Args...>{
    static constexpr QtJambiScope* scopePointer(A, QtJambiScope& _scope, Args...) { return &_scope; }
    static constexpr QtJambiScope& scope(A, QtJambiScope& _scope, Args...) { return _scope; }
    static constexpr QtJambiScope* scopePointer(A, QtJambiScope* _scope, Args...) { return _scope; }
    static constexpr QtJambiScope& scope(A, QtJambiScope* _scope, Args...) { return *_scope; }
    static QtJambiNativeID relatedNativeID(A, QtJambiScope* _scope, Args...) { return _scope ? _scope->relatedNativeID() : InvalidNativeID; }
    static QtJambiNativeID relatedNativeID(A, QtJambiScope& _scope, Args...) { return _scope.relatedNativeID(); }
    static constexpr bool hasScope = true;
};

template<typename A, typename B, typename... Args>
struct cast_var_args_scope<A,B,QtJambiScope&,Args...>{
    static constexpr QtJambiScope* scopePointer(A, B, QtJambiScope& _scope, Args...) { return &_scope; }
    static constexpr QtJambiScope& scope(A, B, QtJambiScope& _scope, Args...) { return _scope; }
    static constexpr QtJambiScope* scopePointer(A, B, QtJambiScope* _scope, Args...) { return _scope; }
    static constexpr QtJambiScope& scope(A, B, QtJambiScope* _scope, Args...) { return *_scope; }
    static QtJambiNativeID relatedNativeID(A, B, QtJambiScope* _scope, Args...) { return _scope ? _scope->relatedNativeID() : InvalidNativeID; }
    static QtJambiNativeID relatedNativeID(A, B, QtJambiScope& _scope, Args...) { return _scope.relatedNativeID(); }
    static constexpr bool hasScope = true;
};

template<typename... Args>
struct cast_var_args_env{
private:
    static constexpr qint64 index = index_of<JNIEnv*,Args...>::value;
public:
    static auto env(Args... args) {
        if constexpr(index>=0){
            return argument_getter<index>::get(args...);
        }else{
            Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
            return JniEnvironment{500};
        }
    }
    static constexpr bool hasJNIEnv = index>=0;
};

template<typename... Args>
struct cast_var_args_env<JNIEnv*,Args...>{
    static constexpr JNIEnv* env(JNIEnv* env,Args...) { return env; }
    static constexpr bool hasJNIEnv = true;
};

template<typename A, typename... Args>
struct cast_var_args_env<A,JNIEnv*,Args...>{
    static constexpr JNIEnv* env(A,JNIEnv* env,Args...) { return env; }
    static constexpr bool hasJNIEnv = true;
};

template<typename A, typename B, typename... Args>
struct cast_var_args_env<A,B,JNIEnv*,Args...>{
    static constexpr JNIEnv* env(A,B,JNIEnv* env,Args...) { return env; }
    static constexpr bool hasJNIEnv = true;
};

template<typename... Args>
struct cast_var_args : cast_var_args_name<Args...>, cast_var_args_scope<Args...>, cast_var_args_env<Args...>{
};

template<typename O>
struct arg_pointer{
};

template<typename O>
using arg_pointer_t = typename arg_pointer<O>::type;

template<typename O>
struct arg_pointer<O*>{
    typedef O* type;
    static constexpr type ref(type o) {
        return o;
    }
    static constexpr type deref(type o,...) {
        return o;
    }
};

template<>
struct arg_pointer<JNIEnv*>{
    typedef JNIEnv* type;
    static constexpr type deref(type,type env) {
        return env;
    }
    static constexpr type ref(type o) {
        return o;
    }
    static constexpr type deref(type o) {
        return o;
    }
};

template<typename O>
struct arg_pointer<O&>{
    typedef O* type;
    static constexpr type ref(O& o) {
        return &o;
    }
    static constexpr O& deref(type o,...) {
        return *o;
    }
};

template<typename NativeType>
struct value_range_requires_scope : std::true_type {

};

template<typename NativeType>
constexpr bool value_range_requires_scope_v = value_range_requires_scope<NativeType>::value;

template<>
struct value_range_requires_scope<QByteArray> : std::false_type {

};

template<>
struct value_range_requires_scope<QString> : std::false_type {

};

template<typename T, size_t N>
struct value_range_requires_scope<std::array<T,N>> : std::false_type {

};

template<typename T>
struct value_range_requires_scope<QList<T>> : std::false_type {

};

template<typename T>
struct value_range_requires_scope<QSet<T>> : std::false_type {

};

template<typename T>
struct value_range_requires_scope<QQueue<T>> : std::false_type {

};

template<typename T>
struct value_range_requires_scope<QStack<T>> : std::false_type {

};

template<typename K, typename T>
struct value_range_requires_scope<QMap<K,T>> : std::false_type {

};

template<typename K, typename T>
struct value_range_requires_scope<QMultiMap<K,T>> : std::false_type {

};

template<typename K, typename T>
struct value_range_requires_scope<QHash<K,T>> : std::false_type {

};

template<typename K, typename T>
struct value_range_requires_scope<QMultiHash<K,T>> : std::false_type {

};

template<size_t s, typename... Args>
struct qtjambi_cast_enabled_spread_test : std::false_type{
};

template<typename T>
struct qtjambi_cast_enabled_spread_test<1,T> : std::true_type{
};

template<typename T, size_t N>
struct qtjambi_cast_enabled_spread_test<2,T,const char (&)[N]> : std::bool_constant<!std::is_assignable_v<JNIEnv*,T> && !std::is_convertible_v<T, JNIEnv*>>{
};

template<typename T>
struct qtjambi_cast_enabled_spread_test<2,JNIEnv*,T> : std::true_type{
};

template<size_t N>
struct qtjambi_cast_enabled_spread_test<2,JNIEnv*,const char (&)[N]> : std::true_type{
};

template<typename T>
struct qtjambi_cast_enabled_spread_test<2,QtJambiScope&,T> : std::true_type{
};

template<typename T>
struct qtjambi_cast_enabled_spread_test<3,JNIEnv*,QtJambiScope&,T> : std::true_type{
};

template<typename T, size_t N>
struct qtjambi_cast_enabled_spread_test<3,JNIEnv*,T,const char (&)[N]> : std::true_type{
};

template<typename T, size_t N>
struct qtjambi_cast_enabled_spread_test<4,QtJambiScope&,T,const char (&)[N]> : std::true_type{
};

template<size_t N>
struct qtjambi_cast_enabled_spread_test<2,QtJambiScope&,const char (&)[N]> : std::true_type{
};

template<typename T, size_t N>
struct qtjambi_cast_enabled_spread_test<4,JNIEnv*,QtJambiScope&,T,const char (&)[N]> : std::true_type{
};

template<typename T, class I>
struct qtjambi_cast_enabled_spread_test<2,T,I> : std::bool_constant<!std::is_assignable_v<JNIEnv*,T> && !std::is_convertible_v<T, JNIEnv*>
                                                                      && ( std::is_integral_v<std::remove_reference_t<I>> || std::is_same_v<QtJambiNativeID,std::remove_cv_t<std::remove_reference_t<T>>>)>{
};

template<typename T, class I, size_t N>
struct qtjambi_cast_enabled_spread_test<3,T,I,const char (&)[N]> : std::bool_constant<!std::is_assignable_v<JNIEnv*,T> && !std::is_convertible_v<T, JNIEnv*>
                                                                                         && ( std::is_integral_v<std::remove_reference_t<I>> || std::is_same_v<QtJambiNativeID,std::remove_cv_t<std::remove_reference_t<T>>>)>{
};

template<typename T, class I>
struct qtjambi_cast_enabled_spread_test<3,JNIEnv*,T,I> : std::bool_constant<std::is_integral_v<std::remove_reference_t<I>> || std::is_same_v<QtJambiNativeID,std::remove_cv_t<std::remove_reference_t<T>>>> {
};

template<typename T, class I>
struct qtjambi_cast_enabled_spread_test<3,QtJambiScope&,T,I> : std::bool_constant<std::is_integral_v<std::remove_reference_t<I>> || std::is_same_v<QtJambiNativeID,std::remove_cv_t<std::remove_reference_t<T>>>> {
};

template<typename T, class I>
struct qtjambi_cast_enabled_spread_test<4,JNIEnv*,QtJambiScope&,T,I> : std::bool_constant<std::is_integral_v<std::remove_reference_t<I>> || std::is_same_v<QtJambiNativeID,std::remove_cv_t<std::remove_reference_t<T>>>> {
};

template<typename T, class I, size_t N>
struct qtjambi_cast_enabled_spread_test<4,JNIEnv*,T,I,const char (&)[N]> : std::bool_constant<std::is_integral_v<std::remove_reference_t<I>> || std::is_same_v<QtJambiNativeID,std::remove_cv_t<std::remove_reference_t<T>>>>{
    static constexpr bool has_size_param = true;
};

template<typename T, class I, size_t N>
struct qtjambi_cast_enabled_spread_test<4,QtJambiScope&,T,I,const char (&)[N]> : std::bool_constant<std::is_integral_v<std::remove_reference_t<I>> || std::is_same_v<QtJambiNativeID,std::remove_cv_t<std::remove_reference_t<T>>>> {
    static constexpr bool has_size_param = true;
};

template<typename T, class I, size_t N>
struct qtjambi_cast_enabled_spread_test<5,JNIEnv*,QtJambiScope&,T,I,const char (&)[N]> : std::bool_constant<std::is_integral_v<std::remove_reference_t<I>> || std::is_same_v<QtJambiNativeID,std::remove_cv_t<std::remove_reference_t<T>>>> {
    static constexpr bool has_size_param = true;
};

template<typename... Args>
struct qtjambi_cast_enabled_test;

template<typename... Args>
struct qtjambi_cast_enabled_test : qtjambi_cast_enabled_spread_test<sizeof...(Args), Args...>{
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_UTIL_H
