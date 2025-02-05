/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0) && defined(QLINKEDLIST_H)
QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
QT_WARNING_DISABLE_DEPRECATED
#endif

template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, T& in);
template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, const T& in);
template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, T* in);

namespace QtJambiPrivate {

template<bool forward, bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_arithmetic, bool t_is_enum, bool t_is_pointer, bool t_is_const, bool t_is_function, bool t_is_qobject, bool t_is_template>
struct qtjambi_smart_pointer_caster{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

//template from any QObject* Pointer<T> to jobject

template<bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_const>
struct qtjambi_smart_pointer_caster<true, has_scope,
                                    Pointer, p_is_pointer, p_is_const, p_is_reference,
                                    T, false, false, false, t_is_const, false, true, false>{
    typedef typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type T_c;
    typedef T_c T_content;
    typedef Pointer<T_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return QtJambiAPI::convertSmartPointerToJavaObject<Pointer,T_content>(env, deref_ptr<p_is_pointer, NativeType_c>::deref(in));
    }
};

template<bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_const>
struct qtjambi_smart_pointer_caster<false, has_scope,
                                    Pointer, p_is_pointer, p_is_const, p_is_reference,
                                    T, false, false, false, t_is_const, false, true, false>{
    typedef typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type T_c;
    typedef T_c T_content;
    typedef Pointer<T_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!(p_is_reference && !p_is_const), "Cannot cast to Pointer<T> &");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        NativeType pointer = QtJambiAPI::convertJavaObjectToSmartPointer<Pointer,T_content>(env, in);
        return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, has_scope, NativeType>::convert(env, scope, std::move(pointer));
    }
};

//template from any non-QObject* Pointer<T> to jobject

template<bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_const>
struct qtjambi_smart_pointer_caster<true, has_scope,
                                    Pointer, p_is_pointer, p_is_const, p_is_reference,
                                    T, false, false, false, t_is_const, false, false, false>{
    typedef typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type T_content;
    typedef Pointer<T_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return QtJambiAPI::convertSmartPointerToJavaObject<Pointer,T_content>(env, deref_ptr<p_is_pointer, NativeType_c>::deref(in));
    }
};

template<bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_const>
struct qtjambi_smart_pointer_caster<false, has_scope,
                                    Pointer, p_is_pointer, p_is_const, p_is_reference,
                                    T, false, false, false, t_is_const, false, false, false>{
    typedef typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type T_content;
    typedef Pointer<T_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!(p_is_reference && !p_is_const), "Cannot cast to Pointer<T> &");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        NativeType pointer = QtJambiAPI::convertJavaObjectToSmartPointer<Pointer,T_content>(env, in);
        return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, has_scope, NativeType>::convert(env, scope, std::move(pointer));
    }
};

//template from any container Pointer<T> to jobject

template<bool forward,
         bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         template<typename...Ts> class Container, bool c_is_const, int parameterCount, typename...Ts>
