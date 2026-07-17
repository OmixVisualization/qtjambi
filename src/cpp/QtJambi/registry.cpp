/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include "containeraccess_export_stringlist.h"
#include "containeraccess_export_variantlist.h"
#include "containeraccess_export_bytearraylist.h"
#include "containeraccess_export_hash.h"
#include "containeraccess_export_pair.h"
#include "containeraccess_export_map.h"
#include "containeraccess_export_list.h"
#include "qtjambi_cast_template2.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
#define qAsConst std::as_const
#endif

#ifdef QTJAMBI_COUNTCACHEMISSES
static void cacheMisses(const char *s)
{
    static int count = 0;

    qCDebug(DebugAPI::internalCategory, "Total # of cache misses: %d : '%s'", ++count, s);
}
#endif

size_t qHash(const char *p, size_t seed) Q_DECL_NOEXCEPT
{
    return qHash(QByteArrayView(p), seed);
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
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->registryLock());
        if(qt_name){
            QByteArray _qt_name(qt_name);
            if(!storage->qtNameTypeHash().contains(_qt_name))
                storage->qtNameTypeHash().insert(_qt_name, &typeId);
        }
        if(java_name)
            storage->javaNameTypeHash()[java_name].append(&typeId);
    }
}

void registerTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name, EntryTypes entryTypes, bool registerJavaToQtType)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->registryLock());
        if(!storage->typeQtNameHash().contains(unique_id(typeId))){
            storage->typeQtNameHash().insert(unique_id(typeId), qt_name);
        }
        storage->qtNameTypeHash().insert(qt_name, &typeId);

        if(!storage->typeJavaNameHash().contains(unique_id(typeId))){
            storage->typeJavaNameHash().insert(unique_id(typeId), java_name);
        }
        if(registerJavaToQtType)
            storage->javaNameTypeHash()[java_name].append(&typeId);
        if(entryTypes!=EntryTypes::Unspecific)
            storage->entryTypesHash().insert(unique_id(typeId), entryTypes);
        if(entryTypes==EntryTypes::FunctionPointerTypeInfo
                || entryTypes==EntryTypes::StdFunctionTypeInfo){
            storage->qtFunctionalJavaNameHash().insert(qt_name, java_name);
            storage->qtFunctionalQtNameHash().insert(java_name, qt_name);
        }
    }
    QByteArray type_name = QtJambiAPI::typeName(typeId);
    if(type_name!=QByteArray(qt_name)){
        registerTypeAlias(typeId, type_name, nullptr);
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
void RegistryAPI::registerNativeInterface(const char* className, QPair<const char*, int>&& nameAndRevision){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->registryLock());
        storage->nativeInterfaceMap().insert(className, std::move(nameAndRevision));
    }
}

bool registeredNativeInterface(JNIEnv* env, jclass cls, QPair<const char*, int>& nameAndRevision){
    const QtJambiStorage* storage = getQtJambiStorage();
    {
        QByteArray className = QtJambiAPI::getClassNameJNI(env, cls);
        QReadLocker locker(storage->registryLock());
        if(storage->nativeInterfaceMap().contains(className)){
            nameAndRevision = storage->nativeInterfaceMap().value(className);
            return true;
        }
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
        JavaException::raiseIllegalArgumentException(env, QStringLiteral("Class %1 is not a native interface type.").arg(QtJambiAPI::getClassNamePrintable(env, cls)) QTJAMBI_STACKTRACEINFO );
    }
    return info;
}
#endif

void RegistryAPI::registerInterfaceTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name, const char *interface_iid)
{
    registerTypeInfo(typeId, qt_name, java_name, EntryTypes::InterfaceTypeInfo);
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->registryLock());
        storage->iidTypeHash().insert(interface_iid, &typeId);
        storage->javaClassIIDHash().insert(java_name, interface_iid);
    }
}

void RegistryAPI::registerIID(const std::type_info& typeId, const char *interface_iid)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->registryLock());
        storage->iidTypeHash().insert(interface_iid, &typeId);
        storage->javaClassIIDHash().insert(storage->typeJavaNameHash().value(unique_id(typeId), nullptr), interface_iid);
    }
}

const char * registeredInterfaceID(const std::type_info& typeId)
{
    const QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->javaClassIIDHash().value(storage->typeJavaNameHash().value(unique_id(typeId), nullptr));
    }
}

const char * registeredInterfaceIDForClassName(QByteArrayView className)
{
    const QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->javaClassIIDHash().value(className);
    }return nullptr;
}

const char * RegistryAPI::registerInterfaceID(JNIEnv* env, jclass cls)
{
    cls = getGlobalClassRef(env, cls);
    QByteArray className = QtJambiAPI::getClassNameJNI(env, cls);
    QByteArray iid = className;
    iid = iid.replace('/', '.');
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        if(storage->javaClassIIDHash().contains(className))
            return storage->javaClassIIDHash()[className];
    }
    {
        QWriteLocker locker(storage->registryLock());
        if(!storage->javaClassIIDHash().contains(className)){
            const char* _iid = storage->iidByteArrayHash().insert(iid, className).key();
            storage->javaClassIIDHash().insert(className, _iid);
            return _iid;
        }else{
            return storage->javaClassIIDHash()[className];
        }
    }
}

void RegistryAPI::registerInterfaceValueTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name, const char *interface_iid)
{
    registerTypeInfo(typeId, qt_name, java_name, EntryTypes::InterfaceValueTypeInfo);
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->registryLock());
        storage->iidTypeHash().insert(interface_iid, &typeId);
        storage->javaClassIIDHash().insert(java_name, interface_iid);
    }
}

void RegistryAPI::registerFunctionalTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name, bool isFunctionPointer, QMetaType&& metaType, QMetaType&& nativeMetaType,
                                             size_t size, size_t alignment, size_t sizeOfShell, size_t alignmentOfShell, FunctionalResolver resolver, uint returnScopes,
                                             RegistryAPI::DestructorFn destructor, std::initializer_list<ConstructorInfo> constructors,
                                             PtrDeleterFunction deleter, std::initializer_list<FunctionInfo> virtualFunctions)
{
    registerTypeInfo(typeId, qt_name, java_name, isFunctionPointer ? EntryTypes::FunctionPointerTypeInfo : EntryTypes::StdFunctionTypeInfo);
    registerSizeOfType(typeId, size, alignment);
    registerMetaType(typeId, metaType);
    int metaTypeId = metaType.id();
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->registryLock());
        storage->javaNameByMetaTypes().insert(metaTypeId, java_name);
        storage->metaTypesByJavaTypeNames().insert(java_name, metaType.iface());
    }
    if(sizeOfShell>0)
        registerSizeOfShell(typeId, sizeOfShell, alignmentOfShell);
    if(resolver)
        registerFunctionalResolver(typeId, resolver);
    if(destructor || constructors.size()>0)
        registerConstructorInfos(typeId, returnScopes, destructor, constructors);
    if(deleter)
        registerDeleter(typeId, deleter);
    if(virtualFunctions.size()>0)
        registerFunctionInfos(typeId, virtualFunctions);
    if(nativeMetaType.isValid()){
        QWriteLocker locker(storage->registryLock());
        storage->metaTypeByNativeMetaType().insert(std::move(nativeMetaType), std::move(metaType));
    }
}

void RegistryAPI::registerQObjectTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, qt_name, java_name, EntryTypes::QObjectTypeInfo);
}

void RegistryAPI::registerObjectTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, qt_name, java_name, EntryTypes::ObjectTypeInfo);
}

void RegistryAPI::registerValueTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, qt_name, java_name, EntryTypes::ValueTypeInfo);
}

void RegistryAPI::registerIteratorTypeInfo(const std::type_info& containerTypeId, const std::type_info& iteratorTypeId, const char *qt_name, const char *java_name)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->registryLock());
        storage->containersIteratorJavaNames()[unique_id(containerTypeId)][unique_id(iteratorTypeId)] = QPair<const char*,const char*>{qt_name,java_name};
    }
    registerTypeInfo(iteratorTypeId, qt_name, java_name, EntryTypes::IteratorTypeInfo);
}

QPair<const char*,const char*> iteratorJavaType(const std::type_info& containerTypeId, const std::type_info& iteratorTypeId)
{
    const QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->containersIteratorJavaNames()[unique_id(containerTypeId)][unique_id(iteratorTypeId)];
    }
}

void RegistryAPI::registerEnumTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, qt_name, java_name, EntryTypes::EnumTypeInfo);
}

void RegistryAPI::registerEnumTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name, const std::type_info& flagsTypeId, const char *flags_qt_name, const char *flags_qt_name_alias, const char *flags_java_name)
{
    registerTypeInfo(typeId, qt_name, java_name, EntryTypes::EnumTypeInfo);
    registerTypeInfo(flagsTypeId, flags_qt_name, flags_java_name, EntryTypes::FlagsTypeInfo);
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->registryLock());
        if(QLatin1String(flags_qt_name)!=QLatin1String(flags_qt_name_alias))
            storage->qtNameTypeHash().insert(flags_qt_name_alias, &flagsTypeId);
        storage->flagEnumIDHash().insert(unique_id(flagsTypeId), &typeId);
        storage->enumFlagIDHash().insert(unique_id(typeId), &flagsTypeId);
    }
}

void RegistryAPI::registerUnspecificTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, qt_name, java_name, EntryTypes::Unspecific);
}

void RegistryAPI::registerPrimitiveTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name)
{
    registerTypeInfo(typeId, qt_name, java_name, EntryTypes::PrimitiveTypeInfo);
}

void registerContainerTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name, const char *java_interface)
{
    registerTypeInfo(typeId, qt_name, java_name, EntryTypes::Unspecific);
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->registryLock());
        if(!storage->typeJavaInterfaceHash().contains(unique_id(typeId))){
            storage->typeJavaInterfaceHash().insert(unique_id(typeId), java_interface);
        }
    }
}

void RegistryAPI::registerNamespaceTypeInfo(const char *qt_name, const char *java_name, const QMetaObject* namespaceMetaObject){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->registryLock());
        if(!storage->javaNamespaceQtNameHash().contains(java_name)){
            storage->javaNamespaceQtNameHash().insert(java_name, qt_name);
        }
        if(!storage->qtNamespaceJavaNameHash().contains(qt_name)){
            storage->qtNamespaceJavaNameHash().insert(qt_name, java_name);
        }
        if(namespaceMetaObject && !storage->namespaceMetaObjectHash().contains(java_name)){
            storage->namespaceMetaObjectHash().insert(java_name, namespaceMetaObject);
        }
    }
}

