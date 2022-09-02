#include <qglobal.h>
QT_WARNING_DISABLE_DEPRECATED

#include <QtCore/QByteArray>
#include <QtCore/QThreadStorage>
#include <QtCore/QEvent>
#include <QtCore/QThread>

#include <typeindex>
#include <typeinfo>

#include "qtjambi_core.h"
#include "qtjambi_registry.h"
#include "qtjambi_repository_p.h"
#include "qtjambi_registry_p.h"
#include "qtjambifunctiontable_p.h"
#include "qtjambi_typeinfo_p.h"
#include "qtjambitypemanager_p.h"
#include "qtjambilink_p.h"
#include "qtjambi_interfaces_p.h"
#include "qtjambi_qml.h"
#include "qtjambi_nativeinterface.h"

//#define QTJAMBI_LOG_CLASSNAMES

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
typedef QMap<size_t, NewContainerAccessFunction> ContainerAccessFactoryHash;
Q_GLOBAL_STATIC(ContainerAccessFactoryHash, gContainerAccessFactoryHash)

typedef QHash<int, QHash<int,std::function<AbstractContainerAccess*()>>> ContainerAccessFactoryByMetaTypeHash;
Q_GLOBAL_STATIC(ContainerAccessFactoryByMetaTypeHash, gContainerAccessFactoryByMetaTypeHash)
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
typedef QHash<int, QByteArray> MetaTypeJavaTypeHash;
Q_GLOBAL_STATIC(MetaTypeJavaTypeHash, gMetaTypeJavaTypeHash)
typedef QMultiHash<QByteArray,int> JavaTypeMetaTypesHash;
Q_GLOBAL_STATIC(JavaTypeMetaTypesHash, gJavaTypeMetaTypesHash)

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

Q_GLOBAL_STATIC(JObjectWrapper, gClassLoader)
Q_GLOBAL_STATIC(QString, oldUrlBase)

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
    QByteArray type_name = qtjambi_type_name(typeId);
    if(type_name!=QByteArray(qt_name)){
        registerTypeAlias(typeId, type_name, nullptr);
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void qtjambi_register_native_interface(const char* className, QPair<const char*, int>&& nameAndRevision){
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gNativeInterfaceMap()->insert(className, std::move(nameAndRevision));
}

bool registeredNativeInterface(JNIEnv* env, jclass cls, QPair<const char*, int>& nameAndRevision){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    QByteArray className = qtjambi_class_name(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')).toUtf8();
    if(gNativeInterfaceMap()->contains(className)){
        nameAndRevision = gNativeInterfaceMap()->value(className);
        return true;
    }
    return false;
}
#endif

void registerInterfaceTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name, const char *interface_iid)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::InterfaceTypeInfo);
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gIIDTypeHash->insert(interface_iid, &typeId);
    gTypeIIDHash->insert(java_name, interface_iid);
}

void registerInterfaceID(const std::type_info& typeId, const char *interface_iid)
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

const char * registerInterfaceID(JNIEnv* env, jclass cls)
{
    cls = getGlobalClassRef(env, cls);
    QString iid = qtjambi_class_name(env, cls);
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

void registerInterfaceValueTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name, const char *interface_iid)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::InterfaceValueTypeInfo);
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gIIDTypeHash->insert(interface_iid, &typeId);
    gTypeIIDHash->insert(java_name, interface_iid);
}

void registerFunctionalTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::FunctionalTypeInfo);
}

void registerQObjectTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::QObjectTypeInfo);
}

void registerObjectTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::ObjectTypeInfo);
}

void registerValueTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::ValueTypeInfo);
}

void registerEnumTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::EnumTypeInfo);
}

void registerEnumTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name, const std::type_info& flagsTypeId, const QtJambiTypeInfo& flagsInfo, const char *flags_qt_name, const char *flags_qt_name_alias, const char *flags_java_name)
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

void registerTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, info, qt_name, java_name, EntryTypes::Unspecific);
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

