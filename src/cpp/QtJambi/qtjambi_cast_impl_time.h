/****************************************************************************
**
** Copyright (C) 2016 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QTJAMBI_CAST_IMPL_TIME_H
#define QTJAMBI_CAST_IMPL_TIME_H

#include "qtjambi_cast_impl_util.h"

namespace QtJambiPrivate {

template<bool forward, bool has_scope, bool is_pointer, bool is_const, bool is_reference, typename NativeType>
struct qtjambi_jnitype_crono_duration_cast{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<bool forward, bool has_scope, bool is_pointer, bool is_const, bool is_reference, typename NativeType>
struct qtjambi_jnitype_crono_time_point_cast{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<intmax_t num, intmax_t den>
struct dutation_cast_type{
    using type = std::chrono::seconds;
};

template<intmax_t den>
struct dutation_cast_type<1, den>{
    using type = std::chrono::nanoseconds;
};

template<>
struct dutation_cast_type<1, std::milli::den>{
    using type = std::chrono::milliseconds;
};

template<>
struct dutation_cast_type<1, std::micro::den>{
    using type = std::chrono::nanoseconds;
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference, typename NativeType>
struct qtjambi_jnitype_crono_duration_cast<true, has_scope, is_pointer, is_const, is_reference, NativeType>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        NativeType_c _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        return QtJambiAPI::convertDuration(env, std::chrono::duration_cast<typename dutation_cast_type<NativeType::period::num, NativeType::period::den>::type>(_in));
    }
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference, typename NativeType>
struct qtjambi_jnitype_crono_time_point_cast<true, has_scope, is_pointer, is_const, is_reference, NativeType>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        NativeType_c _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
        return QtJambiAPI::convertTimePointFromEpoch(env, std::chrono::duration_cast<typename dutation_cast_type<NativeType::period::num, NativeType::period::den>::type>(_in.time_since_epoch()));
    }
};

template<bool has_scope, bool is_const, typename NativeType>
struct qtjambi_jnitype_crono_duration_cast<false, has_scope, false, is_const, false, NativeType>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        QPair<std::chrono::seconds, std::chrono::nanoseconds> time  = QtJambiAPI::readDuration(env, in);
        return std::chrono::duration_cast<NativeType>(time.first + time.second);
    }
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference, typename NativeType>
struct qtjambi_jnitype_crono_duration_cast<false, has_scope, is_pointer, is_const, is_reference, NativeType>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast jobject to std::chrono &");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*name, QtJambiScope* scope){
        std::unique_ptr<NativeType> ptr;
        if(in){
            NativeType value = qtjambi_jnitype_crono_duration_cast<false, has_scope, false, false, false, NativeType>::cast(env, in, name, scope);
            ptr.reset(new NativeType(value));
        }
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, ptr);
    }
};

template<bool has_scope, bool is_const, typename NativeType>
struct qtjambi_jnitype_crono_time_point_cast<false, has_scope, false, is_const, false, NativeType>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        QPair<std::chrono::seconds, std::chrono::nanoseconds> time  = QtJambiAPI::readTimePoint(env, in);
        return NativeType(std::chrono::duration_cast<typename NativeType::duration>(time.first + time.second));
    }
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference, typename NativeType>
struct qtjambi_jnitype_crono_time_point_cast<false, has_scope, is_pointer, is_const, is_reference, NativeType>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast jobject to std::chrono &");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*name, QtJambiScope* scope){
        std::unique_ptr<NativeType> ptr;
        if(in){
            NativeType value = qtjambi_jnitype_crono_time_point_cast<false, has_scope, false, false, false, NativeType>::cast(env, in, name, scope);
            ptr.reset(new NativeType(value));
        }
        return create_container_pointer<is_pointer, is_const, is_reference, has_scope, NativeType>::create(env, scope, ptr);
    }
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_IMPL_TIME_H
