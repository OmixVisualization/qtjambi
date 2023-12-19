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

#ifndef QTJAMBILINK_P_H
#define QTJAMBILINK_P_H

#include <QtCore/QMutex>
#include <QtCore/QPointer>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QRunnable>
#include "testapi.h"
#include "registryutil_p.h"
#include "qtjambiapi.h"
#include "jobjectwrapper.h"
#include "coreapi.h"
#include <typeindex>

#if defined(QTJAMBI_DEBUG_TOOLS) || !defined(QT_NO_DEBUG)
#define QTJAMBI_INCREASE_COUNTER_THIS(counter) TestAPI::increaseDebugCounter(TestAPI::DebugCounters::counter, qtTypeName());
#define QTJAMBI_INCREASE_COUNTER(counter, _this) if(_this) TestAPI::increaseDebugCounter(TestAPI::DebugCounters::counter, _this->qtTypeName());
#define QTJAMBI_INCREASE_COUNTER2(counter, qtTypeName) TestAPI::increaseDebugCounter(TestAPI::DebugCounters::counter, qtTypeName);
namespace TestAPI{
void increaseDebugCounter(TestAPI::DebugCounters debugCounter, const char*className);
}
#else
#define QTJAMBI_INCREASE_COUNTER_THIS(counter)
#define QTJAMBI_INCREASE_COUNTER(counter, _this)
#define QTJAMBI_INCREASE_COUNTER2(counter, qtTypeName)
#endif

#if defined(QTJAMBI_DEBUG_TOOLS)
enum class AquireSources : quint8 {
    None = 0,
    LinkDestructor = 1,
    OnClean = 2,
    OnDispose = 3,
    UserdataDestructor = 4
};
#define QTJAMBI_ACQUIRE_MAGIC(what, _this) _this->acquireMagic(AquireSources::what);
#define QTJAMBI_RELEASE_MAGIC(what, _this) _this->releaseMagic(AquireSources::what);

// Allow useful truncation on 32bit
#define QTJAMBI_LINK_MAGIC (long(0xf6f700fff5f42f04ll))

#define QTJAMBI_REGISTER_LINK registerLink(this);
#define QTJAMBI_UNREGISTER_LINK unregisterLink(this);
#define QTJAMBI_UNREGISTER_LINK_FINALLY if(m_flags.testFlag(Flag::IsListed)) unregisterLink(this);
#else
#define QTJAMBI_REGISTER_LINK
#define QTJAMBI_UNREGISTER_LINK
#define QTJAMBI_UNREGISTER_LINK_FINALLY
#define QTJAMBI_ACQUIRE_MAGIC(what, _this)
#define QTJAMBI_RELEASE_MAGIC(what, _this)
#endif

#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
#define LINK_NAME_ARG(A) A,
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#define LINK_NAME_META_TYPE_ARG(META_TYPE) META_TYPE.name(),
#else
#define LINK_NAME_META_TYPE_ARG(META_TYPE) QMetaType::typeName(META_TYPE.id()),
#endif
#else
#define LINK_NAME_ARG(A)
#define LINK_NAME_META_TYPE_ARG(META_TYPE)
#endif


class SuperTypeInfos;

typedef std::function<void(JNIEnv*, jobject, jvalue *)> CallEmitMethod;

class JObjectSynchronizer{
public:
    JObjectSynchronizer(JNIEnv* env, jobject object);
    ~JObjectSynchronizer();
private:
    JNIEnv* const env;
    jobject const object;
    const bool monitor;
};

typedef void*(*SmartPointerGetter)(const void *);
typedef std::function<void*(const void *)> SmartPointerGetterFunction;

class QtJambiLink;

struct ValueOwnerUserData : public QtJambiObjectData
{
    ValueOwnerUserData(const QObject* object);
    ~ValueOwnerUserData() override;
    void addDeleter(const std::function<void()>& deleter);
    inline const QPointer<const QObject>& pointer() const { return m_object; }
    QTJAMBI_OBJECTUSERDATA_ID_DECL
private:
    Q_DISABLE_COPY_MOVE(ValueOwnerUserData)
    QPointer<const QObject> m_object;
    QList<std::function<void()>> m_deleters;
};

struct DependencyManagerUserData : public QtJambiObjectData
{
    DependencyManagerUserData();
    ~DependencyManagerUserData() override;
    void addDependentObject(const QSharedPointer<QtJambiLink>& dependent);
    void removeDependentObject(const QSharedPointer<QtJambiLink>& dependent);
    void addFinalization(std::function<void(JNIEnv*)>&& finalization);
    bool hasDependencies() const;
    void clear(JNIEnv* env);
    static DependencyManagerUserData* instance(const QObject* object, bool forceConstruction = true);
    QTJAMBI_OBJECTUSERDATA_ID_DECL
private:
    Q_DISABLE_COPY_MOVE(DependencyManagerUserData)
    void clear(JNIEnv* env, QSet<QWeakPointer<QtJambiLink>>& dependentObjects, QList<std::function<void(JNIEnv*)>>& finalizations);
    QSet<QWeakPointer<QtJambiLink>> m_dependentObjects;
    QList<std::function<void(JNIEnv*)>> m_finalizations;
};

class DependentLink{
protected:
    DependentLink() = default;
    virtual ~DependentLink();
    virtual void setDependencyManager(DependencyManagerUserData*) = 0;
    friend QtJambiLink;
    friend DependencyManagerUserData;
};

struct QtJambiLinkUserData : public QtJambiObjectData
{
    QtJambiLinkUserData(const QWeakPointer<QtJambiLink> & link);
    ~QtJambiLinkUserData() override;

    inline const QWeakPointer<QtJambiLink>& link() const { return m_link; }
    virtual inline const QMetaObject* metaObject() const {return nullptr; }

    QTJAMBI_OBJECTUSERDATA_ID_DECL
    static QReadWriteLock* lock();

private:
    Q_DISABLE_COPY_MOVE(QtJambiLinkUserData)
    QWeakPointer<QtJambiLink> m_link;
};

struct QtJambiMetaObjectLinkUserData : public QtJambiLinkUserData{
    QtJambiMetaObjectLinkUserData(const QWeakPointer<QtJambiLink> & link, const QMetaObject* metaObject);
    inline const QMetaObject* metaObject() const override {return m_metaObject; }
private:
    const QMetaObject* m_metaObject;
    Q_DISABLE_COPY_MOVE(QtJambiMetaObjectLinkUserData)
};

