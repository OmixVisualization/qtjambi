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

#if !defined(QTJAMBIAPI_SMARTPOINTER_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBIAPI_SMARTPOINTER_H

#include "global.h"

namespace QtJambiAPI {
QTJAMBI_EXPORT QSharedPointer<QObject> convertJavaObjectToQSharedPointer(JNIEnv *env, jobject java_object);

QTJAMBI_EXPORT QSharedPointer<char> convertJavaObjectToQSharedPointer(JNIEnv *env,
                                                                      const std::type_info* typeId,
                                                                      jobject java_object);

QTJAMBI_EXPORT QWeakPointer<QObject> convertJavaObjectToQWeakPointer(JNIEnv *env, jobject java_object);

QTJAMBI_EXPORT QWeakPointer<char> convertJavaObjectToQWeakPointer(JNIEnv *env,
                                                                  const std::type_info* typeId,
                                                                  jobject java_object);

QTJAMBI_EXPORT std::shared_ptr<QObject> convertJavaObjectToSharedPtr(JNIEnv *env, jobject java_object);

QTJAMBI_EXPORT std::shared_ptr<char> convertJavaObjectToSharedPtr(JNIEnv *env,
                                                                  const std::type_info* typeId,
                                                                  jobject java_object);

QTJAMBI_EXPORT std::weak_ptr<QObject> convertJavaObjectToWeakPtr(JNIEnv *env, jobject java_object);

QTJAMBI_EXPORT std::weak_ptr<char> convertJavaObjectToWeakPtr(JNIEnv *env,
                                                              const std::type_info* typeId,
                                                              jobject java_object);

QTJAMBI_EXPORT jobject convertSmartPointerToJavaObject(JNIEnv *env,
                                                       const std::type_info& typeId,
                                                       const QSharedPointer<QObject>& smartPointer);
QTJAMBI_EXPORT jobject convertSmartPointerToJavaObject(JNIEnv *env,
                                                       const std::type_info& typeId,
                                                       const QSharedPointer<char>& smartPointer);
QTJAMBI_EXPORT jobject convertSmartPointerToJavaObject(JNIEnv *env,
                                                       const std::type_info& typeId,
                                                       const std::shared_ptr<QObject>& smartPointer);
QTJAMBI_EXPORT jobject convertSmartPointerToJavaObject(JNIEnv *env,
                                                       const std::type_info& typeId,
                                                       const std::shared_ptr<char>& smartPointer);

}

namespace QtJambiPrivate{


template<template<typename> class SmartPointer, typename T, bool = std::is_base_of<QObject,T>::value>
struct JavaObjectToNativeAsSmartPointerConverter{
};

template<typename T>
struct JavaObjectToNativeAsSmartPointerConverter<QSharedPointer,T,false>{
    static QSharedPointer<T> convert(JNIEnv *env,
                                     jobject java_object){
        QSharedPointer<char> sp = QtJambiAPI::convertJavaObjectToQSharedPointer(env, &typeid(T), java_object);
        char* _ptr = sp.get();
        return QtSharedPointer::copyAndSetPointer(reinterpret_cast<T*>(_ptr), sp);
    }
    static QSharedPointer<char> convertSmartPointer(const QSharedPointer<T>& sp){
        T* _ptr = sp.get();
        return QtSharedPointer::copyAndSetPointer(reinterpret_cast<char*>(_ptr), sp);
    }
};

template<typename T>
struct JavaObjectToNativeAsSmartPointerConverter<std::shared_ptr,T,false>{
    static std::shared_ptr<T> convert(JNIEnv *env,
                                      jobject java_object){
        std::shared_ptr<char> sp = QtJambiAPI::convertJavaObjectToSharedPtr(env, &typeid(T), java_object);
        char* _ptr = sp.get();
        return std::shared_ptr<T>(sp, reinterpret_cast<T*>(_ptr));
    }
    static std::shared_ptr<char> convertSmartPointer(const std::shared_ptr<T>& sp){
        T* _ptr = sp.get();
        return std::shared_ptr<char>(sp, reinterpret_cast<char*>(_ptr));
    }
};

template<typename T>
struct JavaObjectToNativeAsSmartPointerConverter<QSharedPointer,T,true>{
    static QSharedPointer<T> convert(JNIEnv *env,
                                     jobject java_object){
        QSharedPointer<QObject> sp = QtJambiAPI::convertJavaObjectToQSharedPointer(env, java_object);
#if defined(Q_OS_ANDROID) || defined(Q_OS_FREEBSD)
        return sp.objectCast<T>();
#else
        return sp.dynamicCast<T>();
#endif
    }
    static QSharedPointer<QObject> convertSmartPointer(const QSharedPointer<T>& sp){
        return sp.template staticCast<QObject>();
    }
};

template<typename T>
struct JavaObjectToNativeAsSmartPointerConverter<std::shared_ptr,T,true>{
    static std::shared_ptr<T> convert(JNIEnv *env,
                                      jobject java_object){
        std::shared_ptr<QObject> sp = QtJambiAPI::convertJavaObjectToSharedPtr(env, java_object);
#if defined(Q_OS_ANDROID) || defined(Q_OS_FREEBSD)
        return std::static_pointer_cast<T>(sp);
#else
        return std::dynamic_pointer_cast<T>(sp);
#endif
    }
    static std::shared_ptr<QObject> convertSmartPointer(const std::shared_ptr<T>& sp){
        return std::static_pointer_cast<QObject>(sp);
    }
};

template<>
struct JavaObjectToNativeAsSmartPointerConverter<std::shared_ptr,QObject,true>{
    static std::shared_ptr<QObject> convert(JNIEnv *env,
                                            jobject java_object){
        return QtJambiAPI::convertJavaObjectToSharedPtr(env, java_object);
    }
    static const std::shared_ptr<QObject>& convertSmartPointer(const std::shared_ptr<QObject>& sp){
        return sp;
    }
};

template<>
struct JavaObjectToNativeAsSmartPointerConverter<QSharedPointer,QObject,true>{
    static QSharedPointer<QObject> convert(JNIEnv *env,
                                           jobject java_object){
        return QtJambiAPI::convertJavaObjectToQSharedPointer(env, java_object);
    }
    static const QSharedPointer<QObject>& convertSmartPointer(const QSharedPointer<QObject>& sp){
        return sp;
    }
};

}//namespace QtJambiPrivate

namespace QtJambiAPI{

template<template<typename> class SmartPointer, typename T>
SmartPointer<T> convertJavaObjectToSmartPointer(JNIEnv *env, jobject java_object){
    auto out = QtJambiPrivate::JavaObjectToNativeAsSmartPointerConverter<SmartPointer,T>::convert(env, java_object);
    return SmartPointer<T>(*reinterpret_cast<const SmartPointer<T>*>(&out));
}

template<template<typename> class SmartPointer, typename O>
inline jobject convertSmartPointerToJavaObject(JNIEnv *env, const SmartPointer<O> & smartPointer){
    return convertSmartPointerToJavaObject(env,
                                           typeid(O),
                                           QtJambiPrivate::JavaObjectToNativeAsSmartPointerConverter<SmartPointer,O>::convertSmartPointer(smartPointer));
}

}//namespace QtJambiAPI

#endif // QTJAMBIAPI_SMARTPOINTER_H
