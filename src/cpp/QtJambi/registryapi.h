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

#if !defined(QTJAMBI_REGISTRYAPI_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_REGISTRYAPI_H

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QtCore/qnativeinterface.h>
#endif
#include "utils.h"
#include "typetests.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
QT_WARNING_DISABLE_DEPRECATED
#endif

class QtJambiScope;
class AbstractContainerAccess;

class QtJambiTypeInfo
{
public:
    template<typename T>
    static constexpr QtJambiTypeInfo of(){
        return {
#if QT_VERSION < QT_VERSION_CHECK(6, 6, 0)
                QTypeInfo<T>::isPointer
#else
                std::is_pointer<T>::value
#endif
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                ,QTypeInfo<T>::isStatic
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            };
    }

    uint isPointer : 1;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    uint isStatic : 1;
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
};
struct QTJAMBI_EXPORT SignalMetaInfo
{
    typedef int(*SignalMethodIndexProvider)();
    SignalMetaInfo();
    SignalMetaInfo(const SignalMetaInfo& other);
    SignalMetaInfo(SignalMetaInfo&& other);
    SignalMetaInfo& operator=(const SignalMetaInfo& other);
    SignalMetaInfo& operator=(SignalMetaInfo&& other);
    SignalMetaInfo(const char * _signal_name,
                            const char * _signal_signature,
                            const int _signal_argumentcount,
                            SignalMethodIndexProvider _signal_method_index_provider);
    const char * signal_name;
    const char * signal_signature;
    int signal_argumentcount;
    SignalMethodIndexProvider signal_method_index_provider;
};

struct QTJAMBI_EXPORT FunctionInfo{
    enum Flags : quint8{
        None = 0,
        Abstract = 0x01,
        Private = 0x02,
    };
    FunctionInfo();
    FunctionInfo(const FunctionInfo& other);
    FunctionInfo(FunctionInfo&& other);
    FunctionInfo(const char *_name, const char *_signature, Flags _flags = None);
    FunctionInfo& operator=(const FunctionInfo& other);
    FunctionInfo& operator=(FunctionInfo&& other);
    const char * name;
    const char * signature;
    Flags flags;
};

struct QTJAMBI_EXPORT ConstructorInfo{
    typedef void (*Constructor)(void*, JNIEnv*, jobject, jvalue*);
    ConstructorInfo();
    ConstructorInfo(const ConstructorInfo& other);
    ConstructorInfo(Constructor _constructorFunction, const char *_signature);
    ConstructorInfo(ConstructorInfo&& other);
    ConstructorInfo& operator=(const ConstructorInfo& other);
    ConstructorInfo& operator=(ConstructorInfo&& other);
    Constructor constructorFunction;
    const char *signature;
};

class QtJambiScope;

struct QTJAMBI_EXPORT ParameterInfo{
    typedef bool(*QtToJavaConverterFunction)(JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& out, bool forceBoxedType);
    typedef bool(*JavaToQtConverterFunction)(JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType);
    int metaTypeId = 0;
    const char* javaClass = nullptr;
    QtToJavaConverterFunction qtToJavaConverterFunction = nullptr;
    JavaToQtConverterFunction javaToQtConverterFunction = nullptr;
    ParameterInfo();
    ParameterInfo(const ParameterInfo& other);
    ParameterInfo& operator=(const ParameterInfo& other);
    ParameterInfo(ParameterInfo&& other);
    ParameterInfo& operator=(ParameterInfo&& other);
    ParameterInfo(int _metaTypeId);
    ParameterInfo(int _metaTypeId, const char* _javaClass, QtToJavaConverterFunction _qtToJavaConverterFunction, JavaToQtConverterFunction _javaToQtConverterFunction);
};

typedef bool (*ParameterInfoProvider)(const QMetaMethod& method, QList<ParameterInfo>& infos);

typedef void(*Destructor)(void*);

typedef const char* (*QMetaMethodRenamer)(int);

typedef hash_type(*QHashFunctionPtr)(const void*,hash_type);

namespace RegistryAPI{
QTJAMBI_EXPORT void registerHashFunction(const std::type_info& typeId, QHashFunctionPtr hashFunction);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT bool registerComparator(const QtPrivate::AbstractComparatorFunction *f, int type);
QTJAMBI_EXPORT bool registerDebugStreamOperator(const QtPrivate::AbstractDebugStreamFunction *f, int typeId);
#endif
}

