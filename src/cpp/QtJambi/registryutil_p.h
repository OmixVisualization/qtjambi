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

#ifndef REGISTRYUTIL_P_H
#define REGISTRYUTIL_P_H

#include <QtCore/QSet>
#include <QtCore/QMap>
#include <QtCore/QVector>
#include <typeindex>
#include "registryapi.h"
#include "jobjectwrapper.h"
#include "typeentry_p.h"
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include "jobjectwrapper.h"
#endif
#include "utils_p.h"

class SuperTypeInfos;

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#define QRecursiveMutexLocker QMutexLocker
#else
#define QRecursiveMutexLocker QMutexLocker<QRecursiveMutex>
#endif

struct InterfaceOffsetInfo{
    QMap<size_t,uint> offsets;
    QSet<size_t> interfaces;
    QMap<size_t, QSet<const std::type_info*>> inheritedInterfaces;
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
struct OptionalBool{
    inline OptionalBool() : m_hasValue(false), m_value(false) {}
    inline OptionalBool(bool value) : m_hasValue(true), m_value(value) {}
    inline operator bool() const {return m_hasValue;}
    inline bool value() const {return m_hasValue && m_value;}
    uint m_hasValue:1;
    uint m_value:1;
};
#else
#define OptionalBool std::optional<bool>
#endif

jclass getGlobalClassRef(JNIEnv *env, jclass cls, const char *className = nullptr);

hash_type qHash(const std::type_index& idx, hash_type seed = 0) Q_DECL_NOEXCEPT;
hash_type qHash(const char *p, hash_type seed = 0) Q_DECL_NOEXCEPT;
const QMetaObject* registeredOriginalMetaObject(const std::type_info& typeId);
PtrDeleterFunction deleter(const std::type_info& typeId);
PtrOwnerFunction registeredOwnerFunction(const std::type_info& typeId);
const char* getInterface(const char*qt_name);
const char * getQtName(const std::type_info& typeId);
const char * getJavaName(const std::type_info& typeId);
bool isFunctional(const std::type_info& typeId);
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
int registeredMetaTypeID(const std::type_info& typeId);
int registeredInterfaceOffset(const std::type_info& qt_base, const std::type_info& qt_interface);
const QVector<const SignalMetaInfo>* signalMetaInfos(const QMetaObject* metaObject);
QList<QMetaMethod> getExtraSignalsOfMetaObject(const QMetaObject* metaObject);
ParameterInfoProvider registeredParameterInfoProvider(const QMetaObject* metaObject);
bool isValueOwner(const QMetaObject* metaObject);
const std::type_info* getTypeByMetaType(int metaType);
const std::type_info* getTypeByMetaType(const QMetaType& metaType);
PtrOwnerFunction registeredOwnerFunction(const std::type_info& typeId);
NewContainerAccessFunction getSequentialContainerAccessFactory(const std::type_info& typeId);
bool isPolymorphicBase(const std::type_info& typeId);
size_t getValueSize(const std::type_info& typeId);
size_t getValueAlignment(const std::type_info& typeId);
size_t getShellSize(const std::type_info& typeId);
EntryTypes getEntryType(const std::type_info& typeId);
const std::type_info* getEnumForFlag(const std::type_info& flag);
const std::type_info* getFlagForEnum(const std::type_info& enumerator);
const QVector<const FunctionInfo>* virtualFunctions(const std::type_info& typeId);
bool isInterface(const char*qt_interface);
bool hasCustomMetaObject(const std::type_info& typeId, const QMetaObject** superTypeMetaObject = nullptr);
const QMetaObject* superTypeForCustomMetaObject(const std::type_info& typeId);
Destructor registeredDestructor(const std::type_info& typeId);
TypeInfoSupplier registeredTypeInfoSupplier(const std::type_info& typeId);
FunctionalResolver registeredFunctionalResolver(const std::type_info& typeId);
jmethodID findInternalPrivateConstructor(JNIEnv *env, jclass clazz);
QList<const std::type_info*> getPolymorphicBases(const std::type_info& typeId);
void registeredInterfaceOffsets(const std::type_info& qt_type, InterfaceOffsetInfo* info);
const InterfaceOffsetInfo* getInterfaceOffsets(JNIEnv *env, jclass clazz, const std::type_info& typeId, const SuperTypeInfos* superTypeInfos);
const InterfaceOffsetInfo* getInterfaceOffsets(JNIEnv *env, jclass clazz);
QHashFunctionPtr registeredHashFunction(const std::type_info& typeId);
OptionalBool isRegisteredAsPointerType(const std::type_info& typeId);
jfieldID resolveField(JNIEnv *env, const char *fieldName, const char *signature, jclass clazz, bool isStatic = false, jthrowable* exceptionOccurred = nullptr);
//jfieldID resolveField(JNIEnv *env, const char *fieldName, const char *signature, const char *className, bool isStatic = false, jthrowable* exceptionOccurred = nullptr);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void registerJavaClassForCustomMetaType(JNIEnv *env, const QMetaType& metaType, jclass javaClass, bool isJObjectWrapped = false);
void registerJavaClassForCustomMetaType(JNIEnv *env, int metaType, jclass javaClass, bool isJObjectWrapped);
bool isJObjectWrappedMetaType(const QMetaType& metaType);
OptionalBool isRegisteredAsStaticType(const std::type_info& typeId);
#else
void registerJavaClassForCustomMetaType(JNIEnv *env, QMetaType metaType, jclass javaClass, bool isJObjectWrapped = false);
bool isJObjectWrappedMetaType(QMetaType metaType);
#endif

int registerMetaType(JNIEnv *env, jclass clazz, jboolean isPointer, jboolean isReference);
const QVector<const ConstructorInfo>* registeredConstructorInfos(const std::type_info& typeId);
jclass getArrayClass(JNIEnv *env, jclass cls, int arrayDepth);

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void registerConverterVariant(JNIEnv *env, QMetaType metaType, QString qtName, const QString& fullJavaName, jclass clazz);
#endif

void registerLambdaClass(JNIEnv *env, jclass lambdaClass, const char *className);

const char * getJavaInterfaceName(const std::type_info& typeId);
const std::type_info* getTypeByJavaName(QByteArray javaName);
QMap<QString,QPair<size_t,size_t>> getRegisteredTypeSizesAndAlignments();
void registerTypeInfo(const std::type_info& typeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name, EntryTypes entryTypes);
void registerTypeAlias(const std::type_info& typeId, const char *qt_name, const char *java_name);
void registerContainerTypeInfo(const std::type_info& typeId, QtJambiTypeInfo info, const char *qt_name, const char *java_name, const char *java_interface);
void registerMetaTypeID(const std::type_info& typeId, const std::type_info& nonPointerTypeId, int qtMetaType);
QList<const PolymorphicIdHandler*> getPolymorphicIdHandlers(const std::type_info& polymorphicBaseTypeId);
const char * registeredInterfaceID(const std::type_info& typeId);
bool isQObject(const std::type_info& typeId);

#ifdef JOBJECT_REFCOUNT
#  include <QtCore/QReadWriteLock>
#  include <QtCore/QWriteLocker>
    Q_GLOBAL_STATIC(QReadWriteLock, gRefCountLock);

