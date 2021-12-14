#ifndef QTJAMBI_NATIVEINTERFACE_H
#define QTJAMBI_NATIVEINTERFACE_H

#include "qtjambi_core.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

#include <QtCore/qnativeinterface.h>

struct NITypeInfo{
    const char* name;
    int revision;
};
QTJAMBI_EXPORT NITypeInfo qtjambi_get_native_interface_info(JNIEnv * __jni_env, jclass cls);
QTJAMBI_EXPORT void qtjambi_register_native_interface(const char* className, QPair<const char*, int>&& nameAndRevision);
template<typename T>
void qtjambi_register_native_interface(const char* className){
    qtjambi_register_native_interface(className, {QNativeInterface::Private::TypeInfo<T>::name(), QNativeInterface::Private::TypeInfo<T>::revision()});
}
#endif

#endif // QTJAMBI_NATIVEINTERFACE_H
