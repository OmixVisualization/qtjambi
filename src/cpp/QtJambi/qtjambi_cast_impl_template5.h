#ifndef QTJAMBI_CAST_IMPL_TEMPLATE5_H
#define QTJAMBI_CAST_IMPL_TEMPLATE5_H

#include "qtjambi_cast_impl_util.h"
#include "qtjambi_cast_impl_container_associative.h"

namespace QtJambiPrivate {

template<bool forward, bool has_scope,
         typename JniType,
         template<typename K, typename T, typename A, typename B, typename C> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A, typename B, typename C>
struct qtjambi_jnitype_template5_cast{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

#if defined(_UNORDERED_MAP_) || defined(_UNORDERED_MAP) || defined(_LIBCPP_UNORDERED_MAP) || defined(_GLIBCXX_UNORDERED_MAP)
template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A, typename B, typename C>
struct qtjambi_jnitype_template5_cast<true, has_scope,
                                 jobject,
                                 std::unordered_map, is_pointer, is_const, is_reference,
                                 K, T, A, B, C>{
    typedef std::unordered_map<K, T, A, B, C> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        jobject list = QtJambiAPI::newJavaHashMap(env, jint(_in.size()));
        for (auto it = _in.cbegin(); it != _in.cend(); ++it) {
            const auto& _first = it->first;
            const auto& _second = it->second;
            jobject first = qtjambi_scoped_cast<has_scope,jobject,decltype(_first)>::cast(env, _first, nullptr, scope);
            jobject second = qtjambi_scoped_cast<has_scope,jobject,decltype(_second)>::cast(env, _second, nullptr, scope);
            QtJambiAPI::putJavaMap(env, list, first, second);
        }
        return list;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A, typename B, typename C>
struct qtjambi_jnitype_template5_cast<false, has_scope,
                                 jobject,
                                 std::unordered_map, is_pointer, is_const, is_reference,
                                 K, T, A, B, C>{
    typedef std::unordered_map<K, T, A, B, C> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        std::unique_ptr<NativeType> map;
        if(in){
            map.reset(new NativeType());
            jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, in);
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);
                jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);
                jobject value = QtJambiAPI::valueOfJavaMapEntry(env, entry);
                map->insert({qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, value, nullptr, scope)});
            }
        }
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, map);
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A, typename B, typename C>
struct qtjambi_jnitype_template5_cast<true, has_scope,
                                 jobject,
                                 std::unordered_multimap, is_pointer, is_const, is_reference,
                                 K, T, A, B, C>{
    typedef std::unordered_multimap<K, T, A, B, C> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        jobject list = QtJambiAPI::newJavaHashMap(env, jint(_in.size()));
        for (auto it = _in.cbegin(); it != _in.cend(); ++it) {
            const auto& _first = it->first;
            const auto& _second = it->second;
            jobject first = qtjambi_scoped_cast<has_scope,jobject,decltype(_first)>::cast(env, _first, nullptr, scope);
            jobject second = qtjambi_scoped_cast<has_scope,jobject,decltype(_second)>::cast(env, _second, nullptr, scope);
            QtJambiAPI::putJavaMap(env, list, first, second);
        }
        return list;
    }
};

template<bool has_scope,
         bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T, typename A, typename B, typename C>
struct qtjambi_jnitype_template5_cast<false, has_scope,
                                 jobject,
                                 std::unordered_multimap, is_pointer, is_const, is_reference,
                                 K, T, A, B, C>{
    typedef std::unordered_multimap<K, T, A, B, C> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        std::unique_ptr<NativeType> map;
        if(in){
            map.reset(new NativeType());
            jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, in);
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);
                jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);
                jobject value = QtJambiAPI::valueOfJavaMapEntry(env, entry);
                map->insert({qtjambi_scoped_cast<has_scope,K,jobject>::cast(env, key, nullptr, scope), qtjambi_scoped_cast<has_scope,T,jobject>::cast(env, value, nullptr, scope)});
            }
        }
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, map);
    }
};
#endif

}
#endif // QTJAMBI_CAST_IMPL_TEMPLATE5_H
