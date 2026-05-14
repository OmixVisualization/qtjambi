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

#if !defined(QTJAMBIAPI_NATIVEID_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBIAPI_NATIVEID_H

#include "utils.h"

enum class QtJambiNativeID : jlong { Invalid = 0 };

#define InvalidNativeID QtJambiNativeID::Invalid

QTJAMBI_EXPORT bool operator !(QtJambiNativeID nativeId);
QTJAMBI_EXPORT bool operator &&(QtJambiNativeID nativeId, QtJambiNativeID nativeId2);
QTJAMBI_EXPORT bool operator &&(QtJambiNativeID nativeId, bool b2);
QTJAMBI_EXPORT bool operator &&(bool b1, QtJambiNativeID nativeId);
QTJAMBI_EXPORT bool operator ||(QtJambiNativeID nativeId, QtJambiNativeID nativeId2);
QTJAMBI_EXPORT bool operator ||(QtJambiNativeID nativeId, bool b2);
QTJAMBI_EXPORT bool operator ||(bool b1, QtJambiNativeID nativeId);

template<typename BoolSupplier>
std::enable_if_t<std::is_invocable_r<bool, BoolSupplier>::value, bool>
operator &&(QtJambiNativeID nativeId, BoolSupplier&& b2){
    return nativeId!=InvalidNativeID && b2();
}

template<typename BoolSupplier>
std::enable_if_t<std::is_invocable_r<bool, BoolSupplier>::value, bool>
operator ||(QtJambiNativeID nativeId, BoolSupplier&& b2){
    return nativeId!=InvalidNativeID || b2();
}

namespace QtJambiAPI {

template<class T>
T& checkedAddressOf(JNIEnv *env, T * ptr);

template<typename T>
const T& getDefaultValue();

QTJAMBI_EXPORT QtJambiNativeID javaObjectToNativeId(JNIEnv *env, jobject object);

QTJAMBI_EXPORT QtJambiNativeID javaInterfaceToNativeId(JNIEnv *env, jobject object);

QTJAMBI_EXPORT void *fromNativeId(QtJambiNativeID nativeId);

QTJAMBI_EXPORT void *fromNativeId(QtJambiNativeID nativeId, const std::type_info& typeId);

template<typename T>
T *objectFromNativeId(QtJambiNativeID nativeId)
{
    if constexpr(QtJambiPrivate::is_complete_v<T>){
        return reinterpret_cast<T*>(fromNativeId(nativeId, typeid(T)));
    }else{
        return reinterpret_cast<T*>(fromNativeId(nativeId));
    }
}

template<typename T>
T& objectReferenceFromNativeId(JNIEnv *env, QtJambiNativeID nativeId)
{
    return checkedAddressOf<T>(env, objectFromNativeId<T>(nativeId));
}

template<typename T>
const T& valueReferenceFromNativeId(QtJambiNativeID nativeId){
    if(!!nativeId){
        if(const T* value = objectFromNativeId<T>(nativeId)){
            return *value;
        }
    }
    return getDefaultValue<T>();
}

template<typename T>
T valueFromNativeId(QtJambiNativeID nativeId){
    if(!!nativeId){
        if(const T* value = objectFromNativeId<T>(nativeId)){
            return *value;
        }
    }
    return T{};
}

}

#endif // QTJAMBIAPI_NATIVEID_H
