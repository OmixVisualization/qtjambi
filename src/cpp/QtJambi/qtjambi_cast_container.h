/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_CAST_CONTAINER_H
#define QTJAMBI_CAST_CONTAINER_H

#include "containeraccess_associative.h"
#include "qtjambiapi_smartpointer.h"
#include "qtjambiapi_container.h"

namespace QtJambiPrivate {

template<bool hasScope, typename... Args>
struct IntermediateData{
protected:
    jweak m_object;
    IntermediateData(jobject object, Args... args)
        : m_object(cast_var_args<Args...>::env(args...)->NewWeakGlobalRef(object)) {
    }
};

template<typename... Args>
struct IntermediateData<true,Args...> : IntermediateData<false, Args...>{
protected:
    using IntermediateData<false, Args...>::m_object;
    QtJambiScope& m_scope;
    IntermediateData(jobject object, Args... args)
        : IntermediateData<false, Args...>(object, args...),
        m_scope(cast_var_args<Args...>::scope(args...)) {
    }
};

template<template<typename T> class Container, typename T, typename... Args>
struct IntermediateSequentialContainer : Container<T>, IntermediateData<cast_var_args<Args...>::hasScope, Args...>{
    typedef IntermediateData<cast_var_args<Args...>::hasScope, Args...> Super;
    IntermediateSequentialContainer(jobject object, Args... args)
        : Container<T>(), Super(object, args...) {}
    ~IntermediateSequentialContainer(){
        QTJAMBI_TRY_ANY{
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    jobject object = env->NewLocalRef(Super::m_object);
                    env->DeleteWeakGlobalRef(Super::m_object);
                    if(!env->IsSameObject(object, nullptr)){
                        QtJambiAPI::clearJavaCollection(env, object);
                        for(typename Container<T>::const_iterator i = Container<T>::constBegin(); i!=Container<T>::constEnd(); ++i){
                            JniLocalFrame f(env, 32);
                            jobject val;
                            if constexpr(cast_var_args<Args...>::hasScope){
                                val = ::qtjambi_cast<jobject>(env, Super::m_scope, *i);
                            }else{
                                val = ::qtjambi_cast<jobject>(env, *i);
                            }
                            QtJambiAPI::addToJavaCollection(env, object, val);
                        }
                    }
                }QTJAMBI_CATCH(const JavaException& exn){
                    exn.raiseInJava(env);
                }QTJAMBI_TRY_END
            }
        }QTJAMBI_CATCH_ANY{
            printf("An unknown exception occurred.\n");
        }QTJAMBI_TRY_END
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, typename... Args>
struct IntermediateAssociativeContainer : Container<K,T>, IntermediateData<cast_var_args<Args...>::hasScope, Args...>{
    typedef IntermediateData<cast_var_args<Args...>::hasScope, Args...> Super;
    IntermediateAssociativeContainer(jobject object, Args... args)
        : Container<K,T>(), Super(object, args...){}
    ~IntermediateAssociativeContainer(){
        QTJAMBI_TRY_ANY{
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    jobject object = env->NewLocalRef(Super::m_object);
                    env->DeleteWeakGlobalRef(Super::m_object);
                    if(!env->IsSameObject(object, nullptr)){
                        QtJambiAPI::clearJavaMap(env, object);
                        for(typename Container<K,T>::const_iterator i = Container<K,T>::constBegin(); i!=Container<K,T>::constEnd(); ++i){
                            JniLocalFrame f(env, 64);
                            jobject key;
                            jobject val;
                            if constexpr(cast_var_args<Args...>::hasScope){
                                key = ::qtjambi_cast<jobject>(env, Super::m_scope, i.key());
                                val = ::qtjambi_cast<jobject>(env, Super::m_scope, i.value());
                            }else{
                                key = ::qtjambi_cast<jobject>(env, i.key());
                                val = ::qtjambi_cast<jobject>(env, i.value());
                            }
                            QtJambiAPI::putJavaMap(env, object, key, val);
                        }
                    }
                }QTJAMBI_CATCH(const JavaException& exn){
                    exn.raiseInJava(env);
                }QTJAMBI_TRY_END
            }
        }QTJAMBI_CATCH_ANY{
            printf("An unknown exception occurred.\n");
        }QTJAMBI_TRY_END
    }
};

template<bool forward,
         template<typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_sequential_container_cast;

