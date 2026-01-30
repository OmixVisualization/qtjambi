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

#if !defined(QTJAMBI_REGISTRYAPI_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_REGISTRYAPI_H

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
#include <QtCore/qnativeinterface.h>
#endif
#include "global.h"
#include "typetests.h"

class QtJambiScope;
class AbstractContainerAccess;
enum class jValueType;
class QtJambiScope;

namespace QtJambiAPI{
enum ConstructorOptions : uchar;
typedef void (*ConstructorFn)(void*, JNIEnv*, jobject, jvalue*, QtJambiAPI::ConstructorOptions);
}

namespace RegistryAPI{

struct SignalMetaInfo
{
    typedef int(*SignalMethodIndexProviderFn)();
    const char * signal_name = nullptr;
    const char * signal_signature = nullptr;
    int signal_argumentcount = 0;
    SignalMethodIndexProviderFn signal_method_index_provider = nullptr;
};

struct FunctionInfo{
    enum Flags : quint8{
        None = 0,
        Abstract = 0x01,
        Private = 0x02,
    };
    const char * name = nullptr;
    const char * signature = nullptr;
    Flags flags = None;
};

struct ConstructorInfo{
    QtJambiAPI::ConstructorFn constructorFunction = nullptr;
    const char *signature = nullptr;
};

struct ParameterInfo{
    typedef bool(*QtToJavaConverterFn)(JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& out, bool forceBoxedType);
    typedef bool(*JavaToQtConverterFn)(JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType);
    int metaTypeId = 0;
    const char* javaClass = nullptr;
    QtToJavaConverterFn qtToJavaConverterFunction = nullptr;
    JavaToQtConverterFn javaToQtConverterFunction = nullptr;
};

typedef bool (*ParameterInfoProviderFn)(const QMetaMethod &method, QList<ParameterInfo> &infos);

typedef void (*DestructorFn)(void *);

typedef const char *(*QMetaMethodRenamerFn)(int);

typedef size_t (*qHashFn)(const void *, size_t);

QTJAMBI_EXPORT void registerHashFunction(const std::type_info& typeId, qHashFn hashFunction);
} // namespace RegistryAPI

namespace QtJambiPrivate {

template <class T, bool supported = QtJambiPrivate::supports_IID<T>::value>
constexpr const char * interfaceIID(){
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
    if constexpr(!supported) return nullptr; else
#endif
    return qobject_interface_iid<T*>();
}

template<typename T, bool = QtJambiPrivate::supports_qHash<T>::value>
struct RegistryHelper{
    static void registerHashFunction(){ RegistryAPI::registerHashFunction(typeid(T), [](const void* ptr, size_t seed)->size_t{ return !ptr ? 0 : qHash(*reinterpret_cast<const T*>(ptr), seed); }); }
};

template<typename T>
struct RegistryHelper<T,false>{
    static void registerHashFunction(){}
};

template<typename T>
struct QMetaTypeInterfaceFunctions
{
    static inline constexpr const QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr= QtPrivate::QMetaTypeForType<T>::getDefaultCtr();
    static inline constexpr const QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr= QtPrivate::QMetaTypeForType<T>::getCopyCtr();
    static inline constexpr const QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr= QtPrivate::QMetaTypeForType<T>::getMoveCtr();
    static inline constexpr const QtPrivate::QMetaTypeInterface::DtorFn dtor = QtPrivate::QMetaTypeForType<T>::getDtor();
    static inline constexpr const QtPrivate::QMetaTypeInterface::LegacyRegisterOp  legacyRegisterOp = QtPrivate::QMetaTypeForType<T>::getLegacyRegister();
};
} // end of namespace QtJambiPrivate

typedef void (*AfterRegistrationFunction)(const QtPrivate::QMetaTypeInterface *);
typedef AbstractContainerAccess*(*NewContainerAccessFunction)();
typedef bool (*PolymorphyHandler)(void *object, qintptr& offset);
typedef jobject(* FunctionalResolver)(JNIEnv*,const void*,bool*);
typedef const std::type_info* (*TypeInfoSupplier)(const void *object);

