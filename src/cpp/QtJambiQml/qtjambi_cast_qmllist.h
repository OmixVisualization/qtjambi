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

#ifndef QTJAMBI_CAST_QMLLIST_H
#define QTJAMBI_CAST_QMLLIST_H

#include <QtJambi/QmlAPI>
#include <QtJambi/Cast>
#include <QtQml/QQmlListProperty>

namespace QtJambiPrivate {

template<class T, bool = std::is_base_of<QObject,T>::value>
struct QmlListPropertyUtility{
    static jobject toJavaObject(JNIEnv *env, const QQmlListProperty<T>& d)
    {
        return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &d, typeid(QQmlListProperty<void>));
    }
};

template<class T>
struct QmlListPropertyUtility<T,false>{
    static jobject toJavaObject(JNIEnv *env, const QQmlListProperty<T>& d)
    {
        jobject result = QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &d, typeid(QQmlListProperty<void>));
        jobject mt = ::qtjambi_cast<jobject>(env, QMetaType::fromType<T*>());
        QmlAPI::setQQmlListPropertyElementType(env, result, mt);
        return result;
    }
};

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_qmllist_cast{
    typedef QQmlListProperty<T> NativeType;
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
            return QmlListPropertyUtility<T>::toJavaObject(env, _in);
        }else{
            return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(
                QtJambiAPI::convertJavaObjectToNative<NativeType>(env, in), args...);
        }
    }
};

}// namespace QtJambiPrivate

#endif // QTJAMBI_CAST_QMLLIST_H
