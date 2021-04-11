/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_CAST_SHAREDPOINTER_P_H
#define QTJAMBI_CAST_SHAREDPOINTER_P_H

#include "qtjambi_global.h"
#include "qtjambi_cast_util_p.h"
#include "qtjambi_cast_list_p.h"
#include "qtjambi_cast_map_p.h"
#include "qtjambi_cast_container1_util_p.h"
#include "qtjambi_cast_container2_util_p.h"

QT_WARNING_DISABLE_DEPRECATED

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
struct qtjambi_shared_pointer_caster{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

//template from any QObject* Pointer<T> to jobject

template<bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_const>
struct qtjambi_shared_pointer_caster<true, has_scope,
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
        return qtjambi_from_qobject_pointer<Pointer,T_content>(env, deref_ptr<p_is_pointer, NativeType_c>::deref(in));
    }
};

template<bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_const>
struct qtjambi_shared_pointer_caster<false, has_scope,
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
        const NativeType* pointer = reinterpret_cast<const NativeType*>(qtjambi_to_qobject_from_shared_pointer(env, in, &createPointer<Pointer, T_content>, &deletePointer<Pointer, T_content>, &getQObjectFromPointer<Pointer, T_content>));
        return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope && p_is_pointer && !p_is_const, Pointer, T>::create(
                    env, scope, pointer, "Pointer<T>");
    }
};

//template from any non-QObject* Pointer<T> to jobject

template<bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_const>
struct qtjambi_shared_pointer_caster<true, has_scope,
                                    Pointer, p_is_pointer, p_is_const, p_is_reference,
                                    T, false, false, false, t_is_const, false, false, false>{
    typedef typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type T_content;
    typedef Pointer<T_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_object_pointer<Pointer,T_content>(env, deref_ptr<p_is_pointer, NativeType_c>::deref(in));
    }
};

template<bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_const>
struct qtjambi_shared_pointer_caster<false, has_scope,
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
        const NativeType* pointer = reinterpret_cast<const NativeType*>(qtjambi_to_object_from_shared_pointer(env, in, &createPointer<Pointer, T_content>, &deletePointer<Pointer, T_content>, &getFromPointer<Pointer, T_content>));
        return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope && p_is_pointer && !p_is_const, Pointer, T>::create(
                    env, scope, pointer, "Pointer<T>");
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
struct qtjambi_shared_pointer_caster<forward, has_scope,
                                    Pointer, p_is_pointer, p_is_const, p_is_reference,
                                    Container<Ts...>, false, false, false, t_is_const, false, false, true>
        :qtjambi_shared_pointer_container_cast_decider<forward, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference,
                                                        Container, t_is_const, sizeof... (Ts), Ts...>
{
};