void RegistryAPI::registerContainerAccessFactory(const std::type_info& typeId, NewContainerAccessFunction factory){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        {
            if(!storage->containerAccessFactories().contains(unique_id(typeId))){
                storage->containerAccessFactories().insert(unique_id(typeId), factory);
            }
        }
        if(AbstractContainerAccess* access = factory()){
            if(access->isList()){
                AbstractListAccess* ca = static_cast<AbstractListAccess*>(access);
                const QMetaType& mid = ca->elementMetaType();
                int metaTypeId = mid.id();
                QWriteLocker locker(storage->registryLock());
                if(!storage->sequentialContainerAccessFactoryByMetaTypes().contains(metaTypeId)){
                    storage->sequentialContainerAccessFactoryByMetaTypes()[metaTypeId][SequentialContainerType::QList] = factory;
                }
            }else if(access->isSet()){
                AbstractSetAccess* ca = static_cast<AbstractSetAccess*>(access);
                const QMetaType& mid = ca->elementMetaType();
                int metaTypeId = mid.id();
                QWriteLocker locker(storage->registryLock());
                if(!storage->sequentialContainerAccessFactoryByMetaTypes().contains(metaTypeId)){
                    storage->sequentialContainerAccessFactoryByMetaTypes()[metaTypeId][SequentialContainerType::QSet] = factory;
                }
            }else if(access->isHash()){
                AbstractHashAccess* ca = static_cast<AbstractHashAccess*>(access);
                const QMetaType& kid = ca->keyMetaType();
                const QMetaType& vid = ca->valueMetaType();
                QPair<int,int> pair{kid.id(),vid.id()};
                QWriteLocker locker(storage->registryLock());
                if(!storage->associativeContainerAccessFactoryByMetaTypes().contains(pair)){
                    storage->associativeContainerAccessFactoryByMetaTypes()[pair][AssociativeContainerType::QHash] = factory;
                }
            }else if(access->isMap()){
                AbstractMapAccess* ca = static_cast<AbstractMapAccess*>(access);
                const QMetaType& kid = ca->keyMetaType();
                const QMetaType& vid = ca->valueMetaType();
                QPair<int,int> pair{kid.id(),vid.id()};
                QWriteLocker locker(storage->registryLock());
                if(!storage->associativeContainerAccessFactoryByMetaTypes().contains(pair)){
                    storage->associativeContainerAccessFactoryByMetaTypes()[pair][AssociativeContainerType::QMap] = factory;
                }
            }else if(access->isMultiHash()){
                AbstractMultiHashAccess* ca = static_cast<AbstractMultiHashAccess*>(access);
                const QMetaType& kid = ca->keyMetaType();
                const QMetaType& vid = ca->valueMetaType();
                QPair<int,int> pair{kid.id(),vid.id()};
                QWriteLocker locker(storage->registryLock());
                if(!storage->associativeContainerAccessFactoryByMetaTypes().contains(pair)){
                    storage->associativeContainerAccessFactoryByMetaTypes()[pair][AssociativeContainerType::QMultiHash] = factory;
                }
            }else if(access->isMultiMap()){
                AbstractMultiMapAccess* ca = static_cast<AbstractMultiMapAccess*>(access);
                const QMetaType& kid = ca->keyMetaType();
                const QMetaType& vid = ca->valueMetaType();
                QPair<int,int> pair{kid.id(),vid.id()};
                QWriteLocker locker(storage->registryLock());
                if(!storage->associativeContainerAccessFactoryByMetaTypes().contains(pair)){
                    storage->associativeContainerAccessFactoryByMetaTypes()[pair][AssociativeContainerType::QMultiMap] = factory;
                }
            }
            access->dispose();
        }
    }
}

std::function<AbstractContainerAccess*()> getContainerAccessFactory(SequentialContainerType containerType, const QMetaType& type){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        int metaTypeId = type.id();
        QReadLocker locker(storage->registryLock());
        if(containerType==SequentialContainerType::QQueue){
            containerType = SequentialContainerType::QList;
        }else if(containerType==SequentialContainerType::QStack){
            containerType = SequentialContainerType::QList;
        }
        if(storage->sequentialContainerAccessFactoryByMetaTypes().contains(metaTypeId)){
            return storage->sequentialContainerAccessFactoryByMetaTypes()[metaTypeId][containerType];
        }else{
            return nullptr;
        }
    }
}

std::function<AbstractContainerAccess*()> getContainerAccessFactory(AssociativeContainerType containerType, const QMetaType& keyType, const QMetaType& valueType){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QPair<int,int> pair{keyType.id(),valueType.id()};
        QReadLocker locker(storage->registryLock());
        if(storage->associativeContainerAccessFactoryByMetaTypes().contains(pair)){
            return storage->associativeContainerAccessFactoryByMetaTypes()[pair][containerType];
        }else{
            return nullptr;
        }
    }
}

void registerContainerAccessFactory(SequentialContainerType containerType, const QMetaType& elementType, std::function<AbstractContainerAccess*()>&& factory){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        int metaTypeId = elementType.id();
        QWriteLocker locker(storage->registryLock());
        if(!storage->sequentialContainerAccessFactoryByMetaTypes()[metaTypeId].contains(containerType)
                || !storage->sequentialContainerAccessFactoryByMetaTypes()[metaTypeId][containerType])
            storage->sequentialContainerAccessFactoryByMetaTypes()[metaTypeId][containerType] = std::move(factory);
    }
}

void registerContainerAccessFactory(AssociativeContainerType containerType, const QMetaType& keyType, const QMetaType& valueType, std::function<AbstractContainerAccess*()>&& factory){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QPair<int,int> pair{keyType.id(),valueType.id()};
        QWriteLocker locker(storage->registryLock());
        if(storage->associativeContainerAccessFactoryByMetaTypes().contains(pair)){
            if(!storage->associativeContainerAccessFactoryByMetaTypes()[pair].contains(containerType)
                    || !storage->associativeContainerAccessFactoryByMetaTypes()[pair][containerType])
                storage->associativeContainerAccessFactoryByMetaTypes()[pair][containerType] = std::move(factory);
        }else{
            storage->associativeContainerAccessFactoryByMetaTypes()[pair][containerType] = std::move(factory);
        }
    }
}

NewContainerAccessFunction getContainerAccessFactory(const std::type_info& typeId){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->containerAccessFactories().value(unique_id(typeId));
    }
}


EntryTypes getEntryType(const std::type_info& typeId){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->entryTypesHash().value(unique_id(typeId), EntryTypes::Unspecific);
    }
}

const std::type_info* getInterfaceTypeForIID(const char*interface_iid)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->iidTypeHash().value(interface_iid, nullptr);
    }
}

bool isInterface(const std::type_info& typeId)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        EntryTypes entryType;
        {
            QReadLocker locker(storage->registryLock());
            entryType = storage->entryTypesHash().value(unique_id(typeId), EntryTypes::Unspecific);
        }
        return entryType == EntryTypes::InterfaceTypeInfo || entryType == EntryTypes::InterfaceValueTypeInfo;
    }
}

bool isQObject(const std::type_info& typeId)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->entryTypesHash().value(unique_id(typeId), EntryTypes::Unspecific) == EntryTypes::QObjectTypeInfo;
    }
}

bool isFunctional(const std::type_info& typeId)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        EntryTypes entryType = storage->entryTypesHash().value(unique_id(typeId), EntryTypes::Unspecific);
        return entryType==EntryTypes::StdFunctionTypeInfo || entryType==EntryTypes::FunctionPointerTypeInfo;
    }
}

bool isStdFunction(const std::type_info& typeId)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->entryTypesHash().value(unique_id(typeId), EntryTypes::Unspecific) == EntryTypes::StdFunctionTypeInfo;
    }
}

bool isFunctionPointer(const std::type_info& typeId)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->entryTypesHash().value(unique_id(typeId), EntryTypes::Unspecific) == EntryTypes::FunctionPointerTypeInfo;
    }
}

bool isValueType(JNIEnv * env, jclass valueType, QMetaType* metaTypeOut)
{
    const SuperTypeInfos infos = SuperTypeInfos::fromClass(env, valueType);
    if(metaTypeOut)
        *metaTypeOut = QMetaType(QMetaType::UnknownType);
    if (!infos.isEmpty()){
        if(const std::type_info* typeId = getTypeByJavaName(QtJambiAPI::getClassNameJNI(env, infos.first().javaClass()))){
            QtJambiStorage* storage = getQtJambiStorage();
            {
                QReadLocker locker(storage->registryLock());
                EntryTypes entryType = storage->entryTypesHash().value(unique_id(*typeId), EntryTypes::Unspecific);
                QMetaType metaType = registeredMetaType(*typeId);
                if(metaTypeOut && (metaType.isValid() && (entryType==EntryTypes::ValueTypeInfo || entryType==EntryTypes::InterfaceValueTypeInfo)))
                    *metaTypeOut = metaType;
                return (entryType==EntryTypes::ValueTypeInfo || entryType==EntryTypes::InterfaceValueTypeInfo)
                        && metaType.isValid();
            }
        }
    }
    return false;
}

const std::type_info* getTypeByJavaName(QByteArrayView javaName)
{
    const std::type_info* result = nullptr;
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        if(!storage->javaNameTypeHash().contains(javaName)){
            if(javaName.endsWith("$ConcreteWrapper")){
                javaName = javaName.chopped(16);
            }
            if(javaName.endsWith("$Impl")){
                javaName = javaName.chopped(5);
            }
        }
        if(storage->javaNameTypeHash().contains(javaName)){
            const QList<const std::type_info*>& list = storage->javaNameTypeHash()[javaName];
            if(!list.isEmpty()){
                result = list.first();
            }
        }
    }
    return result;
}

const char* getQtNameByFunctional(QByteArrayView java_name)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->qtFunctionalQtNameHash().value(java_name, nullptr);
    }
}

bool isJavaNameNamespace(const QByteArray& java_name){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->javaNamespaceQtNameHash().contains(java_name);
    }
}

const QMetaObject* registeredNamespaceMetaObject(const QByteArray& java_name){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->namespaceMetaObjectHash()[java_name];
    }
}

bool isQtNameNamespace(const QByteArray& qt_name){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->qtNamespaceJavaNameHash().contains(qt_name);
    }
}

const std::type_info* getTypeByQtName(const QByteArray& qt_name)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->qtNameTypeHash().value(qt_name, nullptr);
    }
}

const std::type_info* getTypeByQtName(QByteArrayView qt_name)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->qtNameTypeHash().value(qt_name, nullptr);
    }
}

const char * getQtName(const std::type_info& typeId)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->typeQtNameHash().value(unique_id(typeId), nullptr);
    }
}

const char * getJavaName(const std::type_info& typeId)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->typeJavaNameHash().value(unique_id(typeId), nullptr);
    }
}

const char * getJavaInterfaceName(const std::type_info& typeId)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        if(storage->typeJavaInterfaceHash().contains(unique_id(typeId)))
            return storage->typeJavaInterfaceHash().value(unique_id(typeId), nullptr);
        return storage->typeJavaNameHash().value(unique_id(typeId), nullptr);
    }
}

void registerMetaType(const std::type_info& typeId,
                        const std::type_info& nonPointerTypeId,
                        const QMetaType& metaType,
                        AbstractContainerAccess* access,
                        const QSharedPointer<AbstractContainerAccess>& sharedAccess)
{
    int metaTypeId = metaType.id();
    QByteArrayView registeredQtName;
    bool insert = false;
    QtJambiStorage* storage = getQtJambiStorage();
    {
        {
            QReadLocker locker(storage->registryLock());
            insert = !storage->metaTypeByTypeInfos().contains(unique_id(typeId))
                    || !storage->typeInfoByMetaTypes().contains(metaTypeId);
            registeredQtName = storage->typeQtNameHash().value(unique_id(typeId), nullptr);
        }
        if(insert){
            QWriteLocker locker(storage->registryLock());
            if(!storage->metaTypeByTypeInfos().contains(unique_id(typeId)))
                storage->metaTypeByTypeInfos().insert(unique_id(typeId), META_TYPE_ACCESS(metaType).iface());
            if(!storage->typeInfoByMetaTypes().contains(metaTypeId))
                storage->typeInfoByMetaTypes().insert(metaTypeId, &typeId);
        }
    }
    if(registeredQtName!=metaType.name() && !access && !sharedAccess){
        registerTypeAlias(typeId, metaType.name(), nullptr);
    }
    if(typeid_not_equals(nonPointerTypeId, typeid(JObjectWrapper))
            && typeid_not_equals(nonPointerTypeId, typeid(QString))
            && typeid_not_equals(nonPointerTypeId, typeid(QStringView))
            && typeid_not_equals(nonPointerTypeId, typeid(qint8))
            && typeid_not_equals(nonPointerTypeId, typeid(qint16))
            && typeid_not_equals(nonPointerTypeId, typeid(qint32))
            && typeid_not_equals(nonPointerTypeId, typeid(qint64))
            && typeid_not_equals(nonPointerTypeId, typeid(quint8))
            && typeid_not_equals(nonPointerTypeId, typeid(quint16))
            && typeid_not_equals(nonPointerTypeId, typeid(quint32))
            && typeid_not_equals(nonPointerTypeId, typeid(quint64))
            && typeid_not_equals(nonPointerTypeId, typeid(bool))
            && typeid_not_equals(nonPointerTypeId, typeid(wchar_t))
            && typeid_not_equals(nonPointerTypeId, typeid(QVariant))
            && typeid_not_equals(nonPointerTypeId, typeid(JObjectArrayWrapper))
            && typeid_not_equals(nonPointerTypeId, typeid(JIntArrayWrapper))
            && typeid_not_equals(nonPointerTypeId, typeid(JLongArrayWrapper))
            && typeid_not_equals(nonPointerTypeId, typeid(JShortArrayWrapper))
            && typeid_not_equals(nonPointerTypeId, typeid(JByteArrayWrapper))
            && typeid_not_equals(nonPointerTypeId, typeid(JBooleanArrayWrapper))
            && typeid_not_equals(nonPointerTypeId, typeid(JCharArrayWrapper))
            && typeid_not_equals(nonPointerTypeId, typeid(JDoubleArrayWrapper))
            && typeid_not_equals(nonPointerTypeId, typeid(JFloatArrayWrapper))
            && typeid_not_equals(nonPointerTypeId, typeid(JCollectionWrapper))
            && typeid_not_equals(nonPointerTypeId, typeid(JMapWrapper))
            && typeid_not_equals(nonPointerTypeId, typeid(std::nullptr_t))
            && !access
            && !sharedAccess){
        const std::type_info* _nonPointerTypeId = &nonPointerTypeId;
        QMetaType jObjectWrapperMetaType = QMetaType::fromType<JObjectWrapper>();
        if(!QMetaType::hasRegisteredConverterFunction(jObjectWrapperMetaType, metaType) && QByteArrayView("QRemoteObjectPendingCall")!=metaType.name()){
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
    }
}

void registerMetaObjectByMetaTypeInterface(const QtPrivate::QMetaTypeInterface* iface, const QMetaObject* mo){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->registryLock());
        storage->metaTypeMetaObjectHash().insert(iface, mo);
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
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->metaTypeMetaObjectHash().value(iface);
    }
}

