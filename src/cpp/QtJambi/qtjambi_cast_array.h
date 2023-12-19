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

#ifndef QTJAMBI_CAST_ARRAY_H
#define QTJAMBI_CAST_ARRAY_H

#include "qtjambi_cast_impl_array.h"

template<class O, class T, class I>
O qtjambi_array_cast(JNIEnv *env, QtJambiScope& scope, T in, I&& size, const char* nativeTypeName){
    typedef typename std::remove_reference<I>::type I_noref;
    Q_STATIC_ASSERT_X(!std::is_pointer<I_noref>::value, "Integer type variable required as size.");
    Q_STATIC_ASSERT_X(std::is_integral<I_noref>::value, "Integer type variable required as size.");
    Q_STATIC_ASSERT_X(!QtJambiPrivate::is_jni_array_type<T>::value || !std::is_const<typename std::remove_reference<I_noref>::type>::value, "Cannot cast native array from java array without non-const size argument.");
    return QtJambiPrivate::qtjambi_array_cast_decider<
            true,
            O, QtJambiPrivate::is_jni_array_type<O>::value,
            T, QtJambiPrivate::is_jni_array_type<T>::value,
            I_noref
        >::cast(env, in, size, nativeTypeName, &scope);
}

template<class O, class T, class I>
O qtjambi_array_cast(JNIEnv *env, QtJambiScope& scope, T in, I&& size){
    typedef typename std::remove_reference<I>::type I_noref;
    Q_STATIC_ASSERT_X(!std::is_pointer<I_noref>::value, "Integer type variable required as size.");
    Q_STATIC_ASSERT_X(std::is_integral<I_noref>::value, "Integer type variable required as size.");
    Q_STATIC_ASSERT_X(!QtJambiPrivate::is_jni_array_type<T>::value || !std::is_const<typename std::remove_reference<I_noref>::type>::value, "Cannot cast native array from java array without non-const size argument.");
    return QtJambiPrivate::qtjambi_array_cast_decider<
            true,
            O, QtJambiPrivate::is_jni_array_type<O>::value,
            T, QtJambiPrivate::is_jni_array_type<T>::value,
            I_noref
        >::cast(env, in, size, nullptr, &scope);
}

template<class O, class T, class I>
O qtjambi_array_cast(JNIEnv *env, T in, I&& size, const char* nativeTypeName){
    typedef typename std::remove_reference<I>::type I_noref;
    Q_STATIC_ASSERT_X(!std::is_pointer<I_noref>::value, "Integer type variable required as size.");
    Q_STATIC_ASSERT_X(std::is_integral<I_noref>::value, "Integer type variable required as size.");
    Q_STATIC_ASSERT_X(!QtJambiPrivate::is_jni_array_type<T>::value || !std::is_const<typename std::remove_reference<I_noref>::type>::value, "Cannot cast native array from java array without non-const size argument.");
    return QtJambiPrivate::qtjambi_array_cast_decider<
        false,
        O, QtJambiPrivate::is_jni_array_type<O>::value,
        T, QtJambiPrivate::is_jni_array_type<T>::value,
        I_noref
        >::cast(env, in, size, nativeTypeName, nullptr);
}

template<class O, class T, class I>
O qtjambi_array_cast(JNIEnv *env, T in, I&& size){
    typedef typename std::remove_reference<I>::type I_noref;
    Q_STATIC_ASSERT_X(!std::is_pointer<I_noref>::value, "Integer type variable required as size.");
    Q_STATIC_ASSERT_X(std::is_integral<I_noref>::value, "Integer type variable required as size.");
    Q_STATIC_ASSERT_X(!QtJambiPrivate::is_jni_array_type<T>::value || !std::is_const<typename std::remove_reference<I_noref>::type>::value, "Cannot cast native array from java array without non-const size argument.");
    return QtJambiPrivate::qtjambi_array_cast_decider<
        false,
        O, QtJambiPrivate::is_jni_array_type<O>::value,
        T, QtJambiPrivate::is_jni_array_type<T>::value,
        I_noref
        >::cast(env, in, size, nullptr, nullptr);
}

#endif // QTJAMBI_CAST_ARRAY_H