namespace RegistryAPI{

template<typename T>
void registerOperators(){
    QtJambiPrivate::RegistryHelper<T>::registerHashFunction();
}

QTJAMBI_EXPORT void registerCustomMetaObject(const std::type_info& typeId, const QMetaObject& superTypeMetaObject);
QTJAMBI_EXPORT void registerMetaObject(const std::type_info& typeId, const QMetaObject& originalMetaObject, bool isValueOwner, QMetaMethodRenamerFn methodRenamer = nullptr);
QTJAMBI_EXPORT void registerMetaObject(const std::type_info& typeId, const QMetaObject& originalMetaObject, bool isValueOwner,
                                       std::initializer_list<SignalMetaInfo> signalMetaInfos,
                                       ParameterInfoProviderFn parameterTypeInfoProvider,
                                       QMetaMethodRenamerFn methodRenamer = nullptr);
QTJAMBI_EXPORT void registerFunctionInfos(const std::type_info& typeId, std::initializer_list<FunctionInfo> virtualFunctions);
QTJAMBI_EXPORT void registerConstructorInfos(const std::type_info& typeId, uint returnScopes, DestructorFn destructor, std::initializer_list<ConstructorInfo> constructors);
QTJAMBI_EXPORT void registerMediaControlInfo(const std::type_info& typeId, const char *media_control_iid);
QTJAMBI_EXPORT void registerMetaType(const std::type_info& typeId, const QMetaType& qtMetaType);

QTJAMBI_EXPORT QMetaType registerMetaType( const std::type_info& typeId,
                                         const std::type_info& nonPointerTypeId,
                                         const char *typeName,
                                         QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                         QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                         QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                                         QtPrivate::QMetaTypeInterface::DtorFn dtor,
                                         QtPrivate::QMetaTypeInterface::EqualsFn equals,
                                         QtPrivate::QMetaTypeInterface::LessThanFn lessThan,
                                         QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream,
                                         QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOutFn,
                                         QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamInFn,
                                         QtPrivate::QMetaTypeInterface::LegacyRegisterOp legacyRegisterOp,
                                         uint size,
                                         ushort align,
                                         int builtInTypeId,
                                         QMetaType::TypeFlags flags,
                                         const QMetaObject *metaObject,
                                         AfterRegistrationFunction afterRegistrationFunction);

QTJAMBI_EXPORT QMetaType registerMetaType( const std::type_info& typeId,
                                         const std::type_info& nonPointerTypeId,
                                         const QByteArray& typeName,
                                         QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                         QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                         QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                                         QtPrivate::QMetaTypeInterface::DtorFn dtor,
                                         QtPrivate::QMetaTypeInterface::EqualsFn equals,
                                         QtPrivate::QMetaTypeInterface::LessThanFn lessThan,
                                         QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream,
                                         QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOutFn,
                                         QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamInFn,
                                         QtPrivate::QMetaTypeInterface::LegacyRegisterOp legacyRegisterOp,
                                         uint size,
                                         ushort align,
                                         int builtInTypeId,
                                         QMetaType::TypeFlags flags,
                                         const QMetaObject *metaObject,
                                         AfterRegistrationFunction afterRegistrationFunction,
                                         AbstractContainerAccess* access = nullptr);

template<typename T>
QMetaType registerMetaTypeNoMetaObject(const char *typeName,
                                          QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                          QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                          QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                                          QtPrivate::QMetaTypeInterface::DtorFn dtor,
                                          QtPrivate::QMetaTypeInterface::EqualsFn equals,
                                          QtPrivate::QMetaTypeInterface::LessThanFn lessThan,
                                          QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream,
                                          QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOutFn,
                                          QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamInFn
                                       )
{
    registerOperators<T>();
    return registerMetaType(typeid(T),
                              typeid(typename std::remove_pointer<T>::type),
                              typeName,
                              defaultCtr,
                              copyCtr,
                              moveCtr,
                              dtor,
                              equals,
                              lessThan,
                              debugStream,
                              dataStreamOutFn,
                              dataStreamInFn,
                              QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::legacyRegisterOp,
                              sizeof(T),
                              alignof(T),
                              QtPrivate::BuiltinMetaType<T>::value,
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                              QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<T>::Flags),
#else
                              QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<T>::flags()),
#endif
                              nullptr,
                              [](const QtPrivate::QMetaTypeInterface *){
                                    QtPrivate::SequentialContainerTransformationHelper<T>::registerConverter();
                                    QtPrivate::AssociativeContainerTransformationHelper<T>::registerConverter();
                                    QtPrivate::SequentialContainerTransformationHelper<T>::registerMutableView();
                                    QtPrivate::AssociativeContainerTransformationHelper<T>::registerMutableView();
                                    QtPrivate::MetaTypePairHelper<T>::registerConverter();
                                    QtPrivate::MetaTypeSmartPointerHelper<T>::registerConverter();
                              });
}

template<typename T>
QMetaType registerMetaTypeNoMetaObject(const char *typeName,
                                  QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                  QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                  QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                                  QtPrivate::QMetaTypeInterface::DtorFn dtor
                                )
{
    return registerMetaTypeNoMetaObject<T>(
                              typeName,
                              defaultCtr,
                              copyCtr,
                              moveCtr,
                              dtor,
                              QtPrivate::QEqualityOperatorForType<T>::equals,
                              QtPrivate::QLessThanOperatorForType<T>::lessThan,
                              QtPrivate::QDebugStreamOperatorForType<T>::debugStream,
                              QtPrivate::QDataStreamOperatorForType<T>::dataStreamOut,
                              QtPrivate::QDataStreamOperatorForType<T>::dataStreamIn);
}

