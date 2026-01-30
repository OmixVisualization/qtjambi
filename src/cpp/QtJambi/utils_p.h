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

#if !defined(QTJAMBI_UTILS_P_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_UTILS_P_H

#include <QtCore/QtGlobal>
#include <QtCore/QLoggingCategory>
#include <QtCore/QReadWriteLock>
#include <QtCore/QMutex>
#include "qtjambiapi.h"
#include "qmlapi.h"
#include "containeraccess.h"
#include "supertypeinfo_p.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
#define META_TYPE_ACCESS(m) const_cast<QMetaType&>(m)
#else
#define META_TYPE_ACCESS(m) m
#endif

struct VariantUtility{
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
    static QVariant createVariant(QMetaType type, void *data);
#else
#endif
    static QVariant createVariant(const QMetaType& type, const void *data);
};

#if defined(Q_OS_ANDROID) || defined(Q_OS_FREEBSD)
#define unique_id(id) qHash(QLatin1String((id).name()))
#define typeid_equals(t1, t2) unique_id(t1)==unique_id(t2)
#define typeid_not_equals(t1, t2) unique_id(t1)!=unique_id(t2)
#else
#define unique_id(id) (id).hash_code()
#define typeid_equals(t1, t2) t1==t2
#define typeid_not_equals(t1, t2) t1!=t2
#endif

void registerContainerAccess(QMetaType metaType, AbstractContainerAccess* access);
void registerContainerAccess(QMetaType metaType, const QSharedPointer<AbstractContainerAccess>& sharedAccess);
bool hasRegisteredContainerAccess(QMetaType metaType);
QSharedPointer<AbstractContainerAccess> findContainerAccess(const QMetaType& metaType);

QString getFunctionLibraryPath(QFunctionPointer function);

bool isValidArray(JNIEnv *env, jobject object, jclass contentType);

jobject resolveLongEnum(JNIEnv *env, jint hashCode, jclass enumClass, jlong value, jstring entryName);
jobject resolveByteEnum(JNIEnv *env, jint hashCode, jclass enumClass, jbyte value, jstring entryName);
jobject resolveShortEnum(JNIEnv *env, jint hashCode, jclass enumClass, jshort value, jstring entryName);
jobject resolveIntEnum(JNIEnv *env, jint hashCode, jclass enumClass, jint value, jstring entryName);
void registerSwitchTableFields(JNIEnv *env, jobject switchTableFields);

bool simpleEventNotify(void **data);
bool threadAffineEventNotify(void **data);
void enableThreadAffinity(bool enabled);
void enableSignalEmitThreadCheck(bool enabled);

bool enabledDanglingPointerCheck(JNIEnv * env = nullptr);
bool compareMetaTypes(const QMetaType& typeA, const QMetaType& typeB);
void checkThreadOnQObject(JNIEnv *env, jobject object);
void checkThreadOnQObject(JNIEnv *env, PtrOwnerFunction owner_function, const void *qt_object, jclass cls);

void containerDisposer(AbstractContainerAccess* _access);

const std::type_info* tryGetTypeInfo(TypeInfoSupplier typeInfoSupplier, const void* ptr);
const std::type_info* checkedGetTypeInfo(TypeInfoSupplier typeInfoSupplier, const void* ptr);

enum class NativeToJavaConversionMode{
    None,
    MakeCopyOfValues,
    TransferOwnership,
    CppOwnership
};

jobject convertSmartPointerToJavaObject(JNIEnv *env, const char *className,
                                                const QSharedPointer<char>& ptr_shared_pointer);
jobject convertSmartPointerToJavaInterface(JNIEnv *env, const std::type_info& interfaceType,
                                                const QSharedPointer<char>& ptr_shared_pointer);
jobject convertSmartPointerToJavaObject(JNIEnv *env, const char *className,
                                                 const std::shared_ptr<char>& ptr_shared_pointer);
jobject convertSmartPointerToJavaInterface(JNIEnv *env, const std::type_info& interfaceType,
                                                    const std::shared_ptr<char>& ptr_shared_pointer);

class QtJambiLink;

namespace DebugAPI{

struct PrinterPrivate;

class Printer{
    typedef void(*Deleter)(void*);
    typedef void(*Invoker)(void*,QDebug&);
public:
    typedef void(*FunctionPointer)(QDebug&);

private:
    explicit Printer(void* data, Invoker invoker, Deleter deleter) noexcept;
public:
    Printer() noexcept;
    ~Printer() noexcept;
    Printer(const Printer& other) noexcept;
    Printer(Printer&& other) noexcept;
    Printer(FunctionPointer functor) noexcept;
    inline Printer(std::nullptr_t) noexcept : Printer(FunctionPointer(nullptr)) {}

    Printer& operator=(const Printer& other) noexcept;
    Printer& operator=(Printer&& other) noexcept;

