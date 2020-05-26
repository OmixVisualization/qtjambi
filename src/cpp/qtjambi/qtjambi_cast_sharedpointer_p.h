/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "qtjambi_core.h"
#include "qtjambi_cast_util_p.h"
#include "qtjambi_cast_list_p.h"
#include "qtjambi_cast_map_p.h"

namespace QtJambiPrivate {

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QList_shared_pointer_caster{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<QList<T_content>>::type, QList<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !c_is_const, "Cannot cast types");
        return nullptr;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QSet_shared_pointer_caster{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<QSet<T_content>>::type, QSet<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !c_is_const, "Cannot cast types");
        return nullptr;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QLinkedList_shared_pointer_caster{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<QLinkedList<T_content>>::type, QLinkedList<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !c_is_const, "Cannot cast types");
        return nullptr;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QStack_shared_pointer_caster{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<QStack<T_content>>::type, QStack<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !c_is_const, "Cannot cast types");
        return nullptr;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QQueue_shared_pointer_caster{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<QQueue<T_content>>::type, QQueue<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !c_is_const, "Cannot cast types");
        return nullptr;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QVector_shared_pointer_caster{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<QVector<T_content>>::type, QVector<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !c_is_const, "Cannot cast types");
        return nullptr;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QMap_shared_pointer_caster{
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
    static jobject castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !c_is_const, "Cannot cast types");
        return nullptr;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QMultiMap_shared_pointer_caster{
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
    static jobject castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !c_is_const, "Cannot cast types");
        return nullptr;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QHash_shared_pointer_caster{
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
    static jobject castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !c_is_const, "Cannot cast types");
        return nullptr;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QMultiHash_shared_pointer_caster{
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
    static jobject castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !c_is_const, "Cannot cast types");
        return nullptr;
    }
};

template<bool forward, bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_arithmetic, bool t_is_enum, bool t_is_pointer, bool t_is_const, bool t_is_function, bool t_is_qobject, bool t_is_template>
struct qtjambi_shared_pointer_caster{
    typedef typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type T_c;
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T_c>::type, T_c>::type T_content;
    typedef Pointer<T_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
        return nullptr;
    }
    static NativeType_out castbck(JNIEnv *, jobject, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
        return NativeType();
    }
};

//template from any QObject* Pointer<T> to jobject

template<bool forward, bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_const>
struct qtjambi_shared_pointer_caster<forward, has_scope,
                                    Pointer, p_is_pointer, p_is_const, p_is_reference,
                                    T, false, false, false, t_is_const, false, true, false>{
    typedef typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type T_c;
    typedef T_c T_content;
    typedef Pointer<T_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qobject_pointer<Pointer,T_content>(env, deref_ptr<p_is_pointer, NativeType_c>::deref(in));
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!(p_is_reference && !p_is_const), "Cannot cast to Pointer<T> &");
        const NativeType* pointer = reinterpret_cast<const NativeType*>(qtjambi_to_qobject_from_shared_pointer(env, in, &createPointer<Pointer, T_content>, &deletePointer<Pointer, T_content>, &getQObjectFromPointer<Pointer, T_content>));
        return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope && p_is_pointer && !p_is_const, Pointer, T>::create(
                    env, scope, pointer, "Pointer<T>");
    }
};

//template from any non-QObject* Pointer<T> to jobject

template<bool forward, bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_const>
struct qtjambi_shared_pointer_caster<forward, has_scope,
                                    Pointer, p_is_pointer, p_is_const, p_is_reference,
                                    T, false, false, false, t_is_const, false, false, false>{
    typedef typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type T_content;
    typedef Pointer<T_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_object_pointer<Pointer,T_content>(env, deref_ptr<p_is_pointer, NativeType_c>::deref(in));
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!(p_is_reference && !p_is_const), "Cannot cast to Pointer<T> &");
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
    typedef typename std::conditional<c_is_const, typename std::add_const<Container<Ts...>>::type, Container<Ts...>>::type T_content;
    typedef Pointer<T_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
        return nullptr;
    }
    static NativeType_out castbck(JNIEnv *, jobject, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
        return NativeType();
    }
};

template<bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         template<typename T> class Container, bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container1_caster{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<Container<T_content>>::type, Container<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
        return nullptr;
    }
    static NativeType_out castbck(JNIEnv *, jobject, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
        return NativeType();
    }
};

template<bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         template<typename K, typename T> class Container, bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container2_caster{
    typedef typename std::conditional<k_is_pointer, typename std::add_pointer<K>::type, K>::type K_ptr;
    typedef typename std::conditional<k_is_const, typename std::add_const<K_ptr>::type, K_ptr>::type K_const;
    typedef typename std::conditional<k_is_reference, typename std::add_lvalue_reference<K_const>::type, K_const>::type K_content;
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::conditional<c_is_const, typename std::add_const<Container<K_content,T_content>>::type, Container<K_content,T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
        return nullptr;
    }
    static NativeType_out castbck(JNIEnv *, jobject, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
        return NativeType();
    }
};

template<bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         template<typename...Ts> class Container, bool c_is_const, typename...Ts>
struct qtjambi_shared_pointer_container_cast_decider<true, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, Container, c_is_const, 1, Ts...>{
    typedef typename std::conditional<c_is_const, typename std::add_const<Container<Ts...>>::type, Container<Ts...>>::type T_content;
    typedef Pointer<T_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    typedef jobject (*CastFwdReturn)(JNIEnv *, NativeType_in, const char*, QtJambiScope*);
    template<template<typename T> class _Container, typename T>
    static constexpr CastFwdReturn _castfwd(){
        return qtjambi_shared_pointer_container1_caster<
                has_scope,
                Pointer, p_is_pointer, p_is_const, p_is_reference,
                _Container, c_is_const,
                typename std::remove_pointer<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::type,
                std::is_pointer<T>::value,
                std::is_const<T>::value,
                std::is_reference<T>::value
            >::castfwd;
    }

    static constexpr jobject (*castfwd)(JNIEnv *, NativeType_in, const char*, QtJambiScope*) = _castfwd<Container, Ts...>();
};

template<bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         template<typename...Ts> class Container, bool c_is_const, typename...Ts>
struct qtjambi_shared_pointer_container_cast_decider<false, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, Container, c_is_const, 1, Ts...>{
    typedef typename std::conditional<c_is_const, typename std::add_const<Container<Ts...>>::type, Container<Ts...>>::type T_content;
    typedef Pointer<T_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    typedef NativeType_out (*CastBckReturn)(JNIEnv *, jobject, const char*, QtJambiScope*);
    template<template<typename T> class _Container, typename T>
    static constexpr CastBckReturn _castbck(){
        return qtjambi_shared_pointer_container1_caster<
                has_scope,
                Pointer, p_is_pointer, p_is_const, p_is_reference,
                _Container, c_is_const,
                typename std::remove_pointer<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::type,
                std::is_pointer<T>::value,
                std::is_const<T>::value,
                std::is_reference<T>::value
            >::castbck;
    }

    static constexpr NativeType_out (*castbck)(JNIEnv *, jobject, const char*, QtJambiScope*) = _castbck<Container, Ts...>();
};

template<bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         template<typename...Ts> class Container, bool c_is_const, typename...Ts>
struct qtjambi_shared_pointer_container_cast_decider<true, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, Container, c_is_const, 2, Ts...>{
    typedef typename std::conditional<c_is_const, typename std::add_const<Container<Ts...>>::type, Container<Ts...>>::type T_content;
    typedef Pointer<T_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    typedef jobject (*CastFwdReturn)(JNIEnv *, NativeType_in, const char*, QtJambiScope*);
    template<template<typename K, typename T> class _Container, typename K, typename T>
    static constexpr CastFwdReturn _castfwd(){
        return qtjambi_shared_pointer_container2_caster<
                has_scope,
                Pointer, p_is_pointer, p_is_const, p_is_reference,
                _Container, c_is_const,
                typename std::remove_pointer<typename std::remove_cv<typename std::remove_reference<K>::type>::type>::type,
                std::is_pointer<K>::value,
                std::is_const<K>::value,
                std::is_reference<K>::value,
                typename std::remove_pointer<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::type,
                std::is_pointer<T>::value,
                std::is_const<T>::value,
                std::is_reference<T>::value
            >::castfwd;
    }