template<typename T>
QMetaType registerMetaTypeNoMetaObject(const char *typeName)
{
    return registerMetaTypeNoMetaObject<T>(typeName,
                               QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::defaultCtr,
                               QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::copyCtr,
                               QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::moveCtr,
                               QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::dtor,
                               QtPrivate::QEqualityOperatorForType<T>::equals,
                               QtPrivate::QLessThanOperatorForType<T>::lessThan,
                               QtPrivate::QDebugStreamOperatorForType<T>::debugStream,
                               QtPrivate::QDataStreamOperatorForType<T>::dataStreamOut,
                               QtPrivate::QDataStreamOperatorForType<T>::dataStreamIn
                        );
}

template<typename T>
QMetaType registerMetaTypeNoMetaObject(const char *typeName,
                                  QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                  QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                  QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr
                                )
{
    return registerMetaTypeNoMetaObject<T>(
                              typeName,
                              defaultCtr,
                              copyCtr,
                              moveCtr,
                              QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::dtor
                );
}

template<typename T>
QMetaType registerMetaType(const char *typeName,
                                   QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                   QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                   QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                                   QtPrivate::QMetaTypeInterface::DtorFn dtor,
                                   QtPrivate::QMetaTypeInterface::EqualsFn equals,
                                   QtPrivate::QMetaTypeInterface::LessThanFn lessThan,
                                   QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream,
                                   QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOutFn,
                                   QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamInFn
                    )
{
    registerOperators<T>();
    return registerMetaType(typeid(T),
                              typeid(typename std::remove_pointer<T>::type),
                              typeName,
                              defaultCtr,
                              copyCtr,
                              moveCtr,
                              dtor,
                              equals,
                              lessThan,
                              debugStream,
                              dataStreamOutFn,
                              dataStreamInFn,
                              QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::legacyRegisterOp,
                              sizeof(T),
                              alignof(T),
                              QtPrivate::BuiltinMetaType<T>::value,
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                              QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<T>::Flags),
#else
                              QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<T>::flags()),
#endif
                              QtPrivate::MetaObjectForType<T>::value(),
                              [](const QtPrivate::QMetaTypeInterface *){
                                    QtPrivate::SequentialContainerTransformationHelper<T>::registerConverter();
                                    QtPrivate::AssociativeContainerTransformationHelper<T>::registerConverter();
                                    QtPrivate::SequentialContainerTransformationHelper<T>::registerMutableView();
                                    QtPrivate::AssociativeContainerTransformationHelper<T>::registerMutableView();
                                    QtPrivate::MetaTypePairHelper<T>::registerConverter();
                                    QtPrivate::MetaTypeSmartPointerHelper<T>::registerConverter();
                                }
                            );
}

template<typename T>
QMetaType registerMetaType(const char *typeName,
                                   QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                   QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                   QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                                   QtPrivate::QMetaTypeInterface::DtorFn dtor
                    )
{
    return registerMetaType<T>(typeName,
                              defaultCtr,
                              copyCtr,
                              moveCtr,
                              dtor,
                              QtPrivate::QEqualityOperatorForType<T>::equals,
                              QtPrivate::QLessThanOperatorForType<T>::lessThan,
                              QtPrivate::QDebugStreamOperatorForType<T>::debugStream,
                              QtPrivate::QDataStreamOperatorForType<T>::dataStreamOut,
                              QtPrivate::QDataStreamOperatorForType<T>::dataStreamIn
                        );
}

template<typename T>
QMetaType registerMetaType(const char *typeName,
                                   QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                   QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                   QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr)
{
    return registerMetaType<T>(typeName,
                              defaultCtr,
                              copyCtr,
                              moveCtr,
                              QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::dtor,
                              QtPrivate::QEqualityOperatorForType<T>::equals,
                              QtPrivate::QLessThanOperatorForType<T>::lessThan,
                              QtPrivate::QDebugStreamOperatorForType<T>::debugStream,
                              QtPrivate::QDataStreamOperatorForType<T>::dataStreamOut,
                              QtPrivate::QDataStreamOperatorForType<T>::dataStreamIn);
}