    template<typename Functor, typename std::enable_if<!std::is_pointer<Functor>::value, bool>::type = true
             , typename std::enable_if<!std::is_same<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type, Printer>::value, bool>::type = true
             , typename std::enable_if<!std::is_null_pointer<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type>::value, bool>::type = true
             , typename std::enable_if<!std::is_same<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type, FunctionPointer>::value, bool>::type = true
             , typename std::enable_if<std::is_invocable<Functor,QDebug&>::value, bool>::type = true
             >
    Printer(Functor&& functor) noexcept
        : Printer(
            new typename std::remove_reference<typename std::remove_cv<Functor>::type>::type(std::move(functor)),
            [](void* data, QDebug& d){
                typename std::remove_reference<typename std::remove_cv<Functor>::type>::type* fct = reinterpret_cast<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type*>(data);
                (*fct)(d);
            },
            [](void* data){
                delete reinterpret_cast<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type*>(data);
            }
            ){}
    bool operator==(const Printer& other) const noexcept;
    void operator()(QDebug&) const;
    operator bool() const noexcept;
    bool operator !() const noexcept;
private:
    friend PrinterPrivate;
    QExplicitlySharedDataPointer<PrinterPrivate> d;
};

Q_DECLARE_LOGGING_CATEGORY(internalCategory)
Q_DECLARE_LOGGING_CATEGORY(debugAPIJavaOverloadsCategory)
Q_DECLARE_LOGGING_CATEGORY(debugAPIEventsCategory)
Q_DECLARE_LOGGING_CATEGORY(debugAPICleanupCallsCategory)
Q_DECLARE_LOGGING_CATEGORY(debugAPIInternalMethodsCategory)

class MethodPrintFromLink : public MethodPrint
{
public:
    MethodPrintFromLink(const QSharedPointer<QtJambiLink>& link, const char* method, const char* file, int line, const char *function);
    MethodPrintFromLink(const QWeakPointer<QtJambiLink>& link, const char* method, const char* file, int line, const char *function);
    MethodPrintFromLink(const QtJambiLink* link, const char* method, const char* file, int line, const char *function);
};

class MethodPrintFromArgs : public MethodPrint
{
public:
    MethodPrintFromArgs(MethodPrint::Type callType, const char* file, int line, const char *function, const char* method, ...);
    MethodPrintFromArgs(const QLoggingCategory& category, const char* file, int line, const char *function, const char* method, ...);
};

class MethodPrintFromSupplier : public MethodPrint
{
public:
    MethodPrintFromSupplier(MethodPrint::Type callType, const char* file, int line, const char *function, Printer&& supplier);
    MethodPrintFromSupplier(const QLoggingCategory& category, const char* file, int line, const char *function, Printer&& supplier);
};

class EventPrint : public MethodPrint
{
public:
    EventPrint(const char* file, int line, const char *function, QObject *receiver, QEvent *event);
};

void printArgs(const char *file, int line, const char *function, const char *format,...);
void print(const char *file, int line, const char *function, Printer&& printer);

}

#ifdef QTJAMBI_NO_METHOD_TRACE
#define QTJAMBI_DEBUG_PRINT_ARGS(...)
#define QTJAMBI_DEBUG_INTERNAL_PRINT(printer)
#define QTJAMBI_INTERNAL_METHOD_CALL_PRINT(printer)
#else
#define QTJAMBI_DEBUG_PRINT_ARGS(...) DebugAPI::printArgs(__FILE__, __LINE__, Q_FUNC_INFO, __VA_ARGS__);
#define QTJAMBI_DEBUG_INTERNAL_PRINT(printer) DebugAPI::print(__FILE__, __LINE__, Q_FUNC_INFO, printer);
#define QTJAMBI_INTERNAL_METHOD_CALL_PRINT(printer) DebugAPI::MethodPrintFromSupplier __debug_method_print(DebugAPI::debugAPIInternalMethodsCategory(), __FILE__, __LINE__, Q_FUNC_INFO, printer);
#endif


class QtJambiExceptionUnraiser{
public:
    QtJambiExceptionUnraiser();
    ~QtJambiExceptionUnraiser();
private:
    quint8 data;
    Q_DISABLE_COPY_MOVE(QtJambiExceptionUnraiser)
};

void reinitializeResettableFlag(JNIEnv * env, const char* property);
bool avoidJNIGlobalRefs(JNIEnv * env = nullptr);

struct ResettableBoolFlag{
    ResettableBoolFlag(const char* property);
    ResettableBoolFlag(JNIEnv * env, const char* property);
    ~ResettableBoolFlag();
    operator bool();
private:
    qint8 value : 1;
    qint8 registered : 1;
    void reinitialize(JNIEnv * env, const char* property);
#ifdef Q_OS_ANDROID
    friend void reinitializeResettableFlags(JNIEnv * env);
#endif
    friend void registerResettableFlag(ResettableBoolFlag* flag, const char* property);
    friend void unregisterResettableFlag(ResettableBoolFlag* flag);
    friend void reinitializeResettableFlag(JNIEnv * env, const char* property);
    friend void clearQtJambiStorage(JNIEnv* env, bool regular);
};