void registerContainerAccessFactory(const std::type_info& typeId, NewContainerAccessFunction factory){
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(!gContainerAccessFactoryHash->contains(unique_id(typeId))){
        gContainerAccessFactoryHash->insert(unique_id(typeId), factory);
    }
    if(AbstractContainerAccess* access = factory()){
        if(AbstractListAccess* ca = dynamic_cast<AbstractListAccess*>(access)){
            int mid = ca->elementMetaType().id();
            if(!gContainerAccessFactoryByMetaTypeHash->contains(mid)){
                (*gContainerAccessFactoryByMetaTypeHash)[mid][int(ContainerType::QList)] = factory;
            }
        }else if(AbstractSetAccess* ca = dynamic_cast<AbstractSetAccess*>(access)){
            int mid = ca->elementMetaType().id();
            if(!gContainerAccessFactoryByMetaTypeHash->contains(mid)){
                (*gContainerAccessFactoryByMetaTypeHash)[mid][int(ContainerType::QSet)] = factory;
            }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        }else if(AbstractLinkedListAccess* ca = dynamic_cast<AbstractLinkedListAccess*>(access)){
            int mid = ca->elementMetaType().id();
            if(!gContainerAccessFactoryByMetaTypeHash->contains(mid)){
                (*gContainerAccessFactoryByMetaTypeHash)[mid][int(ContainerType::QLinkedList)] = factory;
            }
        }else if(AbstractVectorAccess* ca = dynamic_cast<AbstractVectorAccess*>(access)){
            int mid = ca->elementMetaType().id();
            if(!gContainerAccessFactoryByMetaTypeHash->contains(mid)){
                (*gContainerAccessFactoryByMetaTypeHash)[mid][int(ContainerType::QVector)] = factory;
            }
#endif
        }else if(AbstractHashAccess* ca = dynamic_cast<AbstractHashAccess*>(access)){
            int kid = ca->keyMetaType().id();
            int vid = ca->valueMetaType().id();
            if(!gMapAccessFactoryByMetaTypeHash->contains({kid,vid})){
                (*gMapAccessFactoryByMetaTypeHash)[{kid,vid}][int(MapType::QHash)] = factory;
            }
        }else if(AbstractMapAccess* ca = dynamic_cast<AbstractMapAccess*>(access)){
            int kid = ca->keyMetaType().id();
            int vid = ca->valueMetaType().id();
            if(!gMapAccessFactoryByMetaTypeHash->contains({kid,vid})){
                (*gMapAccessFactoryByMetaTypeHash)[{kid,vid}][int(MapType::QMap)] = factory;
            }
        }else if(AbstractMultiHashAccess* ca = dynamic_cast<AbstractMultiHashAccess*>(access)){
            int kid = ca->keyMetaType().id();
            int vid = ca->valueMetaType().id();
            if(!gMapAccessFactoryByMetaTypeHash->contains({kid,vid})){
                (*gMapAccessFactoryByMetaTypeHash)[{kid,vid}][int(MapType::QMultiHash)] = factory;
            }
        }else if(AbstractMultiMapAccess* ca = dynamic_cast<AbstractMultiMapAccess*>(access)){
            int kid = ca->keyMetaType().id();
            int vid = ca->valueMetaType().id();
            if(!gMapAccessFactoryByMetaTypeHash->contains({kid,vid})){
                (*gMapAccessFactoryByMetaTypeHash)[{kid,vid}][int(MapType::QMultiMap)] = factory;
            }
        }
        access->dispose();
    }
}

std::function<AbstractContainerAccess*()> getContainerAccessFactory(ContainerType containerType, const QMetaType& type){
    QReadLocker locker(gLock());
    if(containerType==ContainerType::QQueue){
        containerType = ContainerType::QList;
    }else if(containerType==ContainerType::QStack){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        containerType = ContainerType::QVector;
#else
        containerType = ContainerType::QList;
#endif
    }
    if(gContainerAccessFactoryByMetaTypeHash->contains(type.id())){
        return (*gContainerAccessFactoryByMetaTypeHash)[type.id()][int(containerType)];
    }else{
        return nullptr;
    }
}

std::function<AbstractContainerAccess*()> getContainerAccessFactory(MapType containerType, const QMetaType& keyType, const QMetaType& valueType){
    QReadLocker locker(gLock());
    QPair<int,int> pair{keyType.id(), valueType.id()};
    if(gMapAccessFactoryByMetaTypeHash->contains(pair)){
        return (*gMapAccessFactoryByMetaTypeHash)[pair][int(containerType)];
    }else{
        return nullptr;
    }
}

void registerContainerAccessFactory(ContainerType containerType, const QMetaType& elementType, std::function<AbstractContainerAccess*()>&& factory){
    QWriteLocker locker(gLock());
    if(!(*gContainerAccessFactoryByMetaTypeHash)[elementType.id()].contains(int(containerType))
            || !(*gContainerAccessFactoryByMetaTypeHash)[elementType.id()][int(containerType)])
        (*gContainerAccessFactoryByMetaTypeHash)[elementType.id()][int(containerType)] = std::move(factory);
}

