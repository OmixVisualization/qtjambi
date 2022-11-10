/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include <memory>
#include "qtjambi_global.h"
#include "qtjambi_typetests.h"

namespace QtJambiPrivate {

template<typename T>
struct qtjambi_cast_types{
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;
};

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

#ifdef QABSTRACTITEMMODEL_H
template<>
struct is_lightweight_java_type<QModelIndex> : std::true_type{
};
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
template<>
struct is_lightweight_java_type<QModelRoleDataSpan> : std::true_type{
};
#endif
#endif

#ifdef QOBJECTWRAPPER_H
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
struct is_lightweight_java_type<JObjectArrayWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JIntArrayWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JLongArrayWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JShortArrayWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JByteArrayWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JBooleanArrayWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JCharArrayWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JDoubleArrayWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JFloatArrayWrapper> : std::true_type{
};
#endif

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

template<bool is_reference,typename O>
struct ptr2ref{
};

template<typename O>
struct ptr2ref<false,O>{
    static constexpr O* value(JNIEnv *, O* o){
        return o;
    }
};

template<typename O>
struct ptr2ref<true,O>{
    static O& value(JNIEnv *env, O* o){
        qtjambi_check_resource<O>(env, o);
        return *o;
    }
};

template<bool is_pointer,typename O>
struct deref_ptr{
};

template<typename O>
struct deref_ptr<false,O>{
    static constexpr O& deref(O& o){
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
    static constexpr O* ref(O* o){
        return o;
    }
};

template<typename O>
struct ref_ptr<false,O>{
    static constexpr O* ref(O& o){
        return &o;
    }
};

template<bool is_pointer,typename O>
struct deref_ptr_or_default{
};

template<typename O>
struct deref_ptr_or_default<false,O>{
    static constexpr O& deref(O& o){
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

template<typename O, bool has_std_constructor, bool has_copy_constructor, bool is_const, bool o_is_reference>
struct qtjambi_deref_value{
    Q_STATIC_ASSERT_X(!has_std_constructor && false, "Cannot deref type");
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type O_const;
    static O_const& deref(JNIEnv *, O* o){
        return *o;
    }
};

template<typename O, bool is_const>
struct qtjambi_deref_value<O,false, true, is_const, false>{
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type O_const;
    static O_const deref(JNIEnv *env, O* o){
        qtjambi_check_resource<O>(env, o);
        return O(*o);
    }
};

template<typename O, bool has_copy_constructor, bool is_const>
struct qtjambi_deref_value<O,false, has_copy_constructor, is_const, true>{
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type O_const;
    static O_const& deref(JNIEnv *env, O* o){
        qtjambi_check_resource<O>(env, o);
        return *o;
    }
};

template<typename O, bool has_copy_constructor>
struct qtjambi_deref_value<O,true, has_copy_constructor,true,true>{
    typedef typename std::add_const<O>::type O_const;
    static O_const& deref(JNIEnv *, O_const* o){
        return o ? *o : qtjambi_get_default_value<typename std::remove_const<O>::type>();
    }
};

template<typename O, bool has_copy_constructor>
struct qtjambi_deref_value<O,true,has_copy_constructor,false,true>{
    static O& deref(JNIEnv *env, O* o){
        qtjambi_check_resource<O>(env, o);
        return *o;
    }
};

template<typename O, bool is_const>
struct qtjambi_deref_value<O,true,true,is_const,false>{
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type O_const;
    static O deref(JNIEnv *, O_const* o){
        return o ? O(*o) : O();
    }
};

template<typename T, bool comparable>
struct LessThan{
  static constexpr LessThanFunction function = nullptr;
};

template<typename T>
struct LessThan<T,true>{
  static bool function(JNIEnv *env, jobject t1, jobject t2){
        return qtjambi_scoped_cast<false,T,jobject>::cast(env, t1, nullptr, nullptr) < qtjambi_scoped_cast<false,T,jobject>::cast(env, t2, nullptr, nullptr);
    }
};

template<bool is_pointer, bool is_const, bool is_reference, bool buffer, class Container>
struct create_container_pointer{
    static Container create(JNIEnv *, QtJambiScope*, std::nullptr_t){
        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
    }
    static Container create(JNIEnv *, QtJambiScope*, Container*){
        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
    }
    static Container create(JNIEnv *, QtJambiScope*, const Container*){
        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
    }
    static Container create(JNIEnv *, QtJambiScope*, std::unique_ptr<Container>&){
        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
    }
};

template<bool is_const, class Container>
struct create_container_pointer<true, is_const, false, false, Container>{
    typedef typename std::conditional<is_const, typename std::add_const<Container>::type, Container>::type Container_const;
    static Container* create(JNIEnv *, QtJambiScope*, std::nullptr_t){
        return nullptr;
    }
    static Container* create(JNIEnv *, QtJambiScope*, Container* t){
        return t;
    }
    static Container_const* create(JNIEnv *, QtJambiScope*, const Container* t){
        Q_STATIC_ASSERT_X(is_const, "Cannot cast const pointer to non-const pointer without scope.");
        return t;
    }
    static Container* create(JNIEnv *, QtJambiScope*, std::unique_ptr<Container>& t){
        return t.release();
    }
};

template<bool is_const, class Container>
struct create_container_pointer<true, is_const, false, true, Container>{
    static Container* create(JNIEnv *, QtJambiScope*, std::nullptr_t){
        return nullptr;
    }
    static Container* create(JNIEnv *, QtJambiScope*, Container* t){
        return t;
    }
    static Container* create(JNIEnv *env, QtJambiScope* scope, const Container* t){
        if(!scope)
            JavaException::raiseError(env, QStringLiteral("Cannot cast to %1* without scope.").arg(QLatin1String(qtjambi_type_name(typeid(Container)))) QTJAMBI_STACKTRACEINFO );
        if(t){
            Container* result = new Container(*t);
            scope->addDeletion(result);
            return result;
        }else{
            return nullptr;
        }
    }
    static Container* create(JNIEnv *, QtJambiScope*, std::unique_ptr<Container>& t){
        return t.release();
    }
};

template<bool is_const, bool buffer, class Container>
struct create_container_pointer<false, is_const, false, buffer, Container>{
    static Container create(JNIEnv *, QtJambiScope*, const Container* t){
        return t ? *t : Container();
    }
    static Container create(JNIEnv *, QtJambiScope*, std::unique_ptr<Container>& t){
        return t ? *t : Container();
    }
};

template<bool buffer, class Container>
struct create_container_pointer<false, true, true, buffer, Container>{
    static const Container& create(JNIEnv *, QtJambiScope*, const Container* t){
        return t ? *t : qtjambi_get_default_value<Container>();
    }
    static const Container& create(JNIEnv *env, QtJambiScope* scope, std::unique_ptr<Container>& ptr){
        if(ptr && !ptr->empty() && !scope)
            JavaException::raiseError(env, QStringLiteral("Cannot cast to %1& without scope.").arg(QLatin1String(qtjambi_type_name(typeid(Container)))) QTJAMBI_STACKTRACEINFO );
        if(ptr && scope){
            Container* t = ptr.release();
            scope->addDeletion(t);
            return *t;
        }
        return qtjambi_get_default_value<Container>();
    }
};

template<class Container>
struct create_container_pointer<false, false, true, true, Container>{
    static Container& create(JNIEnv * env, QtJambiScope* scope, std::nullptr_t){
        if(!scope)
            JavaException::raiseError(env, QStringLiteral("Cannot cast to %1& without scope.").arg(QLatin1String(qtjambi_type_name(typeid(Container)))) QTJAMBI_STACKTRACEINFO );
        Container* result = new Container();
        scope->addDeletion(result);
        return *result;
    }
    static Container& create(JNIEnv *env, QtJambiScope* scope, Container* t){
        if(!scope)
            JavaException::raiseError(env, QStringLiteral("Cannot cast to %1& without scope.").arg(QLatin1String(qtjambi_type_name(typeid(Container)))) QTJAMBI_STACKTRACEINFO );
        if(t){
            scope->addDeletion(t);
            return *t;
        }else{
            Container* result = new Container();
            scope->addDeletion(result);
            return *result;
        }
    }

    static const Container& create(JNIEnv *env, QtJambiScope* scope, const Container* t){
        if(!scope)
            JavaException::raiseError(env, QStringLiteral("Cannot cast to %1& without scope.").arg(QLatin1String(qtjambi_type_name(typeid(Container)))) QTJAMBI_STACKTRACEINFO );
        if(t){
            scope->addDeletion(t);
            return *t;
        }else{
            Container* result = new Container();
            scope->addDeletion(result);
            return *result;
        }
    }

    static Container& create(JNIEnv *env, QtJambiScope* scope, std::unique_ptr<Container>& ptr){
        if(!scope)
            JavaException::raiseError(env, QStringLiteral("Cannot cast to %1& without scope.").arg(QLatin1String(qtjambi_type_name(typeid(Container)))) QTJAMBI_STACKTRACEINFO );
        Container* t = ptr.release();
        if(t){
            scope->addDeletion(t);
            return *t;
        }else{
            Container* result = new Container();
            scope->addDeletion(result);
            return *result;
        }
    }
};

template<class Container>
struct create_container_pointer<false, false, true, false, Container>{
    typedef Container Container_KT;
    static Container& create(JNIEnv *env, QtJambiScope*, Container* t){
        if(!t){
            JavaException::raiseNullPointerException(env, QStringLiteral("%1 is null.").arg(QLatin1String(qtjambi_type_name(typeid(Container)))) QTJAMBI_STACKTRACEINFO );
        }
        return *t;
    }
    static Container& create(JNIEnv *, QtJambiScope*, std::unique_ptr<Container>& t){
        Q_STATIC_ASSERT_X(false && std::is_pointer<Container_KT>::value, "Cannot cast to Container reference without scope.");
        return *t;
    }
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_UTIL_P_H
