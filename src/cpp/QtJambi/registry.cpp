/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <qglobal.h>
QT_WARNING_DISABLE_DEPRECATED

#include <QtCore/QByteArray>
#include <QtCore/QThreadStorage>
#include <QtCore/QEvent>
#include <QtCore/QThread>
#include <QtCore/private/qcoreapplication_p.h>

#include <typeindex>
#include <typeinfo>

#include "qtjambiapi.h"
#include "registryapi.h"
#include "containerapi.h"
#include "java_p.h"
#include "registryutil_p.h"
#include "containeraccess_p.h"
#include "coreapi.h"
#include "qtjambimetaobject_p.h"
#include "typeentry_p.h"
#include "typemanager_p.h"
#include "utils_p.h"
#include "qmlapi.h"
#include "supertypeinfo_p.h"
#include "qtjambi_cast.h"

Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gLock, (QReadWriteLock::Recursive))
typedef QMap<size_t, EntryTypes> TypeEntryTypesHash;
typedef QMap<size_t, const char*> TypeStringHash;
typedef QMap<size_t, QSharedPointer<QtJambiTypeInfo>> TypeIdToQTypeInfoHash;
typedef QHash<QByteArray, const char*> StringStringHash;
typedef QHash<QByteArray, const std::type_info*> StringTypeHash;
typedef QHash<QByteArray, QList<const std::type_info*>> StringTypesHash;
Q_GLOBAL_STATIC(TypeStringHash, gTypeQtNameHash)
Q_GLOBAL_STATIC(TypeIdToQTypeInfoHash, gQTypeInfoHash)
Q_GLOBAL_STATIC(TypeEntryTypesHash, gEntryTypesHash)
Q_GLOBAL_STATIC(StringTypeHash, gQtNameTypeHash)
Q_GLOBAL_STATIC(StringStringHash, gQtFunctionalJavaNameHash)
Q_GLOBAL_STATIC(TypeStringHash, gTypeJavaNameHash)
Q_GLOBAL_STATIC(StringTypesHash, gJavaNameTypeHash)
typedef QSet<size_t> TypeSet;
Q_GLOBAL_STATIC(StringTypeHash, gIIDTypeHash)
Q_GLOBAL_STATIC(StringStringHash, gTypeIIDHash)
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
typedef QMap<QByteArray,QPair<const char*, int>> NativeInterfaceMap;
Q_GLOBAL_STATIC(NativeInterfaceMap, gNativeInterfaceMap)
#endif
typedef QHash<QByteArray, QByteArray> IIDByteArrayHash;
Q_GLOBAL_STATIC(IIDByteArrayHash, gIIDByteArrayHash)
typedef QMap<size_t, const std::type_info*> ID2IDHash;
Q_GLOBAL_STATIC(ID2IDHash, gFlagEnumIDHash)
Q_GLOBAL_STATIC(ID2IDHash, gEnumFlagIDHash)
Q_GLOBAL_STATIC(TypeStringHash, gTypeJavaInterfaceHash)
typedef QMap<size_t, NewContainerAccessFunction> SequentialContainerAccessFactoryHash;
Q_GLOBAL_STATIC(SequentialContainerAccessFactoryHash, gSequentialContainerAccessFactoryHash)

typedef QHash<int, QHash<int,std::function<AbstractContainerAccess*()>>> SequentialContainerAccessFactoryByMetaTypeHash;
Q_GLOBAL_STATIC(SequentialContainerAccessFactoryByMetaTypeHash, gSequentialContainerAccessFactoryByMetaTypeHash)
typedef QHash<QPair<int,int>, QHash<int,std::function<AbstractContainerAccess*()>>> MapAccessFactoryByMetaTypeHash;
Q_GLOBAL_STATIC(MapAccessFactoryByMetaTypeHash, gMapAccessFactoryByMetaTypeHash)
typedef QHash<int, const std::type_info*> MetaTypeIDHash;
typedef QMap<size_t, int> MetaTypeIDMap;
Q_GLOBAL_STATIC(MetaTypeIDMap, gMetaTypeIDMap)
Q_GLOBAL_STATIC(MetaTypeIDHash, gMetaTypeIDHash)
typedef QMap<size_t, const void*> DefaultValueHash;
Q_GLOBAL_STATIC(DefaultValueHash, gDefaultValueHash)
typedef QMap<size_t, QVector<FunctionInfo> > FunctionInfoHash;
Q_GLOBAL_STATIC(FunctionInfoHash, gVirtualFunctionInfos)
typedef QMap<size_t, QVector<ConstructorInfo> > ConstructorInfoHash;
Q_GLOBAL_STATIC(ConstructorInfoHash, gConstructorInfos)
typedef QMap<size_t, Destructor> DestructorHash;
Q_GLOBAL_STATIC(DestructorHash, gDestructorHash)
typedef QHash<hash_type, const char*> NameHash;
Q_GLOBAL_STATIC(NameHash, gFlagEnumNameHash)
Q_GLOBAL_STATIC(NameHash, gInterfaceHash)
Q_GLOBAL_STATIC(NameHash, gInterfaceIIDsHash)
typedef QSet<hash_type> HashSet;
Q_GLOBAL_STATIC(HashSet, gFunctionalHash)
typedef QMap<size_t, PtrDeleterFunction> DeleterHash;
Q_GLOBAL_STATIC(DeleterHash, gDeleterHash)
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
typedef QHash<const QtPrivate::QMetaTypeInterface *, jclass> MetaTypeJavaTypeHash;
typedef QMultiHash<QByteArray,QMetaType> JavaTypeMetaTypesHash;
typedef QSet<const QtPrivate::QMetaTypeInterface *> JObjectWrappedMetaTypeHash;
#else
typedef QHash<int, jclass> MetaTypeJavaTypeHash;
typedef QMultiHash<QByteArray,int> JavaTypeMetaTypesHash;
typedef QSet<int> JObjectWrappedMetaTypeHash;
#endif
Q_GLOBAL_STATIC(MetaTypeJavaTypeHash, gMetaTypeJavaTypeHash)
Q_GLOBAL_STATIC(JavaTypeMetaTypesHash, gJavaTypeMetaTypesHash)
Q_GLOBAL_STATIC(JObjectWrappedMetaTypeHash, gJObjectWrappedMetaTypes)

typedef QMap<size_t, PtrOwnerFunction> OwnerFunctionHash;
Q_GLOBAL_STATIC(OwnerFunctionHash, gOwnerFunctionHash)

struct PolymorphicIdHandler;
typedef QMultiHash<size_t, const PolymorphicIdHandler* > PolymorphicIdHash;
Q_GLOBAL_STATIC(PolymorphicIdHash, g_polymorphic_ids)
typedef QMultiMap<size_t, const std::type_info*> PolymorphicBasesHash;
Q_GLOBAL_STATIC(PolymorphicBasesHash, gPolymorphicBasesHash)

typedef QMap<size_t, QHashFunctionPtr> HashFunctionHash;
typedef QMap<size_t, size_t> SizeHash;
typedef QMap<size_t, QMap<size_t, int>> OffsetHash;
typedef QMap<size_t, QSet<const std::type_info*>> InterfacesHash;
Q_GLOBAL_STATIC(SizeHash, gValueSizeHash)
Q_GLOBAL_STATIC(SizeHash, gValueAlignmentHash)
Q_GLOBAL_STATIC(SizeHash, gShellSizeHash)
Q_GLOBAL_STATIC(HashFunctionHash, gHashFunctionHash)
Q_GLOBAL_STATIC(OffsetHash, gOffsetHash)
Q_GLOBAL_STATIC(InterfacesHash, gInterfacesHash)

typedef QHash<uint, InterfaceOffsetInfo > InterfaceOffsetHash;
Q_GLOBAL_STATIC(InterfaceOffsetHash, gInterfaceOffsetHash)

typedef QMap<size_t, const QMetaObject*> MetaObjectHash;
Q_GLOBAL_STATIC(MetaObjectHash, gOriginalMetaObjectHash)
Q_GLOBAL_STATIC(MetaObjectHash, gHasCustomMetaObjectHash)
typedef QHash<const QMetaObject*, QVector<SignalMetaInfo> > SignalMetaInfoHash;
Q_GLOBAL_STATIC(SignalMetaInfoHash, gSignalMetaInfos)
typedef QHash<const QMetaObject*, QList<QMetaMethod> > ExtraSignalsOfMetaObjectHash;
Q_GLOBAL_STATIC(ExtraSignalsOfMetaObjectHash, gExtraSignalsOfMetaObject)
typedef QHash<const QMetaObject*, const std::type_info* > TypeIdsByMetaObjectHash;
Q_GLOBAL_STATIC(TypeIdsByMetaObjectHash, gTypeIdsByMetaObject)
typedef QHash<const QMetaObject*, QMetaMethodRenamer > QMetaMethodRenamerByMetaObjectHash;
Q_GLOBAL_STATIC(QMetaMethodRenamerByMetaObjectHash, gQMetaMethodRenamerByMetaObject)
typedef QSet<const QMetaObject*> ValueOwnerSet;
Q_GLOBAL_STATIC(ValueOwnerSet, gValueOwners)
typedef QHash<const QMetaObject*,bool> ValueOwnerHash;
Q_GLOBAL_STATIC(ValueOwnerHash, gValueOwnerHash)
typedef QHash<const QMetaObject*, ParameterInfoProvider > ParameterTypeInfoProviderHash;
Q_GLOBAL_STATIC(ParameterTypeInfoProviderHash, gParameterTypeInfoProviders)
typedef QHash<const QMetaObject*, QHash<int,const char*>> RenamedMethodsHash;
Q_GLOBAL_STATIC(RenamedMethodsHash, gRenamedMethodsHash)

typedef QMap<size_t, FunctionalResolver> FunctionalResolverHash;
Q_GLOBAL_STATIC(FunctionalResolverHash, gFunctionalResolverHash)

Q_GLOBAL_STATIC(TypeStringHash, gMediaControlIIDHash)
Q_GLOBAL_STATIC(StringTypeHash, gMediaControlIIDClassHash)

Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gConstructorLock, (QReadWriteLock::Recursive))
typedef QHash<hash_type,jmethodID> ConstructorIDHash;
Q_GLOBAL_STATIC(ConstructorIDHash, gConstructorHash)

#ifdef QTJAMBI_LOG_CLASSNAMES
typedef QHash<hash_type, QString> ClassNameHash;
Q_GLOBAL_STATIC(ClassNameHash, gClassNameHash)
#endif

Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gClassLock, (QReadWriteLock::Recursive))
typedef QHash<hash_type, jclass> ClassIdHash;
Q_GLOBAL_STATIC(ClassIdHash, gClassHash)

Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gFieldLock, (QReadWriteLock::Recursive))
typedef QHash<hash_type, jfieldID> FieldIdHash;
Q_GLOBAL_STATIC(FieldIdHash, gFieldHash)

Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gMethodLock, (QReadWriteLock::Recursive))
typedef QHash<hash_type, jmethodID> MethodIdHash;
Q_GLOBAL_STATIC(MethodIdHash, gMethodHash)

Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gQtSuperclassLock, (QReadWriteLock::Recursive))
typedef QHash<hash_type, jclass> ClassHash;
Q_GLOBAL_STATIC(ClassHash, gQtSuperclassHash)

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
typedef QHash<const QtPrivate::QMetaTypeInterface*,const QMetaObject*> MetaTypeMetaObjectHash;
Q_GLOBAL_STATIC(MetaTypeMetaObjectHash, gMetaTypeMetaObjectHash)
typedef QSet<QByteArray> TypeNameSet;
Q_GLOBAL_STATIC(TypeNameSet, gTypeNames)
#endif

#ifdef QTJAMBI_COUNTCACHEMISSES
static void cacheMisses(const char *s)
{
    static int count = 0;

    qDebug("Total # of cache misses: %d : '%s'", ++count, s);
}
#endif

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
class JObjectWrapperConverter : public QtPrivate::AbstractConverterFunction{
public:
    static void registerConverter(const std::type_info& _typeId, int _to);
private:
    JObjectWrapperConverter(const std::type_info& _typeId, int _to);
    ~JObjectWrapperConverter();
    static int jobjectWrapperID();
    static bool convert(const AbstractConverterFunction *converterFunction, const void * from, void* to);
    const std::type_info& typeId;
    int to;

    template <class T, typename Deleter>
    friend struct QtSharedPointer::CustomDeleter;
};
#endif

ParameterInfo::ParameterInfo()
    : metaTypeId(0),
      javaClass(nullptr),
      qtToJavaConverterFunction(nullptr),
      javaToQtConverterFunction(nullptr){}
ParameterInfo::ParameterInfo(const ParameterInfo& other)
    : metaTypeId(other.metaTypeId),
      javaClass(other.javaClass),
      qtToJavaConverterFunction(other.qtToJavaConverterFunction),
      javaToQtConverterFunction(other.javaToQtConverterFunction){}
ParameterInfo::ParameterInfo(int _metaTypeId):metaTypeId(_metaTypeId){}
ParameterInfo::ParameterInfo(int _metaTypeId, const char* _javaClass, QtToJavaConverterFunction _qtToJavaConverterFunction, JavaToQtConverterFunction _javaToQtConverterFunction)
    : metaTypeId(_metaTypeId),
      javaClass(_javaClass),
      qtToJavaConverterFunction(_qtToJavaConverterFunction),
      javaToQtConverterFunction(_javaToQtConverterFunction){}
ParameterInfo& ParameterInfo::operator=(const ParameterInfo& other){
    metaTypeId = other.metaTypeId;
    javaClass = other.javaClass;
    qtToJavaConverterFunction = other.qtToJavaConverterFunction;
    javaToQtConverterFunction = other.javaToQtConverterFunction;
    return *this;
}
ParameterInfo::ParameterInfo(ParameterInfo&& other)
    : metaTypeId(other.metaTypeId),
      javaClass(other.javaClass),
      qtToJavaConverterFunction(other.qtToJavaConverterFunction),
      javaToQtConverterFunction(other.javaToQtConverterFunction){}
ParameterInfo& ParameterInfo::operator=(ParameterInfo&& other){
    metaTypeId = other.metaTypeId;
    javaClass = other.javaClass;
    qtToJavaConverterFunction = other.qtToJavaConverterFunction;
    javaToQtConverterFunction = other.javaToQtConverterFunction;
    return *this;
}

ConstructorInfo::ConstructorInfo(): constructorFunction(nullptr),signature(nullptr){}
ConstructorInfo::ConstructorInfo(const ConstructorInfo& copy): constructorFunction(copy.constructorFunction),signature(copy.signature){}
ConstructorInfo::ConstructorInfo(ConstructorInfo&& copy): constructorFunction(copy.constructorFunction),signature(copy.signature){}
ConstructorInfo::ConstructorInfo(Constructor _constructorFunction, const char *_signature): constructorFunction(_constructorFunction),signature(_signature){}
ConstructorInfo& ConstructorInfo::operator=(const ConstructorInfo& copy){
    constructorFunction = copy.constructorFunction;
    signature = copy.signature;
    return *this;
}
ConstructorInfo& ConstructorInfo::operator=(ConstructorInfo&& copy){
    constructorFunction = copy.constructorFunction;
    signature = copy.signature;
    return *this;
}

FunctionInfo::FunctionInfo(): name(nullptr),signature(nullptr),flags(None){}
FunctionInfo::FunctionInfo(const FunctionInfo& copy): name(copy.name),signature(copy.signature),flags(copy.flags){}
FunctionInfo::FunctionInfo(FunctionInfo&& copy): name(copy.name),signature(copy.signature),flags(copy.flags){}
FunctionInfo::FunctionInfo(const char *_name, const char *_signature, Flags _flags): name(_name),signature(_signature),flags(_flags){}
FunctionInfo& FunctionInfo::operator=(const FunctionInfo& copy){
    name = copy.name;
    signature = copy.signature;
    flags = copy.flags;
    return *this;
}
FunctionInfo& FunctionInfo::operator=(FunctionInfo&& copy){
    name = copy.name;
    signature = copy.signature;
    flags = copy.flags;
    return *this;
}

SignalMetaInfo::SignalMetaInfo():
    signal_name(nullptr),
    signal_signature(nullptr),
    signal_argumentcount(0),
    signal_method_index_provider(nullptr){}
SignalMetaInfo::SignalMetaInfo(const SignalMetaInfo& copy):
    signal_name(copy.signal_name),
    signal_signature(copy.signal_signature),
    signal_argumentcount(copy.signal_argumentcount),
    signal_method_index_provider(copy.signal_method_index_provider)
{}
SignalMetaInfo::SignalMetaInfo(SignalMetaInfo&& copy):
    signal_name(copy.signal_name),
    signal_signature(copy.signal_signature),
    signal_argumentcount(copy.signal_argumentcount),
    signal_method_index_provider(copy.signal_method_index_provider)
{}
SignalMetaInfo::SignalMetaInfo(const char * _signal_name,
                        const char * _signal_signature,
                        const int _signal_argumentcount,
                        SignalMethodIndexProvider _signal_method_index_provider):
    signal_name(_signal_name),
    signal_signature(_signal_signature),
    signal_argumentcount(_signal_argumentcount),
    signal_method_index_provider(_signal_method_index_provider)
{}
SignalMetaInfo& SignalMetaInfo::operator=(const SignalMetaInfo& copy){
    signal_name = copy.signal_name;
    signal_signature = copy.signal_signature;
    signal_argumentcount = copy.signal_argumentcount;
    signal_method_index_provider = copy.signal_method_index_provider;
    return *this;
}
SignalMetaInfo& SignalMetaInfo::operator=(SignalMetaInfo&& copy){
    signal_name = copy.signal_name;
    signal_signature = copy.signal_signature;
    signal_argumentcount = copy.signal_argumentcount;
    signal_method_index_provider = copy.signal_method_index_provider;
    return *this;
}

hash_type qHash(const char *p, hash_type seed) Q_DECL_NOEXCEPT
{
    hash_type h = seed;
    hash_type g;

    if(p){
        while (*p != 0) {
            h = (h << 4) + hash_type(*p++);
            if ((g = (h & 0xf0000000)) != 0)
                h ^= g >> 23;
            h &= ~g;
        }
    }
    return h;
}

namespace std{
hash_type qHash(const type_index& idx, hash_type seed) Q_DECL_NOEXCEPT
{
    return ::qHash(unique_id(idx), seed);
}

hash_type qHash(const type_index& idx) Q_DECL_NOEXCEPT
{
    return ::qHash(unique_id(idx));
}
}

void registerTypeAlias(const std::type_info& typeId, const char *qt_name, const char *java_name)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(qt_name){
        QByteArray _qt_name(qt_name);
        if(!gQtNameTypeHash->contains(_qt_name))
            gQtNameTypeHash->insert(_qt_name, &typeId);
    }
    if(java_name)
        (*gJavaNameTypeHash)[java_name].append(&typeId);
}

void registerTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name, EntryTypes entryTypes)
{
    {
        QWriteLocker locker(gLock());
        Q_UNUSED(locker)
        if(!gTypeQtNameHash->contains(unique_id(typeId))){
            gTypeQtNameHash->insert(unique_id(typeId), qt_name);
        }
        if(!gQTypeInfoHash->contains(unique_id(typeId))){
            gQTypeInfoHash->insert(unique_id(typeId), QSharedPointer<QtJambiTypeInfo>(new QtJambiTypeInfo(info)));
        }
        gQtNameTypeHash->insert(qt_name, &typeId);

        if(!gTypeJavaNameHash->contains(unique_id(typeId))){
            gTypeJavaNameHash->insert(unique_id(typeId), java_name);
        }
        (*gJavaNameTypeHash)[java_name].append(&typeId);
        if(entryTypes!=EntryTypes::Unspecific)
            gEntryTypesHash->insert(unique_id(typeId), entryTypes);
        if(entryTypes==EntryTypes::FunctionalTypeInfo)
            gQtFunctionalJavaNameHash->insert(qt_name, java_name);
    }
    QByteArray type_name = QtJambiAPI::typeName(typeId);
    if(type_name!=QByteArray(qt_name)){
        registerTypeAlias(typeId, type_name, nullptr);
    }
}