class QSharedPointerToQObjectLink;

class PointerContainer : public QObject{
public:
    explicit PointerContainer(JNIEnv* env, jobject jobj, const QMetaObject* metaObject, const QSharedPointer<QtJambiLink>& link, void* ptr_shared_pointer, bool isShell, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException);

    ~PointerContainer() override;
    void* getSharedPointer() const;
    QObject *qobject() const;
    virtual jobject getExtraSignal(JNIEnv * env, const QSharedPointerToQObjectLink* link, const QMetaMethod& method) const;
    void setAsQObjectDeleted();
private:
    bool isDebugMessagingDisabled()const;
    void* m_ptr_shared_pointer;
    bool m_isShell;
    SmartPointerDeleter m_shared_pointer_deleter;
    SmartPointerGetterFunction m_shared_pointer_getter;
    QWeakPointer<QtJambiLink> m_link;
};

class PointerContainerWithExtraSignals : public PointerContainer{
public:
    explicit PointerContainerWithExtraSignals(JNIEnv* env, jobject jobj, const QMetaObject* metaObject, const QSharedPointer<QtJambiLink>& link, void* ptr_shared_pointer, bool isShell, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException);
    ~PointerContainerWithExtraSignals() override = default;
    jobject getExtraSignal(JNIEnv * env, const QSharedPointerToQObjectLink* link, const QMetaMethod& method) const override;
private:
    mutable QHash<int,JObjectWrapper> m_extraSignals;
};

class PointerContainerWithPendingExtraSignals : public PointerContainer{
public:
    explicit PointerContainerWithPendingExtraSignals(JNIEnv* env, jobject jobj, const QMetaObject* metaObject, const QSharedPointer<QtJambiLink>& link, void* ptr_shared_pointer, bool isShell, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException);
    ~PointerContainerWithPendingExtraSignals() override = default;
    jobject getExtraSignal(JNIEnv * env, const QSharedPointerToQObjectLink* link, const QMetaMethod& method) const override;
private:
    mutable QHash<int,JObjectWrapper> m_extraSignals;
};

class QtJambiLink{
public:
    enum class Ownership : quint8 {
        None = 0,
        Java = 0x001,   // Weak ref to java object, deleteNativeObject deletes c++ object
        Cpp = 0x002,    // Strong ref to java object until c++ object is deleted, deleteNativeObject does *not* delete c++ obj.
        Split = 0x004  // Weak ref to java object, deleteNativeObject does *not* delete c++ object. Only for objects not created by Java.
    };

    enum class Flag : quint32
{
        None = 0,
        JavaOwnership = quint32(Ownership::Java),   // Weak ref to java object, deleteNativeObject deletes c++ object
        CppOwnership = quint32(Ownership::Cpp),    // Strong ref to java object until c++ object is deleted, deleteNativeObject does *not* delete c++ obj.
        SplitOwnership = quint32(Ownership::Split),  // Weak ref to java object, deleteNativeObject does *not* delete c++ object. Only for objects not created by Java.
        OwnershipMask = JavaOwnership | CppOwnership | SplitOwnership,
        GlobalReference = 0x0008,
        IsInitialized = 0x0010,
        IsOffsetsUnregistered = 0x0020,
        HasBeenCleaned = 0x0040,
        CreatedByJava = 0x0080,
        IsShell = 0x0100,
        IsJavaLinkDetached = 0x0200,
        BlockDeletion = 0x0400,
        IsDependent = 0x0800,
        HasDependencies = 0x080000,
        IsInDestructor = 0x1000,
        IsDeleteLater = 0x2000,
        NoThreadInitializationOnPurge = 0x4000,
        CreatedByQml = 0x8000,
#if defined(QTJAMBI_DEBUG_TOOLS)
        IsListed = 0x010000,   // Weak ref to java object, deleteNativeObject deletes c++ object
#endif
        HasDisposedSignal = 0x020000,
        IsPendingObjectResolved = 0x040000,
        IsPendingValueOwner = 0x080000,
        NoNativeDeletion = 0x100000,
        NoDebugMessaging = 0x200000
    };
    typedef QFlags<Flag> Flags;