    static constexpr jobject (*castfwd)(JNIEnv *, NativeType_in, const char*, QtJambiScope*) = _castfwd<Container, Ts...>();
};

template<bool has_scope,
         template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         template<typename...Ts> class Container, bool c_is_const, typename...Ts>
struct qtjambi_shared_pointer_container_cast_decider<false, has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, Container, c_is_const, 2, Ts...>{
    typedef typename std::conditional<c_is_const, typename std::add_const<Container<Ts...>>::type, Container<Ts...>>::type T_content;
    typedef Pointer<T_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    typedef NativeType_out (*CastBckReturn)(JNIEnv *, jobject, const char*, QtJambiScope*);
    template<template<typename K, typename T> class _Container, typename K, typename T>
    static constexpr CastBckReturn _castbck(){
        return qtjambi_shared_pointer_container2_caster<
                has_scope,
                Pointer, p_is_pointer, p_is_const, p_is_reference,
                _Container, c_is_const,
                typename std::remove_pointer<typename std::remove_cv<typename std::remove_reference<K>::type>::type>::type,
                std::is_pointer<K>::value,
                std::is_const<K>::value,
                std::is_reference<K>::value,
                typename std::remove_pointer<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::type,
                std::is_pointer<T>::value,
                std::is_const<T>::value,
                std::is_reference<T>::value
            >::castbck;
    }

