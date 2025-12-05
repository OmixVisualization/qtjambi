/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED

#include "pch_p.h"
#include <QtCore/private/qcoreapplication_p.h>
#include <QtCore/private/qmetaobject_p.h>
#include <QtCore/private/qmetaobjectbuilder_p.h>

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
#define qAsConst std::as_const
#endif

Q_GLOBAL_STATIC(QReadWriteLock, gRegistryLock)
typedef SecureContainer<QMap<size_t, EntryTypes>, gRegistryLock> TypeEntryTypesHash;
typedef SecureContainer<QMap<size_t, const char*>, gRegistryLock> TypeStringHash;
typedef SecureContainer<QMap<size_t, RegistryAPI::QtJambiTypeInfo>, gRegistryLock> TypeIdToQTypeInfoHash;
typedef SecureContainer<QHash<QByteArray, const char*>, gRegistryLock> StringStringHash;
typedef SecureContainer<QHash<QByteArray, const std::type_info*>, gRegistryLock> StringTypeHash;
typedef SecureContainer<QHash<QByteArray, QList<const std::type_info*>>, gRegistryLock> StringTypesHash;
Q_GLOBAL_STATIC(TypeStringHash, gTypeQtNameHash)
Q_GLOBAL_STATIC(TypeIdToQTypeInfoHash, gQTypeInfoHash)
Q_GLOBAL_STATIC(TypeEntryTypesHash, gEntryTypesHash)
Q_GLOBAL_STATIC(StringTypeHash, gQtNameTypeHash)
Q_GLOBAL_STATIC(StringStringHash, gQtFunctionalJavaNameHash)
Q_GLOBAL_STATIC(StringStringHash, gQtFunctionalQtNameHash)
Q_GLOBAL_STATIC(StringStringHash, gQtNamespaceJavaNameHash)
Q_GLOBAL_STATIC(StringStringHash, gJavaNamespaceQtNameHash)
typedef SecureContainer<QMap<QByteArray, const QMetaObject*>, gRegistryLock> NamespaceMetaObjectHash;
Q_GLOBAL_STATIC(NamespaceMetaObjectHash, gNamespaceMetaObjectHash)
Q_GLOBAL_STATIC(TypeStringHash, gTypeJavaNameHash)
Q_GLOBAL_STATIC(StringTypesHash, gJavaNameTypeHash)
typedef SecureContainer<QSet<size_t>, gRegistryLock> TypeSet;
Q_GLOBAL_STATIC(StringTypeHash, gIIDTypeHash)
Q_GLOBAL_STATIC(StringStringHash, gJavaClassIIDHash)
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
typedef SecureContainer<QMap<QByteArray,QPair<const char*, int>>, gRegistryLock> NativeInterfaceMap;
Q_GLOBAL_STATIC(NativeInterfaceMap, gNativeInterfaceMap)
#endif
typedef SecureContainer<QHash<QByteArray, QByteArray>, gRegistryLock> IIDByteArrayHash;
Q_GLOBAL_STATIC(IIDByteArrayHash, gIIDByteArrayHash)
typedef SecureContainer<QMap<size_t, const std::type_info*>, gRegistryLock> ID2IDHash;
Q_GLOBAL_STATIC(ID2IDHash, gFlagEnumIDHash)
Q_GLOBAL_STATIC(ID2IDHash, gEnumFlagIDHash)
Q_GLOBAL_STATIC(TypeStringHash, gTypeJavaInterfaceHash)
typedef SecureContainer<QMap<size_t, NewContainerAccessFunction>, gRegistryLock> SequentialContainerAccessFactoryHash;
Q_GLOBAL_STATIC(SequentialContainerAccessFactoryHash, gSequentialContainerAccessFactoryHash)

typedef SecureContainer<QHash<int, QHash<int,std::function<AbstractContainerAccess*()>>>, gRegistryLock> SequentialContainerAccessFactoryByMetaTypeHash;
Q_GLOBAL_STATIC(SequentialContainerAccessFactoryByMetaTypeHash, gSequentialContainerAccessFactoryByMetaTypeHash)
typedef SecureContainer<QHash<QPair<int,int>, QHash<int,std::function<AbstractContainerAccess*()>>>, gRegistryLock> MapAccessFactoryByMetaTypeHash;
Q_GLOBAL_STATIC(MapAccessFactoryByMetaTypeHash, gMapAccessFactoryByMetaTypeHash)
typedef SecureContainer<QHash<int, const std::type_info*>, gRegistryLock> MetaTypeIDHash;
typedef SecureContainer<QMap<size_t, int>, gRegistryLock> MetaTypeIDMap;
Q_GLOBAL_STATIC(MetaTypeIDMap, gMetaTypeIDMap)
Q_GLOBAL_STATIC(MetaTypeIDHash, gMetaTypeIDHash)
typedef SecureContainer<QMap<size_t, const void*>, gRegistryLock> DefaultValueHash;
Q_GLOBAL_STATIC(DefaultValueHash, gDefaultValueHash)
typedef SecureContainer<QMap<size_t, QVector<RegistryAPI::FunctionInfo>>, gRegistryLock> FunctionInfoHash;
Q_GLOBAL_STATIC(FunctionInfoHash, gVirtualFunctionInfos)
typedef SecureContainer<QMap<size_t, QVector<RegistryAPI::ConstructorInfo>>, gRegistryLock> ConstructorInfoHash;
Q_GLOBAL_STATIC(ConstructorInfoHash, gConstructorInfos)
typedef SecureContainer<QMap<size_t, uint>, gRegistryLock> ReturnScopeHash;
Q_GLOBAL_STATIC(ReturnScopeHash, gReturnScopes)
typedef SecureContainer<QMap<size_t, RegistryAPI::DestructorFn>, gRegistryLock> DestructorHash;
Q_GLOBAL_STATIC(DestructorHash, gDestructorHash)
typedef SecureContainer<QHash<size_t, const char*>, gRegistryLock> NameHash;
Q_GLOBAL_STATIC(NameHash, gFlagEnumNameHash)
Q_GLOBAL_STATIC(NameHash, gInterfaceHash)
Q_GLOBAL_STATIC(NameHash, gInterfaceIIDsHash)
typedef SecureContainer<QSet<size_t>, gRegistryLock> HashSet;
Q_GLOBAL_STATIC(HashSet, gFunctionalHash)
typedef SecureContainer<QMap<size_t, PtrDeleterFunction>, gRegistryLock> DeleterHash;
Q_GLOBAL_STATIC(DeleterHash, gDeleterHash)
typedef SecureContainer<QMap<int,QSharedPointer<AbstractContainerAccess>>, gRegistryLock> ContainerAccessMap;
Q_GLOBAL_STATIC(ContainerAccessMap, gContainerAccessMap)
typedef SecureContainer<QHash<const QtPrivate::QMetaTypeInterface *, jclass>, gRegistryLock> MetaTypeJavaTypeHash;
typedef SecureContainer<QHash<const QtPrivate::QMetaTypeInterface *, const char*>, gRegistryLock> MetaTypeJavaNameHash;
typedef SecureContainer<QMultiHash<QByteArray,QMetaType>, gRegistryLock> JavaTypeMetaTypesHash;
typedef SecureContainer<QSet<const QtPrivate::QMetaTypeInterface *>, gRegistryLock> JObjectWrappedMetaTypeHash;
typedef SecureContainer<QHash<int, const QtPrivate::QMetaTypeInterface *>, gRegistryLock> JObjectNativeWrappedMetaTypeHash;
typedef SecureContainer<QHash<const QtPrivate::QMetaTypeInterface *, const QtPrivate::QMetaTypeInterface *>, gRegistryLock> JObjectNativeWrappedMetaTypeReverseHash;
Q_GLOBAL_STATIC(JObjectNativeWrappedMetaTypeHash, gJObjectNativeWrapperMetaTypes)
Q_GLOBAL_STATIC(JObjectNativeWrappedMetaTypeReverseHash, gMetaTypesForJObjectNativeWrappers)
Q_GLOBAL_STATIC(MetaTypeJavaTypeHash, gMetaTypeJavaTypeHash)
Q_GLOBAL_STATIC(MetaTypeJavaNameHash, gMetaTypeJavaNameHash)
Q_GLOBAL_STATIC(JavaTypeMetaTypesHash, gJavaTypeMetaTypesHash)
Q_GLOBAL_STATIC(JObjectWrappedMetaTypeHash, gJObjectWrappedMetaTypes)

typedef SecureContainer<QMap<size_t, PtrOwnerFunction>, gRegistryLock> OwnerFunctionHash;
Q_GLOBAL_STATIC(OwnerFunctionHash, gOwnerFunctionHash)

typedef SecureContainer<QMultiHash<size_t, const PolymorphicIdHandler* >, gRegistryLock> PolymorphicIdHash;
Q_GLOBAL_STATIC(PolymorphicIdHash, g_polymorphic_ids)
typedef SecureContainer<QMultiMap<size_t, const std::type_info*>, gRegistryLock> PolymorphicBasesHash;
Q_GLOBAL_STATIC(PolymorphicBasesHash, gPolymorphicBasesHash)

typedef SecureContainer<QMap<size_t, RegistryAPI::qHashFn>, gRegistryLock> HashFunctionHash;
typedef SecureContainer<QMap<size_t, size_t>, gRegistryLock> SizeHash;
typedef SecureContainer<QMap<size_t, QMap<size_t, int>>, gRegistryLock> OffsetHash;
typedef SecureContainer<QMap<size_t, QSet<const std::type_info*>>, gRegistryLock> InterfacesHash;
Q_GLOBAL_STATIC(SizeHash, gValueSizeHash)
Q_GLOBAL_STATIC(SizeHash, gValueAlignmentHash)
Q_GLOBAL_STATIC(SizeHash, gShellSizeHash)
Q_GLOBAL_STATIC(HashFunctionHash, gHashFunctionHash)
Q_GLOBAL_STATIC(OffsetHash, gOffsetHash)
Q_GLOBAL_STATIC(InterfacesHash, gInterfacesHash)

typedef SecureContainer<QHash<uint, InterfaceOffsetInfo >, gRegistryLock> InterfaceOffsetHash;
Q_GLOBAL_STATIC(InterfaceOffsetHash, gInterfaceOffsetHash)

typedef SecureContainer<QMap<size_t, const QMetaObject*>, gRegistryLock> MetaObjectHash;
Q_GLOBAL_STATIC(MetaObjectHash, gOriginalMetaObjectHash)
Q_GLOBAL_STATIC(MetaObjectHash, gHasCustomMetaObjectHash)
typedef SecureContainer<QHash<const QMetaObject*, QVector<RegistryAPI::SignalMetaInfo> >, gRegistryLock> SignalMetaInfoHash;
Q_GLOBAL_STATIC(SignalMetaInfoHash, gSignalMetaInfos)
typedef SecureContainer<QHash<const QMetaObject*, QList<QMetaMethod> >, gRegistryLock> ExtraSignalsOfMetaObjectHash;
Q_GLOBAL_STATIC(ExtraSignalsOfMetaObjectHash, gExtraSignalsOfMetaObject)
typedef SecureContainer<QHash<const QMetaObject*, const std::type_info* >, gRegistryLock> TypeIdsByMetaObjectHash;
Q_GLOBAL_STATIC(TypeIdsByMetaObjectHash, gTypeIdsByMetaObject)
typedef SecureContainer<QHash<const QMetaObject*, RegistryAPI::QMetaMethodRenamerFn>, gRegistryLock> QMetaMethodRenamerFnByMetaObjectHash;
Q_GLOBAL_STATIC(QMetaMethodRenamerFnByMetaObjectHash, gQMetaMethodRenamerFnByMetaObject)
typedef SecureContainer<QSet<const QMetaObject*>, gRegistryLock> ValueOwnerSet;
Q_GLOBAL_STATIC(ValueOwnerSet, gValueOwners)
typedef SecureContainer<QHash<const QMetaObject*,bool>, gRegistryLock> ValueOwnerHash;
Q_GLOBAL_STATIC(ValueOwnerHash, gValueOwnerHash)
typedef SecureContainer<QHash<const QMetaObject*, RegistryAPI::ParameterInfoProviderFn >, gRegistryLock> ParameterTypeInfoProviderHash;
Q_GLOBAL_STATIC(ParameterTypeInfoProviderHash, gParameterTypeInfoProviders)
typedef SecureContainer<QHash<const QMetaObject*, QHash<int,const char*>>, gRegistryLock> RenamedMethodsHash;
Q_GLOBAL_STATIC(RenamedMethodsHash, gRenamedMethodsHash)

typedef SecureContainer<QMap<size_t, FunctionalResolver>, gRegistryLock> FunctionalResolverHash;
Q_GLOBAL_STATIC(FunctionalResolverHash, gFunctionalResolverHash)

typedef SecureContainer<QMap<size_t, TypeInfoSupplier>, gRegistryLock> TypeInfoSupplierHash;
Q_GLOBAL_STATIC(TypeInfoSupplierHash, gTypeInfoSupplierHash)

Q_GLOBAL_STATIC(TypeStringHash, gMediaControlIIDHash)
Q_GLOBAL_STATIC(StringTypeHash, gMediaControlIIDClassHash)

typedef SecureContainer<QHash<size_t,jmethodID>, gRegistryLock> ConstructorIDHash;
Q_GLOBAL_STATIC(ConstructorIDHash, gConstructorHash)

#ifdef QTJAMBI_LOG_CLASSNAMES
typedef SecureContainer<QHash<size_t, QString>, gRegistryLock> ClassNameHash;
Q_GLOBAL_STATIC(ClassNameHash, gClassNameHash)
#endif

typedef SecureContainer<QHash<size_t, jclass>, gRegistryLock> ClassIdHash;
Q_GLOBAL_STATIC(ClassIdHash, gClassHash)
typedef SecureContainer<QHash<size_t, jfieldID>, gRegistryLock> FieldIdHash;
Q_GLOBAL_STATIC(FieldIdHash, gFieldHash)
typedef SecureContainer<QHash<size_t, jmethodID>, gRegistryLock> MethodIdHash;
Q_GLOBAL_STATIC(MethodIdHash, gMethodHash)
typedef SecureContainer<QHash<size_t, jclass>, gRegistryLock> ClassHash;
Q_GLOBAL_STATIC(ClassHash, gQtSuperclassHash)

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
typedef SecureContainer<QHash<const QtPrivate::QMetaTypeInterface*,const QMetaObject*>, gRegistryLock> MetaTypeMetaObjectHash;
Q_GLOBAL_STATIC(MetaTypeMetaObjectHash, gMetaTypeMetaObjectHash)
typedef SecureContainer<QSet<QByteArray>, gRegistryLock> TypeNameSet;
Q_GLOBAL_STATIC(TypeNameSet, gTypeNames)
#endif