    static void jobjectRefCount(bool create)
    {
        QWriteLocker locker(gRefCountLock());

        static int refs = 0;
        QString s;
        if (!create) {
            s = QString("Deleting jobject reference: %1 references left").arg(--refs);
        } else {
            s = QString("Creating jobject reference: %1 references now").arg(++refs);
        }

        Q_ASSERT(refs >= 0);

        fprintf(stderr, qPrintable(s));
    }

#  define REF_JOBJECT jobjectRefCount(true)
#  define DEREF_JOBJECT jobjectRefCount(false)
#else
#  define REF_JOBJECT // noop
#  define DEREF_JOBJECT // noop
#endif // JOBJECT_REFCOUNT

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
struct JObjectValueWrapperPrivate;

class JObjectValueWrapper : public JObjectWrapper{
public:
    JObjectValueWrapper();
    JObjectValueWrapper(const JObjectValueWrapper& other);
    JObjectValueWrapper(JObjectValueWrapper&& other);
    ~JObjectValueWrapper() override;
    JObjectValueWrapper& operator=(const JObjectWrapper &wrapper)override;
    JObjectValueWrapper& operator=(JObjectWrapper &&wrapper)override;
    JObjectValueWrapper& operator=(const JObjectValueWrapper &wrapper);
    JObjectValueWrapper& operator=(JObjectValueWrapper &&wrapper);
    JObjectValueWrapper& operator=(jobject obj)override;
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
    JObjectValueWrapper(const JObjectValueWrapperPrivate* methods);
    JObjectValueWrapper(JNIEnv* env, jobject object, const JObjectValueWrapperPrivate* methods);
    JObjectValueWrapper(JObjectWrapper&& object, const JObjectValueWrapperPrivate* methods);
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
    friend JObjectValueWrapperPrivate;
    const JObjectValueWrapperPrivate* p;
};

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
#endif

#endif // REGISTRYUTIL_P_H
