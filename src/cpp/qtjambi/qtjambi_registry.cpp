#include <QtCore/QByteArray>
#include <QtCore/QThreadStorage>
#include <QtCore/QEvent>
#include <QtCore/QThread>

#include <typeindex>
#include <typeinfo>

#include "qtjambi_core.h"
#include "qtjambi_registry.h"
#include "qtjambi_functionpointer.h"
#include "qtjambi_repository_p.h"
#include "qtjambi_registry_p.h"
#include "qtjambifunctiontable_p.h"
#include "qtjambi_typeinfo_p.h"
#include "qtjambitypemanager_p.h"
#include "qtjambilink_p.h"
#include "qtjambi_interfaces_p.h"
#include "qtjambi_exceptions.h"
#include "qtjambi_qml.h"

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
typedef QHash<QByteArray, QByteArray> IIDByteArrayHash;
Q_GLOBAL_STATIC(IIDByteArrayHash, gIIDByteArrayHash)
typedef QMap<size_t, const std::type_info*> ID2IDHash;
Q_GLOBAL_STATIC(ID2IDHash, gFlagEnumIDHash)
Q_GLOBAL_STATIC(ID2IDHash, gEnumFlagIDHash)
Q_GLOBAL_STATIC(TypeStringHash, gTypeJavaInterfaceHash)
typedef QMap<size_t, NewContainerAccessFunction> ContainerAccessFactoryHash;
Q_GLOBAL_STATIC(ContainerAccessFactoryHash, gContainerAccessFactoryHash)
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
typedef QMap<size_t, const std::type_info*> PolymorphicBasesHash;
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
#ifdef QT_QTJAMBI_PORT
typedef QHash<const QMetaObject*, SignalConnector> SignalConnectorHash;
Q_GLOBAL_STATIC(SignalConnectorHash, gSignalConnectors)
#endif
typedef QHash<const QMetaObject*, QVector<SignalMetaInfo> > SignalMetaInfoHash;
Q_GLOBAL_STATIC(SignalMetaInfoHash, gSignalMetaInfos)
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

Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gEmitMethodLock, (QReadWriteLock::Recursive))
Q_GLOBAL_STATIC(MethodIdHash, gEmitMethodHash)

typedef QThreadStorage<JavaException> JavaExceptionStorage;
Q_GLOBAL_STATIC(JavaExceptionStorage, gJavaExceptionStorage)

typedef QThreadStorage<bool> BoolStorage;
Q_GLOBAL_STATIC(BoolStorage, gBlockExceptionStorage)

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

hash_type qHash(const std::type_index& idx, hash_type seed) Q_DECL_NOEXCEPT
{
    return qHash(idx.hash_code(), seed);
}

hash_type qHash(const std::type_index& idx) Q_DECL_NOEXCEPT
{
    return qHash(idx.hash_code());
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
        if(!gTypeQtNameHash->contains(typeId.hash_code())){
            gTypeQtNameHash->insert(typeId.hash_code(), qt_name);
        }
        if(!gQTypeInfoHash->contains(typeId.hash_code())){
            gQTypeInfoHash->insert(typeId.hash_code(), QSharedPointer<QtJambiTypeInfo>(new QtJambiTypeInfo(info)));
        }
        gQtNameTypeHash->insert(qt_name, &typeId);

        if(!gTypeJavaNameHash->contains(typeId.hash_code())){
            gTypeJavaNameHash->insert(typeId.hash_code(), java_name);
        }
        (*gJavaNameTypeHash)[java_name].append(&typeId);
        if(entryTypes!=EntryTypes::Unspecific)
            gEntryTypesHash->insert(typeId.hash_code(), entryTypes);
        if(entryTypes==EntryTypes::FunctionalTypeInfo)
            gQtFunctionalJavaNameHash->insert(qt_name, java_name);
    }
    QByteArray type_name = qtjambi_type_name(typeId);
    if(type_name!=QByteArray(qt_name)){
        registerTypeAlias(typeId, type_name, nullptr);
    }
}

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
    gTypeIIDHash->insert(gTypeJavaNameHash->value(typeId.hash_code(), nullptr), interface_iid);
}

const char * registeredInterfaceID(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gTypeIIDHash->value(gTypeJavaNameHash->value(typeId.hash_code(), nullptr));
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
    gFlagEnumIDHash->insert(flagsTypeId.hash_code(), &typeId);
    gEnumFlagIDHash->insert(typeId.hash_code(), &flagsTypeId);
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
    if(!gTypeJavaInterfaceHash->contains(typeId.hash_code())){
        gTypeJavaInterfaceHash->insert(typeId.hash_code(), java_interface);
    }
}