namespace Private{

template<typename T, bool = std::is_polymorphic<T>::value>
struct PolymorphicTypeInfoSupplier{
    static const std::type_info* value(const void *ptr) {
        const T* object = reinterpret_cast<const T*>(ptr);
        try{
            const std::type_info* typeId = &typeid(*object);
            if(!typeId)
                typeId = &typeid(T);
            return typeId;
        }catch(const std::bad_typeid&){
            return nullptr;
        }catch(...){
            return nullptr;
        }
    };
};

template<typename T>
struct PolymorphicTypeInfoSupplier<T,false>{
    static constexpr TypeInfoSupplier value = nullptr;
};

template<typename T, bool = QMetaTypeId<T>::Defined>
struct MetaTypeUtil{
    static QMetaType registerMetaType(const char *typeName){
        return RegistryAPI::registerMetaType<T>(typeName,
                                  QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::defaultCtr,
                                  QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::copyCtr,
                                  QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::moveCtr,
                                  QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::dtor,
                                  QtPrivate::QEqualityOperatorForType<T>::equals,
                                  QtPrivate::QLessThanOperatorForType<T>::lessThan,
                                  QtPrivate::QDebugStreamOperatorForType<T>::debugStream,
                                  QtPrivate::QDataStreamOperatorForType<T>::dataStreamOut,
                                  QtPrivate::QDataStreamOperatorForType<T>::dataStreamIn);
    }
};

template<typename T>
struct MetaTypeUtil<T,true>{
    static QMetaType registerMetaType(const char *){
        registerOperators<T>();
        QMetaType mt(QMetaTypeId<T>::qt_metatype_id());
        RegistryAPI::registerMetaType(typeid(T), mt);
        return mt;
    }
};

}

template<typename T>
QMetaType registerMetaType(const char *typeName)
{
    return Private::MetaTypeUtil<T>::registerMetaType(typeName);
}

template<typename T>
QMetaType registerMetaType(const QByteArray& typeName,
                                   QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                   QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                   QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                                   QtPrivate::QMetaTypeInterface::DtorFn dtor,
                                   QtPrivate::QMetaTypeInterface::EqualsFn equals,
                                   QtPrivate::QMetaTypeInterface::LessThanFn lessThan,
                                   QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream,
                                   QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOutFn,
                                   QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamInFn)
{
    registerOperators<T>();
    return registerMetaType(typeid(T),
                              typeid(typename std::remove_pointer<T>::type),
                              typeName,
                              defaultCtr,
                              copyCtr,
                              moveCtr,
                              dtor,
                              equals,
                              lessThan,
                              debugStream,
                              dataStreamOutFn,
                              dataStreamInFn,
                              QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::legacyRegisterOp,
                              sizeof(T),
                              alignof(T),
                              QtPrivate::BuiltinMetaType<T>::value,
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                              QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<T>::Flags),
#else
                              QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<T>::flags()),
#endif
                              QtPrivate::MetaObjectForType<T>::value(),
                              [](const QtPrivate::QMetaTypeInterface *){
                                    QtPrivate::SequentialContainerTransformationHelper<T>::registerConverter();
                                    QtPrivate::AssociativeContainerTransformationHelper<T>::registerConverter();
                                    QtPrivate::SequentialContainerTransformationHelper<T>::registerMutableView();
                                    QtPrivate::AssociativeContainerTransformationHelper<T>::registerMutableView();
                                    QtPrivate::MetaTypePairHelper<T>::registerConverter();
                                    QtPrivate::MetaTypeSmartPointerHelper<T>::registerConverter();
                              }
                );
}

template<typename T>
QMetaType registerMetaType(const QByteArray& typeName,
                     QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                     QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                     QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                     QtPrivate::QMetaTypeInterface::DtorFn dtor,
                     QtPrivate::QMetaTypeInterface::EqualsFn equals,
                     QtPrivate::QMetaTypeInterface::LessThanFn lessThan,
                     QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream,
                     QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOutFn,
                     QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamInFn,
                     AbstractContainerAccess* access)
{
    registerOperators<T>();
    return registerMetaType(typeid(T),
                            typeid(typename std::remove_pointer<T>::type),
                            typeName,
                            defaultCtr,
                            copyCtr,
                            moveCtr,
                            dtor,
                            equals,
                            lessThan,
                            debugStream,
                            dataStreamOutFn,
                            dataStreamInFn,
                            QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::legacyRegisterOp,
                            sizeof(T),
                            alignof(T),
                            QtPrivate::BuiltinMetaType<T>::value,
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                            QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<T>::Flags),
#else
                            QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<T>::flags()),