void RegistryAPI::registerMetaType(const std::type_info& typeId, const QMetaType& netaType)
{
    ::registerMetaType(typeId, typeId, netaType);
}

const char* getPersistentByteArray(QByteArrayView typeName){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        auto iter = storage->persistentByteArrays().constFind(typeName);
        if(iter!=storage->persistentByteArrays().end())
            return iter.key().data();
    }
    {
        QWriteLocker locker(storage->registryLock());
        auto iter = storage->persistentByteArrays().insert(typeName.toByteArray(), QHashDummyValue());
        return iter.key().data();
    }
}

const char* getPersistentByteArray(QByteArray&& typeName){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        auto iter = storage->persistentByteArrays().constFind(typeName);
        if(iter!=storage->persistentByteArrays().end())
            return iter.key().data();
    }
    {
        QWriteLocker locker(storage->registryLock());
        auto iter = storage->persistentByteArrays().insert(std::move(typeName), QHashDummyValue());
        return iter.key().data();
    }
}

bool QmlAPI::registerMetaTypeConverter(JNIEnv *env, const QMetaType& metaType1, jclass jsvalueClass, const QMetaType& metaType2, jclass targetClass, jmethodID constructor, jmethodID factory){
    if(!QMetaType::hasRegisteredConverterFunction(metaType1, metaType2)){
        QtJambiUtils::InternalToExternalConverter converter1 = QtJambiTypeManager::getInternalToExternalConverter(env, metaType1.name(), metaType1, jsvalueClass);
        QtJambiUtils::ExternalToInternalConverter reconverter1 = QtJambiTypeManager::getExternalToInternalConverter(env, jsvalueClass, metaType1.name(), metaType1);
        QtJambiUtils::InternalToExternalConverter converter2 = QtJambiTypeManager::getInternalToExternalConverter(env, metaType2.name(), metaType2, targetClass);
        QtJambiUtils::ExternalToInternalConverter reconverter2 = QtJambiTypeManager::getExternalToInternalConverter(env, targetClass, metaType2.name(), metaType2);
        if(converter1 && reconverter1 && converter2 && reconverter2){
            if(constructor){
                return QMetaType::registerConverterFunction([parameter1 = ParameterTypeInfo{QMetaType(metaType1), jsvalueClass, std::move(converter1), std::move(reconverter1)},
                                                             parameter2 = ParameterTypeInfo{QMetaType(metaType2), targetClass, std::move(converter2), std::move(reconverter2)},
                                                             constructor](const void *src, void *target)->bool{
                    if(JniEnvironment env{200}){
                        jvalue jv;
                        jv.l = nullptr;
                        if(parameter1.convertInternalToExternal(env, nullptr, src, jv, true)){
                            jobject result{nullptr};
                            try{
                                if(env->ExceptionCheck()){
                                    env->ExceptionDescribe();
                                    env->ExceptionClear();
                                }
                                result = env->NewObject(parameter2.javaClass(), constructor, jv.l);
                                JavaException::check(env QTJAMBI_STACKTRACEINFO );
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
            }else if(factory){
                return QMetaType::registerConverterFunction([parameter1 = ParameterTypeInfo{QMetaType(metaType1), jsvalueClass, std::move(converter1), std::move(reconverter1)},
                                                             parameter2 = ParameterTypeInfo{QMetaType(metaType2), targetClass, std::move(converter2), std::move(reconverter2)},
                                                             factory](const void *src, void *target)->bool{
                    if(JniEnvironment env{200}){
                        jvalue jv;
                        jv.l = nullptr;
                        if(parameter1.convertInternalToExternal(env, nullptr, src, jv, true)){
                            jobject result{nullptr};
                            try{
                                if(env->ExceptionCheck()){
                                    env->ExceptionDescribe();
                                    env->ExceptionClear();
                                }
                                result = env->CallStaticObjectMethod(parameter2.javaClass(), factory, jv.l);
                                JavaException::check(env QTJAMBI_STACKTRACEINFO );
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
    }
    return false;
}

QMetaType QmlAPI::registerQmlMetaType(JNIEnv *env, jclass clazz,
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
    return metaType;
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
            /*.name=*/ copyName ? getPersistentByteArray(typeName) : typeName.data(),
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
    QMetaType result(metaTypeInterface);
    result.id();
    return result;
}

QMetaType RegistryAPI::registerMetaType(const std::type_info& typeId,
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
        bool needsRegistration{false};
        QtJambiStorage* storage = getQtJambiStorage();
        {
            QReadLocker locker(storage->registryLock());
            needsRegistration = !storage->metaTypeByTypeInfos().contains(unique_id(typeId));
        }
        if(needsRegistration)
            ::registerMetaType(typeId, nonPointerTypeId, metaType);
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
            afterRegistrationFunction(metaType.iface());
        ::registerMetaType(typeId, nonPointerTypeId, metaType);
    }
    return metaType;
}

void registerNestedAccess(QtJambiStorage* storage, QWriteLocker &locker, AbstractContainerAccess* access){
    Q_ASSERT(access);
    Q_ASSERT(storage);
    if(access->isPair()){
        auto daccess = static_cast<AbstractPairAccess*>(access);
        locker.unlock();
        const QMetaType& firstMetaType = daccess->firstMetaType();
        const QMetaType& secondMetaType = daccess->secondMetaType();
        int id1 = firstMetaType.id();
        int id2 = secondMetaType.id();
        locker.relock();
        if(!storage->containerAccessMap().contains(id1) || !storage->containerAccessMap().contains(id2)){
            locker.unlock();
            if(AbstractNestedPairAccess* naccess = daccess->asNested()){
                if(QSharedPointer<AbstractContainerAccess> _access = naccess->sharedFirstNestedContainerAccess()){
                    locker.relock();
                    if(!storage->containerAccessMap().contains(id1))
                        storage->containerAccessMap().insert(id1, _access);
                    locker.unlock();
                }
                if(QSharedPointer<AbstractContainerAccess> _access = naccess->sharedSecondNestedContainerAccess()){
                    locker.relock();
                    if(!storage->containerAccessMap().contains(id2))
                        storage->containerAccessMap().insert(id2, _access);
                    locker.unlock();
                }
            }else{
                if(AbstractContainerAccess* _access = daccess->firstNestedContainerAccess()){
                    locker.relock();
                    if(!storage->containerAccessMap().contains(id1))
                        storage->containerAccessMap().insert(id1, QSharedPointer<AbstractContainerAccess>(_access->clone(), &containerDisposer));
                    locker.unlock();
                }
                if(AbstractContainerAccess* _access = daccess->secondNestedContainerAccess()){
                    locker.relock();
                    if(!storage->containerAccessMap().contains(id2))
                        storage->containerAccessMap().insert(id2, QSharedPointer<AbstractContainerAccess>(_access->clone(), &containerDisposer));
                    locker.unlock();
                }
            }
            locker.relock();
        }
    }else if(access->isAssociative()){
        auto daccess = static_cast<AbstractAssociativeAccess*>(access);
        locker.unlock();
        const QMetaType& keyMetaType = daccess->keyMetaType();
        const QMetaType& valueMetaType = daccess->valueMetaType();
        int id1 = keyMetaType.id();
        int id2 = valueMetaType.id();
        locker.relock();
        if(!storage->containerAccessMap().contains(id1) || !storage->containerAccessMap().contains(id2)){
            locker.unlock();
            if(AbstractNestedAssociativeAccess* naccess = daccess->asNested()){
                if(QSharedPointer<AbstractContainerAccess> _access = naccess->sharedKeyNestedContainerAccess()){
                    locker.relock();
                    if(!storage->containerAccessMap().contains(id1))
                        storage->containerAccessMap().insert(id1, _access);
                    locker.unlock();
                }
                if(QSharedPointer<AbstractContainerAccess> _access = naccess->sharedValueNestedContainerAccess()){
                    locker.relock();
                    if(!storage->containerAccessMap().contains(id2))
                        storage->containerAccessMap().insert(id2, _access);
                    locker.unlock();
                }
            }else{
                if(AbstractContainerAccess* _access = daccess->keyNestedContainerAccess()){
                    locker.relock();
                    if(!storage->containerAccessMap().contains(id1))
                        storage->containerAccessMap().insert(id1, QSharedPointer<AbstractContainerAccess>(_access->clone(), &containerDisposer));
                    locker.unlock();
                }
                if(AbstractContainerAccess* _access = daccess->valueNestedContainerAccess()){
                    locker.relock();
                    if(!storage->containerAccessMap().contains(id2))
                        storage->containerAccessMap().insert(id2, QSharedPointer<AbstractContainerAccess>(_access->clone(), &containerDisposer));
                    locker.unlock();
                }
            }
            locker.relock();
        }
    }else if(access->isSequential()){
        auto daccess = static_cast<AbstractSequentialAccess*>(access);
        locker.unlock();
        const QMetaType& elementMetaType = daccess->elementMetaType();
        int id1 = elementMetaType.id();
        locker.relock();
        if(!storage->containerAccessMap().contains(id1)){
            locker.unlock();
            if(AbstractNestedSequentialAccess* naccess = daccess->asNested()){
                if(QSharedPointer<AbstractContainerAccess> _access = naccess->sharedElementNestedContainerAccess()){
                    locker.relock();
                    storage->containerAccessMap().insert(id1, _access);
                    locker.unlock();
                }
            }else{
                if(AbstractContainerAccess* _access = daccess->elementNestedContainerAccess()){
                    locker.relock();
                    storage->containerAccessMap().insert(id1, QSharedPointer<AbstractContainerAccess>(_access->clone(), &containerDisposer));
                    locker.unlock();
                }
            }
            locker.relock();
        }
    }
}

QSharedPointer<AbstractContainerAccess> findContainerAccess(const QMetaType& metaType){
    int id = metaType.id();
    QSharedPointer<AbstractContainerAccess> result;
    QtJambiStorage* storage = getQtJambiStorage();
    {
        {
            QReadLocker locker(storage->registryLock());
            result = storage->containerAccessMap().value(id);
        }
        if(!result){
            switch(metaType.id()){
            case QMetaType::QStringList:
                result.reset(QListAccess<QString>::newInstance(), &containerDisposer);
                {
                    QWriteLocker locker(storage->registryLock());
                    storage->containerAccessMap().insert(id, result);
                    registerNestedAccess(storage, locker, result.data());
                }
                break;
            case QMetaType::QVariantList:
                result.reset(QListAccess<QVariant>::newInstance(), &containerDisposer);
                {
                    QWriteLocker locker(storage->registryLock());
                    storage->containerAccessMap().insert(id, result);
                    registerNestedAccess(storage, locker, result.data());
                }
                break;
            case QMetaType::QVariantHash:
                result.reset(QHashAccess<QString,QVariant>::newInstance(), &containerDisposer);
                {
                    QWriteLocker locker(storage->registryLock());
                    storage->containerAccessMap().insert(id, result);
                    registerNestedAccess(storage, locker, result.data());
                }
                break;
            case QMetaType::QVariantMap:
                result.reset(QMapAccess<QString,QVariant>::newInstance(), &containerDisposer);
                {
                    QWriteLocker locker(storage->registryLock());
                    storage->containerAccessMap().insert(id, result);
                    registerNestedAccess(storage, locker, result.data());
                }
                break;
            case QMetaType::QVariantPair:
                result.reset(QPairAccess<QVariant,QVariant>::newInstance(), &containerDisposer);
                {
                    QWriteLocker locker(storage->registryLock());
                    storage->containerAccessMap().insert(id, result);
                    registerNestedAccess(storage, locker, result.data());
                }
                break;
            default: break;
            }
        }
    }
    return result;
}

bool hasRegisteredContainerAccess(QMetaType metaType){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return !storage->containerAccessMap().value(metaType.id()).isNull();
    }
}

QMetaType registerMetaTypeImpl(const std::type_info* typeId,
                     const std::type_info* nonPointerTypeId,
                     QByteArrayView typeName,
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
                     AfterRegistrationFunction afterRegistrationFunction,
                     AbstractContainerAccess* access,
                     const QSharedPointer<AbstractContainerAccess>& sharedAccess)
{
    QMetaType metaType = QMetaType::fromName(typeName);
    if(metaType.isValid()){
        bool needsRegistration{false};
        bool needsContainerAccessRegistration{false};
        QtJambiStorage* storage = getQtJambiStorage();
        {
            QReadLocker locker(storage->registryLock());
            needsContainerAccessRegistration = (access || sharedAccess) && !storage->containerAccessMap().contains(metaType.id());
            needsRegistration = typeId && !storage->metaTypeByTypeInfos().contains(unique_id(*typeId));
        }
        if(needsContainerAccessRegistration){
            if(sharedAccess){
                QWriteLocker locker(storage->registryLock());
                storage->containerAccessMap().insert(metaType.id(), sharedAccess);
                registerNestedAccess(storage, locker, sharedAccess.data());
            }else if(access){
                QSharedPointer<AbstractContainerAccess> shared(access->clone(), &containerDisposer);
                QWriteLocker locker(storage->registryLock());
                storage->containerAccessMap().insert(metaType.id(), std::move(shared));
                registerNestedAccess(storage, locker, access);
            }
        }
        if(needsRegistration && typeId && nonPointerTypeId)
            ::registerMetaType(*typeId, *nonPointerTypeId, metaType, access, sharedAccess);
    }else{
        metaType = createMetaType(typeName,
                                  copyName,
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
            afterRegistrationFunction(metaType.iface());
        if(access || sharedAccess){
            QtJambiStorage* storage = getQtJambiStorage();
            bool insert{false};
            {
                QReadLocker locker(storage->registryLock());
                insert = !storage->containerAccessMap().contains(metaType.id());
            }
            if(insert){
                if(!storage->containerAccessMap().contains(metaType.id())){
                    if(sharedAccess){
                        QWriteLocker locker(storage->registryLock());
                        storage->containerAccessMap().insert(metaType.id(), sharedAccess);
                        registerNestedAccess(storage, locker, sharedAccess.data());
                    }else if(access){
                        QSharedPointer<AbstractContainerAccess> shared(access->clone(), &containerDisposer);
                        QWriteLocker locker(storage->registryLock());
                        storage->containerAccessMap().insert(metaType.id(), std::move(shared));
                        registerNestedAccess(storage, locker, access);
                    }
                }
            }
        }
        if(typeId && nonPointerTypeId){
            ::registerMetaType(*typeId, *nonPointerTypeId, metaType, access, sharedAccess);
        }
    }
    return metaType;
}

void registerContainerAccess(QMetaType metaType, const QSharedPointer<AbstractContainerAccess>& sharedAccess){
    if(sharedAccess){
        QtJambiStorage* storage = getQtJambiStorage();
        {
            QReadLocker locker(storage->registryLock());
            if(storage->containerAccessMap().contains(metaType.id()))
                return;
        }
        {
            QWriteLocker locker(storage->registryLock());
            storage->containerAccessMap().insert(metaType.id(), sharedAccess);
        }
    }
}

void registerContainerAccess(QMetaType metaType, AbstractContainerAccess* access){
    if(access){
        QtJambiStorage* storage = getQtJambiStorage();
        {
            QReadLocker locker(storage->registryLock());
            if(storage->containerAccessMap().contains(metaType.id()))
                return;
        }
        {
            QSharedPointer<AbstractContainerAccess> shared(access->clone(), &containerDisposer);
            QWriteLocker locker(storage->registryLock());
            storage->containerAccessMap().insert(metaType.id(), std::move(shared));
        }
    }
}

QMetaType RegistryAPI::registerMetaType(const std::type_info& typeId,
                     const std::type_info& nonPointerTypeId,
                     QByteArrayView typeName,
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
                     AbstractContainerAccess* access)
{
    return registerMetaTypeImpl(&typeId,
                                &nonPointerTypeId,
                                typeName,
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
                                afterRegistrationFunction,
                                access, {});
}

QMetaType registerContainerMetaType(QByteArrayView typeName,
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
                                afterRegistrationFunction,
                                nullptr, sharedAccess);
}

QMetaType registerSmartPointerMetaType(const QByteArray& typeName,
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
                                nullptr,
                                nullptr,
                                nullptr, {});
}

QMetaType registeredMetaType(const std::type_info& typeId)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        size_t uid = unique_id(typeId);
        QReadLocker locker(storage->registryLock());
        return QMetaType(storage->metaTypeByTypeInfos().value(uid, nullptr));
    }
}

const std::type_info* getTypeByMetaType(const QMetaType& metaType)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        int metaTypeId = metaType.id();
        {
            QReadLocker locker(storage->registryLock());
            if(const std::type_info* info = storage->typeInfoByMetaTypes().value(metaTypeId, nullptr)){
                return info;
            }
        }
        return getTypeByQtName(metaType.name());
    }
}

void RegistryAPI::registerFunctionInfos(const std::type_info& typeId, std::initializer_list<FunctionInfo> virtualFunctions)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        size_t uid = unique_id(typeId);
        QWriteLocker locker(storage->registryLock());
        if(!storage->virtualFunctionInfos().contains(uid))
            storage->virtualFunctionInfos().insert(uid, virtualFunctions);
    }
}

void RegistryAPI::registerConstructorInfos(const std::type_info& typeId, uint returnScopes, RegistryAPI::DestructorFn destructor, std::initializer_list<ConstructorInfo> constructors){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        size_t uid = unique_id(typeId);
        QWriteLocker locker(storage->registryLock());
        if(!storage->constructorInfos().contains(uid))
            storage->constructorInfos().insert(uid, constructors);
        if(!storage->destructorHash().contains(uid))
            storage->destructorHash().insert(uid, destructor);
        if(returnScopes>0 && !storage->returnScopes().contains(uid))
            storage->returnScopes().insert(uid, returnScopes);
    }
}

uint returnScopes(const std::type_info& typeId){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        size_t uid = unique_id(typeId);
        QReadLocker locker(storage->registryLock());
        return storage->returnScopes().value(uid, 0);
    }
}

const QVector<const RegistryAPI::ConstructorInfo>* registeredConstructorInfos(const std::type_info& typeId){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        size_t uid = unique_id(typeId);
        QReadLocker locker(storage->registryLock());
        if(storage->constructorInfos().contains(uid)){
            return reinterpret_cast<const QVector<const RegistryAPI::ConstructorInfo>*>(&storage->constructorInfos()[uid]);
        }
    }
    return nullptr;
}