void registerContainerAccessFactory(MapType containerType, const QMetaType& keyType, const QMetaType& valueType, std::function<AbstractContainerAccess*()>&& factory){
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

NewContainerAccessFunction getContainerAccessFactory(const std::type_info& typeId){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gContainerAccessFactoryHash->value(unique_id(typeId));
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
    const SuperTypeInfos& infos = getSuperTypeInfos(env, valueType);
    if(metaTypeId)
        *metaTypeId = QMetaType::UnknownType;
    if (!infos.isEmpty()){
        if(const std::type_info* typeId = getTypeByJavaName(qtjambi_class_name(env, infos.first().javaClass()).replace(QLatin1Char('.'), QLatin1Char('/')))){
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
                    if(JNIEnv* env = qtjambi_current_environment()){
                        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                        void * ptr(nullptr);
                        if(qtjambi_convert_to_native(env, *_nonPointerTypeId, wrapper->object(), &ptr)){
                            if((metaType.flags() & QMetaType::IsPointer)){
                                *reinterpret_cast<void**>(target) = ptr;
                            }else {
                                metaType.destruct(target);
                                metaType.construct(target, ptr);
                            }
                            return true;
                        }
                        return qtjambi_convert_to_native(env, *_nonPointerTypeId, wrapper->object(), target);
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
                }else if(JNIEnv* env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                    jobject output(nullptr);
                    bool result = qtjambi_convert_from_native(env, *_nonPointerTypeId, nullptr, src, true, false, output);
                    new (target)JObjectWrapper(env, output);
                    return result;
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

void registerMetaTypeID(const std::type_info& typeId, int qtMetaType)
{
    registerMetaTypeID(typeId, typeId, qtMetaType);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

void registerJavaClassForCustomMetaType(const QMetaType& metaType, const QByteArray& javaClass);

const char* registerMetaTypeName(const QByteArray& typeName){
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    QSet<QByteArray>::iterator iter = gTypeNames->insert(typeName);
    return iter->data();
}

int registerQmlMetaType(const QByteArray& javaClass,
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
    if(!javaClass.isEmpty())
        registerJavaClassForCustomMetaType(metaType, javaClass);
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

int registerMetaType(const std::type_info& typeId,
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
        registerMetaTypeID(typeId, nonPointerTypeId, id);
        registerAlignmentOfType(nonPointerTypeId, align);
    }else{
        bool needsRegistration;
        {
            QReadLocker locker(gLock());
            Q_UNUSED(locker)
            needsRegistration = !gMetaTypeIDMap->contains(unique_id(typeId));
        }
        if(needsRegistration){
            registerMetaTypeID(typeId, nonPointerTypeId, id);
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
            registerMetaTypeID(typeId, nonPointerTypeId, metaType.id());
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
        registerMetaTypeID(typeId, nonPointerTypeId, metaType.id());
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
            registerMetaTypeID(*typeId, *nonPointerTypeId, id);
            registerAlignmentOfType(*nonPointerTypeId, align);
        }
    }else if(typeId && nonPointerTypeId){
        bool needsRegistration;
        {
            QReadLocker locker(gLock());
            Q_UNUSED(locker)
            needsRegistration = !gMetaTypeIDMap->contains(unique_id(*typeId));
        }
        if(needsRegistration){
            registerMetaTypeID(*typeId, *nonPointerTypeId, id);
            registerAlignmentOfType(*nonPointerTypeId, align);
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
            registerMetaTypeID(*typeId, *nonPointerTypeId, metaType.id());
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
            registerMetaTypeID(*typeId, *nonPointerTypeId, metaType.id());
    }
    return metaType.id();
#endif
}

int registerMetaType(const std::type_info& typeId,
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

int registerMetaType(const QByteArray& typeName,
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

const void* qtjambi_get_default_value(const std::type_info& type_info, DefaultValueCreator creator)
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

void registerFunctionInfos(const std::type_info& typeId, std::initializer_list<FunctionInfo> virtualFunctions)
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

void registerConstructorInfos(const std::type_info& typeId, Destructor destructor, std::initializer_list<ConstructorInfo> constructors){
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

void registerDeleter(const std::type_info& typeId, PtrDeleterFunction deleter)
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

void registerOwnerFunction(const std::type_info& typeId, PtrOwnerFunction ownerFunction)
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

void registerPolymorphyHandler(const std::type_info& polymorphicBaseTypeId, const std::type_info& targetTypeId, PolymorphyHandler handler)
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

void registerSizeOfType(const std::type_info& typeId, size_t size)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(gValueSizeHash->contains(unique_id(typeId)))
        return;
    gValueSizeHash->insert(unique_id(typeId), size);
}

void registerAlignmentOfType(const std::type_info& typeId, size_t alignment)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(gValueAlignmentHash->contains(unique_id(typeId)))
        return;
    gValueAlignmentHash->insert(unique_id(typeId), alignment);
}

void registerSizeOfShell(const std::type_info& typeId, size_t size)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(gShellSizeHash->contains(unique_id(typeId)))
        return;
    gShellSizeHash->insert(unique_id(typeId), size);
}

bool qtjambi_is_pointer_type(const QMetaType& metaType);

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
            }else if(qtjambi_is_pointer_type(metaType))
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

void registerHashFunction(const std::type_info& typeId, QHashFunctionPtr hashFunction){
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(gHashFunctionHash->contains(unique_id(typeId)))
        return;
//    fprintf(stderr, "registerHashFunction for %s\n", typeId.name());
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

void registerInterfaceOffset(const std::type_info& qt_base, const std::type_info& qt_interface, int offset)
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
            if(!Java::QtJambi::QtJambiInternal::isGeneratedClass(env, cls)){
                const SuperTypeInfos& infos = getSuperTypeInfos(env, clazz);
                for(const SuperTypeInfo& info : infos){
                    if(info.offset()>0){
                        result.offsets[unique_id(info.typeId())] = uint(info.offset());
                    }
                }
                cls = resolveClosestQtSuperclass(env, cls);
            }

            while(cls && Java::QtJambi::QtJambiInternal::isGeneratedClass(env, cls)){
                QString className = qtjambi_class_name(env, cls).replace(".", "/");
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

hash_type qHash(const QMetaObject* mo){
    hash_type hashCode = qHash(qintptr(mo));
    if(mo){
        hashCode = hashCode * 31 + qHash(mo->className());
    }
    return hashCode;
}

void registerCustomMetaObject(const std::type_info& typeId, const QMetaObject& superTypeMetaObject)
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

void registerMetaObject(const std::type_info& typeId, const QMetaObject& originalMetaObject, bool isValueOwner,
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

void registerMetaObject(const std::type_info& typeId, const QMetaObject& metaObject, bool isValueOwner,
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

const QHash<int,const char*>* renamedMethods(const QMetaObject* metaObject){
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

void registerFunctionalResolver(const std::type_info& typeId, FunctionalResolver resolver)
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

static QmlReportDestruction qmlReportDestructionFunction = nullptr;

void qtjambi_report_qmlobject_destruction(QObject * obj){
    if(qmlReportDestructionFunction)
        qmlReportDestructionFunction(obj);
}

void setQmlReportDestruction(QmlReportDestruction fct){
    if(!qmlReportDestructionFunction)
        qmlReportDestructionFunction = fct;
}

void registerMediaControlInfo(const std::type_info& typeId, const char *media_control_iid){
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gMediaControlIIDHash->insert(unique_id(typeId), media_control_iid);
    gMediaControlIIDClassHash->insert(media_control_iid, &typeId);
}

void registerJavaClassForCustomMetaType(int metaType, const QByteArray& javaClass){
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gMetaTypeJavaTypeHash->insert(metaType, javaClass);
    gJavaTypeMetaTypesHash->insert(javaClass, metaType);
}

void registerJavaClassForCustomMetaType(const QMetaType& metaType, const QByteArray& javaClass){
    registerJavaClassForCustomMetaType(metaType.id(), javaClass);
}

QList<int> registeredCustomMetaTypesForJavaClass(const QByteArray& javaClass){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gJavaTypeMetaTypesHash->values(javaClass);
}

QByteArray registeredJavaClassForCustomMetaType(int metaType){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gMetaTypeJavaTypeHash->value(metaType);
}

QByteArray registeredJavaClassForCustomMetaType(const QMetaType& metaType){
    return registeredJavaClassForCustomMetaType(metaType.id());
}

QList<const char*> getInterfaceIIDs(JNIEnv *env, jclass javaType){
    QList<const char*> iids;
    QString className;
    if(javaType){
        jobject interfaceList = Java::QtJambi::QtJambiInternal::getAllImplementedInterfaces(env, javaType);
        if(interfaceList){
            int count = qtjambi_collection_size(env, interfaceList);
            if(count>0){
                jobject iterator = qtjambi_collection_iterator(env, interfaceList);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jclass interfaceClass = jclass(qtjambi_iterator_next(env, iterator));
                    className = qtjambi_class_name(env, interfaceClass).replace('.', '/');
                    QReadLocker locker(gLock());
                    Q_UNUSED(locker)
                    if(const char* iid = gTypeIIDHash->value(className.toLatin1(), nullptr)){
                        iids << iid;
                    }
                }
            }
        }
        while(javaType){
            className = qtjambi_class_name(env, javaType).replace('.', '/');
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

const char* getInterfaceIID(JNIEnv *env, jclass javaType){
    QList<const char*> iids = getInterfaceIIDs(env, javaType);
    if(!iids.isEmpty())
        return iids.first();
    return nullptr;
}

jclass getInterfaceByIID(JNIEnv *env, const char* iid){
    if(env && iid){
        const std::type_info* typeId(nullptr);
        {
            QReadLocker locker(gLock());
            Q_UNUSED(locker)
            typeId = gIIDTypeHash->value(iid, nullptr);
        }
        if(typeId){
            if(const char* javaName = getJavaName(*typeId)){
                return resolveClass(env, javaName);
            }
        }else{
            const char* javaName(nullptr);
            {
                QReadLocker locker(gLock());
                Q_UNUSED(locker)
                javaName = (*gIIDByteArrayHash)[iid];
            }
            if(javaName){
                return resolveClass(env, javaName);
            }
        }
    }
    return nullptr;
}

const char* mediaControlIID(JNIEnv *env, jclass javaType){
    if(javaType){
        QString className = qtjambi_class_name(env, javaType).replace(".", "/");
        const std::type_info* typeId = getTypeByJavaName(className);
        if(typeId){
            QReadLocker locker(gLock());
            Q_UNUSED(locker)
            return gMediaControlIIDHash->value(unique_id(*typeId), nullptr);
        }
    }
    return nullptr;
}

jclass classByMediaControlIID(JNIEnv *env, const char* iid){
    if(env && iid){
        const std::type_info* typeId(nullptr);
        {
            QReadLocker locker(gLock());
            Q_UNUSED(locker)
            typeId = gMediaControlIIDClassHash->value(iid, nullptr);
        }
        if(typeId){
            if(const char* javaName = getJavaName(*typeId)){
                return resolveClass(env, javaName);
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
        qtjambi_throw_java_exception(env);
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
        jstring className = qtjambi_from_qstring(env, qtClassName);
        Q_ASSERT(className);
        if(classLoader){
            // Look up the class in our custom class loader
            returned = Java::Runtime::ClassLoader::tryLoadClass(env, classLoader, className);
            if (env->ExceptionCheck() || returned == nullptr) {
                env->ExceptionClear();
                classLoader = nullptr;
            }
        }

        if(!classLoader){
            // Check internal property which is set in Eclipse integration
            QString pathSeparator = ";";
            QString qtClassPath;
            {
                if (jstring classPath = Java::Runtime::System::getProperty(env, env->NewStringUTF("io.qt.qtjambi.internal.urlbase")))
                    qtjambi_to_qstring(qtClassPath, env, classPath);
            }
            if (!qtClassPath.isEmpty()) {
                QString oldBase;
                {
                    QReadLocker locker(gLock());
                    classLoader = gClassLoader->object();
                    oldBase = *oldUrlBase();
                }

                if (classLoader == nullptr) {
                    // No class loader instantiated?
                    // We have to make a URL loader, because the default is created with
                    // the wrong class path in Eclipse.

                    QWriteLocker locker(gLock());
                    classLoader = gClassLoader->object();
                    if (classLoader == nullptr) {
                        locker.unlock();
                        jobject currentThread = Java::Runtime::Thread::currentThread(env);
                        Q_ASSERT(currentThread);

                        jobject contextClassLoader = Java::Runtime::Thread::getContextClassLoader(env, currentThread);
                        Q_ASSERT(currentThread);

                        QStringList urlList = qtClassPath.split(pathSeparator);
                        jobjectArray urls = Java::Runtime::URL::newArray(env, jsize(urlList.size()));
                        Q_ASSERT(urls);

                        for (int i=0; i<urlList.size(); ++i) {
                            jobject url = Java::Runtime::URL::newInstance(env, qtjambi_from_qstring(env, urlList.at(i)));
                            Q_ASSERT(url);

                            env->SetObjectArrayElement(urls, i, url);
                        }

                        classLoader = Java::Runtime::URLClassLoader::newInstance(env, urls, contextClassLoader);
                        Q_ASSERT(classLoader);

                        locker.relock();
                        gClassLoader->operator=(JObjectWrapper(env, classLoader));
                        *oldUrlBase() = qtClassPath;
                        oldBase = qtClassPath;
                    }
                }

                if (classLoader && oldBase != qtClassPath) {
                    // If the class path has changed after the class loader
                    // was created, we need to update the class loader.
                    // This is needed to load custom widgets in the current project.

                    QWriteLocker locker(gLock());
                    QStringList oldUrlList = oldUrlBase->split(pathSeparator);
                    QStringList urlList = qtClassPath.split(pathSeparator);


                    locker.unlock();
                    for (int i=0; i<urlList.size(); ++i) {
                        if (!oldUrlList.contains(urlList.at(i))) {
                            jobject url = Java::Runtime::URL::newInstance(env, qtjambi_from_qstring(env, urlList.at(i)));
                            Q_ASSERT(url);
                            Java::Runtime::URLClassLoader::addURL(env, classLoader, url);
                        }
                    }
                    locker.relock();
                    *oldUrlBase() = qtClassPath;
                }
                // Look up the class in our custom class loader
                if(classLoader){
                    returned = Java::Runtime::ClassLoader::tryLoadClass(env, classLoader, className);
                    if (env->ExceptionCheck()) {
                        returned = nullptr;
                        if(!exception)
                            exception = env->ExceptionOccurred();
                        env->ExceptionClear();
                    }
                }
                if (returned == nullptr && exception) {
#ifdef QTJAMBI_STACKTRACE
                    if(qtClassName=="io.qt.internal.QtJambiInternal")
                        JavaException(env, exception).raise();
                    else
                        JavaException(env, exception).raise( QTJAMBI_STACKTRACEINFO_ENV(env) );
#else
                        JavaException(env, exception).raise();
#endif
                }
            } else {
#ifdef Q_OS_ANDROID
                if(!qtClassName.endsWith("android/QtNative")){
                    jclass cls = Java::Android::QtNative::getClass(env);
                    classLoader = Java::Runtime::Class::getClassLoader(env, cls);
                    returned = Java::Runtime::ClassLoader::tryLoadClass(env, classLoader, className);
                    if (env->ExceptionCheck()) {
                        returned = nullptr;
                        if(!exception)
                            exception = env->ExceptionOccurred();
                        env->ExceptionClear();
                    }
                }
                if (returned == nullptr && exception)
#endif
#ifdef QTJAMBI_STACKTRACE
                if(qtClassName=="io.qt.internal.QtJambiInternal")
                    JavaException(env, exception).raise();
                else
                    JavaException(env, exception).raise( QTJAMBI_STACKTRACEINFO_ENV(env) );
#else
                    JavaException(env, exception).raise();
#endif
            }
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
                classNameStrg = qtjambi_class_name(env, cls).replace(".", "/");
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
        className = "[L"+qtjambi_class_name(env, cls).replace(".", "/")+";";
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

jclass resolveClass(JNIEnv *env, const char *className, jobject classLoader)
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
            qtjambi_throw_java_exception(env);
            returned = getGlobalClassRef(env, returned, className);
#ifndef QTJAMBI_NOCACHE
        }
#endif // QTJAMBI_NOCACHE
    }

    return returned;
}

JNIEnv *qtjambi_current_environment(bool initializeJavaThread);

/*******************************************************************************
 * Field Cache
 */

jfieldID resolveField(JNIEnv *env, const char *fieldName, const char *signature,
                      const char *className, bool isStatic)
{
    if(jclass clazz = resolveClass(env, className)){
        return resolveField(env, fieldName, signature, clazz, isStatic);
    }
    else return nullptr;
}

jfieldID resolveField(JNIEnv *env, const char *fieldName, const char *signature,
                      jclass clazz, bool isStatic)
{
    if(!clazz)
        return nullptr;
#ifndef QTJAMBI_NOCACHE
    hash_type classHash = hash_type(Java::Runtime::Object::hashCode(env,clazz));
    hash_type key = hashSum({qHash(fieldName), classHash, hash_type(isStatic ? 1 : 0)});

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

jmethodID resolveMethod(JNIEnv *env, const char *methodName, const char *signature, jclass clazz, bool isStatic)
{
    jmethodID returned = nullptr;
    if(clazz){
#ifndef QTJAMBI_NOCACHE
        hash_type classHash = hash_type(Java::Runtime::Object::hashCode(env,clazz));
        hash_type key = hashSum({qHash(methodName),
                            qHash(signature),
                            classHash,
                            hash_type(isStatic ? 1 : 0)});

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
        qtjambi_throw_java_exception(env);
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
    uint key = uint(Java::Runtime::Object::hashCode(env,clazz));
    jmethodID constructor;
    {
        QReadLocker locker(gConstructorLock());
        Q_UNUSED(locker)
        constructor = gConstructorHash->value(key, nullptr);
    }
    if(!constructor){
        QWriteLocker locker(gConstructorLock());
        Q_UNUSED(locker)
        constructor = env->GetMethodID(clazz, "<init>", "(Lio/qt/QtObject$QPrivateConstructor;)V");
        qtjambi_throw_java_exception(env);
        gConstructorHash->insert(key, constructor);
    }
    return constructor;
}

jclass resolveLambdaInterface(JNIEnv* env, jclass lambdaClass){
    jobjectArray interfaces = Java::Runtime::Class::getInterfaces(env, lambdaClass);
    Q_ASSERT(interfaces && env->GetArrayLength(interfaces)>0);
    return jclass( env->GetObjectArrayElement(interfaces, 0) );
}

jclass resolveClosestQtSuperclass(JNIEnv *env, jclass clazz, jobject classLoader)
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

            if (Java::QtJambi::QtJambiInternal::isGeneratedClass(env,clazz))
                returned = clazz;

            // If not, try the superclass recursively
            if (!returned && clazz) {
                jclass superKey = env->GetSuperclass(clazz);
                if (superKey) {
                    returned = resolveClosestQtSuperclass(env, superKey, classLoader);
                }
            }

            if (returned) {
                returned = getGlobalClassRef(env, returned);
                QWriteLocker locker(gQtSuperclassLock());
                Q_UNUSED(locker)
                if (!gQtSuperclassHash->contains(key)) {
#ifdef QTJAMBI_COUNTCACHEMISSES
                    cacheMisses(qPrintable(qtjambi_class_name(env, returned).replace('.', '/')));
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
                    cacheMisses(qPrintable(qtjambi_class_name(env, returned).replace('.', '/')));
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

static CreateQmlErrorDummyObject createQmlErrorDummyObjectFunction = nullptr;

void registerCreateQmlErrorDummyObjectFunction(CreateQmlErrorDummyObject createQmlErrorDummyObject)
{
    createQmlErrorDummyObjectFunction = createQmlErrorDummyObject;
}

QObject* createQmlErrorDummyObject(const QMetaObject* metaObject, void* placement, int vsCast, int viCast)
{
    if(createQmlErrorDummyObjectFunction){
        return createQmlErrorDummyObjectFunction(metaObject, placement, vsCast, viCast);
    }else{
        return nullptr;
    }
}

jclass qtjambi_find_generated_superclass(JNIEnv * env, jclass clazz){
    return Java::QtJambi::QtJambiInternal::findGeneratedSuperclass(env, clazz);
}

jobject qtjambi_find_QmlAttachedProperties(JNIEnv * env, jclass clazz){
    return Java::QtJambi::QtJambiInternal::findQmlAttachedProperties(env, clazz);
}

void clear_registry_at_shutdown(JNIEnv * env){
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
    static int result = registerMetaType<JObjectWrapper>("JObjectWrapper");
    return result;
}

void JObjectWrapperConverter::registerConverter(const std::type_info& _typeId, int _to){
    if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperID(), _to) && strcmp("QRemoteObjectPendingCall", QMetaType::typeName(_to))!=0)
        qtjambi_register_converter(new JObjectWrapperConverter(_typeId, _to), jobjectWrapperID(), _to);
}

bool qtjambi_register_converter(const QtPrivate::AbstractConverterFunction *f, int from, int to);
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
        if(JNIEnv *env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            void * ptr(nullptr);
            if(qtjambi_convert_to_native(env, converter->typeId, wrapper->object(), &ptr)){
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