    static const QSharedPointer<QtJambiLink>& createLinkForContainer(JNIEnv *env, jobject java, void *ptr,
                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                  bool created_by_java, bool isCopy, PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess);
    static const QSharedPointer<QtJambiLink>& createLinkForContainer(JNIEnv *env, jobject java, void *ptr, const QMetaType& metaType,
                                                                  bool created_by_java, bool isCopy, AbstractContainerAccess* containerAccess);
    static const QSharedPointer<QtJambiLink>& createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                  bool created_by_java, bool isCopy, PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess,
                                                                        QtJambiLink::Ownership ownership);
    static const QSharedPointer<QtJambiLink>& createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool isCopy, PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess,
                                                                        QtJambiLink::Ownership ownership,
                                                                        const QMap<size_t,uint>& interfaceOffsets,
                                                                        const QSet<size_t>& interfaceTypes,
                                                                        const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces);
    static const QSharedPointer<QtJambiLink>& createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr, const QMetaType& metaType,
                                                                  bool created_by_java, bool isCopy, AbstractContainerAccess* containerAccess, QtJambiLink::Ownership ownership);
    static const QSharedPointer<QtJambiLink>& createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr, const QMetaType& metaType,
                                                                     bool created_by_java, bool isCopy, AbstractContainerAccess* containerAccess, QtJambiLink::Ownership ownership,
                                                                     const QMap<size_t,uint>& interfaceOffsets,
                                                                     const QSet<size_t>& interfaceTypes,
                                                                     const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces);
    static const QSharedPointer<QtJambiLink>& createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                  bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, QtJambiLink::Ownership ownership);
    static const QSharedPointer<QtJambiLink>& createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                  bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, QtJambiLink::Ownership ownership);
    static const QSharedPointer<QtJambiLink>& createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                  bool created_by_java, bool is_shell, QtJambiLink::Ownership ownership);
    static const QSharedPointer<QtJambiLink>& createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr, const QMetaType& metaType,
                                                                  bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, QtJambiLink::Ownership ownership);
    static const QSharedPointer<QtJambiLink>& createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr, const QMetaType& metaType,
                                                                  bool created_by_java, bool is_shell, QtJambiLink::Ownership ownership);
    static const QSharedPointer<QtJambiLink>& createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, QtJambiLink::Ownership ownership,
                                                                        const QMap<size_t,uint>& interfaceOffsets,
                                                                        const QSet<size_t>& interfaceTypes,
                                                                        const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces);
    static const QSharedPointer<QtJambiLink>& createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, QtJambiLink::Ownership ownership,
                                                                        const QMap<size_t,uint>& interfaceOffsets,
                                                                        const QSet<size_t>& interfaceTypes,
                                                                        const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces);
    static const QSharedPointer<QtJambiLink>& createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool is_shell, QtJambiLink::Ownership ownership,
                                                                        const QMap<size_t,uint>& interfaceOffsets,
                                                                        const QSet<size_t>& interfaceTypes,
                                                                        const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces);
    static const QSharedPointer<QtJambiLink>& createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr, const QMetaType& metaType,
                                                                        bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, QtJambiLink::Ownership ownership,
                                                                        const QMap<size_t,uint>& interfaceOffsets,
                                                                        const QSet<size_t>& interfaceTypes,
                                                                        const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces);
    static const QSharedPointer<QtJambiLink>& createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr, const QMetaType& metaType,
                                                                        bool created_by_java, bool is_shell, QtJambiLink::Ownership ownership,
                                                                        const QMap<size_t,uint>& interfaceOffsets,
                                                                        const QSet<size_t>& interfaceTypes,
                                                                        const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces);
    static const QSharedPointer<QtJambiLink>& createLinkForDependentObject(JNIEnv *env, jobject java, void *ptr, const QMetaType& metaType,
                                                                           bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, const QObject* dependsOn, QtJambiLink::Ownership ownership);
    static const QSharedPointer<QtJambiLink>& createLinkForDependentObject(JNIEnv *env, jobject java, void *ptr, const QMetaType& metaType,
                                                                           bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, const QObject* dependsOn, QtJambiLink::Ownership ownership,
                                                                           const QMap<size_t,uint>& interfaceOffsets,
                                                                           const QSet<size_t>& interfaceTypes,
                                                                           const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces);
    static const QSharedPointer<QtJambiLink>& createLinkForDependentObject(JNIEnv *env, jobject java, void *ptr,
                                                                           LINK_NAME_ARG(const char* qt_name)
                                                                           bool created_by_java, bool is_shell,
                                                                           PtrDeleterFunction destructor_function,
                                                                           const QObject* dependsOn, QtJambiLink::Ownership ownership);
    static const QSharedPointer<QtJambiLink>& createExtendedLinkForObject(JNIEnv *env, jobject java, void *ptr,
                                                                    LINK_NAME_ARG(const char* qt_name)
                                                                    bool created_by_java, bool is_shell,
                                                                    PtrDeleterFunction destructor_function,
                                                                    const QObject* extension, QtJambiLink::Ownership ownership);
    static const QSharedPointer<QtJambiLink>& createLinkForDependentObject(JNIEnv *env, jobject java, void *ptr, const QMetaType& metaType,
                                                                           bool created_by_java, bool is_shell, const QObject* dependsOn, QtJambiLink::Ownership ownership);
    static const QSharedPointer<QtJambiLink>& createLinkForDependentObject(JNIEnv *env, jobject java, void *ptr, const QMetaType& metaType,
                                                                           bool created_by_java, bool is_shell, const QObject* dependsOn, QtJambiLink::Ownership ownership,
                                                                           const QMap<size_t,uint>& interfaceOffsets,
                                                                           const QSet<size_t>& interfaceTypes,
                                                                           const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces);
    static const QSharedPointer<QtJambiLink>& createLinkForOwnedObject(JNIEnv *env, jobject java, void *ptr,
                                                                       LINK_NAME_ARG(const char* qt_name)
                                                                       QtJambiNativeID owner, PtrDeleterFunction destructor_function);
    static const QSharedPointer<QtJambiLink>& createLinkForOwnedObject(JNIEnv *env, jobject java, void *ptr,
                                                                       LINK_NAME_ARG(const char* qt_name)
                                                                       QtJambiNativeID owner);
    static const QSharedPointer<QtJambiLink>& createLinkForOwnedContainer(JNIEnv *env, jobject java, void *ptr,
                                                                       LINK_NAME_ARG(const char* qt_name)
                                                                       QtJambiNativeID owner, AbstractContainerAccess* containerAccess);
    static const QSharedPointer<QtJambiLink>& createLinkForOwnedContainer(JNIEnv *env, jobject java, void *ptr,
                                                                       LINK_NAME_ARG(const char* qt_name)
                                                                       QtJambiNativeID owner, PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess);
    static const QSharedPointer<QtJambiLink>& createLinkForSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                 LINK_NAME_ARG(const char* qt_name)
                                                                                 bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, void* ptr_shared_pointer, SmartPointerDeleter pointerDeleter, SmartPointerGetterFunction pointerGetter);
    static const QSharedPointer<QtJambiLink>& createLinkForSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                LINK_NAME_ARG(const char* qt_name)
                                                                                bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, void* ptr_shared_pointer, SmartPointerDeleter pointerDeleter, SmartPointerGetterFunction pointerGetter,
                                                                                const QMap<size_t,uint>& offsets,
                                                                                const QSet<size_t>& interfaces,
                                                                                const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces);
    static const QSharedPointer<QtJambiLink>& createExtendedLinkForSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                LINK_NAME_ARG(const char* qt_name)
                                                                                bool created_by_java, bool is_shell, const QObject* extension, PtrOwnerFunction ownerFunction, void* ptr_shared_pointer, SmartPointerDeleter pointerDeleter, SmartPointerGetterFunction pointerGetter);
    static const QSharedPointer<QtJambiLink>& createLinkForSmartPointerToContainer(JNIEnv *env, jobject java,
                                                                                 LINK_NAME_ARG(const char* qt_name)
                                                                                 void* ptr_shared_pointer, SmartPointerDeleter pointerDeleter, SmartPointerGetterFunction pointerGetter, AbstractContainerAccess* containerAccess);
    static const QSharedPointer<QtJambiLink>& createLinkForSmartPointerToContainer(JNIEnv *env, jobject java,
                                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                                   void* ptr_shared_pointer, SmartPointerDeleter pointerDeleter, SmartPointerGetterFunction pointerGetter, AbstractContainerAccess* containerAccess,
                                                                                   const QMap<size_t,uint>& offsets,
                                                                                   const QSet<size_t>& interfaces,
                                                                                   const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces);
    static const QSharedPointer<QtJambiLink>& createLinkForQObject(JNIEnv *env, jobject java, QObject *object, bool created_by_java, bool is_shell, const QMetaObject* superTypeForCustomMetaObject = nullptr);
    static const QSharedPointer<QtJambiLink>& createLinkForNativeQObject(JNIEnv *env, jobject java, QObject *object, bool created_by_java, bool is_shell, const QMetaObject* superTypeForCustomMetaObject);
    static const QSharedPointer<QtJambiLink>& createLinkForNativeQObject(JNIEnv *env, jobject java, QObject *object, bool created_by_java, bool is_shell, const QMetaObject* superTypeForCustomMetaObject,
                                                                         const QMap<size_t,uint>& interfaceOffsets,
                                                                         const QSet<size_t>& interfaceTypes,
                                                                         const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces);
    static const QSharedPointer<QtJambiLink>& createLinkForSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, bool is_shell, void* ptr_shared_pointer, SmartPointerDeleter pointerDeleter, SmartPointerGetterFunction pointerGetter, const QMetaObject* superTypeForCustomMetaObject = nullptr);
    static const QSharedPointer<QtJambiLink>& createLinkForSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, bool is_shell, void* ptr_shared_pointer, SmartPointerDeleter pointerDeleter, SmartPointerGetterFunction pointerGetter,
                                                                                 const QMap<size_t,uint>& offsets,
                                                                                 const QSet<size_t>& interfaces,
                                                                                 const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces, const QMetaObject* superTypeForCustomMetaObject = nullptr);
    static const QSharedPointer<QtJambiLink>& createLinkForNewQObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject java, const std::type_info& typeId, const QMetaObject* metaObject, QObject *object, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool isDeclarativeCall, bool is_shell, bool hasCustomMetaObject, JavaException& ocurredException);
    static const QSharedPointer<QtJambiLink>& createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject java, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, const QMetaType& metaType, bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    static const QSharedPointer<QtJambiLink>& createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject java, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, const QMetaType& metaType, bool created_by_java, bool is_shell, JavaException& ocurredException);
    static const QSharedPointer<QtJambiLink>& createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject java, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    static const QSharedPointer<QtJambiLink>& createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject java, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, JavaException& ocurredException);
    static const QSharedPointer<QtJambiLink>& createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject java, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, JavaException& ocurredException);
    static const QSharedPointer<QtJambiLink>& createLinkForNewContainer(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject java, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, const QMetaType& metaType, bool created_by_java, bool is_shell, AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    static const QSharedPointer<QtJambiLink>& createLinkForNewContainer(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject java, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, JavaException& ocurredException);
    static const QSharedPointer<QtJambiLink>& createLinkForNewContainer(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject java, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    static const QSharedPointer<QtJambiLink>& fromNativeId(QtJambiNativeID native_id);
    static QSharedPointer<QtJambiLink> findLinkForQObject(QObject *qobject);
    static QList<QSharedPointer<QtJambiLink>> findLinksForPointer(const void *ptr);
    static const QSharedPointer<QtJambiLink>& findLinkForJavaObject(JNIEnv *env, jobject java);
    static const QSharedPointer<QtJambiLink>& findLinkForJavaInterface(JNIEnv *env, jobject java);
    static void* findPointerForJavaObject(JNIEnv *env, jobject java);
    static void* findPointerForJavaInterface(JNIEnv *env, jobject java, const std::type_info& typeId);
    static int getObjectCacheOperationCount();
    static int getObjectCacheMode();
    static void setObjectCacheMode(int object_cache_mode);
    static jobject getNativeLink(JNIEnv *env, jobject java);
    static void registerPointer(const QSharedPointer<QtJambiLink>& link);
    static void registerQObjectInitialization(void *ptr, const QSharedPointer<QtJambiLink>& link);
    static void unregisterQObjectInitialization(void *ptr);
private:
public:
    QtJambiLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                    LINK_NAME_ARG(const char* qt_name)
                    bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException);
    virtual ~QtJambiLink();

    bool hasDisposedSignal() const;
    void setHasDisposedSignal();
    jobject getJavaObjectLocalRef(JNIEnv *env) const;
    bool createdByJava() const;
    bool createdByQml() const;
    bool isShell() const;
    bool isDebugMessagingDisabled()const;
    void disableDebugMessaging();
    void setJavaOwnership(JNIEnv *env);
    void setDefaultOwnership(JNIEnv *env);
    virtual void setCppOwnership(JNIEnv *env);
    virtual void setSplitOwnership(JNIEnv *env);
    Ownership ownership() const;
    QWeakPointer<QtJambiLink> getWeakPointer() const;
    const QSharedPointer<QtJambiLink>& getStrongPointer() const;
    void detachJavaLink(JNIEnv *env);
    virtual void invalidate(JNIEnv *env) = 0;
    virtual QObject *qobject() const = 0;
    virtual bool isQObject() const = 0;
    bool noThreadInitializationOnPurge() const;
    void setNoThreadInitializationOnPurge(bool n);
    virtual bool qobjectDeleted() const = 0;
    virtual bool isSmartPointer() const = 0;
    virtual QString describe() const = 0;
    virtual bool isMultiInheritanceType() const;

    bool isInDestructor() const;

    /* Deletes any global references to the java object so that it can
       be finalized by the virtual machine.
       Returns disposition bitmask. */
    void releaseJavaObject(JNIEnv *env);

    /* Triggered by native jni functions when a java object has been
       finalized. */
    virtual void onClean(JNIEnv *env) = 0;

    /* Called by the native jni fucntion when the java object has been
       disposed */
    virtual void onDispose(JNIEnv *env) = 0;

    jobject nativeLink(JNIEnv* env) const;

    virtual void init(JNIEnv* env);

    inline virtual AbstractContainerAccess* containerAccess() const { return nullptr; }

    virtual void *pointer() const = 0;
    virtual void addInterface(const std::type_info& qtInterfaceType);
    virtual void removeInterface(const std::type_info& qtInterfaceType);
    virtual bool isInterfaceAvailable(const std::type_info& qtInterfaceType) const;

    virtual void* typedPointer(const std::type_info& qtType) const;
    void registerDependentObject(const QSharedPointer<QtJambiLink>& link);
    void addFinalization(std::function<void(JNIEnv*)>&& finalization);
    void unregisterDependentObject(const QSharedPointer<QtJambiLink>& link);
    static void registerDependentObject(const QObject* object, const QSharedPointer<QtJambiLink>& link);
    static void unregisterDependentObject(const QObject* object, const QSharedPointer<QtJambiLink>& link);
    static void addFinalization(const QObject* object, std::function<void(JNIEnv*)>&& finalization);

#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
    const char* qtTypeName() const;
#endif
#if defined(QTJAMBI_DEBUG_TOOLS)
    const char *debugFlagsToString(char *buf)const;
    void validateMagic_unlocked(const char *prefix);
    void validateMagic();
    int acquireMagic_unlocked(AquireSources source_id);
    void acquireMagic(AquireSources source_id);
    int releaseMagic_unlocked(AquireSources source_id);
    void releaseMagic(AquireSources source_id);
    // Method shared by QtJambiLinkUserData for description
    static const char *acquire_magic_source_id_to_string(AquireSources source_id);
protected:
    static void registerLink(QtJambiLink* link);
    static void unregisterLink(QtJambiLink* link);
public:
    static int linkCount();
    static int dumpLinks(JNIEnv * env);
    static bool hasDeleteLaterEvents();
    static bool checkLink(QtJambiLink* find);
#endif
public:
    bool isInitialized() const;
    bool isDeleteLater() const { return m_flags.testFlag(Flag::IsDeleteLater); }
    bool isCleanedUp() const { return m_flags.testFlag(Flag::HasBeenCleaned); }
protected:
    void dispose();
    void setDeleteLater(){ m_flags.setFlag(Flag::IsDeleteLater); }
    void invalidateDependentObjects(JNIEnv *env);
    void setInDestructor();
    virtual void unregisterOffsets();
    void registerPointer(void*);
    void unregisterPointer(void*);

    QSharedPointer<QtJambiLink> m_this;

    // This union arrangement helps building on top of GNU ClassPath and maybe
    //  other JVMs using C++ with strict types where jweak != jobject.
    union {
        jobject object;
        jweak weak;
    } m_java;

    jobject m_nativeLink;

    Flags m_flags;
#if defined(QTJAMBI_DEBUG_TOOLS)
    const long m_magic;   // natural bit width
    Ownership m_last_ownership;         // used to track

    // This is a temporary fix and the fallback solution to fine grained locking.
    // On Intel platforms we can use a 8bit atomic spinlock which will not increase
    //  the size of QtJambiLink and still protect the flags.
    QAtomicInt m_atomic_int;
    void *m_pthread_id;
    AquireSources m_magic_source_id;	// gluttony, but we're aligned gluttony
#endif
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
    const char* m_qtTypeName;
#endif
    friend struct QtJambiLinkUserData;
    friend class PointerToQObjectLink;
    friend class PointerContainerWithPendingExtraSignals;
};