    static constexpr NativeType_out (*castbck)(JNIEnv *, jobject, const char*, QtJambiScope*) = _castbck<Container, Ts...>();
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
struct qtjambi_shared_pointer_container1_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, QList, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
        : qtjambi_QList_shared_pointer_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
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
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> pointer;
        if (in) {
            if (qtjambi_is_QList(env, in, qtjambi_type<T>::id())) {
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
                    T  __qt_element = qtjambi_cast<T>(env, element);
                    list->append(__qt_element);
                }
            }
        }
        return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope, Pointer, C_content>::create(env, scope, pointer, "Pointer<QList<T>>");
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container1_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, QSet, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
        : qtjambi_QSet_shared_pointer_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
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
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> pointer;
        if (in) {
            if (qtjambi_is_QSet(env, in, qtjambi_type<T>::id())) {
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
                    T  __qt_element = qtjambi_cast<T>(env, element);
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
struct qtjambi_shared_pointer_container1_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, QLinkedList, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
        : qtjambi_QLinkedList_shared_pointer_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
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
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> pointer;
        if (in) {
            if (qtjambi_is_QLinkedList(env, in, qtjambi_type<T>::id())) {
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
                    T  __qt_element = qtjambi_cast<T>(env, element);
                    list->append(__qt_element);
                }
            }
        }
        return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope, Pointer, C_content>::create(env, scope, pointer, "Pointer<QLinkedList<T>>");
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container1_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, QStack, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
        : qtjambi_QStack_shared_pointer_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
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
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(supports_StandardConstructor<T>::value, "Cannot cast to QStack<T> because T does not have a standard constructor.");
        QScopedPointer<NativeType> pointer;
        if (in) {
            if (qtjambi_is_QStack(env, in, qtjambi_type<T>::id())) {
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
                    T  __qt_element = qtjambi_cast<T>(env, element);
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
struct qtjambi_shared_pointer_container1_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, QQueue, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
        : qtjambi_QQueue_shared_pointer_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
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
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> pointer;
        if (in) {
            if (qtjambi_is_QQueue(env, in, qtjambi_type<T>::id())) {
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
                    T  __qt_element = qtjambi_cast<T>(env, element);
                    list->append(__qt_element);
                }
            }
        }
        return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope, Pointer, C_content>::create(env, scope, pointer, "Pointer<QQueue<T>>");
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container1_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, QVector, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
        : qtjambi_QVector_shared_pointer_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, c_is_const, T, t_is_pointer, t_is_const, t_is_reference>
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
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(supports_StandardConstructor<T>::value, "Cannot cast to QVector<T> because T does not have a standard constructor.");
        QScopedPointer<NativeType> pointer;
        if (in) {
            if (qtjambi_is_QVector(env, in, qtjambi_type<T>::id())) {
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
                    T  __qt_element = qtjambi_cast<T>(env, element);
                    list->append(__qt_element);
                }
            }
        }
        return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope, Pointer, C_content>::create(env, scope, pointer, "Pointer<QVector<T>>");
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_shared_pointer_container2_caster<has_scope,
                                                Pointer, p_is_pointer, p_is_const, p_is_reference,
                                                QMap, c_is_const,
                                                K, k_is_pointer, k_is_const, k_is_reference,
                                                T, t_is_pointer, t_is_const, t_is_reference>
        :qtjambi_QMap_shared_pointer_caster<has_scope,
                                            Pointer, p_is_pointer, p_is_const, p_is_reference,
                                            c_is_const,
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

    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> pointer;
        if (in) {
            if (qtjambi_is_QMap(env, in, qtjambi_type<K>::id(), qtjambi_type<T>::id())) {
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
                    map->insert(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, val));
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
struct qtjambi_shared_pointer_container2_caster<has_scope,
                                                Pointer, p_is_pointer, p_is_const, p_is_reference,
                                                QMultiMap, c_is_const,
                                                K, k_is_pointer, k_is_const, k_is_reference,
                                                T, t_is_pointer, t_is_const, t_is_reference>
        : qtjambi_QMultiMap_shared_pointer_caster<has_scope,
                                                    Pointer, p_is_pointer, p_is_const, p_is_reference,
                                                    c_is_const,
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

    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> pointer;
        if (in) {
            if (qtjambi_is_QMultiMap(env, in, qtjambi_type<K>::id(), qtjambi_type<T>::id())) {
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
                    map->insert(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, val));
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
struct qtjambi_shared_pointer_container2_caster<has_scope,
                                                Pointer, p_is_pointer, p_is_const, p_is_reference,
                                                QHash, c_is_const,
                                                K, k_is_pointer, k_is_const, k_is_reference,
                                                T, t_is_pointer, t_is_const, t_is_reference>
        : qtjambi_QHash_shared_pointer_caster<has_scope,
                                            Pointer, p_is_pointer, p_is_const, p_is_reference,
                                            c_is_const,
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

    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> pointer;
        if (in) {
            if (qtjambi_is_QHash(env, in, qtjambi_type<K>::id(), qtjambi_type<T>::id())) {
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
                    map->insert(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, val));
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
struct qtjambi_shared_pointer_container2_caster<has_scope,
                                                Pointer, p_is_pointer, p_is_const, p_is_reference,
                                                QMultiHash, c_is_const,
                                                K, k_is_pointer, k_is_const, k_is_reference,
                                                T, t_is_pointer, t_is_const, t_is_reference>
        : qtjambi_QMultiHash_shared_pointer_caster<has_scope,
                                                    Pointer, p_is_pointer, p_is_const, p_is_reference,
                                                    c_is_const,
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

    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> pointer;
        if (in) {
            if (qtjambi_is_QMultiHash(env, in, qtjambi_type<K>::id(), qtjambi_type<T>::id())) {
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
                    map->insert(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, val));
                }
            }
        }
        return create_container_pointer<p_is_pointer, p_is_const, p_is_reference, has_scope, Pointer, C_content>::create(env, scope, pointer, "Pointer<QMultiHash<K,T>>");
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QList_shared_pointer_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, true, T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::add_const<QList<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_constQList_shared_pointer(env,
                                     new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                     deletePointer<Pointer,C_content>,
                                     getFromPointer<Pointer,C_content>,
                                     typeid(T),
                                     ContainerAt<QList, T_content>::function(),
                                     ContainerBegin<QList, T_content>::function(),
                                     ContainerContains<QList, T_content>::function(),
                                     ContainerCountObject<QList, T_content>::function(),
                                     ContainerEnd<QList, T_content>::function(),
                                     ContainerEndsWith<QList, T_content>::function(),
                                     ContainerIndexOf<QList, T_content>::function(),
                                     ContainerLastIndexOf<QList, T_content>::function(),
                                     ContainerMid<QList, T_content>::function(),
                                     ContainerEquals<QList, T_content, qtjambi_is_QList>::function(),
                                     ContainerSize<QList, T_content>::function(),
                                     ContainerStartsWith<QList, T_content>::function(),
                                     ContainerToSet<QList, T>::function(),
                                     ContainerValue<QList, T, supports_StandardConstructor<T_content>::value>::function(),
                                     ContainerValueDefault<QList, T_content>::function()
                                 );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QList_shared_pointer_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, false, T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef QList<T_content> C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_QList_shared_pointer(env,
                                new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                deletePointer<Pointer,C_content>,
                                getFromPointer<Pointer,C_content>,
                                typeid(T),
                                ContainerAppend<QList, T_content>::function(),
                                ContainerAppendList<QList, T_content, qtjambi_is_QList>::function(),
                                ContainerAt<QList, T_content>::function(),
                                ContainerBegin<QList, T_content>::function(),
                                ContainerClear<QList, T_content>::function(),
                                ContainerContains<QList, T_content>::function(),
                                ContainerCountObject<QList, T_content>::function(),
                                ContainerEnd<QList, T_content>::function(),
                                ContainerEndsWith<QList, T_content>::function(),
                                ContainerIndexOf<QList, T_content>::function(),
                                ContainerInsertAt<QList, T_content>::function(),
                                ContainerLastIndexOf<QList, T_content>::function(),
                                ContainerMid<QList, T_content>::function(),
                                ContainerMove<QList, T_content>::function(),
                                ContainerEquals<QList, T_content, qtjambi_is_QList>::function(),
                                ContainerPrepend<QList, T_content>::function(),
                                ContainerRemoveAll<QList, T_content>::function(),
                                ContainerRemoveAt<QList, T_content>::function(),
                                ContainerRemoveOne<QList, T_content>::function(),
                                ContainerReplace<QList, T_content>::function(),
                                ContainerReserve<QList, T_content>::function(),
                                ContainerSize<QList, T_content>::function(),
                                ContainerStartsWith<QList, T_content>::function(),
                                ContainerSwap<QList, T_content>::function(),
                                ContainerTakeAt<QList, T_content>::function(),
                                ContainerToSet<QList, T>::function(),
                                ContainerValue<QList, T, supports_StandardConstructor<T_content>::value>::function(),
                                ContainerValueDefault<QList, T_content>::function()
                );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QSet_shared_pointer_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, true, T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::add_const<QSet<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_constQSet_shared_pointer(env,
                                     new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                     deletePointer<Pointer,C_content>,
                                     getFromPointer<Pointer,C_content>,
                                     typeid(T),
                                     ContainerBegin<QSet, T_content>::function(),
                                     ContainerCapacity<QSet, T_content>::function(),
                                     ContainerContains<QSet, T_content>::function(),
                                     ContainerEnd<QSet, T_content>::function(),
                                     ContainerIntersects<QSet, T_content, qtjambi_is_QSet>::function(),
                                     ContainerEquals<QSet, T_content, qtjambi_is_QSet>::function(),
                                     ContainerSize<QSet, T_content>::function(),
                                     ContainerValues<QSet, T_content>::function()
                                 );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QSet_shared_pointer_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, false, T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef QSet<T_content> C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_QSet_shared_pointer(env,
                                new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                deletePointer<Pointer,C_content>,
                                getFromPointer<Pointer,C_content>,
                                typeid(T),
                                QSetBeginFunction(ContainerBegin<QSet, T_content>::function()),
                                QSetCapacityFunction(ContainerCapacity<QSet, T_content>::function()),
                                QSetClearFunction(ContainerClear<QSet, T_content>::function()),
                                QSetContainsFunction(ContainerContains<QSet, T_content>::function()),
                                QSetEndFunction(ContainerEnd<QSet, T_content>::function()),
                                QSetInsertFunction(ContainerInsert<QSet, T_content>::function()),
                                QSetIntersectFunction(ContainerIntersect<QSet, T_content, qtjambi_is_QSet>::function()),
                                QSetIntersectsFunction(ContainerIntersects<QSet, T_content, qtjambi_is_QSet>::function()),
                                QSetEqualFunction(ContainerEquals<QSet, T_content, qtjambi_is_QSet>::function()),
                                QSetRemoveFunction(ContainerRemove<QSet, T_content>::function()),
                                QSetReserveFunction(ContainerReserve<QSet, T_content>::function()),
                                QSetSizeFunction(ContainerSize<QSet, T_content>::function()),
                                QSetSubtractFunction(ContainerSubtract<QSet, T_content, qtjambi_is_QSet>::function()),
                                QSetUniteFunction(ContainerUnite<QSet, T_content, qtjambi_is_QSet>::function()),
                                QSetValuesFunction(ContainerValues<QSet, T_content>::function())

                );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QLinkedList_shared_pointer_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, true, T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::add_const<QLinkedList<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_constQLinkedList_shared_pointer(env,
                                     new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                     deletePointer<Pointer,C_content>,
                                     getFromPointer<Pointer,C_content>,
                                     typeid(T),
                                     ContainerBegin<QLinkedList, T_content>::function(),
                                     ContainerContains<QLinkedList, T_content>::function(),
                                     ContainerCountObject<QLinkedList, T_content>::function(),
                                     ContainerEnd<QLinkedList, T_content>::function(),
                                     ContainerEndsWith<QLinkedList, T_content>::function(),
                                     ContainerFirst<QLinkedList, T_content>::function(),
                                     ContainerLast<QLinkedList, T_content>::function(),
                                     ContainerEquals<QLinkedList, T_content, qtjambi_is_QLinkedList>::function(),
                                     ContainerSize<QLinkedList, T_content>::function(),
                                     ContainerStartsWith<QLinkedList, T_content>::function()
                                 );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QLinkedList_shared_pointer_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, false, T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef QLinkedList<T_content> C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_QLinkedList_shared_pointer(env,
                                new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                deletePointer<Pointer,C_content>,
                                getFromPointer<Pointer,C_content>,
                                typeid(T),
                                QLinkedListAppendFunction(ContainerAppend<QLinkedList, T_content>::function()),
                                QLinkedListBeginFunction(ContainerBegin<QLinkedList, T_content>::function()),
                                QLinkedListClearFunction(ContainerClear<QLinkedList, T_content>::function()),
                                QLinkedListContainsFunction(ContainerContains<QLinkedList, T_content>::function()),
                                QLinkedListCountObjectFunction(ContainerCountObject<QLinkedList, T_content>::function()),
                                QLinkedListEndFunction(ContainerEnd<QLinkedList, T_content>::function()),
                                QLinkedListEndsWithFunction(ContainerEndsWith<QLinkedList, T_content>::function()),
                                QLinkedListFirstFunction(ContainerFirst<QLinkedList, T_content>::function()),
                                QLinkedListLastFunction(ContainerLast<QLinkedList, T_content>::function()),
                                QLinkedListEqualFunction(ContainerEquals<QLinkedList, T_content, qtjambi_is_QLinkedList>::function()),
                                QLinkedListPrependFunction(ContainerPrepend<QLinkedList, T_content>::function()),
                                QLinkedListRemoveAllFunction(ContainerRemoveAll<QLinkedList, T_content>::function()),
                                QLinkedListRemoveFirstFunction(ContainerRemoveFirst<QLinkedList, T_content>::function()),
                                QLinkedListRemoveLastFunction(ContainerRemoveLast<QLinkedList, T_content>::function()),
                                QLinkedListRemoveOneFunction(ContainerRemoveOne<QLinkedList, T_content>::function()),
                                QLinkedListSizeFunction(ContainerSize<QLinkedList, T_content>::function()),
                                QLinkedListStartsWithFunction(ContainerStartsWith<QLinkedList, T_content>::function()),
                                QLinkedListTakeFirstFunction(ContainerTakeFirst<QLinkedList, T_content>::function()),
                                QLinkedListTakeLastFunction(ContainerTakeLast<QLinkedList, T_content>::function())
                );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QQueue_shared_pointer_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, true, T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::add_const<QQueue<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_constQQueue_shared_pointer(env,
                                     new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                     deletePointer<Pointer,C_content>,
                                     getFromPointer<Pointer,C_content>,
                                     typeid(T),
                                     ContainerAt<QQueue, T_content>::function(),
                                     ContainerBegin<QQueue, T_content>::function(),
                                     ContainerContains<QQueue, T_content>::function(),
                                     ContainerCountObject<QQueue, T_content>::function(),
                                     ContainerEnd<QQueue, T_content>::function(),
                                     ContainerEndsWith<QQueue, T_content>::function(),
                                     ContainerIndexOf<QQueue, T_content>::function(),
                                     ContainerLastIndexOf<QQueue, T_content>::function(),
                                     ContainerMid<QQueue, T_content>::function(),
                                     ContainerEquals<QQueue, T_content, qtjambi_is_QQueue>::function(),
                                     ContainerSize<QQueue, T_content>::function(),
                                     ContainerStartsWith<QQueue, T_content>::function(),
                                     ContainerToSet<QQueue, T>::function(),
                                     ContainerValue<QQueue, T, supports_StandardConstructor<T_content>::value>::function(),
                                     ContainerValueDefault<QQueue, T_content>::function()
                                 );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QQueue_shared_pointer_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, false, T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef QQueue<T_content> C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_QQueue_shared_pointer(env,
                                new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                deletePointer<Pointer,C_content>,
                                getFromPointer<Pointer,C_content>,
                                typeid(T),
                                ContainerAppend<QQueue, T_content>::function(),
                                ContainerAppendList<QQueue, T_content, qtjambi_is_QQueue>::function(),
                                ContainerAt<QQueue, T_content>::function(),
                                ContainerBegin<QQueue, T_content>::function(),
                                ContainerClear<QQueue, T_content>::function(),
                                ContainerContains<QQueue, T_content>::function(),
                                ContainerCountObject<QQueue, T_content>::function(),
                                ContainerEnd<QQueue, T_content>::function(),
                                ContainerEndsWith<QQueue, T_content>::function(),
                                ContainerIndexOf<QQueue, T_content>::function(),
                                ContainerInsertAt<QQueue, T_content>::function(),
                                ContainerLastIndexOf<QQueue, T_content>::function(),
                                ContainerMid<QQueue, T_content>::function(),
                                ContainerMove<QQueue, T_content>::function(),
                                ContainerEquals<QQueue, T_content, qtjambi_is_QQueue>::function(),
                                ContainerPrepend<QQueue, T_content>::function(),
                                ContainerRemoveAll<QQueue, T_content>::function(),
                                ContainerRemoveAt<QQueue, T_content>::function(),
                                ContainerRemoveOne<QQueue, T_content>::function(),
                                ContainerReplace<QQueue, T_content>::function(),
                                ContainerReserve<QQueue, T_content>::function(),
                                ContainerSize<QQueue, T_content>::function(),
                                ContainerStartsWith<QQueue, T_content>::function(),
                                ContainerSwap<QQueue, T_content>::function(),
                                ContainerTakeAt<QQueue, T_content>::function(),
                                ContainerToSet<QQueue, T>::function(),
                                ContainerValue<QQueue, T, supports_StandardConstructor<T_content>::value>::function(),
                                ContainerValueDefault<QQueue, T_content>::function()
                );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QVector_shared_pointer_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, true, T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::add_const<QVector<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_constQVector_shared_pointer(env,
                                     new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                     deletePointer<Pointer,C_content>,
                                     getFromPointer<Pointer,C_content>,
                                     typeid(T),
                                     ContainerAt<QVector, T_content>::function(),
                                     ContainerBegin<QVector, T_content>::function(),
                                     ContainerCapacity<QVector, T_content>::function(),
                                     ContainerContains<QVector, T_content>::function(),
                                     ContainerCountObject<QVector, T_content>::function(),
                                     ContainerEnd<QVector, T_content>::function(),
                                     ContainerEndsWith<QVector, T_content>::function(),
                                     ContainerIndexOf<QVector, T_content>::function(),
                                     ContainerLastIndexOf<QVector, T_content>::function(),
                                     ContainerMid<QVector, T_content>::function(),
                                     ContainerEquals<QVector, T_content, qtjambi_is_QVector>::function(),
                                     ContainerSize<QVector, T_content>::function(),
                                     ContainerStartsWith<QVector, T_content>::function(),
                                     ContainerToList<QVector, T_content>::function(),
                                     ContainerValue<QVector, T, supports_StandardConstructor<T_content>::value>::function(),
                                     ContainerValueDefault<QVector, T_content>::function()
                                 );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QVector_shared_pointer_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, false, T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef QVector<T_content> C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_QVector_shared_pointer(env,
                                new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                deletePointer<Pointer,C_content>,
                                getFromPointer<Pointer,C_content>,
                                typeid(T),
                                ContainerAppend<QVector, T_content, supports_StandardConstructor<T_content>::value>::function(),
                                ContainerAppendVector<QVector, T_content, qtjambi_is_QVector>::function(),
                                ContainerAt<QVector, T_content>::function(),
                                ContainerBegin<QVector, T_content>::function(),
                                ContainerCapacity<QVector, T_content>::function(),
                                ContainerClear<QVector, T_content, supports_StandardConstructor<T_content>::value>::function(),
                                ContainerContains<QVector, T_content>::function(),
                                ContainerCountObject<QVector, T_content>::function(),
                                ContainerEnd<QVector, T_content>::function(),
                                ContainerEndsWith<QVector, T_content>::function(),
                                ContainerFill<QVector, T_content>::function(),
                                ContainerIndexOf<QVector, T_content>::function(),
                                ContainerInsertAt<QVector, T_content, supports_StandardConstructor<T_content>::value>::function(),
                                ContainerInsertN<QVector, T_content>::function(),
                                ContainerLastIndexOf<QVector, T_content>::function(),
                                ContainerMid<QVector, T_content>::function(),
                                ContainerMove<QVector, T_content, supports_StandardConstructor<T_content>::value>::function(),
                                ContainerEquals<QVector, T_content, qtjambi_is_QVector>::function(),
                                ContainerPrepend<QVector, T_content, supports_StandardConstructor<T_content>::value>::function(),
                                ContainerRemoveAll<QVector, T_content>::function(),
                                ContainerRemoveI<QVector, T_content>::function(),
                                ContainerRemoveN<QVector, T_content>::function(),
                                ContainerRemoveOne<QVector, T_content>::function(),
                                ContainerReplace<QVector, T_content>::function(),
                                ContainerReserve<QVector, T_content>::function(),
                                ContainerResize<QVector, T_content>::function(),
                                ContainerShrinkToFit<QVector, T_content>::function(),
                                ContainerSize<QVector, T_content>::function(),
                                ContainerStartsWith<QVector, T_content>::function(),
                                ContainerTakeAt<QVector, T_content>::function(),
                                ContainerToList<QVector, T_content>::function(),
                                ContainerValue<QVector, T, supports_StandardConstructor<T_content>::value>::function(),
                                ContainerValueDefault<QVector, T_content>::function()
                );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QStack_shared_pointer_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, true, T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::add_const<QStack<T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_constQStack_shared_pointer(env,
                                     new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                     deletePointer<Pointer,C_content>,
                                     getFromPointer<Pointer,C_content>,
                                     typeid(T),
                                     ContainerAt<QStack, T_content>::function(),
                                     ContainerBegin<QStack, T_content>::function(),
                                     ContainerCapacity<QStack, T_content>::function(),
                                     ContainerContains<QStack, T_content>::function(),
                                     ContainerCountObject<QStack, T_content>::function(),
                                     ContainerEnd<QStack, T_content>::function(),
                                     ContainerEndsWith<QStack, T_content>::function(),
                                     ContainerIndexOf<QStack, T_content>::function(),
                                     ContainerLastIndexOf<QStack, T_content>::function(),
                                     ContainerMid<QStack, T_content>::function(),
                                     ContainerEquals<QStack, T_content, qtjambi_is_QStack>::function(),
                                     ContainerSize<QStack, T_content>::function(),
                                     ContainerStartsWith<QStack, T_content>::function(),
                                     ContainerToList<QStack, T_content>::function(),
                                     ContainerValue<QStack, T, supports_StandardConstructor<T_content>::value>::function(),
                                     ContainerValueDefault<QStack, T_content>::function()
                                 );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QStack_shared_pointer_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference, false, T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef QStack<T_content> C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_QStack_shared_pointer(env,
                                new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                deletePointer<Pointer,C_content>,
                                getFromPointer<Pointer,C_content>,
                                typeid(T),
                                ContainerAppend<QStack, T_content, supports_StandardConstructor<T_content>::value>::function(),
                                ContainerAppendVector<QStack, T_content, qtjambi_is_QStack>::function(),
                                ContainerAt<QStack, T_content>::function(),
                                ContainerBegin<QStack, T_content>::function(),
                                ContainerCapacity<QStack, T_content>::function(),
                                ContainerClear<QStack, T_content, supports_StandardConstructor<T_content>::value>::function(),
                                ContainerContains<QStack, T_content>::function(),
                                ContainerCountObject<QStack, T_content>::function(),
                                ContainerEnd<QStack, T_content>::function(),
                                ContainerEndsWith<QStack, T_content>::function(),
                                ContainerFill<QStack, T_content>::function(),
                                ContainerIndexOf<QStack, T_content>::function(),
                                ContainerInsertAt<QStack, T_content, supports_StandardConstructor<T_content>::value>::function(),
                                ContainerInsertN<QStack, T_content>::function(),
                                ContainerLastIndexOf<QStack, T_content>::function(),
                                ContainerMid<QStack, T_content>::function(),
                                ContainerMove<QStack, T_content, supports_StandardConstructor<T_content>::value>::function(),
                                ContainerEquals<QStack, T_content, qtjambi_is_QStack>::function(),
                                ContainerPrepend<QStack, T_content, supports_StandardConstructor<T_content>::value>::function(),
                                ContainerRemoveAll<QStack, T_content>::function(),
                                ContainerRemoveI<QStack, T_content>::function(),
                                ContainerRemoveN<QStack, T_content>::function(),
                                ContainerRemoveOne<QStack, T_content>::function(),
                                ContainerReplace<QStack, T_content>::function(),
                                ContainerReserve<QStack, T_content>::function(),
                                ContainerResize<QStack, T_content>::function(),
                                ContainerShrinkToFit<QStack, T_content>::function(),
                                ContainerSize<QStack, T_content>::function(),
                                ContainerStartsWith<QStack, T_content>::function(),
                                ContainerTakeAt<QStack, T_content>::function(),
                                ContainerToList<QStack, T_content>::function(),
                                ContainerValue<QStack, T, supports_StandardConstructor<T_content>::value>::function(),
                                ContainerValueDefault<QStack, T_content>::function()
                );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QMap_shared_pointer_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference,
                                            true /*c_is_const*/,
                                            K, k_is_pointer, k_is_const, k_is_reference,
                                            T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<k_is_pointer, typename std::add_pointer<K>::type, K>::type K_ptr;
    typedef typename std::conditional<k_is_const, typename std::add_const<K_ptr>::type, K_ptr>::type K_const;
    typedef typename std::conditional<k_is_reference, typename std::add_lvalue_reference<K_const>::type, K_const>::type K_content;
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::add_const<QMap<K_content,T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static constexpr bool supported = supports_less_than<K>::value && supports_equal<K>::value;
    static constexpr bool supports_value_equals = supports_equal<T>::value;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_constQMap_shared_pointer(env,
                                               new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                               deletePointer<Pointer,C_content>,
                                               getFromPointer<Pointer,C_content>,
                                              qtjambi_type<K>::id(),
                                              qtjambi_type<T>::id(),
                                              QMapBeginFunction(BiContainerBegin<QMap, K_content, T_content, supported>::function()),
                                              QMapContainsFunction(BiContainerContains<QMap, K_content, T_content, supported>::function()),
                                              QMapCountObjectFunction(BiContainerCountObject<QMap, K_content, T_content, supported>::function()),
                                              QMapEndFunction(BiContainerEnd<QMap, K_content, T_content, supported>::function()),
                                              QMapFindFunction(BiContainerFindPair<QMap, K_content, T_content, supported>::function()),
                                              QMapFirstFunction(BiContainerFirst<QMap, K_content, T_content, supported>::function()),
                                              QMapFirstKeyFunction(BiContainerFirstKey<QMap, K_content, T_content, supported>::function()),
                                              QMapKeyFunction(BiContainerKey<QMap, K_content, T_content, supported && supports_value_equals>::function()),
                                              QMapKeysFunction(BiContainerKeys<QMap, K_content, T_content, supported && supports_value_equals>::function()),
                                              QMapKeysForValueFunction(BiContainerKeysForValue<QMap, K_content, T_content, supported>::function()),
                                              QMapLastFunction(BiContainerLast<QMap, K_content, T_content, supported>::function()),
                                              QMapLastKeyFunction(BiContainerLastKey<QMap, K_content, T_content, supported>::function()),
                                              QMapLowerBoundFunction(BiContainerLowerBound<QMap, K_content, T_content, supported>::function()),
                                              QMapEqualFunction(BiContainerEquals<QMap, K_content, T_content, supported && supports_value_equals, qtjambi_is_QMap>::function()),
                                              QMapSizeFunction(BiContainerSize<QMap, K_content, T_content, supported>::function()),
                                              QMapUniqueKeysFunction(BiContainerUniqueKeys<QMap, K_content, T_content, supported>::function()),
                                              QMapUpperBoundFunction(BiContainerUpperBound<QMap, K_content, T_content, supported>::function()),
                                              QMapValueFunction(BiContainerValue<QMap, K_content, T_content, supported>::function()),
                                              QMapValuesFunction(BiContainerValues<QMap, K_content, T_content, supported>::function()),
                                              QMapValuesKeyFunction(BiContainerValuesKey<QMap, K_content, T_content, supported>::function())
                                 );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QMap_shared_pointer_caster<has_scope, Pointer, p_is_pointer, p_is_const, p_is_reference,
                                            false /*c_is_const*/,
                                            K, k_is_pointer, k_is_const, k_is_reference,
                                            T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<k_is_pointer, typename std::add_pointer<K>::type, K>::type K_ptr;
    typedef typename std::conditional<k_is_const, typename std::add_const<K_ptr>::type, K_ptr>::type K_const;
    typedef typename std::conditional<k_is_reference, typename std::add_lvalue_reference<K_const>::type, K_const>::type K_content;
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef QMap<K_content,T_content> C_content;
    typedef Pointer<C_content> NativeType;

    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static constexpr bool supported = supports_less_than<K>::value && supports_equal<K>::value;
    static constexpr bool supports_value_equals = supports_equal<T>::value;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_QMap_shared_pointer(env,
                                         new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                         deletePointer<Pointer,C_content>,
                                         getFromPointer<Pointer,C_content>,
                                         typeid(K),
                                         typeid(T),
                                         QMapBeginFunction(BiContainerBegin<QMap, K_content, T_content, supported>::function()),
                                         QMapClearFunction(BiContainerClear<QMap, K_content, T_content, supported>::function()),
                                         QMapContainsFunction(BiContainerContains<QMap, K_content, T_content, supported>::function()),
                                         QMapCountObjectFunction(BiContainerCountObject<QMap, K_content, T_content, supported>::function()),
                                         QMapEndFunction(BiContainerEnd<QMap, K_content, T_content, supported>::function()),
                                         QMapFindFunction(BiContainerFindPair<QMap, K_content, T_content, supported>::function()),
                                         QMapFirstFunction(BiContainerFirst<QMap, K_content, T_content, supported>::function()),
                                         QMapFirstKeyFunction(BiContainerFirstKey<QMap, K_content, T_content, supported>::function()),
                                         QMapInsertFunction(BiContainerInsert<QMap, K_content, T_content, supported>::function()),
                                         QMapKeyFunction(BiContainerKey<QMap, K_content, T_content, supported && supports_value_equals>::function()),
                                         QMapKeysFunction(BiContainerKeys<QMap, K_content, T_content, supported>::function()),
                                         QMapKeysForValueFunction(BiContainerKeysForValue<QMap, K_content, T_content, supported>::function()),
                                         QMapLastFunction(BiContainerLast<QMap, K_content, T_content, supported>::function()),
                                         QMapLastKeyFunction(BiContainerLastKey<QMap, K_content, T_content, supported>::function()),
                                         QMapLowerBoundFunction(BiContainerLowerBound<QMap, K_content, T_content, supported>::function()),
                                         QMapEqualFunction(BiContainerEquals<QMap, K_content, T_content, supported && supports_value_equals, qtjambi_is_QMap>::function()),
                                         QMapRemoveAllFunction(BiContainerRemove<QMap, K_content, T_content, supported>::function()),
                                         QMapSizeFunction(BiContainerSize<QMap, K_content, T_content, supported>::function()),
                                         QMapTakeFunction(BiContainerTake<QMap, K_content, T_content, supported && supports_StandardConstructor<T>::value>::function()),
                                         QMapUniqueKeysFunction(BiContainerUniqueKeys<QMap, K_content, T_content, supported>::function()),
                                         QMapUniteFunction(BiContainerUnite<QMap, K_content, T_content, supported, qtjambi_is_QMap>::function()),
                                         QMapUpperBoundFunction(BiContainerUpperBound<QMap, K_content, T_content, supported>::function()),
                                         QMapValueFunction(BiContainerValue<QMap, K_content, T_content, supported>::function()),
                                         QMapValuesFunction(BiContainerValues<QMap, K_content, T_content, supported>::function()),
                                         QMapValuesKeyFunction(BiContainerValuesKey<QMap, K_content, T_content, supported>::function())
                            );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QMultiMap_shared_pointer_caster<has_scope,
                                                Pointer, p_is_pointer, p_is_const, p_is_reference, true,
                                                K, k_is_pointer, k_is_const, k_is_reference,
                                                T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<k_is_pointer, typename std::add_pointer<K>::type, K>::type K_ptr;
    typedef typename std::conditional<k_is_const, typename std::add_const<K_ptr>::type, K_ptr>::type K_const;
    typedef typename std::conditional<k_is_reference, typename std::add_lvalue_reference<K_const>::type, K_const>::type K_content;
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::add_const<QMultiMap<K_content,T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;

    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static constexpr bool supported = supports_less_than<K>::value && supports_equal<K>::value;
    static constexpr bool supports_value_equals = supports_equal<T>::value;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_constQMultiMap_shared_pointer(env,
                                                    new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                                    deletePointer<Pointer,C_content>,
                                                    getFromPointer<Pointer,C_content>,
                                                    typeid(K),
                                                    typeid(T),
                                                  BiContainerBegin<QMultiMap, K_content, T_content, supported>::function(),
                                                  BiContainerContains<QMultiMap, K_content, T_content, supported>::function(),
                                                  BiContainerCountObject<QMultiMap, K_content, T_content, supported>::function(),
                                                  BiContainerEnd<QMultiMap, K_content, T_content, supported>::function(),
                                                  BiContainerFindPair<QMultiMap, K_content, T_content, supported>::function(),
                                                  BiContainerFirst<QMultiMap, K_content, T_content, supported>::function(),
                                                  BiContainerFirstKey<QMultiMap, K_content, T_content, supported>::function(),
                                                  BiContainerKey<QMultiMap, K_content, T_content, supported && supports_value_equals>::function(),
                                                  BiContainerKeys<QMultiMap, K_content, T_content, supported>::function(),
                                                  BiContainerKeysForValue<QMultiMap, K_content, T_content, supported && supports_value_equals>::function(),
                                                  BiContainerLast<QMultiMap, K_content, T_content, supported>::function(),
                                                  BiContainerLastKey<QMultiMap, K_content, T_content, supported>::function(),
                                                  BiContainerLowerBound<QMultiMap, K_content, T_content, supported>::function(),
                                                  BiContainerEquals<QMultiMap, K_content, T_content, supported && supports_value_equals, qtjambi_is_QMultiMap>::function(),
                                                  BiContainerSize<QMultiMap, K_content, T_content, supported>::function(),
                                                  BiContainerUniqueKeys<QMultiMap, K_content, T_content, supported>::function(),
                                                  BiContainerUpperBound<QMultiMap, K_content, T_content, supported>::function(),
                                                  BiContainerValue<QMultiMap, K_content, T_content, supported>::function(),
                                                  BiContainerValues<QMultiMap, K_content, T_content, supported>::function(),
                                                  BiContainerValuesKey<QMultiMap, K_content, T_content, supported && supports_value_equals>::function(),
                                                  BiContainerContainsPair<QMultiMap, K_content, T_content, supported && supports_value_equals>::function(),
                                                  BiContainerCountPair<QMultiMap, K_content, T_content, supported && supports_value_equals>::function(),
                                                  BiContainerFindPairs<QMultiMap, K_content, T_content, supported && supports_value_equals>::function()
                                 );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QMultiMap_shared_pointer_caster<has_scope,
                                                Pointer, p_is_pointer, p_is_const, p_is_reference, false,
                                                K, k_is_pointer, k_is_const, k_is_reference,
                                                T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<k_is_pointer, typename std::add_pointer<K>::type, K>::type K_ptr;
    typedef typename std::conditional<k_is_const, typename std::add_const<K_ptr>::type, K_ptr>::type K_const;
    typedef typename std::conditional<k_is_reference, typename std::add_lvalue_reference<K_const>::type, K_const>::type K_content;
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef QMultiMap<K_content,T_content> C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static constexpr bool supported = supports_less_than<K>::value && supports_equal<K>::value;
    static constexpr bool supports_value_equals = supports_equal<T>::value;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_QMultiMap_shared_pointer(env,
                                           new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                           deletePointer<Pointer,C_content>,
                                           getFromPointer<Pointer,C_content>,
                                           typeid(K),
                                           typeid(T),
                                         BiContainerBegin<QMultiMap, K_content, T_content, supported>::function(),
                                         BiContainerClear<QMultiMap, K_content, T_content, supported>::function(),
                                         BiContainerContains<QMultiMap, K_content, T_content, supported>::function(),
                                         BiContainerCountObject<QMultiMap, K_content, T_content, supported>::function(),
                                         BiContainerEnd<QMultiMap, K_content, T_content, supported>::function(),
                                         BiContainerFindPair<QMultiMap, K_content, T_content, supported>::function(),
                                         BiContainerFirst<QMultiMap, K_content, T_content, supported>::function(),
                                         BiContainerFirstKey<QMultiMap, K_content, T_content, supported>::function(),
                                         BiContainerInsert<QMultiMap, K_content, T_content, supported>::function(),
                                         BiContainerKey<QMultiMap, K_content, T_content, supported && supports_value_equals>::function(),
                                         BiContainerKeys<QMultiMap, K_content, T_content, supported>::function(),
                                         BiContainerKeysForValue<QMultiMap, K_content, T_content, supported && supports_value_equals>::function(),
                                         BiContainerLast<QMultiMap, K_content, T_content, supported>::function(),
                                         BiContainerLastKey<QMultiMap, K_content, T_content, supported>::function(),
                                         BiContainerLowerBound<QMultiMap, K_content, T_content, supported>::function(),
                                         BiContainerEquals<QMultiMap, K_content, T_content, supported && supports_value_equals, qtjambi_is_QMultiMap>::function(),
                                         BiContainerRemove<QMultiMap, K_content, T_content, supported>::function(),
                                         BiContainerSize<QMultiMap, K_content, T_content, supported>::function(),
                                         BiContainerTake<QMultiMap, K_content, T_content, supported && supports_StandardConstructor<T>::value>::function(),
                                         BiContainerUniqueKeys<QMultiMap, K_content, T_content, supported>::function(),
                                         BiContainerUnite<QMultiMap, K_content, T_content, supported, qtjambi_is_QMultiMap>::function(),
                                         BiContainerUpperBound<QMultiMap, K_content, T_content, supported>::function(),
                                         BiContainerValue<QMultiMap, K_content, T_content, supported>::function(),
                                         BiContainerValues<QMultiMap, K_content, T_content, supported>::function(),
                                         BiContainerValuesKey<QMultiMap, K_content, T_content, supported>::function(),
                                         BiContainerContainsPair<QMultiMap, K_content, T_content, supported && supports_value_equals>::function(),
                                         BiContainerCountPair<QMultiMap, K_content, T_content, supported && supports_value_equals>::function(),
                                         BiContainerFindPairs<QMultiMap, K_content, T_content, supported && supports_value_equals>::function(),
                                         BiContainerRemovePair<QMultiMap, K_content, T_content, supported && supports_value_equals>::function(),
                                         BiContainerReplacePair<QMultiMap, K_content, T_content, supported && supports_value_equals>::function()
                );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QHash_shared_pointer_caster<has_scope,
                                            Pointer, p_is_pointer, p_is_const, p_is_reference, true,
                                            K, k_is_pointer, k_is_const, k_is_reference,
                                            T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<k_is_pointer, typename std::add_pointer<K>::type, K>::type K_ptr;
    typedef typename std::conditional<k_is_const, typename std::add_const<K_ptr>::type, K_ptr>::type K_const;
    typedef typename std::conditional<k_is_reference, typename std::add_lvalue_reference<K_const>::type, K_const>::type K_content;
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::add_const<QHash<K_content,T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static constexpr bool supported = supports_qHash<K>::value && supports_equal<K>::value;
    static constexpr bool supports_value_equals = supports_equal<T>::value;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_constQHash_shared_pointer(env,
                                                new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                                deletePointer<Pointer,C_content>,
                                                getFromPointer<Pointer,C_content>,
                                                typeid(K),
                                                typeid(T),
                                              QHashBeginFunction(BiContainerBegin<QHash, K_content, T_content, supported>::function()),
                                              QHashCapacityFunction(BiContainerCapacity<QHash, K_content, T_content, supported>::function()),
                                              QHashContainsFunction(BiContainerContains<QHash, K_content, T_content, supported>::function()),
                                              QHashCountObjectFunction(BiContainerCountObject<QHash, K_content, T_content, supported>::function()),
                                              QHashEndFunction(BiContainerEnd<QHash, K_content, T_content, supported>::function()),
                                              QHashFindFunction(BiContainerFindPair<QHash, K_content, T_content, supported>::function()),
                                              QHashKeyFunction(BiContainerKey<QHash, K_content, T_content, supported && supports_value_equals>::function()),
                                              QHashKeysFunction(BiContainerKeys<QHash, K_content, T_content, supported>::function()),
                                              QHashKeysForValueFunction(BiContainerKeysForValue<QHash, K_content, T_content, supported && supports_value_equals>::function()),
                                              QHashEqualFunction(BiContainerEquals<QHash, K_content, T_content, supported && supports_value_equals, qtjambi_is_QHash>::function()),
                                              QHashSizeFunction(BiContainerSize<QHash, K_content, T_content, supported>::function()),
                                              QHashUniqueKeysFunction(BiContainerUniqueKeys<QHash, K_content, T_content, supported>::function()),
                                              QHashValueFunction(BiContainerValue<QHash, K_content, T_content, supported>::function()),
                                              QHashValuesFunction(BiContainerValues<QHash, K_content, T_content, supported>::function()),
                                              QHashValuesKeyFunction(BiContainerValuesKey<QHash, K_content, T_content, supported>::function())
                                 );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QHash_shared_pointer_caster<has_scope,
                                            Pointer, p_is_pointer, p_is_const, p_is_reference, false,
                                            K, k_is_pointer, k_is_const, k_is_reference,
                                            T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<k_is_pointer, typename std::add_pointer<K>::type, K>::type K_ptr;
    typedef typename std::conditional<k_is_const, typename std::add_const<K_ptr>::type, K_ptr>::type K_const;
    typedef typename std::conditional<k_is_reference, typename std::add_lvalue_reference<K_const>::type, K_const>::type K_content;
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef QHash<K_content,T_content> C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static constexpr bool supported = supports_qHash<K>::value && supports_equal<K>::value;
    static constexpr bool supports_value_equals = supports_equal<T>::value;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_QHash_shared_pointer(env,
                                           new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                           deletePointer<Pointer,C_content>,
                                           getFromPointer<Pointer,C_content>,
                                           typeid(K),
                                           typeid(T),
                                         QHashBeginFunction(BiContainerBegin<QHash, K_content, T_content, supported>::function()),
                                         QHashCapacityFunction(BiContainerCapacity<QHash, K_content, T_content, supported>::function()),
                                         QHashClearFunction(BiContainerClear<QHash, K_content, T_content, supported>::function()),
                                         QHashContainsFunction(BiContainerContains<QHash, K_content, T_content, supported>::function()),
                                         QHashCountObjectFunction(BiContainerCountObject<QHash, K_content, T_content, supported>::function()),
                                         QHashEndFunction(BiContainerEnd<QHash, K_content, T_content, supported>::function()),
                                         QHashFindFunction(BiContainerFindPair<QHash, K_content, T_content, supported>::function()),
                                         QHashInsertFunction(BiContainerInsert<QHash, K_content, T_content, supported>::function()),
                                         QHashKeyFunction(BiContainerKey<QHash, K_content, T_content, supported && supports_value_equals>::function()),
                                         QHashKeysFunction(BiContainerKeys<QHash, K_content, T_content, supported>::function()),
                                         QHashKeysForValueFunction(BiContainerKeysForValue<QHash, K_content, T_content, supported && supports_value_equals>::function()),
                                         QHashEqualFunction(BiContainerEquals<QHash, K_content, T_content, supported && supports_value_equals, qtjambi_is_QHash>::function()),
                                         QHashRemoveAllFunction(BiContainerRemove<QHash, K_content, T_content, supported>::function()),
                                         QHashReserveFunction(BiContainerReserve<QHash, K_content, T_content, supported>::function()),
                                         QHashSizeFunction(BiContainerSize<QHash, K_content, T_content, supported>::function()),
                                         QHashTakeFunction(BiContainerTake<QHash, K_content, T_content, supported && supports_StandardConstructor<T>::value>::function()),
                                         QHashUniqueKeysFunction(BiContainerUniqueKeys<QHash, K_content, T_content, supported>::function()),
                                         QHashUniteFunction(BiContainerUnite<QHash, K_content, T_content, supported, qtjambi_is_QHash>::function()),
                                         QHashValueFunction(BiContainerValue<QHash, K_content, T_content, supported>::function()),
                                         QHashValuesFunction(BiContainerValues<QHash, K_content, T_content, supported>::function()),
                                         QHashValuesKeyFunction(BiContainerValuesKey<QHash, K_content, T_content, supported>::function())
                            );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QMultiHash_shared_pointer_caster<has_scope,
                                                Pointer, p_is_pointer, p_is_const, p_is_reference, true,
                                                K, k_is_pointer, k_is_const, k_is_reference,
                                                T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<k_is_pointer, typename std::add_pointer<K>::type, K>::type K_ptr;
    typedef typename std::conditional<k_is_const, typename std::add_const<K_ptr>::type, K_ptr>::type K_const;
    typedef typename std::conditional<k_is_reference, typename std::add_lvalue_reference<K_const>::type, K_const>::type K_content;
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef typename std::add_const<QMultiHash<K_content,T_content>>::type C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static constexpr bool supported = supports_qHash<K>::value && supports_equal<K>::value;
    static constexpr bool supports_value_equals = supports_equal<T>::value;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_constQMultiHash_shared_pointer(env,
                                                 new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                                 deletePointer<Pointer,C_content>,
                                                 getFromPointer<Pointer,C_content>,
                                                 typeid(K),
                                                 typeid(T),
                                              QHashBeginFunction(BiContainerBegin<QMultiHash, K_content, T_content, supported>::function()),
                                              QHashCapacityFunction(BiContainerCapacity<QMultiHash, K_content, T_content, supported>::function()),
                                              QHashContainsFunction(BiContainerContains<QMultiHash, K_content, T_content, supported>::function()),
                                              QHashCountObjectFunction(BiContainerCountObject<QMultiHash, K_content, T_content, supported>::function()),
                                              QHashEndFunction(BiContainerEnd<QMultiHash, K_content, T_content, supported>::function()),
                                              QHashFindFunction(BiContainerFindPair<QMultiHash, K_content, T_content, supported>::function()),
                                              QHashKeyFunction(BiContainerKey<QMultiHash, K_content, T_content, supported && supports_value_equals>::function()),
                                              QHashKeysFunction(BiContainerKeys<QMultiHash, K_content, T_content, supported>::function()),
                                              QHashKeysForValueFunction(BiContainerKeysForValue<QMultiHash, K_content, T_content, supported && supports_value_equals>::function()),
                                              QHashEqualFunction(BiContainerEquals<QMultiHash, K_content, T_content, supported && supports_value_equals, qtjambi_is_QMultiHash>::function()),
                                              QHashSizeFunction(BiContainerSize<QMultiHash, K_content, T_content, supported>::function()),
                                              QHashUniqueKeysFunction(BiContainerUniqueKeys<QMultiHash, K_content, T_content, supported>::function()),
                                              QHashValueFunction(BiContainerValue<QMultiHash, K_content, T_content, supported>::function()),
                                              QHashValuesFunction(BiContainerValues<QMultiHash, K_content, T_content, supported>::function()),
                                              QHashValuesKeyFunction(BiContainerValuesKey<QMultiHash, K_content, T_content, supported>::function()),
                                              BiContainerContainsPair<QMultiMap, K_content, T_content, supported && supports_value_equals>::function(),
                                              BiContainerCountPair<QMultiMap, K_content, T_content, supported && supports_value_equals>::function(),
                                              BiContainerFindPairs<QMultiMap, K_content, T_content, supported && supports_value_equals>::function()
                                 );
        return result;
    }
};

