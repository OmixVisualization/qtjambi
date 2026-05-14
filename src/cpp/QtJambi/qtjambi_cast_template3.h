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

#ifndef QTJAMBI_CAST_TEMPLATE3_H
#define QTJAMBI_CAST_TEMPLATE3_H

#include "qtjambi_cast.h"

namespace QtJambiPrivate {

template<bool forward,
         typename JniType,
         template<typename K, typename T, typename A> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename A, typename... Args>
struct qtjambi_jobject_template3_cast : decltype(qtjambi_jobject_template_plain_cast<forward, JniType, NativeType<K,T,A>, is_pointer, is_const, is_reference, is_rvalue, Args...>()){
};

#if defined(_SET_) || defined(_SET) || defined(_LIBCPP_SET) || defined(_GLIBCXX_SET)
template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename A, typename... Args>
struct qtjambi_jobject_template3_cast<forward,
                                 jobject,
                                 std::set, is_pointer, is_const, is_reference, is_rvalue,
                                 K, T, A, Args...>{
    typedef std::set<K, T, A> NativeType;
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

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename A, typename... Args>
struct qtjambi_jobject_template3_cast<forward,
                                 jobject,
                                 std::multiset, is_pointer, is_const, is_reference, is_rvalue,
                                 K, T, A, Args...>{
    typedef std::multiset<K, T, A> NativeType;
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
