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

#ifndef QTJAMBI_CAST_IMPL_SMARTPOINTER_H
#define QTJAMBI_CAST_IMPL_SMARTPOINTER_H

#include "global.h"
#include "qtjambi_cast_impl_container_associative.h"

namespace QtJambiPrivate {

template<bool forward, class JniType, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         typename T, bool t_is_const, typename... Args>
struct qtjambi_smart_pointer_plain_cast{
    typedef typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type T_c;
    typedef T_c T_content;
    typedef Pointer<T_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::conditional<p_is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<forward, NativeType_in, JniType>::type In;
    typedef typename std::conditional<forward, JniType, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        if constexpr(forward){
            return qtjambi_cast<JniType,T, Args...>::cast(deref_ptr<p_is_pointer, NativeType_c>::deref(in).get(), args...);
        }else{
            NativeType pointer{qtjambi_cast<T, JniType, Args...>::cast(in, args...)};
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
    typedef typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type T_c;
    typedef T_c T_content;
    typedef Pointer<T_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::conditional<p_is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

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
                                             std::is_pointer<T>::value,
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
                                             std::is_pointer<T>::value,
                                             std::is_const<T>::value,
                                             std::is_reference<T>::value,
                                             typename qtjambi_cast_types<K>::T_plain,
                                             std::is_pointer<K>::value,
                                             std::is_const<K>::value,
                                             std::is_reference<K>::value, Args...>{
};

// shared pointer to QList, QLinkedList, QSet etc

#define LISTTYPE(TYPE) QtJambiAPI::ListType::TYPE,

#define QTJAMBI_SHARED_POINTER_CONTAINER1_CASTER(TYPE,SUPERTYPE,append)\
template<bool forward, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,\
         bool c_is_const,\
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, typename... Args>\
struct qtjambi_shared_pointer_container1_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, TYPE, c_is_const, T, t_is_pointer, t_is_const, t_is_reference, Args...>\
{\
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;\
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;\
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;\
    typedef typename std::conditional<c_is_const, typename std::add_const<TYPE<T_content>>::type, TYPE<T_content>>::type C_content;\
    typedef Pointer<C_content> NativeType;\
    typedef TYPE<T_content> Container;\
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;\
    \
    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;\
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;\
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;\
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;\
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;\
    \
    static Out cast(In in, Args... args){\
        auto env = cast_var_args<Args...>::env(args...);\
        if constexpr(forward){\
            return QtJambiAPI::convert##SUPERTYPE##ToJavaObject(env,\
                                                    *reinterpret_cast<const Pointer<char>*>(&deref_ptr<p_is_pointer, NativeType_c>::deref(in)),\
                                                    LISTTYPE(TYPE)\
                                                    SUPERTYPE##Access<T_content>::newInstance()\
                                                    );\
        }else{\
            if (!in)\
                return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, NativeType, Args...>::convert(nullptr, args...);\
            if (ContainerAPI::test##TYPE<T_content>(env, in)) {\
                NativeType pointer = QtJambiAPI::convertJavaObjectToSmartPointer<Pointer,C_content>(env, in);\
                return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, NativeType, Args...>::convert(std::move(pointer), args...);\
            } else {\
                typedef IntermediateSequentialContainer<TYPE,T_content,Args...> IContainer;\
                NativeType pointer;\
                Container* list;\
                if(c_is_const){\
                    list = new Container();\
                    pointer.reset(list);\
                }else{\
                    IContainer* ilist = new IContainer(in, args...);\
                    list =ilist;\
                    pointer = Pointer<IContainer>(ilist);\
                }\
                jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, in);\
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {\
                    jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);\
                    list->append(qtjambi_cast<T_content,jobject, Args...>::cast(element, args...));\
                }\
                return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, NativeType, Args...>::convert(std::move(pointer), args...);\
            }\
        }\
    }\
};