template<bool has_scope, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference>
struct qtjambi_QMultiHash_shared_pointer_caster<has_scope,
                                                Pointer, p_is_pointer, p_is_const, p_is_reference, false,
                                                K, k_is_pointer, k_is_const, k_is_reference,
                                                T, t_is_pointer, t_is_const, t_is_reference>{
    typedef typename std::conditional<k_is_pointer, typename std::add_pointer<K>::type, K>::type K_ptr;
    typedef typename std::conditional<k_is_const, typename std::add_const<K_ptr>::type, K_ptr>::type K_const;
    typedef typename std::conditional<k_is_reference, typename std::add_lvalue_reference<K_const>::type, K_const>::type K_content;
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T>::type, T>::type T_ptr;
    typedef typename std::conditional<t_is_const, typename std::add_const<T_ptr>::type, T_ptr>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_content;
    typedef QMultiHash<K_content,T_content> C_content;
    typedef Pointer<C_content> NativeType;
    typedef typename std::conditional<p_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<p_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<p_is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static constexpr bool supported = supports_qHash<K>::value && supports_equal<K>::value;
    static constexpr bool supports_value_equals = supports_equal<T>::value;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jobject result = qtjambi_from_QMultiHash_shared_pointer(env,
                                            new NativeType(deref_ptr<p_is_pointer, NativeType_c>::deref(in)),
                                            deletePointer<Pointer,C_content>,
                                            getFromPointer<Pointer,C_content>,
                                            typeid(K),
                                            typeid(T),
                                         QHashBeginFunction(BiContainerBegin<QMultiHash, K_content, T_content, supported>::function()),
                                         QHashCapacityFunction(BiContainerCapacity<QMultiHash, K_content, T_content, supported>::function()),
                                         QHashClearFunction(BiContainerClear<QMultiHash, K_content, T_content, supported>::function()),
                                         QHashContainsFunction(BiContainerContains<QMultiHash, K_content, T_content, supported>::function()),
                                         QHashCountObjectFunction(BiContainerCountObject<QMultiHash, K_content, T_content, supported>::function()),
                                         QHashEndFunction(BiContainerEnd<QMultiHash, K_content, T_content, supported>::function()),
                                         QHashFindFunction(BiContainerFindPair<QMultiHash, K_content, T_content, supported>::function()),
                                         QHashInsertFunction(BiContainerInsert<QMultiHash, K_content, T_content, supported>::function()),
                                         QHashKeyFunction(BiContainerKey<QMultiHash, K_content, T_content, supported && supports_value_equals>::function()),
                                         QHashKeysFunction(BiContainerKeys<QMultiHash, K_content, T_content, supported>::function()),
                                         QHashKeysForValueFunction(BiContainerKeysForValue<QMultiHash, K_content, T_content, supported && supports_value_equals>::function()),
                                         QHashEqualFunction(BiContainerEquals<QMultiHash, K_content, T_content, supported && supports_value_equals, qtjambi_is_QMultiHash>::function()),
                                         QHashRemoveAllFunction(BiContainerRemove<QMultiHash, K_content, T_content, supported>::function()),
                                         QHashReserveFunction(BiContainerReserve<QMultiHash, K_content, T_content, supported>::function()),
                                         QHashSizeFunction(BiContainerSize<QMultiHash, K_content, T_content, supported>::function()),
                                         QHashTakeFunction(BiContainerTake<QMultiHash, K_content, T_content, supported && supports_StandardConstructor<T>::value>::function()),
                                         QHashUniqueKeysFunction(BiContainerUniqueKeys<QMultiHash, K_content, T_content, supported>::function()),
                                         QHashUniteFunction(BiContainerUnite<QMultiHash, K_content, T_content, supported, qtjambi_is_QMultiHash>::function()),
                                         QHashValueFunction(BiContainerValue<QMultiHash, K_content, T_content, supported>::function()),
                                         QHashValuesFunction(BiContainerValues<QMultiHash, K_content, T_content, supported>::function()),
                                         QHashValuesKeyFunction(BiContainerValuesKey<QMultiHash, K_content, T_content, supported>::function()),
                                         BiContainerContainsPair<QMultiMap, K_content, T_content, supported && supports_value_equals>::function(),
                                         BiContainerCountPair<QMultiMap, K_content, T_content, supported && supports_value_equals>::function(),
                                         BiContainerFindPairs<QMultiMap, K_content, T_content, supported && supports_value_equals>::function(),
                                         BiContainerRemovePair<QMultiMap, K_content, T_content, supported && supports_value_equals>::function(),
                                         BiContainerReplacePair<QMultiMap, K_content, T_content, supported && supports_value_equals>::function()
                            );
        return result;
    }
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_SHAREDPOINTER_P_H