void registerInterface(const char *qt_interface, const char *java_interface, const char *interface_iid)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto hashcode = qHash(qt_interface);
        auto hashcode2 = interface_iid ? qHash(interface_iid) : 0;
        QWriteLocker locker(storage->registryLock());
        storage->interfaceHash().insert(hashcode, java_interface);
        if(interface_iid)
            storage->interfaceIIDsHash().insert(hashcode2, qt_interface);
    }
}

bool isInterface(QByteArrayView qt_interface)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto hashcode = qHash(qt_interface);
        QReadLocker locker(storage->registryLock());
        return storage->interfaceHash().contains(hashcode);
    }
    return false;
}

const char* getInterface(QByteArrayView qt_interface)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto hashcode = qHash(qt_interface);
        QReadLocker locker(storage->registryLock());
        return storage->interfaceHash().value(hashcode, nullptr);
    }
    return nullptr;
}

void registerFunctional(const char *qt_interface)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto hashcode = qHash(qt_interface);
        QWriteLocker locker(storage->registryLock());
        storage->functionalHash().insert(hashcode);
    }
}

bool isFunctional(const char*qt_interface)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto hashcode = qHash(qt_interface);
        QReadLocker locker(storage->registryLock());
        return storage->functionalHash().contains(hashcode);
    }
    return false;
}

void RegistryAPI::registerDeleter(const std::type_info& typeId, PtrDeleterFunction deleter)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->registryLock());
        storage->deleterHash().insert(unique_id(typeId), deleter);
    }
}

PtrDeleterFunction deleter(const std::type_info& typeId)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->deleterHash().value(unique_id(typeId), nullptr);
    }
    return nullptr;
}

void RegistryAPI::registerOwnerFunction(const std::type_info& typeId, PtrOwnerFunction ownerFunction)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->registryLock());
        storage->ownerFunctionHash().insert(unique_id(typeId), ownerFunction);
    }
}

PtrOwnerFunction registeredOwnerFunction(const std::type_info& typeId)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->ownerFunctionHash().value(unique_id(typeId), nullptr);
    }
    return nullptr;
}

void clear_type_entry(const std::type_info& typeId);

void RegistryAPI::registerPolymorphyHandler(const std::type_info& polymorphicBaseTypeId, const std::type_info& targetTypeId, PolymorphyHandler handler)
{
    if(handler){
        QtJambiStorage* storage = getQtJambiStorage();
        {
            QWriteLocker locker(storage->registryLock());
            storage->polymorphicIDs().insert(unique_id(polymorphicBaseTypeId), QExplicitlySharedDataPointer<const PolymorphicIdHandler>(new PolymorphicIdHandler(targetTypeId, handler)));
            storage->polymorphicBasesHash().insert(unique_id(targetTypeId), &polymorphicBaseTypeId);
        }
        clear_type_entry(polymorphicBaseTypeId);
    }
}

bool isPolymorphicBase(const std::type_info& typeId){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->polymorphicIDs().contains(unique_id(typeId));
    }
    return false;
}

QList<const std::type_info*> getPolymorphicBases(const std::type_info& typeId){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->polymorphicBasesHash().values(unique_id(typeId));
    }
    return {};
}

QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>> getPolymorphicIdHandlers(const std::type_info& polymorphicBaseTypeId){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->polymorphicIDs().values(unique_id(polymorphicBaseTypeId));
    }
    return {};
}

void RegistryAPI::registerSizeOfType(const std::type_info& typeId, size_t size, size_t alignment)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        {
            QReadLocker locker(storage->registryLock());
            if(storage->valueSizeHash().contains(unique_id(typeId)))
                return;
        }
        {
            QWriteLocker locker(storage->registryLock());
            if(!storage->valueSizeHash().contains(unique_id(typeId)))
                storage->valueSizeHash().insert(unique_id(typeId), {size, alignment});
        }
    }
}

void RegistryAPI::registerSizeOfShell(const std::type_info& typeId, size_t size, size_t alignment)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        {
            QReadLocker locker(storage->registryLock());
            if(storage->shellSizeHash().contains(unique_id(typeId)))
                return;
        }
        {
            QWriteLocker locker(storage->registryLock());
            if(!storage->shellSizeHash().contains(unique_id(typeId)))
                storage->shellSizeHash().insert(unique_id(typeId), {size, alignment});
        }
    }
}