#endif
                            QtPrivate::MetaObjectForType<T>::value(),
                            [](const QtPrivate::QMetaTypeInterface *){
                                QtPrivate::SequentialContainerTransformationHelper<T>::registerConverter();
                                QtPrivate::AssociativeContainerTransformationHelper<T>::registerConverter();
                                QtPrivate::SequentialContainerTransformationHelper<T>::registerMutableView();
                                QtPrivate::AssociativeContainerTransformationHelper<T>::registerMutableView();
                                QtPrivate::MetaTypePairHelper<T>::registerConverter();
                                QtPrivate::MetaTypeSmartPointerHelper<T>::registerConverter();
                            },
                            access
                        );
}

template<typename T>
QMetaType registerMetaType(const QByteArray& typeName)
{
    return registerMetaType<T>(typeName,
                              QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::defaultCtr,
                              QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::copyCtr,
                              QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::moveCtr,
                              QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::dtor,
                              QtPrivate::QEqualityOperatorForType<T>::equals,
                              QtPrivate::QLessThanOperatorForType<T>::lessThan,
                              QtPrivate::QDebugStreamOperatorForType<T>::debugStream,
                              QtPrivate::QDataStreamOperatorForType<T>::dataStreamOut,
                              QtPrivate::QDataStreamOperatorForType<T>::dataStreamIn
                            );
}

template<typename T>
QMetaType registerMetaType(const QByteArray& typeName, AbstractContainerAccess* access)
{
    return registerMetaType<T>(typeName,
                               QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::defaultCtr,
                               QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::copyCtr,
                               QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::moveCtr,
                               QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::dtor,
                               QtPrivate::QEqualityOperatorForType<T>::equals,
                               QtPrivate::QLessThanOperatorForType<T>::lessThan,
                               QtPrivate::QDebugStreamOperatorForType<T>::debugStream,
                               QtPrivate::QDataStreamOperatorForType<T>::dataStreamOut,
                               QtPrivate::QDataStreamOperatorForType<T>::dataStreamIn,
                               access);
}

QTJAMBI_EXPORT void registerContainerAccessFactory(const std::type_info& typeId, NewContainerAccessFunction factory);
QTJAMBI_EXPORT void registerValueTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name);
QTJAMBI_EXPORT void registerObjectTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name);
QTJAMBI_EXPORT void registerQObjectTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name);
QTJAMBI_EXPORT void registerInterfaceTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name, const char *interface_iid);
QTJAMBI_EXPORT void registerInterfaceValueTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name, const char *interface_iid);
QTJAMBI_EXPORT void registerFunctionalTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name, bool isFunctionPointer, const QMetaType& metaType,
                                               size_t size, size_t alignment, size_t sizeOfShell, size_t alignmentOfShell, FunctionalResolver resolver, uint returnScopes,
                                               RegistryAPI::DestructorFn destructor, std::initializer_list<ConstructorInfo> constructors, PtrDeleterFunction deleter, std::initializer_list<FunctionInfo> virtualFunctions);
QTJAMBI_EXPORT void registerEnumTypeInfo(const std::type_info& enumTypeId, const char *qt_name, const char *java_name);
QTJAMBI_EXPORT void registerEnumTypeInfo(const std::type_info& enumTypeId, const char *qt_name, const char *java_name, const std::type_info& flagsTypeId, const char *flags_qt_name, const char *flags_qt_name_alias, const char *flags_java_name);
QTJAMBI_EXPORT void registerUnspecificTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name);
QTJAMBI_EXPORT void registerPrimitiveTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name);
QTJAMBI_EXPORT void registerInterfaceID(const std::type_info& typeId, const char *interface_iid);

QTJAMBI_EXPORT void registerNamespaceTypeInfo(const char *qt_name, const char *java_name, const QMetaObject* namespaceMetaObject);
QTJAMBI_EXPORT const char * registerInterfaceID(JNIEnv* env, jclass cls);

QTJAMBI_EXPORT const char* mediaControlIID(JNIEnv *env, jclass javaType);
QTJAMBI_EXPORT jclass classByMediaControlIID(JNIEnv *env, const char* iid);

QTJAMBI_EXPORT void registerDeleter(const std::type_info& typeId, PtrDeleterFunction deleter);
QTJAMBI_EXPORT void registerOwnerFunction(const std::type_info& typeId, PtrOwnerFunction ownerFunction);

QTJAMBI_EXPORT void registerPolymorphyHandler(const std::type_info& typeId, const std::type_info& targetTypeId, PolymorphyHandler handler);

QTJAMBI_EXPORT void registerSizeOfType(const std::type_info& typeId, size_t size, size_t alignment);

QTJAMBI_EXPORT void registerSizeOfShell(const std::type_info& typeId, size_t size, size_t alignment);

QTJAMBI_EXPORT void registerInterfaceOffset(const std::type_info& qt_base, const std::type_info& qt_interface, int offset);

QTJAMBI_EXPORT void registerFunctionalResolver(const std::type_info& typeId, FunctionalResolver resolver);

