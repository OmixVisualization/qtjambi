/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_CAST_MODEL_H
#define QTJAMBI_CAST_MODEL_H

#include "qtjambi_cast_util.h"
#include "qtjambiapi_model.h"

namespace QtJambiPrivate {

template<bool forward,
         typename NativeType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_model_cast;

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_model_cast<forward, QModelIndex, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef QModelIndex NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::add_pointer_t<NativeType> NativeType_ptr;
    typedef std::conditional_t<forward, NativeType_in, jobject> In;
    typedef std::conditional_t<forward, jobject, NativeType_out> Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            if constexpr(is_pointer){
                if constexpr(cast_var_args<Args...>::hasScope)
                    return QtJambiAPI::convertModelIndexToEphemeralJavaObject(env, cast_var_args<Args...>::scope(args...), in);
                else
                    return QtJambiAPI::convertModelIndexToJavaObject(env, in);
            }else if constexpr(is_rvalue){
                return QtJambiAPI::convertModelIndexToJavaObject(env, std::move(in));
            }else if constexpr(is_reference && !is_const && cast_var_args<Args...>::hasScope){
                return QtJambiAPI::convertModelIndexToEphemeralJavaObject(env, cast_var_args<Args...>::scope(args...), in);
            }else{
                return QtJambiAPI::convertModelIndexToJavaObject(env, in);
            }
        }else{
            if constexpr(is_pointer || is_reference){
                QModelIndex* result = nullptr;
                if(!QtJambiAPI::convertJavaToModelIndex(env, in, result
#if defined(QTJAMBI_LIGHTWEIGHT_MODELINDEX)
                                                         , cast_var_args<Args...>::scope(args...)
#endif
                                                         )){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(QModelIndex)))) QTJAMBI_STACKTRACEINFO );
                }
                if constexpr(is_pointer){
                    return result;
                }else if constexpr(is_reference && !is_const){
                    if(!result)
                        JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast null to reference type %1").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    return *result;
                }else{
                    return qtjambi_deref_value<NativeType, is_default_constructible_v<NativeType>, is_copy_constructible_v<NativeType>, is_const, is_reference>::deref(env, result);
                }
            }else{
                QModelIndex result;
                if(!QtJambiAPI::convertJavaToModelIndex(env, in, result)){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(QModelIndex)))) QTJAMBI_STACKTRACEINFO );
                }
                return result;
            }
        }
    }
};

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_model_cast<forward, QModelRoleDataSpan, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef QModelRoleDataSpan NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::add_pointer_t<NativeType> NativeType_ptr;
    typedef std::conditional_t<forward, NativeType_in, jobject> In;
    typedef std::conditional_t<forward, jobject, NativeType_out> Out;

    static Out cast(In in, Args... args){
        if constexpr(forward){
            Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
            QMap<int,QVariant> map;
            for(const QModelRoleData& data : deref_ptr<is_pointer,const QModelRoleDataSpan>::deref(in)){
                map[data.role()] = data.data();
            }
            return qtjambi_cast_with_args<jobject>(map, std::forward<Args>(args)...);
        }else{
            auto env = cast_var_args<Args...>::env(args...);
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast to QModelRoleDataSpan without scope.");
            QModelRoleData *data{nullptr};
            qsizetype length(0);
            QtJambiAPI::convertJavaObjectToQModelRoleData(env, cast_var_args<Args...>::scope(args...), in, data, length);
            if constexpr(is_pointer || is_reference){
                Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast jobject to non-const QModelRoleDataSpan&");
                Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast jobject to non-const QModelRoleDataSpan*");
                QModelRoleDataSpan* result = create<QModelRoleDataSpan>(data, length);
                cast_var_args<Args...>::scope(args...).addDeletion(result);
                if constexpr(is_pointer){
                    return result;
                }else if constexpr(is_reference && !is_const){
                    if(!result)
                        JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast null to reference type %1").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    return *result;
                }else{
                    return qtjambi_deref_value<NativeType, is_default_constructible_v<NativeType>, is_copy_constructible_v<NativeType>, is_const, is_reference>::deref(env, result);
                }
            }else{
                return QModelRoleDataSpan(data, length);
            }
        }
    }
};

}

#endif // QTJAMBI_CAST_MODEL_H