class PointerToObjectLink : public QtJambiLink{
protected:
    PointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                        LINK_NAME_ARG(const char* qt_name)
                        void *ptr, bool created_by_java, bool is_shell, JavaException& ocurredException);
public:
    ~PointerToObjectLink() override;
    void *pointer() const override;
    QObject *qobject() const override;
    bool isQObject() const override;
    virtual void deleteNativeObject(JNIEnv *env, bool forced = false);
    inline bool qobjectDeleted() const override { return false; }
    void onDispose(JNIEnv *env) override;
    void onClean(JNIEnv *env) override;
    void invalidate(JNIEnv *env) override;
    QString describe() const override;
    bool isSmartPointer() const override final;
    void init(JNIEnv* env) override;
    virtual const QMetaType& metaType() const;
    inline virtual PtrOwnerFunction ownerFunction() const{return nullptr;}
protected:
    void *m_pointer;
    friend QtJambiLink;
};

class MetaTypedPointerToObjectLink : public PointerToObjectLink{
protected:
    MetaTypedPointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                        void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, JavaException& ocurredException);
public:
    void deleteNativeObject(JNIEnv *env, bool forced = false) override;
    inline const QMetaType& metaType() const override {return m_meta_type;}
protected:
    QMetaType m_meta_type;
    friend QtJambiLink;
};