template<bool forward, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         template<typename T> class NativeType, bool c_is_const,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, typename... Args>
struct qtjambi_shared_pointer_sequential_container_cast;

#define LISTTYPE(TYPE) QtJambiAPI::ListType::TYPE,

#define QTJAMBI_CONTAINER1_CASTER(TYPE,SUPERTYPE,append)\
template<bool forward,\
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,\
         typename T, typename... Args>\
struct qtjambi_jobject_sequential_container_cast<forward,\
                                                 TYPE, is_pointer, is_const, is_reference, is_rvalue,\
                                                 T, Args...>{\
        typedef TYPE<T> NativeType;\
        typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;\
        typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;\
        typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;\
        typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;\
        typedef std::conditional_t<forward, NativeType_in, jobject> In;\
        typedef std::conditional_t<forward, jobject, NativeType_out> Out;\
            \
        static Out cast(In in, Args... args){\
            auto env = cast_var_args<Args...>::env(args...);\
            if constexpr(forward){\
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");\
                return QtJambiAPI::convert##SUPERTYPE##ToJavaObject(env,\
                                                                    cast_var_args<Args...>::relatedNativeID(args...),\
                                                                    ref_ptr<is_pointer, NativeType_c>::ref(in),\
                                                                    CloneContainer<TYPE,T, is_pointer && !is_const>::function,\
                                                                    DeleteContainer<TYPE,T>::function,\
                                                                    LISTTYPE(TYPE)\
                                                                    SUPERTYPE##Access<T>::newInstance()\
                                                                    );\
        }else{\
                if constexpr(!is_pointer && !is_reference){\
                    if(!in)\
                    return {};\
                    NativeType* pointer{nullptr};\
                    if (ContainerAPI::getAs##TYPE<T>(env, in, pointer)) {\
                        return *pointer;\
                }else{\
                        NativeType list;\
                        jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, in);\
                        while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {\
                            jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);\
                            list.append(qtjambi_cast_with_args<T>(element, std::forward<Args>(args)...));\
                    }\
                        return list;\
                }\
            }else{\
                    NativeType* pointer{nullptr};\
                    if(in){\
                        if (!ContainerAPI::getAs##TYPE<T>(env, in, pointer)) {\
                            if constexpr(!is_reference && !is_pointer){\
                                NativeType result;\
                                jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, in);\
                                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {\
                                    jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);\
                                    result.append(qtjambi_cast_with_args<T>(element, std::forward<Args>(args)...));\
                            }\
                                return result;\
                        }else if constexpr(cast_var_args<Args...>::hasScope){\
                                if(is_const){\
                                    pointer = create<NativeType>();\
                                    cast_var_args<Args...>::scope(args...).addDeletion(pointer);\
                            }else{\
                                    auto ipointer = create<IntermediateSequentialContainer<TYPE,T, Args...>>(in, args...);\
                                    pointer = ipointer;\
                                    cast_var_args<Args...>::scope(args...).addDeletion(ipointer);\
                            }\
                                jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, in);\
                                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {\
                                    jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);\
                                    pointer->append(qtjambi_cast_with_args<T>(element, std::forward<Args>(args)...));\
                            }\
                        }else {\
                                JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );\
                        }\
                    }\
                }\
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(pointer, args...);\
            }\
        }\
    }\
};\
\
template<bool forward, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,\
         bool c_is_const,\
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, typename... Args>\
struct qtjambi_shared_pointer_sequential_container_cast<forward, Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue, TYPE, c_is_const, T, t_is_pointer, t_is_const, t_is_reference, Args...>\
{\
        typedef std::conditional_t<t_is_pointer, std::add_pointer_t<T>, T> T_ptr;\
        typedef std::conditional_t<t_is_const, std::add_const_t<T_ptr>, T_ptr> T_const;\
        typedef std::conditional_t<t_is_reference, std::add_lvalue_reference_t<T_const>, T_const> T_content;\
        typedef std::conditional_t<c_is_const, std::add_const_t<TYPE<T_content>>, TYPE<T_content>> C_content;\
        typedef Pointer<C_content> NativeType;\
        typedef TYPE<T_content> Container;\
        typedef std::conditional_t<p_is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;\
    \
        typedef std::conditional_t<p_is_reference, std::conditional_t<p_is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;\
        typedef std::conditional_t<p_is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;\
        typedef std::conditional_t<p_is_pointer, std::add_pointer_t<NativeType_c>, std::conditional_t<p_is_reference, std::add_lvalue_reference_t<NativeType_c>, NativeType_c>> NativeType_out;\
        typedef std::conditional_t<forward, NativeType_in, jobject> In;\
        typedef std::conditional_t<forward, jobject, NativeType_out> Out;\
    \
        static Out cast(In in, Args... args){\
            auto env = cast_var_args<Args...>::env(args...);\
            if constexpr(forward){\
                return QtJambiAPI::convert##SUPERTYPE##ToJavaObject(env,\
                                                                    *reinterpret_cast<const Pointer<char>*>(&deref_ptr<p_is_pointer, NativeType_c>::deref(in)),\
                                                                    LISTTYPE(TYPE)\
                                                                    SUPERTYPE##Access<T_content>::newInstance()\
                                                                    );\
        }else{\
                if (!in)\
                return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, NativeType, Args...>::convert(nullptr, args...);\
                if (ContainerAPI::test##TYPE<T_content>(env, in)) {\
                    NativeType pointer = QtJambiAPI::convertJavaObjectToSmartPointer<Pointer,C_content>(env, in);\
                    return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, NativeType, Args...>::convert(std::move(pointer), args...);\
            } else {\
                    typedef IntermediateSequentialContainer<TYPE,T_content,Args...> IContainer;\
                    NativeType pointer;\
                    Container* list;\
                    if(c_is_const){\
                        list = create<Container>();\
                        pointer.reset(list);\
                }else{\
                        IContainer* ilist = create<IContainer>(in, args...);\
                        list =ilist;\
                        pointer = Pointer<IContainer>(ilist);\
                }\
                    jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, in);\
                    while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {\
                        jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);\
                        list->append(qtjambi_cast_with_args<T_content>(element, std::forward<Args>(args)...));\
                }\
                    return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, NativeType, Args...>::convert(std::move(pointer), args...);\
            }\
        }\
    }\
};

#define STATICASSERT
QTJAMBI_CONTAINER1_CASTER(QList,QList,append)
QTJAMBI_CONTAINER1_CASTER(QQueue,QList,append)
QTJAMBI_CONTAINER1_CASTER(QStack,QList,append)

#undef LISTTYPE
#define LISTTYPE(TYPE)

QTJAMBI_CONTAINER1_CASTER(QSet,QSet,insert)
#undef LISTTYPE
#undef STATICASSERT
#undef QTJAMBI_CONTAINER1_CASTER

template<bool forward,
         template<typename K, typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_jobject_associative_container_cast;

template<bool forward, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,
         template<typename K, typename T> class NativeType, bool c_is_const,
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, typename... Args>
struct qtjambi_shared_pointer_associative_container_cast;

#define QTJAMBI_CONTAINER2_CASTER(TYPE)\
template<bool forward, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,\
         typename K, typename T, typename... Args>\
    struct qtjambi_jobject_associative_container_cast<forward,\
                                                      TYPE, is_pointer, is_const, is_reference, is_rvalue,\
                                                      K, T, Args...>{\
        typedef TYPE<K,T> NativeType;\
        typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;\
        typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;\
        typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;\
        typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;\
        typedef std::conditional_t<forward, NativeType_in, jobject> In;\
        typedef std::conditional_t<forward, jobject, NativeType_out> Out;\
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
                                    result.insert(qtjambi_cast_with_args<K>(key, std::forward<Args>(args)...), qtjambi_cast_with_args<T>(val, std::forward<Args>(args)...));\
                            }\
                                return result;\
                        }else if constexpr(cast_var_args<Args...>::hasScope){\
                                if(is_const){\
                                    pointer = create<NativeType>();\
                                    cast_var_args<Args...>::scope(args...).addDeletion(pointer);\
                            }else{\
                                    auto ipointer = create<IntermediateAssociativeContainer<TYPE,K,T,Args...>>(in, args...);\
                                    pointer = ipointer;\
                                    cast_var_args<Args...>::scope(args...).addDeletion(ipointer);\
                            }\
                                jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, in);\
                                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {\
                                    jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);\
                                    jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);\
                                    jobject val = QtJambiAPI::valueOfJavaMapEntry(env, entry);\
                                    pointer->insert(qtjambi_cast_with_args<K>(key, std::forward<Args>(args)...), qtjambi_cast_with_args<T>(val, std::forward<Args>(args)...));\
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
                            map.insert(qtjambi_cast_with_args<K>(key, std::forward<Args>(args)...), qtjambi_cast_with_args<T>(val, std::forward<Args>(args)...));\
                    }\
                        return map;\
                }\
            }\
        }\
    }\
};\
\
template<bool forward, template<typename> class Pointer, bool p_is_pointer, bool p_is_const, bool p_is_reference, bool p_is_rvalue,\
         bool c_is_const,\
         typename K, bool k_is_pointer, bool k_is_const, bool k_is_reference,\
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, typename... Args>\
    struct qtjambi_shared_pointer_associative_container_cast<forward,\
                                                             Pointer, p_is_pointer, p_is_const, p_is_reference, p_is_rvalue,\
                                                             TYPE, c_is_const,\
                                                             K, k_is_pointer, k_is_const, k_is_reference,\
                                                             T, t_is_pointer, t_is_const, t_is_reference, Args...>{\
        typedef std::conditional_t<k_is_pointer, std::add_pointer_t<K>, K> K_ptr;\
        typedef std::conditional_t<k_is_const, std::add_const_t<K_ptr>, K_ptr> K_const;\
        typedef std::conditional_t<k_is_reference, std::add_lvalue_reference_t<K_const>, K_const> K_content;\
        typedef std::conditional_t<t_is_pointer, std::add_pointer_t<T>, T> T_ptr;\
        typedef std::conditional_t<t_is_const, std::add_const_t<T_ptr>, T_ptr> T_const;\
        typedef std::conditional_t<t_is_reference, std::add_lvalue_reference_t<T_const>, T_const> T_content;\
        typedef TYPE<K_content,T_content> Container;\
        typedef std::conditional_t<c_is_const, std::add_const_t<Container>, Container> C_content;\
        typedef Pointer<C_content> NativeType;\
        typedef std::conditional_t<p_is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;\
    \
        typedef std::conditional_t<p_is_reference, std::conditional_t<p_is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;\
        typedef std::conditional_t<p_is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;\
        typedef std::conditional_t<p_is_pointer, std::add_pointer_t<NativeType_c>, std::conditional_t<p_is_reference, std::add_lvalue_reference_t<NativeType_c>, NativeType_c>> NativeType_out;\
        typedef std::conditional_t<forward, NativeType_in, jobject> In;\
        typedef std::conditional_t<forward, jobject, NativeType_out> Out;\
    \
        static Out cast(In in, Args... args){\
            auto env = cast_var_args<Args...>::env(args...);\
            if constexpr(forward){\
                return QtJambiAPI::convert##TYPE##ToJavaObject(env,\
                                                               *reinterpret_cast<const Pointer<char>*>(&deref_ptr<p_is_pointer, NativeType_c>::deref(in)),\
                                                               TYPE##Access<K_content,T_content>::newInstance()\
                                                               );\
        }else{\
                if (!in)\
                return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, NativeType, Args...>::convert(nullptr, args...);\
                if (ContainerAPI::test##TYPE<K_content,T_content>(env, in)) {\
                    NativeType pointer = QtJambiAPI::convertJavaObjectToSmartPointer<Pointer,C_content>(env, in);\
                    return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, NativeType, Args...>::convert(std::move(pointer), args...);\
            } else {\
                    typedef IntermediateAssociativeContainer<TYPE,K_content,T_content,Args...> IContainer;\
                    NativeType pointer;\
                    Container* map;\
                    if(c_is_const){\
                        map = create<Container>();\
                        pointer.reset(map);\
                }else{\
                        IContainer* imap = create<IContainer>(in, args...);\
                        map = imap;\
                        pointer = Pointer<IContainer>(imap);\
                }\
                    jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, in);\
                    while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {\
                        jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);\
                        jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);\
                        jobject val = QtJambiAPI::valueOfJavaMapEntry(env, entry);\
                        map->insert(qtjambi_cast_with_args<K_content>(key, std::forward<Args>(args)...), qtjambi_cast_with_args<T_content>(val, std::forward<Args>(args)...));\
                }\
                    return pointer_ref_or_clone_decider<p_is_pointer, p_is_const, p_is_reference, NativeType, Args...>::convert(std::move(pointer), args...);\
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

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_CONTAINER_H
