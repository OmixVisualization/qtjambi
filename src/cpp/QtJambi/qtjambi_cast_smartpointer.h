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

#ifndef QTJAMBI_CAST_SMARTPOINTER_H
#define QTJAMBI_CAST_SMARTPOINTER_H

#include "qtjambi_cast_template1.h"
#include "qtjambiapi_smartpointer.h"

namespace QtJambiPrivate {

template<bool forward, class JniType, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         typename T, bool t_is_const, typename... Args>
struct qtjambi_smart_pointer_plain_cast{
    typedef std::conditional_t<t_is_const, std::add_const_t<T>, T> T_c;
    typedef T_c T_content;
    typedef Pointer<T_content> NativeType;
    typedef std::conditional_t<p_is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;

    typedef std::conditional_t<p_is_reference, std::conditional_t<p_is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<p_is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<p_is_pointer, std::add_pointer_t<NativeType_c>, std::conditional_t<p_is_reference, std::add_lvalue_reference_t<NativeType_c>, NativeType_c>> NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, JniType> In;
    typedef std::conditional_t<forward, JniType, NativeType_out> Out;

    static Out cast(In in, Args... args){
        if constexpr(forward){
            return qtjambi_cast_with_args<JniType>(deref_ptr<p_is_pointer, NativeType_c>::deref(in).get(), std::forward<Args>(args)...);
        }else{
            NativeType pointer{qtjambi_cast_with_args<T>(in, std::forward<Args>(args)...)};
            return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, NativeType, Args...>::convert(std::move(pointer), args...);
        }
    }
};

template<bool forward, class JniType, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         typename T, bool t_is_const, typename... Args>
struct qtjambi_smart_pointer_default_cast{
    Q_STATIC_ASSERT_X(false && !p_is_pointer, "Cannot cast types");
};

template<bool forward, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         typename T, bool t_is_const, typename... Args>
struct qtjambi_smart_pointer_default_cast<forward, jobject, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue,
                                          T, t_is_const, Args...>{
    typedef std::conditional_t<t_is_const, std::add_const_t<T>, T> T_c;
    typedef T_c T_content;
    typedef Pointer<T_content> NativeType;
    typedef std::conditional_t<p_is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;

    typedef std::conditional_t<p_is_reference, std::conditional_t<p_is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<p_is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<p_is_pointer, std::add_pointer_t<NativeType_c>, std::conditional_t<p_is_reference, std::add_lvalue_reference_t<NativeType_c>, NativeType_c>> NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, jobject> In;
    typedef std::conditional_t<forward, jobject, NativeType_out> Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            return QtJambiAPI::convertSmartPointerToJavaObject<Pointer,T_content>(env, deref_ptr<p_is_pointer, NativeType_c>::deref(in));
        }else{
            NativeType pointer = QtJambiAPI::convertJavaObjectToSmartPointer<Pointer,T_content>(env, in);
            return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, NativeType, Args...>::convert(std::move(pointer), args...);
        }
    }
};

template<bool forward, typename JniType, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue, bool t_is_const, typename Args, template<typename... Ts> class Container, typename... Ts>
static constexpr auto qtjambi_smart_pointer_template_cast_impl(const Container<Ts...>&);

template<bool forward, class JniType, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue, typename T, typename... Args>
static constexpr auto qtjambi_smart_pointer_cast_impl() {
    typedef std::remove_cv_t<T> T_nonconst;
    if constexpr(is_template<T_nonconst>::value){
        return decltype(qtjambi_smart_pointer_template_cast_impl<forward, JniType, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, std::is_const_v<T>, std::tuple<Args...>>(std::declval<const T_nonconst&>())){};
    }else if constexpr(std::is_arithmetic<T_nonconst>::value
                         || std::is_same_v<T_nonconst, QChar>
                         || std::is_same_v<T_nonconst, QLatin1Char>
                         || std::is_same_v<T_nonconst, std::byte>
                         || std::is_function_v<T_nonconst>){
        return qtjambi_smart_pointer_plain_cast<forward, JniType, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, T_nonconst, std::is_const_v<T>, Args...>{};
    }else{
        return qtjambi_smart_pointer_default_cast<forward, JniType, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, T_nonconst, std::is_const_v<T>, Args...>{};
    }
}