namespace QtJambiPrivate{
static constexpr int CACHE_HASH = 0xCAFEBABE;
}

struct EnumSwitchTable{
    jfieldID field;
    jclass clazz;
    bool isSwitchTable;
};

class ParameterTypeInfo{
public:
    ParameterTypeInfo();
    ParameterTypeInfo(
        const QMetaType& metaType,
        jclass _javaClass
        );
    ParameterTypeInfo(
        const QMetaType& metaType,
        const QString& typeName,
        jclass _javaClass
        );
    ParameterTypeInfo(
        const QMetaType& metaType,
        jclass _javaClass,
        QtJambiUtils::InternalToExternalConverter&& internalToExternalConverter,
        QtJambiUtils::ExternalToInternalConverter&& externalToInternalConverter
        );
    ParameterTypeInfo(
        const QMetaType& metaType,
        const QString& typeName,
        jclass _javaClass,
        QtJambiUtils::InternalToExternalConverter&& internalToExternalConverter,
        QtJambiUtils::ExternalToInternalConverter&& externalToInternalConverter
        );
    ParameterTypeInfo(const ParameterTypeInfo& other);
    ParameterTypeInfo& operator=(const ParameterTypeInfo& other);
    ParameterTypeInfo(ParameterTypeInfo&& other);
    ParameterTypeInfo& operator=(ParameterTypeInfo&& other);

    bool convertInternalToExternal(JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& out, bool forceBoxedType) const;
    bool convertExternalToInternal(JNIEnv* env, QtJambiScope* scope, jvalue in,void* & out, jValueType valueType) const;
    jclass javaClass() const;
    const QMetaType& metaType() const;
    static QtJambiUtils::InternalToExternalConverter default_internalToExternalConverter();
    static QtJambiUtils::ExternalToInternalConverter default_externalToInternalConverter();
    static ParameterTypeInfo voidTypeInfo(JNIEnv* env);
private:
    void resolveI2E(JNIEnv* env);
    void resolveE2I(JNIEnv* env);
    QMetaType m_metaType;
    QString m_typeName;
    jclass m_javaClass;
    QtJambiUtils::InternalToExternalConverter m_internalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_externalToInternalConverter;
    uint m_resolvedI2E : 1;
    uint m_resolvedE2I : 1;
    friend class QtJambiMetaObject;
};

class VTable;

typedef QExplicitlySharedDataPointer<const class QtJambiTypeEntry> QtJambiTypeEntryPtr;

enum class EntryTypes;

struct JObjectValueWrapperPrivate;

class JObjectValueWrapper : public JObjectWrapper{
public:
    JObjectValueWrapper();
    JObjectValueWrapper(const JObjectValueWrapper& other);
    JObjectValueWrapper(JObjectValueWrapper&& other);
    ~JObjectValueWrapper();
    void assign(const JObjectWrapper &wrapper);
    void assign(JObjectWrapper &&wrapper);
    JObjectValueWrapper& operator=(const JObjectValueWrapper &wrapper);
    JObjectValueWrapper& operator=(JObjectValueWrapper &&wrapper);
    void assign(const JObjectValueWrapper &wrapper);
    void assign(JObjectValueWrapper &&wrapper);
    void assign(JNIEnv* env, jobject obj);
    bool operator==(const JObjectValueWrapper &other) const;
    bool operator<(const JObjectValueWrapper &other) const;
    void writeTo(QDataStream &d)const;
    void readFrom(QDataStream &d);
    void serializeTo(QDataStream &d)const;
    void serializeFrom(QDataStream &d);
    bool isNull() const;
    jobject value(JNIEnv* env) const;
    static bool isValueType(QMetaType metaType);
    static bool hasCustomDebugStreamOperator(QMetaType metaType);
    static JObjectValueWrapper create(JNIEnv* env, jobject object, QMetaType metaType);
private:
    friend JObjectValueWrapperPrivate;
    JObjectValueWrapper(QExplicitlySharedDataPointer<JObjectValueWrapperPrivate> methods);
    JObjectValueWrapper(JNIEnv* env, jobject object, QExplicitlySharedDataPointer<JObjectValueWrapperPrivate> methods);
    static const QMetaObject *metaObject(const QtPrivate::QMetaTypeInterface *iface);
    static void defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *target);
    static void copyCtr(const QtPrivate::QMetaTypeInterface *, void *target, const void *other);
    static void moveCtr(const QtPrivate::QMetaTypeInterface *, void *target, void *other);
    static void dtor(const QtPrivate::QMetaTypeInterface *, void *target);
    static bool equals(const QtPrivate::QMetaTypeInterface *iface, const void *value1, const void *value2);
    static bool lessThan(const QtPrivate::QMetaTypeInterface *iface, const void *value1, const void *value2);
    static void debugStream(const QtPrivate::QMetaTypeInterface *iface, QDebug &d, const void *value);
    static void dataStreamOut(const QtPrivate::QMetaTypeInterface *iface, QDataStream &d, const void *value);
    static void dataStreamIn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &d, void *value);
    static void serializableDataStreamOut(const QtPrivate::QMetaTypeInterface *iface, QDataStream &d, const void *value);
    static void serializableDataStreamIn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &d, void *value);
    QExplicitlySharedDataPointer<JObjectValueWrapperPrivate> p;
};