void registerContainerAccessFactory(const std::type_info& typeId, NewContainerAccessFunction factory){
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(!gContainerAccessFactoryHash->contains(typeId.hash_code())){
        gContainerAccessFactoryHash->insert(typeId.hash_code(), factory);
    }
}

NewContainerAccessFunction getContainerAccessFactory(const std::type_info& typeId){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gContainerAccessFactoryHash->value(typeId.hash_code());
}


EntryTypes getEntryType(const std::type_info& typeId){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gEntryTypesHash->value(typeId.hash_code(), EntryTypes::Unspecific);
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
    return gEntryTypesHash->value(typeId.hash_code(), EntryTypes::Unspecific) == EntryTypes::InterfaceTypeInfo;
}

bool isFunctional(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gEntryTypesHash->value(typeId.hash_code(), EntryTypes::Unspecific) == EntryTypes::FunctionalTypeInfo;
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
            EntryTypes entryType = gEntryTypesHash->value(typeId->hash_code(), EntryTypes::Unspecific);
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
    return gQTypeInfoHash->value(typeId.hash_code(), QSharedPointer<QtJambiTypeInfo>(nullptr)).data();
}

const char * getQtName(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gTypeQtNameHash->value(typeId.hash_code(), nullptr);
}

const char * getJavaName(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gTypeJavaNameHash->value(typeId.hash_code(), nullptr);
}

const char * getJavaInterfaceName(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    if(gTypeJavaInterfaceHash->contains(typeId.hash_code()))
        return gTypeJavaInterfaceHash->value(typeId.hash_code(), nullptr);
    return gTypeJavaNameHash->value(typeId.hash_code(), nullptr);
}

void registerMetaTypeID(const std::type_info& typeId, const std::type_info& nonPointerTypeId, int qtMetaType)
{
    const char * registeredQtName;
    {
        QWriteLocker locker(gLock());
        Q_UNUSED(locker)
        if(!gMetaTypeIDMap->contains(typeId.hash_code()))
            gMetaTypeIDMap->insert(typeId.hash_code(), qtMetaType);
        if(!gMetaTypeIDHash->contains(qtMetaType))
            gMetaTypeIDHash->insert(qtMetaType, &typeId);
        registeredQtName = gTypeQtNameHash->value(typeId.hash_code(), nullptr);
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
            && nonPointerTypeId!=typeid(nullptr_t)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        JObjectWrapperConverter::registerConverter(nonPointerTypeId, qtMetaType);
#else
        const std::type_info* _nonPointerTypeId = &nonPointerTypeId;
        QMetaType metaType(qtMetaType);
        QMetaType jObjectWrapperMetaType = QMetaType::fromType<JObjectWrapper>();
        if(!QMetaType::hasRegisteredConverterFunction(jObjectWrapperMetaType, metaType)){
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                     ushort align,
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
        afterRegistrationFunction(id);
        registerMetaTypeID(typeId, nonPointerTypeId, id);
    }else{
        bool needsRegistration;
        {
            QReadLocker locker(gLock());
            Q_UNUSED(locker)
            needsRegistration = !gMetaTypeIDMap->contains(typeId.hash_code());
        }
        if(needsRegistration)
            registerMetaTypeID(typeId, nonPointerTypeId, id);
    }
    return id;
#else
    QMetaType metaType = QMetaType::fromName(typeName);
    if(metaType.isValid()){
        bool needsRegistration;
        {
            QReadLocker locker(gLock());
            Q_UNUSED(locker)
            needsRegistration = !gMetaTypeIDMap->contains(typeId.hash_code());
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
        afterRegistrationFunction(metaType.id());
        registerMetaTypeID(typeId, nonPointerTypeId, metaType.id());
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                     ushort align,
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
        afterRegistrationFunction(id);
        registerMetaTypeID(typeId, nonPointerTypeId, id);
    }else{
        bool needsRegistration;
        {
            QReadLocker locker(gLock());
            Q_UNUSED(locker)
            needsRegistration = !gMetaTypeIDMap->contains(typeId.hash_code());
        }
        if(needsRegistration)
            registerMetaTypeID(typeId, nonPointerTypeId, id);
    }
    return id;
#else
    QMetaType metaType = QMetaType::fromName(typeName);
    if(metaType.isValid()){
        bool needsRegistration;
        {
            QReadLocker locker(gLock());
            Q_UNUSED(locker)
            needsRegistration = !gMetaTypeIDMap->contains(typeId.hash_code());
        }
        if(needsRegistration)
            registerMetaTypeID(typeId, nonPointerTypeId, metaType.id());
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
        afterRegistrationFunction(metaType.id());
        registerMetaTypeID(typeId, nonPointerTypeId, metaType.id());
    }
    return metaType.id();
#endif
}

int registeredMetaTypeID(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gMetaTypeIDMap->value(typeId.hash_code(), QMetaType::UnknownType);
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
    return gFlagEnumIDHash->value(flag.hash_code(), nullptr);
}

const std::type_info* getFlagForEnum(const std::type_info& enumerator)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gEnumFlagIDHash->value(enumerator.hash_code(), nullptr);
}

const void* qtjambi_get_default_value(const std::type_info& type_info, DefaultValueCreator creator)
{
    const void* result;
    {
        QReadLocker locker(gLock());
        Q_UNUSED(locker)
        result = gDefaultValueHash->value(type_info.hash_code(), nullptr);
    }
    if(!result){
        result = creator();
        QWriteLocker wlocker(gLock());
        Q_UNUSED(wlocker)
        gDefaultValueHash->insert(type_info.hash_code(), result);
    }
    return result;
}

void registerFunctionInfos(const std::type_info& typeId, std::initializer_list<FunctionInfo> virtualFunctions)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(!gVirtualFunctionInfos->contains(typeId.hash_code()))
        gVirtualFunctionInfos->insert(typeId.hash_code(), virtualFunctions);
}