namespace QtJambiPrivate {

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
template <typename T>
void genericSaveEnumerator(QDataStream &stream, const void *t)
{
    stream << *static_cast<const typename QIntegerForSize<sizeof(T)>::Unsigned*>(t);
}

template <typename T>
void genericLoadEnumerator(QDataStream &stream, void *t)
{
    stream >> *static_cast<typename QIntegerForSize<sizeof(T)>::Unsigned*>(t);
}
#endif // QT_NO_DATASTREAM

template <class T, bool supported = QtJambiPrivate::supports_IID<T>::value>
constexpr const char * interfaceIID(){
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
    if constexpr(!supported) return nullptr; else
#endif
    return qobject_interface_iid<T*>();
}

template<typename T, bool = QtJambiPrivate::supports_qHash<T>::value>
struct RegistryHelper{
    static void registerHashFunction(){ RegistryAPI::registerHashFunction(typeid(T), [](const void* ptr, hash_type seed)->hash_type{ return !ptr ? 0 : qHash(*reinterpret_cast<const T*>(ptr), seed); }); }
};

template<typename T>
struct RegistryHelper<T,false>{
    static void registerHashFunction(){}
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
template<typename T,
         bool isSupported = !QMetaTypeId2<T>::IsBuiltIn && QMetaTypeId2<T>::Defined,
         bool hasCompare = supports_less_than<T>::value,
         bool hasEquals = supports_equal<T>::value>
struct MetaTypeComparatorHelper{
    static bool registerComparators(int){return false;}
};

template<typename T>
struct MetaTypeComparatorHelper<T,true, true, true>{
    static bool registerComparators(int id){
        if(!QMetaType::hasRegisteredComparators(id)){
            if(id==qMetaTypeId<T>()){
                return QMetaType::registerComparators<T>();
            }else{
                static const QtPrivate::BuiltInComparatorFunction<T> f;
                return RegistryAPI::registerComparator(&f, id);
            }
        }else return false;
    }
};

template<typename T>
struct MetaTypeComparatorHelper<T,true, false, true>{
    static bool registerComparators(int id){
        if(!QMetaType::hasRegisteredComparators(id)){
            if(id==qMetaTypeId<T>()){
                return QMetaType::registerEqualsComparator<T>();
            }else{
                static const QtPrivate::BuiltInEqualsComparatorFunction<T> f;
                return RegistryAPI::registerComparator(&f, id);
            }
        }else return false;
    }
};

template<typename T>
struct MetaTypeComparatorHelper<T,false, true, true>{
    static bool registerComparators(int typeId){
        static const QtPrivate::BuiltInComparatorFunction<T> f;
        return RegistryAPI::registerComparator(&f, typeId);
    }
};

template<typename T>
struct MetaTypeComparatorHelper<T,false, false, true>{
    static bool registerComparators(int typeId){
        static const QtPrivate::BuiltInEqualsComparatorFunction<T> f;
        return RegistryAPI::registerComparator(&f, typeId);
    }
};

template<typename T,
         bool hasStreamOperators = !QMetaTypeId2<T>::IsBuiltIn && supports_stream_operators<T>::value>
struct MetaTypeStreamOperatorsHelper{
    static void registerStreamOperators(int){}
};

template<typename T>
struct MetaTypeStreamOperatorsHelper<T, true>{
    static void registerStreamOperators(int id){
        QMetaType::registerStreamOperators(id, QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Save,
                                               QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Load);
    }
};

template<typename T,
         bool isSupported = !QMetaTypeId2<T>::IsBuiltIn && QMetaTypeId2<T>::Defined,
         bool hasDebugStream = supports_debugstream<T>::value>
struct MetaTypeDebugStreamOperatorHelper{
    static bool registerDebugStreamOperator(int){return false;}
};

template<typename T>
struct MetaTypeDebugStreamOperatorHelper<T,true,true>{
    static bool registerDebugStreamOperator(int id){
        if(id==qMetaTypeId<T>()){
            if(!QMetaType::hasRegisteredDebugStreamOperator(id))
                return QMetaType::registerDebugStreamOperator<T>();
        }else{
            static const QtPrivate::BuiltInDebugStreamFunction<T> f;
            return RegistryAPI::registerDebugStreamOperator(&f, id);
        }
        return false;
    }
};

template<typename T>
struct MetaTypeDebugStreamOperatorHelper<T,false,true>{
    static bool registerDebugStreamOperator(int typeId){
        static const QtPrivate::BuiltInDebugStreamFunction<T> f;
        return RegistryAPI::registerDebugStreamOperator(&f, typeId);
    }
};
#else
template<typename T>
struct QMetaTypeInterfaceFunctions
{
    static inline constexpr const QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr= QtPrivate::QMetaTypeForType<T>::getDefaultCtr();
    static inline constexpr const QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr= QtPrivate::QMetaTypeForType<T>::getCopyCtr();
    static inline constexpr const QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr= QtPrivate::QMetaTypeForType<T>::getMoveCtr();
    static inline constexpr const QtPrivate::QMetaTypeInterface::DtorFn dtor = QtPrivate::QMetaTypeForType<T>::getDtor();
    static inline constexpr const QtPrivate::QMetaTypeInterface::LegacyRegisterOp  legacyRegisterOp = QtPrivate::QMetaTypeForType<T>::getLegacyRegister();
};
#endif
} // end of namespace QtJambiPrivate

typedef void (*AfterRegistrationFunction)(int);
typedef AbstractContainerAccess*(*NewContainerAccessFunction)();
typedef bool (*PolymorphyHandler)(void *object, qintptr& offset);
typedef jobject(* FunctionalResolver)(JNIEnv*,const void*);
typedef const std::type_info* (*TypeInfoSupplier)(const void *object);

namespace RegistryAPI{

template<typename T>
void registerOperators(){
    QtJambiPrivate::RegistryHelper<T>::registerHashFunction();
}

QTJAMBI_EXPORT void registerCustomMetaObject(const std::type_info& typeId, const QMetaObject& superTypeMetaObject);
QTJAMBI_EXPORT void registerMetaObject(const std::type_info& typeId, const QMetaObject& originalMetaObject, bool isValueOwner, QMetaMethodRenamer methodRenamer = nullptr);
QTJAMBI_EXPORT void registerMetaObject(const std::type_info& typeId, const QMetaObject& originalMetaObject, bool isValueOwner,
                                       std::initializer_list<SignalMetaInfo> signalMetaInfos,
                                       ParameterInfoProvider parameterTypeInfoProvider,
                                       QMetaMethodRenamer methodRenamer = nullptr);
QTJAMBI_EXPORT void registerFunctionInfos(const std::type_info& typeId, std::initializer_list<FunctionInfo> virtualFunctions);
QTJAMBI_EXPORT void registerConstructorInfos(const std::type_info& typeId, Destructor destructor, std::initializer_list<ConstructorInfo> constructors);
QTJAMBI_EXPORT void registerMediaControlInfo(const std::type_info& typeId, const char *media_control_iid);
QTJAMBI_EXPORT void registerMetaTypeID(const std::type_info& typeId, int qtMetaType);

QTJAMBI_EXPORT int registerMetaType( const std::type_info& typeId,
                                     const std::type_info& nonPointerTypeId,
                                     const char *typeName,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                     QMetaType::Destructor destructor,
                                     QMetaType::Constructor constructor,
#else
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
#endif
                                     uint size,
                                     ushort align,
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                     int builtInTypeId,
#endif
                                     QMetaType::TypeFlags flags,
                                     const QMetaObject *metaObject,
                                     AfterRegistrationFunction afterRegistrationFunction);

QTJAMBI_EXPORT int registerMetaType( const std::type_info& typeId,
                                     const std::type_info& nonPointerTypeId,
                                     const QByteArray& typeName,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                     QMetaType::Destructor destructor,
                                     QMetaType::Constructor constructor,
#else
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
#endif
                                     uint size,
                                     ushort align,
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                     int builtInTypeId,
#endif
                                     QMetaType::TypeFlags flags,
                                     const QMetaObject *metaObject,
                                     AfterRegistrationFunction afterRegistrationFunction);

template<typename T>
int registerMetaTypeNoMetaObject(const char *typeName,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                  QMetaType::Destructor destructor,
                                  QMetaType::Constructor constructor
#else
                                  QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                  QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                  QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                                  QtPrivate::QMetaTypeInterface::DtorFn dtor,
                                  QtPrivate::QMetaTypeInterface::EqualsFn equals,
                                  QtPrivate::QMetaTypeInterface::LessThanFn lessThan,
                                  QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream,
                                  QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOutFn,
                                  QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamInFn
#endif
                                )
{
    registerOperators<T>();
    return registerMetaType(typeid(T),
                              typeid(typename std::remove_pointer<T>::type),
                              typeName,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                              destructor,
                              constructor,
#else
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
#endif
                              sizeof(T),
                              alignof(T),
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                              QtPrivate::BuiltinMetaType<T>::value,
#endif
                              QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<T>::Flags),
                              nullptr, [](int id){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                            QtJambiPrivate::MetaTypeStreamOperatorsHelper<T>::registerStreamOperators(id);
                                            QtJambiPrivate::MetaTypeComparatorHelper<T>::registerComparators(id);
                                            QtJambiPrivate::MetaTypeDebugStreamOperatorHelper<T>::registerDebugStreamOperator(id);
                                            QtPrivate::SequentialContainerConverterHelper<T>::registerConverter(id);
                                            QtPrivate::AssociativeContainerConverterHelper<T>::registerConverter(id);
                                            QtPrivate::MetaTypePairHelper<T>::registerConverter(id);
                                            QtPrivate::MetaTypeSmartPointerHelper<T>::registerConverter(id);
#else
                                            Q_UNUSED(id)
                                            QtPrivate::SequentialContainerTransformationHelper<T>::registerConverter();
                                            QtPrivate::AssociativeContainerTransformationHelper<T>::registerConverter();
                                            QtPrivate::SequentialContainerTransformationHelper<T>::registerMutableView();
                                            QtPrivate::AssociativeContainerTransformationHelper<T>::registerMutableView();
                                            QtPrivate::MetaTypePairHelper<T>::registerConverter();
                                            QtPrivate::MetaTypeSmartPointerHelper<T>::registerConverter();
#endif

                                });
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
template<typename T>
int registerMetaTypeNoMetaObject(const char *typeName,
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
#endif

template<typename T>
int registerMetaTypeNoMetaObject(const char *typeName)
{
    return registerMetaTypeNoMetaObject<T>(typeName,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                               QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Destruct,
                               QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Construct
#else
                               QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::defaultCtr,
                               QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::copyCtr,
                               QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::moveCtr,
                               QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::dtor,
                               QtPrivate::QEqualityOperatorForType<T>::equals,
                               QtPrivate::QLessThanOperatorForType<T>::lessThan,
                               QtPrivate::QDebugStreamOperatorForType<T>::debugStream,
                               QtPrivate::QDataStreamOperatorForType<T>::dataStreamOut,
                               QtPrivate::QDataStreamOperatorForType<T>::dataStreamIn
#endif
                        );
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
template<typename T>
int registerMetaTypeNoMetaObject(const char *typeName,
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
#endif

template<typename T>
int registerMetaType(const char *typeName,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                   QMetaType::Destructor destructor,
                                   QMetaType::Constructor constructor
#else
                                   QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                   QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                   QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                                   QtPrivate::QMetaTypeInterface::DtorFn dtor,
                                   QtPrivate::QMetaTypeInterface::EqualsFn equals,
                                   QtPrivate::QMetaTypeInterface::LessThanFn lessThan,
                                   QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream,
                                   QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOutFn,
                                   QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamInFn
#endif
                    )
{
    registerOperators<T>();
    return registerMetaType(typeid(T),
                              typeid(typename std::remove_pointer<T>::type),
                              typeName,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                              destructor,
                              constructor,
#else
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
#endif
                              sizeof(T),
                              alignof(T),
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                              QtPrivate::BuiltinMetaType<T>::value,
#endif
                              QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<T>::Flags),
                              QtPrivate::MetaObjectForType<T>::value(),
                              [](int id){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                    QtJambiPrivate::MetaTypeStreamOperatorsHelper<T>::registerStreamOperators(id);
                                    QtJambiPrivate::MetaTypeComparatorHelper<T>::registerComparators(id);
                                    QtJambiPrivate::MetaTypeDebugStreamOperatorHelper<T>::registerDebugStreamOperator(id);
                                    QtPrivate::SequentialContainerConverterHelper<T>::registerConverter(id);
                                    QtPrivate::AssociativeContainerConverterHelper<T>::registerConverter(id);
                                    QtPrivate::MetaTypePairHelper<T>::registerConverter(id);
                                    QtPrivate::MetaTypeSmartPointerHelper<T>::registerConverter(id);
#else
                                    Q_UNUSED(id)
                                    QtPrivate::SequentialContainerTransformationHelper<T>::registerConverter();
                                    QtPrivate::AssociativeContainerTransformationHelper<T>::registerConverter();
                                    QtPrivate::SequentialContainerTransformationHelper<T>::registerMutableView();
                                    QtPrivate::AssociativeContainerTransformationHelper<T>::registerMutableView();
                                    QtPrivate::MetaTypePairHelper<T>::registerConverter();
                                    QtPrivate::MetaTypeSmartPointerHelper<T>::registerConverter();
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                }
                            );
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
template<typename T>
int registerMetaType(const char *typeName,
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
#endif

template<typename T>
int registerMetaType(const char *typeName,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                   QMetaType::Constructor constructor
#else
                                   QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                   QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                   QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr
#endif
                    )
{
    return registerMetaType<T>(typeName,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                              QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Destruct,
                              constructor
#else
                              defaultCtr,
                              copyCtr,
                              moveCtr,
                              QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::dtor,
                              QtPrivate::QEqualityOperatorForType<T>::equals,
                              QtPrivate::QLessThanOperatorForType<T>::lessThan,
                              QtPrivate::QDebugStreamOperatorForType<T>::debugStream,
                              QtPrivate::QDataStreamOperatorForType<T>::dataStreamOut,
                              QtPrivate::QDataStreamOperatorForType<T>::dataStreamIn
#endif
                            );
}

namespace Private{

template<typename T, bool = std::is_polymorphic<T>::value>
struct PolymorphicTypeInfoSupplier{
    static const std::type_info* value(const void *ptr) {
        const T* object = reinterpret_cast<const T*>(ptr);
        try{
            return &typeid(*object);
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
    static int registerMetaType(const char *typeName){
        return RegistryAPI::registerMetaType<T>(typeName,
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                  QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Destruct,
                                  QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Construct
    #else
                                  QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::defaultCtr,
                                  QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::copyCtr,
                                  QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::moveCtr,
                                  QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::dtor,
                                  QtPrivate::QEqualityOperatorForType<T>::equals,
                                  QtPrivate::QLessThanOperatorForType<T>::lessThan,
                                  QtPrivate::QDebugStreamOperatorForType<T>::debugStream,
                                  QtPrivate::QDataStreamOperatorForType<T>::dataStreamOut,
                                  QtPrivate::QDataStreamOperatorForType<T>::dataStreamIn
    #endif
                                );
    }
};

template<typename T>
struct MetaTypeUtil<T,true>{
    static int registerMetaType(const char *){
        registerOperators<T>();
        registerMetaTypeID(typeid(T), QMetaTypeId<T>::qt_metatype_id());
        return QMetaTypeId<T>::qt_metatype_id();
    }
};

}

template<typename T>
int registerMetaType(const char *typeName)
{
    return Private::MetaTypeUtil<T>::registerMetaType(typeName);
}

template<typename T>
int registerMetaType(const QByteArray& typeName,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                   QMetaType::Destructor destructor,
                                   QMetaType::Constructor constructor
#else
                                   QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                   QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                   QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                                   QtPrivate::QMetaTypeInterface::DtorFn dtor,
                                   QtPrivate::QMetaTypeInterface::EqualsFn equals,
                                   QtPrivate::QMetaTypeInterface::LessThanFn lessThan,
                                   QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream,
                                   QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOutFn,
                                   QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamInFn
#endif
                    )
{
    registerOperators<T>();
    return registerMetaType(typeid(T),
                              typeid(typename std::remove_pointer<T>::type),
                              typeName,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                              destructor,
                              constructor,
#else
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
#endif
                              sizeof(T),
                              alignof(T),
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                              QtPrivate::BuiltinMetaType<T>::value,
#endif
                              QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<T>::Flags),
                              QtPrivate::MetaObjectForType<T>::value(),
                              [](int id){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                    QtJambiPrivate::MetaTypeStreamOperatorsHelper<T>::registerStreamOperators(id);
                                    QtJambiPrivate::MetaTypeComparatorHelper<T>::registerComparators(id);
                                    QtJambiPrivate::MetaTypeDebugStreamOperatorHelper<T>::registerDebugStreamOperator(id);
                                    QtPrivate::SequentialContainerConverterHelper<T>::registerConverter(id);
                                    QtPrivate::AssociativeContainerConverterHelper<T>::registerConverter(id);
                                    QtPrivate::MetaTypePairHelper<T>::registerConverter(id);
                                    QtPrivate::MetaTypeSmartPointerHelper<T>::registerConverter(id);
#else
                                    Q_UNUSED(id)
                                    QtPrivate::SequentialContainerTransformationHelper<T>::registerConverter();
                                    QtPrivate::AssociativeContainerTransformationHelper<T>::registerConverter();
                                    QtPrivate::SequentialContainerTransformationHelper<T>::registerMutableView();
                                    QtPrivate::AssociativeContainerTransformationHelper<T>::registerMutableView();
                                    QtPrivate::MetaTypePairHelper<T>::registerConverter();
                                    QtPrivate::MetaTypeSmartPointerHelper<T>::registerConverter();
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                              }
                );
}

template<typename T>
int registerMetaType(const QByteArray& typeName)
{
    return registerMetaType<T>(typeName,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                              QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Destruct,
                              QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Construct
#else
                              QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::defaultCtr,
                              QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::copyCtr,
                              QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::moveCtr,
                              QtJambiPrivate::QMetaTypeInterfaceFunctions<T>::dtor,
                              QtPrivate::QEqualityOperatorForType<T>::equals,
                              QtPrivate::QLessThanOperatorForType<T>::lessThan,
                              QtPrivate::QDebugStreamOperatorForType<T>::debugStream,
                              QtPrivate::QDataStreamOperatorForType<T>::dataStreamOut,
                              QtPrivate::QDataStreamOperatorForType<T>::dataStreamIn
#endif
                            );
}

QTJAMBI_EXPORT void registerContainerAccessFactory(const std::type_info& typeId, NewContainerAccessFunction factory);
QTJAMBI_EXPORT void registerValueTypeInfo(const std::type_info& typeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name);
QTJAMBI_EXPORT void registerObjectTypeInfo(const std::type_info& typeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name);
QTJAMBI_EXPORT void registerQObjectTypeInfo(const std::type_info& typeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name);
QTJAMBI_EXPORT void registerInterfaceTypeInfo(const std::type_info& typeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name, const char *interface_iid);
QTJAMBI_EXPORT void registerInterfaceValueTypeInfo(const std::type_info& typeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name, const char *interface_iid);
QTJAMBI_EXPORT void registerFunctionalTypeInfo(const std::type_info& typeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name);
QTJAMBI_EXPORT void registerEnumTypeInfo(const std::type_info& enumTypeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name);
QTJAMBI_EXPORT void registerEnumTypeInfo(const std::type_info& enumTypeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name, const std::type_info& flagsTypeId, QtJambiTypeInfo flagsInfo, const char *flags_qt_name, const char *flags_qt_name_alias, const char *flags_java_name);
QTJAMBI_EXPORT void registerUnspecificTypeInfo(const std::type_info& typeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name);
QTJAMBI_EXPORT void registerPrimitiveTypeInfo(const std::type_info& typeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name);
QTJAMBI_EXPORT void registerInterfaceID(const std::type_info& typeId, const char *interface_iid);

QTJAMBI_EXPORT void registerNamespaceTypeInfo(const char *qt_name, const char *java_name, const QMetaObject* namespaceMetaObject);
QTJAMBI_EXPORT const char * registerInterfaceID(JNIEnv* env, jclass cls);

QTJAMBI_EXPORT const char* mediaControlIID(JNIEnv *env, jclass javaType);
QTJAMBI_EXPORT jclass classByMediaControlIID(JNIEnv *env, const char* iid);

QTJAMBI_EXPORT void registerDeleter(const std::type_info& typeId, PtrDeleterFunction deleter);
QTJAMBI_EXPORT void registerOwnerFunction(const std::type_info& typeId, PtrOwnerFunction ownerFunction);

QTJAMBI_EXPORT const QObject* mainThreadOwner(const void *);

QTJAMBI_EXPORT void registerPolymorphyHandler(const std::type_info& typeId, const std::type_info& targetTypeId, PolymorphyHandler handler);

QTJAMBI_EXPORT void registerSizeOfType(const std::type_info& typeId, size_t size);

QTJAMBI_EXPORT void registerAlignmentOfType(const std::type_info& typeId, size_t size);

QTJAMBI_EXPORT void registerSizeOfShell(const std::type_info& typeId, size_t size);

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
    registerValueTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name);
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    registerTypeInfoSupplier(id, Private::PolymorphicTypeInfoSupplier<T>::value);
    return id;
}

template<typename T>
const std::type_info& registerObjectTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerObjectTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name);
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    registerTypeInfoSupplier(id, Private::PolymorphicTypeInfoSupplier<T>::value);
    return id;
}

template<typename T>
const std::type_info& registerQObjectTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerQObjectTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name);
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    registerTypeInfoSupplier(id, Private::PolymorphicTypeInfoSupplier<T>::value);
    return id;
}

template<typename T>
const std::type_info& registerInterfaceTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerInterfaceTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name, QtJambiPrivate::interfaceIID<T>());
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    registerTypeInfoSupplier(id, Private::PolymorphicTypeInfoSupplier<T>::value);
    return id;
}