struct PolymorphicIdHandler;

struct InterfaceOffsetInfo{
    QMap<size_t,uint> offsets;
    QSet<size_t> interfaces;
    QMap<size_t, QSet<const std::type_info*>> inheritedInterfaces;
};

typedef void (*FinalizationExecutor)(JNIEnv* env, void* data);
typedef void (*FinalizationDeleter)(JNIEnv* env, void* data);

#define CACHE_MEMBER(Type,variable)\
private: Type m_##variable;\
public: Type& variable();

struct QtJambiStorage{
    typedef QMap<const QtPrivate::QMetaTypeInterface *, QtMetaContainerPrivate::QMetaAssociationInterface> MetaAssociationByMetaTypeHash;
    typedef QMap<const QtPrivate::QMetaTypeInterface *, QtMetaContainerPrivate::QMetaSequenceInterface> MetaSequenceByMetaTypeHash;
    typedef QHash<size_t, QtJambiUtils::InternalToExternalConverter> InternalToExternalConverterHash;
    typedef QHash<size_t, QtJambiUtils::ExternalToInternalConverter> ExternalToInternalConverterHash;
    typedef QMap<const QtPrivate::QMetaTypeInterface *, QtJambiUtils::QHashFunction> HashFunctionByMetaTypeHash;
    typedef QMap<const QtPrivate::QMetaTypeInterface *, QMetaEnum> MetaEnumHash;
    typedef QHash<quintptr, QList<JObjectWrapper>> ObjectsByFunctionPointerHash;
    typedef QMap<size_t, const QtPrivate::QBindableInterface *> BindableInterfacesHash;
    typedef QMap<const void*, QMetaType> MetaTypesByPointerHash;
    typedef QHash<size_t,QExplicitlySharedDataPointer<QmlAPI::CreatorFunctionMetaData>> CreatorFunctionMetaDataHash;
    typedef QHash<jint,jfieldID> EnumClassValuesHash;
    typedef QHash<jint,bool> EnumClassExtensibleHash;
    typedef QHash<QString,QList<EnumSwitchTable>> SwitchTableFieldHash;
    typedef QMap<size_t, QMap<QByteArray,QtJambiTypeEntryPtr>> TypeEntryHash;
    typedef QHash<jint,SuperTypeInfos> SuperTypeInfoHash;
    typedef QHash<int, QSharedPointer<const VTable>> VTableHash;
    typedef QHash<size_t,const QMetaObject *> MetaObjectByNameHash;
    typedef QHash<size_t, QVector<ParameterTypeInfo>> ParameterTypeHash;
    typedef QHash<int, const QMetaObject *> MetaObjectByMetaTypeHash;
    typedef QHash<size_t, JObjectWrapper> SignalTypesHash;
    typedef QHash<ResettableBoolFlag*,const char*> ResettableBoolFlagHash;
    typedef QSet<QtMsgType> SupportedMessageTypes;
    typedef QMultiHash<QSharedPointer<QtJambiLink>, QWeakPointer<QtJambiLink>> DependencyHash;
    typedef QHash<jint,void*> FinalizationDataHash;
    typedef QHash<QSharedPointer<QtJambiLink>, QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>>> FinalizationHash;
    typedef QMultiHash<const void *, QWeakPointer<QtJambiLink>> UserObjectHash;
    typedef QHash<const void *, QWeakPointer<QtJambiLink>> QObjectHash;
    typedef QMap<size_t, EntryTypes> TypeEntryTypesHash;
    typedef QMap<size_t, const char*> TypeStringHash;
    typedef QHash<QByteArray, const char*> StringStringHash;
    typedef QHash<QByteArray, const std::type_info*> StrintypeHash;
    typedef QHash<QByteArray, QList<const std::type_info*>> StrinbTypesHash;
    typedef QMap<QByteArray, const QMetaObject*> NamespaceMetaObjectHash;
    typedef QSet<size_t> TypeSet;
    typedef QHash<QByteArray, QByteArray> IIDByteArrayHash;
    typedef QMap<size_t, const std::type_info*> ID2IDHash;
    typedef QMap<size_t, NewContainerAccessFunction> ContainerAccessFactoryHash;
    typedef QHash<int, QHash<SequentialContainerType,std::function<AbstractContainerAccess*()>>> SequentialContainerAccessFactoryByMetaTypeHash;
    typedef QHash<QPair<int,int>, QHash<AssociativeContainerType,std::function<AbstractContainerAccess*()>>> AssociativeContainerAccessFactoryByMetaTypeHash;
    typedef QHash<int, const std::type_info*> TypeInfoByMetaTypeHash;
    typedef QMap<size_t, const QtPrivate::QMetaTypeInterface *> MetaTypeByTypeInfoMap;
    typedef QMap<size_t, const void*> DefaultValueHash;
    typedef QMap<size_t, QVector<RegistryAPI::FunctionInfo>> FunctionInfoHash;
    typedef QMap<size_t, QVector<RegistryAPI::ConstructorInfo>> ConstructorInfoHash;
    typedef QMap<size_t, uint> ReturnScopeHash;
    typedef QMap<size_t, RegistryAPI::DestructorFn> DestructorHash;
    typedef QHash<size_t, const char*> NameHash;
    typedef QSet<size_t> HashSet;
    typedef QMap<size_t, PtrDeleterFunction> DeleterHash;
    typedef QMap<int,QSharedPointer<AbstractContainerAccess>> ContainerAccessMap;
    typedef QHash<const QtPrivate::QMetaTypeInterface *, jclass> JavaTypeByMetaTypeHash;
    typedef QHash<int, const char*> JavaNameByMetaTypeHash;
    typedef QMultiHash<QByteArray,const QtPrivate::QMetaTypeInterface *> MetaTypesByJavaTypeNameHash;
    typedef QSet<int> JObjectWrappedMetaTypeHash;
    typedef QHash<int, const QtPrivate::QMetaTypeInterface *> JObjectNativeWrappedMetaTypeHash;
    typedef QHash<const QtPrivate::QMetaTypeInterface *, const QtPrivate::QMetaTypeInterface *> JObjectNativeWrappedMetaTypeReverseHash;
    typedef QMap<size_t, PtrOwnerFunction> OwnerFunctionHash;
    typedef QMultiHash<size_t, QExplicitlySharedDataPointer<const PolymorphicIdHandler>> PolymorphicIdHash;
    typedef QMultiMap<size_t, const std::type_info*> PolymorphicBasesHash;
    typedef QMap<size_t, RegistryAPI::qHashFn> HashFunctionHash;
    typedef QMap<size_t, QPair<size_t,size_t>> SizeHash;
    typedef QMap<size_t, QMap<size_t, int>> OffsetHash;
    typedef QMap<size_t, QSet<const std::type_info*>> InterfacesHash;
    typedef QHash<uint, InterfaceOffsetInfo > InterfaceOffsetHash;
    typedef QMap<size_t, const QMetaObject*> MetaObjectHash;
    typedef QHash<const QMetaObject*, QVector<RegistryAPI::SignalMetaInfo> > SignalMetaInfoHash;
    typedef QHash<const QMetaObject*, QList<QMetaMethod> > ExtraSignalsOfMetaObjectHash;
    typedef QHash<const QMetaObject*, const std::type_info* > TypeIdsByMetaObjectHash;
    typedef QHash<const QMetaObject*, RegistryAPI::QMetaMethodRenamerFn> QMetaMethodRenamerFnByMetaObjectHash;
    typedef QSet<const QMetaObject*> ValueOwnerSet;
    typedef QHash<const QMetaObject*,bool> ValueOwnerHash;
    typedef QHash<const QMetaObject*, RegistryAPI::ParameterInfoProviderFn > ParameterTypeInfoProviderHash;
    typedef QHash<const QMetaObject*, QHash<int,const char*>> RenamedMethodsHash;
    typedef QMap<size_t, FunctionalResolver> FunctionalResolverHash;
    typedef QMap<size_t, TypeInfoSupplier> TypeInfoSupplierHash;
    typedef QHash<size_t,jmethodID> ConstructorIDHash;
    typedef QHash<size_t, jclass> ClassIdHash;
    typedef QHash<size_t, jfieldID> FieldIdHash;
    typedef QHash<size_t, jmethodID> MethodIdHash;
    typedef QHash<const QtPrivate::QMetaTypeInterface*,const QMetaObject*> MetaTypeMetaObjectHash;
    typedef QSet<QByteArray> TypeNameSet;
    typedef QMap<size_t, const QtPrivate::QMetaTypeInterface *> ClassMetaTypeInterfaceHash;
    typedef QMap<const QtPrivate::QMetaTypeInterface *,QExplicitlySharedDataPointer<JObjectValueWrapperPrivate>> MetaTypeInterfacesHash;
    typedef QMap<const QtPrivate::QMetaTypeInterface *,jclass> MetaTypeEnumClassesHash;
    typedef QList<void**> GlobalClassPointers;