const QObject* RegistryAPI::mainThreadOwner(const void *)
{
    if(QCoreApplicationPrivate::is_app_closing){
        return nullptr;
    }
    return QCoreApplication::instance();
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void RegistryAPI::registerNativeInterface(const char* className, QPair<const char*, int>&& nameAndRevision){
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gNativeInterfaceMap()->insert(className, std::move(nameAndRevision));
}

bool registeredNativeInterface(JNIEnv* env, jclass cls, QPair<const char*, int>& nameAndRevision){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    QByteArray className = QtJambiAPI::getClassName(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')).toUtf8();
    if(gNativeInterfaceMap()->contains(className)){
        nameAndRevision = gNativeInterfaceMap()->value(className);
        return true;
    }
    return false;
}

CoreAPI::NITypeInfo CoreAPI::getNativeInterfaceInfo(JNIEnv * env, jclass cls){
    if(!cls)
        JavaException::raiseNullPointerException(env, "Class must not be null." QTJAMBI_STACKTRACEINFO );
    QPair<const char*, int> nameAndRevision;
    NITypeInfo info;
    if(registeredNativeInterface(env, cls, nameAndRevision)){
        info.name = nameAndRevision.first;
        info.revision = nameAndRevision.second;
    }else{
        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Class %1 is not a native interface type.").arg(QtJambiAPI::getClassName(env, cls).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
    }
    return info;
}
#endif

void RegistryAPI::registerInterfaceTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name, const char *interface_iid)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::InterfaceTypeInfo);
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gIIDTypeHash->insert(interface_iid, &typeId);
    gTypeIIDHash->insert(java_name, interface_iid);
}

void RegistryAPI::registerInterfaceID(const std::type_info& typeId, const char *interface_iid)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gIIDTypeHash->insert(interface_iid, &typeId);
    gTypeIIDHash->insert(gTypeJavaNameHash->value(unique_id(typeId), nullptr), interface_iid);
}

const char * registeredInterfaceID(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gTypeIIDHash->value(gTypeJavaNameHash->value(unique_id(typeId), nullptr));
}

const char * RegistryAPI::registerInterfaceID(JNIEnv* env, jclass cls)
{
    cls = getGlobalClassRef(env, cls);
    QString iid = QtJambiAPI::getClassName(env, cls);
    QByteArray className = iid.toLatin1().replace('.', '/');
    {
        QWriteLocker locker(gLock());
        Q_UNUSED(locker)
        if(!gTypeIIDHash->contains(className)){
            const char* _iid = gIIDByteArrayHash->insert(iid.toLatin1(), className).key();
            gTypeIIDHash->insert(className, _iid);
            return _iid;
        }else{
            return (*gTypeIIDHash)[className];
        }
    }
}

void RegistryAPI::registerInterfaceValueTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name, const char *interface_iid)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::InterfaceValueTypeInfo);
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gIIDTypeHash->insert(interface_iid, &typeId);
    gTypeIIDHash->insert(java_name, interface_iid);
}

void RegistryAPI::registerFunctionalTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::FunctionalTypeInfo);
}

void RegistryAPI::registerQObjectTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::QObjectTypeInfo);
}

void RegistryAPI::registerObjectTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::ObjectTypeInfo);
}

void RegistryAPI::registerValueTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::ValueTypeInfo);
}

void RegistryAPI::registerEnumTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::EnumTypeInfo);
}

void RegistryAPI::registerEnumTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name, const std::type_info& flagsTypeId, const QtJambiTypeInfo& flagsInfo, const char *flags_qt_name, const char *flags_qt_name_alias, const char *flags_java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::EnumTypeInfo);
    registerTypeInfo(flagsTypeId, flagsInfo, flags_qt_name, flags_java_name, EntryTypes::FlagsTypeInfo);
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(QLatin1String(flags_qt_name)!=QLatin1String(flags_qt_name_alias))
        gQtNameTypeHash->insert(flags_qt_name_alias, &flagsTypeId);
    gFlagEnumIDHash->insert(unique_id(flagsTypeId), &typeId);
    gEnumFlagIDHash->insert(unique_id(typeId), &flagsTypeId);
}

void RegistryAPI::registerUnspecificTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::Unspecific);
}

void RegistryAPI::registerPrimitiveTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::PrimitiveTypeInfo);
}

void registerContainerTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name, const char *java_interface)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::Unspecific);
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(!gTypeJavaInterfaceHash->contains(unique_id(typeId))){
        gTypeJavaInterfaceHash->insert(unique_id(typeId), java_interface);
    }
}

void RegistryAPI::registerContainerAccessFactory(const std::type_info& typeId, NewContainerAccessFunction factory){
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(!gSequentialContainerAccessFactoryHash->contains(unique_id(typeId))){
        gSequentialContainerAccessFactoryHash->insert(unique_id(typeId), factory);
    }
    if(AbstractContainerAccess* access = factory()){
        if(AbstractListAccess* ca = dynamic_cast<AbstractListAccess*>(access)){
            int mid = ca->elementMetaType().id();
            if(!gSequentialContainerAccessFactoryByMetaTypeHash->contains(mid)){
                (*gSequentialContainerAccessFactoryByMetaTypeHash)[mid][int(SequentialContainerType::QList)] = factory;
            }
        }else if(AbstractSetAccess* ca = dynamic_cast<AbstractSetAccess*>(access)){
            int mid = ca->elementMetaType().id();
            if(!gSequentialContainerAccessFactoryByMetaTypeHash->contains(mid)){
                (*gSequentialContainerAccessFactoryByMetaTypeHash)[mid][int(SequentialContainerType::QSet)] = factory;
            }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        }else if(AbstractLinkedListAccess* ca = dynamic_cast<AbstractLinkedListAccess*>(access)){
            int mid = ca->elementMetaType().id();
            if(!gSequentialContainerAccessFactoryByMetaTypeHash->contains(mid)){
                (*gSequentialContainerAccessFactoryByMetaTypeHash)[mid][int(SequentialContainerType::QLinkedList)] = factory;
            }
        }else if(AbstractVectorAccess* ca = dynamic_cast<AbstractVectorAccess*>(access)){
            int mid = ca->elementMetaType().id();
            if(!gSequentialContainerAccessFactoryByMetaTypeHash->contains(mid)){
                (*gSequentialContainerAccessFactoryByMetaTypeHash)[mid][int(SequentialContainerType::QVector)] = factory;
            }
#endif
        }else if(AbstractHashAccess* ca = dynamic_cast<AbstractHashAccess*>(access)){
            int kid = ca->keyMetaType().id();
            int vid = ca->valueMetaType().id();
            if(!gMapAccessFactoryByMetaTypeHash->contains({kid,vid})){
                (*gMapAccessFactoryByMetaTypeHash)[{kid,vid}][int(AssociativeContainerType::QHash)] = factory;
            }
        }else if(AbstractMapAccess* ca = dynamic_cast<AbstractMapAccess*>(access)){
            int kid = ca->keyMetaType().id();
            int vid = ca->valueMetaType().id();
            if(!gMapAccessFactoryByMetaTypeHash->contains({kid,vid})){
                (*gMapAccessFactoryByMetaTypeHash)[{kid,vid}][int(AssociativeContainerType::QMap)] = factory;
            }
        }else if(AbstractMultiHashAccess* ca = dynamic_cast<AbstractMultiHashAccess*>(access)){
            int kid = ca->keyMetaType().id();
            int vid = ca->valueMetaType().id();
            if(!gMapAccessFactoryByMetaTypeHash->contains({kid,vid})){
                (*gMapAccessFactoryByMetaTypeHash)[{kid,vid}][int(AssociativeContainerType::QMultiHash)] = factory;
            }
        }else if(AbstractMultiMapAccess* ca = dynamic_cast<AbstractMultiMapAccess*>(access)){
            int kid = ca->keyMetaType().id();
            int vid = ca->valueMetaType().id();
            if(!gMapAccessFactoryByMetaTypeHash->contains({kid,vid})){
                (*gMapAccessFactoryByMetaTypeHash)[{kid,vid}][int(AssociativeContainerType::QMultiMap)] = factory;
            }
        }
        access->dispose();
    }
}

std::function<AbstractContainerAccess*()> getSequentialContainerAccessFactory(SequentialContainerType containerType, const QMetaType& type){
    QReadLocker locker(gLock());
    if(containerType==SequentialContainerType::QQueue){
        containerType = SequentialContainerType::QList;
    }else if(containerType==SequentialContainerType::QStack){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        containerType = SequentialContainerType::QVector;
#else
        containerType = SequentialContainerType::QList;
#endif
    }
    if(gSequentialContainerAccessFactoryByMetaTypeHash->contains(type.id())){
        return (*gSequentialContainerAccessFactoryByMetaTypeHash)[type.id()][int(containerType)];
    }else{
        return nullptr;
    }
}

std::function<AbstractContainerAccess*()> getSequentialContainerAccessFactory(AssociativeContainerType containerType, const QMetaType& keyType, const QMetaType& valueType){
    QReadLocker locker(gLock());
    QPair<int,int> pair{keyType.id(), valueType.id()};
    if(gMapAccessFactoryByMetaTypeHash->contains(pair)){
        return (*gMapAccessFactoryByMetaTypeHash)[pair][int(containerType)];
    }else{
        return nullptr;
    }
}

void registerContainerAccessFactory(SequentialContainerType containerType, const QMetaType& elementType, std::function<AbstractContainerAccess*()>&& factory){
    QWriteLocker locker(gLock());
    if(!(*gSequentialContainerAccessFactoryByMetaTypeHash)[elementType.id()].contains(int(containerType))
            || !(*gSequentialContainerAccessFactoryByMetaTypeHash)[elementType.id()][int(containerType)])
        (*gSequentialContainerAccessFactoryByMetaTypeHash)[elementType.id()][int(containerType)] = std::move(factory);
}

void registerContainerAccessFactory(AssociativeContainerType containerType, const QMetaType& keyType, const QMetaType& valueType, std::function<AbstractContainerAccess*()>&& factory){
    QWriteLocker locker(gLock());
    QPair<int,int> pair{keyType.id(), valueType.id()};
    if(gMapAccessFactoryByMetaTypeHash->contains(pair)){
        if(!(*gMapAccessFactoryByMetaTypeHash)[pair].contains(int(containerType))
                || !(*gMapAccessFactoryByMetaTypeHash)[pair][int(containerType)])
            (*gMapAccessFactoryByMetaTypeHash)[pair][int(containerType)] = std::move(factory);
    }else{
        (*gMapAccessFactoryByMetaTypeHash)[pair][int(containerType)] = std::move(factory);
    }
}

NewContainerAccessFunction getSequentialContainerAccessFactory(const std::type_info& typeId){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gSequentialContainerAccessFactoryHash->value(unique_id(typeId));
}


EntryTypes getEntryType(const std::type_info& typeId){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gEntryTypesHash->value(unique_id(typeId), EntryTypes::Unspecific);
}

const std::type_info* getInterfaceTypeForIID(const char*interface_iid)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gIIDTypeHash->value(interface_iid, nullptr);
}

bool isInterface(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gEntryTypesHash->value(unique_id(typeId), EntryTypes::Unspecific) == EntryTypes::InterfaceTypeInfo;
}

bool isQObject(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gEntryTypesHash->value(unique_id(typeId), EntryTypes::Unspecific) == EntryTypes::QObjectTypeInfo;
}

bool isFunctional(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gEntryTypesHash->value(unique_id(typeId), EntryTypes::Unspecific) == EntryTypes::FunctionalTypeInfo;
}

int registeredMetaTypeID(const std::type_info& typeId);

bool isValueType(JNIEnv * env, jclass valueType, int* metaTypeId)
{
    const SuperTypeInfos& infos = SuperTypeInfos::fromClass(env, valueType);
    if(metaTypeId)
        *metaTypeId = QMetaType::UnknownType;
    if (!infos.isEmpty()){
        if(const std::type_info* typeId = getTypeByJavaName(QtJambiAPI::getClassName(env, infos.first().javaClass()).replace(QLatin1Char('.'), QLatin1Char('/')))){
            QReadLocker locker(gLock());
            Q_UNUSED(locker)
            EntryTypes entryType = gEntryTypesHash->value(unique_id(*typeId), EntryTypes::Unspecific);
            int _metaTypeId = registeredMetaTypeID(*typeId);
            if(metaTypeId && (entryType==EntryTypes::ValueTypeInfo || entryType==EntryTypes::InterfaceValueTypeInfo))
                *metaTypeId = _metaTypeId;
            return (entryType==EntryTypes::ValueTypeInfo || entryType==EntryTypes::InterfaceValueTypeInfo)
                    && _metaTypeId!=QMetaType::UnknownType;
        }
    }
    return false;
}

const std::type_info* getTypeByJavaName(QByteArray javaName)
{
    const std::type_info* result = nullptr;
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    if(!gJavaNameTypeHash->contains(javaName.data())){
        if(javaName.endsWith("$ConcreteWrapper")){
            javaName = javaName.chopped(16);
        }
        if(javaName.endsWith("$Impl")){
            javaName = javaName.chopped(5);
        }
    }
    if(gJavaNameTypeHash->contains(javaName.data())){
        const QList<const std::type_info*>& list = (*gJavaNameTypeHash)[javaName];
        if(!list.isEmpty()){
            result = list.first();
        }
    }
    return result;
}

const std::type_info* getTypeByJavaName(const char * java_name)
{
    return getTypeByJavaName(QByteArray(java_name));
}

const std::type_info* getTypeByJavaName(const QString& java_name)
{
    return getTypeByJavaName(java_name.toLatin1());
}

const char* getJavaNameByFunctional(const char* qt_name)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gQtFunctionalJavaNameHash->value(qt_name, nullptr);
}

const std::type_info* getTypeByQtName(const QByteArray& qt_name)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gQtNameTypeHash->value(qt_name, nullptr);
}

const std::type_info* getTypeByQtName(const QString& qt_name)
{
    return getTypeByQtName(qt_name.toLatin1());
}

const std::type_info* getTypeByQtName(const char* qt_name)
{
    return getTypeByQtName(QByteArray(qt_name));
}

const QtJambiTypeInfo* getQTypeInfo(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gQTypeInfoHash->value(unique_id(typeId), QSharedPointer<QtJambiTypeInfo>(nullptr)).data();
}

const char * getQtName(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gTypeQtNameHash->value(unique_id(typeId), nullptr);
}

const char * getJavaName(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gTypeJavaNameHash->value(unique_id(typeId), nullptr);
}

const char * getJavaInterfaceName(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    if(gTypeJavaInterfaceHash->contains(unique_id(typeId)))
        return gTypeJavaInterfaceHash->value(unique_id(typeId), nullptr);
    return gTypeJavaNameHash->value(unique_id(typeId), nullptr);
}

