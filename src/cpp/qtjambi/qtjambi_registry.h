/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

#ifndef QTJAMBI_REGISTRY_H
#define QTJAMBI_REGISTRY_H

#include "qtjambi_global.h"
#include "qtjambi_templates.h"
#include "qtjambi_typetests.h"

#include <QtCore/QtCore>

class QtJambiScope;

class QtJambiTypeInfo
{
public:
    template<typename T>
    static QtJambiTypeInfo of(){
        return QtJambiTypeInfo(  QTypeInfo<T>::isSpecialized,
                                 QTypeInfo<T>::isPointer,
                                 QTypeInfo<T>::isIntegral,
                                 QTypeInfo<T>::isComplex,
                                 QTypeInfo<T>::isStatic,
                                 QTypeInfo<T>::isRelocatable,
                                 QTypeInfo<T>::isLarge,
                                 QTypeInfo<T>::isDummy,
                                 QTypeInfo<T>::sizeOf);
    }

    QtJambiTypeInfo(const QtJambiTypeInfo& info)
        :        isSpecialized(info.isSpecialized),
                 isPointer(info.isPointer),
                 isIntegral(info.isIntegral),
                 isComplex(info.isComplex),
                 isStatic(info.isStatic),
                 isRelocatable(info.isRelocatable),
                 isLarge(info.isLarge),
                 isDummy(info.isDummy),
                 sizeOf(info.sizeOf){}

    const bool isSpecialized;
    const bool isPointer;
    const bool isIntegral;
    const bool isComplex;
    const bool isStatic;
    const bool isRelocatable;
    const bool isLarge;
    const bool isDummy;
    const size_t sizeOf;
private:
    QtJambiTypeInfo(bool _isSpecialized,
                    bool _isPointer,
                    bool _isIntegral,
                    bool _isComplex,
                    bool _isStatic,
                    bool _isRelocatable,
                    bool _isLarge,
                    bool _isDummy,
                    size_t _sizeOf)
        :        isSpecialized(_isSpecialized),
                 isPointer(_isPointer),
                 isIntegral(_isIntegral),
                 isComplex(_isComplex),
                 isStatic(_isStatic),
                 isRelocatable(_isRelocatable),
                 isLarge(_isLarge),
                 isDummy(_isDummy),
                 sizeOf(_sizeOf){}
};

typedef int(*SignalMethodIndexProvider)();

struct SignalMetaInfo
{
    inline SignalMetaInfo():
        signal_declaring_class_name(nullptr),
        signal_name(nullptr),
        signal_signature(nullptr),
        signal_method_signature(nullptr),
        signal_argumentcount(0),
        signal_method_index_provider(nullptr){}
    inline SignalMetaInfo(const SignalMetaInfo& copy):
        signal_declaring_class_name(copy.signal_declaring_class_name),
        signal_name(copy.signal_name),
        signal_signature(copy.signal_signature),
        signal_method_signature(copy.signal_method_signature),
        signal_argumentcount(copy.signal_argumentcount),
        signal_method_index_provider(copy.signal_method_index_provider)
    {}
    inline SignalMetaInfo(const char * _signal_declaring_class_name,
    const char * _signal_name,
    const char * _signal_signature,
    const char * _signal_method_signature,
    const int _signal_argumentcount,
    SignalMethodIndexProvider _signal_method_index_provider):
        signal_declaring_class_name(_signal_declaring_class_name),
        signal_name(_signal_name),
        signal_signature(_signal_signature),
        signal_method_signature(_signal_method_signature),
        signal_argumentcount(_signal_argumentcount),
        signal_method_index_provider(_signal_method_index_provider)
    {}
    const char * signal_declaring_class_name;
    const char * signal_name;
    const char * signal_signature;
    const char * signal_method_signature;
    const int signal_argumentcount;
    SignalMethodIndexProvider signal_method_index_provider;
};

enum class jValueType {
    z = 1,
    b,
    c,
    s,
    i,
    j,
    f,
    d,
    l,
};