template<bool forward, class JniType, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue, typename T, typename... Args>
struct qtjambi_smart_pointer_cast : decltype(qtjambi_smart_pointer_cast_impl<forward, JniType, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, T, Args...>()){
};

//template from any container Pointer<T> to jobject

template<bool forward,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         template<typename...Ts> class Container, bool c_is_const, int parameterCount, typename...Ts>
struct qtjambi_shared_pointer_template_cast{
    Q_STATIC_ASSERT_X(false && !p_is_pointer, "Cannot cast types");
};

template<bool forward,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         template<typename T> class Container, bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, typename... Args>
struct qtjambi_shared_pointer_container1_cast{
    Q_STATIC_ASSERT_X(false && !p_is_pointer, "Cannot cast types");
};

template<bool forward,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         template<typename K, typename T> class Container, bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, typename... Args>
struct qtjambi_shared_pointer_container2_cast{
    Q_STATIC_ASSERT_X(false && !p_is_pointer, "Cannot cast types");
};

template<bool forward,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         template<typename...> class Container, bool t_is_const, typename Args, size_t N, typename...Ts>
struct qtjambi_smart_pointer_template_cast{
};

template<bool forward, typename JniType, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue, bool t_is_const, typename Args, template<typename... Ts> class Container, typename... Ts>
static constexpr auto qtjambi_smart_pointer_template_cast_impl(const Container<Ts...>&){
    return qtjambi_smart_pointer_template_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, Container, t_is_const, Args, sizeof...(Ts), Ts...>{};
}

template<bool forward,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         template<typename T> class Container, bool t_is_const, typename T, typename... Args>
struct qtjambi_smart_pointer_template_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, Container, t_is_const, std::tuple<Args...>, 1, T>
    : qtjambi_shared_pointer_container1_cast<forward, Pointer, p_is_pointer,
                                             p_is_const, p_is_reference, p_is_rvalue,
                                             Container, t_is_const, typename qtjambi_cast_types<T>::T_plain,
                                             std::is_pointer_v<T>,
                                             std::is_const<T>::value,
                                             std::is_reference<T>::value, Args...>{
};

template<bool forward,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         template<typename T,typename K> class Container, bool t_is_const, typename T, typename K, typename... Args>
struct qtjambi_smart_pointer_template_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, Container, t_is_const, std::tuple<Args...>, 2, T, K>
    : qtjambi_shared_pointer_container2_cast<forward, Pointer, p_is_pointer,
                                             p_is_const, p_is_reference, p_is_rvalue,
                                             Container, t_is_const, typename qtjambi_cast_types<T>::T_plain,
                                             std::is_pointer_v<T>,
                                             std::is_const_v<T>,
                                             std::is_reference_v<T>,
                                             typename qtjambi_cast_types<K>::T_plain,
                                             std::is_pointer_v<K>,
                                             std::is_const_v<K>,
                                             std::is_reference_v<K>, Args...>{
};

// shared pointer to QList, QLinkedList, QSet etc

template<bool forward, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         template<typename T> class NativeType, bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, typename... Args>
struct qtjambi_shared_pointer_sequential_container_cast;

template<bool forward, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         template<typename T> class NativeType, bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, typename... Args>
static constexpr auto find_qtjambi_shared_pointer_sequential_container_cast(){
    constexpr bool hasCastImpl = is_complete_v< qtjambi_shared_pointer_sequential_container_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, NativeType, c_is_const, T, t_is_pointer, t_is_const, t_is_reference, Args...> >;
    Q_STATIC_ASSERT_X(hasCastImpl, "Cannot cast without including <QtJambi/ContainerCast>");
    return qtjambi_shared_pointer_sequential_container_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, NativeType, c_is_const, T, t_is_pointer, t_is_const, t_is_reference, Args...>{};
}

template<bool forward, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, typename... Args>
struct qtjambi_shared_pointer_container1_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, QList, c_is_const, T, t_is_pointer, t_is_const, t_is_reference, Args...>
    : decltype( find_qtjambi_shared_pointer_sequential_container_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, QList, c_is_const, T, t_is_pointer, t_is_const, t_is_reference, Args...>() ) {
};

