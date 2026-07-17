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

#ifndef QTJAMBI_CAST_TEMPLATE2_H
#define QTJAMBI_CAST_TEMPLATE2_H

#include "qtjambi_cast.h"
#include "qtjambiapi.h"

namespace QtJambiPrivate {

template<bool forward,
         typename JniType,
         template<typename K, typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_jobject_template2_cast : decltype(qtjambi_jobject_template_plain_cast<forward, JniType, NativeType<K,T>, is_pointer, is_const, is_reference, is_rvalue, Args...>()){
};

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, class _Alloc, typename... Args>
struct qtjambi_jobject_template2_cast<forward,
                                 jobject,
                                 std::vector, is_pointer, is_const, is_reference, is_rvalue,
                                 T, _Alloc, Args...>{
    typedef std::vector<T,_Alloc> NativeType;
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
            jobject list = QtJambiAPI::newJavaArrayList(env, jint(_in.size()));
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
                list.push_back(qtjambi_cast_with_args<T>(element, std::forward<Args>(args)...));
            }
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(list), args...);
        }
    }
};

#if defined(_LIST_) || defined(_LIST) || defined(_LIBCPP_LIST) || defined(_GLIBCXX_LIST)
template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, class _Alloc, typename... Args>
struct qtjambi_jobject_template2_cast<forward,
                                 jobject,
                                 std::list, is_pointer, is_const, is_reference, is_rvalue,
                                 T, _Alloc, Args...>{
    typedef std::list<T,_Alloc> NativeType;
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
            jobject list = QtJambiAPI::newJavaArrayList(env, jint(_in.size()));
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
                list.push_back(qtjambi_cast_with_args<T>(element, std::forward<Args>(args)...));
            }
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(list), args...);
        }
    }
};

#endif

#if defined(_FORWARD_LIST_) || defined(_FORWARD_LIST) || defined(_LIBCPP_FORWARD_LIST) || defined(_GLIBCXX_FORWARD_LIST)
template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, class _Alloc, typename... Args>
struct qtjambi_jobject_template2_cast<forward,
                                 jobject,
                                 std::forward_list, is_pointer, is_const, is_reference, is_rvalue,
                                 T, _Alloc, Args...>{
    typedef std::forward_list<T,_Alloc> NativeType;
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
            jobject list = QtJambiAPI::newJavaArrayList(env, jint(_in.size()));
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
                list.push_back(qtjambi_cast_with_args<T>(element, std::forward<Args>(args)...));
            }
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(list), args...);
        }
    }
};
#endif

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_jobject_template2_cast<forward,
                                        jobject,
                                        std::pair, is_pointer, is_const, is_reference, is_rvalue,
                                        K, T, Args...>{
    typedef std::pair<K,T> NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::conditional_t<is_const, std::add_const_t<K>, K> K_const;
    typedef std::conditional_t<is_const, std::add_const_t<T>, T> T_const;
    typedef std::conditional_t<forward, NativeType_in, jobject> In;
    typedef std::conditional_t<forward, jobject, NativeType_out> Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
            return QtJambiAPI::newQPair(env,
                                        qtjambi_cast_with_args<jobject>(_in.first, std::forward<Args>(args)...),
                                        qtjambi_cast_with_args<jobject>(_in.second, std::forward<Args>(args)...));
        }else{
            if(!in)
                return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(nullptr, args...);
            jobject first = QtJambiAPI::getQPairFirst(env, in);
            jobject second = QtJambiAPI::getQPairSecond(env, in);
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(
                                std::pair<K,T>{qtjambi_cast_with_args<K>(first, std::forward<Args>(args)...),
                                                qtjambi_cast_with_args<T>(second, std::forward<Args>(args)...)}, args...);
        }
    }
};

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_jobject_template2_cast<forward,
                                        jobject,
                                        QScopedPointer, is_pointer, is_const, is_reference, is_rvalue,
                                        K, T, Args...>{
    typedef QScopedPointer<K,T> NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, jobject> In;
    typedef std::conditional_t<forward, jobject, NativeType_out> Out;

    static Out cast(In in, Args... args){
        if constexpr(forward){
            NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
            K* qo = _in.take();
            jobject o = qtjambi_cast_with_args<jobject>(qo, std::forward<Args>(args)...);
            qtjambi_ownership_decider<K, Args...>::setJavaOwnership(o, qo, args...);
            return o;
        }else{
            if constexpr(is_pointer || is_reference){
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast without scope");
                NativeType* scp = create<NativeType>(qtjambi_cast_with_args<K*>(in, std::forward<Args>(args)...));
                cast_var_args<Args...>::scope(args...).addDeletion(scp);
                qtjambi_ownership_decider<K, Args...>::setCppOwnershipAndInvalidate(in, scp->get(), args...);
                return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(scp, args...);
            }else{
                K* pointer{nullptr};
                if(in){
                    pointer = qtjambi_cast_with_args<K*>(in, std::forward<Args>(args)...);
                    qtjambi_ownership_decider<K, Args...>::setCppOwnershipAndInvalidate(in, pointer, args...);
                }
                return QScopedPointer<K,T>(pointer);
            }
        }
   }
};

