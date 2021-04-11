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

#ifndef QTJAMBI_CAST_CONTAINER4_P_H
#define QTJAMBI_CAST_CONTAINER4_P_H

#include "qtjambi_cast_util_p.h"
#include "qtjambi_cast_map_p.h"

namespace QtJambiPrivate {

template<bool forward, bool has_scope,
         typename JniType,
         template<typename K, typename T, typename A, typename B> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A, typename B>
struct qtjambi_jnitype_container4_cast{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A, typename B>
struct qtjambi_jnitype_container4_cast<true, has_scope,
                                 jobject,
                                 std::map, is_pointer, is_const, is_reference,
                                 K, T, A, B>{
    typedef std::map<K, T, A, B> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        jobject list = qtjambi_hashmap_new(env, jint(_in.size()));
        for (auto it = _in.begin(); it != _in.end(); ++it) {
            jobject first = qtjambi_scoped_cast<has_scope,jobject,decltype(it->first)>::cast(env, it->first, nullptr, scope);
            jobject second = qtjambi_scoped_cast<has_scope,jobject,decltype(it->second)>::cast(env, it->second, nullptr, scope);
            qtjambi_map_put(env, list, first, second);
        }
        return list;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A, typename B>
struct qtjambi_jnitype_container4_cast<false, has_scope,
                                 jobject,
                                 std::map, is_pointer, is_const, is_reference,
                                 K, T, A, B>{
    typedef std::map<K, T, A, B> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> map;
        if(in){
            map.reset(new NativeType());
            jobject iterator = qtjambi_map_entryset_iterator(env, in);
            while(qtjambi_iterator_has_next(env, iterator)) {
                jobject entry = qtjambi_iterator_next(env, iterator);
                jobject key = qtjambi_map$entry_key(env, entry);
                jobject value = qtjambi_map$entry_value(env, entry);
                map->insert(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, value, nullptr, scope));
            }
        }
        return create_container4_pointer<is_pointer, is_const, is_reference, has_scope, std::map, K, T, A, B>::create(env, scope, map, "std::map<K,T>");
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A, typename B>
struct qtjambi_jnitype_container4_cast<true, has_scope,
                                 jobject,
                                 std::multimap, is_pointer, is_const, is_reference,
                                 K, T, A, B>{
    typedef std::multimap<K, T, A, B> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        jobject list = qtjambi_hashmap_new(env, jint(_in.size()));
        for (auto it = _in.begin(); it != _in.end(); ++it) {
            jobject first = qtjambi_scoped_cast<has_scope,jobject,decltype(it->first)>::cast(env, it->first, nullptr, scope);
            jobject second = qtjambi_scoped_cast<has_scope,jobject,decltype(it->second)>::cast(env, it->second, nullptr, scope);
            qtjambi_map_put(env, list, first, second);
        }
        return list;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A, typename B>
struct qtjambi_jnitype_container4_cast<false, has_scope,
                                 jobject,
                                 std::multimap, is_pointer, is_const, is_reference,
                                 K, T, A, B>{
    typedef std::multimap<K, T, A, B> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> map;
        if(in){
            map.reset(new NativeType());
            jobject iterator = qtjambi_map_entryset_iterator(env, in);
            while(qtjambi_iterator_has_next(env, iterator)) {
                jobject entry = qtjambi_iterator_next(env, iterator);
                jobject key = qtjambi_map$entry_key(env, entry);
                jobject value = qtjambi_map$entry_value(env, entry);
                map->insert(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, value, nullptr, scope));
            }
        }
        return create_container4_pointer<is_pointer, is_const, is_reference, has_scope, std::map, K, T, A, B>::create(env, scope, map, "std::multimap<K,T>");
    }
};

#if defined(_UNORDERED_MAP_) || defined(_UNORDERED_MAP) || (defined(_LIBCPP_UNORDERED_MAP) && _LIBCPP_STD_VER > 14) || defined(_GLIBCXX_UNORDERED_MAP)
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A, typename B>
struct qtjambi_jnitype_container4_cast<true, has_scope,
                                 jobject,
                                 std::unordered_map, is_pointer, is_const, is_reference,
                                 K, T, A, B>{
    typedef std::unordered_map<K, T, A, B> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        jobject list = qtjambi_hashmap_new(env, jint(_in.size()));
        for (auto it = _in.begin(); it != _in.end(); ++it) {
            jobject first = qtjambi_scoped_cast<has_scope,jobject,decltype(it->first)>::cast(env, it->first, nullptr, scope);
            jobject second = qtjambi_scoped_cast<has_scope,jobject,decltype(it->second)>::cast(env, it->second, nullptr, scope);
            qtjambi_map_put(env, list, first, second);
        }
        return list;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A, typename B>
struct qtjambi_jnitype_container4_cast<false, has_scope,
                                 jobject,
                                 std::unordered_map, is_pointer, is_const, is_reference,
                                 K, T, A, B>{
    typedef std::unordered_map<K, T, A, B> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> map;
        if(in){
            map.reset(new NativeType());
            jobject iterator = qtjambi_map_entryset_iterator(env, in);
            while(qtjambi_iterator_has_next(env, iterator)) {
                jobject entry = qtjambi_iterator_next(env, iterator);
                jobject key = qtjambi_map$entry_key(env, entry);
                jobject value = qtjambi_map$entry_value(env, entry);
                map->insert(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, value, nullptr, scope));
            }
        }
        return create_container4_pointer<is_pointer, is_const, is_reference, has_scope, std::map, K, T, A, B>::create(env, scope, map, "std::map<K,T>");
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A, typename B>
struct qtjambi_jnitype_container4_cast<true, has_scope,
                                 jobject,
                                 std::unordered_multimap, is_pointer, is_const, is_reference,
                                 K, T, A, B>{
    typedef std::unordered_multimap<K, T, A, B> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        jobject list = qtjambi_hashmap_new(env, jint(_in.size()));
        for (auto it = _in.begin(); it != _in.end(); ++it) {
            jobject first = qtjambi_scoped_cast<has_scope,jobject,decltype(it->first)>::cast(env, it->first, nullptr, scope);
            jobject second = qtjambi_scoped_cast<has_scope,jobject,decltype(it->second)>::cast(env, it->second, nullptr, scope);
            qtjambi_map_put(env, list, first, second);
        }
        return list;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A, typename B>
struct qtjambi_jnitype_container4_cast<false, has_scope,
                                 jobject,
                                 std::unordered_multimap, is_pointer, is_const, is_reference,
                                 K, T, A, B>{
    typedef std::unordered_multimap<K, T, A, B> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> map;
        if(in){
            map.reset(new NativeType());
            jobject iterator = qtjambi_map_entryset_iterator(env, in);
            while(qtjambi_iterator_has_next(env, iterator)) {
                jobject entry = qtjambi_iterator_next(env, iterator);
                jobject key = qtjambi_map$entry_key(env, entry);
                jobject value = qtjambi_map$entry_value(env, entry);
                map->insert(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, value, nullptr, scope));
            }
        }
        return create_container4_pointer<is_pointer, is_const, is_reference, has_scope, std::map, K, T, A, B>::create(env, scope, map, "std::multimap<K,T>");
    }
};
#endif

#if defined(_UNORDERED_SET_) || defined(_UNORDERED_SET) || (defined(_LIBCPP_UNORDERED_SET) && _LIBCPP_STD_VER > 14) || defined(_GLIBCXX_UNORDERED_SET)
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A, typename B>
struct qtjambi_jnitype_container4_cast<true, has_scope,
                                 jobject,
                                 std::unordered_set, is_pointer, is_const, is_reference,
                                 K, T, A, B>{
    typedef std::unordered_set<K, T, A, B> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        jobject list = qtjambi_hashset_new(env);
        for(decltype(*(_in.begin())) entry : _in){
            jobject _entry = qtjambi_scoped_cast<has_scope,jobject,decltype(entry)>::cast(env, entry, nullptr, scope);
            qtjambi_collection_add(env, list, _entry);
        }
        return list;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A, typename B>
struct qtjambi_jnitype_container4_cast<false, has_scope,
                                 jobject,
                                 std::unordered_set, is_pointer, is_const, is_reference,
                                 K, T, A, B>{
    typedef std::unordered_set<K, T, A, B> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        QScopedPointer<NativeType> list;
        if(in){
            list.reset(new NativeType());
            jobject iterator = qtjambi_collection_iterator(env, in);
            while(qtjambi_iterator_has_next(env, iterator)) {
                jobject element = qtjambi_iterator_next(env, iterator);
                list->insert(qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, element, nullptr, scope));
            }
        }
        return create_container4_pointer<is_pointer, is_const, is_reference, has_scope, std::unordered_set, K, T, A, B>::create(env, scope, list, "std::unordered_set<K,T>");
    }
};
#endif

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_CONTAINER2_P_H
