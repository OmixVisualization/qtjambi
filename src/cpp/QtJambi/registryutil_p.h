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

#ifndef REGISTRYUTIL_P_H
#define REGISTRYUTIL_P_H

#include <QtCore/QSet>
#include <QtCore/QMap>
#include <QtCore/QVector>
#include <typeindex>
#include "registryapi.h"
#include "jobjectwrapper.h"
#include "typeentry_p.h"
#include "utils_p.h"

class SuperTypeInfos;

#define QRecursiveMutexLocker QMutexLocker<QRecursiveMutex>
#define OptionalBool std::optional<bool>
jclass getGlobalClassRef(JNIEnv *env, jclass cls, QByteArrayView className = {});

size_t qHash(const std::type_index& idx, size_t seed = 0) Q_DECL_NOEXCEPT;
size_t qHash(const char *p, size_t seed = 0) Q_DECL_NOEXCEPT;
const QMetaObject* registeredOriginalMetaObject(const std::type_info& typeId);
PtrDeleterFunction deleter(const std::type_info& typeId);
const char* getInterface(const char*qt_name);
const char * getQtName(const std::type_info& typeId);
const char * getJavaName(const std::type_info& typeId);
bool isFunctional(const std::type_info& typeId);
bool isStdFunction(const std::type_info& typeId);
bool isFunctionPointer(const std::type_info& typeId);
bool isInterface(const std::type_info& typeId);
const std::type_info* getInterfaceTypeForIID(const char*interface_iid);
const std::type_info* getTypeByJavaName(const char * java_name);
const std::type_info* getTypeByJavaName(const QString& java_name);
bool isJavaNameNamespace(const QString& java_name);
bool isJavaNameNamespace(const QByteArray& java_name);
bool isQtNameNamespace(const QByteArray& qt_name);
const QMetaObject* registeredNamespaceMetaObject(const QString& java_name);
const std::type_info* getTypeByQtName(const char * qt_name);
const std::type_info* getTypeByQtName(const QString& qt_name);
const std::type_info* getTypeByQtName(const QByteArray& qt_name);
const std::type_info* getTypeByMetaObject(const QMetaObject* metaObject);
QMetaType registeredMetaType(const std::type_info& typeId);
int registeredInterfaceOffset(const std::type_info& qt_base, const std::type_info& qt_interface);
const QVector<const RegistryAPI::SignalMetaInfo>* signalMetaInfos(const QMetaObject* metaObject);
QList<QMetaMethod> getExtraSignalsOfMetaObject(const QMetaObject* metaObject);
RegistryAPI::ParameterInfoProviderFn registeredParameterInfoProviderFn(const QMetaObject* metaObject);
bool isValueOwner(const QMetaObject* metaObject);
const std::type_info* getTypeByMetaType(const QMetaType& metaType);
PtrOwnerFunction registeredOwnerFunction(const std::type_info& typeId);
NewContainerAccessFunction getContainerAccessFactory(const std::type_info& typeId);
bool isPolymorphicBase(const std::type_info& typeId);
QPair<size_t,size_t> getValueSizeAndAlignment(const std::type_info& typeId);
QPair<size_t,size_t> getShellSizeAndAlignment(const std::type_info& typeId);

enum class EntryTypes
{
    Unspecific = 0,
    ObjectTypeInfo,
    EnumTypeInfo,
    FlagsTypeInfo,
    StdFunctionTypeInfo,
    FunctionPointerTypeInfo,
    InterfaceValueTypeInfo,
    InterfaceTypeInfo,
    QObjectTypeInfo,
    ValueTypeInfo,
    SpecialTypeInfo,
    StringTypeInfo,
    PrimitiveTypeInfo
};
EntryTypes getEntryType(const std::type_info& typeId);

bool isInterface(const char*qt_interface);
bool hasCustomMetaObject(const std::type_info& typeId, const QMetaObject** superTypeMetaObject = nullptr);
const QMetaObject* customMetaObject(const std::type_info& typeId);
RegistryAPI::DestructorFn registeredDestructor(const std::type_info& typeId);
FunctionalResolver registeredFunctionalResolver(const std::type_info& typeId);
jmethodID findInternalPrivateConstructor(JNIEnv *env, jclass clazz);
QList<const std::type_info*> getPolymorphicBases(const std::type_info& typeId);

void registeredInterfaceOffsets(const std::type_info& qt_type, InterfaceOffsetInfo* info);
const InterfaceOffsetInfo* getInterfaceOffsets(JNIEnv *env, jclass clazz, const std::type_info& typeId, const SuperTypeInfos* superTypeInfos);
const InterfaceOffsetInfo* getInterfaceOffsets(JNIEnv *env, jclass clazz);

RegistryAPI::qHashFn registeredHashFunction(const std::type_info& typeId);
jfieldID resolveField(JNIEnv *env, const char *fieldName, const char *signature, jclass clazz, bool isStatic = false, jthrowable* exceptionOccurred = nullptr);
//jfieldID resolveField(JNIEnv *env, const char *fieldName, const char *signature, const char *className, bool isStatic = false, jthrowable* exceptionOccurred = nullptr);
void registerJavaClassForCustomMetaType(JNIEnv *env, const QMetaType& metaType, jclass javaClass, bool isJObjectWrapped = false);
bool isJObjectWrappedMetaType(const QMetaType& metaType);
QMetaType getNativeWrapperType(const QMetaType& metaType);
bool isNativeWrapperMetaType(const QMetaType& metaType);