template<bool forward,
         template<typename K, typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_jobject_associative_container_cast;

template<bool forward,
         template<typename K, typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
static constexpr auto find_qtjambi_jobject_associative_container_cast() {
    QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/ContainerCast, is_complete_v< qtjambi_jobject_associative_container_cast<forward, NativeType, is_pointer, is_const, is_reference, is_rvalue, K, T, Args...> >);
    return qtjambi_jobject_associative_container_cast<forward, NativeType, is_pointer, is_const, is_reference, is_rvalue, K, T, Args...>{};
}

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_jobject_template2_cast<forward, jobject, QMap, is_pointer, is_const, is_reference, is_rvalue, K, T, Args...>
    : decltype( find_qtjambi_jobject_associative_container_cast<forward, QMap, is_pointer, is_const, is_reference, is_rvalue, K, T, Args...>() ){
};

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_jobject_template2_cast<forward, jobject, QMultiMap, is_pointer, is_const, is_reference, is_rvalue, K, T, Args...>
    : decltype( find_qtjambi_jobject_associative_container_cast<forward, QMultiMap, is_pointer, is_const, is_reference, is_rvalue, K, T, Args...>() ){
};

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_jobject_template2_cast<forward, jobject, QHash, is_pointer, is_const, is_reference, is_rvalue, K, T, Args...>
    : decltype( find_qtjambi_jobject_associative_container_cast<forward, QHash, is_pointer, is_const, is_reference, is_rvalue, K, T, Args...>() ){
};

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_jobject_template2_cast<forward, jobject, QMultiHash, is_pointer, is_const, is_reference, is_rvalue, K, T, Args...>
    : decltype( find_qtjambi_jobject_associative_container_cast<forward, QMultiHash, is_pointer, is_const, is_reference, is_rvalue, K, T, Args...>() ){
};

#if defined(_MEMORY_) || defined(_LIBCPP_MEMORY) || defined(_GLIBCXX_MEMORY)
template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_jobject_template2_cast<forward,
                                        jobject,
                                        std::unique_ptr, is_pointer, is_const, is_reference, is_rvalue,
                                        K, T, Args...>{
    typedef std::unique_ptr<K,T> NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, jobject> In;
    typedef std::conditional_t<forward, jobject, NativeType_out> Out;

    static Out cast(In in, Args... args){
        if constexpr(forward){
            NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
            K* qo = _in.get();
            jobject o = qtjambi_cast_with_args<jobject>(qo, std::forward<Args>(args)...);
            if(o){
                qtjambi_ownership_decider<K, Args...>::setJavaOwnership(o, qo, args...);
                (void)_in.release();
            }
            return o;
        }else{
            Q_STATIC_ASSERT_X(!(is_reference || is_pointer) ||cast_var_args<Args...>::hasScope, "Cannot cast jobject to std::unique_ptr<K,T> pointer or reference");
            if(!in)
                return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(nullptr, args...);
            NativeType pointer;
            pointer.reset(qtjambi_cast_with_args<K*>(in, std::forward<Args>(args)...));
            qtjambi_ownership_decider<K, Args...>::setCppOwnershipAndInvalidate(in, pointer.get(), args...);
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(pointer), args...);
        }
   }
};
#endif // defined(_MEMORY_)

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename NativeType, typename... Args>
struct qtjambi_jnitype_crono_duration_cast;

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename NativeType, typename... Args>
static constexpr auto find_qtjambi_jnitype_crono_duration_cast() {
    QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/TimeCast, is_complete_v< qtjambi_jnitype_crono_duration_cast<forward, is_pointer, is_const, is_reference, is_rvalue, NativeType, Args...> >);
    return qtjambi_jnitype_crono_duration_cast<forward, is_pointer, is_const, is_reference, is_rvalue, NativeType, Args...>{};
}

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_jobject_template2_cast<forward,
                                      jobject,
                                      std::chrono::duration, is_pointer, is_const, is_reference, is_rvalue,
                                      K, T, Args...> : decltype(find_qtjambi_jnitype_crono_duration_cast<forward, is_pointer, is_const, is_reference, is_rvalue, std::chrono::duration<K,T>, Args...>()){
};

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename NativeType, typename... Args>
struct qtjambi_jnitype_crono_time_point_cast;

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename NativeType, typename... Args>
static constexpr auto find_qtjambi_jnitype_crono_time_point_cast() {
    QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/TimeCast, is_complete_v< qtjambi_jnitype_crono_time_point_cast<forward, is_pointer, is_const, is_reference, is_rvalue, NativeType, Args...> >);
    return qtjambi_jnitype_crono_time_point_cast<forward, is_pointer, is_const, is_reference, is_rvalue, NativeType, Args...>{};
}

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_jobject_template2_cast<forward,
                                      jobject,
                                      std::chrono::time_point, is_pointer, is_const, is_reference, is_rvalue,
                                      K, T, Args...> : decltype(find_qtjambi_jnitype_crono_time_point_cast<forward, is_pointer, is_const, is_reference, is_rvalue, std::chrono::time_point<K,T>, Args...>()){
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_TEMPLATE2_H