struct FunctionInfo{
    enum Flags : quint8{
        None = 0,
        Abstract = 0x01,
        Private = 0x02,
    };
    inline FunctionInfo(): name(nullptr),signature(nullptr),flags(None){}
    inline FunctionInfo(const FunctionInfo& copy): name(copy.name),signature(copy.signature),flags(copy.flags){}
    inline FunctionInfo(const char *_name, const char *_signature, Flags _flags = None): name(_name),signature(_signature),flags(_flags){}
    const char *const name;
    const char *const signature;
    const Flags flags;
};

#ifdef QT_QTJAMBI_PORT
class WeakNativeIDPrivate;

class QTJAMBI_EXPORT WeakNativeID{
    WeakNativeID(const WeakNativeID&);
private:
    WeakNativeID(WeakNativeIDPrivate&);
    QSharedDataPointer<WeakNativeIDPrivate> d;
    friend WeakNativeIDPrivate;
};

QTJAMBI_EXPORT void qtjambi_emit_signal(JNIEnv * env, const WeakNativeID& weakNativeID, SignalMethodIndexProvider signalMethodIndexProvider, jvalue *arguments);

typedef void (*SignalConnector)(WeakNativeID, QObject*, QHash<int,QMetaObject::Connection>*);
#endif // QT_QTJAMBI_PORT

typedef void (*ConstructorFunction)(void*, JNIEnv*, jobject, jvalue*);

struct ConstructorInfo{
    inline ConstructorInfo(): constructorFunction(nullptr),signature(nullptr){}
    inline ConstructorInfo(const ConstructorInfo& copy): constructorFunction(copy.constructorFunction),signature(copy.signature){}
    inline ConstructorInfo(ConstructorFunction _constructorFunction, const char *_signature): constructorFunction(_constructorFunction),signature(_signature){}
    ConstructorFunction constructorFunction;
    const char *signature;
};

typedef std::function<bool(JNIEnv*, QtJambiScope*, const void*, jvalue*, bool)> InternalToExternalConverter;

enum class ConvertResponse{
    Successful,
    NotSuccessful,
    JavaObjectNotLinked,
};
typedef std::function<ConvertResponse(JNIEnv*, QtJambiScope*, const jvalue&, void* &, jValueType)> ExternalToInternalConverter;

class QTJAMBI_EXPORT ParameterTypeInfo{
public:
    ParameterTypeInfo();
    ParameterTypeInfo(
            JNIEnv* env,
            int metaTypeId,
            const QByteArray& _qTypeName,
            const char* _javaClass,
            const InternalToExternalConverter& _internalToExternalConverter,
            const ExternalToInternalConverter& _externalToInternalConverter
            );
    ParameterTypeInfo(
            JNIEnv* env,
            int metaTypeId,
            const QByteArray& _qTypeName,
            jclass _javaClass,
            const InternalToExternalConverter& _internalToExternalConverter,
            const ExternalToInternalConverter& _externalToInternalConverter
            );

    bool convertInternalToExternal(JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* out, bool forceBoxedType) const;
    ConvertResponse convertExternalToInternal(JNIEnv* env, QtJambiScope* scope, const jvalue& in,void* & out, jValueType valueType) const;
    jclass javaClass() const;
    int metaType() const;
    static const InternalToExternalConverter& default_internalToExternalConverter();
    static const ExternalToInternalConverter& default_externalToInternalConverter();
    static ParameterTypeInfo voidTypeInfo(JNIEnv* env);
private:
    int m_qTypeId;
    QByteArray m_qTypeName;
    jclass m_javaClass;
    InternalToExternalConverter m_internalToExternalConverter;
    ExternalToInternalConverter m_externalToInternalConverter;
};

typedef bool (*ParameterTypeInfoProvider)(JNIEnv * env, const QMetaMethod& method, QVector<ParameterTypeInfo>& infos);

typedef void(*Destructor)(void*);

typedef const char* (*QMetaMethodRenamer)(int);

typedef std::function<uint(const void*)> QHashFunction;