void registerMetaTypeID(const std::type_info& typeId, const std::type_info& nonPointerTypeId, int qtMetaType)
{
    const char * registeredQtName;
    {
        QWriteLocker locker(gLock());
        Q_UNUSED(locker)
        if(!gMetaTypeIDMap->contains(unique_id(typeId)))
            gMetaTypeIDMap->insert(unique_id(typeId), qtMetaType);
        if(!gMetaTypeIDHash->contains(qtMetaType))
            gMetaTypeIDHash->insert(qtMetaType, &typeId);
        registeredQtName = gTypeQtNameHash->value(unique_id(typeId), nullptr);
    }
    QByteArray metaTypeName = QMetaType::typeName(qtMetaType);
    if(registeredQtName && metaTypeName!=registeredQtName){
        registerTypeAlias(typeId, metaTypeName, nullptr);
    }
    if(nonPointerTypeId!=typeid(JObjectWrapper)
            && nonPointerTypeId!=typeid(QString)
            && nonPointerTypeId!=typeid(QStringView)
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            && nonPointerTypeId!=typeid(QStringRef)
#endif
            && nonPointerTypeId!=typeid(qint8)
            && nonPointerTypeId!=typeid(qint16)
            && nonPointerTypeId!=typeid(qint32)
            && nonPointerTypeId!=typeid(qint64)
            && nonPointerTypeId!=typeid(quint8)
            && nonPointerTypeId!=typeid(quint16)
            && nonPointerTypeId!=typeid(quint32)
            && nonPointerTypeId!=typeid(quint64)
            && nonPointerTypeId!=typeid(bool)
            && nonPointerTypeId!=typeid(wchar_t)
            && nonPointerTypeId!=typeid(QVariant)
            && nonPointerTypeId!=typeid(JObjectArrayWrapper)
            && nonPointerTypeId!=typeid(JIntArrayWrapper)
            && nonPointerTypeId!=typeid(JLongArrayWrapper)
            && nonPointerTypeId!=typeid(JShortArrayWrapper)
            && nonPointerTypeId!=typeid(JByteArrayWrapper)
            && nonPointerTypeId!=typeid(JBooleanArrayWrapper)
            && nonPointerTypeId!=typeid(JCharArrayWrapper)
            && nonPointerTypeId!=typeid(JDoubleArrayWrapper)
            && nonPointerTypeId!=typeid(JFloatArrayWrapper)
            && nonPointerTypeId!=typeid(JEnumWrapper)
            && nonPointerTypeId!=typeid(JCollectionWrapper)
            && nonPointerTypeId!=typeid(JMapWrapper)
            && nonPointerTypeId!=typeid(JIteratorWrapper)
            && nonPointerTypeId!=typeid(std::nullptr_t)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        JObjectWrapperConverter::registerConverter(nonPointerTypeId, qtMetaType);
#else
        const std::type_info* _nonPointerTypeId = &nonPointerTypeId;
        QMetaType metaType(qtMetaType);
        QMetaType jObjectWrapperMetaType = QMetaType::fromType<JObjectWrapper>();
        if(!QMetaType::hasRegisteredConverterFunction(jObjectWrapperMetaType, metaType) && strcmp("QRemoteObjectPendingCall", metaType.name())!=0){
            QMetaType::registerConverterFunction([metaType,_nonPointerTypeId](const void *src, void *target) -> bool {
                const JObjectWrapper * wrapper = reinterpret_cast<const JObjectWrapper *>(src);
                if(wrapper->object()){
                    if(JniEnvironment env{200}){
                        void * ptr(nullptr);
                        if(QtJambiAPI::convertJavaToNative(env, *_nonPointerTypeId, wrapper->object(), &ptr)){
                            if((metaType.flags() & QMetaType::IsPointer)){
                                *reinterpret_cast<void**>(target) = ptr;
                            }else {
                                metaType.destruct(target);
                                metaType.construct(target, ptr);
                            }
                            return true;
                        }
                        return QtJambiAPI::convertJavaToNative(env, *_nonPointerTypeId, wrapper->object(), target);
                    }
                }
                return false;
            }, jObjectWrapperMetaType, metaType);
        }
        if(!QMetaType::hasRegisteredConverterFunction(metaType, jObjectWrapperMetaType)){
            QMetaType::registerConverterFunction([_nonPointerTypeId](const void *src, void *target) -> bool {
                if(!src){
                    new (target)JObjectWrapper();
                    return true;
                }else if(JniEnvironment env{200}){
                    try{
                        jobject output = QtJambiAPI::convertNativeToJavaObjectAsCopy(env, src, *_nonPointerTypeId);
                        new (target)JObjectWrapper(env, output);
                        return true;
                    }catch(const JavaException& exn){
                        exn.report(env);
                    }
                }
                return false;
            }, metaType, jObjectWrapperMetaType);
        }
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void registerMetaObjectByMetaTypeInterface(const QtPrivate::QMetaTypeInterface* iface, const QMetaObject* mo){
    gMetaTypeMetaObjectHash->insert(iface, mo);
}

const QMetaObject* metaobjectByMetaTypeInterface(const QtPrivate::QMetaTypeInterface* iface){
    return gMetaTypeMetaObjectHash->value(iface);
}
#endif

void RegistryAPI::registerMetaTypeID(const std::type_info& typeId, int qtMetaType)
{
    ::registerMetaTypeID(typeId, typeId, qtMetaType);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

const char* registerMetaTypeName(const QByteArray& typeName){
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    QSet<QByteArray>::iterator iter = gTypeNames->insert(typeName);
    return iter->data();
}

bool QmlAPI::registerMetaTypeConverter(JNIEnv *env, const QMetaType& metaType1, jclass jsvalueClass, const QMetaType& metaType2, jclass targetClass, jmethodID constructor){
    if(!QMetaType::hasRegisteredConverterFunction(metaType1, metaType2)){
        InternalToExternalConverter converter1 = QtJambiTypeManager::getInternalToExternalConverter(env, metaType1.name(), metaType1, jsvalueClass);
        ExternalToInternalConverter reconverter1 = QtJambiTypeManager::getExternalToInternalConverter(env, jsvalueClass, metaType1.name(), metaType1);
        InternalToExternalConverter converter2 = QtJambiTypeManager::getInternalToExternalConverter(env, metaType2.name(), metaType2, targetClass);
        ExternalToInternalConverter reconverter2 = QtJambiTypeManager::getExternalToInternalConverter(env, targetClass, metaType2.name(), metaType2);
        if(converter1 && reconverter1 && converter2 && reconverter2){
            ParameterTypeInfo parameter1{metaType1.id(), jsvalueClass, std::move(converter1), std::move(reconverter1)};
            ParameterTypeInfo parameter2{metaType2.id(), targetClass, std::move(converter2), std::move(reconverter2)};
            return QMetaType::registerConverterFunction([parameter1, parameter2, constructor](const void *src, void *target)->bool{
                if(JniEnvironment env{200}){
                    jvalue jv;
                    jv.l = nullptr;
                    if(parameter1.convertInternalToExternal(env, nullptr, src, &jv, true)){
                        jobject result{nullptr};
                        try{
                            result = env->NewObject(parameter2.javaClass(), constructor, jv.l);
                        }catch(const JavaException&){
                            return false;
                        }
                        if(!result && !(QMetaType(parameter2.metaType()).flags() & QMetaType::IsPointer))
                            return false;
                        jv.l = result;
                        return parameter2.convertExternalToInternal(env, nullptr, jv, target, jValueType::l);
                    }
                }
                return false;
            }, metaType1, metaType2);
        }
    }
    return false;
}

int QmlAPI::registerQmlMetaType(JNIEnv *env, jclass clazz,
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
                        QtPrivate::QMetaTypeInterface::MetaObjectFn metaObjectFn){
    QMetaType metaType = createMetaType(  typeName,
                                          true,
                                          defaultCtr,
                                          copyCtr,
                                          moveCtr,
                                          dtor,
                                          equals,
                                          lessThan,
                                          debugStream,
                                          dataStreamOutFn,
                                          dataStreamInFn,
                                          legacyRegisterOp,
                                          size,
                                          align,
                                          builtInTypeId,
                                          flags,
                                          metaObject,
                                          metaObjectFn);
    if(clazz)
        registerJavaClassForCustomMetaType(env, metaType, clazz);
    return metaType.id();
}

QMetaType createMetaType(QByteArrayView typeName,
                         bool copyName,
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
                         QtPrivate::QMetaTypeInterface::MetaObjectFn metaObjectFn){
    QtPrivate::QMetaTypeInterface* metaTypeInterface = new QtPrivate::QMetaTypeInterface{
            /*.revision=*/ 0,
            /*.alignment=*/ align,
            /*.size=*/ size,
            /*.flags=*/ uint(flags),
            /*.typeId=*/ builtInTypeId,
            /*.metaObject=*/ metaObjectFn ? metaObjectFn : (metaObject ? &metaobjectByMetaTypeInterface : nullptr),
            /*.name=*/ copyName ? registerMetaTypeName(typeName.toByteArray()) : typeName.data(),
            /*.defaultCtr=*/ defaultCtr,
            /*.copyCtr=*/ copyCtr,
            /*.moveCtr=*/ moveCtr,
            /*.dtor=*/ dtor,
            /*.equals=*/ equals,
            /*.lessThan=*/ lessThan,
            /*.debugStream=*/ debugStream,
            /*.dataStreamOutFn=*/ dataStreamOutFn,
            /*.dataStreamInFn=*/ dataStreamInFn,
            /*.legacyRegisterOp=*/ legacyRegisterOp
    };
    if(!metaObjectFn && metaObject){
        registerMetaObjectByMetaTypeInterface(metaTypeInterface, metaObject);
    }
    return QMetaType(metaTypeInterface);
}
#endif

int RegistryAPI::registerMetaType(const std::type_info& typeId,
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
                     AfterRegistrationFunction afterRegistrationFunction)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QByteArray normalizedTypeName(QMetaObject::normalizedType(typeName));
    int id = QMetaType::type(normalizedTypeName);
    if(id==QMetaType::UnknownType){
        id = QMetaType::registerNormalizedType(
                    normalizedTypeName,
                    destructor,
                    constructor,
                    int(size),
                    flags,
                    metaObject);
        if(afterRegistrationFunction)
            afterRegistrationFunction(id);
        ::registerMetaTypeID(typeId, nonPointerTypeId, id);
        registerAlignmentOfType(nonPointerTypeId, align);
    }else{
        bool needsRegistration;
        {
            QReadLocker locker(gLock());
            Q_UNUSED(locker)
            needsRegistration = !gMetaTypeIDMap->contains(unique_id(typeId));
        }
        if(needsRegistration){
            ::registerMetaTypeID(typeId, nonPointerTypeId, id);
            registerAlignmentOfType(nonPointerTypeId, align);
        }
    }
    return id;
#else
    QMetaType metaType = QMetaType::fromName(typeName);
    if(metaType.isValid()){
        bool needsRegistration;
        {
            QReadLocker locker(gLock());
            Q_UNUSED(locker)
            needsRegistration = !gMetaTypeIDMap->contains(unique_id(typeId));
        }
        if(needsRegistration)
            ::registerMetaTypeID(typeId, nonPointerTypeId, metaType.id());
    }else{
        metaType = createMetaType(typeName,
                                  false,
                                  defaultCtr,
                                  copyCtr,
                                  moveCtr,
                                  dtor,
                                  equals,
                                  lessThan,
                                  debugStream,
                                  dataStreamOutFn,
                                  dataStreamInFn,
                                  legacyRegisterOp,
                                  size,
                                  align,
                                  builtInTypeId,
                                  flags,
                                  metaObject, nullptr);
        if(afterRegistrationFunction)
            afterRegistrationFunction(metaType.id());
        ::registerMetaTypeID(typeId, nonPointerTypeId, metaType.id());
    }
    return metaType.id();
#endif
}

int registerMetaTypeImpl(const std::type_info* typeId,
                     const std::type_info* nonPointerTypeId,
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
                     AfterRegistrationFunction afterRegistrationFunction)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QByteArray normalizedTypeName(QMetaObject::normalizedType(typeName));
    int id = QMetaType::type(normalizedTypeName);
    if(id==QMetaType::UnknownType){
        id = QMetaType::registerNormalizedType(
                    normalizedTypeName,
                    destructor,
                    constructor,
                    int(size),
                    flags,
                    metaObject);
        if(afterRegistrationFunction)
            afterRegistrationFunction(id);
        if(typeId && nonPointerTypeId){
            ::registerMetaTypeID(*typeId, *nonPointerTypeId, id);
            RegistryAPI::registerAlignmentOfType(*nonPointerTypeId, align);
        }
    }else if(typeId && nonPointerTypeId){
        bool needsRegistration;
        {
            QReadLocker locker(gLock());
            Q_UNUSED(locker)
            needsRegistration = !gMetaTypeIDMap->contains(unique_id(*typeId));
        }
        if(needsRegistration){
            ::registerMetaTypeID(*typeId, *nonPointerTypeId, id);
            RegistryAPI::registerAlignmentOfType(*nonPointerTypeId, align);
        }
    }
    return id;
#else
    QMetaType metaType = QMetaType::fromName(typeName);
    if(metaType.isValid()){
        bool needsRegistration;
        {
            QReadLocker locker(gLock());
            Q_UNUSED(locker)
            needsRegistration = typeId && !gMetaTypeIDMap->contains(unique_id(*typeId));
        }
        if(needsRegistration && typeId && nonPointerTypeId)
            ::registerMetaTypeID(*typeId, *nonPointerTypeId, metaType.id());
    }else{
        metaType = createMetaType(typeName,
                                  true,
                                  defaultCtr,
                                  copyCtr,
                                  moveCtr,
                                  dtor,
                                  equals,
                                  lessThan,
                                  debugStream,
                                  dataStreamOutFn,
                                  dataStreamInFn,
                                  legacyRegisterOp,
                                  size,
                                  align,
                                  builtInTypeId,
                                  flags,
                                  metaObject, nullptr);
        if(afterRegistrationFunction)
            afterRegistrationFunction(metaType.id());
        if(typeId && nonPointerTypeId)
            ::registerMetaTypeID(*typeId, *nonPointerTypeId, metaType.id());
    }
    return metaType.id();
#endif
}

int RegistryAPI::registerMetaType(const std::type_info& typeId,
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
                     AfterRegistrationFunction afterRegistrationFunction)
{
    return registerMetaTypeImpl(&typeId,
                                &nonPointerTypeId,
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
                                legacyRegisterOp,
#endif
                                size,
                                align,
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                builtInTypeId,
#endif
                                flags,
                                metaObject,
                                afterRegistrationFunction);
}

int registerContainerMetaType(const QByteArray& typeName,
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
                     AfterRegistrationFunction afterRegistrationFunction)
{
    return registerMetaTypeImpl(nullptr,
                                nullptr,
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
                                legacyRegisterOp,
#endif
                                size,
                                align,
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                builtInTypeId,
#endif
                                flags,
                                metaObject,
                                afterRegistrationFunction);
}

int registeredMetaTypeID(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gMetaTypeIDMap->value(unique_id(typeId), QMetaType::UnknownType);
}

const std::type_info* getTypeByMetaType(const QMetaType& metaType)
{
    return getTypeByMetaType(metaType.id());
}

const std::type_info* getTypeByMetaType(int metaType)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    if(const std::type_info* info = gMetaTypeIDHash->value(metaType, nullptr)){
        return info;
    }
    return getTypeByQtName(QMetaType::typeName(metaType));
}

const std::type_info* getEnumForFlag(const std::type_info& flag)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gFlagEnumIDHash->value(unique_id(flag), nullptr);
}

const std::type_info* getFlagForEnum(const std::type_info& enumerator)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gEnumFlagIDHash->value(unique_id(enumerator), nullptr);
}

const void* QtJambiPrivate::getDefaultValue(const std::type_info& type_info, QtJambiPrivate::DefaultValueCreator creator)
{
    const void* result;
    {
        QReadLocker locker(gLock());
        Q_UNUSED(locker)
        result = gDefaultValueHash->value(unique_id(type_info), nullptr);
    }
    if(!result){
        result = creator();
        QWriteLocker wlocker(gLock());
        Q_UNUSED(wlocker)
        gDefaultValueHash->insert(unique_id(type_info), result);
    }
    return result;
}

void RegistryAPI::registerFunctionInfos(const std::type_info& typeId, std::initializer_list<FunctionInfo> virtualFunctions)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(!gVirtualFunctionInfos->contains(unique_id(typeId)))
        gVirtualFunctionInfos->insert(unique_id(typeId), virtualFunctions);
}

const QVector<const FunctionInfo>* virtualFunctions(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    if(gVirtualFunctionInfos->contains(unique_id(typeId))){
        return reinterpret_cast<const QVector<const FunctionInfo>*>(&(*gVirtualFunctionInfos())[unique_id(typeId)]);
    }else{
        return nullptr;
    }
}

void RegistryAPI::registerConstructorInfos(const std::type_info& typeId, Destructor destructor, std::initializer_list<ConstructorInfo> constructors){
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(!gConstructorInfos->contains(unique_id(typeId)))
        gConstructorInfos->insert(unique_id(typeId), constructors);
    if(!gDestructorHash->contains(unique_id(typeId)))
        gDestructorHash->insert(unique_id(typeId), destructor);
}

const QVector<const ConstructorInfo>* registeredConstructorInfos(const std::type_info& typeId){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    if(gConstructorInfos->contains(unique_id(typeId))){
        return reinterpret_cast<const QVector<const ConstructorInfo>*>(&(*gConstructorInfos())[unique_id(typeId)]);
    }else{
        return nullptr;
    }
}

void registerFlagToEnum(const char *flag_name, const char *enum_name)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gFlagEnumNameHash->insert(qHash(flag_name), enum_name);
}

const char* getEnumName(const char*flag_name)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gFlagEnumNameHash->value(qHash(flag_name), nullptr);
}

void registerInterface(const char *qt_interface, const char *java_interface, const char *interface_iid)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gInterfaceHash->insert(qHash(qt_interface), java_interface);
    if(interface_iid)
        gInterfaceIIDsHash->insert(qHash(interface_iid), qt_interface);
}

const char* getInterfaceForIID(const char*interface_iid)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gInterfaceIIDsHash->value(qHash(interface_iid), nullptr);
}

bool isInterface(const char*qt_interface)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gInterfaceHash->contains(qHash(qt_interface));
}

const char* getInterface(const char*qt_interface)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gInterfaceHash->value(qHash(qt_interface), nullptr);
}

void registerFunctional(const char *qt_interface)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gFunctionalHash->insert(qHash(qt_interface));
}

bool isFunctional(const char*qt_interface)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gFunctionalHash->contains(qHash(qt_interface));
}

void RegistryAPI::registerDeleter(const std::type_info& typeId, PtrDeleterFunction deleter)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gDeleterHash->insert(unique_id(typeId), deleter);
}

PtrDeleterFunction deleter(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gDeleterHash->value(unique_id(typeId), nullptr);
}

void RegistryAPI::registerOwnerFunction(const std::type_info& typeId, PtrOwnerFunction ownerFunction)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gOwnerFunctionHash->insert(unique_id(typeId), ownerFunction);
}

PtrOwnerFunction registeredOwnerFunction(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gOwnerFunctionHash->value(unique_id(typeId), nullptr);
}

void clear_type_entry(const std::type_info& typeId);

void RegistryAPI::registerPolymorphyHandler(const std::type_info& polymorphicBaseTypeId, const std::type_info& targetTypeId, PolymorphyHandler handler)
{
    if(handler){
        {
            QWriteLocker locker(gLock());
            Q_UNUSED(locker)
            g_polymorphic_ids->insert(unique_id(polymorphicBaseTypeId), new PolymorphicIdHandler(targetTypeId, handler));
            gPolymorphicBasesHash->insert(unique_id(targetTypeId), &polymorphicBaseTypeId);
        }
        clear_type_entry(polymorphicBaseTypeId);
    }
}

bool isPolymorphicBase(const std::type_info& typeId){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return g_polymorphic_ids->contains(unique_id(typeId));
}

QList<const std::type_info*> getPolymorphicBases(const std::type_info& typeId){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gPolymorphicBasesHash->values(unique_id(typeId));
}

QList<const PolymorphicIdHandler*> getPolymorphicIdHandlers(const std::type_info& polymorphicBaseTypeId){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return g_polymorphic_ids->values(unique_id(polymorphicBaseTypeId));
}

void RegistryAPI::registerSizeOfType(const std::type_info& typeId, size_t size)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(gValueSizeHash->contains(unique_id(typeId)))
        return;
    gValueSizeHash->insert(unique_id(typeId), size);
}

void RegistryAPI::registerAlignmentOfType(const std::type_info& typeId, size_t alignment)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(gValueAlignmentHash->contains(unique_id(typeId)))
        return;
    gValueAlignmentHash->insert(unique_id(typeId), alignment);
}

void RegistryAPI::registerSizeOfShell(const std::type_info& typeId, size_t size)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(gShellSizeHash->contains(unique_id(typeId)))
        return;
    gShellSizeHash->insert(unique_id(typeId), size);
}

QMap<QString,QPair<size_t,size_t>> getRegisteredTypeSizesAndAlignments(){
    QSet<int> types;
    for(int i=QMetaType::FirstCoreType; i<=QMetaType::LastCoreType; ++i){
        if(QMetaType::isRegistered(i))
            types << i;
    }
    for(int i=QMetaType::FirstGuiType; i<=QMetaType::LastGuiType; ++i){
        if(QMetaType::isRegistered(i))
            types << i;
    }
    for(int i=QMetaType::FirstWidgetsType; i<=QMetaType::LastWidgetsType; ++i){
        if(QMetaType::isRegistered(i))
            types << i;
    }
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    for(int mt : gMetaTypeIDMap->values())
        types << mt;
    QMap<QString,QPair<size_t,size_t>> result;
    for(int type : qAsConst(types)){
        if(type!=QMetaType::Nullptr
                && type!=QMetaType::UnknownType
                && type!=QMetaType::Void){
            QMetaType metaType(type);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            size_t alignment = 0;
            if(const std::type_info* typeId = gMetaTypeIDHash->value(type)){
                alignment = gValueAlignmentHash->value(unique_id(*typeId));
            }else if(AbstractContainerAccess::isPointerType(metaType))
                alignment = alignof(void*);
            if(alignment==0)
                continue;
#endif
            result[QLatin1String(metaType.name())] = {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    metaType.sizeOf(),
                    metaType.alignOf()
#else
                    size_t(metaType.sizeOf()),
                    alignment
#endif
        };
        }
    }
    return result;
}

void RegistryAPI::registerHashFunction(const std::type_info& typeId, QHashFunctionPtr hashFunction){
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(gHashFunctionHash->contains(unique_id(typeId)))
        return;
    gHashFunctionHash->insert(unique_id(typeId), hashFunction);
}

QHashFunctionPtr registeredHashFunction(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gHashFunctionHash->value(unique_id(typeId), nullptr);
}

size_t getValueSize(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gValueSizeHash->value(unique_id(typeId), 0);
}

size_t getValueAlignment(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gValueAlignmentHash->value(unique_id(typeId), 0);
}

size_t getShellSize(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gShellSizeHash->value(unique_id(typeId), 0);
}

void RegistryAPI::registerInterfaceOffset(const std::type_info& qt_base, const std::type_info& qt_interface, int offset)
{
    if(offset!=0){
        QWriteLocker locker(gLock());
        Q_UNUSED(locker)
        ( *gOffsetHash )[unique_id(qt_base)].insert(unique_id(qt_interface), offset);
        ( *gOffsetHash )[unique_id(qt_interface)].insert(unique_id(qt_base), -offset);
        ( *gInterfacesHash )[unique_id(qt_base)] << &qt_interface;
    }
}

int registeredInterfaceOffset(const std::type_info& qt_base, const std::type_info& qt_interface){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    if(gOffsetHash->contains(unique_id(qt_base))){
        return ( *gOffsetHash )[unique_id(qt_base)].value(unique_id(qt_interface), -1);
    }
    return -1;
}

void registeredInterfaceOffsets(const std::type_info& qt_type, InterfaceOffsetInfo* info){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    if(gOffsetHash->contains(unique_id(qt_type))){
        const QMap<size_t, int>& values = ( *gOffsetHash )[unique_id(qt_type)];
        for(QMap<size_t, int>::const_key_value_iterator i = values.keyValueBegin(); i!=values.keyValueEnd(); ++i){
            info->interfaces.insert(i.base().key());
            if(i.base().value()>0)
                info->offsets.insert(i.base().key(), uint(i.base().value()));
        }
    }
    if(gInterfacesHash->contains(unique_id(qt_type))){
        info->inheritedInterfaces[unique_id(qt_type)] = ( *gInterfacesHash )[unique_id(qt_type)];
    }
}

const InterfaceOffsetInfo* getInterfaceOffsets(JNIEnv *env, jclass clazz){
    if(clazz){
        uint classHash = uint(Java::Runtime::Object::hashCode(env,clazz));
        {
            QReadLocker locker(gLock());
            Q_UNUSED(locker)
            if(gInterfaceOffsetHash->contains(classHash))
                return &( *gInterfaceOffsetHash )[classHash];
        }
        {
            InterfaceOffsetInfo result;
            jclass cls = clazz;
            if(!Java::QtJambi::ClassAnalyzerUtility::isGeneratedClass(env, cls)){
                const SuperTypeInfos& infos = SuperTypeInfos::fromClass(env, clazz);
                for(const SuperTypeInfo& info : infos){
                    if(info.offset()>0){
                        result.offsets[unique_id(info.typeId())] = uint(info.offset());
                    }
                }
                cls = JavaAPI::resolveClosestQtSuperclass(env, cls);
            }

            while(cls && Java::QtJambi::ClassAnalyzerUtility::isGeneratedClass(env, cls)){
                QString className = QtJambiAPI::getClassName(env, cls).replace(".", "/");
                const std::type_info* qt_type = getTypeByJavaName(qPrintable(className));
                Q_ASSERT(qt_type);
                registeredInterfaceOffsets(*qt_type, &result);
                cls = env->GetSuperclass(cls);
            }
            QWriteLocker wlocker(gLock());
            Q_UNUSED(wlocker)
            if(gInterfaceOffsetHash->contains(classHash))
                return &( *gInterfaceOffsetHash )[classHash];
            gInterfaceOffsetHash->insert(classHash, result);
            return &( *gInterfaceOffsetHash )[classHash];
        }
    }
    return nullptr;
}

const InterfaceOffsetInfo* getInterfaceOffsets(JNIEnv *env, jclass clazz, const std::type_info& typeId, const SuperTypeInfos* superTypeInfos){
    if(clazz){
        uint classHash = uint(Java::Runtime::Object::hashCode(env,clazz));
        {
            QReadLocker locker(gLock());
            Q_UNUSED(locker)
            if(gInterfaceOffsetHash->contains(classHash))
                return &( *gInterfaceOffsetHash )[classHash];
        }
        {
            InterfaceOffsetInfo result;
            if(superTypeInfos){
                const SuperTypeInfos& infos = *superTypeInfos;
                for(const SuperTypeInfo& info : infos){
                    if(info.offset()>0){
                        result.offsets[unique_id(info.typeId())] = uint(info.offset());
                    }
                }
            }
            registeredInterfaceOffsets(typeId, &result);
            QWriteLocker wlocker(gLock());
            Q_UNUSED(wlocker)
            if(gInterfaceOffsetHash->contains(classHash))
                return &( *gInterfaceOffsetHash )[classHash];
            ( *gInterfaceOffsetHash )[classHash] = std::move(result);
            return &( *gInterfaceOffsetHash )[classHash];
        }
    }
    return nullptr;
}