    CACHE_MEMBER(BindableInterfacesHash, bindableInterfacesHash);
    CACHE_MEMBER(MetaTypesByPointerHash, metaTypesByPointer);
    CACHE_MEMBER(CreatorFunctionMetaDataHash, creatorFunctionMetaDataHash);
    CACHE_MEMBER(EnumClassValuesHash, enumClassValuesFields);
    CACHE_MEMBER(EnumClassExtensibleHash, enumClassExtensible);
    CACHE_MEMBER(SwitchTableFieldHash, switchTableFields);
    CACHE_MEMBER(ObjectsByFunctionPointerHash, objectsByFunctionPointers);
    CACHE_MEMBER(InternalToExternalConverterHash, internalToExternalConverters);
    CACHE_MEMBER(ExternalToInternalConverterHash, externalToInternalConverters);
    CACHE_MEMBER(HashFunctionByMetaTypeHash, hashFunctionByMetaTypeHash);
    CACHE_MEMBER(MetaEnumHash, metaEnumByMetaTypeHash);
    CACHE_MEMBER(TypeEntryHash, typeEntries);
    CACHE_MEMBER(SuperTypeInfoHash, superTypeInfos);
    CACHE_MEMBER(VTableHash, virtualTables);
    CACHE_MEMBER(MetaObjectByNameHash, metaObjectsByName);
    CACHE_MEMBER(ParameterTypeHash, parameterTypeInfos);
    CACHE_MEMBER(MetaObjectByMetaTypeHash, metaObjects);
    CACHE_MEMBER(SignalTypesHash, signalTypes);
    CACHE_MEMBER(ResettableBoolFlagHash, resettableBoolFlags);
    CACHE_MEMBER(SupportedMessageTypes, supportedMessageTypes);
    CACHE_MEMBER(JObjectWrapper, signalEmitThreadCheckHandler);
    CACHE_MEMBER(MetaAssociationByMetaTypeHash, metaAssociationsByMetaType);
    CACHE_MEMBER(MetaSequenceByMetaTypeHash, metaSequencesByMetaType);
    CACHE_MEMBER(DependencyHash, dependencies);
    CACHE_MEMBER(DependencyHash, conDestroyedDependencies);
    CACHE_MEMBER(FinalizationDataHash, finalizationData);
    CACHE_MEMBER(FinalizationHash, finalizations);
    CACHE_MEMBER(UserObjectHash, userObjectCache);
    CACHE_MEMBER(QObjectHash, qObjectCache);
    CACHE_MEMBER(TypeStringHash, typeQtNameHash)
    CACHE_MEMBER(TypeEntryTypesHash, entryTypesHash)
    CACHE_MEMBER(StrintypeHash, qtNameTypeHash)
    CACHE_MEMBER(StringStringHash, qtFunctionalJavaNameHash)
    CACHE_MEMBER(StringStringHash, qtFunctionalQtNameHash)
    CACHE_MEMBER(StringStringHash, qtNamespaceJavaNameHash)
    CACHE_MEMBER(StringStringHash, javaNamespaceQtNameHash)
    CACHE_MEMBER(NamespaceMetaObjectHash, namespaceMetaObjectHash)
    CACHE_MEMBER(TypeStringHash, typeJavaNameHash)
    CACHE_MEMBER(StrinbTypesHash, javaNameTypeHash)
    CACHE_MEMBER(StrintypeHash, iidTypeHash)
    CACHE_MEMBER(StringStringHash, javaClassIIDHash)
    CACHE_MEMBER(IIDByteArrayHash, iidByteArrayHash)
    CACHE_MEMBER(ID2IDHash, flagEnumIDHash)
    CACHE_MEMBER(ID2IDHash, enumFlagIDHash)
    CACHE_MEMBER(TypeStringHash, typeJavaInterfaceHash)
    CACHE_MEMBER(ContainerAccessFactoryHash, containerAccessFactories)
    CACHE_MEMBER(SequentialContainerAccessFactoryByMetaTypeHash, sequentialContainerAccessFactoryByMetaTypes)
    CACHE_MEMBER(AssociativeContainerAccessFactoryByMetaTypeHash, associativeContainerAccessFactoryByMetaTypes)
    CACHE_MEMBER(MetaTypeByTypeInfoMap, metaTypeByTypeInfos)
    CACHE_MEMBER(TypeInfoByMetaTypeHash, typeInfoByMetaTypes)
    CACHE_MEMBER(DefaultValueHash, defaultValueHash)
    CACHE_MEMBER(FunctionInfoHash, virtualFunctionInfos)
    CACHE_MEMBER(ConstructorInfoHash, constructorInfos)
    CACHE_MEMBER(ReturnScopeHash, returnScopes)
    CACHE_MEMBER(DestructorHash, destructorHash)
    CACHE_MEMBER(NameHash, flagEnumNameHash)
    CACHE_MEMBER(NameHash, interfaceHash)
    CACHE_MEMBER(NameHash, interfaceIIDsHash)
    CACHE_MEMBER(HashSet, functionalHash)
    CACHE_MEMBER(DeleterHash, deleterHash)
    CACHE_MEMBER(ContainerAccessMap, containerAccessMap)
    CACHE_MEMBER(JObjectNativeWrappedMetaTypeHash, jobjectNativeWrapperMetaTypes)
    CACHE_MEMBER(JObjectNativeWrappedMetaTypeReverseHash, metaTypesForJObjectNativeWrappers)
    CACHE_MEMBER(JavaTypeByMetaTypeHash, javaTypeByCustomMetaTypes)
    CACHE_MEMBER(JavaNameByMetaTypeHash, javaNameByMetaTypes)
    CACHE_MEMBER(MetaTypesByJavaTypeNameHash, metaTypesByJavaTypeNames)
    CACHE_MEMBER(JObjectWrappedMetaTypeHash, jobjectWrappedMetaTypes)
    CACHE_MEMBER(OwnerFunctionHash, ownerFunctionHash)
    CACHE_MEMBER(PolymorphicIdHash, polymorphicIDs)
    CACHE_MEMBER(PolymorphicBasesHash, polymorphicBasesHash)
    CACHE_MEMBER(SizeHash, valueSizeHash)
    CACHE_MEMBER(SizeHash, shellSizeHash)
    CACHE_MEMBER(HashFunctionHash, hashFunctionHash)
    CACHE_MEMBER(OffsetHash, offsetHash)
    CACHE_MEMBER(InterfacesHash, interfacesHash)
    CACHE_MEMBER(InterfaceOffsetHash, interfaceOffsetHash)
    CACHE_MEMBER(MetaObjectHash, originalMetaObjectHash)
    CACHE_MEMBER(MetaObjectHash, customMetaObject)
    CACHE_MEMBER(SignalMetaInfoHash, signalMetaInfos)
    CACHE_MEMBER(ExtraSignalsOfMetaObjectHash, extraSignalsOfMetaObject)
    CACHE_MEMBER(TypeIdsByMetaObjectHash, typeIdsByMetaObject)
    CACHE_MEMBER(QMetaMethodRenamerFnByMetaObjectHash, metaMethodRenamerFnByMetaObject)
    CACHE_MEMBER(ValueOwnerSet, valueOwners)
    CACHE_MEMBER(ValueOwnerHash, valueOwnerHash)
    CACHE_MEMBER(ParameterTypeInfoProviderHash, parameterTypeInfoProviders)
    CACHE_MEMBER(RenamedMethodsHash, renamedMethodsHash)
    CACHE_MEMBER(FunctionalResolverHash, functionalResolverHash)
    CACHE_MEMBER(TypeInfoSupplierHash, typeInfoSupplierHash)
    CACHE_MEMBER(TypeStringHash, mediaControlIIDHash)
    CACHE_MEMBER(StrintypeHash, mediaControlIIDClassHash)
    CACHE_MEMBER(ClassIdHash, classHash)
    CACHE_MEMBER(FieldIdHash, fieldHash)
    CACHE_MEMBER(MethodIdHash, methodHash)
    CACHE_MEMBER(ClassIdHash, qtSuperclassHash)
    CACHE_MEMBER(MetaTypeMetaObjectHash, metaTypeMetaObjectHash)
    CACHE_MEMBER(TypeNameSet, typeNames)
    CACHE_MEMBER(ClassMetaTypeInterfaceHash, classMetaTypeInterfaces)
    CACHE_MEMBER(MetaTypeInterfacesHash, metaTypeInterfaces)
    CACHE_MEMBER(MetaTypeEnumClassesHash, metaTypeEnumClasses)
    CACHE_MEMBER(GlobalClassPointers, globalClassPointers)

#if !defined(QTJAMBI_NO_GLOBALREFS)
    typedef QHash<const QMetaObject *, jweak> JavaMetaObjectHash;
    CACHE_MEMBER(JavaMetaObjectHash, javaMetaObjects);
#endif
#if defined(QTJAMBI_DEBUG_TOOLS)
    typedef QSet<QtJambiLink*> LinkSet;
    CACHE_MEMBER(LinkSet, linkSet);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
    typedef QMap<QByteArray,QPair<const char*, int>> NativeInterfaceMap;
    CACHE_MEMBER(NativeInterfaceMap, nativeInterfaceMap)
#endif
#ifdef QTJAMBI_LOG_CLASSNAMES
    typedef QHash<size_t, QString> ClassNameHash;
    CACHE_MEMBER(ClassNameHash, classNameHash)
#endif
#if defined(QTJAMBI_GENERIC_ACCESS)
    typedef QMap<size_t, ContainerAccessAPI::SequentialContainerAccessFactory> SequentialContainerAccessFactoryHash;
    typedef QMap<size_t, ContainerAccessAPI::AssociativeContainerAccessFactory> AssociativeContainerAccessFactoryHash;
    typedef QHash<size_t, QtMetaContainerPrivate::QMetaAssociationInterface> MetaAssociationHash;
    typedef QHash<size_t, QtMetaContainerPrivate::QMetaSequenceInterface> MetaSequenceHash;
    typedef QHash<const QtPrivate::QMetaTypeInterface*,ContainerTypeInfo> ContainerTypeInfoHash;
    typedef QHash<const QtPrivate::QMetaTypeInterface*,BiContainerTypeInfo> BiContainerTypeInfoHash;
    CACHE_MEMBER(MetaSequenceHash, metaSequenceHash);
    CACHE_MEMBER(MetaAssociationHash, metaAssociationHash);
    CACHE_MEMBER(ContainerTypeInfoHash, containerTypeInfos);
    CACHE_MEMBER(BiContainerTypeInfoHash, biContainerTypeInfos);
    CACHE_MEMBER(SequentialContainerAccessFactoryHash, sequentialContainerAccessFactoryHash);
    CACHE_MEMBER(AssociativeContainerAccessFactoryHash, associativeContainerAccessFactoryHash);
#endif
#if defined(ALLOW_SCOPED_POINTER_METATYPE)
    typedef QHash<const QtPrivate::QMetaTypeInterface *,const QtPrivate::QMetaTypeInterface *> ElementMetaTypesOfSmartPointersHash;
    typedef QHash<const QtPrivate::QMetaTypeInterface *,PtrDeleterFunction> ElementDeletersOfSmartPointersHash;
    CACHE_MEMBER(ElementMetaTypesOfSmartPointersHash, elementMetaTypesOfSmartPointers);
    CACHE_MEMBER(ElementDeletersOfSmartPointersHash, elementDeletersOfSmartPointers);
#endif