#ifdef QTJAMBI_COUNTCACHEMISSES
static void cacheMisses(const char *s)
{
    static int count = 0;

    qCDebug(DebugAPI::internalCategory, "Total # of cache misses: %d : '%s'", ++count, s);
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

size_t qHash(const char *p, size_t seed) Q_DECL_NOEXCEPT
{
    size_t h = seed;
    size_t g;

    if(p){
        while (*p != 0) {
            h = (h << 4) + size_t(*p++);
            if ((g = (h & 0xf0000000)) != 0)
                h ^= g >> 23;
            h &= ~g;
        }
    }
    return h;
}

namespace std{
size_t qHash(const type_index& idx, size_t seed) Q_DECL_NOEXCEPT
{
    return ::qHash(unique_id(idx), seed);
}

size_t qHash(const type_index& idx) Q_DECL_NOEXCEPT
{
    return ::qHash(unique_id(idx));
}
}

void registerTypeAlias(const std::type_info& typeId, const char *qt_name, const char *java_name)
{
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    if(qt_name){
        QByteArray _qt_name(qt_name);
        if(!gQtNameTypeHash->contains(_qt_name))
            gQtNameTypeHash->insert(_qt_name, &typeId);
    }
    if(java_name)
        (*gJavaNameTypeHash)[java_name].append(&typeId);
}

void registerTypeInfo(const std::type_info& typeId, RegistryAPI::QtJambiTypeInfo info, const char *qt_name, const char *java_name, EntryTypes entryTypes)
{
    {
        QWriteLocker locker(gRegistryLock());
        Q_UNUSED(locker)
        if(!gTypeQtNameHash->contains(unique_id(typeId))){
            gTypeQtNameHash->insert(unique_id(typeId), qt_name);
        }
        if(entryTypes==EntryTypes::FunctionalTypeInfo 
                && !gQTypeInfoHash->contains(unique_id(typeId))){
            gQTypeInfoHash->insert(unique_id(typeId), info);
        }
        gQtNameTypeHash->insert(qt_name, &typeId);

        if(!gTypeJavaNameHash->contains(unique_id(typeId))){
            gTypeJavaNameHash->insert(unique_id(typeId), java_name);
        }
        (*gJavaNameTypeHash)[java_name].append(&typeId);
        if(entryTypes!=EntryTypes::Unspecific)
            gEntryTypesHash->insert(unique_id(typeId), entryTypes);
        if(entryTypes==EntryTypes::FunctionalTypeInfo){
            gQtFunctionalJavaNameHash->insert(qt_name, java_name);
            gQtFunctionalQtNameHash->insert(java_name, qt_name);
        }
    }
    QByteArray type_name = QtJambiAPI::typeName(typeId);
    if(type_name!=QByteArray(qt_name)){
        registerTypeAlias(typeId, type_name, nullptr);
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
void RegistryAPI::registerNativeInterface(const char* className, QPair<const char*, int>&& nameAndRevision){
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    gNativeInterfaceMap()->insert(className, std::move(nameAndRevision));
}

bool registeredNativeInterface(JNIEnv* env, jclass cls, QPair<const char*, int>& nameAndRevision){
    QByteArray className = QtJambiAPI::getClassName(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')).toUtf8();
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
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

void RegistryAPI::registerInterfaceTypeInfo(const std::type_info& typeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name, const char *interface_iid)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::InterfaceTypeInfo);
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    gIIDTypeHash->insert(interface_iid, &typeId);
    gJavaClassIIDHash->insert(java_name, interface_iid);
}

void RegistryAPI::registerInterfaceID(const std::type_info& typeId, const char *interface_iid)
{
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    gIIDTypeHash->insert(interface_iid, &typeId);
    gJavaClassIIDHash->insert(gTypeJavaNameHash->value(unique_id(typeId), nullptr), interface_iid);
}

const char * registeredInterfaceID(const std::type_info& typeId)
{
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gJavaClassIIDHash->value(gTypeJavaNameHash->value(unique_id(typeId), nullptr));
}

const char * registeredInterfaceIDForClassName(const QString& className)
{
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gJavaClassIIDHash->value(className.toLocal8Bit());
}

const char * RegistryAPI::registerInterfaceID(JNIEnv* env, jclass cls)
{
    cls = getGlobalClassRef(env, cls);
    QString iid = QtJambiAPI::getClassName(env, cls);
    QByteArray className = iid.toLatin1().replace('.', '/');
    {
        QReadLocker locker(gRegistryLock());
        if(gJavaClassIIDHash->contains(className))
            return (*gJavaClassIIDHash)[className];
    }
    {
        QWriteLocker locker(gRegistryLock());
        Q_UNUSED(locker)
        if(!gJavaClassIIDHash->contains(className)){
            const char* _iid = gIIDByteArrayHash->insert(iid.toLatin1(), className).key();
            gJavaClassIIDHash->insert(className, _iid);
            return _iid;
        }else{
            return (*gJavaClassIIDHash)[className];
        }
    }
}

void RegistryAPI::registerInterfaceValueTypeInfo(const std::type_info& typeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name, const char *interface_iid)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::InterfaceValueTypeInfo);
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    gIIDTypeHash->insert(interface_iid, &typeId);
    gJavaClassIIDHash->insert(java_name, interface_iid);
}

void RegistryAPI::registerFunctionalTypeInfo(const std::type_info& typeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name, int metaType,
                                             size_t size, size_t alignment, size_t sizeOfShell, FunctionalResolver resolver, uint returnScopes,
                                             RegistryAPI::DestructorFn destructor, std::initializer_list<ConstructorInfo> constructors,
                                             PtrDeleterFunction deleter, std::initializer_list<FunctionInfo> virtualFunctions)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::FunctionalTypeInfo);
    registerSizeOfType(typeId, size);
    registerAlignmentOfType(typeId, alignment);
    registerMetaTypeID(typeId, metaType);
    {
        QWriteLocker locker(gRegistryLock());
        gMetaTypeJavaNameHash->insert(QMetaType(metaType).iface(), java_name);
        gJavaTypeMetaTypesHash->insert(java_name, QMetaType(metaType));
    }
    if(sizeOfShell>0)
        registerSizeOfShell(typeId, sizeOfShell);
    if(resolver)
        registerFunctionalResolver(typeId, resolver);
    if(destructor || constructors.size()>0)
        registerConstructorInfos(typeId, returnScopes, destructor, constructors);
    if(deleter)
        registerDeleter(typeId, deleter);
    if(virtualFunctions.size()>0)
        registerFunctionInfos(typeId, virtualFunctions);
}

void RegistryAPI::registerQObjectTypeInfo(const std::type_info& typeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::QObjectTypeInfo);
}

void RegistryAPI::registerObjectTypeInfo(const std::type_info& typeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::ObjectTypeInfo);
}

void RegistryAPI::registerValueTypeInfo(const std::type_info& typeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::ValueTypeInfo);
}

void RegistryAPI::registerEnumTypeInfo(const std::type_info& typeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::EnumTypeInfo);
}

void RegistryAPI::registerEnumTypeInfo(const std::type_info& typeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name, const std::type_info& flagsTypeId, QtJambiTypeInfo flagsInfo, const char *flags_qt_name, const char *flags_qt_name_alias, const char *flags_java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::EnumTypeInfo);
    registerTypeInfo(flagsTypeId, flagsInfo, flags_qt_name, flags_java_name, EntryTypes::FlagsTypeInfo);
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    if(QLatin1String(flags_qt_name)!=QLatin1String(flags_qt_name_alias))
        gQtNameTypeHash->insert(flags_qt_name_alias, &flagsTypeId);
    gFlagEnumIDHash->insert(unique_id(flagsTypeId), &typeId);
    gEnumFlagIDHash->insert(unique_id(typeId), &flagsTypeId);
}

void RegistryAPI::registerUnspecificTypeInfo(const std::type_info& typeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::Unspecific);
}

void RegistryAPI::registerPrimitiveTypeInfo(const std::type_info& typeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::PrimitiveTypeInfo);
}

void registerContainerTypeInfo(const std::type_info& typeId, RegistryAPI::QtJambiTypeInfo info, const char *qt_name, const char *java_name, const char *java_interface)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::Unspecific);
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    if(!gTypeJavaInterfaceHash->contains(unique_id(typeId))){
        gTypeJavaInterfaceHash->insert(unique_id(typeId), java_interface);
    }
}

void RegistryAPI::registerNamespaceTypeInfo(const char *qt_name, const char *java_name, const QMetaObject* namespaceMetaObject){
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    if(!gJavaNamespaceQtNameHash->contains(java_name)){
        gJavaNamespaceQtNameHash->insert(java_name, qt_name);
    }
    if(!gQtNamespaceJavaNameHash->contains(qt_name)){
        gQtNamespaceJavaNameHash->insert(qt_name, java_name);
    }
    if(namespaceMetaObject && !gNamespaceMetaObjectHash->contains(java_name)){
        gNamespaceMetaObjectHash->insert(java_name, namespaceMetaObject);
    }
}

void RegistryAPI::registerContainerAccessFactory(const std::type_info& typeId, NewContainerAccessFunction factory){
    QWriteLocker locker(gRegistryLock());
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
    QReadLocker locker(gRegistryLock());
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
    QReadLocker locker(gRegistryLock());
    QPair<int,int> pair{keyType.id(), valueType.id()};
    if(gMapAccessFactoryByMetaTypeHash->contains(pair)){
        return (*gMapAccessFactoryByMetaTypeHash)[pair][int(containerType)];
    }else{
        return nullptr;
    }
}

void registerContainerAccessFactory(SequentialContainerType containerType, const QMetaType& elementType, std::function<AbstractContainerAccess*()>&& factory){
    QWriteLocker locker(gRegistryLock());
    if(!(*gSequentialContainerAccessFactoryByMetaTypeHash)[elementType.id()].contains(int(containerType))
            || !(*gSequentialContainerAccessFactoryByMetaTypeHash)[elementType.id()][int(containerType)])
        (*gSequentialContainerAccessFactoryByMetaTypeHash)[elementType.id()][int(containerType)] = std::move(factory);
}

void registerContainerAccessFactory(AssociativeContainerType containerType, const QMetaType& keyType, const QMetaType& valueType, std::function<AbstractContainerAccess*()>&& factory){
    QWriteLocker locker(gRegistryLock());
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
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gSequentialContainerAccessFactoryHash->value(unique_id(typeId));
}


EntryTypes getEntryType(const std::type_info& typeId){
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gEntryTypesHash->value(unique_id(typeId), EntryTypes::Unspecific);
}

const std::type_info* getInterfaceTypeForIID(const char*interface_iid)
{
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gIIDTypeHash->value(interface_iid, nullptr);
}

bool isInterface(const std::type_info& typeId)
{
    EntryTypes entryType;
    {
        QReadLocker locker(gRegistryLock());
        entryType = gEntryTypesHash->value(unique_id(typeId), EntryTypes::Unspecific);
    }
    return entryType == EntryTypes::InterfaceTypeInfo || entryType == EntryTypes::InterfaceValueTypeInfo;
}

bool isQObject(const std::type_info& typeId)
{
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gEntryTypesHash->value(unique_id(typeId), EntryTypes::Unspecific) == EntryTypes::QObjectTypeInfo;
}

bool isFunctional(const std::type_info& typeId)
{
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gEntryTypesHash->value(unique_id(typeId), EntryTypes::Unspecific) == EntryTypes::FunctionalTypeInfo;
}

int registeredMetaTypeID(const std::type_info& typeId);