void registerDestructor(const std::type_info& typeId, Destructor destructor)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gDestructorHash->insert(unique_id(typeId), destructor);
}

Destructor registeredDestructor(const std::type_info& typeId){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gDestructorHash->value(unique_id(typeId), nullptr);
}

hash_type qHash(const QMetaObject* mo, hash_type seed = 0){
    QtPrivate::QHashCombine hash;
    seed = hash(seed, qintptr(mo));
    if(mo){
        seed = hash(seed, mo->className());
    }
    return seed;
}

void RegistryAPI::registerCustomMetaObject(const std::type_info& typeId, const QMetaObject& superTypeMetaObject)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gHasCustomMetaObjectHash->insert(unique_id(typeId), &superTypeMetaObject);
}

bool hasCustomMetaObject(const std::type_info& typeId, const QMetaObject** superTypeMetaObject){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    if(gHasCustomMetaObjectHash->contains(unique_id(typeId))){
        if(superTypeMetaObject)
            *superTypeMetaObject = gHasCustomMetaObjectHash->value(unique_id(typeId), nullptr);
        return true;
    }
    return false;
}

const QMetaObject* superTypeForCustomMetaObject(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gHasCustomMetaObjectHash->value(unique_id(typeId), nullptr);
}

void RegistryAPI::registerMetaObject(const std::type_info& typeId, const QMetaObject& originalMetaObject, bool isValueOwner,
                        QMetaMethodRenamer methodRenamer)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gOriginalMetaObjectHash->insert(unique_id(typeId), &originalMetaObject);
    gTypeIdsByMetaObject->insert(&originalMetaObject, &typeId);
    if(methodRenamer)
        gQMetaMethodRenamerByMetaObject->insert(&originalMetaObject, methodRenamer);
    if(isValueOwner)
        gValueOwners->insert(&originalMetaObject);
}

void RegistryAPI::registerMetaObject(const std::type_info& typeId, const QMetaObject& metaObject, bool isValueOwner,
                        std::initializer_list<SignalMetaInfo> signalMetaInfos,
                        ParameterInfoProvider parameterTypeInfoProvider,
                        QMetaMethodRenamer methodRenamer)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gOriginalMetaObjectHash->insert(unique_id(typeId), &metaObject);
    gTypeIdsByMetaObject->insert(&metaObject, &typeId);
    if(!gSignalMetaInfos->contains(&metaObject))
        gSignalMetaInfos->insert(&metaObject, signalMetaInfos);
    if(parameterTypeInfoProvider && !gParameterTypeInfoProviders->contains(&metaObject))
        gParameterTypeInfoProviders->insert(&metaObject, parameterTypeInfoProvider);
    if(methodRenamer)
        gQMetaMethodRenamerByMetaObject->insert(&metaObject, methodRenamer);
    if(isValueOwner)
        gValueOwners->insert(&metaObject);
}

bool isValueOwner(const QMetaObject* metaObject){
    {
        QReadLocker locker(gLock());
        Q_UNUSED(locker)
        if(gValueOwnerHash->contains(metaObject)){
            return gValueOwnerHash->value(metaObject);
        }
    }
    bool result = false;
    {
        QReadLocker locker(gLock());
        Q_UNUSED(locker)
        for(const QMetaObject* mo : *gValueOwners){
            if(metaObject->inherits(mo)){
                result = true;
                break;
            }
        }
    }
    {
        QWriteLocker locker(gLock());
        Q_UNUSED(locker)
        gValueOwnerHash->insert(metaObject, result);
    }
    return result;
}

const std::type_info* getTypeByMetaObject(const QMetaObject* metaObject)
{
    return gTypeIdsByMetaObject->value(metaObject, nullptr);
}



const QMetaObject* registeredOriginalMetaObject(const std::type_info& typeId){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gOriginalMetaObjectHash->value(unique_id(typeId), nullptr);
}

const QVector<const SignalMetaInfo>* signalMetaInfos(const QMetaObject* metaObject)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    if(gSignalMetaInfos->contains(metaObject)){
        return reinterpret_cast<const QVector<const SignalMetaInfo>*>(&(*gSignalMetaInfos())[metaObject]);
    }else{
        return nullptr;
    }
}

QList<QMetaMethod> getExtraSignalsOfMetaObject(const QMetaObject* metaObject){
    {
        QReadLocker lock(gLock());
        if(gExtraSignalsOfMetaObject->contains(metaObject))
            return gExtraSignalsOfMetaObject->value(metaObject);
    }
    QList<QMetaMethod> extraSignals;
    QSet<int> availableSignalInfos;
    {
        const QMetaObject* currentMetaObject = metaObject;
        do{
            if(const QVector<const SignalMetaInfo>* infos = signalMetaInfos(currentMetaObject)){
                for(const SignalMetaInfo& info : *infos){
                    availableSignalInfos.insert(info.signal_method_index_provider());
                }
            }
            currentMetaObject = currentMetaObject->superClass();
        }while(currentMetaObject);
    }
    const int methodCount = metaObject->methodCount();
    for(int i=0; i<methodCount; ++i){
        QMetaMethod method = metaObject->method(i);
        if(method.isValid()
                && method.methodType()==QMetaMethod::Signal
                && !availableSignalInfos.contains(method.methodIndex())
                && !QtJambiMetaObject::isInstance(method.enclosingMetaObject())){
            extraSignals << method;
        }
    }
    {
        QWriteLocker lock(gLock());
        if(gExtraSignalsOfMetaObject->contains(metaObject))
            return gExtraSignalsOfMetaObject->value(metaObject);
        gExtraSignalsOfMetaObject->insert(metaObject, extraSignals);
    }
    return extraSignals;
}

ParameterInfoProvider registeredParameterInfoProvider(const QMetaObject* metaObject)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    if(gSignalMetaInfos->contains(metaObject)){
        return (*gParameterTypeInfoProviders())[metaObject];
    }else{
        return nullptr;
    }
}

const QHash<int,const char*>* CoreAPI::renamedMethods(const QMetaObject* metaObject){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    if(gQMetaMethodRenamerByMetaObject->contains(metaObject)){
        if(!gRenamedMethodsHash->contains(metaObject)){
            QMetaMethodRenamer registeredMetaMethodRenamer = (*gQMetaMethodRenamerByMetaObject())[metaObject];
            QHash<int,const char*> renamedMethods;
            for(int i=metaObject->methodOffset(); i<metaObject->methodCount(); ++i){
                if(const char* newName = registeredMetaMethodRenamer(i)){
                    renamedMethods[i] = newName;
                }
            }
            gRenamedMethodsHash->insert(metaObject, renamedMethods);
        }
        return & (*gRenamedMethodsHash)[metaObject];
    }
    return nullptr;
}

QMetaMethodRenamer registeredMetaMethodRenamer(const QMetaObject* metaObject)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    if(gQMetaMethodRenamerByMetaObject->contains(metaObject)){
        return (*gQMetaMethodRenamerByMetaObject())[metaObject];
    }else{
        return nullptr;
    }
}

void RegistryAPI::registerFunctionalResolver(const std::type_info& typeId, FunctionalResolver resolver)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gFunctionalResolverHash->insert(unique_id(typeId), resolver);
}

FunctionalResolver registeredFunctionalResolver(const std::type_info& typeId){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gFunctionalResolverHash->value(unique_id(typeId), nullptr);
}

static QmlAPI::QmlReportDestruction qmlReportDestructionFunction = nullptr;

void QtJambiPrivate::reportDeclarativeObjectDestruction(QObject * obj){
    if(qmlReportDestructionFunction)
        qmlReportDestructionFunction(obj);
}

void QmlAPI::setQmlReportDestruction(QmlReportDestruction fct){
    if(!qmlReportDestructionFunction)
        qmlReportDestructionFunction = fct;
}

void RegistryAPI::registerMediaControlInfo(const std::type_info& typeId, const char *media_control_iid){
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gMediaControlIIDHash->insert(unique_id(typeId), media_control_iid);
    gMediaControlIIDClassHash->insert(media_control_iid, &typeId);
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void QmlAPI::registerJavaClassForCustomMetaType(JNIEnv *env, int metaType, jclass javaClass){
    return ::registerJavaClassForCustomMetaType(env, metaType, javaClass, false);
}

void registerJavaClassForCustomMetaType(JNIEnv *env, int metaType, jclass javaClass, bool isJObjectWrapped){
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    QByteArray javaName = QtJambiAPI::getClassName(env, javaClass).toLatin1().replace('.', '/');
    gMetaTypeJavaTypeHash->insert(metaType, getGlobalClassRef(env, javaClass, javaName));
    gJavaTypeMetaTypesHash->insert(javaName, metaType);
    if(isJObjectWrapped)
        gJObjectWrappedMetaTypes->insert(metaType);
}

void registerJavaClassForCustomMetaType(JNIEnv *env, const QMetaType& metaType, jclass javaClass, bool isJObjectWrapped){
    registerJavaClassForCustomMetaType(env, metaType.id(), javaClass, isJObjectWrapped);
}

bool isJObjectWrappedMetaType(const QMetaType& metaType){
    QReadLocker locker(gLock());
    return gJObjectWrappedMetaTypes->contains(metaType.id());
}

jclass registeredJavaClassForCustomMetaType(int metaType){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gMetaTypeJavaTypeHash->value(metaType);
}

jclass registeredJavaClassForCustomMetaType(const QMetaType& metaType){
    return registeredJavaClassForCustomMetaType(metaType.id());
}

QList<int> registeredCustomMetaTypesForJavaClass(const QByteArray& javaClass){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gJavaTypeMetaTypesHash->values(javaClass);
}
#else
void registerJavaClassForCustomMetaType(JNIEnv *env, QMetaType metaType, jclass javaClass, bool isJObjectWrapped){
    const QtPrivate::QMetaTypeInterface * iface = metaType.iface();
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    QByteArray javaName = QtJambiAPI::getClassName(env, javaClass).toLatin1().replace('.', '/');
    gMetaTypeJavaTypeHash->insert(iface, getGlobalClassRef(env, javaClass, javaName));
    gJavaTypeMetaTypesHash->insert(javaName, metaType);
    if(isJObjectWrapped)
        gJObjectWrappedMetaTypes->insert(iface);
}

bool isJObjectWrappedMetaType(QMetaType metaType){
    QReadLocker locker(gLock());
    return gJObjectWrappedMetaTypes->contains(metaType.iface());
}

jclass registeredJavaClassForCustomMetaType(const QtPrivate::QMetaTypeInterface * metaType){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gMetaTypeJavaTypeHash->value(metaType);
}

jclass registeredJavaClassForCustomMetaType(QMetaType metaType){
    return registeredJavaClassForCustomMetaType(metaType.iface());
}

const QMetaObject *findWrappersMetaObject(const QtPrivate::QMetaTypeInterface *iface){
    if(JniEnvironment env{200}){
        try {
            jclass clazz = nullptr;
            {
                QReadLocker locker(gLock());
                clazz = (*gMetaTypeJavaTypeHash)[iface];
            }
            if(clazz){
                if(const QMetaObject* meta_object = CoreAPI::metaObjectForClass(env, clazz)){
                    registerMetaObjectByMetaTypeInterface(iface, meta_object);
                    const_cast<QtPrivate::QMetaTypeInterface*>(iface)->metaObjectFn = &metaobjectByMetaTypeInterface;
                }else{
                    const_cast<QtPrivate::QMetaTypeInterface*>(iface)->flags &= ~(QMetaType::IsGadget | QMetaType::PointerToGadget);
                    const_cast<QtPrivate::QMetaTypeInterface*>(iface)->metaObjectFn = [](const QtPrivate::QMetaTypeInterface*)->const QMetaObject*{return nullptr;};
                }
            }
        }catch(const JavaException&){
        }
    }
    return nullptr;
}

QList<QMetaType> registeredCustomMetaTypesForJavaClass(const QByteArray& javaClass){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gJavaTypeMetaTypesHash->values(javaClass);
}
#endif

QList<const char*> CoreAPI::getInterfaceIIDs(JNIEnv *env, jclass javaType){
    QList<const char*> iids;
    QString className;
    if(javaType){
        jobject interfaceList = Java::QtJambi::ClassAnalyzerUtility::getAllImplementedInterfaces(env, javaType);
        if(interfaceList){
            int count = QtJambiAPI::sizeOfJavaCollection(env, interfaceList);
            if(count>0){
                jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, interfaceList);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jclass interfaceClass = jclass(QtJambiAPI::nextOfJavaIterator(env, iterator));
                    className = QtJambiAPI::getClassName(env, interfaceClass).replace('.', '/');
                    QReadLocker locker(gLock());
                    Q_UNUSED(locker)
                    if(const char* iid = gTypeIIDHash->value(className.toLatin1(), nullptr)){
                        iids << iid;
                    }
                }
            }
        }
        while(javaType){
            className = QtJambiAPI::getClassName(env, javaType).replace('.', '/');
            QReadLocker locker(gLock());
            Q_UNUSED(locker)
            if(const char* iid = gTypeIIDHash->value(className.toLatin1(), nullptr)){
                iids << iid;
            }
            javaType = env->GetSuperclass(javaType);
        }
    }
    return iids;
}

const char* CoreAPI::getInterfaceIID(JNIEnv *env, jclass javaType){
    QList<const char*> iids = getInterfaceIIDs(env, javaType);
    if(!iids.isEmpty())
        return iids.first();
    return nullptr;
}

jclass CoreAPI::getInterfaceByIID(JNIEnv *env, const char* iid){
    if(env && iid){
        const std::type_info* typeId(nullptr);
        {
            QReadLocker locker(gLock());
            Q_UNUSED(locker)
            typeId = gIIDTypeHash->value(iid, nullptr);
        }
        if(typeId){
            if(const char* javaName = getJavaName(*typeId)){
                return JavaAPI::resolveClass(env, javaName);
            }
        }else{
            const char* javaName(nullptr);
            {
                QReadLocker locker(gLock());
                Q_UNUSED(locker)
                javaName = (*gIIDByteArrayHash)[iid];
            }
            if(javaName){
                return JavaAPI::resolveClass(env, javaName);
            }
        }
    }
    return nullptr;
}

const char* RegistryAPI::mediaControlIID(JNIEnv *env, jclass javaType){
    if(javaType){
        QString className = QtJambiAPI::getClassName(env, javaType).replace(".", "/");
        const std::type_info* typeId = getTypeByJavaName(className);
        if(typeId){
            QReadLocker locker(gLock());
            Q_UNUSED(locker)
            return gMediaControlIIDHash->value(unique_id(*typeId), nullptr);
        }
    }
    return nullptr;
}

jclass RegistryAPI::classByMediaControlIID(JNIEnv *env, const char* iid){
    if(env && iid){
        const std::type_info* typeId(nullptr);
        {
            QReadLocker locker(gLock());
            Q_UNUSED(locker)
            typeId = gMediaControlIIDClassHash->value(iid, nullptr);
        }
        if(typeId){
            if(const char* javaName = getJavaName(*typeId)){
                return JavaAPI::resolveClass(env, javaName);
            }
        }
    }
    return nullptr;
}

/*******************************************************************************
 * Class Cache
 */
jclass findClass(JNIEnv *env, const char *qualifiedName, jobject classLoader = nullptr)
{
    if (env->ExceptionCheck()) {
        // It is not allowed to call env->FindClass() with an exception pending, this is illegal JNI
        //  usage and must be the result of a software design error.
        qWarning("findClass(\"%s\") with Exception pending", qualifiedName);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        Q_ASSERT(false);
        return nullptr;
    }

    jclass returned = nullptr;
    if(strcmp("byte", qualifiedName)==0){
        returned = Java::Runtime::Byte::primitiveType(env);
    }else if(strcmp("long", qualifiedName)==0){
        returned = Java::Runtime::Long::primitiveType(env);
    }else if(strcmp("int", qualifiedName)==0){
        returned = Java::Runtime::Integer::primitiveType(env);
    }else if(strcmp("short", qualifiedName)==0){
        returned = Java::Runtime::Short::primitiveType(env);
    }else if(strcmp("float", qualifiedName)==0){
        returned = Java::Runtime::Float::primitiveType(env);
    }else if(strcmp("double", qualifiedName)==0){
        returned = Java::Runtime::Double::primitiveType(env);
    }else if(strcmp("boolean", qualifiedName)==0){
        returned = Java::Runtime::Boolean::primitiveType(env);
    }else if(strcmp("char", qualifiedName)==0){
        returned = Java::Runtime::Character::primitiveType(env);
    }else if(strcmp("void", qualifiedName)==0){
        returned = Java::Runtime::Void::primitiveType(env);
    }else{
        // This should do the trick when running outside Eclipse,
        // or in the context of a previously loaded class
        returned = env->FindClass(qualifiedName);
    }

    if (returned == nullptr) {
        jthrowable exception = env->ExceptionOccurred();
        env->ExceptionClear();

        QString qtClassName = QString::fromLatin1(qualifiedName).replace('/', '.');
        jstring className = qtjambi_cast<jstring>(env, qtClassName);
        Q_ASSERT(className);
        if(classLoader){
            // Look up the class in our custom class loader
            returned = Java::Runtime::ClassLoader::tryLoadClass(env, classLoader, className);
            if (env->ExceptionCheck() || returned == nullptr) {
                env->ExceptionClear();
                classLoader = nullptr;
            }
        }
        if(!returned){
            jobject currentThread = Java::Runtime::Thread::currentThread(env);
            Q_ASSERT(currentThread);

            if(jobject contextClassLoader = Java::Runtime::Thread::getContextClassLoader(env, currentThread)){
                returned = Java::Runtime::ClassLoader::tryLoadClass(env, contextClassLoader, className);
                if (env->ExceptionCheck() || returned == nullptr) {
                    env->ExceptionClear();
                    classLoader = nullptr;
                }
            }
        }
#ifdef Q_OS_ANDROID
        if(!returned && !qtClassName.endsWith("android/QtNative")){
            jclass cls = Java::Android::QtNative::getClass(env);
            classLoader = Java::Runtime::Class::getClassLoader(env, cls);
            if(classLoader)
                returned = Java::Runtime::ClassLoader::tryLoadClass(env, classLoader, className);
            if (env->ExceptionCheck()) {
                returned = nullptr;
                if(!exception)
                    exception = env->ExceptionOccurred();
                env->ExceptionClear();
            }
        }
#endif

        if(!returned && exception){
#ifdef QTJAMBI_STACKTRACE
            if(qtClassName=="io.qt.internal.ExceptionUtility"){
                JavaException(env, exception).raise();
            }else{
                JavaException(env, exception).raise( QTJAMBI_STACKTRACEINFO_ENV(env) );
            }
#else
            JavaException(env, exception).raise();
#endif
        }
    }

    return returned;
}

jclass getGlobalClassRef(JNIEnv *env, jclass cls, const char *className){
#ifndef QTJAMBI_NOCACHE
    hash_type key;
#ifdef QTJAMBI_LOG_CLASSNAMES
    QString classNameStrg;
#endif
    if(className){
#ifdef QTJAMBI_LOG_CLASSNAMES
        classNameStrg = QLatin1String(className);
#endif
        key = qHash(className);
    }else{
#ifndef QTJAMBI_LOG_CLASSNAMES
        QString
#endif
                classNameStrg = QtJambiAPI::getClassName(env, cls).replace(".", "/");
        key = qHash(qPrintable(classNameStrg));
    }
    {
        QReadLocker locker(gClassLock());
        Q_UNUSED(locker)
        if(gClassHash->contains(key))
            return gClassHash->value(key);
    }
    QWriteLocker locker(gClassLock());
    Q_UNUSED(locker)
    if(gClassHash->contains(key)) // if it has been inserted meanwhile by other thread
        return gClassHash->value(key);
#ifdef QTJAMBI_LOG_CLASSNAMES
    gClassNameHash->insert(key, classNameStrg);
#endif
    if(env->GetObjectRefType(cls)!=JNIGlobalRefType){
        cls = jclass(env->NewGlobalRef(cls));
    }
    gClassHash->insert(key, cls);
    return cls;
#else
    Q_UNUSED(env)
    Q_UNUSED(className)
    return cls;
#endif
}

jclass JavaAPI::toGlobalReference(JNIEnv * env, jclass clazz){
    return getGlobalClassRef(env, clazz, nullptr);
}