const QVector<const FunctionInfo>* virtualFunctions(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    if(gVirtualFunctionInfos->contains(typeId.hash_code())){
        return reinterpret_cast<const QVector<const FunctionInfo>*>(&(*gVirtualFunctionInfos())[typeId.hash_code()]);
    }else{
        return nullptr;
    }
}

void registerConstructorInfos(const std::type_info& typeId, Destructor destructor, std::initializer_list<ConstructorInfo> constructors){
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(!gConstructorInfos->contains(typeId.hash_code()))
        gConstructorInfos->insert(typeId.hash_code(), constructors);
    if(!gDestructorHash->contains(typeId.hash_code()))
        gDestructorHash->insert(typeId.hash_code(), destructor);
}

const QVector<const ConstructorInfo>* registeredConstructorInfos(const std::type_info& typeId){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    if(gConstructorInfos->contains(typeId.hash_code())){
        return reinterpret_cast<const QVector<const ConstructorInfo>*>(&(*gConstructorInfos())[typeId.hash_code()]);
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
    gDeleterHash->insert(typeId.hash_code(), deleter);
}

PtrDeleterFunction deleter(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gDeleterHash->value(typeId.hash_code(), nullptr);
}

void registerOwnerFunction(const std::type_info& typeId, PtrOwnerFunction ownerFunction)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gOwnerFunctionHash->insert(typeId.hash_code(), ownerFunction);
}

PtrOwnerFunction registeredOwnerFunction(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gOwnerFunctionHash->value(typeId.hash_code(), nullptr);
}

void registerPolymorphyHandler(const std::type_info& polymorphicBaseTypeId, PolymorphyHandler handler, const char *class_name, const std::type_info& targetTypeId, bool isQObject)
{
    if(handler){
        QWriteLocker locker(gLock());
        Q_UNUSED(locker)
        g_polymorphic_ids->insert(polymorphicBaseTypeId.hash_code(), new PolymorphicIdHandler(handler, class_name, targetTypeId, isQObject));
        gPolymorphicBasesHash->insert(targetTypeId.hash_code(), &polymorphicBaseTypeId);
    }
}

bool isPolymorphicBase(const std::type_info& typeId){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return g_polymorphic_ids->contains(typeId.hash_code());
}

const std::type_info* getPolymorphicBase(const std::type_info& typeId){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gPolymorphicBasesHash->value(typeId.hash_code(), nullptr);
}

QList<const PolymorphicIdHandler*> getPolymorphicIdHandlers(const std::type_info& polymorphicBaseTypeId){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return g_polymorphic_ids->values(polymorphicBaseTypeId.hash_code());
}