class DependentMetaTypedPointerToObjectLink : public MetaTypedPointerToObjectLink, public DependentLink{
protected:
    DependentMetaTypedPointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                                          void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, JavaException& ocurredException);
    void setDependencyManager(DependencyManagerUserData*) override;
    void deleteNativeObject(JNIEnv *env, bool forced) override;
    void invalidate(JNIEnv *env) override;
private:
    DependencyManagerUserData* m_dependencyManager;
    friend QtJambiLink;
};

class MetaTypedPointerToContainerLink : public MetaTypedPointerToObjectLink{
protected:
    MetaTypedPointerToContainerLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                        void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell,
                        AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    ~MetaTypedPointerToContainerLink() override;
public:
    void deleteNativeObject(JNIEnv *env, bool forced = false) override;
    inline AbstractContainerAccess* containerAccess() const override { return m_containerAccess; }
private:
    AbstractContainerAccess *const m_containerAccess;
    friend QtJambiLink;
};

class PointerToContainerLink : public PointerToObjectLink{
protected:
    PointerToContainerLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                        LINK_NAME_ARG(const char* qt_name)
                        void *ptr, bool created_by_java, bool is_shell,
                        AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    ~PointerToContainerLink() override;
public:
    inline AbstractContainerAccess* containerAccess() const override { return m_containerAccess; }
private:
    AbstractContainerAccess *const m_containerAccess;
    friend QtJambiLink;
};

class OwnedMetaTypedPointerToObjectLink : public MetaTypedPointerToObjectLink{
protected:
    OwnedMetaTypedPointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                        void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell,
                        PtrOwnerFunction ownerFunction, JavaException& ocurredException);
public:
    void init(JNIEnv* env) override;
    void deleteNativeObject(JNIEnv *env, bool forced = false) override;
    inline PtrOwnerFunction ownerFunction() const override {return m_owner_function;}
private:
    PtrOwnerFunction m_owner_function;
    friend QtJambiLink;
};