void registerLambdaClass(JNIEnv *env, jclass lambdaClass, const char *className){
    (void)getGlobalClassRef(env, lambdaClass, className);
}


jclass getArrayClass(JNIEnv *env, jclass cls, int arrayDepth){
    if(arrayDepth==0)
        return cls;
    QString className;
    if(Java::Runtime::Integer::isPrimitiveType(env, cls)){
        className = "[I";
    }else if(Java::Runtime::Long::isPrimitiveType(env, cls)){
        className = "[L";
    }else if(Java::Runtime::Short::isPrimitiveType(env, cls)){
        className = "[S";
    }else if(Java::Runtime::Byte::isPrimitiveType(env, cls)){
        className = "[B";
    }else if(Java::Runtime::Double::isPrimitiveType(env, cls)){
        className = "[D";
    }else if(Java::Runtime::Float::isPrimitiveType(env, cls)){
        className = "[F";
    }else if(Java::Runtime::Boolean::isPrimitiveType(env, cls)){
        className = "[Z";
    }else if(Java::Runtime::Character::isPrimitiveType(env, cls)){
        className = "[C";
    }else{
        className = "[L"+QtJambiAPI::getClassName(env, cls).replace(".", "/")+";";
    }
    hash_type key = qHash(qPrintable( className ));
    jclass cachedClass(nullptr);
    {
        QReadLocker locker(gClassLock());
        Q_UNUSED(locker)
        if(gClassHash->contains(key)){
            cachedClass = gClassHash->value(key);
        }
    }
    if(!cachedClass){
        if(Java::Runtime::Integer::isPrimitiveType(env, cls)){
            cls = env->GetObjectClass(env->NewIntArray(0));
        }else if(Java::Runtime::Long::isPrimitiveType(env, cls)){
            cls = env->GetObjectClass(env->NewLongArray(0));
        }else if(Java::Runtime::Short::isPrimitiveType(env, cls)){
            cls = env->GetObjectClass(env->NewShortArray(0));
        }else if(Java::Runtime::Byte::isPrimitiveType(env, cls)){
            cls = env->GetObjectClass(env->NewByteArray(0));
        }else if(Java::Runtime::Double::isPrimitiveType(env, cls)){
            cls = env->GetObjectClass(env->NewDoubleArray(0));
        }else if(Java::Runtime::Float::isPrimitiveType(env, cls)){
            cls = env->GetObjectClass(env->NewFloatArray(0));
        }else if(Java::Runtime::Boolean::isPrimitiveType(env, cls)){
            cls = env->GetObjectClass(env->NewBooleanArray(0));
        }else if(Java::Runtime::Character::isPrimitiveType(env, cls)){
            cls = env->GetObjectClass(env->NewCharArray(0));
        }else{
            cls = env->GetObjectClass(env->NewObjectArray(0, cls, nullptr));
        }
        cls = getGlobalClassRef(env, cls, qPrintable( className ));
        return getArrayClass(env, cls, arrayDepth-1);
    }else {
        return getArrayClass(env, cachedClass, arrayDepth-1);
    }
}

jclass JavaAPI::resolveClass(JNIEnv *env, const char *className, jobject classLoader)
{
    jclass returned = nullptr;
#ifndef QTJAMBI_NOCACHE
    hash_type key = qHash(className);

    {
        QReadLocker locker(gClassLock());
        Q_UNUSED(locker)
        returned = gClassHash->value(key, nullptr);
    }

    if (returned == nullptr) {
#endif // QTJAMBI_NOCACHE
        if(className && !QString(QLatin1String(className)).contains("$Lambda$") && !QLatin1String(className).isEmpty()){
            returned = findClass(env, className, classLoader);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            returned = getGlobalClassRef(env, returned, className);
#ifndef QTJAMBI_NOCACHE
        }
#endif // QTJAMBI_NOCACHE
    }

    return returned;
}

hash_type computeHash(JNIEnv* env, jclass clazz, const char *memberName, const char *signature, bool isStatic) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if(signature)
        return qHashMulti(0, clazz ? Java::Runtime::Object::hashCode(env, clazz) : 0, memberName, signature, isStatic);
    else
        return qHashMulti(0, clazz ? Java::Runtime::Object::hashCode(env, clazz) : 0, memberName, isStatic);
#else
    QList<hash_type> range;
    range << qHash(clazz ? Java::Runtime::Object::hashCode(env, clazz) : 0);
    range << qHash(memberName);
    if(signature)
        range << qHash(signature);
    range << qHash(isStatic);
    return qHashRange(range.begin(), range.end());
#endif
}

jfieldID resolveField(JNIEnv *env, const char *fieldName, const char *signature,
                      jclass clazz, bool isStatic, jthrowable* exceptionOccurred)
{
    if(!clazz)
        return nullptr;
#ifndef QTJAMBI_NOCACHE
    hash_type key = computeHash(env,clazz,fieldName,nullptr,isStatic);
    {
        QReadLocker locker(gFieldLock());
        Q_UNUSED(locker)
        jfieldID returned = gFieldHash->value(key, nullptr);
        if(returned)
            return returned;
    }
#endif // QTJAMBI_NOCACHE
    jfieldID returned = nullptr;
    if (!isStatic)
        returned = env->GetFieldID(clazz, fieldName, signature);
    else
        returned = env->GetStaticFieldID(clazz, fieldName, signature);
    if(env->ExceptionCheck()){
        if(exceptionOccurred)
            *exceptionOccurred = env->ExceptionOccurred();
        env->ExceptionClear();
        return nullptr;
    }
#ifndef QTJAMBI_NOCACHE
    {
        QWriteLocker locker(gFieldLock());
        Q_UNUSED(locker)
#ifdef QTJAMBI_COUNTCACHEMISSES
        cacheMisses(fieldName);
#endif
        if (returned != nullptr && !gFieldHash->contains(key)) {
            gFieldHash->insert(key, returned);
        }
    }
#endif // QTJAMBI_NOCACHE
    return returned;
}

/*******************************************************************************
 * Method Cache
 */

jmethodID JavaAPI::resolveMethod(JNIEnv *env, const char *methodName, const char *signature, jclass clazz, bool isStatic, jthrowable* exceptionOccurred)
{
    jmethodID returned = nullptr;
    if(clazz){
#ifndef QTJAMBI_NOCACHE
        hash_type key = computeHash(env,clazz,methodName,signature,isStatic);
        {
            QReadLocker locker(gMethodLock());
            Q_UNUSED(locker)
            returned = gMethodHash->value(key, nullptr);
            if(returned)
                return returned;
        }
#endif
        if (!isStatic)
            returned = env->GetMethodID(clazz, methodName, signature);
        else
            returned = env->GetStaticMethodID(clazz, methodName, signature);
        if(env->ExceptionCheck()){
            if(exceptionOccurred)
                *exceptionOccurred = env->ExceptionOccurred();
            env->ExceptionClear();
            return nullptr;
        }
#ifndef QTJAMBI_NOCACHE
        QWriteLocker locker(gMethodLock());
        Q_UNUSED(locker)
#ifdef QTJAMBI_COUNTCACHEMISSES
        cacheMisses(methodName);
#endif
        if (!gMethodHash->contains(key)) {
            gMethodHash->insert(key, returned);
        }
#endif // QTJAMBI_NOCACHE
    }
    return returned;
}

jmethodID findInternalPrivateConstructor(JNIEnv *env, jclass clazz){
    return JavaAPI::resolveMethod(env, "<init>", "(Lio/qt/QtObject$QPrivateConstructor;)V", clazz);
}

jclass resolveLambdaInterface(JNIEnv* env, jclass lambdaClass){
    jobjectArray interfaces = Java::Runtime::Class::getInterfaces(env, lambdaClass);
    Q_ASSERT(interfaces && env->GetArrayLength(interfaces)>0);
    return jclass( env->GetObjectArrayElement(interfaces, 0) );
}

jclass JavaAPI::resolveClosestQtSuperclass(JNIEnv *env, jclass clazz, jobject classLoader)
{
    if(!env->IsSameObject(clazz, nullptr)){
        uint key = uint(Java::Runtime::Object::hashCode(env,clazz));
        {
            QReadLocker locker(gQtSuperclassLock());
            Q_UNUSED(locker)
            jclass returned = gQtSuperclassHash->value(key, nullptr);
            if(returned)
                return returned;
        }

        jclass returned = nullptr;
        if (clazz) {
            // Check if key is a Qt class

            if (Java::QtJambi::ClassAnalyzerUtility::isGeneratedClass(env,clazz))
                returned = clazz;

            // If not, try the superclass recursively
            if (!returned && clazz) {
                jclass superKey = env->GetSuperclass(clazz);
                if (superKey) {
                    returned = JavaAPI::resolveClosestQtSuperclass(env, superKey, classLoader);
                }
            }

            if (returned) {
                returned = getGlobalClassRef(env, returned);
                QWriteLocker locker(gQtSuperclassLock());
                Q_UNUSED(locker)
                if (!gQtSuperclassHash->contains(key)) {
#ifdef QTJAMBI_COUNTCACHEMISSES
                    cacheMisses(qPrintable(QtJambiAPI::getClassName(env, returned).replace('.', '/')));
#endif
                    gQtSuperclassHash->insert(key, returned);
                }else{
                    returned = gQtSuperclassHash->value(key, nullptr);
                }
            }
            if (!returned) {
                QWriteLocker locker(gQtSuperclassLock());
                Q_UNUSED(locker)
                if (!gQtSuperclassHash->contains(key)) {
#ifdef QTJAMBI_COUNTCACHEMISSES
                    cacheMisses(qPrintable(QtJambiAPI::getClassName(env, returned).replace('.', '/')));
#endif
                    gQtSuperclassHash->insert(key, nullptr);
                }else{
                    returned = gQtSuperclassHash->value(key, nullptr);
                }
            }
        }

        return returned;
    }
    return nullptr;
}

static QmlAPI::CreateQmlErrorDummyObject createQmlErrorDummyObjectFunction = nullptr;

void QmlAPI::registerCreateQmlErrorDummyObjectFunction(CreateQmlErrorDummyObject createQmlErrorDummyObject)
{
    createQmlErrorDummyObjectFunction = createQmlErrorDummyObject;
}

QObject* QmlAPI::createQmlErrorDummyObject(const QMetaObject* metaObject, void* placement, int vsCast, int viCast)
{
    if(createQmlErrorDummyObjectFunction){
        return createQmlErrorDummyObjectFunction(metaObject, placement, vsCast, viCast);
    }else{
        return nullptr;
    }
}

size_t QmlAPI::extendedSizeForClass(JNIEnv *env, jclass object_class)
{
    size_t size = 0;
    const SuperTypeInfos& infos = SuperTypeInfos::fromClass(env, object_class);
    if(!infos.isEmpty()){
        for(const SuperTypeInfo& info : infos){
            size += info.size();
            size += sizeof(void*);
        }
    }
    return size;
}

void clearRegistryAtShutdown(JNIEnv * env){
    PolymorphicIdHash polymorphic_ids;
    {
        QWriteLocker locker(gLock());
        Q_UNUSED(locker)
        if(!g_polymorphic_ids.isDestroyed())
            polymorphic_ids.swap(*g_polymorphic_ids);
    }
    for(const PolymorphicIdHandler* handler : polymorphic_ids){
        delete handler;
    }
#ifndef QTJAMBI_NOCACHE
    ClassIdHash classIdHash;
    {
        QWriteLocker locker(gClassLock());
        Q_UNUSED(locker)
        if(!gClassHash.isDestroyed())
            classIdHash.swap(*gClassHash);
    }
    {
        QWriteLocker locker(gConstructorLock());
        Q_UNUSED(locker)
        if(!gConstructorHash.isDestroyed())
            gConstructorHash->clear();
    }
    {
        QWriteLocker locker(gQtSuperclassLock());
        Q_UNUSED(locker)
        if(!gQtSuperclassHash.isDestroyed())
            gQtSuperclassHash->clear();
    }
    {
        QWriteLocker locker(gMethodLock());
        Q_UNUSED(locker)
        if(!gMethodHash.isDestroyed())
            gMethodHash->clear();
    }
    {
        QWriteLocker locker(gFieldLock());
        Q_UNUSED(locker)
        if(!gFieldHash.isDestroyed())
            gFieldHash->clear();
    }
    {
        QWriteLocker locker(gLock());
        Q_UNUSED(locker)
        if(!gExtraSignalsOfMetaObject.isDestroyed())
            gExtraSignalsOfMetaObject->clear();
    }
    if(env){
#ifdef QTJAMBI_LOG_CLASSNAMES
        printf("DeleteGlobalRef of %i classes:\n", classIdHash.keys().size());
        int counter = 0;
#endif
        for(hash_type key : classIdHash.keys()){
            jclass cls = classIdHash[key];
#ifdef QTJAMBI_LOG_CLASSNAMES
            printf("%i.: class %s\n", ++counter, qPrintable((*gClassNameHash)[key]));
#endif
            if(env->GetObjectRefType(cls)==JNIGlobalRefType)
                env->DeleteGlobalRef(cls);
        }
    }
#endif // QTJAMBI_NOCACHE
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

typedef QVector<QSharedPointer<JObjectWrapperConverter>> ConverterList;
Q_GLOBAL_STATIC(ConverterList, gConverterList)

int JObjectWrapperConverter::jobjectWrapperID(){
    static int result = RegistryAPI::registerMetaType<JObjectWrapper>("JObjectWrapper");
    return result;
}

void JObjectWrapperConverter::registerConverter(const std::type_info& _typeId, int _to){
    if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperID(), _to) && strcmp("QRemoteObjectPendingCall", QMetaType::typeName(_to))!=0)
        ::registerConverter(new JObjectWrapperConverter(_typeId, _to), jobjectWrapperID(), _to);
}

bool registerConverter(const QtPrivate::AbstractConverterFunction *f, int from, int to);
void qtjambi_unregister_converter(int from, int to);

JObjectWrapperConverter::JObjectWrapperConverter(const std::type_info& _typeId, int _to)
    : QtPrivate::AbstractConverterFunction(&convert),
      typeId(_typeId), to(_to) {
    gConverterList->append(QSharedPointer<JObjectWrapperConverter>(this));
}

JObjectWrapperConverter::~JObjectWrapperConverter(){
    qtjambi_unregister_converter(jobjectWrapperID(), to);
}

bool JObjectWrapperConverter::convert(const AbstractConverterFunction *converterFunction, const void * from, void* to) {
    const JObjectWrapperConverter* converter = reinterpret_cast<const JObjectWrapperConverter*>(converterFunction);
    const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(from);
    if(converter && wrapper && wrapper->object()){
        if(JniEnvironment env{200}){
            void * ptr(nullptr);
            if(QtJambiAPI::convertJavaToNative(env, converter->typeId, wrapper->object(), &ptr)){
                if((QMetaType(converter->to).name().endsWith("*"))){
                    *reinterpret_cast<void**>(to) = ptr;
                }else {
                    QMetaType::construct(converter->to, to, ptr);
                }
                return true;
            }
        }
        return false;
    }
    return true;
}
#endif


#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

struct JObjectValueWrapperPrivate{
    jclass clazz = nullptr;
    jmethodID constructor = 0;
    jmethodID clone = 0;
    jmethodID writeTo = 0;
    jmethodID readFrom = 0;
    bool isSerializable = false;
    QByteArray name;
    const QMetaObject * metaObject = reinterpret_cast<const QMetaObject *>(quintptr(std::numeric_limits<quintptr>::max()));
    static constexpr auto metaObjectFn = &JObjectValueWrapper::metaObject;
    static constexpr auto defaultCtr = &JObjectValueWrapper::defaultCtr;
    static constexpr auto copyCtr = &JObjectValueWrapper::copyCtr;
    static constexpr auto moveCtr = &JObjectValueWrapper::moveCtr;
    static constexpr auto dtor = &JObjectValueWrapper::dtor;
    static constexpr auto equals = &JObjectValueWrapper::equals;
    static constexpr auto lessThan = &JObjectValueWrapper::lessThan;
    static constexpr auto debugStream = &JObjectValueWrapper::debugStream;
    static constexpr auto dataStreamOut = &JObjectValueWrapper::dataStreamOut;
    static constexpr auto dataStreamIn = &JObjectValueWrapper::dataStreamIn;
    static constexpr auto serializableDataStreamOut = &JObjectValueWrapper::serializableDataStreamOut;
    static constexpr auto serializableDataStreamIn = &JObjectValueWrapper::serializableDataStreamIn;
};

JObjectWrapper qtjambiCreateObject(const JObjectValueWrapperPrivate* methods){
    if(methods){
        if(JniEnvironment env{200}){
            try{
                return JObjectWrapper(env, env->NewObject(methods->clazz, methods->constructor));
            }catch(const JavaException& exn){
                exn.report(env);
            }
        }
    }
    return JObjectWrapper();
}

JObjectWrapper qtjambiCopyObject(JNIEnv* env, const JObjectValueWrapperPrivate* methods, const JObjectWrapper& other){
    if(methods && env){
        try{
            jobject o = env->CallObjectMethod(other.object(), methods->clone);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            return JObjectWrapper(env, o);
        }catch(const JavaException& exn){
            exn.report(env);
        }
    }
    return JObjectWrapper(other);
}

JObjectWrapper qtjambiCopyObject(const JObjectValueWrapperPrivate* methods, const JObjectWrapper& other){
    if(JniEnvironment env{200}){
        return qtjambiCopyObject(env, methods, other);
    }
    return JObjectWrapper(other);
}

typedef QMap<size_t, const QtPrivate::QMetaTypeInterface *> ClassMetaTypeInterfaceHash;
Q_GLOBAL_STATIC(ClassMetaTypeInterfaceHash, gClassMetaTypeInterfaces)
typedef QMap<const QtPrivate::QMetaTypeInterface *,JObjectValueWrapperPrivate> MetaTypeInterfacesHash;
Q_GLOBAL_STATIC(MetaTypeInterfacesHash, gMetaTypeInterfaces)
typedef QMap<const QtPrivate::QMetaTypeInterface *,jclass> MetaTypeEnumClassesHash;
Q_GLOBAL_STATIC(MetaTypeEnumClassesHash, gMetaTypeEnumClasses)

bool JObjectValueWrapper::isValueType(QMetaType metaType){
    const JObjectValueWrapperPrivate* methods{nullptr};
    {
        QReadLocker locker(gLock());
        if(gMetaTypeInterfaces->contains(metaType.iface()))
            methods = &(*gMetaTypeInterfaces)[metaType.iface()];
    }
    return methods;
}

bool JObjectValueWrapper::hasCustomDebugStreamOperator(QMetaType metaType){
    return metaType.iface()->debugStream!=JObjectValueWrapper::debugStream;
}

JObjectValueWrapper JObjectValueWrapper::create(JNIEnv* env, jobject object, QMetaType metaType){
    const JObjectValueWrapperPrivate* methods{nullptr};
    {
        QReadLocker locker(gLock());
        if(gMetaTypeInterfaces->contains(metaType.iface()))
            methods = &(*gMetaTypeInterfaces)[metaType.iface()];
    }
    if(methods){
        jobject o = env->IsSameObject(object, nullptr) ? nullptr : env->CallObjectMethod(object, methods->clone);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        return JObjectValueWrapper(env, o, methods);
    }
    return {};
}

JObjectValueWrapper::JObjectValueWrapper() : JObjectWrapper(), p(nullptr) {}
JObjectValueWrapper::JObjectValueWrapper(const JObjectValueWrapperPrivate* _p)
    : JObjectWrapper(qtjambiCreateObject(_p)),
      p(_p){}

JObjectValueWrapper::JObjectValueWrapper(JNIEnv* env, jobject object, const JObjectValueWrapperPrivate* _p)
    : JObjectWrapper(env, object),
      p(_p){}

JObjectValueWrapper::JObjectValueWrapper(JObjectWrapper&& object, const JObjectValueWrapperPrivate* _p)
    : JObjectWrapper(std::move(object)),
      p(_p){}


JObjectValueWrapper::JObjectValueWrapper(const JObjectValueWrapper& other)
    : JObjectWrapper(qtjambiCopyObject(other.p, other)),
      p(other.p){}

JObjectValueWrapper::JObjectValueWrapper(JObjectValueWrapper&& other)
    : JObjectWrapper(std::move(other)),
      p(other.p){}

JObjectValueWrapper::~JObjectValueWrapper() {}

jobject JObjectValueWrapper::value(JNIEnv* env) const{
    jobject o = env->CallObjectMethod(object(), p->clone);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    return o;
}

JObjectValueWrapper& JObjectValueWrapper::operator=(const JObjectValueWrapper &wrapper){
    JObjectWrapper::operator=(qtjambiCopyObject(wrapper.p, wrapper));
    p = wrapper.p;
    return *this;
}