void qtjambi_resolve_polymorphic_id(const std::type_info& polymorphicBaseTypeId, const void *object,
                                    char const* &class_name, const std::type_info* &targetTypeId, bool& isQObject)
{
    if(object){
        void *_object = const_cast<void *>(object);
        QReadLocker locker(gLock());
        Q_UNUSED(locker)
        for(const PolymorphicIdHandler* handler : g_polymorphic_ids->values(polymorphicBaseTypeId.hash_code())){
            Q_ASSERT(handler->m_polymorphyHandler);
            if(handler->m_polymorphyHandler(_object)) {
                class_name = handler->m_class_name;
                isQObject = handler->m_isQObject;
                targetTypeId = &handler->m_targetTypeId;
                return;
            }
        }
    }
}

void registerSizeOfType(const std::type_info& typeId, size_t size)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gValueSizeHash->insert(typeId.hash_code(), size);
}

void registerAlignmentOfType(const std::type_info& typeId, size_t alignment)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gValueAlignmentHash->insert(typeId.hash_code(), alignment);
}

void registerSizeOfShell(const std::type_info& typeId, size_t size)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gShellSizeHash->insert(typeId.hash_code(), size);
}

void registerHashFunction(const std::type_info& typeId, QHashFunctionPtr hashFunction){
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(gHashFunctionHash->contains(typeId.hash_code()))
        return;
//    fprintf(stderr, "registerHashFunction for %s\n", typeId.name());
    gHashFunctionHash->insert(typeId.hash_code(), hashFunction);
}

QHashFunctionPtr registeredHashFunction(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gHashFunctionHash->value(typeId.hash_code(), nullptr);
}

size_t getValueSize(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gValueSizeHash->value(typeId.hash_code(), 0);
}

size_t getValueAlignment(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gValueAlignmentHash->value(typeId.hash_code(), 0);
}

size_t getShellSize(const std::type_info& typeId)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gShellSizeHash->value(typeId.hash_code(), 0);
}

void registerInterfaceOffset(const std::type_info& qt_base, const std::type_info& qt_interface, int offset)
{
    if(offset!=0){
        QWriteLocker locker(gLock());
        Q_UNUSED(locker)
        ( *gOffsetHash )[qt_base.hash_code()].insert(qt_interface.hash_code(), offset);
        ( *gOffsetHash )[qt_interface.hash_code()].insert(qt_base.hash_code(), -offset);
        ( *gInterfacesHash )[qt_base.hash_code()] << &qt_interface;
    }
}

int registeredInterfaceOffset(const std::type_info& qt_base, const std::type_info& qt_interface){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    if(gOffsetHash->contains(qt_base.hash_code())){
        return ( *gOffsetHash )[qt_base.hash_code()].value(qt_interface.hash_code(), -1);
    }
    return -1;
}

void registeredInterfaceOffsets(const std::type_info& qt_type, InterfaceOffsetInfo* info){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    if(gOffsetHash->contains(qt_type.hash_code())){
        const QMap<size_t, int>& values = ( *gOffsetHash )[qt_type.hash_code()];
        for(QMap<size_t, int>::const_key_value_iterator i = values.keyValueBegin(); i!=values.keyValueEnd(); ++i){
            info->interfaces.insert(i.base().key());
            if(i.base().value()>0)
                info->offsets.insert(i.base().key(), uint(i.base().value()));
        }
    }
    if(gInterfacesHash->contains(qt_type.hash_code())){
        info->inheritedInterfaces[qt_type.hash_code()] = ( *gInterfacesHash )[qt_type.hash_code()];
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
                        result.offsets[info.typeId().hash_code()] = uint(info.offset());
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
                        result.offsets[info.typeId().hash_code()] = uint(info.offset());
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
    gDestructorHash->insert(typeId.hash_code(), destructor);
}

Destructor registeredDestructor(const std::type_info& typeId){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gDestructorHash->value(typeId.hash_code(), nullptr);
}

hash_type qHash(const QMetaObject* mo){
    hash_type hashCode = qHash(qintptr(mo));
    if(mo){
        hashCode = hashCode * 31 + qHash(mo->className());
    }
    return hashCode;
}

void registerMetaObject(const std::type_info& typeId, const QMetaObject& originalMetaObject, bool isValueOwner,
                        QMetaMethodRenamer methodRenamer)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gOriginalMetaObjectHash->insert(typeId.hash_code(), &originalMetaObject);
    gTypeIdsByMetaObject->insert(&originalMetaObject, &typeId);
    if(methodRenamer)
        gQMetaMethodRenamerByMetaObject->insert(&originalMetaObject, methodRenamer);
    if(isValueOwner)
        gValueOwners->insert(&originalMetaObject);
}

