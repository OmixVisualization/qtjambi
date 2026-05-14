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

#ifndef QTJAMBI_CAST_DBUS_H
#define QTJAMBI_CAST_DBUS_H

#include <QtCore/QVariant>
#include <QtDBus/QDBusReply>
#include <QtJambi/Cast>

template<class T>
struct QDBusReplyUtility{
    static QDBusReply<T> reverseCreateFrom(const QDBusReply<QVariant>& dBusReply)
    {
        if(dBusReply.isValid()){
            QDBusMessage message;
            message.setArguments({QVariant::fromValue<T>(dBusReply.value().value<T>())});
            return QDBusReply<T>(message);
        }else{
            return QDBusReply<T>(dBusReply.error());
        }
    }

    static QDBusReply<QVariant> createFrom(const QDBusReply<T>& dBusReply)
    {
        if(dBusReply.isValid()){
            QDBusMessage message;
            message.setArguments({QVariant::fromValue<QDBusVariant>(QDBusVariant(QVariant::fromValue<T>(dBusReply.value())))});
            return QDBusReply<QVariant>(message);
        }else{
            return QDBusReply<QVariant>(dBusReply.error());
        }
    }
};

template<>
struct QDBusReplyUtility<QVariant>{
    static QDBusReply<QVariant> reverseCreateFrom(const QDBusReply<QVariant>& dBusReply)
    {
        return dBusReply;
    }
    static QDBusReply<QVariant> createFrom(const QDBusReply<QVariant>& dBusReply)
    {
        return dBusReply;
    }
};

template<>
struct QDBusReplyUtility<void>{
    static QDBusReply<void> reverseCreateFrom(const QDBusReply<QVariant>& dBusReply)
    {
        if(dBusReply.isValid()){
            return QDBusReply<void>(QDBusMessage());
        }else{
            return QDBusReply<void>(dBusReply.error());
        }
    }
    static QDBusReply<QVariant> createFrom(const QDBusReply<void>& dBusReply)
    {
        if(dBusReply.isValid()){
            QDBusVariant dbusVariant(QVariant(QMetaType(QMetaType::Void), nullptr));
            QDBusMessage message;
            message.setArguments({QVariant::fromValue<QDBusVariant>(dbusVariant)});
            return QDBusReply<QVariant>(message);
        }else{
            return QDBusReply<QVariant>(dBusReply.error());
        }
    }
};

namespace QtJambiPrivate {

template<typename T>
struct qtjambi_type_container1<QDBusReply,T>{
    using type = QDBusReply<QVariant>;
};

template<bool forward,
         typename JniType,
         template<typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast;

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast<forward,
                                      jobject,
                                      QDBusReply, is_pointer, is_const, is_reference, is_rvalue,
                                      T, Args...>{
    typedef QDBusReply<T> NativeType;
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
            if constexpr (std::is_same_v<QVariant, T>){
                NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
                return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &_in);
            }else{
                NativeType_c& _in = deref_ptr<is_pointer, NativeType_c>::deref(in);
                QDBusReply<QVariant> reply = QDBusReplyUtility<T>::createFrom(_in);
                return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &reply);
            }
        }else{
            if constexpr (std::is_same_v<QVariant, T>){
                QDBusReply<QVariant>* reply = QtJambiAPI::convertJavaObjectToNative<QDBusReply<QVariant>>(env, in);
                return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(reply, args...);
            }else{
                QDBusReply<QVariant>* reply = QtJambiAPI::convertJavaObjectToNative<QDBusReply<QVariant>>(env, in);
                if(reply){
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(QDBusReplyUtility<T>::reverseCreateFrom(*reply), args...);
                }else{
                    return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(nullptr, args...);
                }
            }
        }
    }
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_DBUS_H
