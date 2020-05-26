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

#ifndef QTJAMBI_CAST_UTIL_P_H
#define QTJAMBI_CAST_UTIL_P_H

#include <type_traits>
#include <typeinfo>
#include <utility>
#include "qtjambi_core.h"
#include "qtjambi_typetests.h"

template<class O, class T>
O qtjambi_cast(JNIEnv *env, T&& in);

template<class O, class T>
O qtjambi_cast(JNIEnv *env, QtJambiScope& scope, T&& in);

template<class O, class T>
O qtjambi_cast(JNIEnv *env, T&& in, const char* nativeTypeName);

template<class O, class T>
O qtjambi_cast(JNIEnv *env, QtJambiScope& scope, T&& in, const char* nativeTypeName);

namespace QtJambiPrivate {

template<bool has_scope,
         typename O, bool o_is_arithmetic, bool o_is_enum, bool o_is_jni_type,
         typename T, bool t_is_arithmetic, bool t_is_enum, bool t_is_jni_type,
         bool is_same>
struct qtjambi_cast_decider;

template<typename O>
struct is_jni_type;

template<bool has_scope,class O, class T>
struct qtjambi_scoped_cast
        : qtjambi_cast_decider<has_scope,
        O,
        std::is_arithmetic<typename std::remove_reference<O>::type>::value,
        std::is_enum<typename std::remove_reference<O>::type>::value,
        is_jni_type<typename std::remove_cv<typename std::remove_reference<O>::type>::type>::value,
        T,
        std::is_arithmetic<typename std::remove_reference<T>::type>::value,
        std::is_enum<typename std::remove_reference<T>::type>::value,
        is_jni_type<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::value,
        std::is_same<typename std::remove_reference<O>::type, typename std::remove_reference<T>::type>::value
        >
{
};

template<class T>
struct is_template : std::false_type{
};

template<template<typename...Ts> class C, typename...Ts>
struct is_template<C<Ts...>> : std::true_type{
};

template<typename O>
struct is_lightweight_java_type : std::false_type{
};

template<>
struct is_lightweight_java_type<QMetaObject::Connection> : std::true_type{
};

template<>
struct is_lightweight_java_type<QMetaProperty> : std::true_type{
};

template<>
struct is_lightweight_java_type<QMetaMethod> : std::true_type{
};

template<>
struct is_lightweight_java_type<QMetaEnum> : std::true_type{
};

template<>
struct is_lightweight_java_type<QModelIndex> : std::true_type{
};

template<>
struct is_lightweight_java_type<JIteratorWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JCollectionWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JMapWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JObjectWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JEnumWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<QString> : std::true_type{
};

template<>
struct is_lightweight_java_type<QVariant> : std::true_type{
};

template<typename O>
struct is_jni_type : std::false_type{
};

template<>
struct is_jni_type<jobject> : std::true_type{
};

template<>
struct is_jni_type<jthrowable> : std::true_type{
};

template<>
struct is_jni_type<jstring> : std::true_type{
};

template<>
struct is_jni_type<jclass> : std::true_type{
};

template<>
struct is_jni_type<jarray> : std::true_type{
};

template<>
struct is_jni_type<jobjectArray> : std::true_type{
};

template<>
struct is_jni_type<jintArray> : std::true_type{
};

template<>
struct is_jni_type<jbyteArray> : std::true_type{
};

template<>
struct is_jni_type<jshortArray> : std::true_type{
};

template<>
struct is_jni_type<jlongArray> : std::true_type{
};

template<>
struct is_jni_type<jcharArray> : std::true_type{
};

template<>
struct is_jni_type<jbooleanArray> : std::true_type{
};

template<>
struct is_jni_type<jdoubleArray> : std::true_type{
};

template<>
struct is_jni_type<jfloatArray> : std::true_type{
};

template<typename O>
struct is_jni_array_type : std::false_type{
};

template<>
struct is_jni_array_type<jobjectArray> : std::true_type{
};

template<>
struct is_jni_array_type<jintArray> : std::true_type{
};

template<>
struct is_jni_array_type<jbyteArray> : std::true_type{
};

template<>
struct is_jni_array_type<jshortArray> : std::true_type{
};

template<>
struct is_jni_array_type<jlongArray> : std::true_type{
};

template<>
struct is_jni_array_type<jcharArray> : std::true_type{
};

template<>
struct is_jni_array_type<jbooleanArray> : std::true_type{
};

template<>
struct is_jni_array_type<jdoubleArray> : std::true_type{
};

template<>
struct is_jni_array_type<jfloatArray> : std::true_type{
};

template<typename Iterator>
struct IteratorIncrement{
 static QIteratorIncrementFunction function() { return  [](void* ptr) {
         Iterator* iterator = static_cast<Iterator*>(ptr);
         ++(*iterator);
     }; }
};

template<typename Iterator>
struct IteratorDecrement{
 static QIteratorDecrementFunction function() { return  [](void* ptr) {
         Iterator* iterator = static_cast<Iterator*>(ptr);
         --(*iterator);
     }; }
};

template<typename Iterator, bool supports_less_than>
struct IteratorLessThan{
 static QIteratorLessThanFunction function() { return nullptr; }
};

template<typename Iterator>
struct IteratorLessThan<Iterator, true>{
 static QIteratorLessThanFunction function() { return  [](void* ptr, void* ptr2) -> jboolean {
         Iterator* iterator = static_cast<Iterator*>(ptr);
         Iterator* iterator2 = static_cast<Iterator*>(ptr2);
         return (*iterator)<(*iterator2);
     }; }
};

template<typename Iterator>
struct IteratorEquals{
 static QIteratorEqualsFunction function() { return  [](void* ptr, void* ptr2) -> jboolean {
         Iterator* iterator = static_cast<Iterator*>(ptr);
         Iterator* iterator2 = static_cast<Iterator*>(ptr2);
         return (*iterator)==(*iterator2);
     }; }
};

template<bool is_pointer,typename O>
struct deref_ptr{
};

template<typename O>
struct deref_ptr<false,O>{
    static O& deref(O& o){
        return o;
    }
};

template<typename O>
struct deref_ptr<true,O>{
    static O& deref(O* o){
        Q_ASSERT(o);
        return *o;
    }
};

template<bool is_pointer,typename O>
struct ref_ptr{
};

template<typename O>
struct ref_ptr<true,O>{
    static O* ref(O* o){
        return o;
    }
};

template<typename O>
struct ref_ptr<false,O>{
    static O* ref(O& o){
        return &o;
    }
};

template<bool is_pointer,typename O>
struct deref_ptr_or_default{
};

template<typename O>
struct deref_ptr_or_default<false,O>{
    static O& deref(O& o){
        return o;
    }
};

template<typename O>
struct deref_ptr_or_default<true,O>{
    typedef typename std::add_const<O>::type O_const;
    static O_const& deref(O_const* o){
        return o ? *o : qtjambi_get_default_value<typename std::remove_const<O>::type>();
    }
};

template<typename O, bool has_std_constructor, bool is_const, bool o_is_reference>
struct qtjambi_deref_value{
};

template<typename O, bool is_const, bool o_is_reference>
struct qtjambi_deref_value<O,false, is_const, o_is_reference>{
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type O_const;
    static O_const& deref(JNIEnv *env, O_const* o){
        return checked_deref<O_const>(env, o);
    }
};

template<typename O, bool has_std_constructor>
struct qtjambi_deref_value<O,has_std_constructor,false,true>{
    static O& deref(JNIEnv *env, O* o){
        return checked_deref<O>(env, o);
    }
};

template<typename O>
struct qtjambi_deref_value<O,true,true,true>{
    typedef typename std::add_const<O>::type O_const;
    static O_const& deref(JNIEnv *, O_const* o){
        return o ? *o : qtjambi_get_default_value<typename std::remove_const<O>::type>();
    }
};

template<typename O, bool is_const>
struct qtjambi_deref_value<O,true,is_const,false>{
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type O_const;
    static O deref(JNIEnv *, O_const* o){
        return o ? *o : O();
    }
};

template<typename T, bool comparable>
struct LessThan{
  static LessThanFunction function(){ return nullptr; }
};

template<typename T>
struct LessThan<T,false>{
  static LessThanFunction function(){ return nullptr; }
};

template<typename T>
struct LessThan<T,true>{
  static LessThanFunction function(){
      return [](JNIEnv *env, jobject t1, jobject t2)->bool{
          return qtjambi_cast<T>(env, t1) < qtjambi_cast<T>(env, t2);
      };
  }
};

template<template<typename T> class Container, typename T>
struct IntermediateContainer : Container<T>{
    IntermediateContainer(JNIEnv *env, jobject object, QtJambiScope& scope) : Container<T>(), m_scope(scope), m_object(env->NewWeakGlobalRef(object)){}
    ~IntermediateContainer(){
        if(JNIEnv *env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            if(!env->IsSameObject(m_object, nullptr)){
                qtjambi_collection_clear(env, m_object);
                for(typename Container<T>::const_iterator i = Container<T>::constBegin(); i!=Container<T>::constEnd(); ++i){
                    qtjambi_collection_add(env, m_object, qtjambi_cast<jobject>(env, m_scope, *i));
                }
            }
        }
    }
    QtJambiScope& m_scope;
    jobject m_object;
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct IntermediateBiContainer : Container<K,T>{
    IntermediateBiContainer(JNIEnv *env, jobject object, QtJambiScope& scope) : Container<K,T>(), m_scope(scope), m_object(env->NewWeakGlobalRef(object)){}
    ~IntermediateBiContainer(){
        if(JNIEnv *env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            if(!env->IsSameObject(m_object, nullptr)){
                qtjambi_map_clear(env, m_object);
                for(typename Container<K,T>::const_iterator i = Container<K,T>::constBegin(); i!=Container<K,T>::constEnd(); ++i){
                    jobject key = qtjambi_cast<jobject>(env, m_scope, i.key());
                    jobject val = qtjambi_cast<jobject>(env, m_scope, i.value());
                    qtjambi_map_put(env, m_object, key, val);
                }
            }
        }
    }
    QtJambiScope& m_scope;
    jobject m_object;
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_UTIL_P_H