QMap<QString,QPair<size_t,size_t>> getRegisteredTypeSizesAndAlignments(){
    QSet<int> types;
    QSet<const QtPrivate::QMetaTypeInterface *> types2;
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
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        for(const QtPrivate::QMetaTypeInterface * mt : storage->metaTypeByTypeInfos().values())
            types2 << mt;
    }
    QMap<QString,QPair<size_t,size_t>> result;
    for(int type : qAsConst(types)){
        if(type!=QMetaType::Nullptr
                && type!=QMetaType::UnknownType
                && type!=QMetaType::Void){
            QMetaType metaType(type);
            result[QLatin1String(metaType.name())] = {
                    metaType.sizeOf(),
                    metaType.alignOf()};
        }
    }
    for(const QtPrivate::QMetaTypeInterface * iface : qAsConst(types2)){
        QMetaType metaType(iface);
        if(metaType.isValid()){
            result[QLatin1String(metaType.name())] = {
                    metaType.sizeOf(),
                    metaType.alignOf()};
        }
    }
    return result;
}

void RegistryAPI::registerHashFunction(const std::type_info& typeId, qHashFn hashFunction){
    auto uniq = unique_id(typeId);
    QtJambiStorage* storage = getQtJambiStorage();
    {
        {
            QReadLocker locker(storage->registryLock());
            if(storage->hashFunctionHash().contains(uniq))
                return;
        }
        {
            QWriteLocker locker(storage->registryLock());
            if(!storage->hashFunctionHash().contains(uniq))
                storage->hashFunctionHash().insert(uniq, hashFunction);
        }
    }
}

RegistryAPI::qHashFn registeredHashFunction(const std::type_info& typeId)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto uniq = unique_id(typeId);
        QReadLocker locker(storage->registryLock());
        return storage->hashFunctionHash().value(uniq, nullptr);
    }
}

QPair<size_t,size_t> getValueSizeAndAlignment(const std::type_info& typeId)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto uniq = unique_id(typeId);
        QReadLocker locker(storage->registryLock());
        return storage->valueSizeHash().value(uniq, {0,0});
    }
}

QPair<size_t,size_t> getShellSizeAndAlignment(const std::type_info& typeId)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto uniq = unique_id(typeId);
        QReadLocker locker(storage->registryLock());
        return storage->shellSizeHash().value(uniq, {0,0});
    }
}

void RegistryAPI::registerInterfaceOffset(const std::type_info& qt_base, const std::type_info& qt_interface, int offset)
{
    if(offset!=0){
        auto uniq = unique_id(qt_base);
        auto uniq2 = unique_id(qt_interface);
        QtJambiStorage* storage = getQtJambiStorage();
        {
            QWriteLocker locker(storage->registryLock());
            storage->offsetHash()[uniq].insert(uniq2, offset);
            storage->offsetHash()[uniq2].insert(uniq, -offset);
            storage->interfacesHash()[uniq] << &qt_interface;
        }
    }
}

int registeredInterfaceOffset(const std::type_info& qt_base, const std::type_info& qt_interface){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto uniq = unique_id(qt_base);
        auto uniq2 = unique_id(qt_interface);
        QReadLocker locker(storage->registryLock());
        if(storage->offsetHash().contains(uniq)){
            return storage->offsetHash()[uniq].value(uniq2, -1);
        }
    }
    return -1;
}

void registeredInterfaceOffsets(const std::type_info& qt_type, InterfaceOffsetInfo* info){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto uniq = unique_id(qt_type);
        QReadLocker locker(storage->registryLock());
        if(storage->offsetHash().contains(uniq)){
            const QMap<size_t, int>& values = storage->offsetHash()[uniq];
            for(QMap<size_t, int>::const_key_value_iterator i = values.keyValueBegin(); i!=values.keyValueEnd(); ++i){
                info->interfaces.insert(i.base().key());
                if(i.base().value()>0)
                    info->offsets.insert(i.base().key(), uint(i.base().value()));
            }
        }
        if(storage->interfacesHash().contains(uniq)){
            info->inheritedInterfaces[uniq] = storage->interfacesHash()[uniq];
        }
    }
}

const InterfaceOffsetInfo* getInterfaceOffsets(JNIEnv *env, jclass clazz){
    if(clazz){
        QtJambiStorage* storage = getQtJambiStorage();
        {
            uint classHash = uint(Java::Runtime::Object::hashCode(env,clazz));
            {
                QReadLocker locker(storage->registryLock());
                if(storage->interfaceOffsetHash().contains(classHash))
                    return &storage->interfaceOffsetHash()[classHash];
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
                    QByteArray className = QtJambiAPI::getClassNameJNI(env, cls);
                    const std::type_info* qt_type = getTypeByJavaName(className);
                    Q_ASSERT(qt_type);
                    registeredInterfaceOffsets(*qt_type, &result);
                    cls = env->GetSuperclass(cls);
                }
                QWriteLocker wlocker(storage->registryLock());
                if(storage->interfaceOffsetHash().contains(classHash))
                    return &storage->interfaceOffsetHash()[classHash];
                storage->interfaceOffsetHash().insert(classHash, result);
                return &storage->interfaceOffsetHash()[classHash];
            }
        }
    }
    return nullptr;
}

const InterfaceOffsetInfo* getInterfaceOffsets(JNIEnv *env, jclass clazz, const std::type_info& typeId, const SuperTypeInfos* superTypeInfos){
    if(clazz){
        QtJambiStorage* storage = getQtJambiStorage();
        {
            uint classHash = uint(Java::Runtime::Object::hashCode(env,clazz));
            {
                QReadLocker locker(storage->registryLock());
                if(storage->interfaceOffsetHash().contains(classHash))
                    return &storage->interfaceOffsetHash()[classHash];
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
                QWriteLocker wlocker(storage->registryLock());
                if(storage->interfaceOffsetHash().contains(classHash))
                    return &storage->interfaceOffsetHash()[classHash];
                storage->interfaceOffsetHash()[classHash] = std::move(result);
                return &storage->interfaceOffsetHash()[classHash];
            }
        }
    }
    return nullptr;
}

void registerDestructor(const std::type_info& typeId, RegistryAPI::DestructorFn destructor)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto uniq = unique_id(typeId);
        QWriteLocker locker(storage->registryLock());
        storage->destructorHash().insert(uniq, destructor);
    }
}

RegistryAPI::DestructorFn registeredDestructor(const std::type_info& typeId){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto uniq = unique_id(typeId);
        QReadLocker locker(storage->registryLock());
        return storage->destructorHash().value(uniq, nullptr);
    }
    return nullptr;
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
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto uniq = unique_id(typeId);
        QWriteLocker locker(storage->registryLock());
        storage->customMetaObject().insert(uniq, &superTypeMetaObject);
    }
}

bool hasCustomMetaObject(const std::type_info& typeId, const QMetaObject** superTypeMetaObject){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto uniq = unique_id(typeId);
        QReadLocker locker(storage->registryLock());
        if(storage->customMetaObject().contains(uniq)){
            if(superTypeMetaObject)
                *superTypeMetaObject = storage->customMetaObject().value(uniq, nullptr);
            return true;
        }
    }
    return false;
}

const QMetaObject* customMetaObject(const std::type_info& typeId)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->customMetaObject().value(unique_id(typeId), nullptr);
    }
    return nullptr;
}

RegisteredTypeInfo registeredTypeInfo(const std::type_info& typeId, const char* qtName){
    RegisteredTypeInfo result;
    QtJambiStorage* storage = getQtJambiStorage();
    size_t uid = unique_id(typeId);
    {
        QReadLocker locker(storage->registryLock());
        result.entryType = storage->entryTypesHash().value(uid, EntryTypes::Unspecific);
        result.metaType = QMetaType(storage->metaTypeByTypeInfos().value(uid, nullptr));
        if(result.metaType.isValid() && !(result.metaType.flags() & QMetaType::IsPointer)){
            switch(result.entryType){
            case EntryTypes::ObjectTypeInfo:
                result.entryType = EntryTypes::ValueTypeInfo;
                break;
            case EntryTypes::InterfaceTypeInfo:
                result.entryType = EntryTypes::InterfaceValueTypeInfo;
                break;
            default: break;
            }
        }
        switch(result.entryType){
        case EntryTypes::Unspecific:
            if(qtName && (result.javaName = storage->qtFunctionalJavaNameHash().value(qtName, nullptr))){
                result.qtName = qtName;
                if(result.metaType.flags() & QMetaType::IsPointer){
                    result.entryType = EntryTypes::FunctionPointerTypeInfo;
                }else{
                    result.entryType = EntryTypes::StdFunctionTypeInfo;
                }
            }
            break;
        case EntryTypes::FunctionPointerTypeInfo:
        case EntryTypes::StdFunctionTypeInfo:
            result.qtName = storage->typeQtNameHash().value(uid, nullptr);
            if(qtName){
                if(QByteArrayView(qtName)!=QByteArrayView(result.qtName)){
                    result.qtName = qtName;
                }
            }
            result.javaName = storage->qtFunctionalJavaNameHash().value(result.qtName, nullptr);
            break;
        default:
            result.qtName = storage->typeQtNameHash().value(uid, nullptr);
            result.javaName = storage->typeJavaNameHash().value(uid, nullptr);
            break;
        }

        if(result.entryType!=EntryTypes::Unspecific){
            if(qtName && (result.entryType==EntryTypes::StdFunctionTypeInfo || result.entryType==EntryTypes::FunctionPointerTypeInfo)){
                if(QByteArrayView(qtName)!=QByteArrayView(result.qtName)){
                    result.qtName = qtName;
                    result.javaName = storage->qtFunctionalJavaNameHash().value(qtName, nullptr);
                }
            }
            switch(result.entryType){
            case EntryTypes::EnumTypeInfo:
                result.flagForEnum = storage->enumFlagIDHash().value(uid, nullptr);
                result.valueSizeAndAlignment = storage->valueSizeHash().value(uid, {0,0});
                break;
            case EntryTypes::FlagsTypeInfo:
                result.enumForFlag = storage->flagEnumIDHash().value(uid, nullptr);
                result.valueSizeAndAlignment = storage->valueSizeHash().value(uid, {0,0});
                break;
            case EntryTypes::FunctionPointerTypeInfo:
            case EntryTypes::StdFunctionTypeInfo:
                result.valueSizeAndAlignment = storage->valueSizeHash().value(uid, {0,0});
                result.shellSizeAndAlignment = storage->shellSizeHash().value(uid, {0,0});
                result.virtualFunctions = storage->virtualFunctionInfos().value(uid, {});;
                result.destructor = storage->destructorHash().value(uid, nullptr);
                result.functionalResolver = storage->functionalResolverHash().value(uid, nullptr);
                break;
            case EntryTypes::ValueTypeInfo:
            case EntryTypes::ObjectTypeInfo:
            case EntryTypes::InterfaceTypeInfo:
            case EntryTypes::InterfaceValueTypeInfo:
            case EntryTypes::QObjectTypeInfo:
                result.containerAccessFactory = storage->containerAccessFactories().value(uid, nullptr);
                result.valueSizeAndAlignment = storage->valueSizeHash().value(uid, {0,0});
                result.shellSizeAndAlignment = storage->shellSizeHash().value(uid, {0,0});
                result.virtualFunctions = storage->virtualFunctionInfos().value(uid, {});;
                result.deleter = storage->deleterHash().value(uid, nullptr);
                result.ownerFunction = storage->ownerFunctionHash().value(uid, nullptr);
                result.destructor = storage->destructorHash().value(uid, nullptr);
                result.functionalResolver = storage->functionalResolverHash().value(uid, nullptr);
                result.typeInfoSupplier = storage->typeInfoSupplierHash().value(uid, nullptr);
                result.polymorphicIdHandlers = storage->polymorphicIDs().values(uid);
                if(result.entryType==EntryTypes::QObjectTypeInfo){
                    result.customMetaObject = storage->customMetaObject().value(uid, nullptr);
                    result.originalMetaObject = storage->originalMetaObjectHash().value(uid, nullptr);
                }
                if(storage->offsetHash().contains(uid)){
                    const QMap<size_t, int>& values = storage->offsetHash()[uid];
                    for(QMap<size_t, int>::const_key_value_iterator i = values.keyValueBegin(); i!=values.keyValueEnd(); ++i){
                        result.interfaceOffsetInfo.interfaces.insert(i.base().key());
                        if(i.base().value()>0)
                            result.interfaceOffsetInfo.offsets.insert(i.base().key(), uint(i.base().value()));
                    }
                }
                if(storage->interfacesHash().contains(uid)){
                    result.interfaceOffsetInfo.inheritedInterfaces[uid] = storage->interfacesHash()[uid];
                }
                break;
            case EntryTypes::StringTypeInfo:
            case EntryTypes::PrimitiveTypeInfo:
            case EntryTypes::SpecialTypeInfo:
                result.valueSizeAndAlignment = storage->valueSizeHash().value(uid, {0,0});
            default:
                break;
            }
        }
    }
    return result;
}

