/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#ifndef QTJAMBI_REGISTRY_P_H
#define QTJAMBI_REGISTRY_P_H

#include <QtCore/QSet>
#include <QtCore/QMap>
#include "qtjambi_registry.h"
#include "qtjambi_typeinfo_p.h"
#include <typeindex>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#define QRecursiveMutexLocker QMutexLocker
#else
#define QRecursiveMutexLocker QMutexLocker<QRecursiveMutex>
#endif
class SuperTypeInfos;

struct InterfaceOffsetInfo{
    QMap<size_t,uint> offsets;
    QSet<size_t> interfaces;
    QMap<size_t, QSet<const std::type_info*>> inheritedInterfaces;
};

jclass getGlobalClassRef(JNIEnv *env, jclass cls, const char *className = nullptr);

hash_type qHash(const std::type_index& idx, hash_type seed = 0) Q_DECL_NOEXCEPT;
hash_type qHash(const char *p, hash_type seed = 0) Q_DECL_NOEXCEPT;
const QMetaObject* registeredOriginalMetaObject(const std::type_info& typeId);
PtrDeleterFunction deleter(const std::type_info& typeId);
PtrOwnerFunction registeredOwnerFunction(const std::type_info& typeId);
const char* getInterface(const char*qt_name);
const char * getQtName(const std::type_info& typeId);
const char * getJavaName(const std::type_info& typeId);
void storeFunctionTable(const QString &className, QtJambiFunctionTable *table);
bool isFunctional(const std::type_info& typeId);
bool isInterface(const std::type_info& typeId);
const std::type_info* getInterfaceTypeForIID(const char*interface_iid);
const std::type_info* getTypeByJavaName(const char * java_name);
const std::type_info* getTypeByJavaName(const QString& java_name);
const std::type_info* getTypeByQtName(const char * qt_name);
const std::type_info* getTypeByQtName(const QString& qt_name);
const std::type_info* getTypeByQtName(const QByteArray& qt_name);
const std::type_info* getTypeByMetaObject(const QMetaObject* metaObject);
int registeredMetaTypeID(const std::type_info& typeId);
void qtjambi_resolve_polymorphic_id(const std::type_info& polymorphicBaseTypeId, const void *object,
                                    char const* &class_name, const std::type_info* &targetTypeId, bool& isQObject);
int registeredInterfaceOffset(const std::type_info& qt_base, const std::type_info& qt_interface);
const QVector<const SignalMetaInfo>* signalMetaInfos(const QMetaObject* metaObject);
ParameterInfoProvider registeredParameterInfoProvider(const QMetaObject* metaObject);
bool isValueOwner(const QMetaObject* metaObject);
#ifdef QT_QTJAMBI_PORT
SignalConnector signalConnector(const QMetaObject* metaObject);
#endif
const std::type_info* getTypeByMetaType(int metaType);
const std::type_info* getTypeByMetaType(const QMetaType& metaType);
PtrOwnerFunction registeredOwnerFunction(const std::type_info& typeId);
NewContainerAccessFunction getContainerAccessFactory(const std::type_info& typeId);
bool isPolymorphicBase(const std::type_info& typeId);
size_t getValueSize(const std::type_info& typeId);
size_t getValueAlignment(const std::type_info& typeId);
size_t getShellSize(const std::type_info& typeId);
EntryTypes getEntryType(const std::type_info& typeId);
const std::type_info* getEnumForFlag(const std::type_info& flag);
const std::type_info* getFlagForEnum(const std::type_info& enumerator);
const QVector<const FunctionInfo>* virtualFunctions(const std::type_info& typeId);
bool isInterface(const char*qt_interface);
const QMetaObject* registeredOriginalMetaObject(const std::type_info& typeId);
Destructor registeredDestructor(const std::type_info& typeId);
FunctionalResolver registeredFunctionalResolver(const std::type_info& typeId);
jmethodID findInternalPrivateConstructor(JNIEnv *env, jclass clazz);
const std::type_info* getPolymorphicBase(const std::type_info& typeId);
void registeredInterfaceOffsets(const std::type_info& qt_type, InterfaceOffsetInfo* info);
const InterfaceOffsetInfo* getInterfaceOffsets(JNIEnv *env, jclass clazz, const std::type_info& typeId, const SuperTypeInfos* superTypeInfos);
jmethodID findEmitMethod(JNIEnv * env, jclass signalClass);
QHashFunctionPtr registeredHashFunction(const std::type_info& typeId);
const QtJambiTypeInfo* getQTypeInfo(const std::type_info& typeId);
jfieldID resolveField(JNIEnv *env, const char *fieldName, const char *signature, jclass clazz, bool isStatic = false);
jfieldID resolveField(JNIEnv *env, const char *fieldName, const char *signature, const char *className, bool isStatic = false);

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

template<typename T>
T hashSum(std::initializer_list<T> list){
    T result = 1;
    for(const T* i = list.begin(); i!=list.end(); ++i){
        result = 31 * result + *i;
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

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

class QtJambiObjectDataContainerPrivate;
struct QtJambiObjectData;

class QtJambiObjectDataContainer{
public:
    QtJambiObjectDataContainer();
    QtJambiObjectDataContainer(const QtJambiObjectDataContainer& other);
    QtJambiObjectDataContainer(QtJambiObjectDataContainer&& other);
    ~QtJambiObjectDataContainer();
    QtJambiObjectDataContainer& operator=(const QtJambiObjectDataContainer& other);
    QtJambiObjectDataContainer& operator=(QtJambiObjectDataContainer&& other);
    bool operator==(const QtJambiObjectDataContainer& other);
    void swap(QtJambiObjectDataContainer& other);
    void setUserData(const std::type_info& id, QtJambiObjectData* data);
    QtJambiObjectData* userData(const std::type_info& id) const;
private:
    QExplicitlySharedDataPointer<QtJambiObjectDataContainerPrivate> p;
};

Q_DECLARE_METATYPE(QtJambiObjectDataContainer)

void swap(QtJambiObjectDataContainer& a, QtJambiObjectDataContainer& b);

#endif

#endif // QTJAMBI_REGISTRY_P_H