JObjectValueWrapper& JObjectValueWrapper::operator=(JObjectValueWrapper &&wrapper){
    JObjectWrapper::operator=(std::move(wrapper));
    p = wrapper.p;
    return *this;
}

JObjectValueWrapper& JObjectValueWrapper::operator=(const JObjectWrapper &wrapper){
    if(const JObjectValueWrapper* _wrapper = dynamic_cast<const JObjectValueWrapper*>(&wrapper)){
        return operator=(*_wrapper);
    }else if(p){
        if(JniEnvironment env{200}){
            if(env->IsInstanceOf(wrapper.object(), p->clazz)){
                return operator=(JObjectValueWrapper(qtjambiCopyObject(env, p, wrapper.object()), p));
            }
        }
    }
    return *this;
}

JObjectValueWrapper& JObjectValueWrapper::operator=(JObjectWrapper &&wrapper){
    if(JObjectValueWrapper* _wrapper = dynamic_cast<JObjectValueWrapper*>(&wrapper)){
        return operator=(std::move(*_wrapper));
    }else{
        if(JniEnvironment env{200}){
            if(env->IsInstanceOf(wrapper.object(), p->clazz)){
                return operator=(JObjectValueWrapper(std::move(wrapper), p));
            }
        }
    }
    return *this;
}

JObjectValueWrapper& JObjectValueWrapper::operator=(jobject obj){
    if(p){
        if(JniEnvironment env{200}){
            if(env->IsInstanceOf(obj, p->clazz)){
                JObjectWrapper::operator=(obj);
            }
        }
    }
    return *this;
}

bool JObjectValueWrapper::operator==(const JObjectValueWrapper &other) const{
    return p==other.p && JObjectWrapper::operator==(other);
}

bool JObjectValueWrapper::operator<(const JObjectValueWrapper &other) const{
    return p==other.p && JObjectWrapper::operator<(other);

}

bool JObjectValueWrapper::isNull()const{return !p;}

void JObjectValueWrapper::writeTo(QDataStream &d)const{
    if(JniEnvironment env{200}){
        try{
            jobject _d = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, &d);
            QTJAMBI_INVALIDATE_AFTER_USE(env, _d);
            env->CallVoidMethod(object(), p->writeTo, _d);
        }catch(const JavaException& exn){
            exn.report(env);
        }
    }
}
void JObjectValueWrapper::readFrom(QDataStream &d){
    if(JniEnvironment env{200}){
        try{
            jobject _d = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, &d);
            QTJAMBI_INVALIDATE_AFTER_USE(env, _d);
            env->CallVoidMethod(object(), p->readFrom, _d);
        }catch(const JavaException& exn){
            exn.report(env);
        }
    }
}

void JObjectValueWrapper::serializeTo(QDataStream &d)const{
    if(JniEnvironment env{200}){
        try{
            jobject _d = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, &d);
            QTJAMBI_INVALIDATE_AFTER_USE(env, _d);
            Java::QtJambi::MetaTypeUtility::writeSerializableJavaObject(env, _d, object());
        }catch(const JavaException& exn){
            exn.report(env);
        }
    }
}
void JObjectValueWrapper::serializeFrom(QDataStream &d){
    if(JniEnvironment env{200}){
        try{
            jobject _d = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, &d);
            QTJAMBI_INVALIDATE_AFTER_USE(env, _d);
            jobject newValue = Java::QtJambi::MetaTypeUtility::readSerializableJavaObject(env, _d);
            *this = JObjectValueWrapper(env, newValue, p);
        }catch(const JavaException& exn){
            exn.report(env);
        }
    }
}

const QMetaObject *JObjectValueWrapper::metaObject(const QtPrivate::QMetaTypeInterface *iface){
    const JObjectValueWrapperPrivate* methods{nullptr};
    {
        QReadLocker locker(gLock());
        if(gMetaTypeInterfaces->contains(iface))
            methods = &(*gMetaTypeInterfaces)[iface];
    }
    if(methods){
        if(quintptr(methods->metaObject) == std::numeric_limits<quintptr>::max()){
            if(JniEnvironment env{200}){
                const_cast<JObjectValueWrapperPrivate*>(methods)->metaObject = CoreAPI::metaObjectForClass(env, methods->clazz);
                if(!methods->metaObject){
                    const_cast<QtPrivate::QMetaTypeInterface*>(iface)->flags &= ~QMetaType::IsGadget;
                    const_cast<QtPrivate::QMetaTypeInterface*>(iface)->flags &= ~QMetaType::PointerToGadget;
                }
            }else return nullptr;
        }
        return methods->metaObject;
    }
    return nullptr;
}

void JObjectValueWrapper::defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *target){
    const JObjectValueWrapperPrivate* methods{nullptr};
    {
        QReadLocker locker(gLock());
        if(gMetaTypeInterfaces->contains(iface))
            methods = &(*gMetaTypeInterfaces)[iface];
    }
    new (target) JObjectValueWrapper(qtjambiCreateObject(methods), methods);
}

void JObjectValueWrapper::copyCtr(const QtPrivate::QMetaTypeInterface *, void *target, const void *other){
    const JObjectValueWrapper* _other = reinterpret_cast<const JObjectValueWrapper*>(other);
    new (target) JObjectValueWrapper(*_other);
}

void JObjectValueWrapper::moveCtr(const QtPrivate::QMetaTypeInterface *, void *target, void *other){
    JObjectValueWrapper* _other = reinterpret_cast<JObjectValueWrapper*>(other);
    new (target) JObjectValueWrapper(std::move(*_other));
}

void JObjectValueWrapper::dtor(const QtPrivate::QMetaTypeInterface *, void *target){
    reinterpret_cast<JObjectValueWrapper*>(target)->~JObjectValueWrapper();
}

bool JObjectValueWrapper::equals(const QtPrivate::QMetaTypeInterface *iface, const void *value1, const void *value2){
    const JObjectValueWrapper* _value1 = reinterpret_cast<const JObjectValueWrapper*>(value1);
    const JObjectValueWrapper* _value2 = reinterpret_cast<const JObjectValueWrapper*>(value2);
    if(_value1->isNull() || _value2->isNull()){
        const JObjectValueWrapperPrivate* methods{nullptr};
        {
            QReadLocker locker(gLock());
            if(gMetaTypeInterfaces->contains(iface))
                methods = &(*gMetaTypeInterfaces)[iface];
        }
        if(methods){
            if(_value1->isNull())
                *const_cast<JObjectValueWrapper*>(_value1) = JObjectValueWrapper(methods);
            if(_value2->isNull())
                *const_cast<JObjectValueWrapper*>(_value2) = JObjectValueWrapper(methods);
        }else return false;
    }
    try{
        return *_value1 == *_value2;
    }catch(const JavaException& exn){
        JniEnvironment env{200};
        exn.report(env);
        return false;
    }
}

bool JObjectValueWrapper::lessThan(const QtPrivate::QMetaTypeInterface *iface, const void *value1, const void *value2){
    const JObjectValueWrapper* _value1 = reinterpret_cast<const JObjectValueWrapper*>(value1);
    const JObjectValueWrapper* _value2 = reinterpret_cast<const JObjectValueWrapper*>(value2);
    if(_value1->isNull() || _value2->isNull()){
        const JObjectValueWrapperPrivate* methods{nullptr};
        {
            QReadLocker locker(gLock());
            if(gMetaTypeInterfaces->contains(iface))
                methods = &(*gMetaTypeInterfaces)[iface];
        }
        if(methods){
            if(_value1->isNull())
                *const_cast<JObjectValueWrapper*>(_value1) = JObjectValueWrapper(methods);
            if(_value2->isNull())
                *const_cast<JObjectValueWrapper*>(_value2) = JObjectValueWrapper(methods);
        }else return false;
    }
    try{
        return *_value1 < *_value2;
    }catch(const JavaException& exn){
        JniEnvironment env{200};
        exn.report(env);
        return false;
    }
}

void JObjectValueWrapper::debugStream(const QtPrivate::QMetaTypeInterface *iface, QDebug &d, const void *value){
    const JObjectValueWrapper* _value = reinterpret_cast<const JObjectValueWrapper*>(value);
    if(_value->isNull()){
        const JObjectValueWrapperPrivate* methods{nullptr};
        {
            QReadLocker locker(gLock());
            if(gMetaTypeInterfaces->contains(iface))
                methods = &(*gMetaTypeInterfaces)[iface];
        }
        if(methods)
            *const_cast<JObjectValueWrapper*>(_value) = JObjectValueWrapper(methods);
        else return;
    }
    try{
        bool ok = false;
        QString strg = _value->toString(&ok);
        if(ok)
            d << strg;
    }catch(const JavaException& exn){
        JniEnvironment env{200};
        exn.report(env);
    }
}

void JObjectValueWrapper::dataStreamOut(const QtPrivate::QMetaTypeInterface *iface, QDataStream &d, const void *value){
    try{
        const JObjectValueWrapper* _value = reinterpret_cast<const JObjectValueWrapper*>(value);
        if(_value->isNull()){
            const JObjectValueWrapperPrivate* methods{nullptr};
            {
                QReadLocker locker(gLock());
                if(gMetaTypeInterfaces->contains(iface))
                    methods = &(*gMetaTypeInterfaces)[iface];
            }
            if(methods)
                *const_cast<JObjectValueWrapper*>(_value) = JObjectValueWrapper(methods);
            else return;
        }
        _value->writeTo(d);
    }catch(const JavaException& exn){
        JniEnvironment env{200};
        exn.report(env);
    }
}

void JObjectValueWrapper::dataStreamIn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &d, void *value){
    try{
        JObjectValueWrapper* _value = reinterpret_cast<JObjectValueWrapper*>(value);
        if(_value->isNull()){
            const JObjectValueWrapperPrivate* methods{nullptr};
            {
                QReadLocker locker(gLock());
                if(gMetaTypeInterfaces->contains(iface))
                    methods = &(*gMetaTypeInterfaces)[iface];
            }
            if(methods)
                *_value = JObjectValueWrapper(methods);
            else return;
        }
        _value->readFrom(d);
    }catch(const JavaException& exn){
        JniEnvironment env{200};
        exn.report(env);
    }
}

void JObjectValueWrapper::serializableDataStreamOut(const QtPrivate::QMetaTypeInterface *iface, QDataStream &d, const void *value){
    try{
        const JObjectValueWrapper* _value = reinterpret_cast<const JObjectValueWrapper*>(value);
        if(_value->isNull()){
            const JObjectValueWrapperPrivate* methods{nullptr};
            {
                QReadLocker locker(gLock());
                if(gMetaTypeInterfaces->contains(iface))
                    methods = &(*gMetaTypeInterfaces)[iface];
            }
            if(methods)
                *const_cast<JObjectValueWrapper*>(_value) = JObjectValueWrapper(methods);
            else return;
        }
        _value->serializeTo(d);
    }catch(const JavaException& exn){
        JniEnvironment env{200};
        exn.report(env);
    }
}

void JObjectValueWrapper::serializableDataStreamIn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &d, void *value){
    try{
        JObjectValueWrapper* _value = reinterpret_cast<JObjectValueWrapper*>(value);
        if(_value->isNull()){
            const JObjectValueWrapperPrivate* methods{nullptr};
            {
                QReadLocker locker(gLock());
                if(gMetaTypeInterfaces->contains(iface))
                    methods = &(*gMetaTypeInterfaces)[iface];
            }
            if(methods)
                *_value = JObjectValueWrapper(methods);
            else return;
        }
        _value->serializeFrom(d);
    }catch(const JavaException& exn){
        JniEnvironment env{200};
        exn.report(env);
    }
}

int QmlAPI::registerMetaType(JNIEnv *env, SequentialContainerType containerType, const QMetaType& elementType){
    int result = 0;
    AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(AbstractContainerAccess::create(env, containerType, elementType));
    if(!listAccess){
        QHashFunction hashFunction;
        InternalToExternalConverter internalToExternalConverter;
        ExternalToInternalConverter externalToInternalConverter;
        const JObjectValueWrapperPrivate* methods{nullptr};
        {
            const QtPrivate::QMetaTypeInterface *iface = QMetaType(elementType).iface();
            QReadLocker locker(gLock());
            if(gMetaTypeInterfaces->contains(iface))
                methods = &(*gMetaTypeInterfaces)[iface];
        }
        if(methods){
            hashFunction = [](const void* ptr, hash_type seed)->hash_type{
                if(JniEnvironment env{200}){
                    try{
                        seed = qHash(Java::Runtime::Object::hashCode(env, reinterpret_cast<const JObjectValueWrapper*>(ptr)->object()), seed);
                    }catch(const JavaException& exn){
                        exn.report(env);
                    }
                }
                return seed;
            };
            internalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(env, QLatin1String(elementType.name()), elementType, methods->clazz);
            externalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(env, methods->clazz, QLatin1String(elementType.name()), elementType);
        }else{
            const std::type_info* typeId;
            {
                QReadLocker locker(gLock());
                typeId = gMetaTypeIDHash->value(elementType.id());
            }
            if(typeId)
                hashFunction = registeredHashFunction(*typeId);
            if(jclass clazz = CoreAPI::getClassForMetaType(env, elementType)){
                internalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(env, QLatin1String(elementType.name()), elementType, clazz);
                externalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(env, clazz, QLatin1String(elementType.name()), elementType);
            }
        }
        listAccess = dynamic_cast<AbstractListAccess*>(AbstractContainerAccess::create(
                                                                           env,
                                                                           containerType,
                                                                           elementType,
                                                                           elementType.alignOf(),
                                                                           elementType.sizeOf(),
                                                                           false,
                                                                           hashFunction,
                                                                           internalToExternalConverter,
                                                                           externalToInternalConverter
                                                                           ));
    }
    if(listAccess){
        QString containerName;
        switch(containerType){
        case SequentialContainerType::QSet:
            containerName = QStringLiteral("QSet");
        case SequentialContainerType::QQueue:
            containerName = QStringLiteral("QQueue");
        case SequentialContainerType::QStack:
            containerName = QStringLiteral("QStack");
        default:
            containerName = QStringLiteral("QList");
        }

        result = listAccess->registerContainer(QStringLiteral("%1<%2>").arg(containerName, QLatin1String(elementType.name())).toUtf8());
        listAccess->dispose();
    }
    return result;
}