class OwnedDependentMetaTypedPointerToObjectLink : public OwnedMetaTypedPointerToObjectLink, public DependentLink{
protected:
    OwnedDependentMetaTypedPointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                        void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell,
                        PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    void setDependencyManager(DependencyManagerUserData*) override;
    void deleteNativeObject(JNIEnv *env, bool forced) override;
    void invalidate(JNIEnv *env) override;
private:
    DependencyManagerUserData* m_dependencyManager;
    friend QtJambiLink;
};

class DeletableOwnedPointerToObjectLink : public PointerToObjectLink{
protected:
    DeletableOwnedPointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                        LINK_NAME_ARG(const char* qt_name)
                        void *ptr, bool created_by_java, bool is_shell,
                        PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
public:
    inline PtrOwnerFunction ownerFunction() const override {return m_owner_function;}
private:
    void init(JNIEnv* env) override;
    void deleteNativeObject(JNIEnv *env, bool forced = false) override;
    PtrOwnerFunction m_owner_function;
    PtrDeleterFunction m_deleter_function;
    friend QtJambiLink;
    friend class DeletableOwnedPointerToContainerLink;
};

class DeletablePointerToObjectLink : public PointerToObjectLink{
public:
    PtrDeleterFunction deleterFunction() const {return m_deleter_function;}
protected:
    DeletablePointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                        LINK_NAME_ARG(const char* qt_name)
                        void *ptr, bool created_by_java, bool is_shell,
                        PtrDeleterFunction destructor_function, JavaException& ocurredException);
    void deleteNativeObject(JNIEnv *env, bool forced = false) override;
private:
    PtrDeleterFunction m_deleter_function;
    friend QtJambiLink;
    friend class DeletablePointerToContainerLink;
};

class ExtendedLink{
protected:
    ExtendedLink() = default;
    virtual ~ExtendedLink();
public:
    virtual bool hasExtension()const = 0;
};

class ExtendedDeletablePointerToObjectLink : public DeletablePointerToObjectLink, public ExtendedLink{
protected:
    ExtendedDeletablePointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                                          LINK_NAME_ARG(const char* qt_name)
                                          void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, const QObject* extension, JavaException& ocurredException);
public:
    inline bool hasExtension()const override{return !m_extension.isNull(); }
private:
    QPointer<const QObject> m_extension;
    friend QtJambiLink;
};

class DependentDeletablePointerToObjectLink : public DeletablePointerToObjectLink, public DependentLink{
protected:
    DependentDeletablePointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                                 LINK_NAME_ARG(const char* qt_name)
                                 void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, JavaException& ocurredException);
    void setDependencyManager(DependencyManagerUserData*) override;
    void deleteNativeObject(JNIEnv *env, bool forced) override;
    void invalidate(JNIEnv *env) override;
private:
    DependencyManagerUserData* m_dependencyManager;
    friend QtJambiLink;
};

class DeletablePointerToContainerLink : public DeletablePointerToObjectLink{
protected:
    DeletablePointerToContainerLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                        LINK_NAME_ARG(const char* qt_name)
                        void *ptr, bool created_by_java, bool is_shell,
                        PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    ~DeletablePointerToContainerLink() override;
public:
    void deleteNativeObject(JNIEnv *env, bool forced = false) final override;
    inline AbstractContainerAccess* containerAccess() const override { return m_containerAccess; }
private:
    AbstractContainerAccess *const m_containerAccess;
    friend QtJambiLink;
};

class DeletableOwnedPointerToContainerLink : public DeletableOwnedPointerToObjectLink{
protected:
    DeletableOwnedPointerToContainerLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                        LINK_NAME_ARG(const char* qt_name)
                        void *ptr, bool created_by_java, bool is_shell,
                        PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    ~DeletableOwnedPointerToContainerLink() override;
public:
    void deleteNativeObject(JNIEnv *env, bool forced = false) final override;
    inline AbstractContainerAccess* containerAccess() const override { return m_containerAccess; }
private:
    AbstractContainerAccess *const m_containerAccess;
    friend QtJambiLink;
};

#define INTERFACE_DECL(LINKTYPE)\
public:\
    ~LINKTYPE() override;\
    void unregisterOffsets() override;\
    void addInterface(const std::type_info& qtInterfaceType) override;\
    void removeInterface(const std::type_info& qtInterfaceType) override;\
    bool isInterfaceAvailable(const std::type_info& qtInterfaceType) const override;\
    void* typedPointer(const std::type_info& qtType) const override;\
    bool isMultiInheritanceType() const override;\
private:\
    QMap<size_t, uint> m_interfaceOffsets;\
    QSet<size_t> m_availableInterfaces;\
    QMap<size_t, QSet<const std::type_info*>> m_inheritedInterfaces;\
    friend QtJambiLink;

class PointerToObjectInterfaceLink : public PointerToObjectLink{
protected:
    PointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                                           LINK_NAME_ARG(const char* qt_name)
                                 void *ptr, bool created_by_java, bool is_shell, JavaException& ocurredException);
    INTERFACE_DECL(PointerToObjectInterfaceLink)
};

class MetaTypedPointerToObjectInterfaceLink : public MetaTypedPointerToObjectLink{
protected:
    MetaTypedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                 void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, JavaException& ocurredException);
    INTERFACE_DECL(MetaTypedPointerToObjectInterfaceLink)
};

class DependentMetaTypedPointerToObjectInterfaceLink : public MetaTypedPointerToObjectInterfaceLink, public DependentLink{
protected:
    DependentMetaTypedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                   void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, JavaException& ocurredException);
    void setDependencyManager(DependencyManagerUserData*) override;
    void deleteNativeObject(JNIEnv *env, bool forced) override;
    void invalidate(JNIEnv *env) override;
private:
    DependencyManagerUserData* m_dependencyManager;
    friend QtJambiLink;
};

class OwnedMetaTypedPointerToObjectInterfaceLink : public OwnedMetaTypedPointerToObjectLink{
protected:
    OwnedMetaTypedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                 void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    INTERFACE_DECL(OwnedMetaTypedPointerToObjectInterfaceLink)
};

class OwnedDependentMetaTypedPointerToObjectInterfaceLink : public OwnedMetaTypedPointerToObjectInterfaceLink, public DependentLink{
protected:
    OwnedDependentMetaTypedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                 void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    void setDependencyManager(DependencyManagerUserData*) override;
    void deleteNativeObject(JNIEnv *env, bool forced) override;
    void invalidate(JNIEnv *env) override;
private:
    DependencyManagerUserData* m_dependencyManager;
    friend QtJambiLink;
};