template<typename T>
const std::type_info& registerInterfaceValueTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerInterfaceValueTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name, QtJambiPrivate::interfaceIID<T>());
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    registerTypeInfoSupplier(id, Private::PolymorphicTypeInfoSupplier<T>::value);
    return id;
}

template<typename T,typename Tshell>
const std::type_info& registerFunctionalTypeInfo(const char *qt_name, const char *java_name,
                                                 PtrDeleterFunction deleter,
                                                 Destructor destructor, std::initializer_list<ConstructorInfo> constructors,
                                                 std::initializer_list<FunctionInfo> virtualFunctions)
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QMetaType::registerStreamOperators(metaid, &QtJambiPrivate::genericSaveEnumerator<T>, &QtJambiPrivate::genericLoadEnumerator<T>);
        QtJambiPrivate::MetaTypeComparatorHelper<T>::registerComparators(metaid);
        QtJambiPrivate::MetaTypeDebugStreamOperatorHelper<T>::registerDebugStreamOperator(metaid);
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QMetaType::registerNormalizedTypedef(flags_qt_name_alias, QMetaType(metaid));
#else
        metaid = QMetaType::registerNormalizedTypedef(flags_qt_name_alias, metaid);
        QMetaType::registerStreamOperators(metaid, &QtJambiPrivate::genericSaveEnumerator<QFlags<T>>, &QtJambiPrivate::genericLoadEnumerator<QFlags<T>>);
        QtJambiPrivate::MetaTypeComparatorHelper<QFlags<T>>::registerComparators(metaid);
        QtJambiPrivate::MetaTypeDebugStreamOperatorHelper<QFlags<T>>::registerDebugStreamOperator(metaid);