typedef std::function<bool(const void*, const void*)> QLessFunction;

typedef std::function<bool(const void*, const void*)> QEqualFunction;

QTJAMBI_EXPORT void registerHashFunction(const std::type_info& typeId, QHashFunction hashFunction);

QTJAMBI_EXPORT void registerLessFunction(const std::type_info& typeId, QLessFunction lessFunction);

QTJAMBI_EXPORT void registerEqualFunction(const std::type_info& typeId, QEqualFunction equalFunction);

template<typename T>
void registerLessFunction(){
    registerLessFunction(typeid(T), [](const void* ptr1, const void* ptr2)->bool{ return !ptr1 || !ptr2 ? false : *reinterpret_cast<const T*>(ptr1) < *reinterpret_cast<const T*>(ptr2); });
}

template<typename T>
void registerEqualFunction(){
    registerEqualFunction(typeid(T), [](const void* ptr1, const void* ptr2)->bool{ return !ptr1 || !ptr2 ? false : *reinterpret_cast<const T*>(ptr1) == *reinterpret_cast<const T*>(ptr2); });
}

template<typename T>
void registerHashFunction(){
    registerHashFunction(typeid(T), [](const void* ptr)->uint{ return !ptr ? 0 : qHash(*reinterpret_cast<const T*>(ptr)); });
};