// shared pointer to QList, QLinkedList, QSet etc

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container1_caster<true, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, QList, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<QList<T_content>>::type, QList<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_QList(env,
                                     new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                     deletePointer<Pointer,C_content>,
                                     getFromPointer<Pointer,C_content>,
                                     ListType::QList,
                                     QListAccess<T,c_is_const>::newInstance()
                                 );
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container1_caster<false, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, QList, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<QList<T_content>>::type, QList<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> pointer;
        if (in) {
            if (qtjambi_is_QList<T>(env, in)) {
                return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope && p_is_pointer && !p_is_const, Pointer, C_content>::create(env, scope,
                                                    reinterpret_cast<const NativeType *>(qtjambi_to_object_from_shared_pointer(env, in,
                                                                                                            createPointer<Pointer,C_content>,
                                                                                                            deletePointer<Pointer,C_content>,
                                                                                                            getFromPointer<Pointer,C_content>)),
                                                                                                            "Pointer<QList<T>>");
            } else {
                QList<T> *list = nullptr;
                if(c_is_const){
                    list = new QList<T>();
                    pointer.reset(new NativeType(list));
                }else{
                    list = new IntermediateContainer<QList,T>(env, in, *scope);
                    pointer.reset(new NativeType(list, [](void * ptr){ delete static_cast<IntermediateContainer<QList,T>*>(ptr); }));
                }
                jobject iterator = qtjambi_collection_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject element = qtjambi_iterator_next(env, iterator);
                    T  __qt_element = qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope);
                    list->append(__qt_element);
                }
            }
        }
        return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope, Pointer, C_content>::create(env, scope, pointer, "Pointer<QList<T>>");
    }
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#ifdef QLINKEDLIST_H
template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container1_caster<true, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, QLinkedList, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<QLinkedList<T_content>>::type, QLinkedList<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_QLinkedList(env,
                                     new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                     deletePointer<Pointer,C_content>,
                                     getFromPointer<Pointer,C_content>,
                                     QLinkedListAccess<T,c_is_const>::newInstance()
                                 );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container1_caster<false, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, QLinkedList, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<QLinkedList<T_content>>::type, QLinkedList<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> pointer;
        if (in) {
            if (qtjambi_is_QLinkedList<T>(env, in)) {
                return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope && p_is_pointer && !p_is_const, Pointer, C_content>::create(env, scope,
                                                        reinterpret_cast<const NativeType *>(qtjambi_to_object_from_shared_pointer(env, in,
                                                                                                            createPointer<Pointer,C_content>,
                                                                                                            deletePointer<Pointer,C_content>,
                                                                                                            getFromPointer<Pointer,C_content>)),
                                                                                                            "Pointer<QLinkedList<T>>");
            } else {
                QLinkedList<T> *list = nullptr;
                if(c_is_const){
                    list = new QLinkedList<T>();
                    pointer.reset(new NativeType(list));
                }else{
                    list = new IntermediateContainer<QLinkedList,T>(env, in, *scope);
                    pointer.reset(new NativeType(list, [](void * ptr){ delete static_cast<IntermediateContainer<QLinkedList,T>*>(ptr); }));
                }
                jobject iterator = qtjambi_collection_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject element = qtjambi_iterator_next(env, iterator);
                    T  __qt_element = qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope);
                    list->append(__qt_element);
                }
            }
        }
        return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope, Pointer, C_content>::create(env, scope, pointer, "Pointer<QLinkedList<T>>");
    }
};
#endif
#endif

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container1_caster<true, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, QSet, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<QSet<T_content>>::type, QSet<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_QSet(env,
                                     new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                     deletePointer<Pointer,C_content>,
                                     getFromPointer<Pointer,C_content>,
                                     QSetAccess<T,c_is_const>::newInstance()
                                 );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container1_caster<false, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, QSet, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<QSet<T_content>>::type, QSet<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> pointer;
        if (in) {
            if (qtjambi_is_QSet<T>(env, in)) {
                return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope && p_is_pointer && !p_is_const, Pointer, C_content>::create(env, scope,
                                                reinterpret_cast<const NativeType *>(qtjambi_to_object_from_shared_pointer(env, in,
                                                                                                            createPointer<Pointer,C_content>,
                                                                                                            deletePointer<Pointer,C_content>,
                                                                                                            getFromPointer<Pointer,C_content>)),
                                                                                                            "Pointer<QSet<T>>");
            } else {
                QSet<T> *list = nullptr;
                if(c_is_const){
                    list = new QSet<T>();
                    pointer.reset(new NativeType(list));
                }else{
                    list = new IntermediateContainer<QSet,T>(env, in, *scope);
                    pointer.reset(new NativeType(list, [](void * ptr){ delete static_cast<IntermediateContainer<QSet,T>*>(ptr); }));
                }
                jobject iterator = qtjambi_collection_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject element = qtjambi_iterator_next(env, iterator);
                    T  __qt_element = qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope);
                    list->insert(__qt_element);
                }
            }
        }
        return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope, Pointer, C_content>::create(env, scope, pointer, "Pointer<QSet<T>>");
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container1_caster<true, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, QStack, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef QStack<T_content> Container;
    typedef typename std::conditional<c_is_const, typename std::add_const<Container>::type, Container>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        return qtjambi_from_QVector(env,
                                     new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                     deletePointer<Pointer,C_content>,
                                     getFromPointer<Pointer,C_content>,
                                     VectorType::QStack,
                                     QVectorAccess<T,c_is_const>::newInstance()
                                 );
#else
        return qtjambi_from_QList(env,
                                     new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                     deletePointer<Pointer,C_content>,
                                     getFromPointer<Pointer,C_content>,
                                     ListType::QStack,
                                     QListAccess<T,c_is_const>::newInstance()
                                 );