#endif
    }
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    {
        int metaid = qRegisterMetaType<T>(qt_name);
        registerMetaTypeID(id, metaid);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QMetaType::registerStreamOperators(metaid, &QtJambiPrivate::genericSaveEnumerator<T>, &QtJambiPrivate::genericLoadEnumerator<T>);
        QtJambiPrivate::MetaTypeComparatorHelper<T>::registerComparators(metaid);
        QtJambiPrivate::MetaTypeDebugStreamOperatorHelper<T>::registerDebugStreamOperator(metaid);
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        int metaid = QMetaType::registerType(qt_name,
                        nullptr,
                        nullptr,
                        QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Destruct,
                        QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Construct,
                        sizeof(T),
                        QMetaType::TypeFlags(QMetaType::MovableType | QMetaType::IsEnumeration),
                        nullptr
        );
        QMetaType::registerStreamOperators(metaid, &QtJambiPrivate::genericSaveEnumerator<T>, &QtJambiPrivate::genericLoadEnumerator<T>);
#else
        int metaid = registerMetaType(id,
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
                          [](int id){
                                Q_UNUSED(id)
                                QtPrivate::SequentialContainerTransformationHelper<T>::registerConverter();
                                QtPrivate::AssociativeContainerTransformationHelper<T>::registerConverter();
                                QtPrivate::SequentialContainerTransformationHelper<T>::registerMutableView();
                                QtPrivate::AssociativeContainerTransformationHelper<T>::registerMutableView();
                                QtPrivate::MetaTypePairHelper<T>::registerConverter();
                                QtPrivate::MetaTypeSmartPointerHelper<T>::registerConverter();
                            }
                         );
#endif
        registerMetaTypeID(id, metaid);
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        int metaid = QMetaType::registerType(flags_qt_name,
                        nullptr,
                        nullptr,
                        QtMetaTypePrivate::QMetaTypeFunctionHelper<QFlags<T>>::Destruct,
                        QtMetaTypePrivate::QMetaTypeFunctionHelper<QFlags<T>>::Construct,
                        sizeof(QFlags<T>),
                        QMetaType::TypeFlags(QMetaType::MovableType | QMetaType::IsEnumeration),
                        nullptr
         );
        QMetaType::registerStreamOperators(metaid, &QtJambiPrivate::genericSaveEnumerator<T>, &QtJambiPrivate::genericLoadEnumerator<T>);
#else
        int metaid = registerMetaType(id,
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
                                      [](int id){
                                            Q_UNUSED(id)
                                            QtPrivate::SequentialContainerTransformationHelper<QFlags<T>>::registerConverter();
                                            QtPrivate::AssociativeContainerTransformationHelper<QFlags<T>>::registerConverter();
                                            QtPrivate::SequentialContainerTransformationHelper<QFlags<T>>::registerMutableView();
                                            QtPrivate::AssociativeContainerTransformationHelper<QFlags<T>>::registerMutableView();
                                            QtPrivate::MetaTypePairHelper<QFlags<T>>::registerConverter();
                                            QtPrivate::MetaTypeSmartPointerHelper<QFlags<T>>::registerConverter();
                                        }
                                    );
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

        registerMetaTypeID(fid, metaid);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QMetaType::registerNormalizedTypedef(flags_qt_name_alias, QMetaType(metaid));
#else
        QMetaType::registerNormalizedTypedef(flags_qt_name_alias, metaid);
#endif
    }
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        int metaid = QMetaType::registerType(qt_name,
                        nullptr,
                        nullptr,
                        QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Destruct,
                        QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Construct,
                        sizeof(T),
                        QMetaType::TypeFlags(QMetaType::MovableType | QMetaType::IsEnumeration),
                        nullptr
        );
        QMetaType::registerStreamOperators(metaid, &QtJambiPrivate::genericSaveEnumerator<T>, &QtJambiPrivate::genericLoadEnumerator<T>);