namespace QtJambiPrivate {

template<typename T, bool hasHashFunction>
struct RegistryHashFunctionCheckerHelper{
    static void registerFunction(){}
};

template<typename T>
struct RegistryHashFunctionCheckerHelper<T,true>{
    static void registerFunction(){ registerHashFunction<T>(); }
};

template<typename T, bool hasLessFunction>
struct RegistryLessFunctionCheckerHelper{
    static void registerFunction(){}
};

template<typename T>
struct RegistryLessFunctionCheckerHelper<T,true>{
    static void registerFunction(){ registerLessFunction<T>(); }
};

template<typename T, bool hasEqualFunction>
struct RegistryEqualFunctionCheckerHelper{
    static void registerFunction(){}
};

template<typename T>
struct RegistryEqualFunctionCheckerHelper<T,true>{
    static void registerFunction(){ registerEqualFunction<T>(); }
};

template<template<typename T> class Container, typename T, bool hasEquals, bool hasHashFunction, bool hasLess>
struct RegistryContainerEntryCheckerHelper{
    static void registerOperators(){}
};

template<template<typename T> class Container, typename T>
struct RegistryContainerEntryCheckerHelper<Container, T, true, true, true>{
    static void registerOperators(){
        RegistryLessFunctionCheckerHelper<Container<T>, QtJambiPrivate::supports_less_than<Container<T>>::value>::registerFunction();
        RegistryEqualFunctionCheckerHelper<Container<T>, QtJambiPrivate::supports_equal<Container<T>>::value>::registerFunction();
        RegistryHashFunctionCheckerHelper<Container<T>, QtJambiPrivate::supports_qHash<Container<T>>::value>::registerFunction();
    }
};

template<template<typename T> class Container, typename T>
struct RegistryContainerEntryCheckerHelper<Container, T, true, false, true>{
    static void registerOperators(){
        RegistryEqualFunctionCheckerHelper<Container<T>, QtJambiPrivate::supports_equal<Container<T>>::value>::registerFunction();
        RegistryLessFunctionCheckerHelper<Container<T>, QtJambiPrivate::supports_less_than<Container<T>>::value>::registerFunction();
    }
};

template<template<typename T> class Container, typename T>
struct RegistryContainerEntryCheckerHelper<Container, T, false, false, true>{
    static void registerOperators(){
        RegistryLessFunctionCheckerHelper<Container<T>, QtJambiPrivate::supports_less_than<Container<T>>::value>::registerFunction();
    }
};

template<template<typename T> class Container, typename T>
struct RegistryContainerEntryCheckerHelper<Container, T, true, true, false>{
    static void registerOperators(){
        RegistryEqualFunctionCheckerHelper<Container<T>, QtJambiPrivate::supports_equal<Container<T>>::value>::registerFunction();
        RegistryHashFunctionCheckerHelper<Container<T>, QtJambiPrivate::supports_qHash<Container<T>>::value>::registerFunction();
    }
};

template<template<typename T> class Container, typename T>
struct RegistryContainerEntryCheckerHelper<Container, T, false, true, false>{
    static void registerOperators(){
        RegistryHashFunctionCheckerHelper<Container<T>, QtJambiPrivate::supports_qHash<Container<T>>::value>::registerFunction();
    }
};

template<template<typename T1, typename T2> class Container, typename T1, typename T2, bool hasEquals, bool hasHashFunction, bool hasLess>
struct RegistryContainer2EntryCheckerHelper{
    static void registerOperators(){}
};

template<template<typename T1, typename T2> class Container, typename T1, typename T2>
struct RegistryContainer2EntryCheckerHelper<Container, T1, T2, true, true, true>{
    static void registerOperators(){
        RegistryLessFunctionCheckerHelper<Container<T1, T2>, QtJambiPrivate::supports_less_than<Container<T1, T2>>::value>::registerFunction();
        RegistryEqualFunctionCheckerHelper<Container<T1, T2>, QtJambiPrivate::supports_equal<Container<T1, T2>>::value>::registerFunction();
        RegistryHashFunctionCheckerHelper<Container<T1, T2>, QtJambiPrivate::supports_qHash<Container<T1, T2>>::value>::registerFunction();
    }
};

template<template<typename T1, typename T2> class Container, typename T1, typename T2>
struct RegistryContainer2EntryCheckerHelper<Container, T1, T2, true, false, true>{
    static void registerOperators(){
        RegistryEqualFunctionCheckerHelper<Container<T1, T2>, QtJambiPrivate::supports_equal<Container<T1, T2>>::value>::registerFunction();
        RegistryLessFunctionCheckerHelper<Container<T1, T2>, QtJambiPrivate::supports_less_than<Container<T1, T2>>::value>::registerFunction();
    }
};

template<template<typename T1, typename T2> class Container, typename T1, typename T2>
struct RegistryContainer2EntryCheckerHelper<Container, T1, T2, false, false, true>{
    static void registerOperators(){
        RegistryLessFunctionCheckerHelper<Container<T1, T2>, QtJambiPrivate::supports_less_than<Container<T1, T2>>::value>::registerFunction();
    }
};

template<template<typename T1, typename T2> class Container, typename T1, typename T2>
struct RegistryContainer2EntryCheckerHelper<Container, T1, T2, true, true, false>{
    static void registerOperators(){
        RegistryEqualFunctionCheckerHelper<Container<T1, T2>, QtJambiPrivate::supports_equal<Container<T1, T2>>::value>::registerFunction();
        RegistryHashFunctionCheckerHelper<Container<T1, T2>, QtJambiPrivate::supports_qHash<Container<T1, T2>>::value>::registerFunction();
    }
};

template<template<typename T1, typename T2> class Container, typename T1, typename T2>
struct RegistryContainer2EntryCheckerHelper<Container, T1, T2, false, true, false>{
    static void registerOperators(){
        RegistryHashFunctionCheckerHelper<Container<T1, T2>, QtJambiPrivate::supports_qHash<Container<T1, T2>>::value>::registerFunction();
    }
};

template<template<typename... Ts> class Container, int size, typename... Ts>
struct RegistryContainerHelper{
    static void registerOperators(){
    }
};

template<template<typename... Ts> class Container, typename... Ts>
struct RegistryContainerHelper<Container, 1, Ts...>{
    typedef void(*RegisterOperators)();
    template<template<typename T> class _Container, typename T>
    static constexpr RegisterOperators target(){
        return RegistryContainerEntryCheckerHelper<_Container, T,
                QtJambiPrivate::supports_equal<T>::value,
                QtJambiPrivate::supports_qHash<T>::value,
                QtJambiPrivate::supports_less_than<T>::value>::registerOperators;
    }