void RegistryAPI::registerMetaObject(const std::type_info& typeId, const QMetaObject& originalMetaObject, bool isValueOwner,
                        QMetaMethodRenamerFn methodRenamer)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto uniq = unique_id(typeId);
        QWriteLocker locker(storage->registryLock());
        storage->originalMetaObjectHash().insert(uniq, &originalMetaObject);
        storage->typeIdsByMetaObject().insert(&originalMetaObject, &typeId);
        if(methodRenamer)
            storage->metaMethodRenamerFnByMetaObject().insert(&originalMetaObject, methodRenamer);
        if(isValueOwner)
            storage->valueOwners().insert(&originalMetaObject);
    }
}

void RegistryAPI::registerMetaObject(const std::type_info& typeId, const QMetaObject& metaObject, bool isValueOwner,
                        std::initializer_list<SignalMetaInfo> signalMetaInfos,
                        ParameterInfoProviderFn parameterTypeInfoProvider,
                        QMetaMethodRenamerFn methodRenamer)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto uniq = unique_id(typeId);
        QWriteLocker locker(storage->registryLock());
        storage->originalMetaObjectHash().insert(uniq, &metaObject);
        storage->typeIdsByMetaObject().insert(&metaObject, &typeId);
        if(!storage->signalMetaInfos().contains(&metaObject))
            storage->signalMetaInfos().insert(&metaObject, signalMetaInfos);
        if(parameterTypeInfoProvider && !storage->parameterTypeInfoProviders().contains(&metaObject))
            storage->parameterTypeInfoProviders().insert(&metaObject, parameterTypeInfoProvider);
        if(methodRenamer)
            storage->metaMethodRenamerFnByMetaObject().insert(&metaObject, methodRenamer);
        if(isValueOwner)
            storage->valueOwners().insert(&metaObject);
    }
}

bool isValueOwner(const QMetaObject* metaObject){
    bool result = false;
    QtJambiStorage* storage = getQtJambiStorage();
    {
        {
            QReadLocker locker(storage->registryLock());
            if(storage->valueOwnerHash().contains(metaObject)){
                return storage->valueOwnerHash().value(metaObject);
            }
        }
        QtJambiStorage::ValueOwnerSet valueOwners;
        {
            QReadLocker locker(storage->registryLock());
            valueOwners = storage->valueOwners();
        }
        for(const QMetaObject* mo : qAsConst(valueOwners)){
            if(metaObject->inherits(mo)){
                result = true;
                break;
            }
        }
        {
            QWriteLocker locker(storage->registryLock());
            storage->valueOwnerHash().insert(metaObject, result);
        }
    }
    return result;
}

const std::type_info* getTypeByMetaObject(const QMetaObject* metaObject)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->typeIdsByMetaObject().value(metaObject, nullptr);
    }
    return nullptr;
}



const QMetaObject* registeredOriginalMetaObject(const std::type_info& typeId){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto uniq = unique_id(typeId);
        QReadLocker locker(storage->registryLock());
        return storage->originalMetaObjectHash().value(uniq, nullptr);
    }
    return nullptr;
}

const QVector<const RegistryAPI::SignalMetaInfo>* signalMetaInfos(const QMetaObject* metaObject)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        if(storage->signalMetaInfos().contains(metaObject)){
            return reinterpret_cast<const QVector<const RegistryAPI::SignalMetaInfo>*>(&storage->signalMetaInfos()[metaObject]);
        }
    }
    return nullptr;
}

QList<QMetaMethod> getExtraSignalsOfMetaObject(const QMetaObject* metaObject){
    QList<QMetaMethod> extraSignals;
    QtJambiStorage* storage = getQtJambiStorage();
    {
        {
            QReadLocker lock(storage->registryLock());
            if(storage->extraSignalsOfMetaObject().contains(metaObject))
                return storage->extraSignalsOfMetaObject().value(metaObject);
        }
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
            QWriteLocker lock(storage->registryLock());
            if(storage->extraSignalsOfMetaObject().contains(metaObject))
                return storage->extraSignalsOfMetaObject().value(metaObject);
            storage->extraSignalsOfMetaObject().insert(metaObject, extraSignals);
        }
    }
    return extraSignals;
}

RegistryAPI::ParameterInfoProviderFn registeredParameterInfoProviderFn(const QMetaObject* metaObject)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        if(storage->parameterTypeInfoProviders().contains(metaObject)){
            return storage->parameterTypeInfoProviders()[metaObject];
        }
    }
    return nullptr;
}

const QHash<int,const char*>* CoreAPI::renamedMethods(const QMetaObject* metaObject){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        if(storage->metaMethodRenamerFnByMetaObject().contains(metaObject)){
            if(!storage->renamedMethodsHash().contains(metaObject)){
                RegistryAPI::QMetaMethodRenamerFn registeredMetaMethodRenamer = storage->metaMethodRenamerFnByMetaObject()[metaObject];
                QHash<int,const char*> renamedMethods;
                for(int i=metaObject->methodOffset(); i<metaObject->methodCount(); ++i){
                    if(const char* newName = registeredMetaMethodRenamer(i)){
                        renamedMethods[i] = newName;
                    }
                }
                storage->renamedMethodsHash().insert(metaObject, renamedMethods);
            }
            return &storage->renamedMethodsHash()[metaObject];
        }
    }
    return nullptr;
}

RegistryAPI::QMetaMethodRenamerFn registeredMetaMethodRenamer(const QMetaObject* metaObject)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        if(storage->metaMethodRenamerFnByMetaObject().contains(metaObject)){
            return storage->metaMethodRenamerFnByMetaObject()[metaObject];
        }
    }
    return nullptr;
}

void RegistryAPI::registerTypeInfoSupplier(const std::type_info& typeId, TypeInfoSupplier resolver){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto uniq = unique_id(typeId);
        QWriteLocker locker(storage->registryLock());
        storage->typeInfoSupplierHash().insert(uniq, resolver);
    }
}

void RegistryAPI::registerFunctionalResolver(const std::type_info& typeId, FunctionalResolver resolver)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto uniq = unique_id(typeId);
        QWriteLocker locker(storage->registryLock());
        storage->functionalResolverHash().insert(uniq, resolver);
    }
}

FunctionalResolver registeredFunctionalResolver(const std::type_info& typeId){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto uniq = unique_id(typeId);
        QReadLocker locker(storage->registryLock());
        return storage->functionalResolverHash().value(uniq, nullptr);
    }
    return nullptr;
}

void registerJavaClassForCustomMetaType(JNIEnv *env, const QMetaType& metaType, jclass javaClass, bool isJObjectWrapped){
    int metaTypeId = metaType.id();
    const QtPrivate::QMetaTypeInterface * iface = META_TYPE_ACCESS(metaType).iface();
    QByteArray javaName = QtJambiAPI::getClassNameJNI(env, javaClass);
    if(Java::Runtime::Class::isSynthetic(env,javaClass)){
        javaClass = JavaAPI::resolveClass(env, javaName);
    }else{
        javaClass = getGlobalClassRef(env, javaClass, javaName);
    }
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->registryLock());
        storage->javaTypeByCustomMetaTypes().insert(iface, javaClass);
        storage->metaTypesByJavaTypeNames().insert(javaName, iface);
        if(isJObjectWrapped)
            storage->jobjectWrappedMetaTypes().insert(metaTypeId);
    }
}

bool isJObjectWrappedMetaType(const QMetaType& metaType){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        int metaTypeId = metaType.id();
        QReadLocker locker(storage->registryLock());
        return storage->jobjectWrappedMetaTypes().contains(metaTypeId);
    }
    return false;
}

bool isNativeWrapperMetaType(const QMetaType& metaType){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        auto iface = META_TYPE_ACCESS(metaType).iface();
        QReadLocker locker(storage->registryLock());
        return storage->metaTypesForJObjectNativeWrappers().contains(iface);
    }
    return false;
}

jclass registeredJavaClassForCustomMetaType(const QtPrivate::QMetaTypeInterface * metaType){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->javaTypeByCustomMetaTypes().value(metaType);
    }
    return nullptr;
}

jclass registeredJavaClassForCustomMetaType(QMetaType metaType){
    return registeredJavaClassForCustomMetaType(metaType.iface());
}

const char* registeredClassNameForFunctionalMetaType(const QMetaType& metaType){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        int metaTypeId = metaType.id();
        QReadLocker locker(storage->registryLock());
        return storage->javaNameByMetaTypes().value(metaTypeId);
    }
    return nullptr;
}