class PointerToContainerInterfaceLink : public PointerToContainerLink{
protected:
    PointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                                           LINK_NAME_ARG(const char* qt_name)
                                 void *ptr, bool created_by_java, bool is_shell, AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    INTERFACE_DECL(PointerToContainerInterfaceLink)
};

class MetaTypedPointerToContainerInterfaceLink : public MetaTypedPointerToContainerLink{
protected:
    MetaTypedPointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                 void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    INTERFACE_DECL(MetaTypedPointerToContainerInterfaceLink)
};

class DeletablePointerToObjectInterfaceLink : public DeletablePointerToObjectLink{
protected:
    DeletablePointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                                           LINK_NAME_ARG(const char* qt_name)
                                 void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, JavaException& ocurredException);
    INTERFACE_DECL(DeletablePointerToObjectInterfaceLink)
};

class DeletablePointerToContainerInterfaceLink : public DeletablePointerToContainerLink{
protected:
    DeletablePointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                                           LINK_NAME_ARG(const char* qt_name)
                                 void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    INTERFACE_DECL(DeletablePointerToContainerInterfaceLink)
};

class DeletableOwnedPointerToContainerInterfaceLink : public DeletableOwnedPointerToContainerLink{
protected:
    DeletableOwnedPointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                                           LINK_NAME_ARG(const char* qt_name)
                                 void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    INTERFACE_DECL(DeletableOwnedPointerToContainerInterfaceLink)
};

class DeletableOwnedPointerToObjectInterfaceLink : public DeletableOwnedPointerToObjectLink{
protected:
    DeletableOwnedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                                           LINK_NAME_ARG(const char* qt_name)
                                 void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    INTERFACE_DECL(DeletableOwnedPointerToObjectInterfaceLink)
};

class PointerToQObjectLink : public QtJambiLink{
protected:
    PointerToQObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMetaObject* metaObject, QObject *ptr, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException);
public:
    ~PointerToQObjectLink() override;
    void *pointer() const override;
    QObject *qobject() const override;
    bool isQObject() const override;
    void setAsQObjectDeleted();
    inline bool qobjectDeleted() const override { return !m_pointer; }
    void deleteNativeObject(JNIEnv *env, bool forced = false);
    void onDispose(JNIEnv *env) override;
    void onClean(JNIEnv *env) override;
    void invalidate(JNIEnv *env) override;
    virtual jobject getExtraSignal(JNIEnv * env, const QMetaMethod& method) const;
    QString describe() const override;
    void init(JNIEnv* env) override;
    void removeInterface(const std::type_info&) override;
    bool isSmartPointer() const override final;
private:
    QObject* m_pointer;
    friend class QtJambiLink;
};

class PointerToQObjectWithExtraSignalsLink : public PointerToQObjectLink{
protected:
    PointerToQObjectWithExtraSignalsLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMetaObject* metaObject, QObject *ptr, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException);
public:
    ~PointerToQObjectWithExtraSignalsLink() override = default;
    jobject getExtraSignal(JNIEnv * env, const QMetaMethod& method) const override;
private:
    mutable QHash<int,JObjectWrapper> m_extraSignals;
    friend QtJambiLink;
};

class PointerToPendingQObjectLink : public PointerToQObjectLink{
protected:
    PointerToPendingQObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMetaObject* metaObject, QObject *ptr, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException);
public:
    ~PointerToPendingQObjectLink() override = default;
    jobject getExtraSignal(JNIEnv * env, const QMetaMethod& method) const override;
    void init(JNIEnv* env) override;
private:
    mutable QHash<int,JObjectWrapper> m_extraSignals;
    friend QtJambiLink;
};

class SmartPointerLink : public QtJambiLink{
protected:
    SmartPointerLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                       LINK_NAME_ARG(const char *qt_name)
                       bool created_by_java, bool is_shell, JavaException& ocurredException);
public:
    ~SmartPointerLink() override;

    void setCppOwnership(JNIEnv *env) override final;
    void setSplitOwnership(JNIEnv *env) override final;

    bool isSmartPointer() const override final;
    virtual void* getSmartPointer() const = 0;
private:
    friend QtJambiLink;
};

class SmartPointerToObjectLink : public SmartPointerLink{
protected:
    SmartPointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                               LINK_NAME_ARG(const char* qt_name)
                               bool created_by_java, bool is_shell,
                               PtrOwnerFunction ownerFunction, void* ptr_shared_pointer,
                               SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException);
    friend QtJambiLink;
public:
    ~SmartPointerToObjectLink() override;
    virtual QObject *qobject() const override;
    virtual bool isQObject() const override;
    virtual void* getSmartPointer() const override;
    virtual void *pointer() const override;
    inline bool qobjectDeleted() const override { return false; }
    void deleteNativeObject(JNIEnv *env, bool forced = false);
    void onDispose(JNIEnv *env) override;
    void onClean(JNIEnv *env) override;
    void invalidate(JNIEnv *env) override;
    QString describe() const override;
    bool isPointerZeroed() const { return m_flags.testFlag(Flag::IsDeleteLater); }
    void setPointerZeroed(){ m_flags.setFlag(Flag::IsDeleteLater); }
    void init(JNIEnv* env) override;
public:
    inline PtrOwnerFunction ownerFunction() const {return m_owner_function;}
private:
    void* m_ptr_shared_pointer;
    SmartPointerDeleter m_shared_pointer_deleter;
    SmartPointerGetterFunction m_shared_pointer_getter;
    PtrOwnerFunction m_owner_function;
    friend QtJambiLink;
};

class ExtendedSmartPointerToObjectLink : public SmartPointerToObjectLink, public ExtendedLink{
protected:
    ExtendedSmartPointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                             LINK_NAME_ARG(const char* qt_name)
                             bool created_by_java, bool is_shell, const QObject* extension,
                             PtrOwnerFunction ownerFunction, void* ptr_shared_pointer,
                             SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException);
    friend QtJambiLink;
public:
    inline bool hasExtension()const override{return !m_extension.isNull(); }
private:
    QPointer<const QObject> m_extension;
};

