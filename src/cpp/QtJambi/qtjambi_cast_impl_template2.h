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

#ifndef QTJAMBI_CAST_IMPL_TEMPLATE2_H
#define QTJAMBI_CAST_IMPL_TEMPLATE2_H

#include "qtjambi_cast_impl_util.h"
#include "qtjambi_cast_impl_container_associative.h"
#include "qtjambi_cast_impl_time.h"

namespace QtJambiPrivate {

template<bool forward,
         typename JniType,
         template<typename K, typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_jobject_template2_cast{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType<K,T>>::type, NativeType<K,T>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
};

template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, class _Alloc, typename... Args>
struct qtjambi_jobject_template2_cast<forward,
                                 jobject,
                                 std::vector, is_pointer, is_const, is_reference, is_rvalue,
                                 T, _Alloc, Args...>{
    typedef std::vector<T,_Alloc> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
            jobject list = QtJambiAPI::newJavaArrayList(env, jint(_in.size()));
            for(const auto& entry : _in){
                jobject _entry = qtjambi_cast<jobject,decltype(entry), Args...>::cast(entry, args...);
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
                list.push_back(qtjambi_cast<T,jobject, Args...>::cast(element, args...));
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
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
            jobject list = QtJambiAPI::newJavaArrayList(env, jint(_in.size()));
            for(const auto& entry : _in){
                jobject _entry = qtjambi_cast<jobject,decltype(entry), Args...>::cast(entry, args...);
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
                list.push_back(qtjambi_cast<T,jobject, Args...>::cast(element, args...));
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
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
            jobject list = QtJambiAPI::newJavaArrayList(env, jint(_in.size()));
            for(const auto& entry : _in){
                jobject _entry = qtjambi_cast<jobject,decltype(entry), Args...>::cast(entry, args...);
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
                list.push_back(qtjambi_cast<T,jobject, Args...>::cast(element, args...));
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
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<is_const, typename std::add_const<K>::type, K>::type K_const;
    typedef typename std::conditional<is_const, typename std::add_const<T>::type, T>::type T_const;
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
            return QtJambiAPI::newQPair(env,
                                    qtjambi_cast<jobject,K_const, Args...>::cast(_in.first, args...),
                                    qtjambi_cast<jobject,T_const, Args...>::cast(_in.second, args...));
        }else{
            if(!in)
                return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(nullptr, args...);
            jobject first = QtJambiAPI::getQPairFirst(env, in);
            jobject second = QtJambiAPI::getQPairSecond(env, in);
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(
                                std::pair<K,T>{qtjambi_cast<K,jobject, Args...>::cast(first, args...),
                                                qtjambi_cast<T,jobject, Args...>::cast(second, args...)}, args...);
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
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        if constexpr(forward){
            NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
            K* qo = _in.take();
            jobject o = qtjambi_cast<jobject,K*, Args...>::cast(qo, args...);
            qtjambi_ownership_decider<K, Args...>::setJavaOwnership(o, qo, args...);
            return o;
        }else{
            if constexpr(is_pointer || is_reference){
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast without scope");
                NativeType* scp = new NativeType(qtjambi_cast<K*,jobject, Args...>::cast(in, args...));
                cast_var_args<Args...>::scope(args...).addDeletion(scp);
                qtjambi_ownership_decider<K, Args...>::setCppOwnershipAndInvalidate(in, scp->get(), args...);
                return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(scp, args...);
            }else{
                K* pointer{nullptr};
                if(in){
                    pointer = qtjambi_cast<K*,jobject, Args...>::cast(in, args...);
                    qtjambi_ownership_decider<K, Args...>::setCppOwnershipAndInvalidate(in, pointer, args...);
                }
                return QScopedPointer<K,T>(pointer);
            }
        }
   }
};

#ifdef QURL_H
template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_template2_cast<forward,
                                      jobject,
                                      QUrlTwoFlags, is_pointer, is_const, is_reference, is_rvalue,
                                      QUrl::UrlFormattingOption, QUrl::ComponentFormattingOption, Args...>{
    typedef QUrl::FormattingOptions NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
            return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &_in, typeid(QUrl::FormattingOptions));
        }else{
            if constexpr(is_pointer || is_reference){
                Q_STATIC_ASSERT_X(!is_reference || cast_var_args<Args...>::hasScope, "Cannot cast to QUrl::FormattingOptions& without scope");
                Q_STATIC_ASSERT_X(!is_pointer || cast_var_args<Args...>::hasScope, "Cannot cast to QUrl::FormattingOptions* without scope");
                NativeType* result = new NativeType(0);
                if(!QtJambiAPI::convertJavaToNative(env, in, result, typeid(NativeType))){
                    delete result;
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null")).arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                }
                if constexpr(is_const){
                    cast_var_args<Args...>::scope(args...).addDeletion(result);
                }else{
#if defined(QTJAMBI_JOBJECTWRAPPER_H)
                    cast_var_args<Args...>::scope(args...).addFinalAction([result, wrapper = JObjectWrapper(env, in)](){
                        if(JniEnvironment env{128}){
                            jobject o = wrapper.object(env);
                            QtJambiAPI::setFlagsValue(env, o, jint(int(*result)));
                        }
                        delete result;
                    });
#else
                    cast_var_args<Args...>::scope(args...).addFinalAction([result, env, in](){
                        QtJambiAPI::setFlagsValue(env, in, jint(int(*result)));
                        delete result;
                    });
#endif //defined(QTJAMBI_JOBJECTWRAPPER_H)
                }

                if constexpr(is_pointer){
                    return result;
                }else{
                    return qtjambi_deref_value<NativeType, is_default_constructible<NativeType>::value, is_copy_constructible<NativeType>::value, is_const, is_reference>::deref(env, result);
                }
            }else{
                NativeType result;
                if(!QtJambiAPI::convertJavaToNative(env, in, &result, typeid(NativeType))){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to QUrl::FormattingOptions").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null")) QTJAMBI_STACKTRACEINFO );
                }
                return result;
            }
        }
    }
};
#endif //QURL_H