bool isValueType(JNIEnv * env, jclass valueType, int* metaTypeId)
{
    const SuperTypeInfos infos = SuperTypeInfos::fromClass(env, valueType);
    if(metaTypeId)
        *metaTypeId = QMetaType::UnknownType;
    if (!infos.isEmpty()){
        if(const std::type_info* typeId = getTypeByJavaName(QtJambiAPI::getClassName(env, infos.first().javaClass()).replace(QLatin1Char('.'), QLatin1Char('/')))){
            QReadLocker locker(gRegistryLock());
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
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    if(!gJavaNameTypeHash->contains(javaName)){
        if(javaName.endsWith("$ConcreteWrapper")){
            javaName = javaName.chopped(16);
        }
        if(javaName.endsWith("$Impl")){
            javaName = javaName.chopped(5);
        }
    }
    if(gJavaNameTypeHash->contains(javaName)){
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
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gQtFunctionalJavaNameHash->value(qt_name, nullptr);
}

const char* getQtNameByFunctional(const char* java_name)
{
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gQtFunctionalQtNameHash->value(java_name, nullptr);
}

bool isJavaNameNamespace(const QString& java_name){
    return isJavaNameNamespace(java_name.toLatin1());
}

bool isJavaNameNamespace(const QByteArray& java_name){
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gJavaNamespaceQtNameHash->contains(java_name);
}

const QMetaObject* registeredNamespaceMetaObject(const QString& java_name){
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gNamespaceMetaObjectHash->value(java_name.toLatin1());
}

bool isQtNameNamespace(const QByteArray& qt_name){
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gQtNamespaceJavaNameHash->contains(qt_name);
}

const std::type_info* getTypeByQtName(const QByteArray& qt_name)
{
    QReadLocker locker(gRegistryLock());
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

OptionalBool isRegisteredAsPointerType(const std::type_info& typeId){
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    if(gQTypeInfoHash->contains(unique_id(typeId))){
        return OptionalBool(gQTypeInfoHash->value(unique_id(typeId)).isPointer);
    }
    return OptionalBool();
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
OptionalBool isRegisteredAsStaticType(const std::type_info& typeId){
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    if(gQTypeInfoHash->contains(unique_id(typeId))){
        return OptionalBool(gQTypeInfoHash->value(unique_id(typeId)).isStatic);
    }
    return OptionalBool();
}
#endif

const char * getQtName(const std::type_info& typeId)
{
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gTypeQtNameHash->value(unique_id(typeId), nullptr);
}

const char * getJavaName(const std::type_info& typeId)
{
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gTypeJavaNameHash->value(unique_id(typeId), nullptr);
}

const char * getJavaInterfaceName(const std::type_info& typeId)
{
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    if(gTypeJavaInterfaceHash->contains(unique_id(typeId)))
        return gTypeJavaInterfaceHash->value(unique_id(typeId), nullptr);
    return gTypeJavaNameHash->value(unique_id(typeId), nullptr);
}

void registerMetaTypeID(const std::type_info& typeId,
                        const std::type_info& nonPointerTypeId,
                        int qtMetaType,
                        AbstractContainerAccess* access,
                        const QSharedPointer<AbstractContainerAccess>& sharedAccess)
{
    const char * registeredQtName;
    bool insert;
    {
        QReadLocker locker(gRegistryLock());
        insert = !gMetaTypeIDMap->contains(unique_id(typeId))
                || !gMetaTypeIDHash->contains(qtMetaType);
        registeredQtName = gTypeQtNameHash->value(unique_id(typeId), nullptr);
    }
    if(insert){
        QWriteLocker locker(gRegistryLock());
        if(!gMetaTypeIDMap->contains(unique_id(typeId)))
            gMetaTypeIDMap->insert(unique_id(typeId), qtMetaType);
        if(!gMetaTypeIDHash->contains(qtMetaType))
            gMetaTypeIDHash->insert(qtMetaType, &typeId);
    }
    QByteArray metaTypeName = QMetaType::typeName(qtMetaType);
    if(registeredQtName && metaTypeName!=registeredQtName && !access && !sharedAccess){
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
            && nonPointerTypeId!=typeid(std::nullptr_t)
            && !access
            && !sharedAccess){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        JObjectWrapperConverter::registerConverter(nonPointerTypeId, qtMetaType);
#else
        const std::type_info* _nonPointerTypeId = &nonPointerTypeId;
        QMetaType metaType(qtMetaType);
        QMetaType jObjectWrapperMetaType = QMetaType::fromType<JObjectWrapper>();
        if(!QMetaType::hasRegisteredConverterFunction(jObjectWrapperMetaType, metaType) && strcmp("QRemoteObjectPendingCall", metaType.name())!=0){
            QMetaType::registerConverterFunction([metaType,_nonPointerTypeId](const void *src, void *target) -> bool {
                const JObjectWrapper * wrapper = reinterpret_cast<const JObjectWrapper *>(src);
                if(JniEnvironment env{200}){
                    if(wrapper->object(env)){
                        void * ptr(nullptr);
                        if(QtJambiAPI::convertJavaToNative(env, wrapper->object(env), &ptr, *_nonPointerTypeId)){
                            if((metaType.flags() & QMetaType::IsPointer)){
                                *reinterpret_cast<void**>(target) = ptr;
                            }else {
                                metaType.destruct(target);
                                metaType.construct(target, ptr);
                            }
                            return true;
                        }
                        return QtJambiAPI::convertJavaToNative(env, wrapper->object(env), target, *_nonPointerTypeId);
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
    {
        QWriteLocker locker(gRegistryLock());
        Q_UNUSED(locker)
        gMetaTypeMetaObjectHash->insert(iface, mo);
    }
    if(const QtJambiMetaObject* cqjmo = QtJambiMetaObject::cast(mo)){
        if(cqjmo->revision()>=10){
#if QT_VERSION < QT_VERSION_CHECK(7, 0, 0)
            const qsizetype offset = cqjmo->revision() < 12
                    ? cqjmo->propertyCount()-cqjmo->propertyOffset()
                    : cqjmo->propertyCount()-cqjmo->propertyOffset() + cqjmo->enumeratorCount()-cqjmo->enumeratorOffset();
#else
            const qsizetype offset = cqjmo->propertyCount()-cqjmo->propertyOffset() + cqjmo->enumeratorCount()-cqjmo->enumeratorOffset();
#endif
            if(!mo->d.metaTypes[offset])
                const_cast<const QtPrivate::QMetaTypeInterface**>(mo->d.metaTypes)[offset] = iface;
        }
    }
}

const QMetaObject* metaobjectByMetaTypeInterface(const QtPrivate::QMetaTypeInterface* iface){
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gMetaTypeMetaObjectHash->value(iface);
}
#endif

void RegistryAPI::registerMetaTypeID(const std::type_info& typeId, int qtMetaType)
{
    ::registerMetaTypeID(typeId, typeId, qtMetaType);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

const char* registerMetaTypeName(const QByteArray& typeName){
    {
        QReadLocker locker(gRegistryLock());
        QSet<QByteArray>::const_iterator iter = gTypeNames->constFind(typeName);
        if(iter!=gTypeNames->end())
            return iter->data();
    }
    {
        QWriteLocker locker(gRegistryLock());
        Q_UNUSED(locker)
        QSet<QByteArray>::iterator iter = gTypeNames->insert(typeName);
        return iter->data();
    }
}

bool QmlAPI::registerMetaTypeConverter(JNIEnv *env, const QMetaType& metaType1, jclass jsvalueClass, const QMetaType& metaType2, jclass targetClass, jmethodID constructor){
    if(!QMetaType::hasRegisteredConverterFunction(metaType1, metaType2)){
        QtJambiUtils::InternalToExternalConverter converter1 = QtJambiTypeManager::getInternalToExternalConverter(env, metaType1.name(), metaType1, jsvalueClass);
        QtJambiUtils::ExternalToInternalConverter reconverter1 = QtJambiTypeManager::getExternalToInternalConverter(env, jsvalueClass, metaType1.name(), metaType1);
        QtJambiUtils::InternalToExternalConverter converter2 = QtJambiTypeManager::getInternalToExternalConverter(env, metaType2.name(), metaType2, targetClass);
        QtJambiUtils::ExternalToInternalConverter reconverter2 = QtJambiTypeManager::getExternalToInternalConverter(env, targetClass, metaType2.name(), metaType2);
        if(converter1 && reconverter1 && converter2 && reconverter2){
            ParameterTypeInfo parameter1{metaType1, jsvalueClass, std::move(converter1), std::move(reconverter1)};
            ParameterTypeInfo parameter2{metaType2, targetClass, std::move(converter2), std::move(reconverter2)};
            return QMetaType::registerConverterFunction([parameter1, parameter2, constructor](const void *src, void *target)->bool{
                if(JniEnvironment env{200}){
                    jvalue jv;
                    jv.l = nullptr;
                    if(parameter1.convertInternalToExternal(env, nullptr, src, jv, true)){
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

#if QT_VERSION < QT_VERSION_CHECK(6,5,0)
#define QMetaTypeInterface_CurrentRevision 0
#else
#define QMetaTypeInterface_CurrentRevision QtPrivate::QMetaTypeInterface::CurrentRevision
#endif

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
            /*.revision=*/ QMetaTypeInterface_CurrentRevision,
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
                     AfterRegistrationFunction afterRegistrationFunction)
{
    QMetaType metaType = QMetaType::fromName(typeName);
    if(metaType.isValid()){
        bool needsRegistration;
        {
            QReadLocker locker(gRegistryLock());
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
}

void registerNestedAccess(QWriteLocker &locker, AbstractContainerAccess* access){
    if(auto daccess = dynamic_cast<AbstractPairAccess*>(access)){
        locker.unlock();
        int id1 = daccess->firstMetaType().id();
        int id2 = daccess->secondMetaType().id();
        locker.relock();
        if(!gContainerAccessMap->contains(id1) || !gContainerAccessMap->contains(id2)){
            locker.unlock();
            if(auto naccess = dynamic_cast<AbstractNestedPairAccess*>(daccess)){
                if(QSharedPointer<AbstractContainerAccess> _access = naccess->sharedFirstNestedContainerAccess()){
                    locker.relock();
                    if(!gContainerAccessMap->contains(id1))
                        gContainerAccessMap->insert(id1, _access);
                    locker.unlock();
                }
                if(QSharedPointer<AbstractContainerAccess> _access = naccess->sharedSecondNestedContainerAccess()){
                    locker.relock();
                    if(!gContainerAccessMap->contains(id2))
                        gContainerAccessMap->insert(id2, _access);
                    locker.unlock();
                }
            }else{
                if(AbstractContainerAccess* _access = daccess->firstNestedContainerAccess()){
                    locker.relock();
                    if(!gContainerAccessMap->contains(id1))
                        gContainerAccessMap->insert(id1, QSharedPointer<AbstractContainerAccess>(_access->clone(), &containerDisposer));
                    locker.unlock();
                }
                if(AbstractContainerAccess* _access = daccess->secondNestedContainerAccess()){
                    locker.relock();
                    if(!gContainerAccessMap->contains(id2))
                        gContainerAccessMap->insert(id2, QSharedPointer<AbstractContainerAccess>(_access->clone(), &containerDisposer));
                    locker.unlock();
                }
            }
            locker.relock();
        }
    }else if(auto daccess = dynamic_cast<AbstractAssociativeAccess*>(access)){
        locker.unlock();
        int id1 = daccess->keyMetaType().id();
        int id2 = daccess->valueMetaType().id();
        locker.relock();
        if(!gContainerAccessMap->contains(id1) || !gContainerAccessMap->contains(id2)){
            locker.unlock();
            if(auto naccess = dynamic_cast<AbstractNestedAssociativeAccess*>(daccess)){
                if(QSharedPointer<AbstractContainerAccess> _access = naccess->sharedKeyNestedContainerAccess()){
                    locker.relock();
                    if(!gContainerAccessMap->contains(id1))
                        gContainerAccessMap->insert(id1, _access);
                    locker.unlock();
                }
                if(QSharedPointer<AbstractContainerAccess> _access = naccess->sharedValueNestedContainerAccess()){
                    locker.relock();
                    if(!gContainerAccessMap->contains(id2))
                        gContainerAccessMap->insert(id2, _access);
                    locker.unlock();
                }
            }else{
                if(AbstractContainerAccess* _access = daccess->keyNestedContainerAccess()){
                    locker.relock();
                    if(!gContainerAccessMap->contains(id1))
                        gContainerAccessMap->insert(id1, QSharedPointer<AbstractContainerAccess>(_access->clone(), &containerDisposer));
                    locker.unlock();
                }
                if(AbstractContainerAccess* _access = daccess->valueNestedContainerAccess()){
                    locker.relock();
                    if(!gContainerAccessMap->contains(id2))
                        gContainerAccessMap->insert(id2, QSharedPointer<AbstractContainerAccess>(_access->clone(), &containerDisposer));
                    locker.unlock();
                }
            }
            locker.relock();
        }
    }else if(auto daccess = dynamic_cast<AbstractSequentialAccess*>(access)){
        locker.unlock();
        int id1 = daccess->elementMetaType().id();
        locker.relock();
        if(!gContainerAccessMap->contains(id1)){
            locker.unlock();
            if(auto naccess = dynamic_cast<AbstractNestedSequentialAccess*>(daccess)){
                if(QSharedPointer<AbstractContainerAccess> _access = naccess->sharedElementNestedContainerAccess()){
                    locker.relock();
                    gContainerAccessMap->insert(id1, _access);
                    locker.unlock();
                }
            }else{
                if(AbstractContainerAccess* _access = daccess->elementNestedContainerAccess()){
                    locker.relock();
                    gContainerAccessMap->insert(id1, QSharedPointer<AbstractContainerAccess>(_access->clone(), &containerDisposer));
                    locker.unlock();
                }
            }
            locker.relock();
        }
    }
}

QSharedPointer<AbstractContainerAccess> findContainerAccess(int metaType){
    QSharedPointer<AbstractContainerAccess> result;
    {
        QReadLocker locker(gRegistryLock());
        result = gContainerAccessMap->value(metaType);
    }
    if(!result){
        switch(metaType){
        case QMetaType::QStringList:
            result.reset(QtJambiPrivate::QListAccess<QString>::newInstance(), &containerDisposer);
            {
                QWriteLocker locker(gRegistryLock());
                gContainerAccessMap->insert(metaType, result);
                registerNestedAccess(locker, result.data());
            }
            break;
        case QMetaType::QVariantList:
            result.reset(QtJambiPrivate::QListAccess<QVariant>::newInstance(), &containerDisposer);
            {
                QWriteLocker locker(gRegistryLock());
                gContainerAccessMap->insert(metaType, result);
                registerNestedAccess(locker, result.data());
            }
            break;
        case QMetaType::QVariantHash:
            result.reset(QtJambiPrivate::QHashAccess<QString,QVariant>::newInstance(), &containerDisposer);
            {
                QWriteLocker locker(gRegistryLock());
                gContainerAccessMap->insert(metaType, result);
                registerNestedAccess(locker, result.data());
            }
            break;
        case QMetaType::QVariantMap:
            result.reset(QtJambiPrivate::QMapAccess<QString,QVariant>::newInstance(), &containerDisposer);
            {
                QWriteLocker locker(gRegistryLock());
                gContainerAccessMap->insert(metaType, result);
                registerNestedAccess(locker, result.data());
            }
            break;
        case QMetaType::QVariantPair:
            result.reset(QtJambiPrivate::QPairAccess<QVariant,QVariant>::newInstance(), &containerDisposer);
            {
                QWriteLocker locker(gRegistryLock());
                gContainerAccessMap->insert(metaType, result);
                registerNestedAccess(locker, result.data());
            }
            break;
        default: break;
        }
    }
    return result;
}

QSharedPointer<AbstractContainerAccess> findContainerAccess(const QMetaType& metaType){
    QSharedPointer<AbstractContainerAccess> result;
    {
        QReadLocker locker(gRegistryLock());
        result = gContainerAccessMap->value(metaType.id());
    }
    if(!result){
        switch(metaType.id()){
        case QMetaType::QStringList:
            result.reset(QtJambiPrivate::QListAccess<QString>::newInstance(), &containerDisposer);
            {
                QWriteLocker locker(gRegistryLock());
                gContainerAccessMap->insert(metaType.id(), result);
                registerNestedAccess(locker, result.data());
            }
            break;
        case QMetaType::QVariantList:
            result.reset(QtJambiPrivate::QListAccess<QVariant>::newInstance(), &containerDisposer);
            {
                QWriteLocker locker(gRegistryLock());
                gContainerAccessMap->insert(metaType.id(), result);
                registerNestedAccess(locker, result.data());
            }
            break;
        case QMetaType::QVariantHash:
            result.reset(QtJambiPrivate::QHashAccess<QString,QVariant>::newInstance(), &containerDisposer);
            {
                QWriteLocker locker(gRegistryLock());
                gContainerAccessMap->insert(metaType.id(), result);
                registerNestedAccess(locker, result.data());
            }
            break;
        case QMetaType::QVariantMap:
            result.reset(QtJambiPrivate::QMapAccess<QString,QVariant>::newInstance(), &containerDisposer);
            {
                QWriteLocker locker(gRegistryLock());
                gContainerAccessMap->insert(metaType.id(), result);
                registerNestedAccess(locker, result.data());
            }
            break;
        case QMetaType::QVariantPair:
            result.reset(QtJambiPrivate::QPairAccess<QVariant,QVariant>::newInstance(), &containerDisposer);
            {
                QWriteLocker locker(gRegistryLock());
                gContainerAccessMap->insert(metaType.id(), result);
                registerNestedAccess(locker, result.data());
            }
            break;
        default: break;
        }
    }
    return result;
}

bool hasRegisteredContainerAccess(const QMetaType& metaType){
    QReadLocker locker(gRegistryLock());
    return !gContainerAccessMap->value(metaType.id()).isNull();
}

int registerMetaTypeImpl(const std::type_info* typeId,
                     const std::type_info* nonPointerTypeId,
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
                     AbstractContainerAccess* access,
                     const QSharedPointer<AbstractContainerAccess>& sharedAccess)
{
    QMetaType metaType = QMetaType::fromName(typeName);
    if(metaType.isValid()){
        auto id = metaType.id();
        bool needsRegistration;
        bool needsContainerAccessRegistration;
        {
            QReadLocker locker(gRegistryLock());
            Q_UNUSED(locker)
            needsContainerAccessRegistration = (access || sharedAccess) && !gContainerAccessMap->contains(id);
            needsRegistration = typeId && !gMetaTypeIDMap->contains(unique_id(*typeId));
        }
        if(needsContainerAccessRegistration){
            if(sharedAccess){
                QWriteLocker locker(gRegistryLock());
                gContainerAccessMap->insert(id, sharedAccess);
                registerNestedAccess(locker, sharedAccess.data());
            }else if(access){
                QSharedPointer<AbstractContainerAccess> shared(access->clone(), &containerDisposer);
                QWriteLocker locker(gRegistryLock());
                gContainerAccessMap->insert(id, std::move(shared));
                registerNestedAccess(locker, access);
            }
        }
        if(needsRegistration && typeId && nonPointerTypeId)
            ::registerMetaTypeID(*typeId, *nonPointerTypeId, id, access, sharedAccess);
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
        if(access || sharedAccess){
            auto id = metaType.id();
            bool insert;
            {
                QReadLocker locker(gRegistryLock());
                insert = !gContainerAccessMap->contains(id);
            }
            if(insert){
                if(!gContainerAccessMap->contains(id)){
                    if(sharedAccess){
                        QWriteLocker locker(gRegistryLock());
                        gContainerAccessMap->insert(id, sharedAccess);
                        registerNestedAccess(locker, sharedAccess.data());
                    }else if(access){
                        QSharedPointer<AbstractContainerAccess> shared(access->clone(), &containerDisposer);
                        QWriteLocker locker(gRegistryLock());
                        gContainerAccessMap->insert(id, std::move(shared));
                        registerNestedAccess(locker, access);
                    }
                }
            }
        }
        if(typeId && nonPointerTypeId){
            ::registerMetaTypeID(*typeId, *nonPointerTypeId, metaType.id(), access, sharedAccess);
        }
    }
    return metaType.id();
}

void registerContainerAccess(int metaType, const QSharedPointer<AbstractContainerAccess>& sharedAccess){
    if(sharedAccess){
        {
            QReadLocker locker(gRegistryLock());
            if(gContainerAccessMap->contains(metaType))
                return;
        }
        {
            QWriteLocker locker(gRegistryLock());
            gContainerAccessMap->insert(metaType, sharedAccess);
        }
    }
}

void registerContainerAccess(int metaType, AbstractContainerAccess* access){
    if(access){
        {
            QReadLocker locker(gRegistryLock());
            if(gContainerAccessMap->contains(metaType))
                return;
        }
        {
            QSharedPointer<AbstractContainerAccess> shared(access->clone(), &containerDisposer);
            QWriteLocker locker(gRegistryLock());
            gContainerAccessMap->insert(metaType, std::move(shared));
        }
    }
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
                     AfterRegistrationFunction afterRegistrationFunction,
                     AbstractContainerAccess* access)
{
    return registerMetaTypeImpl(&typeId,
                                &nonPointerTypeId,
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
                                legacyRegisterOp,
                                size,
                                align,
                                builtInTypeId,
                                flags,
                                metaObject,
                                afterRegistrationFunction,
                                access, {});
}

int registerContainerMetaType(const QByteArray& typeName,
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
                     const QSharedPointer<AbstractContainerAccess>& sharedAccess)
{
    return registerMetaTypeImpl(nullptr,
                                nullptr,
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
                                legacyRegisterOp,
                                size,
                                align,
                                builtInTypeId,
                                flags,
                                metaObject,
                                afterRegistrationFunction,
                                nullptr, sharedAccess);
}

int registerSmartPointerMetaType(const QByteArray& typeName,
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
                     QMetaType::TypeFlags flags)
{
    return registerMetaTypeImpl(nullptr,
                                nullptr,
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
                                legacyRegisterOp,
                                size,
                                align,
                                builtInTypeId,
                                flags,
                                nullptr,
                                nullptr,
                                nullptr, {});
}

int registeredMetaTypeID(const std::type_info& typeId)
{
    size_t uid = unique_id(typeId);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gMetaTypeIDMap->value(uid, QMetaType::UnknownType);
}

const std::type_info* getTypeByMetaType(const QMetaType& metaType)
{
    return getTypeByMetaType(metaType.id());
}

const std::type_info* getTypeByMetaType(int metaType)
{
    {
        QReadLocker locker(gRegistryLock());
        Q_UNUSED(locker)
        if(const std::type_info* info = gMetaTypeIDHash->value(metaType, nullptr)){
            return info;
        }
    }
    return getTypeByQtName(QMetaType::typeName(metaType));
}

const std::type_info* getEnumForFlag(const std::type_info& flag)
{
    size_t uid = unique_id(flag);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gFlagEnumIDHash->value(uid, nullptr);
}

const std::type_info* getFlagForEnum(const std::type_info& enumerator)
{
    size_t uid = unique_id(enumerator);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gEnumFlagIDHash->value(uid, nullptr);
}

const void* QtJambiPrivate::getDefaultValue(const std::type_info& type_info, QtJambiPrivate::DefaultValueCreator creator)
{
    size_t uid = unique_id(type_info);
    const void* result;
    {
        QReadLocker locker(gRegistryLock());
        Q_UNUSED(locker)
        result = gDefaultValueHash->value(uid, nullptr);
    }
    if(!result){
        result = creator();
        QWriteLocker wlocker(gRegistryLock());
        Q_UNUSED(wlocker)
        gDefaultValueHash->insert(uid, result);
    }
    return result;
}

void RegistryAPI::registerFunctionInfos(const std::type_info& typeId, std::initializer_list<FunctionInfo> virtualFunctions)
{
    size_t uid = unique_id(typeId);
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    if(!gVirtualFunctionInfos->contains(uid))
        gVirtualFunctionInfos->insert(uid, virtualFunctions);
}

QVector<RegistryAPI::FunctionInfo> virtualFunctions(const std::type_info& typeId)
{
    size_t uid = unique_id(typeId);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    if(gVirtualFunctionInfos->contains(uid)){
        return (*gVirtualFunctionInfos())[uid];
    }else{
        return QVector<RegistryAPI::FunctionInfo>{};
    }
}

void RegistryAPI::registerConstructorInfos(const std::type_info& typeId, uint returnScopes, RegistryAPI::DestructorFn destructor, std::initializer_list<ConstructorInfo> constructors){
    size_t uid = unique_id(typeId);
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    if(!gConstructorInfos->contains(uid))
        gConstructorInfos->insert(uid, constructors);
    if(!gDestructorHash->contains(uid))
        gDestructorHash->insert(uid, destructor);
    if(returnScopes>0 && !gReturnScopes->contains(uid))
        gReturnScopes->insert(uid, returnScopes);
}

uint returnScopes(const std::type_info& typeId){
    size_t uid = unique_id(typeId);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gReturnScopes()->value(uid, 0);
}

const QVector<const RegistryAPI::ConstructorInfo>* registeredConstructorInfos(const std::type_info& typeId){
    size_t uid = unique_id(typeId);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    if(gConstructorInfos->contains(uid)){
        return reinterpret_cast<const QVector<const RegistryAPI::ConstructorInfo>*>(&(*gConstructorInfos())[uid]);
    }else{
        return nullptr;
    }
}

void registerFlagToEnum(const char *flag_name, const char *enum_name)
{
    auto hashcode = qHash(flag_name);
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    gFlagEnumNameHash->insert(hashcode, enum_name);
}

const char* getEnumName(const char*flag_name)
{
    auto hashcode = qHash(flag_name);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gFlagEnumNameHash->value(hashcode, nullptr);
}

void registerInterface(const char *qt_interface, const char *java_interface, const char *interface_iid)
{
    auto hashcode = qHash(qt_interface);
    auto hashcode2 = interface_iid ? qHash(interface_iid) : 0;
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    gInterfaceHash->insert(hashcode, java_interface);
    if(interface_iid)
        gInterfaceIIDsHash->insert(hashcode2, qt_interface);
}

const char* getInterfaceForIID(const char*interface_iid)
{
    auto hashcode = qHash(interface_iid);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gInterfaceIIDsHash->value(hashcode, nullptr);
}

bool isInterface(const char*qt_interface)
{
    auto hashcode = qHash(qt_interface);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gInterfaceHash->contains(hashcode);
}

const char* getInterface(const char*qt_interface)
{
    auto hashcode = qHash(qt_interface);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gInterfaceHash->value(hashcode, nullptr);
}

void registerFunctional(const char *qt_interface)
{
    auto hashcode = qHash(qt_interface);
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    gFunctionalHash->insert(hashcode);
}

bool isFunctional(const char*qt_interface)
{
    auto hashcode = qHash(qt_interface);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gFunctionalHash->contains(hashcode);
}

void RegistryAPI::registerDeleter(const std::type_info& typeId, PtrDeleterFunction deleter)
{
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    gDeleterHash->insert(unique_id(typeId), deleter);
}

PtrDeleterFunction deleter(const std::type_info& typeId)
{
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gDeleterHash->value(unique_id(typeId), nullptr);
}

void RegistryAPI::registerOwnerFunction(const std::type_info& typeId, PtrOwnerFunction ownerFunction)
{
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    gOwnerFunctionHash->insert(unique_id(typeId), ownerFunction);
}

PtrOwnerFunction registeredOwnerFunction(const std::type_info& typeId)
{
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gOwnerFunctionHash->value(unique_id(typeId), nullptr);
}

void clear_type_entry(const std::type_info& typeId);

void RegistryAPI::registerPolymorphyHandler(const std::type_info& polymorphicBaseTypeId, const std::type_info& targetTypeId, PolymorphyHandler handler)
{
    if(handler){
        {
            QWriteLocker locker(gRegistryLock());
            Q_UNUSED(locker)
            g_polymorphic_ids->insert(unique_id(polymorphicBaseTypeId), new PolymorphicIdHandler(targetTypeId, handler));
            gPolymorphicBasesHash->insert(unique_id(targetTypeId), &polymorphicBaseTypeId);
        }
        clear_type_entry(polymorphicBaseTypeId);
    }
}

bool isPolymorphicBase(const std::type_info& typeId){
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return g_polymorphic_ids->contains(unique_id(typeId));
}

QList<const std::type_info*> getPolymorphicBases(const std::type_info& typeId){
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gPolymorphicBasesHash->values(unique_id(typeId));
}

QList<const PolymorphicIdHandler*> getPolymorphicIdHandlers(const std::type_info& polymorphicBaseTypeId){
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return g_polymorphic_ids->values(unique_id(polymorphicBaseTypeId));
}

void RegistryAPI::registerSizeOfType(const std::type_info& typeId, size_t size)
{
    {
        QReadLocker locker(gRegistryLock());
        if(gValueSizeHash->contains(unique_id(typeId)))
            return;
    }
    {
        QWriteLocker locker(gRegistryLock());
        if(!gValueSizeHash->contains(unique_id(typeId)))
            gValueSizeHash->insert(unique_id(typeId), size);
    }
}

void RegistryAPI::registerAlignmentOfType(const std::type_info& typeId, size_t alignment)
{
    {
        QReadLocker locker(gRegistryLock());
        if(gValueAlignmentHash->contains(unique_id(typeId)))
            return;
    }
    {
        QWriteLocker locker(gRegistryLock());
        if(!gValueAlignmentHash->contains(unique_id(typeId)))
            gValueAlignmentHash->insert(unique_id(typeId), alignment);
    }
}

void RegistryAPI::registerSizeOfShell(const std::type_info& typeId, size_t size)
{
    {
        QReadLocker locker(gRegistryLock());
        if(gShellSizeHash->contains(unique_id(typeId)))
            return;
    }
    {
        QWriteLocker locker(gRegistryLock());
        if(!gShellSizeHash->contains(unique_id(typeId)))
            gShellSizeHash->insert(unique_id(typeId), size);
    }
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
    {
        QReadLocker locker(gRegistryLock());
        Q_UNUSED(locker)
        for(int mt : gMetaTypeIDMap->values())
            types << mt;
    }
    QMap<QString,QPair<size_t,size_t>> result;
    for(int type : qAsConst(types)){
        if(type!=QMetaType::Nullptr
                && type!=QMetaType::UnknownType
                && type!=QMetaType::Void){
            QMetaType metaType(type);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            size_t alignment = 0;
            const std::type_info* typeId;
            {
                QReadLocker locker(gRegistryLock());
                if(const std::type_info* typeId = gMetaTypeIDHash->value(type)){
                    alignment = gValueAlignmentHash->value(unique_id(*typeId));
            }
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

void RegistryAPI::registerHashFunction(const std::type_info& typeId, qHashFn hashFunction){
    auto uniq = unique_id(typeId);
    {
        QReadLocker locker(gRegistryLock());
        if(gHashFunctionHash->contains(uniq))
            return;
    }
    {
        QWriteLocker locker(gRegistryLock());
        Q_UNUSED(locker)
        if(!gHashFunctionHash->contains(uniq))
            gHashFunctionHash->insert(uniq, hashFunction);
    }
}

RegistryAPI::qHashFn registeredHashFunction(const std::type_info& typeId)
{
    auto uniq = unique_id(typeId);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gHashFunctionHash->value(uniq, nullptr);
}

size_t getValueSize(const std::type_info& typeId)
{
    auto uniq = unique_id(typeId);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gValueSizeHash->value(uniq, 0);
}

size_t getValueAlignment(const std::type_info& typeId)
{
    auto uniq = unique_id(typeId);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gValueAlignmentHash->value(uniq, 0);
}

size_t getShellSize(const std::type_info& typeId)
{
    auto uniq = unique_id(typeId);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gShellSizeHash->value(uniq, 0);
}

void RegistryAPI::registerInterfaceOffset(const std::type_info& qt_base, const std::type_info& qt_interface, int offset)
{
    if(offset!=0){
        auto uniq = unique_id(qt_base);
        auto uniq2 = unique_id(qt_interface);
        QWriteLocker locker(gRegistryLock());
        Q_UNUSED(locker)
        ( *gOffsetHash )[uniq].insert(uniq2, offset);
        ( *gOffsetHash )[uniq2].insert(uniq, -offset);
        ( *gInterfacesHash )[uniq] << &qt_interface;
    }
}

int registeredInterfaceOffset(const std::type_info& qt_base, const std::type_info& qt_interface){
    auto uniq = unique_id(qt_base);
    auto uniq2 = unique_id(qt_interface);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    if(gOffsetHash->contains(uniq)){
        return ( *gOffsetHash )[uniq].value(uniq2, -1);
    }
    return -1;
}

void registeredInterfaceOffsets(const std::type_info& qt_type, InterfaceOffsetInfo* info){
    auto uniq = unique_id(qt_type);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    if(gOffsetHash->contains(uniq)){
        const QMap<size_t, int>& values = ( *gOffsetHash )[uniq];
        for(QMap<size_t, int>::const_key_value_iterator i = values.keyValueBegin(); i!=values.keyValueEnd(); ++i){
            info->interfaces.insert(i.base().key());
            if(i.base().value()>0)
                info->offsets.insert(i.base().key(), uint(i.base().value()));
        }
    }
    if(gInterfacesHash->contains(uniq)){
        info->inheritedInterfaces[uniq] = ( *gInterfacesHash )[uniq];
    }
}

const InterfaceOffsetInfo* getInterfaceOffsets(JNIEnv *env, jclass clazz){
    if(clazz){
        uint classHash = uint(Java::Runtime::Object::hashCode(env,clazz));
        {
            QReadLocker locker(gRegistryLock());
            Q_UNUSED(locker)
            if(gInterfaceOffsetHash->contains(classHash))
                return &( *gInterfaceOffsetHash )[classHash];
        }
        {
            InterfaceOffsetInfo result;
            jclass cls = clazz;
            if(!Java::QtJambi::ClassAnalyzerUtility::isGeneratedClass(env, cls)){
                const SuperTypeInfos infos = SuperTypeInfos::fromClass(env, clazz);
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
            QWriteLocker wlocker(gRegistryLock());
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
            QReadLocker locker(gRegistryLock());
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
            QWriteLocker wlocker(gRegistryLock());
            Q_UNUSED(wlocker)
            if(gInterfaceOffsetHash->contains(classHash))
                return &( *gInterfaceOffsetHash )[classHash];
            ( *gInterfaceOffsetHash )[classHash] = std::move(result);
            return &( *gInterfaceOffsetHash )[classHash];
        }
    }
    return nullptr;
}

void registerDestructor(const std::type_info& typeId, RegistryAPI::DestructorFn destructor)
{
    auto uniq = unique_id(typeId);
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    gDestructorHash->insert(uniq, destructor);
}

RegistryAPI::DestructorFn registeredDestructor(const std::type_info& typeId){
    auto uniq = unique_id(typeId);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gDestructorHash->value(uniq, nullptr);
}

size_t qHash(const QMetaObject* mo, size_t seed = 0){
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, qintptr(mo));
    return seed;
}

void RegistryAPI::registerCustomMetaObject(const std::type_info& typeId, const QMetaObject& superTypeMetaObject)
{
    auto uniq = unique_id(typeId);
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    gHasCustomMetaObjectHash->insert(uniq, &superTypeMetaObject);
}

bool hasCustomMetaObject(const std::type_info& typeId, const QMetaObject** superTypeMetaObject){
    auto uniq = unique_id(typeId);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    if(gHasCustomMetaObjectHash->contains(uniq)){
        if(superTypeMetaObject)
            *superTypeMetaObject = gHasCustomMetaObjectHash->value(uniq, nullptr);
        return true;
    }
    return false;
}

const QMetaObject* superTypeForCustomMetaObject(const std::type_info& typeId)
{
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gHasCustomMetaObjectHash->value(unique_id(typeId), nullptr);
}

void RegistryAPI::registerMetaObject(const std::type_info& typeId, const QMetaObject& originalMetaObject, bool isValueOwner,
                        QMetaMethodRenamerFn methodRenamer)
{
    auto uniq = unique_id(typeId);
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    gOriginalMetaObjectHash->insert(uniq, &originalMetaObject);
    gTypeIdsByMetaObject->insert(&originalMetaObject, &typeId);
    if(methodRenamer)
        gQMetaMethodRenamerFnByMetaObject->insert(&originalMetaObject, methodRenamer);
    if(isValueOwner)
        gValueOwners->insert(&originalMetaObject);
}

void RegistryAPI::registerMetaObject(const std::type_info& typeId, const QMetaObject& metaObject, bool isValueOwner,
                        std::initializer_list<SignalMetaInfo> signalMetaInfos,
                        ParameterInfoProviderFn parameterTypeInfoProvider,
                        QMetaMethodRenamerFn methodRenamer)
{
    auto uniq = unique_id(typeId);
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    gOriginalMetaObjectHash->insert(uniq, &metaObject);
    gTypeIdsByMetaObject->insert(&metaObject, &typeId);
    if(!gSignalMetaInfos->contains(&metaObject))
        gSignalMetaInfos->insert(&metaObject, signalMetaInfos);
    if(parameterTypeInfoProvider && !gParameterTypeInfoProviders->contains(&metaObject))
        gParameterTypeInfoProviders->insert(&metaObject, parameterTypeInfoProvider);
    if(methodRenamer)
        gQMetaMethodRenamerFnByMetaObject->insert(&metaObject, methodRenamer);
    if(isValueOwner)
        gValueOwners->insert(&metaObject);
}

bool isValueOwner(const QMetaObject* metaObject){
    {
        QReadLocker locker(gRegistryLock());
        Q_UNUSED(locker)
        if(gValueOwnerHash->contains(metaObject)){
            return gValueOwnerHash->value(metaObject);
        }
    }
    bool result = false;
    ValueOwnerSet valueOwners;
    {
        QReadLocker locker(gRegistryLock());
        Q_UNUSED(locker)
        valueOwners = *gValueOwners;
    }
    for(const QMetaObject* mo : qAsConst(valueOwners)){
        if(metaObject->inherits(mo)){
            result = true;
            break;
        }
    }
    {
        QWriteLocker locker(gRegistryLock());
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
    auto uniq = unique_id(typeId);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gOriginalMetaObjectHash->value(uniq, nullptr);
}

const QVector<const RegistryAPI::SignalMetaInfo>* signalMetaInfos(const QMetaObject* metaObject)
{
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    if(gSignalMetaInfos->contains(metaObject)){
        return reinterpret_cast<const QVector<const RegistryAPI::SignalMetaInfo>*>(&(*gSignalMetaInfos())[metaObject]);
    }else{
        return nullptr;
    }
}

QList<QMetaMethod> getExtraSignalsOfMetaObject(const QMetaObject* metaObject){
    {
        QReadLocker lock(gRegistryLock());
        if(gExtraSignalsOfMetaObject->contains(metaObject))
            return gExtraSignalsOfMetaObject->value(metaObject);
    }
    QList<QMetaMethod> extraSignals;
    QSet<int> availableSignalInfos;
    {
        const QMetaObject* currentMetaObject = metaObject;
        do{
            if(const QVector<const RegistryAPI::SignalMetaInfo>* infos = signalMetaInfos(currentMetaObject)){
                for(const RegistryAPI::SignalMetaInfo& info : *infos){
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
                && !(method.attributes() & QMetaMethod::Cloned)
                && !availableSignalInfos.contains(method.methodIndex())
                && !QtJambiMetaObject::isInstance(method.enclosingMetaObject())){
            extraSignals << method;
        }
    }
    {
        QWriteLocker lock(gRegistryLock());
        if(gExtraSignalsOfMetaObject->contains(metaObject))
            return gExtraSignalsOfMetaObject->value(metaObject);
        gExtraSignalsOfMetaObject->insert(metaObject, extraSignals);
    }
    return extraSignals;
}

RegistryAPI::ParameterInfoProviderFn registeredParameterInfoProviderFn(const QMetaObject* metaObject)
{
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    if(gSignalMetaInfos->contains(metaObject)){
        return (*gParameterTypeInfoProviders())[metaObject];
    }else{
        return nullptr;
    }
}

const QHash<int,const char*>* CoreAPI::renamedMethods(const QMetaObject* metaObject){
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    if(gQMetaMethodRenamerFnByMetaObject->contains(metaObject)){
        if(!gRenamedMethodsHash->contains(metaObject)){
            RegistryAPI::QMetaMethodRenamerFn registeredMetaMethodRenamer = (*gQMetaMethodRenamerFnByMetaObject())[metaObject];
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

RegistryAPI::QMetaMethodRenamerFn registeredMetaMethodRenamer(const QMetaObject* metaObject)
{
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    if(gQMetaMethodRenamerFnByMetaObject->contains(metaObject)){
        return (*gQMetaMethodRenamerFnByMetaObject())[metaObject];
    }else{
        return nullptr;
    }
}

void RegistryAPI::registerTypeInfoSupplier(const std::type_info& typeId, TypeInfoSupplier resolver){
    auto uniq = unique_id(typeId);
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    gTypeInfoSupplierHash->insert(uniq, resolver);
}

TypeInfoSupplier registeredTypeInfoSupplier(const std::type_info& typeId){
    auto uniq = unique_id(typeId);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gTypeInfoSupplierHash->value(uniq, nullptr);
}

void RegistryAPI::registerFunctionalResolver(const std::type_info& typeId, FunctionalResolver resolver)
{
    auto uniq = unique_id(typeId);
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    gFunctionalResolverHash->insert(uniq, resolver);
}

FunctionalResolver registeredFunctionalResolver(const std::type_info& typeId){
    auto uniq = unique_id(typeId);
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gFunctionalResolverHash->value(uniq, nullptr);
}

static QmlAPI::QmlReportDestruction qmlReportDestructionFunction = nullptr;

static QmlAPI::GetQmlOwnership fnGetQmlOwnership = nullptr;

void QtJambiAPI::DeclarativeUtil::reportDestruction(QObject * obj){
    if(qmlReportDestructionFunction)
        qmlReportDestructionFunction(obj);
}

void QmlAPI::setQmlReportDestruction(QmlReportDestruction fct){
    if(!qmlReportDestructionFunction)
        qmlReportDestructionFunction = fct;
}

void QmlAPI::setGetQmlOwnership(GetQmlOwnership fct){
    if(!fnGetQmlOwnership)
        fnGetQmlOwnership = fct;
}

bool isQmlJavaScriptOwnership(QObject * obj){
    return fnGetQmlOwnership && fnGetQmlOwnership(obj).testFlag(QmlAPI::JavaScriptOwnership);
}

bool isQmlExplicitCppOwnership(QObject * obj){
    return fnGetQmlOwnership && fnGetQmlOwnership(obj)==(QmlAPI::CppOwnership | QmlAPI::ExplicitSet);
}

void RegistryAPI::registerMediaControlInfo(const std::type_info& typeId, const char *media_control_iid){
    auto uniq = unique_id(typeId);
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    gMediaControlIIDHash->insert(uniq, media_control_iid);
    gMediaControlIIDClassHash->insert(media_control_iid, &typeId);
}

void registerJavaClassForCustomMetaType(JNIEnv *env, QMetaType metaType, jclass javaClass, bool isJObjectWrapped){
    const QtPrivate::QMetaTypeInterface * iface = metaType.iface();
    QByteArray javaName = QtJambiAPI::getClassName(env, javaClass).toLatin1().replace('.', '/');
    if(Java::Runtime::Class::isSynthetic(env,javaClass)){
        javaClass = JavaAPI::resolveClass(env, javaName);
    }else{
        javaClass = getGlobalClassRef(env, javaClass, javaName);
    }
    QWriteLocker locker(gRegistryLock());
    gMetaTypeJavaTypeHash->insert(iface, javaClass);
    gJavaTypeMetaTypesHash->insert(javaName, metaType);
    if(isJObjectWrapped)
        gJObjectWrappedMetaTypes->insert(iface);
}

bool isJObjectWrappedMetaType(QMetaType metaType){
    auto iface = metaType.iface();
    QReadLocker locker(gRegistryLock());
    return gJObjectWrappedMetaTypes->contains(iface);
}

bool isNativeWrapperMetaType(QMetaType metaType){
    auto iface = metaType.iface();
    QReadLocker locker(gRegistryLock());
    return gMetaTypesForJObjectNativeWrappers->contains(iface);
}

const QtPrivate::QMetaTypeInterface * getOriginalForNativeWrapperMetaType(const QtPrivate::QMetaTypeInterface *iface){
    const QtPrivate::QMetaTypeInterface *original{nullptr};
    {
        QReadLocker locker(gRegistryLock());
        original = (*gMetaTypesForJObjectNativeWrappers)[iface];
    }
    return original;
}

QMetaType getNativeWrapperType(const QMetaType& metaType){
    const QtPrivate::QMetaTypeInterface * nativeWrapperType{nullptr};
    if(metaType.flags().testFlag(QMetaType::IsPointer)){
        {
            auto id = metaType.id();
            QReadLocker locker(gRegistryLock());
            nativeWrapperType = (*gJObjectNativeWrapperMetaTypes)[id];
        }
        if(!nativeWrapperType){
            bool isQObject = metaType.flags().testFlag(QMetaType::PointerToQObject);
            const QtPrivate::QMetaTypeInterface *original{QMetaType(metaType).iface()};
            QByteArray typeName = QByteArray("JObjectWrapper<") + QByteArray(metaType.name()) + ">";
            const char* name = registerMetaTypeName(typeName);
            QtPrivate::QMetaTypeInterface* metaTypeInterface;
            const QMetaObject *originalMetaObject{nullptr};
            if(isQObject){
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                uint flags = QtPrivate::QMetaTypeTypeFlags<JQObjectWrapper>::Flags;
#else
                uint flags = QtPrivate::QMetaTypeForType<JQObjectWrapper>::flags();
#endif
                flags |= QMetaType::PointerToQObject;
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
                if(metaType.flags().testFlag(QMetaType::IsConst))
                    flags |= QMetaType::IsConst;
#endif
                metaTypeInterface = new QtPrivate::QMetaTypeInterface{
                    /*.revision=*/ QMetaTypeInterface_CurrentRevision,
                    /*.alignment=*/ alignof(JQObjectWrapper),
                    /*.size=*/ sizeof(JQObjectWrapper),
                    /*.flags=*/ flags,
                    /*.typeId=*/ QMetaType::UnknownType,
                    /*.metaObject=*/ original->metaObjectFn,
                    /*.name=*/ name,
                    /*.defaultCtr=*/ QtPrivate::QMetaTypeForType<JQObjectWrapper>::getDefaultCtr(),
                    /*.copyCtr=*/ QtPrivate::QMetaTypeForType<JQObjectWrapper>::getCopyCtr(),
                    /*.moveCtr=*/ QtPrivate::QMetaTypeForType<JQObjectWrapper>::getMoveCtr(),
                    /*.dtor=*/ QtPrivate::QMetaTypeForType<JQObjectWrapper>::getDtor(),
                    /*.equals=*/ [](const QtPrivate::QMetaTypeInterface *, const void *value1, const void *value2) -> bool{
                        const JQObjectWrapper* _value1 = reinterpret_cast<const JQObjectWrapper*>(value1);
                        const JQObjectWrapper* _value2 = reinterpret_cast<const JQObjectWrapper*>(value2);
                        return _value1->qobject()==_value2->qobject();
                    },
                    /*.lessThan=*/ [](const QtPrivate::QMetaTypeInterface *, const void *value1, const void *value2) -> bool{
                        const JQObjectWrapper* _value1 = reinterpret_cast<const JQObjectWrapper*>(value1);
                        const JQObjectWrapper* _value2 = reinterpret_cast<const JQObjectWrapper*>(value2);
                        return _value1->qobject()<_value2->qobject();
                    },
                    /*.debugStream=*/ original->debugStream ? [](const QtPrivate::QMetaTypeInterface *iface, QDebug &d, const void *value){
                        if(const QtPrivate::QMetaTypeInterface *original = getOriginalForNativeWrapperMetaType(iface)){
                            const JQObjectWrapper* _value = reinterpret_cast<const JQObjectWrapper*>(value);
                            original->debugStream(original, d, _value->qobject());
                        }
                    } : QtPrivate::QMetaTypeInterface::DebugStreamFn(nullptr),
                    /*.dataStreamOutFn=*/ original->dataStreamOut ? [](const QtPrivate::QMetaTypeInterface *iface, QDataStream &d, const void *value){
                        if(const QtPrivate::QMetaTypeInterface *original = getOriginalForNativeWrapperMetaType(iface)){
                            const JQObjectWrapper* _value = reinterpret_cast<const JQObjectWrapper*>(value);
                            original->dataStreamOut(original, d, _value->qobject());
                        }
                    } : QtPrivate::QMetaTypeInterface::DataStreamOutFn(nullptr),
                    /*.dataStreamInFn=*/ original->dataStreamIn ? [](const QtPrivate::QMetaTypeInterface *iface, QDataStream &d, void *value){
                        if(const QtPrivate::QMetaTypeInterface *original = getOriginalForNativeWrapperMetaType(iface)){
                            JQObjectWrapper* _value = reinterpret_cast<JQObjectWrapper*>(value);
                            original->dataStreamIn(original, d, _value->qobject());
                        }
                    } : QtPrivate::QMetaTypeInterface::DataStreamInFn(nullptr),
                    /*.legacyRegisterOp=*/ nullptr
                };
            }else{
                if(original->metaObjectFn)
                    originalMetaObject = original->metaObjectFn(original);
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                uint flags = QtPrivate::QMetaTypeTypeFlags<JObjectWrapper>::Flags;
#else
                uint flags = QtPrivate::QMetaTypeForType<JObjectWrapper>::flags();
#endif
                if(originalMetaObject)
                    flags |= QMetaType::IsGadget;
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
                if(metaType.flags().testFlag(QMetaType::IsConst))
                    flags |= QMetaType::IsConst;
#endif
                metaTypeInterface = new QtPrivate::QMetaTypeInterface{
                    /*.revision=*/ QMetaTypeInterface_CurrentRevision,
                    /*.alignment=*/ alignof(JObjectWrapper),
                    /*.size=*/ sizeof(JObjectWrapper),
                    /*.flags=*/ flags,
                    /*.typeId=*/ QMetaType::UnknownType,
                    /*.metaObject=*/ originalMetaObject ? &metaobjectByMetaTypeInterface : nullptr,
                    /*.name=*/ name,
                    /*.defaultCtr=*/ QtPrivate::QMetaTypeForType<JObjectWrapper>::getDefaultCtr(),
                    /*.copyCtr=*/ QtPrivate::QMetaTypeForType<JObjectWrapper>::getCopyCtr(),
                    /*.moveCtr=*/ QtPrivate::QMetaTypeForType<JObjectWrapper>::getMoveCtr(),
                    /*.dtor=*/ QtPrivate::QMetaTypeForType<JObjectWrapper>::getDtor(),
                    /*.equals=*/ [](const QtPrivate::QMetaTypeInterface *, const void *value1, const void *value2) -> bool{
                        const JObjectWrapper* _value1 = reinterpret_cast<const JObjectWrapper*>(value1);
                        const JObjectWrapper* _value2 = reinterpret_cast<const JObjectWrapper*>(value2);
                        if(JniEnvironment env{200}){
                            if (QSharedPointer<QtJambiLink> link1 = QtJambiLink::findLinkForJavaInterface(env, _value1->object(env))){
                                if (QSharedPointer<QtJambiLink> link2 = QtJambiLink::findLinkForJavaInterface(env, _value2->object(env))){
                                    return link1->pointer()==link2->pointer();
                                }
                            }
                        }
                        return false;
                    },
                    /*.lessThan=*/ [](const QtPrivate::QMetaTypeInterface *, const void *value1, const void *value2) -> bool{
                        const JObjectWrapper* _value1 = reinterpret_cast<const JObjectWrapper*>(value1);
                        const JObjectWrapper* _value2 = reinterpret_cast<const JObjectWrapper*>(value2);
                        if(JniEnvironment env{200}){
                            if (QSharedPointer<QtJambiLink> link1 = QtJambiLink::findLinkForJavaInterface(env, _value1->object(env))){
                                if (QSharedPointer<QtJambiLink> link2 = QtJambiLink::findLinkForJavaInterface(env, _value2->object(env))){
                                    return link1->pointer()<link2->pointer();
                                }
                            }
                        }
                        return false;
                    },
                    /*.debugStream=*/ original->debugStream ? [](const QtPrivate::QMetaTypeInterface *iface, QDebug &d, const void *value){
                        if(const QtPrivate::QMetaTypeInterface *original = getOriginalForNativeWrapperMetaType(iface)){
                            const JObjectWrapper* _value = reinterpret_cast<const JObjectWrapper*>(value);
                            if(JniEnvironment env{200}){
                                if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, _value->object(env))){
                                    original->debugStream(original, d, link->pointer());
                                }
                            }
                        }
                    } : QtPrivate::QMetaTypeInterface::DebugStreamFn(nullptr),
                    /*.dataStreamOutFn=*/ original->dataStreamOut ? [](const QtPrivate::QMetaTypeInterface *iface, QDataStream &d, const void *value){
                        if(const QtPrivate::QMetaTypeInterface *original = getOriginalForNativeWrapperMetaType(iface)){
                            const JObjectWrapper* _value = reinterpret_cast<const JObjectWrapper*>(value);
                            if(JniEnvironment env{200}){
                                if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, _value->object(env))){
                                    original->dataStreamOut(original, d, link->pointer());
                                }
                            }
                        }
                    } : QtPrivate::QMetaTypeInterface::DataStreamOutFn(nullptr),
                    /*.dataStreamInFn=*/ original->dataStreamIn ? [](const QtPrivate::QMetaTypeInterface *iface, QDataStream &d, void *value){
                        if(const QtPrivate::QMetaTypeInterface *original = getOriginalForNativeWrapperMetaType(iface)){
                            JObjectWrapper* _value = reinterpret_cast<JObjectWrapper*>(value);
                            if(JniEnvironment env{200}){
                                if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, _value->object(env))){
                                    original->dataStreamIn(original, d, link->pointer());
                                }
                            }
                        }
                    } : QtPrivate::QMetaTypeInterface::DataStreamInFn(nullptr),
                    /*.legacyRegisterOp=*/ nullptr
                };
            }
            {
                QWriteLocker locker(gRegistryLock());
                if(const QtPrivate::QMetaTypeInterface * iface = (*gJObjectNativeWrapperMetaTypes)[metaType.id()]){
                    delete metaTypeInterface;
                    nativeWrapperType = iface;
                }else{
                    (*gJObjectNativeWrapperMetaTypes)[metaType.id()] = metaTypeInterface;
                    (*gMetaTypesForJObjectNativeWrappers)[metaTypeInterface] = original;
                    if(originalMetaObject){
                        locker.unlock();
                        QMetaObjectBuilder builder;
                        builder.setClassName(name);
                        for(int i=0, l=originalMetaObject->methodCount(); i<l; ++i){
                            QMetaMethod method = originalMetaObject->method(i);
                            builder.addMethod(method);
                        }
                        for(int i=0, l=originalMetaObject->constructorCount(); i<l; ++i){
                            QMetaMethod constructor = originalMetaObject->constructor(i);
                            builder.addConstructor(constructor);
                        }
                        for(int i=0, l=originalMetaObject->propertyCount(); i<l; ++i){
                            QMetaProperty property = originalMetaObject->property(i);
                            builder.addProperty(property);
                        }
                        if(isQObject){
                                builder.setFlags(PropertyAccessInStaticMetaCall);
                                builder.setStaticMetacallFunction([](QObject *gadget, QMetaObject::Call call, int argc, void **argv){
                                    JObjectWrapper* wrapper = reinterpret_cast<JObjectWrapper*>(gadget);
                                    if(JniEnvironment env{200}){
                                        if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, wrapper->object(env))){
                                            Q_ASSERT(link->isQObject());
                                            link->qobject()->qt_metacall(call, argc, argv);
                                        }
                                    }
                                });
                        }else if(originalMetaObject->d.static_metacall){
                            builder.setFlags(PropertyAccessInStaticMetaCall);
                            builder.setStaticMetacallFunction([](QObject *gadget, QMetaObject::Call call, int argc, void **argv){
                                JObjectWrapper* wrapper = reinterpret_cast<JObjectWrapper*>(gadget);
                                if(JniEnvironment env{200}){
                                    if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, wrapper->object(env))){
                                        QMetaType metaType;
                                        if(const QMetaType* metaTypePtr = link->metaType()){
                                            metaType = *metaTypePtr;
                                        }else if(jclass object_class = env->GetObjectClass(wrapper->object(env))){
                                            QString qtName = QtJambiTypeManager::getInternalTypeName(env, object_class, false);
                                            metaType = QMetaType::fromName(qPrintable(qtName));
                                        }
                                        if(metaType.metaObject() && metaType.metaObject()->d.static_metacall)
                                            metaType.metaObject()->d.static_metacall(reinterpret_cast<QObject*>(link->pointer()), call, argc, argv);
                                    }
                                }
                            });
                        }
                        QMetaObject* mo = builder.toMetaObject();
                        mo->d.superdata.direct = nullptr;
#ifdef QT_NO_DATA_RELOCATION
                        mo->d.superdata.indirect = nullptr;
#endif
                        locker.relock();
                        gMetaTypeMetaObjectHash->insert(metaTypeInterface, mo);
                    }
                    nativeWrapperType = metaTypeInterface;
                    QMetaType nativeWrapperMetaType(nativeWrapperType);
                    (void)nativeWrapperMetaType.id();
                    locker.unlock();
                    if(!QMetaType::hasRegisteredConverterFunction(nativeWrapperMetaType, QMetaType::fromType<QString>())){
                        if(isQObject){
                            QMetaType::registerConverterFunction([metaType](const void *src, void *target) -> bool {
                                new (target)QString();
                                if(src){
                                    QObject* obj = reinterpret_cast<const JQObjectWrapper*>(src)->qobject();
                                    return QMetaType::convert(metaType, &obj, QMetaType::fromType<QString>(), target);
                                }
                                return true;
                            }, nativeWrapperMetaType, QMetaType::fromType<QString>());
                        }else{
                            QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                                bool ok = false;
                                if(src){
                                    new (target)QString(reinterpret_cast<const JObjectWrapper*>(src)->toString(&ok));
                                }else{
                                    ok = true;
                                    new (target)QString();
                                }
                                return ok;
                            }, nativeWrapperMetaType, QMetaType::fromType<QString>());
                        }
                    }
                    locker.relock();
                }
            }
        }
    }
    return QMetaType(nativeWrapperType);
}

jclass registeredJavaClassForCustomMetaType(const QtPrivate::QMetaTypeInterface * metaType){
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gMetaTypeJavaTypeHash->value(metaType);
}

const char* registeredClassNameForFunctionalMetaType(const QtPrivate::QMetaTypeInterface * metaType){
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gMetaTypeJavaNameHash->value(metaType);
}

jclass registeredJavaClassForCustomMetaType(QMetaType metaType){
    return registeredJavaClassForCustomMetaType(metaType.iface());
}

const char* registeredClassNameForFunctionalMetaType(QMetaType metaType){
    return registeredClassNameForFunctionalMetaType(metaType.iface());
}

const QMetaObject *findWrappersMetaObject(const QtPrivate::QMetaTypeInterface *iface){
    const QMetaObject* meta_object = nullptr;
    if(JniEnvironment env{200}){
        try {
            jclass clazz = nullptr;
            {
                QReadLocker locker(gRegistryLock());
                clazz = (*gMetaTypeJavaTypeHash)[iface];
            }
            if(clazz){
                meta_object = CoreAPI::metaObjectForClass(env, clazz);
                if(meta_object){
                    registerMetaObjectByMetaTypeInterface(iface, meta_object);
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
                    const_cast<QtPrivate::QMetaTypeInterface*>(iface)->metaObjectFn = &metaobjectByMetaTypeInterface;
#else
                    const_cast<QtPrivate::QMetaTypeInterface::MetaObjectFn&>(iface->metaObjectFn) = &metaobjectByMetaTypeInterface;
#endif
                }else{
                    const_cast<QtPrivate::QMetaTypeInterface*>(iface)->flags &= ~(QMetaType::IsGadget | QMetaType::PointerToGadget);
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
                    const_cast<QtPrivate::QMetaTypeInterface*>(iface)->metaObjectFn = [](const QtPrivate::QMetaTypeInterface*)->const QMetaObject*{return nullptr;};
#else
                    const_cast<QtPrivate::QMetaTypeInterface::MetaObjectFn&>(iface->metaObjectFn) = [](const QtPrivate::QMetaTypeInterface*)->const QMetaObject*{return nullptr;};
#endif
                }
            }
        }catch(const JavaException&){
        }
    }
    return meta_object;
}

QList<QMetaType> registeredCustomMetaTypesForJavaClass(const QByteArray& javaClass){
    QReadLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    return gJavaTypeMetaTypesHash->values(javaClass);
}

QList<jclass> getFlatClassHirarchy(JNIEnv *env, jclass clazz){
    QList<jclass> hirarchy;
    {
        QSet<jint> classHashes;
        classHashes.insert(Java::Runtime::Object::hashCode(env, clazz));
        hirarchy << clazz;
        jint hash = 0;
        QList<jclass> superTypes{clazz};
        while(!superTypes.isEmpty()){
            jclass cls = superTypes.takeFirst();
            JConstObjectArrayPointer<jclass> interfaces(env, Java::Runtime::Class::getInterfaces(env, cls));
            for(int i=0; i<interfaces.length(); ++i){
                if(jclass interfaceClass = interfaces[i]){
                    if(!classHashes.contains(hash = Java::Runtime::Object::hashCode(env, interfaceClass))){
                        classHashes.insert(hash);
                        hirarchy << interfaceClass;
                    }
                    superTypes << interfaceClass;
                }
            }
            if(!Java::Runtime::Class::isInterface(env, cls)){
                jclass sc = env->GetSuperclass(cls);
                if(sc && !Java::QtJambi::QtObject::isSameClass(env, sc)
                      && !Java::Runtime::Object::isSameClass(env, sc)){
                    if(!classHashes.contains(hash = Java::Runtime::Object::hashCode(env, sc))){
                        hirarchy << sc;
                        classHashes.insert(hash);
                    }
                    superTypes << sc;
                }
            }
        }
    }
    return hirarchy;
}

QList<const char*> CoreAPI::getInterfaceIIDs(JNIEnv *env, jclass javaType){
    QList<const char*> iids;
    if(javaType){
        const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, javaType);
        for(const SuperTypeInfo& superTypeInfo : superTypeInfos){
            if(const char* iid = superTypeInfo.interfaceID())
                iids << iid;
        }
        QString className;
        const QList<jclass> hirarchy = getFlatClassHirarchy(env, javaType);
        for(jclass sc : hirarchy){
            className = QtJambiAPI::getClassName(env, sc).replace('.', '/');
            if(const char* iid = registeredInterfaceIDForClassName(className)){
                if(!iids.contains(iid))
                    iids << iid;
            }
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
            QReadLocker locker(gRegistryLock());
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
                QReadLocker locker(gRegistryLock());
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
            auto unique = unique_id(*typeId);
            QReadLocker locker(gRegistryLock());
            Q_UNUSED(locker)
            return gMediaControlIIDHash->value(unique, nullptr);
        }
    }
    return nullptr;
}

jclass RegistryAPI::classByMediaControlIID(JNIEnv *env, const char* iid){
    if(env && iid){
        const std::type_info* typeId(nullptr);
        {
            QReadLocker locker(gRegistryLock());
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
        qCWarning(DebugAPI::internalCategory, "findClass(\"%s\") with Exception pending", qualifiedName);
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
            if(jobject currentThread = Java::Runtime::Thread::currentThread(env)){
                if(jobject contextClassLoader = Java::Runtime::Thread::getContextClassLoader(env, currentThread)){
                    returned = Java::Runtime::ClassLoader::tryLoadClass(env, contextClassLoader, className);
                    if (env->ExceptionCheck() || returned == nullptr) {
                        env->ExceptionClear();
                        classLoader = nullptr;
                    }
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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
jclass getGlobalClassRef(JNIEnv *env, jclass cls, const char *className){
#else
jclass getGlobalClassRef(JNIEnv *env, jclass cls, QByteArrayView className){
#endif
#ifndef QTJAMBI_NOCACHE
    size_t key;
#ifdef QTJAMBI_LOG_CLASSNAMES
    QString classNameStrg;
#endif
    if(className!=nullptr){
#ifdef QTJAMBI_LOG_CLASSNAMES
        classNameStrg = QLatin1String(className);
#endif
        key = qHash(className);
    }else{
#ifndef QTJAMBI_LOG_CLASSNAMES
        QString
#endif
            classNameStrg = qtjambi_cast<QString>(env, Java::Runtime::Class::getName(env, cls)).replace(".", "/");
        key = qHash(qPrintable(classNameStrg));
    }
    {
        QReadLocker locker(gRegistryLock());
        Q_UNUSED(locker)
        if(gClassHash->contains(key))
            return gClassHash->value(key);
    }
    bool deleteGlobalRef = false;
    if(env->GetObjectRefType(cls)!=JNIGlobalRefType){
        cls = jclass(env->NewGlobalRef(cls));
        deleteGlobalRef = true;
    }
    QWriteLocker locker(gRegistryLock());
    Q_UNUSED(locker)
    if(gClassHash->contains(key)) { // if it has been inserted meanwhile by other thread
        if(deleteGlobalRef)
            env->DeleteGlobalRef(cls);
        return gClassHash->value(key);
    }
#ifdef QTJAMBI_LOG_CLASSNAMES
    gClassNameHash->insert(key, classNameStrg);
#endif
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
    size_t key = qHash(qPrintable( className ));
    jclass cachedClass(nullptr);
    {
        QReadLocker locker(gRegistryLock());
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
    size_t key = qHash(className);

    {
        QReadLocker locker(gRegistryLock());
        Q_UNUSED(locker)
        returned = gClassHash->value(key, nullptr);
    }

    if (returned == nullptr) {
#endif // QTJAMBI_NOCACHE
        if(className && !QByteArrayView(className).contains("$Lambda$") && !QByteArrayView(className).contains("$$Lambda/") && !QLatin1String(className).isEmpty()){
            returned = findClass(env, className, classLoader);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            returned = getGlobalClassRef(env, returned, className);
#ifndef QTJAMBI_NOCACHE
        }
#endif // QTJAMBI_NOCACHE
    }

    return returned;
}

size_t computeHash(JNIEnv* env, jclass clazz, const char *memberName, const char *signature, bool isStatic) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if(signature)
        return qHashMulti(0, clazz ? Java::Runtime::Object::hashCode(env, clazz) : 0, memberName, signature, isStatic);
    else
        return qHashMulti(0, clazz ? Java::Runtime::Object::hashCode(env, clazz) : 0, memberName, isStatic);
#else
    QList<size_t> range;
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
    size_t key = computeHash(env,clazz,fieldName,nullptr,isStatic);
    {
        QReadLocker locker(gRegistryLock());
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
        QWriteLocker locker(gRegistryLock());
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
        size_t key = computeHash(env,clazz,methodName,signature,isStatic);
        {
            QReadLocker locker(gRegistryLock());
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
        QWriteLocker locker(gRegistryLock());
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
            QReadLocker locker(gRegistryLock());
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
                QWriteLocker locker(gRegistryLock());
                Q_UNUSED(locker)
                if (!gQtSuperclassHash->contains(key)) {
#ifdef QTJAMBI_COUNTCACHEMISSES
                    locker.unlock();
                    QString className = QtJambiAPI::getClassName(env, returned).replace('.', '/');
                    locker.relock();
                    cacheMisses(qPrintable(className));
#endif
                    gQtSuperclassHash->insert(key, returned);
                }else{
                    returned = gQtSuperclassHash->value(key, nullptr);
                }
            }
            if (!returned) {
                QWriteLocker locker(gRegistryLock());
                Q_UNUSED(locker)
                if (!gQtSuperclassHash->contains(key)) {
#ifdef QTJAMBI_COUNTCACHEMISSES
                    locker.unlock();
                    QString className = QtJambiAPI::getClassName(env, returned).replace('.', '/');
                    cacheMisses(qPrintable(className));
                    locker.relock();
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
    const SuperTypeInfos infos = SuperTypeInfos::fromClass(env, object_class);
    if(!infos.isEmpty()){
        for(const SuperTypeInfo& info : infos){
            size += info.size();
            size += sizeof(void*);
        }
    }
    return size;
}

typedef SecureContainer<QList<void**>, gRegistryLock> GlobalClassPointers;
Q_GLOBAL_STATIC(GlobalClassPointers, gGlobalClassPointers)

void QtJambiPrivate::registerGlobalClassPointer(jclass& cls){
    QWriteLocker locker(gRegistryLock());
    gGlobalClassPointers->append(reinterpret_cast<void**>(&cls));
}

void unregisterGlobalClassPointer(jclass& cls){
    QWriteLocker locker(gRegistryLock());
    gGlobalClassPointers->removeAll(reinterpret_cast<void**>(&cls));
}

void clearRegistryAtShutdown(JNIEnv * env){
#ifndef QTJAMBI_NOCACHE
    QHash<size_t, jclass> classIdHash;
#endif
    QList<void**> globalClassPointers;
    QMultiHash<size_t, const PolymorphicIdHandler* > polymorphic_ids;
    QMap<int,QSharedPointer<AbstractContainerAccess>> containerAccessMap;
    {
        QWriteLocker locker(gRegistryLock());
        if(!gGlobalClassPointers.isDestroyed())
            globalClassPointers.swap(*gGlobalClassPointers);
        if(!g_polymorphic_ids.isDestroyed())
            polymorphic_ids.swap(*g_polymorphic_ids);
        if(!gExtraSignalsOfMetaObject.isDestroyed())
            gExtraSignalsOfMetaObject->clear();
        if(!gContainerAccessMap.isDestroyed())
            gContainerAccessMap->swap(containerAccessMap);
#ifndef QTJAMBI_NOCACHE
        if(!gClassHash.isDestroyed())
            classIdHash.swap(*gClassHash);
        if(!gConstructorHash.isDestroyed())
            gConstructorHash->clear();
        if(!gQtSuperclassHash.isDestroyed())
            gQtSuperclassHash->clear();
        if(!gMethodHash.isDestroyed())
            gMethodHash->clear();
        if(!gFieldHash.isDestroyed())
            gFieldHash->clear();
#endif
    }
    for(const PolymorphicIdHandler* handler : polymorphic_ids){
        delete handler;
    }
#ifndef QTJAMBI_NOCACHE
    if(env && Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.purge-globalrefs-at-shutdown"))){
        for(void** ptr : globalClassPointers){
            *ptr = nullptr;
        }
#ifdef QTJAMBI_LOG_CLASSNAMES
        printf("DeleteGlobalRef of %i classes:\n", classIdHash.keys().size());
        int counter = 0;
#endif
#ifdef QTJAMBI_LOG_CLASSNAMES
        for(size_t key : classIdHash.keys()){
            jclass cls = classIdHash[key];
                printf("%i.: class %s\n", ++counter, qPrintable((*gClassNameHash)[key]));
#else
        for(jclass cls : classIdHash){
#endif
            if(env->GetObjectRefType(cls)==JNIGlobalRefType)
                env->DeleteGlobalRef(cls);
        }
    }
#endif // QTJAMBI_NOCACHE
}

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
            jobject o = env->CallObjectMethod(other.object(env), methods->clone);
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

typedef SecureContainer<QMap<size_t, const QtPrivate::QMetaTypeInterface *>, gRegistryLock> ClassMetaTypeInterfaceHash;
Q_GLOBAL_STATIC(ClassMetaTypeInterfaceHash, gClassMetaTypeInterfaces)
typedef SecureContainer<QMap<const QtPrivate::QMetaTypeInterface *,JObjectValueWrapperPrivate>, gRegistryLock> MetaTypeInterfacesHash;
Q_GLOBAL_STATIC(MetaTypeInterfacesHash, gMetaTypeInterfaces)
typedef SecureContainer<QMap<const QtPrivate::QMetaTypeInterface *,jclass>, gRegistryLock> MetaTypeEnumClassesHash;
Q_GLOBAL_STATIC(MetaTypeEnumClassesHash, gMetaTypeEnumClasses)

bool JObjectValueWrapper::isValueType(QMetaType metaType){
    const JObjectValueWrapperPrivate* methods{nullptr};
    {
        auto iface = metaType.iface();
        QReadLocker locker(gRegistryLock());
        if(gMetaTypeInterfaces->contains(iface))
            methods = &(*gMetaTypeInterfaces)[iface];
    }
    return methods;
}

bool JObjectValueWrapper::hasCustomDebugStreamOperator(QMetaType metaType){
    return metaType.iface()->debugStream!=JObjectValueWrapper::debugStream;
}

JObjectValueWrapper JObjectValueWrapper::create(JNIEnv* env, jobject object, QMetaType metaType){
    const JObjectValueWrapperPrivate* methods{nullptr};
    {
        auto iface = metaType.iface();
        QReadLocker locker(gRegistryLock());
        if(gMetaTypeInterfaces->contains(iface))
            methods = &(*gMetaTypeInterfaces)[iface];
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
    jobject o = env->CallObjectMethod(object(env), p->clone);
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

void JObjectValueWrapper::assign(JNIEnv* env, JObjectValueWrapper &&wrapper){
    JObjectWrapper::assign(env, std::move(wrapper));
    p = wrapper.p;
}

void JObjectValueWrapper::assign(const JObjectWrapper &wrapper){
    if(const JObjectValueWrapper* _wrapper = dynamic_cast<const JObjectValueWrapper*>(&wrapper)){
        operator=(*_wrapper);
    }else if(p){
        if(JniEnvironment env{200}){
            if(env->IsInstanceOf(wrapper.object(env), p->clazz)){
                operator=(JObjectValueWrapper(qtjambiCopyObject(env, p, wrapper.object(env)), p));
            }
        }
    }
}

void JObjectValueWrapper::assign(JNIEnv* env, JObjectWrapper &&wrapper){
    if(JObjectValueWrapper* _wrapper = dynamic_cast<JObjectValueWrapper*>(&wrapper)){
        assign(env, std::move(*_wrapper));
    }else{
        if(env->IsInstanceOf(wrapper.object(env), p->clazz)){
            assign(env, JObjectValueWrapper(std::move(wrapper), p));
        }
    }
}

void JObjectValueWrapper::assign(JNIEnv* env, jobject obj){
    if(p){
        if(env->IsInstanceOf(obj, p->clazz)){
            JObjectWrapper::assign(env, obj);
        }
    }
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
            env->CallVoidMethod(object(env), p->writeTo, _d);
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
            env->CallVoidMethod(object(env), p->readFrom, _d);
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
            Java::QtJambi::MetaTypeUtility::writeSerializableJavaObject(env, _d, object(env));
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
        QReadLocker locker(gRegistryLock());
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
        QReadLocker locker(gRegistryLock());
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
            QReadLocker locker(gRegistryLock());
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
            QReadLocker locker(gRegistryLock());
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
            QReadLocker locker(gRegistryLock());
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
                QReadLocker locker(gRegistryLock());
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
                QReadLocker locker(gRegistryLock());
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
                QReadLocker locker(gRegistryLock());
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
                QReadLocker locker(gRegistryLock());
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

struct JQObjectWrapperPrivate{
    JQObjectWrapperPrivate(QObject*& object, JNIEnv* env, QSharedPointer<QtJambiLink>&& link);
    void unref(QObject*& pointer);
    void ref(QObject*& pointer);
    void exchange(QObject*& pointer, QObject*& byPointer);
    QSharedPointer<QtJambiLink> m_link;
    QMutex m_mutex;
    QSet<QObject**> m_pointers;
private:
    ~JQObjectWrapperPrivate();
};

JQObjectWrapperPrivate::JQObjectWrapperPrivate(QObject*& object, JNIEnv* env, QSharedPointer<QtJambiLink>&& link)
    : m_link(std::move(link)), m_mutex(), m_pointers() {
    Q_ASSERT(!m_link.isNull());
    Q_ASSERT(m_link->ownership()==QtJambiLink::Ownership::Java);
    m_link->setCppOwnership(env);
    m_pointers.insert(&object);
    if(DependencyManagerUserData* dm = DependencyManagerUserData::instance(object)){
        dm->addFinalization(this,
            [](JNIEnv*, void* data){
                JQObjectWrapperPrivate* p = reinterpret_cast<JQObjectWrapperPrivate*>(data);
                QSet<QObject**> pointers;
                {
                    QMutexLocker<QMutex> locker(&p->m_mutex);
                    p->m_pointers.swap(pointers);
                }
                for(QObject** pointer : std::as_const(pointers)){
                    *pointer = nullptr;
                }
            }, nullptr);
    }
}

void JQObjectWrapperPrivate::unref(QObject*& pointer){
    bool isEmpty;
    {
        QMutexLocker<QMutex> locker(&m_mutex);
        m_pointers.remove(&pointer);
        isEmpty = m_pointers.isEmpty();
    }
    if(isEmpty)
        delete this;
}

void JQObjectWrapperPrivate::ref(QObject*& pointer){
    {
        QMutexLocker<QMutex> locker(&m_mutex);
        m_pointers.insert(&pointer);
    }
}

void JQObjectWrapperPrivate::exchange(QObject*& pointer, QObject*& byPointer){
    {
        QMutexLocker<QMutex> locker(&m_mutex);
        m_pointers.remove(&pointer);
        m_pointers.insert(&byPointer);
    }
}

JQObjectWrapperPrivate::~JQObjectWrapperPrivate(){
    try{
        if(QObject* object = m_link->qobject()){
            JniEnvironment env{200};
            try{
                if(DependencyManagerUserData* dm = DependencyManagerUserData::instance(object, false)){
                    dm->removeFinalization(env, this);
                }
                if(!isQmlJavaScriptOwnership(object) && env)
                    m_link->setJavaOwnership(env);
            }catch(const JavaException& exn){
                exn.report(env);
            }
        }
    }catch(...){
    }
}

JQObjectWrapper::JQObjectWrapper()
    : m_qobject(nullptr), p(nullptr) {
}

JQObjectWrapper::JQObjectWrapper(JNIEnv* env, QSharedPointer<QtJambiLink>&& link)
    : m_qobject(link->qobject()), p(new JQObjectWrapperPrivate(m_qobject, env, std::move(link))){
}

JQObjectWrapper::~JQObjectWrapper(){
    if(p)
        p->unref(m_qobject);
    m_qobject = nullptr;
    p = nullptr;
}

JQObjectWrapper::JQObjectWrapper(const JQObjectWrapper& other)
    : m_qobject(other.m_qobject), p(other.p){
    if(p)
        p->ref(m_qobject);
}

JQObjectWrapper::JQObjectWrapper(JQObjectWrapper&& other)
    : m_qobject(other.m_qobject), p(other.p){
    if(p)
        p->exchange(other.m_qobject, m_qobject);
    other.p = nullptr;
    other.m_qobject = nullptr;
}

JQObjectWrapper& JQObjectWrapper::operator=(const JQObjectWrapper &other){
    if(other.p!=p){
        if(p)
            p->unref(m_qobject);
        m_qobject = other.m_qobject;
        p = other.p;
        if(p){
            p->ref(m_qobject);
        }
    }
    return *this;
}

JQObjectWrapper& JQObjectWrapper::operator=(JQObjectWrapper &&other){
    if(other.p!=p){
        if(p)
            p->unref(m_qobject);
        m_qobject = other.m_qobject;
        p = other.p;
        if(p){
            p->exchange(other.m_qobject, m_qobject);
        }
        other.p = nullptr;
        other.m_qobject = nullptr;
    }
    return *this;
}

QSharedPointer<QtJambiLink> JQObjectWrapper::link() const{
    if(p)
        return  p->m_link;
    else
        return {};
}

jobject JQObjectWrapper::javaObject(JNIEnv* env) const{
    if(p)
        return  p->m_link->getJavaObjectLocalRef(env);
    else
        return nullptr;
}


int QmlAPI::registerMetaType(JNIEnv *env, SequentialContainerType containerType, const QMetaType& elementType){
    int result = 0;
    AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(ContainerAccessAPI::createContainerAccess(containerType, elementType));
    if(!listAccess){
        QtJambiUtils::QHashFunction hashFunction;
        QtJambiUtils::InternalToExternalConverter internalToExternalConverter;
        QtJambiUtils::ExternalToInternalConverter externalToInternalConverter;
        const JObjectValueWrapperPrivate* methods{nullptr};
        {
            const QtPrivate::QMetaTypeInterface *iface = QMetaType(elementType).iface();
            QReadLocker locker(gRegistryLock());
            if(gMetaTypeInterfaces->contains(iface))
                methods = &(*gMetaTypeInterfaces)[iface];
        }
        if(methods){
            hashFunction = [](const void* ptr, size_t seed)->size_t{
                if(JniEnvironment env{200}){
                    try{
                        seed = qHash(Java::Runtime::Object::hashCode(env, reinterpret_cast<const JObjectValueWrapper*>(ptr)->object(env)), seed);
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
                auto id = elementType.id();
                QReadLocker locker(gRegistryLock());
                typeId = gMetaTypeIDHash->value(id);
            }
            if(typeId)
                hashFunction = registeredHashFunction(*typeId);
            if(jclass clazz = CoreAPI::getClassForMetaType(env, elementType)){
                internalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(env, QLatin1String(elementType.name()), elementType, clazz);
                externalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(env, clazz, QLatin1String(elementType.name()), elementType);
            }
        }
        QSharedPointer<AbstractContainerAccess> memberNestedContainerAccess = findContainerAccess(elementType);
        const std::type_info* typeId = getTypeByQtName(elementType.name());
        if(!typeId){
            typeId = getTypeByMetaType(elementType);
        }
        PtrOwnerFunction memberOwnerFunction = nullptr;
        if(typeId)
            memberOwnerFunction = ContainerAPI::registeredOwnerFunction(*typeId);
        listAccess = dynamic_cast<AbstractListAccess*>(ContainerAccessAPI::createContainerAccess(
                                                                           env,
                                                                           containerType,
                                                                           elementType,
                                                                           elementType.alignOf(),
                                                                           elementType.sizeOf(),
                                                                           false,
                                                                           hashFunction,
                                                                           internalToExternalConverter,
                                                                           externalToInternalConverter,
                                                                           memberNestedContainerAccess,
                                                                           memberOwnerFunction
                                                                           ));
    }
    if(listAccess){
        QString containerName;
        switch(containerType){
        case SequentialContainerType::QSet:
            containerName = QStringLiteral("QSet");
            break;
        case SequentialContainerType::QQueue:
            containerName = QStringLiteral("QQueue");
            break;
        case SequentialContainerType::QStack:
            containerName = QStringLiteral("QStack");
            break;
        default:
            containerName = QStringLiteral("QList");
            break;
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
            /*.revision=*/ QMetaTypeInterface_CurrentRevision,
            /*.alignment=*/ alignof(EnumOrFlags),
            /*.size=*/ sizeof(EnumOrFlags),
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
            /*.flags=*/ uint(QtPrivate::QMetaTypeTypeFlags<EnumOrFlags>::Flags),
#else
            /*.flags=*/ uint(QtPrivate::QMetaTypeForType<EnumOrFlags>::flags()),
#endif
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
    const QtPrivate::QMetaTypeInterface * iface;
    {
        auto hash = qHash(javaClassName);
        {
            QReadLocker locker(gRegistryLock());
            iface = gClassMetaTypeInterfaces->value(hash);
        }
        if(iface){
            delete metaTypeInterface;
            return iface->typeId;
        }else{
            jclass cls = getGlobalClassRef(env, clazz, qPrintable(javaClassName));
            QWriteLocker locker(gRegistryLock());
            iface = gClassMetaTypeInterfaces->value(hash);
            if(iface){
                delete metaTypeInterface;
                return iface->typeId;
            }
            gClassMetaTypeInterfaces->insert(hash, metaTypeInterface);
            (*gMetaTypeEnumClasses)[metaTypeInterface] = cls;
        }
    }
    QMetaType metaType(metaTypeInterface);
    int result = metaType.id();
    auto conv = [metaTypeInterface](const void *src, void *target) -> bool {
        if(src){
            jclass clazz;
            {
                QReadLocker locker(gRegistryLock());
                clazz = (*gMetaTypeEnumClasses)[metaTypeInterface];
            }
            if(clazz){
                if(JniEnvironment env{200}){
                    try{
                        if constexpr(isQtEnum0OrFlag1==0){
                            new (target)Wrapper(CoreAPI::convertEnumToJavaObject(env, *reinterpret_cast<const INT*>(src), clazz));
                        }else if constexpr(isQtEnum0OrFlag1==1){
                            typename std::conditional<sizeof(EnumOrFlags)==sizeof(jlong), jlong, jint>::type value = *reinterpret_cast<const INT*>(src);
                            new (target)Wrapper(CoreAPI::convertQFlagsToJavaObject(env, value, clazz));
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
    if(!QMetaType::hasRegisteredConverterFunction(metaType, QMetaType::fromType<JObjectWrapper>()))
        QMetaType::registerConverterFunction(conv, metaType, QMetaType::fromType<JObjectWrapper>());
    if constexpr(isQtEnum0OrFlag1!=1){
        if(!QMetaType::hasRegisteredConverterFunction(metaType, QMetaType::fromType<JEnumWrapper>()))
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
    if(!QMetaType::hasRegisteredConverterFunction(metaType, QMetaType::fromType<typename QIntegerForSizeof<EnumOrFlags>::Signed>()))
        QMetaType::registerConverterFunction(iconv, metaType, QMetaType::fromType<typename QIntegerForSizeof<EnumOrFlags>::Signed>());
    if(!QMetaType::hasRegisteredConverterFunction(metaType, QMetaType::fromType<typename QIntegerForSizeof<EnumOrFlags>::Unsigned>()))
        QMetaType::registerConverterFunction(iconv, metaType, QMetaType::fromType<typename QIntegerForSizeof<EnumOrFlags>::Unsigned>());
    if(!QMetaType::hasRegisteredConverterFunction(QMetaType::fromType<typename QIntegerForSizeof<EnumOrFlags>::Signed>(), metaType))
        QMetaType::registerConverterFunction(iconv, QMetaType::fromType<typename QIntegerForSizeof<EnumOrFlags>::Signed>(), metaType);
    if(!QMetaType::hasRegisteredConverterFunction(QMetaType::fromType<typename QIntegerForSizeof<EnumOrFlags>::Unsigned>(), metaType))
        QMetaType::registerConverterFunction(iconv, QMetaType::fromType<typename QIntegerForSizeof<EnumOrFlags>::Unsigned>(), metaType);
    registerJavaClassForCustomMetaType(env, metaType, clazz);
    return result;
}

int registerMetaType(JNIEnv *env, jclass clazz, jboolean isPointer, jboolean isReference, const QString* javaClassNamePtr, int superId = QMetaType::UnknownType)
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
                auto hashcode = qHash(javaClassName);
                QReadLocker locker(gRegistryLock());
                if(const QtPrivate::QMetaTypeInterface * iface = gClassMetaTypeInterfaces->value(hashcode)){
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
                    Q_FALLTHROUGH();
                case EntryTypes::ObjectTypeInfo:
                case EntryTypes::InterfaceTypeInfo:
                case EntryTypes::FunctionalTypeInfo: {
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                    flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<void*>::Flags);
#else
                    flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<void*>::flags());
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                    flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<Qt::Orientation>::Flags);
#else
                    flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<Qt::Orientation>::flags());
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                                                             /*.flags=*/ QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<E>::Flags),
#else
                                                             /*.flags=*/ QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<E>::flags()),
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                                                        /*.flags=*/ QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<QFlags<E>>::Flags),
#else
                                                        /*.flags=*/ QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<QFlags<E>>::flags()),
#endif
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
                const SuperTypeInfos superTypes = SuperTypeInfos::fromClass(env, clazz);
                QByteArray metaTypeName = javaClassName.toUtf8().replace("/", "::").replace("$", "::");
                if(superTypes.isEmpty())
                {
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
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                            uint flags = QtPrivate::QMetaTypeTypeFlags<JObjectValueWrapper>::Flags;
#else
                            uint flags = QtPrivate::QMetaTypeForType<JObjectValueWrapper>::flags();
#endif
                            flags |= QMetaType::IsGadget;
                            QtPrivate::QMetaTypeInterface* metaTypeInterface = new QtPrivate::QMetaTypeInterface{
                                    /*.revision=*/ QMetaTypeInterface_CurrentRevision,
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
                                auto hashcode = qHash(javaClassName);
                                QWriteLocker locker(gRegistryLock());
                                if(const QtPrivate::QMetaTypeInterface * iface = gClassMetaTypeInterfaces->value(hashcode)){
                                    delete metaTypeInterface;
                                    return iface->typeId;
                                }
                                gClassMetaTypeInterfaces->insert(hashcode, metaTypeInterface);
                                (*gMetaTypeInterfaces)[metaTypeInterface] = std::move(p);
                            }
                            QMetaType metaType(metaTypeInterface);
                            int result = metaType.id();
                            if(!QMetaType::hasRegisteredConverterFunction(metaType, QMetaType::fromType<QString>())){
                                QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                                    bool ok = false;
                                    if(src){
                                        new (target)QString(reinterpret_cast<const JObjectValueWrapper*>(src)->toString(&ok));
                                    }else{
                                        ok = true;
                                        new (target)QString();
                                    }
                                    return ok;
                                }, metaType, QMetaType::fromType<QString>());
                            }
                            if(!QMetaType::hasRegisteredConverterFunction(metaType, QMetaType::fromType<JObjectWrapper>())){
                                QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                                    if(src){
                                        new (target)JObjectWrapper(*reinterpret_cast<const JObjectValueWrapper*>(src));
                                    }else{
                                        new (target)JObjectWrapper();
                                    }
                                    return true;
                                }, metaType, QMetaType::fromType<JObjectWrapper>());
                            }
                            if(!QMetaType::hasRegisteredConverterFunction(QMetaType::fromType<JObjectWrapper>(), metaType)){
                                QMetaType::registerConverterFunction([metaType](const void *src, void *target) -> bool {
                                    if(src){
                                        if(JniEnvironment env{200}){
                                            jobject value = reinterpret_cast<const JObjectWrapper*>(src)->object(env);
                                            if(value!=nullptr){
                                                const JObjectValueWrapperPrivate* methods{nullptr};
                                                {
                                                    auto iface = QMetaType(metaType).iface();
                                                    QReadLocker locker(gRegistryLock());
                                                    if(gMetaTypeInterfaces->contains(iface))
                                                        methods = &(*gMetaTypeInterfaces)[iface];
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
                            }
                            registerJavaClassForCustomMetaType(env, metaType, clazz);
                            registerConverterVariant(env, metaType, QLatin1String(metaTypeInterface->name), javaClassName, clazz);
                            return result;
                        }
                        if(Java::Runtime::Collection::isAssignableFrom(env, clazz)){
                            return QMetaType::fromType<JCollectionWrapper>().id();
                        }else if(Java::Runtime::Map::isAssignableFrom(env, clazz)){
                            return QMetaType::fromType<JMapWrapper>().id();
                        }
                        metaTypeName = "JObjectWrapper<" + metaTypeName + ">";
                        QMetaType metaType = createMetaType(metaTypeName,
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
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                                                                QtPrivate::QMetaTypeTypeFlags<JObjectWrapper>::Flags
#else
                                                                QtPrivate::QMetaTypeForType<JObjectWrapper>::flags()
#endif
                                                                | QMetaType::IsGadget
                                                                ),
                                                            nullptr, &findWrappersMetaObject);
                        int result = metaType.id();
                        registerJavaClassForCustomMetaType(env, metaType, clazz, true);
                        if(!QMetaType::hasRegisteredConverterFunction(metaType, QMetaType::fromType<QString>())){
                            QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                                bool ok = false;
                                if(src){
                                    new (target)QString(reinterpret_cast<const JObjectWrapper*>(src)->toString(&ok));
                                }else{
                                    ok = true;
                                    new (target)QString();
                                }
                                return ok;
                            }, metaType, QMetaType::fromType<QString>());
                        }
                        registerConverterVariant(env, metaType, QLatin1String(metaTypeName), javaClassName, clazz);
                        return result;
                    }
#endif
                }else{
                    int id = QMetaType::UnknownType;
                    if(superId!=QMetaType::UnknownType){
                        jclass cls = CoreAPI::getClassForMetaType(env, QMetaType(superId));
                        if(env->IsSameObject(cls, superTypes[0].javaClass())){
                            id = superId;
                        }
                    }
                    if(id==QMetaType::UnknownType)
                        id = registerMetaType(env, superTypes[0].javaClass(), isPointer, isReference, &superTypes[0].className());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    int typeId = QMetaType::registerNormalizedTypedef(metaTypeName, id);
                    RegistryAPI::registerComparator(new QtPrivate::BuiltInEqualsComparatorFunction<JObjectWrapper>(), typeId);
                    return typeId;
#else
                    if(id!=QMetaType::UnknownType){
                        QMetaType superMetaType(id);
                        QSharedPointer<AbstractContainerAccess> access = findContainerAccess(id);
                        if(superMetaType.flags() & QMetaType::IsPointer){
                            metaTypeName = metaTypeName + "*";
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
#if 1
                        }else if(access){
                            metaTypeName = metaTypeName + "*";
                            QMetaType metaType = createMetaType(metaTypeName,
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
                                                                /*.flags=*/ QMetaType::TypeFlags(
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                                                                        QtPrivate::QMetaTypeTypeFlags<void*>::Flags
#else
                                                                        QtPrivate::QMetaTypeForType<void*>::flags()
#endif
                                                                        | QMetaType::IsGadget
                                                                        | QMetaType::PointerToGadget
                                                                        ),
                                                                nullptr, &findWrappersMetaObject);
                            id = metaType.id();
                            registerJavaClassForCustomMetaType(env, metaType, clazz);
                            registerConverterVariant(env, metaType, QLatin1String(metaTypeName), javaClassName, clazz);
                            return id;
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                                uint flags = QtPrivate::QMetaTypeTypeFlags<JObjectValueWrapper>::Flags;
#else
                                uint flags = QtPrivate::QMetaTypeForType<JObjectValueWrapper>::flags();
#endif
                                flags |= QMetaType::IsGadget;
                                QtPrivate::QMetaTypeInterface* metaTypeInterface = new QtPrivate::QMetaTypeInterface{
                                        /*.revision=*/ QMetaTypeInterface_CurrentRevision,
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
                                    auto hashcode = qHash(javaClassName);
                                    QWriteLocker locker(gRegistryLock());
                                    if(const QtPrivate::QMetaTypeInterface * iface = gClassMetaTypeInterfaces->value(hashcode)){
                                        delete metaTypeInterface;
                                        return iface->typeId;
                                    }
                                    gClassMetaTypeInterfaces->insert(hashcode, metaTypeInterface);
                                    (*gMetaTypeInterfaces)[metaTypeInterface] = std::move(p);
                                }
                                QMetaType metaType(metaTypeInterface);
                                int result = metaType.id();
                                if(!QMetaType::hasRegisteredConverterFunction(metaType, QMetaType::fromType<QString>())){
                                    QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                                        bool ok = false;
                                        if(src){
                                            new (target)QString(reinterpret_cast<const JObjectValueWrapper*>(src)->toString(&ok));
                                        }else{
                                            ok = true;
                                            new (target)QString();
                                        }
                                        return ok;
                                    }, metaType, QMetaType::fromType<QString>());
                                }
                                if(!QMetaType::hasRegisteredConverterFunction(metaType, QMetaType::fromType<JObjectWrapper>())){
                                    QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                                        if(src){
                                            new (target)JObjectWrapper(*reinterpret_cast<const JObjectValueWrapper*>(src));
                                        }else{
                                            new (target)JObjectWrapper();
                                        }
                                        return true;
                                    }, metaType, QMetaType::fromType<JObjectWrapper>());
                                }
                                if(!QMetaType::hasRegisteredConverterFunction(QMetaType::fromType<JObjectWrapper>(), metaType)){
                                    QMetaType::registerConverterFunction([metaType](const void *src, void *target) -> bool {
                                        if(src){
                                            if(JniEnvironment env{200}){
                                                jobject value = reinterpret_cast<const JObjectWrapper*>(src)->object(env);
                                                if(value!=nullptr){
                                                    const JObjectValueWrapperPrivate* methods{nullptr};
                                                    {
                                                        QReadLocker locker(gRegistryLock());
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
                                }
                                registerJavaClassForCustomMetaType(env, metaType, clazz);
                                registerConverterVariant(env, metaType, QLatin1String(metaTypeInterface->name), javaClassName, clazz);
                                return result;
                            }
                            if(!access){
                                if(Java::Runtime::Collection::isAssignableFrom(env, clazz)){
                                    return QMetaType::fromType<JCollectionWrapper>().id();
                                }else if(Java::Runtime::Map::isAssignableFrom(env, clazz)){
                                    return QMetaType::fromType<JMapWrapper>().id();
                                }
                            }
                            metaTypeName = "JObjectWrapper<" + metaTypeName + ">";
                            QMetaType metaType = createMetaType(metaTypeName,
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
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                                                                        QtPrivate::QMetaTypeTypeFlags<JObjectWrapper>::Flags
#else
                                                                        QtPrivate::QMetaTypeForType<JObjectWrapper>::flags()
#endif
                                                                        | QMetaType::IsGadget
                                                                        ),
                                                                nullptr, &findWrappersMetaObject);
                            int result = metaType.id();
                            registerJavaClassForCustomMetaType(env, metaType, clazz, true);
                            if(!QMetaType::hasRegisteredConverterFunction(metaType, QMetaType::fromType<QString>())){
                                QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                                    bool ok = false;
                                    if(src){
                                        new (target)QString(reinterpret_cast<const JObjectWrapper*>(src)->toString(&ok));
                                    }else{
                                        ok = true;
                                        new (target)QString();
                                    }
                                    return ok;
                                }, metaType, QMetaType::fromType<QString>());
                            }
                            registerConverterVariant(env, metaType, QLatin1String(metaTypeName), javaClassName, clazz);
                            return result;
                        }
                    }else{
                        if(!Java::QtCore::AbstractContainer::isAssignableFrom(env, clazz)){
                            if(Java::Runtime::Collection::isAssignableFrom(env, clazz)){
                                return QMetaType::fromType<JCollectionWrapper>().id();
                            }else if(Java::Runtime::Map::isAssignableFrom(env, clazz)){
                                return QMetaType::fromType<JMapWrapper>().id();
                            }
                        }
                        metaTypeName = "JObjectWrapper<" + metaTypeName + ">";
                        QMetaType metaType = createMetaType(metaTypeName,
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
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                                                                            QtPrivate::QMetaTypeTypeFlags<JObjectWrapper>::Flags
#else
                                                                            QtPrivate::QMetaTypeForType<JObjectWrapper>::flags()
#endif
                                                                            | QMetaType::IsGadget
                                                                    ),
                                                            nullptr, &findWrappersMetaObject);
                        int result = metaType.id();
                        if(!QMetaType::hasRegisteredConverterFunction(metaType, QMetaType::fromType<QString>())){
                            QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                                bool ok = false;
                                if(src){
                                    new (target)QString(reinterpret_cast<const JObjectWrapper*>(src)->toString(&ok));
                                }else{
                                    ok = true;
                                    new (target)QString();
                                }
                                return ok;
                            }, metaType, QMetaType::fromType<QString>());
                        }
                        registerJavaClassForCustomMetaType(env, metaType, clazz, true);
                        registerConverterVariant(env, metaType, QLatin1String(metaTypeName), javaClassName, clazz);
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

int registerMetaType(JNIEnv *env, jclass clazz, jboolean isPointer, jboolean isReference, int superId){
    return registerMetaType(env, clazz, isPointer, isReference, nullptr, superId);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
int QmlAPI::registerMetaType(JNIEnv *env, jclass clazz, const QString& javaClassName){
    return registerMetaType(env, clazz, false, false, &javaClassName);
}
#endif