class QSharedPointerToContainerLink : public SmartPointerToObjectLink{
protected:
    QSharedPointerToContainerLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                                  LINK_NAME_ARG(const char* qt_name)
                                  bool created_by_java, bool is_shell,
                                  PtrOwnerFunction ownerFunction, void* ptr_shared_pointer,
                                  SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter,
                                  AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    ~QSharedPointerToContainerLink() override;
public:
    inline AbstractContainerAccess* containerAccess() const override { return m_containerAccess; }
private:
    AbstractContainerAccess *const m_containerAccess;
    friend QtJambiLink;
};

class QSharedPointerToQObjectLink : public SmartPointerLink{
protected:
    QSharedPointerToQObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj, QObject* object, bool created_by_java, bool is_shell, void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, const QMetaObject* superTypeForCustomMetaObject, JavaException& ocurredException);
public:
    ~QSharedPointerToQObjectLink() override;
    virtual QObject *qobject() const override;
    virtual bool isQObject() const override;
    virtual void* getSmartPointer() const override;
    virtual void *pointer() const override;
    void deleteNativeObject(JNIEnv *env, bool forced = false);
    void setAsQObjectDeleted();
    void removePointerContainer();
    inline bool qobjectDeleted() const override { return !m_pointerContainer; }
    void onDispose(JNIEnv *env) override;
    void onClean(JNIEnv *env) override;
    void invalidate(JNIEnv *env) override;
    QString describe() const override;
    void removeInterface(const std::type_info&) override;
    bool isDeleteLater() const { return m_flags.testFlag(Flag::IsDeleteLater); }
    jobject getExtraSignal(JNIEnv * env, const QMetaMethod& method) const;
private:
    void setDeleteLater(){ m_flags.setFlag(Flag::IsDeleteLater); }
    PointerContainer* m_pointerContainer;
    friend QtJambiLink;
};

class PointerToQObjectInterfaceLink : public PointerToQObjectLink{
protected:
    PointerToQObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces, const QMetaObject* metaObject, QObject *ptr, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException);
public:
    ~PointerToQObjectInterfaceLink() override;
    void unregisterOffsets() override;
    void addInterface(const std::type_info& qtInterfaceType) override;
    void removeInterface(const std::type_info& qtInterfaceType) override;
    bool isInterfaceAvailable(const std::type_info& qtInterfaceType) const override;
    void* typedPointer(const std::type_info& qtType) const override;
    bool isMultiInheritanceType() const override;
private:
    QMap<size_t, uint> m_interfaceOffsets;
    QSet<size_t> m_availableInterfaces;
    QMap<size_t, QSet<const std::type_info*>> m_inheritedInterfaces;
    friend QtJambiLink;
};

class PointerToPendingQObjectInterfaceLink : public PointerToQObjectInterfaceLink{
protected:
    PointerToPendingQObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces, const QMetaObject* metaObject, QObject *ptr, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException);
public:
    ~PointerToPendingQObjectInterfaceLink() override = default;
    jobject getExtraSignal(JNIEnv * env, const QMetaMethod& method) const override;
    void init(JNIEnv* env) override;
private:
    mutable QHash<int,JObjectWrapper> m_extraSignals;
    friend QtJambiLink;
};

class PointerToQObjectInterfaceWithExtraSignalsLink : public PointerToQObjectInterfaceLink{
protected:
    PointerToQObjectInterfaceWithExtraSignalsLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces, const QMetaObject* metaObject, QObject *ptr, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException);
public:
    ~PointerToQObjectInterfaceWithExtraSignalsLink() override = default;
    jobject getExtraSignal(JNIEnv * env, const QMetaMethod& method) const override;
private:
    friend QtJambiLink;
    friend class PointerToQObjectInterfaceWithDetectedExtraSignalsLink;
    mutable QHash<int,JObjectWrapper> m_extraSignals;
};

class SmartPointerToObjectInterfaceLink : public SmartPointerToObjectLink{
private:
    SmartPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                        LINK_NAME_ARG(const char* qt_name)
                                        bool created_by_java, bool is_shell,
                                        PtrOwnerFunction ownerFunction, void* ptr_shared_pointer,
                                        SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException);
    INTERFACE_DECL(SmartPointerToObjectInterfaceLink)
};

class QSharedPointerToContainerInterfaceLink : public QSharedPointerToContainerLink{
private:
    QSharedPointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                        LINK_NAME_ARG(const char* qt_name)
                                        bool created_by_java, bool is_shell,
                                        PtrOwnerFunction ownerFunction, void* ptr_shared_pointer,
                                        SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    INTERFACE_DECL(QSharedPointerToContainerInterfaceLink)
};

class QSharedPointerToQObjectInterfaceLink : public QSharedPointerToQObjectLink{
private:
    QSharedPointerToQObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces, QObject* object, bool created_by_java, bool is_shell, void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, const QMetaObject* superTypeForCustomMetaObject, JavaException& ocurredException);
    INTERFACE_DECL(QSharedPointerToQObjectInterfaceLink)
};

class QtJambiDestructor : public QObject
{

public:
    QtJambiDestructor(const QPointer<const QObject>& parent, const QSharedPointer<QtJambiLink>& link, void *pointer, PtrDeleterFunction deleter_function, bool isShell);
    ~QtJambiDestructor() override;
    bool event(QEvent * e) override;
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
    const char* qtTypeName() const;
#endif
    bool isDebugMessagingDisabled() const;

private:
    QPointer<const QObject> m_parent;
    QWeakPointer<QtJambiLink> m_link;
    void * m_pointer;
    PtrDeleterFunction m_deleter_function;
    bool m_isShell;
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
    const char* m_qtTypeName;
#endif
};

class QtJambiMetaTypeDestructor : public QObject
{

public:
    QtJambiMetaTypeDestructor(const QPointer<const QObject>& parent, const QSharedPointer<QtJambiLink>& link, void *pointer, const QMetaType& meta_type);
    ~QtJambiMetaTypeDestructor() override;
    bool event(QEvent * e) override;
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
    const char* qtTypeName() const;
#endif
    bool isDebugMessagingDisabled() const;
private:
    QPointer<const QObject> m_parent;
    QWeakPointer<QtJambiLink> m_link;
    void * m_pointer;
    QMetaType m_meta_type;
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
    const char* m_qtTypeName;
#endif
};

class QtJambiLinkScope : public QtJambiScope{
public:
    QtJambiLinkScope(const QSharedPointer<QtJambiLink>& _link);
    Q_DISABLE_COPY_MOVE(QtJambiLinkScope)

    QtJambiLink * link() const;
};

#endif // QTJAMBILINK_P_H