    static constexpr void(*registerOperators)() = target<Container,Ts...>();
};

template<template<typename... Ts> class Container, typename... Ts>
struct RegistryContainerHelper<Container, 2, Ts...>{
    typedef void(*RegisterOperators)();
    template<template<typename T1, typename T2> class _Container, typename T1, typename T2>
    static constexpr RegisterOperators target(){
        return RegistryContainer2EntryCheckerHelper<_Container, T1, T2,
                QtJambiPrivate::supports_equal<T1>::value && QtJambiPrivate::supports_equal<T2>::value,
                QtJambiPrivate::supports_qHash<T1>::value && QtJambiPrivate::supports_qHash<T2>::value,
                QtJambiPrivate::supports_less_than<T1>::value && QtJambiPrivate::supports_less_than<T2>::value>::registerOperators;
    }

    static constexpr void(*registerOperators)() = target<Container,Ts...>();
};

template<typename T>
struct RegistryHelper{
    static void registerOperators(){
        RegistryLessFunctionCheckerHelper<T, QtJambiPrivate::supports_less_than<T>::value>::registerFunction();
        RegistryEqualFunctionCheckerHelper<T, QtJambiPrivate::supports_equal<T>::value>::registerFunction();
        RegistryHashFunctionCheckerHelper<T, QtJambiPrivate::supports_qHash<T>::value>::registerFunction();
    }
};

template<template<typename... Ts> class Container, typename... Ts>
struct RegistryHelper<Container<Ts...>>
        : RegistryContainerHelper<Container, sizeof...(Ts), Ts...>
{
};

}

template<typename T>
void registerOperators(){
    QtJambiPrivate::RegistryHelper<T>::registerOperators();
}

QTJAMBI_EXPORT void registerMetaObject(const std::type_info& typeId, const QMetaObject& originalMetaObject, bool isValueOwner, QMetaMethodRenamer methodRenamer = nullptr);
QTJAMBI_EXPORT void registerMetaObject(const std::type_info& typeId, const QMetaObject& originalMetaObject, bool isValueOwner,
#ifdef QT_QTJAMBI_PORT
                                       SignalConnector signalConnector,
#endif
                                       const QVector<SignalMetaInfo>& signalMetaInfos,
                                       ParameterTypeInfoProvider parameterTypeInfoProvider,
                                       QMetaMethodRenamer methodRenamer = nullptr
                                    );
QTJAMBI_EXPORT void registerFunctionInfos(const std::type_info& typeId, const QVector<FunctionInfo>& virtualFunctions);
QTJAMBI_EXPORT void registerConstructorInfos(const std::type_info& typeId, Destructor destructor, const QVector<ConstructorInfo>& constructors);
QTJAMBI_EXPORT void registerMediaControlInfo(const std::type_info& typeId, const char *media_control_iid);
QTJAMBI_EXPORT void registerMetaTypeID(const std::type_info& typeId, int qtMetaType);
QTJAMBI_EXPORT int registerMetaType(const std::type_info& typeId,
                                     const char *typeName,
                                     QMetaType::Destructor destructor,
                                     QMetaType::Constructor constructor,
                                     int size,
                                     QMetaType::TypeFlags flags,
                                     const QMetaObject *metaObject = nullptr);

template<typename T>
int registerMetaTypeNoMetaObject(const char *typeName, QMetaType::Destructor destructor, QMetaType::Constructor constructor)
{
    int id = registerMetaType(typeid(T),
                              typeName,
                              destructor,
                              constructor,
                              sizeof(T),
                              QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<T>::Flags),
                              nullptr);
    if(id>0){
        QtPrivate::SequentialContainerConverterHelper<T>::registerConverter(id);
        QtPrivate::AssociativeContainerConverterHelper<T>::registerConverter(id);
        QtPrivate::MetaTypePairHelper<T>::registerConverter(id);
        QtPrivate::MetaTypeSmartPointerHelper<T>::registerConverter(id);
    }
    registerOperators<T>();
    return id;
}