    QReadWriteLock* lock();
    QReadWriteLock* linkLock();
    QReadWriteLock* registryLock();
private:
    explicit QtJambiStorage(bool init = false);
    bool ref();
    void deref();
    void cleanup(GlobalClassPointers& globalClassPointers,
                 ClassIdHash& classHash,
                 JObjectWrapper& signalEmitThreadCheckHandler,
                 ResettableBoolFlagHash& resettableBoolFlags,
                 MetaObjectByMetaTypeHash& metaObjectsHash,
                 SignalTypesHash& signalTypes,
#ifdef QTJAMBI_LOG_CLASSNAMES
                 ClassNameHash& classNameHash,
#endif
#if !defined(QTJAMBI_NO_GLOBALREFS)
                 JavaMetaObjectHash javaMetaObjects,
#endif
                 ObjectsByFunctionPointerHash& objectsByFunctionPointers,
                 SuperTypeInfoHash& superTypeInfos);
    ~QtJambiStorage();
    int hash = QtJambiPrivate::CACHE_HASH;
    QAtomicInt c{1};
    QReadWriteLock* m_lock = nullptr;
    QReadWriteLock* m_linkLock = nullptr;
    QReadWriteLock* m_registryLock = nullptr;

    static QtJambiStorage* INSTANCE;