struct qtjambi_shared_pointer_container_cast_decider{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<bool forward,
         bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         template<typename T> class Container, bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container1_caster{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<bool forward, bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         template<typename K, typename T> class Container, bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container2_caster{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<bool forward, bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         template<typename T> class Container, bool c_is_const, typename T>
static constexpr qtjambi_shared_pointer_container1_caster<forward,
                                                            has_scope,
                                                            Pointer, p_is_pointer, p_is_const, p_is_reference,
                                                            Container, c_is_const,
                                                            typename std::remove_pointer<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::type,
                                                            std::is_pointer<T>::value,
                                                            std::is_const<T>::value,
                                                            std::is_reference<T>::value
                                                            > qtjambi_shared_pointer_container1_supertype(){
    return {};
}

template<bool forward, bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         template<typename...Ts> class Container, bool c_is_const, typename...Ts>
struct qtjambi_shared_pointer_container_cast_decider<forward, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, Container, c_is_const, 1, Ts...>
        : decltype(qtjambi_shared_pointer_container1_supertype<forward, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, Container, c_is_const, Ts...>())
{
};

template<bool forward, bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         template<typename K, typename T> class Container, bool c_is_const, typename K, typename T>
static constexpr qtjambi_shared_pointer_container2_caster<forward,
                                                            has_scope,
                                                            Pointer, p_is_pointer, p_is_const, p_is_reference,
                                                            Container, c_is_const,
                                                            typename std::remove_pointer<typename std::remove_cv<typename std::remove_reference<K>::type>::type>::type,
                                                            std::is_pointer<K>::value,
                                                            std::is_const<K>::value,
                                                            std::is_reference<K>::value,
                                                            typename std::remove_pointer<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::type,
                                                            std::is_pointer<T>::value,
                                                            std::is_const<T>::value,
                                                            std::is_reference<T>::value
                                                            > qtjambi_shared_pointer_container2_supertype(){
    return {};
}

template<bool forward, bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         template<typename...Ts> class Container, bool c_is_const, typename...Ts>
struct qtjambi_shared_pointer_container_cast_decider<forward, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, Container, c_is_const, 2, Ts...>
        : decltype(qtjambi_shared_pointer_container2_supertype<forward, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, Container, c_is_const, Ts...>())
{
};

template<bool forward, bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         template<typename...Ts> class Container, bool t_is_const, typename...Ts>
struct qtjambi_smart_pointer_caster<forward, has_scope,
                                    Pointer, p_is_pointer, p_is_const, p_is_reference,
                                    Container<Ts...>, false, false, false, t_is_const, false, false, true>
        :qtjambi_shared_pointer_container_cast_decider<forward, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference,
                                                        Container, t_is_const, sizeof... (Ts), Ts...>
{
};

// shared pointer to QList, QLinkedList, QSet etc

#define LISTTYPE(TYPE) QtJambiAPI::ListType::TYPE,

#define QTJAMBI_SHARED_POINTER_CONTAINER1_CASTER(TYPE,SUPERTYPE,append)\
template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,\
         bool c_is_const,\
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>\
struct qtjambi_shared_pointer_container1_caster<true, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, TYPE, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>\
{\
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;\
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;\
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;\
    typedef typename std::conditional<c_is_const, typename std::add_const<TYPE<T_content>>::type, TYPE<T_content>>::type C_content;\
    typedef Pointer<C_content> NativeType;\
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;\
    \
    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;\
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;\
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;\
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){\
        return QtJambiAPI::convert##SUPERTYPE##ToJavaObject(env,\
                                                    *reinterpret_cast<const Pointer<char>*>(&deref_ptr<p_is_pointer, NativeType_c>::deref(in)),\
                                                    LISTTYPE(TYPE)\
                                                    SUPERTYPE##Access<T_content>::newInstance()\
                                                    );\
    }\
};\
  \
template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,\
         bool c_is_const,\
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>\
struct qtjambi_shared_pointer_container1_caster<false, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, TYPE, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>\
{\
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;\
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;\
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;\
    typedef TYPE<T_content> Container;\
    typedef typename std::conditional<c_is_const, typename std::add_const<Container>::type, Container>::type C_content;\
    typedef Pointer<C_content> NativeType;\
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;\
    \
    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;\
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;\
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;\
    typedef IntermediateSequentialContainer<TYPE,T_content> IContainer;\
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){\
        if (!in)\
            return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, has_scope, NativeType>::convert(env, scope, nullptr);\
        if (ContainerAPI::test##TYPE<T_content>(env, in)) {\
            NativeType pointer = QtJambiAPI::convertJavaObjectToSmartPointer<Pointer,C_content>(env, in);\
            return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, has_scope, NativeType>::convert(env, scope, std::move(pointer));\
        } else {\
            NativeType pointer;\
            Container* list;\
            if(c_is_const){\
                list = new Container();\
                pointer.reset(list);\
            }else{\
                IContainer* ilist = new IContainer(env, in, *scope);\
                list =ilist;\
                pointer = Pointer<IContainer>(ilist);\
            }\
            jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, in);\
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {\
                jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);\
                list->append(qtjambi_scoped_cast<has_scope,T_content,jobject>::cast(env, element, nullptr, scope));\
            }\
            return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, has_scope, NativeType>::convert(env, scope, std::move(pointer));\
        }\
    }\
};