template<typename INT, int isQtEnum0OrFlag1 = 0>
int qtjambi_register_enum_meta_type(JNIEnv *env, jclass clazz, const QString& javaClassName, const QByteArray& javaTypeName){
    enum E:INT{};
    typedef typename std::conditional<isQtEnum0OrFlag1==1,QFlags<E>,E>::type EnumOrFlags;
    typedef typename std::conditional<isQtEnum0OrFlag1==1,JObjectWrapper,JEnumWrapper>::type Wrapper;
    QtPrivate::QMetaTypeInterface* metaTypeInterface = new QtPrivate::QMetaTypeInterface{
            /*.revision=*/ 0,
            /*.alignment=*/ alignof(EnumOrFlags),
            /*.size=*/ sizeof(EnumOrFlags),
            /*.flags=*/ uint(QtPrivate::QMetaTypeTypeFlags<EnumOrFlags>::Flags),
            /*.typeId=*/ QMetaType::UnknownType,
            /*.metaObject=*/ nullptr,
            /*.name=*/ registerMetaTypeName(javaTypeName),
            /*.defaultCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<EnumOrFlags>::defaultCtr,
            /*.copyCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<EnumOrFlags>::copyCtr,
            /*.moveCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<EnumOrFlags>::moveCtr,
            /*.dtor=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<EnumOrFlags>::dtor,
            /*.equals=*/ QtPrivate::QEqualityOperatorForType<EnumOrFlags>::equals,
            /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<EnumOrFlags>::lessThan,
            /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<EnumOrFlags>::debugStream,
            /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<EnumOrFlags>::dataStreamOut,
            /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<EnumOrFlags>::dataStreamIn,
            /*.legacyRegisterOp=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<EnumOrFlags>::legacyRegisterOp
    };
    {
        QWriteLocker locker(gLock());
        if(const QtPrivate::QMetaTypeInterface * iface = gClassMetaTypeInterfaces->value(qHash(javaClassName))){
            delete metaTypeInterface;
            return iface->typeId;
        }
        gClassMetaTypeInterfaces->insert(qHash(javaClassName), metaTypeInterface);
        (*gMetaTypeEnumClasses)[metaTypeInterface] = getGlobalClassRef(env, clazz, qPrintable(javaClassName));
    }
    QMetaType metaType(metaTypeInterface);
    int result = metaType.id();
    auto conv = [metaTypeInterface](const void *src, void *target) -> bool {
        if(src){
            jclass clazz;
            {
                QReadLocker locker(gLock());
                clazz = (*gMetaTypeEnumClasses)[metaTypeInterface];
            }
            if(clazz){
                if(JniEnvironment env{200}){
                    try{
                        if constexpr(isQtEnum0OrFlag1==0){
                            new (target)Wrapper(QtJambiAPI::convertEnumToJavaObject(env, *reinterpret_cast<const INT*>(src), clazz));
                        }else if constexpr(isQtEnum0OrFlag1==1){
                            new (target)Wrapper(QtJambiAPI::convertQFlagsToJavaObject(env, *reinterpret_cast<const INT*>(src), clazz));
                        }else{
                            jobjectArray enumConstants = Java::Runtime::Class::getEnumConstants(env, clazz);
                            jobject e = env->GetObjectArrayElement(enumConstants, *reinterpret_cast<const INT*>(src));
                            new (target)Wrapper(e);
                        }
                    }catch(const JavaException& exn){
                        exn.report(env);
                        return false;
                    }
                }else{
                    return false;
                }
            }
        }else{
            new (target)Wrapper();
        }
        return true;
    };
    QMetaType::registerConverterFunction(conv, metaType, QMetaType::fromType<JObjectWrapper>());
    if constexpr(isQtEnum0OrFlag1!=1){
        QMetaType::registerConverterFunction(conv, metaType, QMetaType::fromType<JEnumWrapper>());
    }
    auto iconv = [](const void *src, void *target) -> bool {
        if(src){
            new (target)typename QIntegerForSizeof<EnumOrFlags>::Signed(*reinterpret_cast<const E*>(src));
        }else{
            new (target)typename QIntegerForSizeof<EnumOrFlags>::Signed();
        }
        return true;
    };
    QMetaType::registerConverterFunction(iconv, metaType, QMetaType::fromType<typename QIntegerForSizeof<EnumOrFlags>::Signed>());
    QMetaType::registerConverterFunction(iconv, metaType, QMetaType::fromType<typename QIntegerForSizeof<EnumOrFlags>::Unsigned>());
    QMetaType::registerConverterFunction(iconv, QMetaType::fromType<typename QIntegerForSizeof<EnumOrFlags>::Signed>(), metaType);
    QMetaType::registerConverterFunction(iconv, QMetaType::fromType<typename QIntegerForSizeof<EnumOrFlags>::Unsigned>(), metaType);
    registerJavaClassForCustomMetaType(env, metaType, clazz);
    return result;
}
#else
void destructHelper(void *){}

void * pointerConstructHelper(void * where, const void *pointer)
{
    if(pointer){
        *reinterpret_cast<void**>(where) = *reinterpret_cast<void**>(const_cast<void*>(pointer));
    }
    return where;
}

template<size_t SIZE>
void * constructHelper(void * where, const void *pointer)
{
    memcpy(where, pointer, SIZE);
    return where;
}

typedef void*(*ConstructorFn)(void*, const void*);
ConstructorFn findEnumConstructor(int size){
    switch(size){
    case 1: return &constructHelper<1>;
    case 2: return &constructHelper<2>;
    case 4: return &constructHelper<4>;
    case 8: return &constructHelper<8>;
    default:
        return nullptr;
    }
}
#endif

int registerMetaType(JNIEnv *env, jclass clazz, jboolean isPointer, jboolean isReference, const QString* javaClassNamePtr)
{
    using namespace RegistryAPI;
    try{
        int id = CoreAPI::metaTypeId(env, clazz);
        if(id!=QMetaType::UnknownType)
            return id;
        if(Java::Runtime::Class::isArray(env, clazz) && !isPointer){
            jclass componentType = Java::Runtime::Class::getComponentType(env, clazz);
            if(Java::Runtime::Integer::isPrimitiveType(env, componentType)){
                return registerMetaType<JIntArrayWrapper>("JIntArrayWrapper");
            }else if(Java::Runtime::Short::isPrimitiveType(env, componentType)){
                return registerMetaType<JShortArrayWrapper>("JShortArrayWrapper");
            }else if(Java::Runtime::Byte::isPrimitiveType(env, componentType)){
                return registerMetaType<JByteArrayWrapper>("JByteArrayWrapper");
            }else if(Java::Runtime::Long::isPrimitiveType(env, componentType)){
                return registerMetaType<JLongArrayWrapper>("JLongArrayWrapper");
            }else if(Java::Runtime::Character::isPrimitiveType(env, componentType)){
                return registerMetaType<JCharArrayWrapper>("JCharArrayWrapper");
            }else if(Java::Runtime::Boolean::isPrimitiveType(env, componentType)){
                return registerMetaType<JBooleanArrayWrapper>("JBooleanArrayWrapper");
            }else if(Java::Runtime::Double::isPrimitiveType(env, componentType)){
                return registerMetaType<JDoubleArrayWrapper>("JDoubleArrayWrapper");
            }else if(Java::Runtime::Float::isPrimitiveType(env, componentType)){
                return registerMetaType<JFloatArrayWrapper>("JFloatArrayWrapper");
            }else{
                return registerMetaType<JObjectArrayWrapper>("JObjectArrayWrapper");
            }
        }else if((Java::Runtime::Integer::isSameClass(env, clazz) || Java::Runtime::Integer::isPrimitiveType(env, clazz)) && !isPointer){
            return QMetaType::Int;
        }else if((Java::Runtime::Long::isSameClass(env, clazz) || Java::Runtime::Long::isPrimitiveType(env, clazz)) && !isPointer){
            return QMetaType::LongLong;
        }else if((Java::Runtime::Short::isSameClass(env, clazz) || Java::Runtime::Short::isPrimitiveType(env, clazz)) && !isPointer){
            return QMetaType::Short;
        }else if((Java::Runtime::Byte::isSameClass(env, clazz) || Java::Runtime::Byte::isPrimitiveType(env, clazz)) && !isPointer){
            return QMetaType::SChar;
        }else if((Java::Runtime::Boolean::isSameClass(env, clazz) || Java::Runtime::Boolean::isPrimitiveType(env, clazz)) && !isPointer){
            return QMetaType::Bool;
        }else if((Java::Runtime::Character::isSameClass(env, clazz) || Java::Runtime::Character::isPrimitiveType(env, clazz)) && !isPointer){
            return QMetaType::QChar;
        }else if((Java::Runtime::Float::isSameClass(env, clazz) || Java::Runtime::Float::isPrimitiveType(env, clazz)) && !isPointer){
            return QMetaType::Float;
        }else if((Java::Runtime::Double::isSameClass(env, clazz) || Java::Runtime::Double::isPrimitiveType(env, clazz)) && !isPointer){
            return QMetaType::Double;
        }else if((Java::Runtime::Void::isPrimitiveType(env, clazz)) && !isPointer){
            return QMetaType::Void;
        }else{
            QString javaClassName = javaClassNamePtr ? *javaClassNamePtr : QtJambiAPI::getClassName(env, clazz).replace('.', '/');
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            {
                QReadLocker locker(gLock());
                if(const QtPrivate::QMetaTypeInterface * iface = gClassMetaTypeInterfaces->value(qHash(javaClassName))){
                    return iface->typeId;
                }
            }
#endif
            if(const std::type_info* typeId = getTypeByJavaName(javaClassName)){
                QByteArray qtType = getQtName(*typeId);
                if((!isPointer || qtType.endsWith("*")) && (!isReference || !qtType.endsWith("*"))){
                    int id = registeredMetaTypeID(*typeId);
                    if(id!=QMetaType::UnknownType)
                        return id;
                }
                const QMetaObject *const original_meta_object = registeredOriginalMetaObject(*typeId);
                QMetaType::TypeFlags flags;
                QByteArray typeName = QMetaObject::normalizedType(qtType);
                EntryTypes entryType = getEntryType(*typeId);
                if(isReference){
                    switch(entryType){
                    case EntryTypes::ObjectTypeInfo:
                    case EntryTypes::QObjectTypeInfo:
                        if(typeName.endsWith("*")){
                            typeName.chop(1);
                        }
                        entryType = EntryTypes::ValueTypeInfo; break;
                    case EntryTypes::InterfaceTypeInfo:
                        if(typeName.endsWith("*")){
                            typeName.chop(1);
                        }
                        entryType = EntryTypes::InterfaceValueTypeInfo; break;
                    default: break;
                    }
                }else if(isPointer){
                    switch(entryType){
                    case EntryTypes::ValueTypeInfo:
                        entryType = EntryTypes::ObjectTypeInfo; break;
                    case EntryTypes::InterfaceValueTypeInfo:
                        entryType = EntryTypes::InterfaceTypeInfo; break;
                    default: break;
                    }
                }
                bool isQObject = false;
                switch(entryType){
                case EntryTypes::QObjectTypeInfo:
                    isQObject = true;
                case EntryTypes::ObjectTypeInfo:
                case EntryTypes::InterfaceTypeInfo:
                case EntryTypes::FunctionalTypeInfo: {
                    flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<void*>::Flags);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                    flags |= QMetaType::IsPointer;
#endif
                    if(entryType!=EntryTypes::FunctionalTypeInfo && !typeName.endsWith("*")){
                        typeName = QMetaObject::normalizedType(typeName + "*");
                    }
                    if(isQObject){
                        flags |= QMetaType::PointerToQObject;
                    }else if(original_meta_object){
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                        flags |= QMetaType::IsGadget;
#endif
                        flags |= QMetaType::PointerToGadget;
                    }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    QMetaType::Destructor destructor = &destructHelper;
                    QMetaType::Constructor constructor = &pointerConstructHelper;
                    int typeId = QMetaType::registerNormalizedType(
                            typeName,
                            destructor,
                            constructor,
                            sizeof(void*),
                            flags,
                            original_meta_object
                        );
                    RegistryAPI::registerComparator(new QtPrivate::BuiltInComparatorFunction<void*>(), typeId);
                    QmlAPI::registerJavaClassForCustomMetaType(env, typeId, clazz);
                    return typeId;
#else
                    QMetaType metaType = createMetaType(typeName,
                                                        true,
                                                        /*.defaultCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<void*>::defaultCtr,
                                                        /*.copyCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<void*>::copyCtr,
                                                        /*.moveCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<void*>::moveCtr,
                                                        /*.dtor=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<void*>::dtor,
                                                        /*.equals=*/ QtPrivate::QEqualityOperatorForType<void*>::equals,
                                                        /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<void*>::lessThan,
                                                        /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<void*>::debugStream,
                                                        /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamOut,
                                                        /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamIn,
                                                        /*.legacyRegisterOp=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<void*>::legacyRegisterOp,
                                                        /*.size=*/ sizeof(void*),
                                                        /*.alignment=*/ alignof(void*),
                                                        /*.typeId=*/ QMetaType::UnknownType,
                                                        /*.flags=*/ QMetaType::TypeFlags(flags),
                                                        original_meta_object,
                                                        nullptr);
                    metaType.id();
                    registerJavaClassForCustomMetaType(env, metaType, clazz);
                    registerConverterVariant(env, metaType, typeName, javaClassName, clazz);
                    return metaType.id();
#endif
                }
                case EntryTypes::EnumTypeInfo:{
                    flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<Qt::Orientation>::Flags);
                    if(Java::QtJambi::QtShortEnumerator::isAssignableFrom(env, clazz)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        int typeId = QMetaType::registerNormalizedType(
                                typeName,
                                &destructHelper,
                                findEnumConstructor(sizeof(qint16)),
                                sizeof(sizeof(qint16)),
                                flags,
                                original_meta_object
                            );
                        RegistryAPI::registerComparator(new QtPrivate::BuiltInComparatorFunction<qint16>(), typeId);
                        QmlAPI::registerJavaClassForCustomMetaType(env, typeId, clazz);
                        return typeId;
#else
                        QMetaType metaType = createMetaType(typeName,
                                                            true,
                                                            /*.defaultCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<qint16>::defaultCtr,
                                                            /*.copyCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<qint16>::copyCtr,
                                                            /*.moveCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<qint16>::moveCtr,
                                                            /*.dtor=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<qint16>::dtor,
                                                            /*.equals=*/ QtPrivate::QEqualityOperatorForType<qint16>::equals,
                                                            /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<qint16>::lessThan,
                                                            /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<qint16>::debugStream,
                                                            /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<qint16>::dataStreamOut,
                                                            /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<qint16>::dataStreamIn,
                                                            /*.legacyRegisterOp=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<qint16>::legacyRegisterOp,
                                                            /*.size=*/ sizeof(qint16),
                                                            /*.alignment=*/ alignof(qint16),
                                                            /*.typeId=*/ QMetaType::UnknownType,
                                                            /*.flags=*/ QMetaType::TypeFlags(flags),
                                                            original_meta_object,
                                                            nullptr);
                        int _id = metaType.id();
                        registerJavaClassForCustomMetaType(env, metaType, clazz);
                        return _id;
#endif
                    }else if(Java::QtJambi::QtLongEnumerator::isAssignableFrom(env, clazz)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        int typeId = QMetaType::registerNormalizedType(
                                typeName,
                                &destructHelper,
                                findEnumConstructor(sizeof(qint64)),
                                sizeof(sizeof(qint64)),
                                flags,
                                original_meta_object
                            );
                        RegistryAPI::registerComparator(new QtPrivate::BuiltInComparatorFunction<qint64>(), typeId);
                        QmlAPI::registerJavaClassForCustomMetaType(env, typeId, clazz);
                        return typeId;
#else
                        QMetaType metaType = createMetaType(typeName,
                                                            true,
                                                            /*.defaultCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<qint64>::defaultCtr,
                                                            /*.copyCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<qint64>::copyCtr,
                                                            /*.moveCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<qint64>::moveCtr,
                                                            /*.dtor=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<qint64>::dtor,
                                                            /*.equals=*/ QtPrivate::QEqualityOperatorForType<qint64>::equals,
                                                            /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<qint64>::lessThan,
                                                            /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<qint64>::debugStream,
                                                            /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<qint64>::dataStreamOut,
                                                            /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<qint64>::dataStreamIn,
                                                            /*.legacyRegisterOp=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<qint64>::legacyRegisterOp,
                                                            /*.size=*/ sizeof(qint64),
                                                            /*.alignment=*/ alignof(qint64),
                                                            /*.typeId=*/ QMetaType::UnknownType,
                                                            /*.flags=*/ QMetaType::TypeFlags(flags),
                                                            original_meta_object,
                                                            nullptr);
                        int _id = metaType.id();
                        registerJavaClassForCustomMetaType(env, metaType, clazz);
                        return _id;
#endif
                    }else if(Java::QtJambi::QtByteEnumerator::isAssignableFrom(env, clazz)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        int typeId = QMetaType::registerNormalizedType(
                                typeName,
                                &destructHelper,
                                findEnumConstructor(sizeof(qint8)),
                                sizeof(sizeof(qint8)),
                                flags,
                                original_meta_object
                            );
                        RegistryAPI::registerComparator(new QtPrivate::BuiltInComparatorFunction<qint8>(), typeId);
                        QmlAPI::registerJavaClassForCustomMetaType(env, typeId, clazz);
                        return typeId;
#else
                        QMetaType metaType = createMetaType(typeName,
                                                            true,
                                                            /*.defaultCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<qint8>::defaultCtr,
                                                            /*.copyCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<qint8>::copyCtr,
                                                            /*.moveCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<qint8>::moveCtr,
                                                            /*.dtor=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<qint8>::dtor,
                                                            /*.equals=*/ QtPrivate::QEqualityOperatorForType<qint8>::equals,
                                                            /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<qint8>::lessThan,
                                                            /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<qint8>::debugStream,
                                                            /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<qint8>::dataStreamOut,
                                                            /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<qint8>::dataStreamIn,
                                                            /*.legacyRegisterOp=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<qint8>::legacyRegisterOp,
                                                            /*.size=*/ sizeof(qint8),
                                                            /*.alignment=*/ alignof(qint8),
                                                            /*.typeId=*/ QMetaType::UnknownType,
                                                            /*.flags=*/ QMetaType::TypeFlags(flags),
                                                            original_meta_object,
                                                            nullptr);
                        int _id = metaType.id();
                        registerJavaClassForCustomMetaType(env, metaType, clazz);
                        return _id;
#endif
                    }else /*if(Java::Runtime::Enum::isAssignableFrom(env, clazz)
                         || Java::QtJambi::QtEnumerator::isAssignableFrom(env, clazz))*/ {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                         int typeId = QMetaType::registerNormalizedType(
                                 typeName,
                                 &destructHelper,
                                 findEnumConstructor(sizeof(qint32)),
                                 sizeof(sizeof(qint32)),
                                 flags,
                                 original_meta_object
                             );
                         RegistryAPI::registerComparator(new QtPrivate::BuiltInComparatorFunction<qint32>(), typeId);
                         QmlAPI::registerJavaClassForCustomMetaType(env, typeId, clazz);
                         return typeId;
#else
                         enum E:qint32{};
                         QMetaType metaType = createMetaType(typeName,
                                                             true,
                                                             /*.defaultCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<E>::defaultCtr,
                                                             /*.copyCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<E>::copyCtr,
                                                             /*.moveCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<E>::moveCtr,
                                                             /*.dtor=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<E>::dtor,
                                                             /*.equals=*/ QtPrivate::QEqualityOperatorForType<E>::equals,
                                                             /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<E>::lessThan,
                                                             /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<E>::debugStream,
                                                             /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<E>::dataStreamOut,
                                                             /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<E>::dataStreamIn,
                                                             /*.legacyRegisterOp=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<E>::legacyRegisterOp,
                                                             /*.size=*/ sizeof(E),
                                                             /*.alignment=*/ alignof(E),
                                                             /*.typeId=*/ QMetaType::UnknownType,
                                                             /*.flags=*/ QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<E>::Flags),
                                                             original_meta_object,
                                                             nullptr);
                         int _id = metaType.id();
                         registerJavaClassForCustomMetaType(env, metaType, clazz);
                         return _id;
#endif
                    }
                }
                case EntryTypes::FlagsTypeInfo:{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    int typeId = QMetaType::registerNormalizedType(
                            typeName,
                            &destructHelper,
                            findEnumConstructor(sizeof(qint32)),
                            sizeof(sizeof(qint32)),
                            flags,
                            original_meta_object
                        );
                    RegistryAPI::registerComparator(new QtPrivate::BuiltInComparatorFunction<qint32>(), typeId);
                    QmlAPI::registerJavaClassForCustomMetaType(env, typeId, clazz);
                    return typeId;
#else
                    enum E:qint32{};
                    QMetaType metaType = createMetaType(typeName,
                                                        true,
                                                        /*.defaultCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QFlags<E>>::defaultCtr,
                                                        /*.copyCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QFlags<E>>::copyCtr,
                                                        /*.moveCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QFlags<E>>::moveCtr,
                                                        /*.dtor=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QFlags<E>>::dtor,
                                                        /*.equals=*/ QtPrivate::QEqualityOperatorForType<QFlags<E>>::equals,
                                                        /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<QFlags<E>>::lessThan,
                                                        /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<QFlags<E>>::debugStream,
                                                        /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<QFlags<E>>::dataStreamOut,
                                                        /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<QFlags<E>>::dataStreamIn,
                                                        /*.legacyRegisterOp=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QFlags<E>>::legacyRegisterOp,
                                                        /*.size=*/ sizeof(QFlags<E>),
                                                        /*.alignment=*/ alignof(QFlags<E>),
                                                        /*.typeId=*/ QMetaType::UnknownType,
                                                        /*.flags=*/ QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<QFlags<E>>::Flags),
                                                        original_meta_object,
                                                        nullptr);
                    int _id = metaType.id();
                    registerJavaClassForCustomMetaType(env, metaType, clazz);
                    return _id;