const QMetaObject *findWrappersMetaObject(const QtPrivate::QMetaTypeInterface *iface){
    const QMetaObject* meta_object = nullptr;
    QtJambiStorage* storage = getQtJambiStorage();
    {
        if(JniEnvironment env{200}){
            try {
                jclass clazz = nullptr;
                {
                    QReadLocker locker(storage->registryLock());
                    clazz = storage->javaTypeByCustomMetaTypes()[iface];
                }
                if(clazz){
                    meta_object = metaObjectForClass(env, clazz);
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
    }
    return meta_object;
}

QList<const QtPrivate::QMetaTypeInterface *> registeredCustomMetaTypesForJavaClass(QByteArrayView javaClassName){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        return storage->metaTypesByJavaTypeNames().values(javaClassName);
    }
    return {};
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
        QByteArray className;
        const QList<jclass> hirarchy = getFlatClassHirarchy(env, javaType);
        for(jclass sc : hirarchy){
            className = QtJambiAPI::getClassNameJNI(env, sc);
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
    QtJambiStorage* storage = getQtJambiStorage();
    {
        if(env && iid){
            const std::type_info* typeId(nullptr);
            {
                QReadLocker locker(storage->registryLock());
                typeId = storage->iidTypeHash().value(iid, nullptr);
            }
            if(typeId){
                if(const char* javaName = getJavaName(*typeId)){
                    return JavaAPI::resolveClass(env, javaName);
                }
            }else{
                const char* javaName(nullptr);
                {
                    QReadLocker locker(storage->registryLock());
                    javaName = storage->iidByteArrayHash()[iid];
                }
                if(javaName){
                    return JavaAPI::resolveClass(env, javaName);
                }
            }
        }
    }
    return nullptr;
}

/*******************************************************************************
 * Class Cache
 */
jclass findClass(JNIEnv *env, QByteArrayView qualifiedName, const char *className, jobject classLoader = nullptr)
{
    if (env->ExceptionCheck()) {
        // It is not allowed to call env->FindClass() with an exception pending, this is illegal JNI
        //  usage and must be the result of a software design error.
        qCWarning(DebugAPI::internalCategory) << "findClass(\"" << qualifiedName << "\") with Exception pending";
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        Q_ASSERT(false);
        return nullptr;
    }

    if(qualifiedName=="byte"){
        return Java::Runtime::Byte::primitiveType(env);
    }else if(qualifiedName=="long"){
        return Java::Runtime::Long::primitiveType(env);
    }else if(qualifiedName=="int"){
        return Java::Runtime::Integer::primitiveType(env);
    }else if(qualifiedName=="short"){
        return Java::Runtime::Short::primitiveType(env);
    }else if(qualifiedName=="float"){
        return Java::Runtime::Float::primitiveType(env);
    }else if(qualifiedName=="double"){
        return Java::Runtime::Double::primitiveType(env);
    }else if(qualifiedName=="boolean"){
        return Java::Runtime::Boolean::primitiveType(env);
    }else if(qualifiedName=="char"){
        return Java::Runtime::Character::primitiveType(env);
    }else if(qualifiedName=="void"){
        return Java::Runtime::Void::primitiveType(env);
    }else{
        jclass returned = env->FindClass(className ? className : qualifiedName.toByteArray().constData());
        if (returned == nullptr) {
            jthrowable exception = env->ExceptionOccurred();
            env->ExceptionClear();

            QByteArray qtClassName;
            if(className)
                qtClassName = className;
            else
                qtClassName = qualifiedName.toByteArray();
            qtClassName = qtClassName.replace('/', '.');
            jstring className = env->NewStringUTF(qtClassName);
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
}

jclass getGlobalClassRef(JNIEnv *env, jclass cls, QByteArrayView className){
    QtJambiStorage* storage = getQtJambiStorage();
#ifndef QTJAMBI_NOCACHE
    size_t key;
#ifdef QTJAMBI_LOG_CLASSNAMES
    QByteArray classNameStrg;
#endif
    if(!className.isEmpty()){
#ifdef QTJAMBI_LOG_CLASSNAMES
        classNameStrg = className;
#endif
        key = qHash(className);
    }else{
#ifndef QTJAMBI_LOG_CLASSNAMES
        QByteArray
#endif
            classNameStrg = QtJambiAPI::getClassNameJNI(env, cls);
        key = qHash(QByteArrayView(classNameStrg));
    }
    {
        QReadLocker locker(storage->registryLock());
        if(storage->classHash().contains(key))
            return storage->classHash().value(key);
    }
    bool deleteGlobalRef = false;
    if(env->GetObjectRefType(cls)!=JNIGlobalRefType){
        cls = jclass(env->NewGlobalRef(cls));
        deleteGlobalRef = true;
    }
    QWriteLocker locker(storage->registryLock());
    if(storage->classHash().contains(key)) { // if it has been inserted meanwhile by other thread
        if(deleteGlobalRef)
            env->DeleteGlobalRef(cls);
        return storage->classHash().value(key);
    }
#ifdef QTJAMBI_LOG_CLASSNAMES
    storage->classNameHash().insert(key, classNameStrg);
#endif
    storage->classHash().insert(key, cls);
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
    QByteArray className;
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
        className = "[L"+QtJambiAPI::getClassNameJNI(env, cls)+";";
    }
    size_t key = qHash(QByteArrayView(className));
    jclass cachedClass(nullptr);
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        if(storage->classHash().contains(key)){
            cachedClass = storage->classHash().value(key);
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
        cls = getGlobalClassRef(env, cls, className);
        return getArrayClass(env, cls, arrayDepth-1);
    }else {
        return getArrayClass(env, cachedClass, arrayDepth-1);
    }
}

jclass resolveClass(JNIEnv *env, QByteArrayView className, const char *classNamePtr, jobject classLoader)
{
    jclass returned = nullptr;
#ifndef QTJAMBI_NOCACHE
    size_t key = qHash(className);

    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->registryLock());
        returned = storage->classHash().value(key, nullptr);
    }

    if (returned == nullptr) {
#endif // QTJAMBI_NOCACHE
        if(!className.isEmpty() && !className.contains("$Lambda$") && !className.contains("$$Lambda/")){
            returned = findClass(env, className, classNamePtr, classLoader);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            returned = getGlobalClassRef(env, returned, className);
#ifndef QTJAMBI_NOCACHE
        }
#endif // QTJAMBI_NOCACHE
    }

    return returned;
}

jclass resolveClass(JNIEnv *env, const char *className, jobject classLoader){
    return ::resolveClass(env, QByteArrayView(className), className, classLoader);
}

jclass resolveClass(JNIEnv *env, QByteArrayView className, jobject classLoader){
    return ::resolveClass(env, className, nullptr, classLoader);
}

jclass resolveClass(JNIEnv *env, const QByteArray& className, jobject classLoader){
    return ::resolveClass(env, className, className, classLoader);
}

jclass JavaAPI::resolveClass(JNIEnv *env, const char *className, jobject classLoader)
{
    return ::resolveClass(env, QByteArrayView(className), className, classLoader);
}

size_t computeHash(JNIEnv* env, jclass clazz, const char *memberName, const char *signature, bool isStatic) {
    if(signature)
        return qHashMulti(0, clazz ? Java::Runtime::Object::hashCode(env, clazz) : 0, memberName, signature, isStatic);
    else
        return qHashMulti(0, clazz ? Java::Runtime::Object::hashCode(env, clazz) : 0, memberName, isStatic);
}

jfieldID resolveField(JNIEnv *env, const char *fieldName, const char *signature,
                      jclass clazz, bool isStatic, jthrowable* exceptionOccurred)
{
    if(!clazz)
        return nullptr;
    jfieldID returned = nullptr;
    QtJambiStorage* storage = getQtJambiStorage();
    {
#ifndef QTJAMBI_NOCACHE
        size_t key = computeHash(env,clazz,fieldName,nullptr,isStatic);
        {
            QReadLocker locker(storage->registryLock());
            jfieldID returned = storage->fieldHash().value(key, nullptr);
            if(returned)
                return returned;
        }
#endif // QTJAMBI_NOCACHE
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
#ifdef QTJAMBI_COUNTCACHEMISSES
            cacheMisses(fieldName);
#endif
            QWriteLocker locker(storage->registryLock());
            if (returned != nullptr && !storage->fieldHash().contains(key)) {
                storage->fieldHash().insert(key, returned);
            }
        }
#endif // QTJAMBI_NOCACHE
    }
    return returned;
}

/*******************************************************************************
 * Method Cache
 */

jmethodID JavaAPI::resolveMethod(JNIEnv *env, const char *methodName, const char *signature, jclass clazz, bool isStatic, jthrowable* exceptionOccurred)
{
    jmethodID returned = nullptr;
    QtJambiStorage* storage = getQtJambiStorage();
    {
        if(clazz){
#ifndef QTJAMBI_NOCACHE
            size_t key = computeHash(env,clazz,methodName,signature,isStatic);
            {
                QReadLocker locker(storage->registryLock());
                returned = storage->methodHash().value(key, nullptr);
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
#ifdef QTJAMBI_COUNTCACHEMISSES
            cacheMisses(methodName);
#endif
            QWriteLocker locker(storage->registryLock());
            if (!storage->methodHash().contains(key)) {
                storage->methodHash().insert(key, returned);
            }
#endif // QTJAMBI_NOCACHE
        }
    }
    return returned;
}

jmethodID findInternalPrivateConstructor(JNIEnv *env, jclass clazz){
    jmethodID constructor{nullptr};
    jobject declaredConstructor = Java::QtJambi::ClassAnalyzerUtility::findInternalPrivateConstructor(env, clazz);
#ifdef Q_OS_ANDROID
    if(declaredConstructor && !env->IsSameObject(clazz, Java::Runtime::Constructor::getDeclaringClass(env, declaredConstructor))){
        declaredConstructor = nullptr;
    }
#endif
    if(declaredConstructor){
        constructor = env->FromReflectedMethod(declaredConstructor);
    }
    return constructor;
}

jclass resolveLambdaInterface(JNIEnv* env, jclass lambdaClass){
    jobjectArray interfaces = Java::Runtime::Class::getInterfaces(env, lambdaClass);
    Q_ASSERT(interfaces && env->GetArrayLength(interfaces)>0);
    return jclass( env->GetObjectArrayElement(interfaces, 0) );
}

jclass JavaAPI::resolveClosestQtSuperclass(JNIEnv *env, jclass clazz, jobject classLoader)
{
    jclass returned = nullptr;
    QtJambiStorage* storage = getQtJambiStorage();
    {
        if(!env->IsSameObject(clazz, nullptr)){
            uint key = uint(Java::Runtime::Object::hashCode(env,clazz));
            {
                QReadLocker locker(storage->registryLock());
                jclass returned = storage->qtSuperclassHash().value(key, nullptr);
                if(returned)
                    return returned;
            }

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
                    QWriteLocker locker(storage->registryLock());
                    if (!storage->qtSuperclassHash().contains(key)) {
#ifdef QTJAMBI_COUNTCACHEMISSES
                        locker.unlock();
                        QString className = QtJambiAPI::getClassNameJNI(env, returned);
                        cacheMisses(qPrintable(className));
                        locker.relock();
#endif
                        storage->qtSuperclassHash().insert(key, returned);
                    }else{
                        returned = storage->qtSuperclassHash().value(key, nullptr);
                    }
                }
                if (!returned) {
                    QWriteLocker locker(storage->registryLock());
                    if (!storage->qtSuperclassHash().contains(key)) {
#ifdef QTJAMBI_COUNTCACHEMISSES
                        locker.unlock();
                        QString className = QtJambiAPI::getClassNameJNI(env, returned);
                        cacheMisses(qPrintable(className));
                        locker.relock();
#endif
                        storage->qtSuperclassHash().insert(key, nullptr);
                    }else{
                        returned = storage->qtSuperclassHash().value(key, nullptr);
                    }
                }
            }
        }
    }
    return returned;
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

void QtJambiPrivate::registerGlobalClassPointer(jclass& cls){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->registryLock());
        storage->globalClassPointers().append(reinterpret_cast<void**>(&cls));
    }
}

void unregisterGlobalClassPointer(jclass& cls){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->registryLock());
        storage->globalClassPointers().removeAll(reinterpret_cast<void**>(&cls));
    }
}

bool getFunctions(JNIEnv *env, const QMetaType& elementType, QtJambiUtils::QHashFunction& hashFunction, QtJambiUtils::InternalToExternalConverter& internalToExternalConverter, QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter);