QTJAMBI_EXPORT void registerTypeInfoSupplier(const std::type_info& typeId, TypeInfoSupplier resolver);

template<typename Super, typename Class>
void registerDefaultPolymorphyHandler(){
    registerPolymorphyHandler(typeid(Super), typeid(Class), [](void *ptr, qintptr& offset) -> bool {
        Super* object = reinterpret_cast<Super*>(ptr);
        Q_ASSERT(object);
        offset = 0;
        bool _result = false;
        try{
            _result = dynamic_cast<Class*>(object);
        }catch(...){
        }
        return _result;
    });
}

template<typename T>
const std::type_info& registerValueTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerValueTypeInfo(id, qt_name, java_name);
    registerSizeOfType(id, sizeof(T), alignof(T));
    registerTypeInfoSupplier(id, Private::PolymorphicTypeInfoSupplier<T>::value);
    return id;
}

template<typename T>
const std::type_info& registerObjectTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerObjectTypeInfo(id, qt_name, java_name);
    registerSizeOfType(id, sizeof(T), alignof(T));
    registerTypeInfoSupplier(id, Private::PolymorphicTypeInfoSupplier<T>::value);
    return id;
}

template<typename T>
const std::type_info& registerQObjectTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerQObjectTypeInfo(id, qt_name, java_name);
    registerSizeOfType(id, sizeof(T), alignof(T));
    registerTypeInfoSupplier(id, Private::PolymorphicTypeInfoSupplier<T>::value);
    return id;
}

template<typename T>
const std::type_info& registerInterfaceTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerInterfaceTypeInfo(id, qt_name, java_name, QtJambiPrivate::interfaceIID<T>());
    registerSizeOfType(id, sizeof(T), alignof(T));
    registerTypeInfoSupplier(id, Private::PolymorphicTypeInfoSupplier<T>::value);
    return id;
}

template<typename T>
const std::type_info& registerInterfaceValueTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerInterfaceValueTypeInfo(id, qt_name, java_name, QtJambiPrivate::interfaceIID<T>());
    registerSizeOfType(id, sizeof(T), alignof(T));
    registerTypeInfoSupplier(id, Private::PolymorphicTypeInfoSupplier<T>::value);
    return id;
}

template<typename T>
const std::type_info& registerFunctionalTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    QMetaType metaType = QMetaType::fromType<T>();
    if(metaType.name()!=QByteArrayView(qt_name)){
        const QtPrivate::QMetaTypeInterface *iface = metaType.iface();
        QtPrivate::QMetaTypeInterface* metaTypeInterface = new QtPrivate::QMetaTypeInterface{
            iface->revision,
            iface->alignment,
            iface->size,
            iface->flags,
            0,
            iface->metaObjectFn,
            qt_name,
            iface->defaultCtr,
            iface->copyCtr,
            iface->moveCtr,
            iface->dtor,
            iface->equals,
            iface->lessThan,
            iface->debugStream,
            iface->dataStreamOut,
            iface->dataStreamIn,
            iface->legacyRegisterOp
        };
        metaType = QMetaType(metaTypeInterface);
    }
    registerFunctionalTypeInfo(id, qt_name, java_name, std::is_function_v<std::remove_pointer_t<T>>, metaType,
                               sizeof(T), alignof(T), 0, 0, nullptr,
                               0, nullptr, {}, nullptr, {});
    return id;
}

template<typename T, typename Tshell>
const std::type_info& registerFunctionalTypeInfo(const char *qt_name, const char *java_name, bool needsReturnScope,
                                                 PtrDeleterFunction deleter,
                                                 DestructorFn destructor, std::initializer_list<ConstructorInfo> constructors,
                                                 std::initializer_list<FunctionInfo> virtualFunctions)
{
    const std::type_info& id = typeid(T);
    QMetaType metaType = QMetaType::fromType<T>();
    if(metaType.name()!=QByteArrayView(qt_name)){
        const QtPrivate::QMetaTypeInterface *iface = metaType.iface();
        QtPrivate::QMetaTypeInterface* metaTypeInterface = new QtPrivate::QMetaTypeInterface{
            iface->revision,
            iface->alignment,
            iface->size,
            iface->flags,
            0,
            iface->metaObjectFn,
            qt_name,
            iface->defaultCtr,
            iface->copyCtr,
            iface->moveCtr,
            iface->dtor,
            iface->equals,
            iface->lessThan,
            iface->debugStream,
            iface->dataStreamOut,
            iface->dataStreamIn,
            iface->legacyRegisterOp
        };
        metaType = QMetaType(metaTypeInterface);
    }
    registerFunctionalTypeInfo(id, qt_name, java_name, std::is_function_v<std::remove_pointer_t<T>>, metaType,
                               sizeof(T), alignof(T), sizeof(Tshell), alignof(Tshell), &Tshell::resolveFunctional,
                               needsReturnScope ? 1 : 0, destructor, constructors, deleter, virtualFunctions);
    return id;
}