#endif
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container1_caster<false, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, QStack, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<QStack<T_content>>::type, QStack<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(std::is_default_constructible<T>::value || std::is_copy_assignable<T>::value || std::is_move_assignable<T>::value, "Cannot cast to QStack<T> because T does not have a standard constructor.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> pointer;
        if (in) {
            if (qtjambi_is_QStack<T>(env, in)) {
                return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope && p_is_pointer && !p_is_const, Pointer, C_content>::create(env, scope,
                                            reinterpret_cast<const NativeType *>(qtjambi_to_object_from_shared_pointer(env, in,
                                                                                                            createPointer<Pointer,C_content>,
                                                                                                            deletePointer<Pointer,C_content>,
                                                                                                            getFromPointer<Pointer,C_content>)),
                                                                                                            "Pointer<QStack<T>>");
            } else {
                QStack<T> *list = nullptr;
                if(c_is_const){
                    list = new QStack<T>();
                    pointer.reset(new NativeType(list));
                }else{
                    list = new IntermediateContainer<QStack,T>(env, in, *scope);
                    pointer.reset(new NativeType(list, [](void * ptr){ delete static_cast<IntermediateContainer<QStack,T>*>(ptr); }));
                }
                jobject iterator = qtjambi_collection_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject element = qtjambi_iterator_next(env, iterator);
                    T  __qt_element = qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope);
                    list->append(__qt_element);
                }
            }
        }
        return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope, Pointer, C_content>::create(env, scope, pointer, "Pointer<QStack<T>>");
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container1_caster<true, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, QQueue, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef QQueue<T_content> Container;
    typedef typename std::conditional<c_is_const, typename std::add_const<Container>::type, Container>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_QList(env,
                                     new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                     deletePointer<Pointer,C_content>,
                                     getFromPointer<Pointer,C_content>,
                                     ListType::QQueue,
                                     QListAccess<T,c_is_const>::newInstance()
                                 );
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container1_caster<false, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, QQueue, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<QQueue<T_content>>::type, QQueue<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> pointer;
        if (in) {
            if (qtjambi_is_QQueue<T>(env, in)) {
                return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope && p_is_pointer && !p_is_const, Pointer, C_content>::create(env, scope,
                                                reinterpret_cast<const NativeType *>(qtjambi_to_object_from_shared_pointer(env, in,
                                                                                                            createPointer<Pointer,C_content>,
                                                                                                            deletePointer<Pointer,C_content>,
                                                                                                            getFromPointer<Pointer,C_content>)),
                                                                                                            "Pointer<QQueue<T>>");
            } else {
                QQueue<T> *list = nullptr;
                if(c_is_const){
                    list = new QQueue<T>();
                    pointer.reset(new NativeType(list));
                }else{
                    list = new IntermediateContainer<QQueue,T>(env, in, *scope);
                    pointer.reset(new NativeType(list, [](void * ptr){ delete static_cast<IntermediateContainer<QQueue,T>*>(ptr); }));
                }
                jobject iterator = qtjambi_collection_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject element = qtjambi_iterator_next(env, iterator);
                    T  __qt_element = qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope);
                    list->append(__qt_element);
                }
            }
        }
        return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope, Pointer, C_content>::create(env, scope, pointer, "Pointer<QQueue<T>>");
    }
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#ifdef QVECTOR_H
template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container1_caster<true, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, QVector, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef QVector<T_content> Container;
    typedef typename std::conditional<c_is_const, typename std::add_const<Container>::type, Container>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_QVector(env,
                                     new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                     deletePointer<Pointer,C_content>,
                                     getFromPointer<Pointer,C_content>,
                                     VectorType::QVector,
                                     QVectorAccess<T,c_is_const>::newInstance()
                                 );
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container1_caster<false, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, QVector, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<QVector<T_content>>::type, QVector<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(std::is_default_constructible<T>::value || std::is_copy_assignable<T>::value || std::is_move_assignable<T>::value, "Cannot cast to QVector<T> because T does not have a standard constructor.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> pointer;
        if (in) {
            if (qtjambi_is_QVector<T>(env, in)) {
                return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope && p_is_pointer && !p_is_const, Pointer, C_content>::create(env, scope,
                                                        reinterpret_cast<const NativeType *>(qtjambi_to_object_from_shared_pointer(env, in,
                                                                                                            createPointer<Pointer,C_content>,
                                                                                                            deletePointer<Pointer,C_content>,
                                                                                                            getFromPointer<Pointer,C_content>)),
                                                                                                            "Pointer<QVector<T>>");
            } else {
                QVector<T> *list = nullptr;
                if(c_is_const){
                    list = new QVector<T>();
                    pointer.reset(new NativeType(list));
                }else{
                    list = new IntermediateContainer<QVector,T>(env, in, *scope);
                    pointer.reset(new NativeType(list, [](void * ptr){ delete static_cast<IntermediateContainer<QVector,T>*>(ptr); }));
                }
                jobject iterator = qtjambi_collection_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject element = qtjambi_iterator_next(env, iterator);
                    T  __qt_element = qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope);
                    list->append(__qt_element);
                }
            }
        }
        return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope, Pointer, C_content>::create(env, scope, pointer, "Pointer<QVector<T>>");
    }
};
#endif
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container2_caster<true, has_scope,
                                                Pointer, p_is_pointer, p_is_const, p_is_reference,
                                                QMap, c_is_const,
                                                K, k_is_pointer, k_is_const, k_is_reference,
                                                T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<k_is_pointer, typename std::add_pointer<K>::type, K>::type K_ptr;
    typedef typename std::conditional<k_is_const, typename std::add_const<K_ptr>::type, K_ptr>::type K_const;
    typedef typename std::conditional<k_is_reference, typename std::add_lvalue_reference<K_const>::type, K_const>::type K_content;
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef QMap<K_content,T_content> Container;
    typedef typename std::conditional<c_is_const, typename std::add_const<Container>::type, Container>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_QMap(env,
                                     new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                     deletePointer<Pointer,C_content>,
                                     getFromPointer<Pointer,C_content>,
                                     QMapAccess<K,T,c_is_const>::newInstance()
                                 );
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container2_caster<false, has_scope,
                                                Pointer, p_is_pointer, p_is_const, p_is_reference,
                                                QMap, c_is_const,
                                                K, k_is_pointer, k_is_const, k_is_reference,
                                                T, t_is_pointer, t_is_const, t_is_reference>
{
    typedef typename std::conditional<k_is_pointer, typename std::add_pointer<K>::type, K>::type K_ptr;
    typedef typename std::conditional<k_is_const, typename std::add_const<K_ptr>::type, K_ptr>::type K_const;
    typedef typename std::conditional<k_is_reference, typename std::add_lvalue_reference<K_const>::type, K_const>::type K_content;
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<QMap<K_content,T_content>>::type, QMap<K_content,T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> pointer;
        if (in) {
            if (qtjambi_is_QMap<K,T>(env, in)) {
                return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope && p_is_pointer && !p_is_const, Pointer, C_content>::create(env, scope,
                                                    reinterpret_cast<const NativeType *>(qtjambi_to_object_from_shared_pointer(env, in,
                                                                                                            createPointer<Pointer,C_content>,
                                                                                                            deletePointer<Pointer,C_content>,
                                                                                                            getFromPointer<Pointer,C_content>)),
                                                                                                            "Pointer<QMap<K,T>>");
            } else {
                QMap<K,T> *map = nullptr;
                if(c_is_const){
                    map = new QMap<K,T>();
                    pointer.reset(new NativeType(map));
                }else{
                    map = new IntermediateBiContainer<QMap,K,T>(env, in, *scope);
                    pointer.reset(new NativeType(map, [](void * ptr){ delete static_cast<IntermediateBiContainer<QMap,K,T>*>(ptr); }));
                }
                jobject iterator = qtjambi_map_entryset_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject entry = qtjambi_iterator_next(env, iterator);
                    jobject key = qtjambi_map$entry_key(env, entry);
                    jobject val = qtjambi_map$entry_value(env, entry);
                    map->insert(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, val, nullptr, scope));
                }
            }
        }
        return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope, Pointer, C_content>::create(env, scope, pointer, "Pointer<QMap<K,T>>");
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container2_caster<true, has_scope,
                                                Pointer, p_is_pointer, p_is_const, p_is_reference,
                                                QMultiMap, c_is_const,
                                                K, k_is_pointer, k_is_const, k_is_reference,
                                                T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<k_is_pointer, typename std::add_pointer<K>::type, K>::type K_ptr;
    typedef typename std::conditional<k_is_const, typename std::add_const<K_ptr>::type, K_ptr>::type K_const;
    typedef typename std::conditional<k_is_reference, typename std::add_lvalue_reference<K_const>::type, K_const>::type K_content;
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef QMultiMap<K_content,T_content> Container;
    typedef typename std::conditional<c_is_const, typename std::add_const<Container>::type, Container>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_QMultiMap(env,
                                     new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                     deletePointer<Pointer,C_content>,
                                     getFromPointer<Pointer,C_content>,
                                     QMultiMapAccess<K,T,c_is_const>::newInstance()
                                 );
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container2_caster<false, has_scope,
                                                Pointer, p_is_pointer, p_is_const, p_is_reference,
                                                QMultiMap, c_is_const,
                                                K, k_is_pointer, k_is_const, k_is_reference,
                                                T, t_is_pointer, t_is_const, t_is_reference>
{
    typedef typename std::conditional<k_is_pointer, typename std::add_pointer<K>::type, K>::type K_ptr;
    typedef typename std::conditional<k_is_const, typename std::add_const<K_ptr>::type, K_ptr>::type K_const;
    typedef typename std::conditional<k_is_reference, typename std::add_lvalue_reference<K_const>::type, K_const>::type K_content;
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<QMultiMap<K_content,T_content>>::type, QMultiMap<K_content,T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> pointer;
        if (in) {
            if (qtjambi_is_QMultiMap<K,T>(env, in)) {
                return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope && p_is_pointer && !p_is_const, Pointer, C_content>::create(env, scope,
                                                    reinterpret_cast<const NativeType *>(qtjambi_to_object_from_shared_pointer(env, in,
                                                                                                            createPointer<Pointer,C_content>,
                                                                                                            deletePointer<Pointer,C_content>,
                                                                                                            getFromPointer<Pointer,C_content>)),
                                                                                                            "Pointer<QMultiMap<K,T>>");
            } else {
                QMultiMap<K,T> *map = nullptr;
                if(c_is_const){
                    map = new QMultiMap<K,T>();
                    pointer.reset(new NativeType(map));
                }else{
                    map = new IntermediateBiContainer<QMultiMap,K,T>(env, in, *scope);
                    pointer.reset(new NativeType(map, [](void * ptr){ delete static_cast<IntermediateBiContainer<QMultiMap,K,T>*>(ptr); }));
                }
                jobject iterator = qtjambi_map_entryset_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject entry = qtjambi_iterator_next(env, iterator);
                    jobject key = qtjambi_map$entry_key(env, entry);
                    jobject val = qtjambi_map$entry_value(env, entry);
                    map->insert(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, val, nullptr, scope));
                }
            }
        }
        return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope, Pointer, C_content>::create(env, scope, pointer, "Pointer<QMultiMap<K,T>>");
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container2_caster<true, has_scope,
                                                Pointer, p_is_pointer, p_is_const, p_is_reference,
                                                QHash, c_is_const,
                                                K, k_is_pointer, k_is_const, k_is_reference,
                                                T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<k_is_pointer, typename std::add_pointer<K>::type, K>::type K_ptr;
    typedef typename std::conditional<k_is_const, typename std::add_const<K_ptr>::type, K_ptr>::type K_const;
    typedef typename std::conditional<k_is_reference, typename std::add_lvalue_reference<K_const>::type, K_const>::type K_content;
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef QHash<K_content,T_content> Container;
    typedef typename std::conditional<c_is_const, typename std::add_const<Container>::type, Container>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_QHash(env,
                                     new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                     deletePointer<Pointer,C_content>,
                                     getFromPointer<Pointer,C_content>,
                                     QHashAccess<K,T,c_is_const>::newInstance()
                                 );
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container2_caster<false, has_scope,
                                                Pointer, p_is_pointer, p_is_const, p_is_reference,
                                                QHash, c_is_const,
                                                K, k_is_pointer, k_is_const, k_is_reference,
                                                T, t_is_pointer, t_is_const, t_is_reference>
{
    typedef typename std::conditional<k_is_pointer, typename std::add_pointer<K>::type, K>::type K_ptr;
    typedef typename std::conditional<k_is_const, typename std::add_const<K_ptr>::type, K_ptr>::type K_const;
    typedef typename std::conditional<k_is_reference, typename std::add_lvalue_reference<K_const>::type, K_const>::type K_content;
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<QHash<K_content,T_content>>::type, QHash<K_content,T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> pointer;
        if (in) {
            if (qtjambi_is_QHash<K,T>(env, in)) {
                return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope && p_is_pointer && !p_is_const, Pointer, C_content>::create(env, scope,
                                                            reinterpret_cast<const NativeType *>(qtjambi_to_object_from_shared_pointer(env, in,
                                                                                                            createPointer<Pointer,C_content>,
                                                                                                            deletePointer<Pointer,C_content>,
                                                                                                            getFromPointer<Pointer,C_content>)),
                                                                                                            "Pointer<QHash<K,T>>");
            } else {
                QHash<K,T> *map = nullptr;
                if(c_is_const){
                    map = new QHash<K,T>();
                    pointer.reset(new NativeType(map));
                }else{
                    map = new IntermediateBiContainer<QHash,K,T>(env, in, *scope);
                    pointer.reset(new NativeType(map, [](void * ptr){ delete static_cast<IntermediateBiContainer<QHash,K,T>*>(ptr); }));
                }
                jobject iterator = qtjambi_map_entryset_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject entry = qtjambi_iterator_next(env, iterator);
                    jobject key = qtjambi_map$entry_key(env, entry);
                    jobject val = qtjambi_map$entry_value(env, entry);
                    map->insert(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, val, nullptr, scope));
                }
            }
        }
        return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope, Pointer, C_content>::create(env, scope, pointer, "Pointer<QHash<K,T>>");
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container2_caster<true, has_scope,
                                                Pointer, p_is_pointer, p_is_const, p_is_reference,
                                                QMultiHash, c_is_const,
                                                K, k_is_pointer, k_is_const, k_is_reference,
                                                T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<k_is_pointer, typename std::add_pointer<K>::type, K>::type K_ptr;
    typedef typename std::conditional<k_is_const, typename std::add_const<K_ptr>::type, K_ptr>::type K_const;
    typedef typename std::conditional<k_is_reference, typename std::add_lvalue_reference<K_const>::type, K_const>::type K_content;
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef QMultiHash<K_content,T_content> Container;
    typedef typename std::conditional<c_is_const, typename std::add_const<Container>::type, Container>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_QMultiHash(env,
                                     new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                     deletePointer<Pointer,C_content>,
                                     getFromPointer<Pointer,C_content>,
                                     QMultiHashAccess<K,T,c_is_const>::newInstance()
                                 );
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container2_caster<false, has_scope,
                                                Pointer, p_is_pointer, p_is_const, p_is_reference,
                                                QMultiHash, c_is_const,
                                                K, k_is_pointer, k_is_const, k_is_reference,
                                                T, t_is_pointer, t_is_const, t_is_reference>
{
    typedef typename std::conditional<k_is_pointer, typename std::add_pointer<K>::type, K>::type K_ptr;
    typedef typename std::conditional<k_is_const, typename std::add_const<K_ptr>::type, K_ptr>::type K_const;
    typedef typename std::conditional<k_is_reference, typename std::add_lvalue_reference<K_const>::type, K_const>::type K_content;
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<QMultiHash<K_content,T_content>>::type, QMultiHash<K_content,T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> pointer;
        if (in) {
            if (qtjambi_is_QMultiHash<K,T>(env, in)) {
                return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope && p_is_pointer && !p_is_const, Pointer, C_content>::create(env, scope,
                                                        reinterpret_cast<const NativeType *>(qtjambi_to_object_from_shared_pointer(env, in,
                                                                                                            createPointer<Pointer,C_content>,
                                                                                                            deletePointer<Pointer,C_content>,
                                                                                                            getFromPointer<Pointer,C_content>)),
                                                                                                            "Pointer<QMultiHash<K,T>>");
            } else {
                QMultiHash<K,T> *map = nullptr;
                if(c_is_const){
                    map = new QMultiHash<K,T>();
                    pointer.reset(new NativeType(map));
                }else{
                    map = new IntermediateBiContainer<QMultiHash,K,T>(env, in, *scope);
                    pointer.reset(new NativeType(map, [](void * ptr){ delete static_cast<IntermediateBiContainer<QMultiHash,K,T>*>(ptr); }));
                }
                jobject iterator = qtjambi_map_entryset_iterator(env, in);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject entry = qtjambi_iterator_next(env, iterator);
                    jobject key = qtjambi_map$entry_key(env, entry);
                    jobject val = qtjambi_map$entry_value(env, entry);
                    map->insert(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, val, nullptr, scope));
                }
            }
        }
        return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope, Pointer, C_content>::create(env, scope, pointer, "Pointer<QMultiHash<K,T>>");
    }
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_SHAREDPOINTER_P_H