template<typename T>
int registerMetaTypeNoMetaObject(const char *typeName)
{
    return registerMetaTypeNoMetaObject<T>(typeName,
                               QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Destruct,
                               QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Construct);
}
template<typename T>
int registerMetaType(const char *typeName, QMetaType::Destructor destructor, QMetaType::Constructor constructor)
{
    int id = registerMetaType(typeid(T),
                              typeName,
                              destructor,
                              constructor,
                              sizeof(T),
                              QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<T>::Flags),
                              QtPrivate::MetaObjectForType<T>::value());
    if(id>0){
        QtPrivate::SequentialContainerConverterHelper<T>::registerConverter(id);
        QtPrivate::AssociativeContainerConverterHelper<T>::registerConverter(id);
        QtPrivate::MetaTypePairHelper<T>::registerConverter(id);
        QtPrivate::MetaTypeSmartPointerHelper<T>::registerConverter(id);
    }
    registerOperators<T>();
    return id;
}

template<typename T>
int registerMetaType(const char *typeName)
{
    return registerMetaType<T>(typeName,
                               QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Destruct,
                               QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Construct);
}

QTJAMBI_EXPORT void registerValueTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name);
QTJAMBI_EXPORT void registerObjectTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name);
QTJAMBI_EXPORT void registerQObjectTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name);
QTJAMBI_EXPORT void registerInterfaceTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name, const char *interface_iid);
QTJAMBI_EXPORT void registerInterfaceValueTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name, const char *interface_iid);
QTJAMBI_EXPORT void registerFunctionalTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name);
QTJAMBI_EXPORT void registerEnumTypeInfo(const std::type_info& enumTypeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name);
QTJAMBI_EXPORT void registerEnumTypeInfo(const std::type_info& enumTypeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name, const std::type_info& flagsTypeId, const QtJambiTypeInfo& flagsInfo, const char *flags_qt_name, const char *flags_qt_name_alias, const char *flags_java_name);
QTJAMBI_EXPORT void registerTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name);
QTJAMBI_EXPORT void registerInterfaceID(const std::type_info& typeId, const char *interface_iid);
QTJAMBI_EXPORT const char * registerInterfaceID(JNIEnv* env, jclass cls);

QTJAMBI_EXPORT const char* mediaControlIID(JNIEnv *env, jclass javaType);
QTJAMBI_EXPORT jclass classByMediaControlIID(JNIEnv *env, const char* iid);

QTJAMBI_EXPORT void registerDeleter(const std::type_info& typeId, PtrDeleterFunction deleter);
QTJAMBI_EXPORT void registerOwnerFunction(const std::type_info& typeId, PtrOwnerFunction ownerFunction);

QTJAMBI_EXPORT const QObject* qtjambi_main_thread_owner(void *);

typedef const void* (*DefaultValueCreator)();
QTJAMBI_EXPORT const void* getDefaultValue(const std::type_info& type_info, DefaultValueCreator creator);

typedef bool (*PolymorphyHandler)(void *object);

QTJAMBI_EXPORT void registerPolymorphyHandler(const std::type_info& typeId, PolymorphyHandler handler, const char *class_name, const std::type_info& targetTypeId, bool isQObject);

QTJAMBI_EXPORT void registerSizeOfType(const std::type_info& typeId, size_t size);

QTJAMBI_EXPORT void registerAlignmentOfType(const std::type_info& typeId, size_t size);

QTJAMBI_EXPORT void registerSizeOfShell(const std::type_info& typeId, size_t size);

QTJAMBI_EXPORT void registerInterfaceOffset(const std::type_info& qt_base, const std::type_info& qt_interface, int offset);

typedef jobject(* FunctionalResolver)(JNIEnv*,const void*);
QTJAMBI_EXPORT void registerFunctionalResolver(const std::type_info& typeId, FunctionalResolver resolver);

template<typename T>
const std::type_info& registerValueTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerValueTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name);
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    return id;
}

