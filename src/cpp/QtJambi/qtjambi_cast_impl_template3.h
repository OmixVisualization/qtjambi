/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_CAST_IMPL_TEMPLATE3_H
#define QTJAMBI_CAST_IMPL_TEMPLATE3_H

#include "qtjambi_cast_impl_util.h"
#include "qtjambi_cast_impl_container_associative.h"

namespace QtJambiPrivate {

template<bool forward, bool has_scope,
         typename JniType,
         template<typename K, typename T, typename A> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A>
struct qtjambi_jnitype_template3_cast{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

#if defined(_SET_) || defined(_SET) || defined(_LIBCPP_SET) || defined(_GLIBCXX_SET)
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A>
struct qtjambi_jnitype_template3_cast<true, has_scope,
                                 jobject,
                                 std::set, is_pointer, is_const, is_reference,
                                 K, T, A>{
    typedef std::set<K, T, A> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        jobject list = QtJambiAPI::newJavaHashSet(env);
        for(const auto& entry : _in){
            jobject _entry = qtjambi_scoped_cast<has_scope,jobject,decltype(entry)>::cast(env, entry, nullptr, scope);
            QtJambiAPI::addToJavaCollection(env, list, _entry);
        }
        return list;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A>
struct qtjambi_jnitype_template3_cast<false, has_scope,
                                 jobject,
                                 std::set, is_pointer, is_const, is_reference,
                                 K, T, A>{
    typedef std::set<K, T, A> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        std::unique_ptr<NativeType> list;
        if(in){
            list.reset(new NativeType());
            jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in);
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                list->insert(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, element, nullptr, scope));
            }
        }
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, list);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A>
struct qtjambi_jnitype_template3_cast<true, has_scope,
                                 jobject,
                                 std::multiset, is_pointer, is_const, is_reference,
                                 K, T, A>{
    typedef std::multiset<K, T, A> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        jobject list = QtJambiAPI::newJavaHashSet(env);
        for(const auto& entry : _in){
            jobject _entry = qtjambi_scoped_cast<has_scope,jobject,decltype(entry)>::cast(env, entry, nullptr, scope);
            QtJambiAPI::addToJavaCollection(env, list, _entry);
        }
        return list;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A>
struct qtjambi_jnitype_template3_cast<false, has_scope,
                                 jobject,
                                 std::multiset, is_pointer, is_const, is_reference,
                                 K, T, A>{
    typedef std::multiset<K, T, A> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        std::unique_ptr<NativeType> list;
        if(in){
            list.reset(new NativeType());
            jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in);
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                list->insert(qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, element, nullptr, scope));
            }
        }
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, list);
    }
};
#endif

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_CONTAINER2_P_H