QTJAMBI_SHARED_POINTER_CONTAINER1_CASTER(QList,QList,append)
QTJAMBI_SHARED_POINTER_CONTAINER1_CASTER(QQueue,QList,append)

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#ifdef QLINKEDLIST_H
#undef LISTTYPE
#define LISTTYPE(TYPE)

template<typename T>
class QLinkedListAccess;

QTJAMBI_SHARED_POINTER_CONTAINER1_CASTER(QLinkedList,QLinkedList,append)
#endif // def QLINKEDLIST_H

#ifdef QVECTOR_H
#undef LISTTYPE
#define LISTTYPE(TYPE) QtJambiAPI::VectorType::TYPE,
QTJAMBI_SHARED_POINTER_CONTAINER1_CASTER(QStack,QVector,append)
QTJAMBI_SHARED_POINTER_CONTAINER1_CASTER(QVector,QVector,append)
#endif // def QVECTOR_H
#else
QTJAMBI_SHARED_POINTER_CONTAINER1_CASTER(QStack,QList,append)
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

#undef LISTTYPE
#define LISTTYPE(TYPE)

QTJAMBI_SHARED_POINTER_CONTAINER1_CASTER(QSet,QSet,insert)
#undef LISTTYPE
#undef QTJAMBI_SHARED_POINTER_CONTAINER1_CASTER

#define QTJAMBI_SHARED_POINTER_CONTAINER2_CASTER(TYPE)\
template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,\
         bool c_is_const,\
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,\
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>\
struct qtjambi_shared_pointer_container2_caster<true, has_scope,\
                                                Pointer, p_is_pointer, p_is_const, p_is_reference,\
                                                TYPE, c_is_const,\
                                                K, k_is_pointer, k_is_const, k_is_reference,\
                                                T, t_is_pointer, t_is_const, t_is_reference>{\
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
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){\
        return QtJambiAPI::convert##TYPE##ToJavaObject(env,\
                                     *reinterpret_cast<const Pointer<char>*>(&deref_ptr<p_is_pointer, NativeType_c>::deref(in)),\
                                     TYPE##Access<K_content,T_content>::newInstance()\
                                 );\
    }\
};\
\
template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,\
         bool c_is_const,\
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,\
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>\
struct qtjambi_shared_pointer_container2_caster<false, has_scope,\
                                                Pointer, p_is_pointer, p_is_const, p_is_reference,\
                                                TYPE, c_is_const,\
                                                K, k_is_pointer, k_is_const, k_is_reference,\
                                                T, t_is_pointer, t_is_const, t_is_reference>\
{\
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
    typedef IntermediateAssociativeContainer<TYPE,K_content,T_content> IContainer;\
\
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){\
        if (!in)\
            return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, has_scope, NativeType>::convert(env, scope, nullptr);\
        if (ContainerAPI::test##TYPE<K_content,T_content>(env, in)) {\
            NativeType pointer = QtJambiAPI::convertJavaObjectToSmartPointer<Pointer,C_content>(env, in);\
            return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, has_scope, NativeType>::convert(env, scope, std::move(pointer));\
        } else {\
            NativeType pointer;\
            Container* map;\
            if(c_is_const){\
                map = new Container();\
                pointer.reset(map);\
            }else{\
                IContainer* imap = new IContainer(env, in, *scope);\
                map = imap;\
                pointer = Pointer<IContainer>(imap);\
            }\
            jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, in);\
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {\
                jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);\
                jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);\
                jobject val = QtJambiAPI::valueOfJavaMapEntry(env, entry);\
                map->insert(qtjambi_scoped_cast<has_scope,K_content,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T_content,jobject>::cast(env, val, nullptr, scope));\
            }\
            return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, has_scope, NativeType>::convert(env, scope, std::move(pointer));\
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