QMetaType registerMetaType(JNIEnv *env, jclass clazz, jboolean isPointer, jboolean isReference, const QMetaType& superMetaType = {});
const QVector<const RegistryAPI::ConstructorInfo>* registeredConstructorInfos(const std::type_info& typeId);
uint returnScopes(const std::type_info& typeId);
jclass getArrayClass(JNIEnv *env, jclass cls, int arrayDepth);

void registerConverterVariant(JNIEnv *env, const QMetaType& metaType, QString qtName, const QString& fullJavaName, jclass clazz, const QMetaType& nativeWrapperType = {});

void registerLambdaClass(JNIEnv *env, jclass lambdaClass, const char *className);

const char * getJavaInterfaceName(const std::type_info& typeId);
const std::type_info* getTypeByJavaName(QByteArray javaName);
QMap<QString,QPair<size_t,size_t>> getRegisteredTypeSizesAndAlignments();
void registerTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name, EntryTypes entryTypes);
void registerTypeAlias(const std::type_info& typeId, const char *qt_name, const char *java_name);
void registerContainerTypeInfo(const std::type_info& typeId, const char *qt_name, const char *java_name, const char *java_interface);
void registerMetaType(const std::type_info& typeId,
                        const std::type_info& nonPointerTypeId,
                        const QMetaType& metaType,
                        AbstractContainerAccess* access = nullptr,
                        const QSharedPointer<AbstractContainerAccess>& sharedAccess = {});

struct PolymorphicIdHandler : QSharedData{
    PolymorphicIdHandler(const std::type_info& targetTypeId, PolymorphyHandler polymorphyHandler)
        : QSharedData(),
          m_targetTypeId(targetTypeId),
          m_polymorphyHandler(polymorphyHandler)
    {
    }
    const std::type_info& m_targetTypeId;
    PolymorphyHandler m_polymorphyHandler;
};
QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>> getPolymorphicIdHandlers(const std::type_info& polymorphicBaseTypeId);
const char * registeredInterfaceID(const std::type_info& typeId);
const char * registeredInterfaceIDForClassName(const QString& className);
bool isQObject(const std::type_info& typeId);
QList<jclass> getFlatClassHirarchy(JNIEnv *env, jclass clazz);

struct RegisteredTypeInfo{
    EntryTypes entryType = EntryTypes::Unspecific;
    QMetaType metaType;
    const char * qtName = nullptr;
    const char * javaName = nullptr;
    const char * javaInterfaceName = nullptr;
    const std::type_info* flagForEnum = nullptr;
    QPair<size_t,size_t> valueSizeAndAlignment;
    QPair<size_t,size_t> shellSizeAndAlignment;
    const std::type_info* enumForFlag = nullptr;
    QVector<RegistryAPI::FunctionInfo> virtualFunctions;
    RegistryAPI::DestructorFn destructor = nullptr;
    FunctionalResolver functionalResolver = nullptr;
    PtrDeleterFunction deleter = nullptr;
    PtrOwnerFunction ownerFunction = nullptr;
    TypeInfoSupplier typeInfoSupplier = nullptr;
    QList<QExplicitlySharedDataPointer<const PolymorphicIdHandler>> polymorphicIdHandlers;
    NewContainerAccessFunction containerAccessFactory = nullptr;
    const QMetaObject*  customMetaObject = nullptr;
    const QMetaObject*  originalMetaObject = nullptr;
    InterfaceOffsetInfo interfaceOffsetInfo;
};

RegisteredTypeInfo registeredTypeInfo(const std::type_info& typeId, const char* qtName);

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
                                        QtPrivate::QMetaTypeInterface::MetaObjectFn metaObjectFn);

QSharedPointer<AbstractContainerAccess> findContainerAccess(const QMetaType& metaType);

const char* registerMetaTypeName(const QByteArray& typeName);
const QMetaObject* metaobjectByMetaTypeInterface(const QtPrivate::QMetaTypeInterface* iface);
bool isQmlJavaScriptOwnership(QObject * obj);

struct JQObjectWrapperPrivate;

class JQObjectWrapper{
private:
    QObject* m_qobject;
    friend JQObjectWrapperPrivate;
    JQObjectWrapperPrivate* p;
public:
    JQObjectWrapper();
    JQObjectWrapper(JNIEnv* env, QSharedPointer<QtJambiLink>&& link);
    ~JQObjectWrapper();
    JQObjectWrapper(const JQObjectWrapper& other);
    JQObjectWrapper(JQObjectWrapper&& other);
    JQObjectWrapper& operator=(const JQObjectWrapper &wrapper);
    JQObjectWrapper& operator=(JQObjectWrapper &&wrapper);
    QSharedPointer<QtJambiLink> link() const;
    jobject javaObject(JNIEnv* env) const;
    inline QObject* qobject() const { return m_qobject; }
};

#endif // REGISTRYUTIL_P_H