void registerMetaObject(const std::type_info& typeId, const QMetaObject& metaObject, bool isValueOwner,
#ifdef QT_QTJAMBI_PORT
                        SignalConnector signalConnector,
#endif
                        std::initializer_list<SignalMetaInfo> signalMetaInfos,
                        ParameterInfoProvider parameterTypeInfoProvider,
                        QMetaMethodRenamer methodRenamer)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gOriginalMetaObjectHash->insert(typeId.hash_code(), &metaObject);
    gTypeIdsByMetaObject->insert(&metaObject, &typeId);
#ifdef QT_QTJAMBI_PORT
    if(!gSignalConnectors->contains(&metaObject))
        gSignalConnectors->insert(&metaObject, signalConnector);
#endif
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
    return gOriginalMetaObjectHash->value(typeId.hash_code(), nullptr);
}

#ifdef QT_QTJAMBI_PORT
SignalConnector signalConnector(const QMetaObject* metaObject)
{
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gSignalConnectors->value(metaObject, nullptr);
}
#endif

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

#ifdef QT_QTJAMBI_PORT
static QSignalMapperConnector signalMapperConnector = nullptr;

void registerSignalMapperConnector(QSignalMapperConnector _signalMapperConnector)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(signalMapperConnector==nullptr)
        signalMapperConnector = _signalMapperConnector;
}

QMetaObject::Connection connectSignalMapper(QSignalMapper* object, const WeakNativeID& link, QSignalMapperMapped signalMapperMapped){
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    if(signalMapperConnector)
        return signalMapperConnector(object, link, signalMapperMapped);
    else return QMetaObject::Connection();
}
#endif // QT_QTJAMBI_PORT

void registerFunctionalResolver(const std::type_info& typeId, FunctionalResolver resolver)
{
    QWriteLocker locker(gLock());
    Q_UNUSED(locker)
    gFunctionalResolverHash->insert(typeId.hash_code(), resolver);
}