template<typename T>
const std::type_info& registerEnumTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerEnumTypeInfo(id, qt_name, java_name);
    registerSizeOfType(id, sizeof(T), alignof(T));
    {
        QMetaType metaType = QMetaType::fromType<T>();
        if(QByteArrayView(metaType.name())!=qt_name){
            QMetaType::registerNormalizedTypedef(qt_name, metaType);
        }
        registerMetaType(id, metaType);
    }
    registerOperators<T>();
    return id;
}

template<typename T>
const std::type_info& registerEnumTypeInfo(const char *qt_name, const char *java_name, const char *flags_qt_name, const char *flags_qt_name_alias, const char *flags_java_name)
{
    const std::type_info& id = typeid(T);
    const std::type_info& fid = typeid(QFlags<T>);
    registerEnumTypeInfo(id, qt_name, java_name, fid, flags_qt_name, flags_qt_name_alias, flags_java_name);
    registerSizeOfType(fid, sizeof(QFlags<T>), alignof(QFlags<T>));
    {
        QMetaType metaType = QMetaType::fromType<QFlags<T>>();
        if(QByteArrayView(metaType.name())!=flags_qt_name){
            QMetaType::registerNormalizedTypedef(flags_qt_name, metaType);
        }
        registerMetaType(fid, metaType);
    }
    registerSizeOfType(id, sizeof(T), alignof(T));
    {
        QMetaType metaType = QMetaType::fromType<T>();
        if(QByteArrayView(metaType.name())!=qt_name){
            QMetaType::registerNormalizedTypedef(qt_name, metaType);
        }
        registerMetaType(id, metaType);
    }
    registerOperators<T>();
    registerOperators<QFlags<T>>();
    return id;
}

template<typename T>
const std::type_info& registerEnumTypeInfoNoMetaObject(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerEnumTypeInfo(id, qt_name, java_name);
    registerSizeOfType(id, sizeof(T), alignof(T));
    {
        QMetaType metaType = registerMetaType(id,
                                 id,
                                 qt_name,
                                 /*.defaultCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::defaultCtr,
                                 /*.copyCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::copyCtr,
                                 /*.moveCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::moveCtr,
                                 /*.dtor=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::dtor,
                                 /*.equals=*/ QtPrivate::QEqualityOperatorForType<T>::equals,
                                 /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<T>::lessThan,
                                 /*.debugStream=*/ nullptr,
                                 /*.dataStreamOutFn=*/ QtPrivate::QDataStreamOperatorForType<T>::dataStreamOut,
                                 /*.dataStreamInFn=*/ QtPrivate::QDataStreamOperatorForType<T>::dataStreamIn,
                                 /*.legacyRegisterOp=*/ nullptr,
                                 /*.size=*/ sizeof(T),
                                 /*.alignment=*/ alignof(T),
                                 /*.typeId=*/ QtPrivate::BuiltinMetaType<T>::value,
                                 /*.flags=*/ QMetaType::RelocatableType | QMetaType::IsEnumeration,
                                 nullptr,
                                 [](const QtPrivate::QMetaTypeInterface *){
                                        QtPrivate::SequentialContainerTransformationHelper<T>::registerConverter();
                                        QtPrivate::AssociativeContainerTransformationHelper<T>::registerConverter();
                                        QtPrivate::SequentialContainerTransformationHelper<T>::registerMutableView();
                                        QtPrivate::AssociativeContainerTransformationHelper<T>::registerMutableView();
                                        QtPrivate::MetaTypePairHelper<T>::registerConverter();
                                        QtPrivate::MetaTypeSmartPointerHelper<T>::registerConverter();
                                    }
                            );
        registerMetaType(id, metaType);
    }
    registerOperators<T>();
    return id;
}