QTJAMBI_SHARED_POINTER_CONTAINER1_CASTER(QList,QList,append)
QTJAMBI_SHARED_POINTER_CONTAINER1_CASTER(QQueue,QList,append)
QTJAMBI_SHARED_POINTER_CONTAINER1_CASTER(QStack,QList,append)

#undef LISTTYPE
#define LISTTYPE(TYPE)

QTJAMBI_SHARED_POINTER_CONTAINER1_CASTER(QSet,QSet,insert)
#undef LISTTYPE
#undef QTJAMBI_SHARED_POINTER_CONTAINER1_CASTER

#define QTJAMBI_SHARED_POINTER_CONTAINER2_CASTER(TYPE)\
template<bool forward, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,\
         bool c_is_const,\
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,\
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, typename... Args>\
struct qtjambi_shared_pointer_container2_cast<forward,\
                                                Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue,\
                                                TYPE, c_is_const,\
                                                K, k_is_pointer, k_is_const, k_is_reference,\
                                                T, t_is_pointer, t_is_const, t_is_reference, Args...>{\
    typedef typename std::conditional<k_is_pointer, typename std::add_pointer<K>::type, K>::type K_ptr;\
    typedef typename std::conditional<k_is_const, typename std::add_const<K_ptr>::type, K_ptr>::type K_const;\
    typedef typename std::conditional<k_is_reference, typename std::add_lvalue_reference<K_const>::type, K_const>::type K_content;\
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;\
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;\
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;\
    typedef TYPE<K_content,T_content> Container;\
    typedef typename std::conditional<c_is_const, typename std::add_const<Container>::type, Container>::type C_content;\
    typedef Pointer<C_content> NativeType;\
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;\
    \
    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;\
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;\
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;\
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;\
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;\
    \
    static Out cast(In in, Args... args){\
        auto env = cast_var_args<Args...>::env(args...);\
        if constexpr(forward){\
            return QtJambiAPI::convert##TYPE##ToJavaObject(env,\
                                     *reinterpret_cast<const Pointer<char>*>(&deref_ptr<p_is_pointer, NativeType_c>::deref(in)),\
                                     TYPE##Access<K_content,T_content>::newInstance()\
                                 );\
        }else{\
            if (!in)\
                return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, NativeType, Args...>::convert(nullptr, args...);\
            if (ContainerAPI::test##TYPE<K_content,T_content>(env, in)) {\
                NativeType pointer = QtJambiAPI::convertJavaObjectToSmartPointer<Pointer,C_content>(env, in);\
                return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, NativeType, Args...>::convert(std::move(pointer), args...);\
            } else {\
                typedef IntermediateAssociativeContainer<TYPE,K_content,T_content,Args...> IContainer;\
                NativeType pointer;\
                Container* map;\
                if(c_is_const){\
                    map = new Container();\
                    pointer.reset(map);\
                }else{\
                    IContainer* imap = new IContainer(in, args...);\
                    map = imap;\
                    pointer = Pointer<IContainer>(imap);\
                }\
                jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, in);\
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {\
                    jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);\
                    jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);\
                    jobject val = QtJambiAPI::valueOfJavaMapEntry(env, entry);\
                    map->insert(qtjambi_cast<K_content,jobject, Args...>::cast(key, args...), qtjambi_cast<T_content,jobject, Args...>::cast(val, args...));\
                }\
                return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, NativeType, Args...>::convert(std::move(pointer), args...);\
            }\
        }\
    }\
};

QTJAMBI_SHARED_POINTER_CONTAINER2_CASTER(QMap)
QTJAMBI_SHARED_POINTER_CONTAINER2_CASTER(QMultiMap)
QTJAMBI_SHARED_POINTER_CONTAINER2_CASTER(QHash)
QTJAMBI_SHARED_POINTER_CONTAINER2_CASTER(QMultiHash)
#undef QTJAMBI_SHARED_POINTER_CONTAINER2_CASTER

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_IMPL_SMARTPOINTER_H