template<bool forward, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, typename... Args>
struct qtjambi_shared_pointer_container1_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, QQueue, c_is_const, T, t_is_pointer, t_is_const, t_is_reference, Args...>
    : decltype( find_qtjambi_shared_pointer_sequential_container_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, QQueue, c_is_const, T, t_is_pointer, t_is_const, t_is_reference, Args...>() ) {
};

template<bool forward, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, typename... Args>
struct qtjambi_shared_pointer_container1_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, QStack, c_is_const, T, t_is_pointer, t_is_const, t_is_reference, Args...>
    : decltype( find_qtjambi_shared_pointer_sequential_container_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, QStack, c_is_const, T, t_is_pointer, t_is_const, t_is_reference, Args...>() ) {
};

template<bool forward, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, typename... Args>
struct qtjambi_shared_pointer_container1_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, QSet, c_is_const, T, t_is_pointer, t_is_const, t_is_reference, Args...>
    : decltype( find_qtjambi_shared_pointer_sequential_container_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, QSet, c_is_const, T, t_is_pointer, t_is_const, t_is_reference, Args...>() ) {
};

template<bool forward, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         template<typename K, typename T> class NativeType, bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, typename... Args>
struct qtjambi_shared_pointer_associative_container_cast;

template<bool forward, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         template<typename K, typename T> class NativeType, bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, typename... Args>
static constexpr auto find_qtjambi_shared_pointer_associative_container_cast(){
    constexpr bool hasCastImpl = is_complete_v< qtjambi_shared_pointer_associative_container_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, NativeType, c_is_const, K, k_is_pointer, k_is_const, k_is_reference, T, t_is_pointer, t_is_const, t_is_reference, Args...> >;
    Q_STATIC_ASSERT_X(hasCastImpl, "Cannot cast without including <QtJambi/ContainerCast>");
    return qtjambi_shared_pointer_associative_container_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, NativeType, c_is_const, K, k_is_pointer, k_is_const, k_is_reference, T, t_is_pointer, t_is_const, t_is_reference, Args...>{};
}

template<bool forward, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, typename... Args>
struct qtjambi_shared_pointer_container2_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, QMap, c_is_const, K, k_is_pointer, k_is_const, k_is_reference, T, t_is_pointer, t_is_const, t_is_reference, Args...>
    : decltype( qtjambi_shared_pointer_associative_container_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, QMap, c_is_const, K, k_is_pointer, k_is_const, k_is_reference, T, t_is_pointer, t_is_const, t_is_reference, Args...>() ) {
};

template<bool forward, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, typename... Args>
struct qtjambi_shared_pointer_container2_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, QMultiMap, c_is_const, K, k_is_pointer, k_is_const, k_is_reference, T, t_is_pointer, t_is_const, t_is_reference, Args...>
    : decltype( qtjambi_shared_pointer_associative_container_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, QMultiMap, c_is_const, K, k_is_pointer, k_is_const, k_is_reference, T, t_is_pointer, t_is_const, t_is_reference, Args...>() ) {
};

template<bool forward, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, typename... Args>
struct qtjambi_shared_pointer_container2_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, QHash, c_is_const, K, k_is_pointer, k_is_const, k_is_reference, T, t_is_pointer, t_is_const, t_is_reference, Args...>
    : decltype( qtjambi_shared_pointer_associative_container_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, QHash, c_is_const, K, k_is_pointer, k_is_const, k_is_reference, T, t_is_pointer, t_is_const, t_is_reference, Args...>() ) {
};

template<bool forward, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, typename... Args>
struct qtjambi_shared_pointer_container2_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, QMultiHash, c_is_const, K, k_is_pointer, k_is_const, k_is_reference, T, t_is_pointer, t_is_const, t_is_reference, Args...>
    : decltype( qtjambi_shared_pointer_associative_container_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, QMultiHash, c_is_const, K, k_is_pointer, k_is_const, k_is_reference, T, t_is_pointer, t_is_const, t_is_reference, Args...>() ) {
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_SMARTPOINTER_H