FunctionalResolver registeredFunctionalResolver(const std::type_info& typeId){
    QReadLocker locker(gLock());
    Q_UNUSED(locker)
    return gFunctionalResolverHash->value(typeId.hash_code(), nullptr);
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
    gMediaControlIIDHash->insert(typeId.hash_code(), media_control_iid);
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
            return gMediaControlIIDHash->value(typeId->hash_code(), nullptr);
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
        qtjambi_throw_java_exception(env)
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
            returned = Java::Runtime::ClassLoader::loadClass(env, className);
            if (env->ExceptionCheck() || returned == nullptr) {
                env->ExceptionClear();
                classLoader = nullptr;
            }
        }

        if(classLoader == nullptr){
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



                    for (int i=0; i<urlList.size(); ++i) {
                        if (!oldUrlList.contains(urlList.at(i))) {
                            jobject url = Java::Runtime::URL::newInstance(env, qtjambi_from_qstring(env, urlList.at(i)));
                            Q_ASSERT(url);
                            Java::Runtime::URLClassLoader::addURL(env, classLoader, url);
                        }
                    }

                    *oldUrlBase() = qtClassPath;
                }
                // Look up the class in our custom class loader
                returned = Java::Runtime::ClassLoader::loadClass(env, className);
                if (returned == nullptr && exception) {
#ifdef QTJAMBI_STACKTRACE
                    if(qtClassName=="io.qt.internal.QtJambiInternal")
                        JavaException(env, exception).raise();
                    else
#endif
                        JavaException(env, exception).raise( QTJAMBI_STACKTRACEINFO_ENV(env) );
                }
            } else {
#ifdef QTJAMBI_STACKTRACE
                if(qtClassName=="io.qt.internal.QtJambiInternal")
                    JavaException(env, exception).raise();
                else
#endif
                    JavaException(env, exception).raise( QTJAMBI_STACKTRACEINFO_ENV(env) );
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
            qtjambi_throw_java_exception(env)
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
        qtjambi_throw_java_exception(env)
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
        qtjambi_throw_java_exception(env)
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

bool qtjambi_block_exceptions(bool block){
    bool old = gBlockExceptionStorage->localData();
    gBlockExceptionStorage->setLocalData(block);
    return old;
}

bool qtjambi_is_exceptions_blocked(){
    return gBlockExceptionStorage->localData();
}


class JavaExceptionChecker : public QObject{
public:
    JavaExceptionChecker(){}
    ~JavaExceptionChecker(){}
    bool event(QEvent *event){
        if(event->type()==QEvent::DeferredDelete){
            bool b = QObject::event(event);
            qtjambi_check_blocked_exception();
            return b;
        }else{
            return QObject::event(event);
        }
    }
};

void qtjambi_push_blocked_exception(JNIEnv * env, const JavaException& exn){
    if(qtjambi_is_exceptions_blocked()){
        if(gJavaExceptionStorage->hasLocalData()){
            gJavaExceptionStorage->localData().addSuppressed(env, exn);
        }else{
            gJavaExceptionStorage->setLocalData(exn);
        }
        if(QThread::currentThread()->eventDispatcher()){
            JavaExceptionChecker* checker = new JavaExceptionChecker();
            checker->deleteLater();
        }
    }else{
        exn.raise();
    }
}

void qtjambi_check_blocked_exception(){
    if(gJavaExceptionStorage->hasLocalData()){
        JavaException exn = gJavaExceptionStorage->localData();
        gJavaExceptionStorage->setLocalData(JavaException());
        exn.raise();
    }
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

#ifdef QT_QTJAMBI_PORT

class WeakNativeIDPrivate : public QSharedData{
public:
    WeakNativeIDPrivate(const QSharedPointer<QtJambiLink>& link)
        :QSharedData(),
          weakLink(link)
    {}

    static WeakNativeID weakNativeID(const QSharedPointer<QtJambiLink>& link)
    {
        return new WeakNativeID(*new WeakNativeIDPrivate(link));
    }

    static QWeakPointer<QtJambiLink> getLink(const WeakNativeID& weakNativeID)
    {
        return weakNativeID.d->weakLink;
    }
private:
    QWeakPointer<QtJambiLink> weakLink;
};

WeakNativeID qtjambi_to_Weak_native_id(const QSharedPointer<QtJambiLink>& link)
{
    return WeakNativeIDPrivate::weakNativeID(link);
}

void qtjambi_emit_signal(JNIEnv * env, const WeakNativeID& weakNativeID, SignalMethodIndexProvider signalMethodIndexProvider, jvalue *arguments)
{
    if(QSharedPointer<QtJambiLink> link = getLink(weakNativeID)){
        if(link->isQObject()){
            if(link->isSharedPointer()){
                static_cast<QSharedPointerToQObjectLink*>(link.data())->emitSignal(env, signalMethodIndexProvider, arguments);
            }else{
                static_cast<PointerToQObjectLink*>(link.data())->emitSignal(env, signalMethodIndexProvider, arguments);
            }
        }
    }
}
#endif

jmethodID findEmitMethod(JNIEnv * env, jclass signalClass){
    if(!env->IsSameObject(signalClass, nullptr)){
        uint key = uint(Java::Runtime::Object::hashCode(env,signalClass));
        {
            QReadLocker locker(gEmitMethodLock());
            Q_UNUSED(locker)
            if(gEmitMethodHash->contains(key)){
                return (*gEmitMethodHash)[key];
            }
        }
        QWriteLocker locker(gEmitMethodLock());
        Q_UNUSED(locker)
        jobject signal_emit_method = Java::QtJambi::QtJambiInternal::findEmitMethod(env, signalClass);
        if(signal_emit_method){
            (*gEmitMethodHash)[key] = env->FromReflectedMethod(signal_emit_method);
            qtjambi_throw_java_exception(env)
            return (*gEmitMethodHash)[key];
        }
    }
    return nullptr;
}

void clear_registry_at_shutdown(JNIEnv * env){
    PolymorphicIdHash polymorphic_ids;
    {
        QWriteLocker locker(gLock());
        Q_UNUSED(locker)
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
        classIdHash.swap(*gClassHash);
    }
    {
        QWriteLocker locker(gEmitMethodLock());
        Q_UNUSED(locker)
        gEmitMethodHash->clear();
    }
    {
        QWriteLocker locker(gConstructorLock());
        Q_UNUSED(locker)
        gConstructorHash->clear();
    }
    {
        QWriteLocker locker(gQtSuperclassLock());
        Q_UNUSED(locker)
        gQtSuperclassHash->clear();
    }
    {
        QWriteLocker locker(gMethodLock());
        Q_UNUSED(locker)
        gMethodHash->clear();
    }
    {
        QWriteLocker locker(gFieldLock());
        Q_UNUSED(locker)
        gFieldHash->clear();
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
    if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperID(), _to))
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