    template<typename Cache>
    friend struct CacheRef;
    template<typename Cache>
    friend struct CacheOptional;
    friend void clearQtJambiStorage(JNIEnv*, bool);
};

#undef CACHE_MEMBER

QtJambiStorage* getQtJambiStorage();

template<typename Cache>
struct CacheOptional{
    CacheOptional() : cache(Cache::INSTANCE) {
        if(Q_LIKELY(cache)){
            if(Q_UNLIKELY(!cache->ref()))
                cache = nullptr;
        }
    }
    ~CacheOptional() {
        if(Q_LIKELY(cache))
            cache->deref();
    }
    inline operator bool() const {return cache;}
    inline bool operator !() const {return !cache;}
    inline Cache* get() {return cache;}
    inline Cache* operator->() {return cache;}
    inline Cache& operator()() {return *cache;}
    inline Cache& operator*() {return *cache;}
private:
    Cache* cache;
};

template<typename Cache>
struct CacheRef{
    CacheRef() : cache(Cache::INSTANCE) {
        if(Q_LIKELY(cache)){
            if(Q_UNLIKELY(!cache->ref()))
                cache = new Cache;
        }else{
            cache = new Cache;
        }
    }
    ~CacheRef() {
        cache->deref();
    }
    inline Cache* get() {return cache;}
    inline Cache* operator->() {return cache;}
    inline Cache& operator()() {return *cache;}
    inline Cache& operator*() {return *cache;}
private:
    Cache* cache;
};

typedef CacheRef<QtJambiStorage> QtJambiStorageRef;
typedef CacheOptional<QtJambiStorage> QtJambiStorageOptional;

#endif // QTJAMBI_UTILS_P_H