template<typename T>
const std::type_info& registerObjectTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerObjectTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name);
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    return id;
}

template<typename T>
const std::type_info& registerQObjectTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerQObjectTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name);
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    return id;
}

template<typename T>
const std::type_info& registerInterfaceTypeInfo(const char *qt_name, const char *java_name, const char *interface_iid)
{
    const std::type_info& id = typeid(T);
    registerInterfaceTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name, interface_iid);
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    return id;
}

template<typename T>
const std::type_info& registerInterfaceValueTypeInfo(const char *qt_name, const char *java_name, const char *interface_iid)
{
    const std::type_info& id = typeid(T);
    registerInterfaceValueTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name, interface_iid);
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    return id;
}

template<typename T,typename Tshell>
const std::type_info& registerFunctionalTypeInfo(const char *qt_name, const char *java_name,
                                                 PtrDeleterFunction deleter,
                                                 Destructor destructor, const QVector<ConstructorInfo>& constructors,
                                                 const QVector<FunctionInfo>& virtualFunctions)
{
    const std::type_info& id = typeid(T);
    registerFunctionalTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name);
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    registerMetaTypeID(id, qRegisterMetaType<T>(qt_name));
    registerSizeOfShell(id, sizeof(Tshell));
    registerFunctionalResolver(id, [](JNIEnv* env, const void* ptr) -> jobject {
                return Tshell::resolveFunctional(env, reinterpret_cast<const T*>(ptr));
            });
    registerConstructorInfos(id, destructor, constructors);
    registerDeleter(id, deleter);
    registerFunctionInfos(id, virtualFunctions);
    return id;
}

template<typename T>
const std::type_info& registerEnumTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerEnumTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name);
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    {
        int metaid = qRegisterMetaType<T>(qt_name);
        registerMetaTypeID(id, metaid);
#ifndef QT_NO_DATASTREAM
        QMetaType::registerStreamOperators(metaid, &qGenericSaveEnumerator<T>, &qGenericLoadEnumerator<T>);
#endif // QT_NO_DATASTREAM
    }
    registerOperators<T>();
    return id;
}

template<typename T>
const std::type_info& registerEnumTypeInfo(const char *qt_name, const char *java_name, const char *flags_qt_name, const char *flags_qt_name_alias, const char *flags_java_name)
{
    const std::type_info& id = typeid(T);
    const std::type_info& fid = typeid(QFlags<T>);
    registerEnumTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name, fid, QtJambiTypeInfo::of<QFlags<T>>(), flags_qt_name, flags_qt_name_alias, flags_java_name);
    registerSizeOfType(fid, sizeof(QFlags<T>));
    registerAlignmentOfType(fid, Q_ALIGNOF(QFlags<T>));
    {
        int metaid = qRegisterMetaType<QFlags<T>>(flags_qt_name);
        registerMetaTypeID(fid, metaid);
        QMetaType::registerTypedef(flags_qt_name_alias, metaid);
#ifndef QT_NO_DATASTREAM
        QMetaType::registerStreamOperators(metaid, &qGenericSaveEnumerator<QFlags<T>>, &qGenericLoadEnumerator<QFlags<T>>);
#endif // QT_NO_DATASTREAM
    }
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    {
        int metaid = qRegisterMetaType<T>(qt_name);
        registerMetaTypeID(id, metaid);
#ifndef QT_NO_DATASTREAM
        QMetaType::registerStreamOperators(metaid, &qGenericSaveEnumerator<T>, &qGenericLoadEnumerator<T>);
#endif // QT_NO_DATASTREAM
    }
    registerOperators<T>();
    registerOperators<QFlags<T>>();
    return id;
}