#define QTJAMBI_CONTAINER2_CASTER(TYPE)\
template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,\
         typename K, typename T, typename... Args>\
struct qtjambi_jobject_template2_cast<forward,\
                                        jobject,\
                                        TYPE, is_pointer, is_const, is_reference, is_rvalue,\
                                        K, T, Args...>{\
    typedef TYPE<K,T> NativeType;\
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;\
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;\
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;\
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;\
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;\
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;\
    \
    static Out cast(In in, Args... args){\
        auto env = cast_var_args<Args...>::env(args...);\
        if constexpr(forward){\
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");\
            return  QtJambiAPI::convert##TYPE##ToJavaObject(env,\
                                                       cast_var_args<Args...>::relatedNativeID(args...),\
                                                       ref_ptr<is_pointer, NativeType_c>::ref(in),\
                                                       CloneAssociativeContainer<TYPE,K,T, is_pointer && !is_const>::function,\
                                                       DeleteAssociativeContainer<TYPE,K,T>::function,\
                                                       TYPE##Access<K,T>::newInstance()\
                              );\
        }else{\
            if constexpr(is_pointer || is_reference){\
                NativeType* pointer{nullptr};\
                if(in){\
                    if (!ContainerAPI::getAs##TYPE<K,T>(env, in, pointer)) {\
                        if constexpr(!is_reference && !is_pointer){\
                            NativeType result;\
                            jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, in);\
                            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {\
                                jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);\
                                jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);\
                                jobject val = QtJambiAPI::valueOfJavaMapEntry(env, entry);\
                                result.insert(qtjambi_cast<K,jobject, Args...>::cast(key, args...), qtjambi_cast<T,jobject, Args...>::cast(val, args...));\
                            }\
                            return result;\
                        }else if constexpr(cast_var_args<Args...>::hasScope){\
                            if(is_const){\
                                pointer = new NativeType();\
                                cast_var_args<Args...>::scope(args...).addDeletion(pointer);\
                            }else{\
                                auto ipointer = new IntermediateAssociativeContainer<TYPE,K,T,Args...>(in, args...);\
                                pointer = ipointer;\
                                cast_var_args<Args...>::scope(args...).addDeletion(ipointer);\
                            }\
                            jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, in);\
                            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {\
                                jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);\
                                jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);\
                                jobject val = QtJambiAPI::valueOfJavaMapEntry(env, entry);\
                                pointer->insert(qtjambi_cast<K,jobject, Args...>::cast(key, args...), qtjambi_cast<T,jobject, Args...>::cast(val, args...));\
                            }\
                        }else {\
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );\
                        }\
                    }\
                }\
                return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(pointer, args...);\
            }else{\
                if(!in)\
                    return {};\
                NativeType* pointer{nullptr};\
                if (ContainerAPI::getAs##TYPE<K,T>(env, in, pointer)) {\
                    return *pointer;\
                } else {\
                    NativeType map;\
                    jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, in);\
                    while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {\
                        jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);\
                        jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);\
                        jobject val = QtJambiAPI::valueOfJavaMapEntry(env, entry);\
                        map.insert(qtjambi_cast<K,jobject, Args...>::cast(key, args...), qtjambi_cast<T,jobject, Args...>::cast(val, args...));\
                    }\
                    return map;\
                }\
            }\
        }\
     }\
};

#define STATICASSERT Q_STATIC_ASSERT_X(supports_less_than<K>::value, "Key type of map does not support operator <.");
QTJAMBI_CONTAINER2_CASTER(QMap)
QTJAMBI_CONTAINER2_CASTER(QMultiMap)
#undef STATICASSERT
#define STATICASSERT
QTJAMBI_CONTAINER2_CASTER(QHash)
QTJAMBI_CONTAINER2_CASTER(QMultiHash)
#undef QTJAMBI_CONTAINER2_CASTER
#undef STATICASSERT

#if defined(_MEMORY_) || defined(_LIBCPP_MEMORY) || defined(_GLIBCXX_MEMORY)
template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_jobject_template2_cast<forward,
                                        jobject,
                                        std::unique_ptr, is_pointer, is_const, is_reference, is_rvalue,
                                        K, T, Args...>{
    typedef std::unique_ptr<K,T> NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        if constexpr(forward){
            NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
            K* qo = _in.get();
            jobject o = qtjambi_cast<jobject,K*, Args...>::cast(qo, args...);
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
            pointer.reset(qtjambi_cast<K*,jobject, Args...>::cast(in, args...));
            qtjambi_ownership_decider<K, Args...>::setCppOwnershipAndInvalidate(in, pointer.get(), args...);
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(pointer), args...);
        }
   }
};
#endif // defined(_MEMORY_)

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_jobject_template2_cast<forward,
                                      jobject,
                                      std::chrono::duration, is_pointer, is_const, is_reference, is_rvalue,
                                      K, T, Args...> : qtjambi_jnitype_crono_duration_cast<forward, is_pointer, is_const, is_reference, is_rvalue, std::chrono::duration<K,T>, Args...>{
};


template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_jobject_template2_cast<forward,
                                      jobject,
                                      std::chrono::time_point, is_pointer, is_const, is_reference, is_rvalue,
                                      K, T, Args...> : qtjambi_jnitype_crono_time_point_cast<forward, is_pointer, is_const, is_reference, is_rvalue, std::chrono::time_point<K,T>, Args...>{
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_IMPL_TEMPLATE2_H