#endif
                }
                case EntryTypes::InterfaceValueTypeInfo:
                case EntryTypes::ValueTypeInfo:{
                    int id = qRegisterMetaType<JObjectWrapper>("JObjectWrapper");
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    int typeId = QMetaType::registerNormalizedTypedef(typeName, id);
                    RegistryAPI::registerComparator(new QtPrivate::BuiltInEqualsComparatorFunction<JObjectWrapper>(), typeId);
                    QmlAPI::registerJavaClassForCustomMetaType(env, typeId, clazz);
                    return typeId;
#else
                    QMetaType::registerNormalizedTypedef(typeName, QMetaType(id));
                    return id;
#endif
                }
                default: break;
                }
            }else{
                const SuperTypeInfos& superTypes = SuperTypeInfos::fromClass(env, clazz);
                QByteArray metaTypeName = javaClassName.toUtf8().replace("/", "::").replace("$", "::");
                if(superTypes.isEmpty()){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    int id;
                    if(Java::QtJambi::QtShortEnumerator::isAssignableFrom(env, clazz)
                            || Java::QtJambi::QtLongEnumerator::isAssignableFrom(env, clazz)
                            || Java::QtJambi::QtByteEnumerator::isAssignableFrom(env, clazz)
                            || Java::QtJambi::QtEnumerator::isAssignableFrom(env, clazz)
                            || Java::Runtime::Enum::isAssignableFrom(env, clazz)){
                        id = qRegisterMetaType<JEnumWrapper>("JEnumWrapper");
                    }else if(Java::Runtime::Map::isAssignableFrom(env, clazz)){
                         id = qRegisterMetaType<JMapWrapper>("JMapWrapper");
                    }else if(Java::Runtime::Collection::isAssignableFrom(env, clazz)){
                         id = qRegisterMetaType<JCollectionWrapper>("JCollectionWrapper");
                    }else if(Java::Runtime::Iterator::isAssignableFrom(env, clazz)){
                         id = qRegisterMetaType<JIteratorWrapper>("JIteratorWrapper");
                    }else{
                        id = qRegisterMetaType<JObjectWrapper>("JObjectWrapper");
                    }
                    int typeId = QMetaType::registerNormalizedTypedef(metaTypeName, id);
                    RegistryAPI::registerComparator(new QtPrivate::BuiltInEqualsComparatorFunction<JObjectWrapper>(), typeId);
                    QmlAPI::registerJavaClassForCustomMetaType(env, typeId, clazz);
                    return typeId;
#else
                    if(Java::Runtime::Enum::isAssignableFrom(env, clazz)){
                        if(Java::QtJambi::QtShortEnumerator::isAssignableFrom(env, clazz)){
                            return qtjambi_register_enum_meta_type<qint16>(env, clazz, javaClassName, metaTypeName);
                        }else if(Java::QtJambi::QtLongEnumerator::isAssignableFrom(env, clazz)){
                            return qtjambi_register_enum_meta_type<qint64>(env, clazz, javaClassName, metaTypeName);
                        }else if(Java::QtJambi::QtByteEnumerator::isAssignableFrom(env, clazz)){
                            return qtjambi_register_enum_meta_type<qint8>(env, clazz, javaClassName, metaTypeName);
                        }else if(Java::QtJambi::QtEnumerator::isAssignableFrom(env, clazz)){
                            return qtjambi_register_enum_meta_type<qint32>(env, clazz, javaClassName, metaTypeName);
                        }else{
                            return qtjambi_register_enum_meta_type<qint32,-1>(env, clazz, javaClassName, metaTypeName);
                        }
                    }else if(Java::QtJambi::QFlags::isAssignableFrom(env, clazz) && !Java::QtJambi::QFlags::isSameClass(env, clazz)){
                        return qtjambi_register_enum_meta_type<qint32,1>(env, clazz, javaClassName, metaTypeName);
                    }else{
                        JObjectValueWrapperPrivate p;
                        p.constructor = env->GetMethodID(clazz, "<init>", "()V");
                        if(env->ExceptionCheck())
                            env->ExceptionClear();
                        p.clone = env->GetMethodID(clazz, "clone", qPrintable(QString("()L%1;").arg(javaClassName)));
                        if(env->ExceptionCheck())
                            env->ExceptionClear();
                        if(!p.clone && p.constructor && Java::Runtime::Cloneable::isAssignableFrom(env, clazz)){
                            jobject newObject = env->NewObject(clazz, p.constructor);
                            if(env->ExceptionCheck())
                                env->ExceptionClear();
                            if(!newObject){
                                p.constructor = nullptr;
                            }else{
                                p.clone = Java::Runtime::Object::clone_ID(env);
                                jobject clonedObject = env->CallObjectMethod(newObject, p.clone);
                                if(env->ExceptionCheck())
                                    env->ExceptionClear();
                                if(!env->IsInstanceOf(clonedObject, clazz))
                                    p.clone = nullptr;
                            }
                        }
                        if(p.constructor && p.clone){
                            p.writeTo = env->GetMethodID(clazz, "writeTo", "(Lio/qt/core/QDataStream;)V");
                            if(env->ExceptionCheck())
                                env->ExceptionClear();
                            p.readFrom = env->GetMethodID(clazz, "readFrom", "(Lio/qt/core/QDataStream;)V");
                            if(env->ExceptionCheck())
                                env->ExceptionClear();
                            if(!p.writeTo && !p.readFrom){
                                p.isSerializable = Java::Runtime::Serializable::isAssignableFrom(env, clazz);
                            }
                            p.clazz = getGlobalClassRef(env, clazz, qPrintable(javaClassName));
                            p.name = metaTypeName;
                            uint flags = QtPrivate::QMetaTypeTypeFlags<JObjectValueWrapper>::Flags;
                            flags |= QMetaType::IsGadget;
                            QtPrivate::QMetaTypeInterface* metaTypeInterface = new QtPrivate::QMetaTypeInterface{
                                    /*.revision=*/ 0,
                                    /*.alignment=*/ alignof(JObjectValueWrapper),
                                    /*.size=*/ sizeof(JObjectValueWrapper),
                                    /*.flags=*/ flags,
                                    /*.typeId=*/ QMetaType::UnknownType,
                                    /*.metaObject=*/ JObjectValueWrapperPrivate::metaObjectFn,
                                    /*.name=*/ p.name.constData(),
                                    /*.defaultCtr=*/ JObjectValueWrapperPrivate::defaultCtr,
                                    /*.copyCtr=*/ JObjectValueWrapperPrivate::copyCtr,
                                    /*.moveCtr=*/ JObjectValueWrapperPrivate::moveCtr,
                                    /*.dtor=*/ JObjectValueWrapperPrivate::dtor,
                                    /*.equals=*/ JObjectValueWrapperPrivate::equals,
                                    /*.lessThan=*/ Java::Runtime::Comparable::isInstanceOf(env, clazz) ? JObjectValueWrapperPrivate::lessThan : nullptr,
                                    /*.debugStream=*/ JObjectValueWrapperPrivate::debugStream,
                                    /*.dataStreamOutFn=*/ p.writeTo ? JObjectValueWrapperPrivate::dataStreamOut : (p.isSerializable ? JObjectValueWrapperPrivate::serializableDataStreamOut : nullptr),
                                    /*.dataStreamInFn=*/ p.readFrom ? JObjectValueWrapperPrivate::dataStreamIn : (p.isSerializable ? JObjectValueWrapperPrivate::serializableDataStreamIn : nullptr),
                                    /*.legacyRegisterOp=*/ nullptr
                            };
                            {
                                QWriteLocker locker(gLock());
                                if(const QtPrivate::QMetaTypeInterface * iface = gClassMetaTypeInterfaces->value(qHash(javaClassName))){
                                    delete metaTypeInterface;
                                    return iface->typeId;
                                }
                                gClassMetaTypeInterfaces->insert(qHash(javaClassName), metaTypeInterface);
                                (*gMetaTypeInterfaces)[metaTypeInterface] = std::move(p);
                            }
                            QMetaType metaType(metaTypeInterface);
                            int result = metaType.id();
                            QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                                if(src){
                                    new (target)JObjectWrapper(*reinterpret_cast<const JObjectValueWrapper*>(src));
                                }else{
                                    new (target)JObjectWrapper();
                                }
                                return true;
                            }, metaType, QMetaType::fromType<JObjectWrapper>());
                            QMetaType::registerConverterFunction([metaType](const void *src, void *target) -> bool {
                                if(src){
                                    jobject value = reinterpret_cast<const JObjectWrapper*>(src)->object();
                                    if(value!=nullptr){
                                        if(JniEnvironment env{200}){
                                            const JObjectValueWrapperPrivate* methods{nullptr};
                                            {
                                                QReadLocker locker(gLock());
                                                if(gMetaTypeInterfaces->contains(QMetaType(metaType).iface()))
                                                    methods = &(*gMetaTypeInterfaces)[QMetaType(metaType).iface()];
                                            }
                                            if(!methods || !env->IsInstanceOf(value, methods->clazz))
                                                return false;
                                            new (target)JObjectValueWrapper(JObjectValueWrapper::create(env, *reinterpret_cast<const JObjectWrapper*>(src), metaType));
                                            return true;
                                        }else{
                                            return false;
                                        }
                                    }
                                }
                                new (target)JObjectValueWrapper();
                                return true;
                            }, QMetaType::fromType<JObjectWrapper>(), metaType);
                            registerConverterVariant(env, metaType, QLatin1String(metaTypeInterface->name), javaClassName, clazz);
                            registerJavaClassForCustomMetaType(env, metaType, clazz);
                            return result;
                        }
                        if(Java::Runtime::Collection::isAssignableFrom(env, clazz)){
                            return QMetaType::fromType<JCollectionWrapper>().id();
                        }else if(Java::Runtime::Map::isAssignableFrom(env, clazz)){
                            return QMetaType::fromType<JMapWrapper>().id();
                        }
                        QMetaType metaType = createMetaType(metaTypeName+"*",
                                                            true,
                                                            /*.defaultCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<JObjectWrapper>::defaultCtr,
                                                            /*.copyCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<JObjectWrapper>::copyCtr,
                                                            /*.moveCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<JObjectWrapper>::moveCtr,
                                                            /*.dtor=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<JObjectWrapper>::dtor,
                                                            /*.equals=*/ QtPrivate::QEqualityOperatorForType<JObjectWrapper>::equals,
                                                            /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<JObjectWrapper>::lessThan,
                                                            /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<JObjectWrapper>::debugStream,
                                                            /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<JObjectWrapper>::dataStreamOut,
                                                            /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<JObjectWrapper>::dataStreamIn,
                                                            /*.legacyRegisterOp=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<JObjectWrapper>::legacyRegisterOp,
                                                            /*.size=*/ sizeof(JObjectWrapper),
                                                            /*.alignment=*/ alignof(JObjectWrapper),
                                                            /*.typeId=*/ QMetaType::UnknownType,
                                                            /*.flags=*/ QMetaType::TypeFlags(
                                                                QtPrivate::QMetaTypeTypeFlags<JObjectWrapper>::Flags
                                                                | QMetaType::IsGadget
                                                                ),
                                                            nullptr, &findWrappersMetaObject);
                        int result = metaType.id();
                        registerJavaClassForCustomMetaType(env, metaType, clazz, true);
                        QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                            if(src){
                                new (target)JObjectWrapper(*reinterpret_cast<const JObjectWrapper*>(src));
                            }else{
                                new (target)JObjectWrapper();
                            }
                            return true;
                        }, metaType, QMetaType::fromType<JObjectWrapper>());
                        JObjectWrapper classWrapper(env, clazz);
                        QMetaType::registerConverterFunction([classWrapper](const void *src, void *target) -> bool {
                            if(src){
                                jobject value = reinterpret_cast<const JObjectWrapper*>(src)->object();
                                if(value!=nullptr){
                                    if(JniEnvironment env{200}){
                                        if(!env->IsInstanceOf(value, jclass(classWrapper.object())))
                                            return false;
                                    }
                                }
                                new (target)JObjectWrapper(*reinterpret_cast<const JObjectWrapper*>(src));
                            }else{
                                new (target)JObjectWrapper();
                            }
                            return true;
                        }, QMetaType::fromType<JObjectWrapper>(), metaType);
                        return result;
                    }
#endif
                }else{
                    int id = registerMetaType(env, superTypes[0].javaClass(), isPointer, isReference, &superTypes[0].className());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    int typeId = QMetaType::registerNormalizedTypedef(metaTypeName, id);
                    RegistryAPI::registerComparator(new QtPrivate::BuiltInEqualsComparatorFunction<JObjectWrapper>(), typeId);
                    return typeId;
#else
                    if(id!=QMetaType::UnknownType){
                        QMetaType superMetaType(id);
                        if(superMetaType.flags() & QMetaType::IsPointer){
                            metaTypeName += "*";
                            QMetaType metaType = createMetaType(metaTypeName,
                                                                true,
                                                                /*.defaultCtr=*/ superMetaType.iface()->defaultCtr,
                                                                /*.copyCtr=*/ superMetaType.iface()->copyCtr,
                                                                /*.moveCtr=*/ superMetaType.iface()->moveCtr,
                                                                /*.dtor=*/ superMetaType.iface()->dtor,
                                                                /*.equals=*/ superMetaType.iface()->equals,
                                                                /*.lessThan=*/ superMetaType.iface()->lessThan,
                                                                /*.debugStream=*/ superMetaType.iface()->debugStream,
                                                                /*.dataStreamOut=*/ superMetaType.iface()->dataStreamOut,
                                                                /*.dataStreamIn=*/ superMetaType.iface()->dataStreamIn,
                                                                /*.legacyRegisterOp=*/ superMetaType.iface()->legacyRegisterOp,
                                                                /*.size=*/ superMetaType.iface()->size,
                                                                /*.alignment=*/ superMetaType.iface()->alignment,
                                                                /*.typeId=*/ QMetaType::UnknownType,
                                                                /*.flags=*/ QMetaType::TypeFlags(superMetaType.iface()->flags),
                                                                nullptr, &findWrappersMetaObject);
                            id = metaType.id();
                            registerJavaClassForCustomMetaType(env, metaType, clazz);
                            registerConverterVariant(env, metaType, QLatin1String(metaTypeName), javaClassName, clazz);
                            return id;
                        }else{
                            JObjectValueWrapperPrivate p;
                            p.constructor = env->GetMethodID(clazz, "<init>", "()V");
                            if(env->ExceptionCheck())
                                env->ExceptionClear();
                            p.clone = env->GetMethodID(clazz, "clone", qPrintable(QString("()L%1;").arg(javaClassName)));
                            if(env->ExceptionCheck())
                                env->ExceptionClear();
                            if(!p.clone && p.constructor && Java::Runtime::Cloneable::isAssignableFrom(env, clazz)){
                                jobject newObject = env->NewObject(clazz, p.constructor);
                                if(env->ExceptionCheck())
                                    env->ExceptionClear();
                                if(!newObject){
                                    p.constructor = nullptr;
                                }else{
                                    p.clone = Java::Runtime::Object::clone_ID(env);
                                    jobject clonedObject = env->CallObjectMethod(newObject, p.clone);
                                    if(env->ExceptionCheck())
                                        env->ExceptionClear();
                                    if(!env->IsInstanceOf(clonedObject, clazz))
                                        p.clone = nullptr;
                                }
                            }
                            if(p.constructor && p.clone){
                                p.writeTo = env->GetMethodID(clazz, "writeTo", "(Lio/qt/core/QDataStream;)V");
                                if(env->ExceptionCheck())
                                    env->ExceptionClear();
                                p.readFrom = env->GetMethodID(clazz, "readFrom", "(Lio/qt/core/QDataStream;)V");
                                if(env->ExceptionCheck())
                                    env->ExceptionClear();
                                if(!p.writeTo && !p.readFrom){
                                    p.isSerializable = Java::Runtime::Serializable::isAssignableFrom(env, clazz);
                                }
                                p.clazz = getGlobalClassRef(env, clazz, qPrintable(javaClassName));
                                p.name = metaTypeName;
                                uint flags = QtPrivate::QMetaTypeTypeFlags<JObjectValueWrapper>::Flags;
                                flags |= QMetaType::IsGadget;
                                QtPrivate::QMetaTypeInterface* metaTypeInterface = new QtPrivate::QMetaTypeInterface{
                                        /*.revision=*/ 0,
                                        /*.alignment=*/ alignof(JObjectValueWrapper),
                                        /*.size=*/ sizeof(JObjectValueWrapper),
                                        /*.flags=*/ flags,
                                        /*.typeId=*/ QMetaType::UnknownType,
                                        /*.metaObject=*/ JObjectValueWrapperPrivate::metaObjectFn,
                                        /*.name=*/ p.name.constData(),
                                        /*.defaultCtr=*/ JObjectValueWrapperPrivate::defaultCtr,
                                        /*.copyCtr=*/ JObjectValueWrapperPrivate::copyCtr,
                                        /*.moveCtr=*/ JObjectValueWrapperPrivate::moveCtr,
                                        /*.dtor=*/ JObjectValueWrapperPrivate::dtor,
                                        /*.equals=*/ JObjectValueWrapperPrivate::equals,
                                        /*.lessThan=*/ Java::Runtime::Comparable::isInstanceOf(env, clazz) ? JObjectValueWrapperPrivate::lessThan : nullptr,
                                        /*.debugStream=*/ JObjectValueWrapperPrivate::debugStream,
                                        /*.dataStreamOutFn=*/ p.writeTo ? JObjectValueWrapperPrivate::dataStreamOut : (p.isSerializable ? JObjectValueWrapperPrivate::serializableDataStreamOut : nullptr),
                                        /*.dataStreamInFn=*/ p.readFrom ? JObjectValueWrapperPrivate::dataStreamIn : (p.isSerializable ? JObjectValueWrapperPrivate::serializableDataStreamIn : nullptr),
                                        /*.legacyRegisterOp=*/ nullptr
                                };
                                {
                                    QWriteLocker locker(gLock());
                                    if(const QtPrivate::QMetaTypeInterface * iface = gClassMetaTypeInterfaces->value(qHash(javaClassName))){
                                        delete metaTypeInterface;
                                        return iface->typeId;
                                    }
                                    gClassMetaTypeInterfaces->insert(qHash(javaClassName), metaTypeInterface);
                                    (*gMetaTypeInterfaces)[metaTypeInterface] = std::move(p);
                                }
                                QMetaType metaType(metaTypeInterface);
                                int result = metaType.id();
                                QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                                    if(src){
                                        new (target)JObjectWrapper(*reinterpret_cast<const JObjectValueWrapper*>(src));
                                    }else{
                                        new (target)JObjectWrapper();
                                    }
                                    return true;
                                }, metaType, QMetaType::fromType<JObjectWrapper>());
                                QMetaType::registerConverterFunction([metaType](const void *src, void *target) -> bool {
                                    if(src){
                                        jobject value = reinterpret_cast<const JObjectWrapper*>(src)->object();
                                        if(value!=nullptr){
                                            if(JniEnvironment env{200}){
                                                const JObjectValueWrapperPrivate* methods{nullptr};
                                                {
                                                    QReadLocker locker(gLock());
                                                    if(gMetaTypeInterfaces->contains(QMetaType(metaType).iface()))
                                                        methods = &(*gMetaTypeInterfaces)[QMetaType(metaType).iface()];
                                                }
                                                if(!methods || !env->IsInstanceOf(value, methods->clazz))
                                                    return false;
                                                new (target)JObjectValueWrapper(JObjectValueWrapper::create(env, *reinterpret_cast<const JObjectWrapper*>(src), metaType));
                                                return true;
                                            }else{
                                                return false;
                                            }
                                        }
                                    }
                                    new (target)JObjectValueWrapper();
                                    return true;
                                }, QMetaType::fromType<JObjectWrapper>(), metaType);
                                registerConverterVariant(env, metaType, QLatin1String(metaTypeInterface->name), javaClassName, clazz);
                                registerJavaClassForCustomMetaType(env, metaType, clazz);
                                return result;
                            }
                            if(Java::Runtime::Collection::isAssignableFrom(env, clazz)){
                                return QMetaType::fromType<JCollectionWrapper>().id();
                            }else if(Java::Runtime::Map::isAssignableFrom(env, clazz)){
                                return QMetaType::fromType<JMapWrapper>().id();
                            }
                            QMetaType metaType = createMetaType(metaTypeName+"*",
                                                                true,
                                                                /*.defaultCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<JObjectWrapper>::defaultCtr,
                                                                /*.copyCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<JObjectWrapper>::copyCtr,
                                                                /*.moveCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<JObjectWrapper>::moveCtr,
                                                                /*.dtor=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<JObjectWrapper>::dtor,
                                                                /*.equals=*/ QtPrivate::QEqualityOperatorForType<JObjectWrapper>::equals,
                                                                /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<JObjectWrapper>::lessThan,
                                                                /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<JObjectWrapper>::debugStream,
                                                                /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<JObjectWrapper>::dataStreamOut,
                                                                /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<JObjectWrapper>::dataStreamIn,
                                                                /*.legacyRegisterOp=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<JObjectWrapper>::legacyRegisterOp,
                                                                /*.size=*/ sizeof(JObjectWrapper),
                                                                /*.alignment=*/ alignof(JObjectWrapper),
                                                                /*.typeId=*/ QMetaType::UnknownType,
                                                                /*.flags=*/ QMetaType::TypeFlags(
                                                                    QtPrivate::QMetaTypeTypeFlags<JObjectWrapper>::Flags
                                                                        | QMetaType::IsGadget
                                                                        ),
                                                                nullptr, &findWrappersMetaObject);
                            int result = metaType.id();
                            registerJavaClassForCustomMetaType(env, metaType, clazz, true);
                            QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                                if(src){
                                    new (target)JObjectWrapper(*reinterpret_cast<const JObjectWrapper*>(src));
                                }else{
                                    new (target)JObjectWrapper();
                                }
                                return true;
                            }, metaType, QMetaType::fromType<JObjectWrapper>());
                            JObjectWrapper classWrapper(env, clazz);
                            QMetaType::registerConverterFunction([classWrapper](const void *src, void *target) -> bool {
                                if(src){
                                    jobject value = reinterpret_cast<const JObjectWrapper*>(src)->object();
                                    if(value!=nullptr){
                                        if(JniEnvironment env{200}){
                                            if(!env->IsInstanceOf(value, jclass(classWrapper.object())))
                                                return false;
                                        }
                                    }
                                    new (target)JObjectWrapper(*reinterpret_cast<const JObjectWrapper*>(src));
                                }else{
                                    new (target)JObjectWrapper();
                                }
                                return true;
                            }, QMetaType::fromType<JObjectWrapper>(), metaType);
                            return result;
                        }
                    }else{
                        if(Java::Runtime::Collection::isAssignableFrom(env, clazz)){
                            return QMetaType::fromType<JCollectionWrapper>().id();
                        }else if(Java::Runtime::Map::isAssignableFrom(env, clazz)){
                            return QMetaType::fromType<JMapWrapper>().id();
                        }
                        QMetaType metaType = createMetaType(metaTypeName+"*",
                                                            true,
                                                            /*.defaultCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<JObjectWrapper>::defaultCtr,
                                                            /*.copyCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<JObjectWrapper>::copyCtr,
                                                            /*.moveCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<JObjectWrapper>::moveCtr,
                                                            /*.dtor=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<JObjectWrapper>::dtor,
                                                            /*.equals=*/ QtPrivate::QEqualityOperatorForType<JObjectWrapper>::equals,
                                                            /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<JObjectWrapper>::lessThan,
                                                            /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<JObjectWrapper>::debugStream,
                                                            /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<JObjectWrapper>::dataStreamOut,
                                                            /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<JObjectWrapper>::dataStreamIn,
                                                            /*.legacyRegisterOp=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<JObjectWrapper>::legacyRegisterOp,
                                                            /*.size=*/ sizeof(JObjectWrapper),
                                                            /*.alignment=*/ alignof(JObjectWrapper),
                                                            /*.typeId=*/ QMetaType::UnknownType,
                                                            /*.flags=*/ QMetaType::TypeFlags(
                                                                            QtPrivate::QMetaTypeTypeFlags<JObjectWrapper>::Flags
                                                                            | QMetaType::IsGadget
                                                                    ),
                                                            nullptr, &findWrappersMetaObject);
                        int result = metaType.id();
                        registerJavaClassForCustomMetaType(env, metaType, clazz, true);
                        QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                            if(src){
                                new (target)JObjectWrapper(*reinterpret_cast<const JObjectWrapper*>(src));
                            }else{
                                new (target)JObjectWrapper();
                            }
                            return true;
                        }, metaType, QMetaType::fromType<JObjectWrapper>());
                        JObjectWrapper classWrapper(env, clazz);
                        QMetaType::registerConverterFunction([classWrapper](const void *src, void *target) -> bool {
                            if(src){
                                jobject value = reinterpret_cast<const JObjectWrapper*>(src)->object();
                                if(value!=nullptr){
                                    if(JniEnvironment env{200}){
                                        if(!env->IsInstanceOf(value, jclass(classWrapper.object())))
                                            return false;
                                    }
                                }
                                new (target)JObjectWrapper(*reinterpret_cast<const JObjectWrapper*>(src));
                            }else{
                                new (target)JObjectWrapper();
                            }
                            return true;
                        }, QMetaType::fromType<JObjectWrapper>(), metaType);
                        return result;
                    }
#endif
                }
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return QMetaType::UnknownType;
}

int registerMetaType(JNIEnv *env, jclass clazz, jboolean isPointer, jboolean isReference){
    return registerMetaType(env, clazz, isPointer, isReference, nullptr);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
int QmlAPI::registerMetaType(JNIEnv *env, jclass clazz, const QString& javaClassName){
    return registerMetaType(env, clazz, false, false, &javaClassName);
}
#endif