template<typename T>
const std::type_info& registerEnumTypeInfoNoMetaObject(const char *qt_name, const char *java_name, const char *flags_qt_name, const char *flags_qt_name_alias, const char *flags_java_name)
{
    const std::type_info& id = typeid(T);
    const std::type_info& fid = typeid(QFlags<T>);
    registerEnumTypeInfo(id, qt_name, java_name, fid, flags_qt_name, flags_qt_name_alias, flags_java_name);
    registerSizeOfType(fid, sizeof(QFlags<T>), alignof(QFlags<T>));
    {
        QMetaType metaType = registerMetaType(id,
                                      id,
                                      flags_qt_name,
                                      /*.defaultCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QFlags<T>>::defaultCtr,
                                      /*.copyCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QFlags<T>>::copyCtr,
                                      /*.moveCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QFlags<T>>::moveCtr,
                                      /*.dtor=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QFlags<T>>::dtor,
                                      /*.equals=*/ QtPrivate::QEqualityOperatorForType<QFlags<T>>::equals,
                                      /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<QFlags<T>>::lessThan,
                                      /*.debugStream=*/ nullptr,
                                      /*.dataStreamOutFn=*/ QtPrivate::QDataStreamOperatorForType<QFlags<T>>::dataStreamOut,
                                      /*.dataStreamInFn=*/ QtPrivate::QDataStreamOperatorForType<QFlags<T>>::dataStreamIn,
                                      /*.legacyRegisterOp=*/ nullptr,
                                      /*.size=*/ sizeof(QFlags<T>),
                                      /*.alignment=*/ alignof(QFlags<T>),
                                      /*.typeId=*/ QtPrivate::BuiltinMetaType<QFlags<T>>::value,
                                      /*.flags=*/ QMetaType::RelocatableType | QMetaType::IsEnumeration,
                                      nullptr,
                                      [](const QtPrivate::QMetaTypeInterface *){
                                            QtPrivate::SequentialContainerTransformationHelper<QFlags<T>>::registerConverter();
                                            QtPrivate::AssociativeContainerTransformationHelper<QFlags<T>>::registerConverter();
                                            QtPrivate::SequentialContainerTransformationHelper<QFlags<T>>::registerMutableView();
                                            QtPrivate::AssociativeContainerTransformationHelper<QFlags<T>>::registerMutableView();
                                            QtPrivate::MetaTypePairHelper<QFlags<T>>::registerConverter();
                                            QtPrivate::MetaTypeSmartPointerHelper<QFlags<T>>::registerConverter();
                                        }
                                    );
        registerMetaType(fid, metaType);
        QMetaType::registerNormalizedTypedef(flags_qt_name_alias, metaType);
    }
    registerSizeOfType(id, sizeof(T), alignof(T));
    {
        QMetaType metaType = registerMetaType(id,
                                     id,
                                     qt_name,
                                     /*.defaultCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::defaultCtr,
                                     /*.copyCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::copyCtr,
                                     /*.moveCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::moveCtr,
                                     /*.dtor=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::dtor,
                                     /*.equals=*/ QtPrivate::QEqualityOperatorForType<T>::equals,
                                     /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<T>::lessThan,
                                     /*.debugStream=*/ nullptr,
                                     /*.dataStreamOutFn=*/ QtPrivate::QDataStreamOperatorForType<T>::dataStreamOut,
                                     /*.dataStreamInFn=*/ QtPrivate::QDataStreamOperatorForType<T>::dataStreamIn,
                                     /*.legacyRegisterOp=*/ nullptr,
                                     /*.size=*/ sizeof(T),
                                     /*.alignment=*/ alignof(T),
                                     /*.typeId=*/ QtPrivate::BuiltinMetaType<T>::value,
                                     /*.flags=*/ QMetaType::RelocatableType | QMetaType::IsEnumeration,
                                     nullptr,
                                     [](const QtPrivate::QMetaTypeInterface *){
                                            QtPrivate::SequentialContainerTransformationHelper<T>::registerConverter();
                                            QtPrivate::AssociativeContainerTransformationHelper<T>::registerConverter();
                                            QtPrivate::SequentialContainerTransformationHelper<T>::registerMutableView();
                                            QtPrivate::AssociativeContainerTransformationHelper<T>::registerMutableView();
                                            QtPrivate::MetaTypePairHelper<T>::registerConverter();
                                            QtPrivate::MetaTypeSmartPointerHelper<T>::registerConverter();
                                        }
                                     );
        registerMetaType(id, metaType);
    }
    registerOperators<T>();
    registerOperators<QFlags<T>>();
    return id;
}

template<typename T>
const std::type_info& registerPrimitiveTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerPrimitiveTypeInfo(id, qt_name, java_name);
    registerSizeOfType(id, sizeof(T), alignof(T));
    registerOperators<T>();
    return id;
}

template<typename T>
const std::type_info& registerUnspecificTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerUnspecificTypeInfo(id, qt_name, java_name);
    registerSizeOfType(id, sizeof(T), alignof(T));
    registerTypeInfoSupplier(id, Private::PolymorphicTypeInfoSupplier<T>::value);
    return id;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
QTJAMBI_EXPORT void registerNativeInterface(const char* className, QPair<const char*, int>&& nameAndRevision);
template<typename T>
void registerNativeInterface(const char* className){
    registerNativeInterface(className, {QNativeInterface::Private::TypeInfo<T>::name(), QNativeInterface::Private::TypeInfo<T>::revision()});
}
#endif

} // end of namespace RegistryAPI

#endif // QTJAMBI_REGISTRYAPI_H