QMetaType QmlAPI::registerMetaType(JNIEnv *env, SequentialContainerType containerType, const QMetaType& elementType){
    QMetaType result;
    auto containerAcess = createContainerAccess(containerType, elementType);
    AbstractListAccess* listAccess{nullptr};
    if(containerAcess && containerAcess->isList()){
        listAccess = static_cast<AbstractListAccess*>(containerAcess);
    }else{
        QtJambiUtils::QHashFunction hashFunction;
        QtJambiUtils::InternalToExternalConverter internalToExternalConverter;
        QtJambiUtils::ExternalToInternalConverter externalToInternalConverter;
        if(!getFunctions(env, elementType, hashFunction, internalToExternalConverter, externalToInternalConverter)){
            int elementTypeId = elementType.id();
            const std::type_info* typeId;
            QtJambiStorage* storage = getQtJambiStorage();
            {
                QReadLocker locker(storage->registryLock());
                typeId = storage->typeInfoByMetaTypes().value(elementTypeId);
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
        auto containerAccess = createContainerAccess(
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
        );
        if(containerAccess && containerAccess->isList())
            listAccess = static_cast<AbstractListAccess*>(containerAccess);
    }
    if(listAccess){
        QByteArray containerName;
        switch(containerType){
        case SequentialContainerType::QSet:
            containerName = "QSet<%1>";
            break;
        case SequentialContainerType::QQueue:
            containerName = "QQueue<%1>";
            break;
        case SequentialContainerType::QStack:
            containerName = "QStack<%1>";
            break;
        default:
            containerName = "QList<%1>";
            break;
        }

        result = listAccess->registerContainer(containerName.replace("%1", elementType.name()));
        listAccess->dispose();
    }
    return result;
}

template<typename INT, int isQtEnum0OrFlag1 = 0>
QMetaType qtjambi_register_enum_meta_type(JNIEnv *env, jclass clazz, QByteArrayView javaClassName, QByteArray&& javaTypeName){
    enum E:INT{};
    typedef std::conditional_t<isQtEnum0OrFlag1==1,QFlags<E>,E> EnumOrFlags;
    typedef JObjectWrapper Wrapper;
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
            /*.name=*/ getPersistentByteArray(std::move(javaTypeName)),
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
    QtJambiStorage* storage = getQtJambiStorage();
    {
        const QtPrivate::QMetaTypeInterface * iface{nullptr};
        {
            QReadLocker locker(storage->registryLock());
            iface = storage->metaTypesByJavaTypeNames().value(javaClassName);
        }
        if(iface){
            delete metaTypeInterface;
            return QMetaType(iface);
        }else{
            jclass cls = getGlobalClassRef(env, clazz, javaClassName);
            QWriteLocker locker(storage->registryLock());
            iface = storage->metaTypesByJavaTypeNames().value(javaClassName);
            if(iface){
                delete metaTypeInterface;
                return QMetaType(iface);
            }
            storage->metaTypesByJavaTypeNames().insert(javaClassName.toByteArray(), metaTypeInterface);
            storage->metaTypeEnumClasses()[metaTypeInterface] = cls;
        }
    }
    QMetaType metaType(metaTypeInterface);
    auto conv = [metaTypeInterface](const void *src, void *target) -> bool {
        if(src){
            jclass clazz{nullptr};
            QtJambiStorage* storage = getQtJambiStorage();
            {
                QReadLocker locker(storage->registryLock());
                clazz = storage->metaTypeEnumClasses()[metaTypeInterface];
            }
            if(clazz){
                if(JniEnvironment env{200}){
                    try{
                        if constexpr(isQtEnum0OrFlag1==0){
                            new (target)Wrapper(CoreAPI::convertEnumToJavaObject(env, *reinterpret_cast<const INT*>(src), clazz));
                        }else if constexpr(isQtEnum0OrFlag1==1){
                            std::conditional_t<sizeof(EnumOrFlags)==sizeof(jlong), jlong, jint> value = *reinterpret_cast<const INT*>(src);
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
    return metaType;
}

QMetaType registerJObjectValueWrapper(JNIEnv *env, jclass clazz, QByteArrayView javaClassName, const QByteArray& metaTypeName);

QMetaType registerMetaType(JNIEnv *env, jclass clazz, jboolean isPointer, jboolean isReference, const QMetaType& superMetaType){
    using namespace RegistryAPI;
    try{
        QMetaType metaType = CoreAPI::registeredMetaType(env, clazz);
        if(metaType.isValid())
            return metaType;
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
            return QMetaType(QMetaType::Int);
        }else if((Java::Runtime::Long::isSameClass(env, clazz) || Java::Runtime::Long::isPrimitiveType(env, clazz)) && !isPointer){
            return QMetaType(QMetaType::LongLong);
        }else if((Java::Runtime::Short::isSameClass(env, clazz) || Java::Runtime::Short::isPrimitiveType(env, clazz)) && !isPointer){
            return QMetaType(QMetaType::Short);
        }else if((Java::Runtime::Byte::isSameClass(env, clazz) || Java::Runtime::Byte::isPrimitiveType(env, clazz)) && !isPointer){
            return QMetaType(QMetaType::SChar);
        }else if((Java::Runtime::Boolean::isSameClass(env, clazz) || Java::Runtime::Boolean::isPrimitiveType(env, clazz)) && !isPointer){
            return QMetaType(QMetaType::Bool);
        }else if((Java::Runtime::Character::isSameClass(env, clazz) || Java::Runtime::Character::isPrimitiveType(env, clazz)) && !isPointer){
            return QMetaType(QMetaType::QChar);
        }else if((Java::Runtime::Float::isSameClass(env, clazz) || Java::Runtime::Float::isPrimitiveType(env, clazz)) && !isPointer){
            return QMetaType(QMetaType::Float);
        }else if((Java::Runtime::Double::isSameClass(env, clazz) || Java::Runtime::Double::isPrimitiveType(env, clazz)) && !isPointer){
            return QMetaType(QMetaType::Double);
        }else if((Java::Runtime::Void::isPrimitiveType(env, clazz)) && !isPointer){
            return QMetaType(QMetaType::Void);
        }else{
            QByteArray javaClassName = QtJambiAPI::getClassNameJNI(env, clazz);
            QtJambiStorage* storage = getQtJambiStorage();
            {
                QReadLocker locker(storage->registryLock());
                if(const QtPrivate::QMetaTypeInterface * iface = storage->metaTypesByJavaTypeNames().value(javaClassName)){
                    return QMetaType(iface);
                }
            }
            if(const std::type_info* typeId = getTypeByJavaName(javaClassName)){
                const RegisteredTypeInfo typeInfo = registeredTypeInfo(*typeId, nullptr);
                QByteArray qtType = typeInfo.qtName;
                if((!isPointer || qtType.endsWith("*")) && (!isReference || !qtType.endsWith("*"))){
                    if(typeInfo.metaType.isValid())
                        return typeInfo.metaType;
                }
                const QMetaObject *const original_meta_object = typeInfo.originalMetaObject;
                QMetaType::TypeFlags flags;
                QByteArray typeName = QMetaObject::normalizedType(qtType);
                EntryTypes entryType = typeInfo.entryType;
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
                case EntryTypes::FunctionPointerTypeInfo: {
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                    flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<void*>::Flags);
#else
                    flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<void*>::flags());
#endif
                    flags |= QMetaType::IsPointer;
                    if(entryType!=EntryTypes::FunctionPointerTypeInfo && !typeName.endsWith("*")){
                        typeName = QMetaObject::normalizedType(typeName + "*");
                    }
                    if(isQObject){
                        flags |= QMetaType::PointerToQObject;
                    }else if(original_meta_object){
                        flags |= QMetaType::IsGadget;
                        flags |= QMetaType::PointerToGadget;
                    }
                    metaType = createMetaType(typeName,
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
                    return metaType;
                }
                case EntryTypes::EnumTypeInfo:{
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                    flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<Qt::Orientation>::Flags);
#else
                    flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<Qt::Orientation>::flags());
#endif
                    if(Java::QtJambi::QtShortEnumerator::isAssignableFrom(env, clazz)){
                        metaType = createMetaType(typeName,
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
                        metaType.id();
                        registerJavaClassForCustomMetaType(env, metaType, clazz);
                        return metaType;
                    }else if(Java::QtJambi::QtLongEnumerator::isAssignableFrom(env, clazz)){
                        metaType = createMetaType(typeName,
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
                        metaType.id();
                        registerJavaClassForCustomMetaType(env, metaType, clazz);
                        return metaType;
                    }else if(Java::QtJambi::QtByteEnumerator::isAssignableFrom(env, clazz)){
                        metaType = createMetaType(typeName,
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
                        metaType.id();
                        registerJavaClassForCustomMetaType(env, metaType, clazz);
                        return metaType;
                    }else /*if(Java::Runtime::Enum::isAssignableFrom(env, clazz)
                         || Java::QtJambi::QtEnumerator::isAssignableFrom(env, clazz))*/ {
                         enum E:qint32{};
                         metaType = createMetaType(typeName,
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
                         metaType.id();
                         registerJavaClassForCustomMetaType(env, metaType, clazz);
                         return metaType;
                    }
                }
                case EntryTypes::FlagsTypeInfo:{
                    enum E:qint32{};
                    metaType = createMetaType(typeName,
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
                    metaType.id();
                    registerJavaClassForCustomMetaType(env, metaType, clazz);
                    return metaType;
                }
                case EntryTypes::StdFunctionTypeInfo:
                case EntryTypes::InterfaceValueTypeInfo:
                case EntryTypes::ValueTypeInfo:{
                    metaType = QMetaType::fromType<JObjectWrapper>();
                    QMetaType::registerNormalizedTypedef(typeName, metaType);
                    return metaType;
                }
                default: break;
                }
            }else{
                const SuperTypeInfos superTypes = SuperTypeInfos::fromClass(env, clazz);
                QByteArray metaTypeName = javaClassName;
                metaTypeName.replace("/", "::").replace("$", "::");
                if(superTypes.isEmpty())
                {
                    if(Java::Runtime::Enum::isAssignableFrom(env, clazz)){
                        if(Java::QtJambi::QtShortEnumerator::isAssignableFrom(env, clazz)){
                            return qtjambi_register_enum_meta_type<qint16>(env, clazz, javaClassName, std::move(metaTypeName));
                        }else if(Java::QtJambi::QtLongEnumerator::isAssignableFrom(env, clazz)){
                            return qtjambi_register_enum_meta_type<qint64>(env, clazz, javaClassName, std::move(metaTypeName));
                        }else if(Java::QtJambi::QtByteEnumerator::isAssignableFrom(env, clazz)){
                            return qtjambi_register_enum_meta_type<qint8>(env, clazz, javaClassName, std::move(metaTypeName));
                        }else if(Java::QtJambi::QtEnumerator::isAssignableFrom(env, clazz)){
                            return qtjambi_register_enum_meta_type<qint32>(env, clazz, javaClassName, std::move(metaTypeName));
                        }else{
                            return qtjambi_register_enum_meta_type<qint32,-1>(env, clazz, javaClassName, std::move(metaTypeName));
                        }
                    }else if(Java::QtJambi::QFlags::isAssignableFrom(env, clazz) && !Java::QtJambi::QFlags::isSameClass(env, clazz)){
                        return qtjambi_register_enum_meta_type<qint32,1>(env, clazz, javaClassName, std::move(metaTypeName));
                    }else{
                        metaType = registerJObjectValueWrapper(env, clazz, javaClassName, metaTypeName);
                        if(metaType.isValid()){
                            return metaType;
                        }
                        if(Java::Runtime::Collection::isAssignableFrom(env, clazz)){
                            return QMetaType::fromType<JCollectionWrapper>();
                        }else if(Java::Runtime::Map::isAssignableFrom(env, clazz)){
                            return QMetaType::fromType<JMapWrapper>();
                        }
                        metaTypeName = "JObjectWrapper<" + metaTypeName + ">";
                        metaType = createMetaType(metaTypeName,
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
                        registerConverterVariant(env, metaType, metaTypeName, javaClassName, clazz);
                        return metaType;
                    }
                }else{
                    QMetaType _superMetaType;
                    if(superMetaType.isValid()){
                        jclass cls = CoreAPI::getClassForMetaType(env, superMetaType);
                        if(env->IsSameObject(cls, superTypes[0].javaClass())){
                            _superMetaType = superMetaType;
                        }
                    }
                    if(!_superMetaType.isValid())
                        _superMetaType = registerMetaType(env, superTypes[0].javaClass(), isPointer, isReference);
                    if(_superMetaType.isValid()){
                        QMetaType superMetaType(_superMetaType);
                        QSharedPointer<AbstractContainerAccess> access = findContainerAccess(_superMetaType);
                        if(superMetaType.flags() & QMetaType::IsPointer){
                            metaTypeName = metaTypeName + "*";
                            metaType = createMetaType(metaTypeName,
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
                            registerJavaClassForCustomMetaType(env, metaType, clazz);
                            registerConverterVariant(env, metaType, metaTypeName, javaClassName, clazz);
                            return metaType;
                        }else if(access){
                            metaTypeName = metaTypeName + "*";
                            metaType = createMetaType(metaTypeName,
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
                            metaType.id();
                            registerJavaClassForCustomMetaType(env, metaType, clazz);
                            registerConverterVariant(env, metaType, metaTypeName, javaClassName, clazz);
                            return metaType;
                        }else{
                            metaType = registerJObjectValueWrapper(env, clazz, javaClassName, metaTypeName);
                            if(metaType.isValid()){
                                return metaType;
                            }
                            if(Java::Runtime::Collection::isAssignableFrom(env, clazz)){
                                return QMetaType::fromType<JCollectionWrapper>();
                            }else if(Java::Runtime::Map::isAssignableFrom(env, clazz)){
                                return QMetaType::fromType<JMapWrapper>();
                            }
                            metaTypeName = "JObjectWrapper<" + metaTypeName + ">";
                            metaType = createMetaType(metaTypeName,
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
                            registerConverterVariant(env, metaType, metaTypeName, javaClassName, clazz);
                            return metaType;
                        }
                    }else{
                        if(!Java::QtCore::AbstractContainer::isAssignableFrom(env, clazz)){
                            if(Java::Runtime::Collection::isAssignableFrom(env, clazz)){
                                return QMetaType::fromType<JCollectionWrapper>();
                            }else if(Java::Runtime::Map::isAssignableFrom(env, clazz)){
                                return QMetaType::fromType<JMapWrapper>();
                            }
                        }
                        metaTypeName = "JObjectWrapper<" + metaTypeName + ">";
                        metaType = createMetaType(metaTypeName,
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
                        registerConverterVariant(env, metaType, metaTypeName, javaClassName, clazz);
                        return metaType;
                    }
                }
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return QMetaType();
}

QMetaType QmlAPI::registerMetaType(JNIEnv *env, jclass clazz){
    return ::registerMetaType(env, clazz);
}