template<typename T>
const std::type_info& registerEnumTypeInfoNoMetaObject(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerEnumTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name);
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    {
        int metaid = QMetaType::registerType(qt_name,
                        nullptr,
                        nullptr,
                        QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Destruct,
                        QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Construct,
                        sizeof(T),
                        QMetaType::TypeFlags(QMetaType::MovableType | QMetaType::IsEnumeration),
                        nullptr
        );
        registerMetaTypeID(id, metaid);
#ifndef QT_NO_DATASTREAM
        QMetaType::registerStreamOperators(metaid, &qGenericSaveEnumerator<T>, &qGenericLoadEnumerator<T>);
#endif // QT_NO_DATASTREAM
    }
    registerOperators<T>();
    return id;
}

template<typename T>
const std::type_info& registerEnumTypeInfoNoMetaObject(const char *qt_name, const char *java_name, const char *flags_qt_name, const char *flags_qt_name_alias, const char *flags_java_name)
{
    const std::type_info& id = typeid(T);
    const std::type_info& fid = typeid(QFlags<T>);
    registerEnumTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name, fid, QtJambiTypeInfo::of<QFlags<T>>(), flags_qt_name, flags_qt_name_alias, flags_java_name);
    registerSizeOfType(fid, sizeof(QFlags<T>));
    registerAlignmentOfType(fid, Q_ALIGNOF(QFlags<T>));
    {
        int metaid = QMetaType::registerType(flags_qt_name,
                        nullptr,
                        nullptr,
                        QtMetaTypePrivate::QMetaTypeFunctionHelper<QFlags<T>>::Destruct,
                        QtMetaTypePrivate::QMetaTypeFunctionHelper<QFlags<T>>::Construct,
                        sizeof(QFlags<T>),
                        QMetaType::TypeFlags(QMetaType::MovableType | QMetaType::IsEnumeration),
                        nullptr
         );

        registerMetaTypeID(fid, metaid);
        QMetaType::registerTypedef(flags_qt_name_alias, metaid);
#ifndef QT_NO_DATASTREAM
        QMetaType::registerStreamOperators(metaid, &qGenericSaveEnumerator<QFlags<T>>, &qGenericLoadEnumerator<QFlags<T>>);
#endif // QT_NO_DATASTREAM
    }
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    {
        int metaid = QMetaType::registerType(qt_name,
                        nullptr,
                        nullptr,
                        QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Destruct,
                        QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Construct,
                        sizeof(T),
                        QMetaType::TypeFlags(QMetaType::MovableType | QMetaType::IsEnumeration),
                        nullptr
        );
        registerMetaTypeID(id, metaid);
#ifndef QT_NO_DATASTREAM
        QMetaType::registerStreamOperators(metaid, &qGenericSaveEnumerator<T>, &qGenericLoadEnumerator<T>);
#endif // QT_NO_DATASTREAM
    }
    registerOperators<T>();
    registerOperators<QFlags<T>>();
    return id;
}

template<typename T>
const std::type_info& registerTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name);
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    return id;
}

#ifdef QT_QTJAMBI_PORT
class QSignalMapper;
typedef void (*QSignalMapperMapped)(QWidget*  widget, const WeakNativeID& link);
typedef QMetaObject::Connection (*QSignalMapperConnector)(QSignalMapper* object, WeakNativeID link, QSignalMapperMapped signalMapperMapped);

QTJAMBI_EXPORT void registerSignalMapperConnector(QSignalMapperConnector signalMapperConnector);
QTJAMBI_EXPORT QMetaObject::Connection connectSignalMapper(QSignalMapper* object, const WeakNativeID& link, QSignalMapperMapped signalMapperMapped);
#endif // QT_QTJAMBI_PORT

QTJAMBI_EXPORT jclass resolveClosestQtSuperclass(JNIEnv *env, jclass clazz, jobject classLoader = nullptr);

QTJAMBI_EXPORT const char* getInterfaceIID(JNIEnv *env, jclass javaType);

QTJAMBI_EXPORT jclass getInterfaceByIID(JNIEnv *env, const char* iid);

typedef void (*QmlReportDestruction)(QObject * obj);

QTJAMBI_EXPORT void setQmlReportDestruction(QmlReportDestruction fct);

#endif // QTJAMBI_REGISTRY_H
