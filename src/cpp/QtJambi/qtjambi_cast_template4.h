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

#ifndef QTJAMBI_CAST_TEMPLATE4_H
#define QTJAMBI_CAST_TEMPLATE4_H

#include "qtjambi_cast.h"

namespace QtJambiPrivate {

template<bool forward,
         typename JniType,
         template<typename K, typename T, typename A, typename B> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename A, typename B, typename... Args>
struct qtjambi_jobject_template4_cast : decltype(qtjambi_jobject_template_plain_cast<forward, JniType, NativeType<K,T,A,B>, is_pointer, is_const, is_reference, is_rvalue, Args...>()){
};

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename A, typename B, typename... Args>
struct qtjambi_jobject_template4_cast<forward,
                                 jobject,
                                 std::map, is_pointer, is_const, is_reference, is_rvalue,
                                 K, T, A, B, Args...>{
    typedef std::map<K, T, A, B> NativeType;
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
            NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
            jobject list = QtJambiAPI::newJavaHashMap(env, jint(_in.size()));
            for (auto it = _in.cbegin(); it != _in.cend(); ++it) {
                const auto& _first = it->first;
                const auto& _second = it->second;
                jobject first = qtjambi_cast_with_args<jobject>(_first, std::forward<Args>(args)...);
                jobject second = qtjambi_cast_with_args<jobject>(_second, std::forward<Args>(args)...);
                QtJambiAPI::putJavaMap(env, list, first, second);
            }
            return list;
        }else{
            if(!in)
                return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(nullptr, args...);
            NativeType map;
            jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, in);
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);
                jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);
                jobject value = QtJambiAPI::valueOfJavaMapEntry(env, entry);
                map.insert({qtjambi_cast_with_args<K>(key, std::forward<Args>(args)...), qtjambi_cast_with_args<T>(value, std::forward<Args>(args)...)});
            }
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(map), args...);
        }
    }
};

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename A, typename B, typename... Args>
struct qtjambi_jobject_template4_cast<forward,
                                 jobject,
                                 std::multimap, is_pointer, is_const, is_reference, is_rvalue,
                                 K, T, A, B, Args...>{
    typedef std::multimap<K, T, A, B> NativeType;
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
            NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
            jobject list = QtJambiAPI::newJavaHashMap(env, jint(_in.size()));
            for (auto it = _in.cbegin(); it != _in.cend(); ++it) {
                const auto& _first = it->first;
                const auto& _second = it->second;
                jobject first = qtjambi_cast_with_args<jobject>(_first, std::forward<Args>(args)...);
                jobject second = qtjambi_cast_with_args<jobject>(_second, std::forward<Args>(args)...);
                QtJambiAPI::putJavaMap(env, list, first, second);
            }
            return list;
        }else{
            if(!in)
                return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(nullptr, args...);
            NativeType map;
            jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, in);
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);
                jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);
                jobject value = QtJambiAPI::valueOfJavaMapEntry(env, entry);
                map.insert({qtjambi_cast_with_args<K>(key, std::forward<Args>(args)...), qtjambi_cast_with_args<T>(value, std::forward<Args>(args)...)});
            }
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(map), args...);
        }
    }
};

#if defined(_UNORDERED_SET_) || defined(_UNORDERED_SET) || defined(_LIBCPP_UNORDERED_SET) || defined(_GLIBCXX_UNORDERED_SET)
template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename A, typename B, typename... Args>
struct qtjambi_jobject_template4_cast<forward,
                                 jobject,
                                 std::unordered_set, is_pointer, is_const, is_reference, is_rvalue,
                                 K, T, A, B, Args...>{
    typedef std::unordered_set<K, T, A, B> NativeType;
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
            NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
            jobject list = QtJambiAPI::newJavaHashSet(env);
            for(const auto& entry : _in){
                jobject _entry = qtjambi_cast_with_args<jobject>(entry, std::forward<Args>(args)...);
                QtJambiAPI::addToJavaCollection(env, list, _entry);
            }
            return list;
        }else{
            if(!in)
                return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(nullptr, args...);
            NativeType list;
            jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, in);
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                list.insert(qtjambi_cast_with_args<K>(element, std::forward<Args>(args)...));
            }
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(list), args...);
        }
    }
};

#endif

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_CONTAINER2_P_H