#else
        int metaid = registerMetaType(id,
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
                          [](int id){
                                Q_UNUSED(id)
                                QtPrivate::SequentialContainerTransformationHelper<T>::registerConverter();
                                QtPrivate::AssociativeContainerTransformationHelper<T>::registerConverter();
                                QtPrivate::SequentialContainerTransformationHelper<T>::registerMutableView();
                                QtPrivate::AssociativeContainerTransformationHelper<T>::registerMutableView();
                                QtPrivate::MetaTypePairHelper<T>::registerConverter();
                                QtPrivate::MetaTypeSmartPointerHelper<T>::registerConverter();
                            }
                         );
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        registerMetaTypeID(id, metaid);
    }
    registerOperators<T>();
    registerOperators<QFlags<T>>();
    return id;
}

template<typename T>
const std::type_info& registerPrimitiveTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerPrimitiveTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name);
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    registerSizeOfType(id, sizeof(T));
    registerOperators<T>();
    return id;
}

template<typename T>
const std::type_info& registerUnspecificTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerUnspecificTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name);
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    registerTypeInfoSupplier(id, Private::PolymorphicTypeInfoSupplier<T>::value);
    return id;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT void registerNativeInterface(const char* className, QPair<const char*, int>&& nameAndRevision);
template<typename T>
void registerNativeInterface(const char* className){
    registerNativeInterface(className, {QNativeInterface::Private::TypeInfo<T>::name(), QNativeInterface::Private::TypeInfo<T>::revision()});
}
#endif

} // end of namespace RegistryAPI

#endif // QTJAMBI_REGISTRYAPI_H
