/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include <QtCore/QEvent>
#include <QtCore/QList>
#include <QtCore/QRunnable>
#include "registryutil_p.h"
#include "objectdata.h"

enum class QtJambiNativeID : jlong;

#if defined(QTJAMBI_DEBUG_TOOLS) || !defined(QT_NO_DEBUG)
#define QTJAMBI_INCREASE_COUNTER_THIS(counter) TestAPI::increaseDebugCounter(TestAPI::DebugCounters::counter, QtJambiLink::qtTypeName());
#define QTJAMBI_INCREASE_COUNTER(counter, _this) if(_this) TestAPI::increaseDebugCounter(TestAPI::DebugCounters::counter, _this->qtTypeName());
#define QTJAMBI_INCREASE_COUNTER2(counter, qtTypeName) TestAPI::increaseDebugCounter(TestAPI::DebugCounters::counter, qtTypeName);
namespace TestAPI{
enum class DebugCounters;
void increaseDebugCounter(DebugCounters debugCounter, const char*className);
}
#else
#define QTJAMBI_INCREASE_COUNTER_THIS(counter)
#define QTJAMBI_INCREASE_COUNTER(counter, _this)
#define QTJAMBI_INCREASE_COUNTER2(counter, qtTypeName)
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

typedef void (*FinalizationExecutor)(JNIEnv* env, void* data);
typedef void (*FinalizationDeleter)(JNIEnv* env, void* data);

template<template<typename> class SmartPointer>
class SmartPointerToQObjectLink;
template<template<typename> class SmartPointer>
class SmartPointerToObjectLink;

class SuperTypeInfos;

typedef std::function<void(JNIEnv*, jobject, jvalue *)> CallEmitMethod;

class QtJambiLink;

struct ValueOwnerUserData : public QtJambiObjectData
{
    ValueOwnerUserData(const QObject* object);
    ~ValueOwnerUserData() override;
    void addDeleter(const QtJambiUtils::Runnable& deleter);
    void addDeleter(QtJambiUtils::Runnable&& deleter);
    inline const QPointer<const QObject>& pointer() const { return m_object; }
    QTJAMBI_OBJECTUSERDATA_ID_DECL
private:
    Q_DISABLE_COPY_MOVE(ValueOwnerUserData)
    QPointer<const QObject> m_object;
    QList<QtJambiUtils::Runnable> m_deleters;
};

struct DependencyManagerUserData : public QtJambiObjectData
{
    DependencyManagerUserData();
    ~DependencyManagerUserData() override;
    void addFinalization(void* data, FinalizationExecutor executor, FinalizationDeleter deleter);
    void addFinalization(JNIEnv* env, jobject data, FinalizationExecutor executor, FinalizationDeleter deleter);
    void removeFinalization(JNIEnv* env, void* data, bool execute = false);
    void removeFinalization(JNIEnv* env, jobject data, bool execute = false);
    bool hasDependencies() const;
    void invalidateDependentObjects(QObject* obj, JNIEnv* env);
    void clear(QObject* obj, JNIEnv* env);
    void clear(QObject* obj);
    void addDependentObject(const QSharedPointer<QtJambiLink>& dependent);
    void removeDependentObject(const QSharedPointer<QtJambiLink>& dependent);
    static DependencyManagerUserData* instance(const QObject* object, bool forceConstruction = true);
    QTJAMBI_OBJECTUSERDATA_ID_DECL
private:
    void addConDestroyedObject(const QSharedPointer<QtJambiLink>& dependent);
    void removeConDestroyedObject(const QSharedPointer<QtJambiLink>& dependent);
    Q_DISABLE_COPY_MOVE(DependencyManagerUserData)
    void clear(JNIEnv* env,
               QSet<QWeakPointer<QtJambiLink>>& dependentObjects,
               QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>>& finalizations,
               QSet<QWeakPointer<QtJambiLink>>& qpropertyObjects);
    QSet<QWeakPointer<QtJambiLink>> m_dependentObjects;
    QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>> m_finalizations;
    QHash<jint,void*> m_finalizationData;
    QSet<QWeakPointer<QtJambiLink>> m_condestroyedObjects;
    friend QtJambiLink;
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
    void clear(JNIEnv * env);

    QTJAMBI_OBJECTUSERDATA_ID_DECL
    static QReadWriteLock* lock();

private:
    inline void resetLink(QWeakPointer<QtJambiLink>&& link) {m_link = std::move(link);}
    Q_DISABLE_COPY_MOVE(QtJambiLinkUserData)
    QWeakPointer<QtJambiLink> m_link;
    friend class QtJambiLink;
    friend struct QtJambiThreadLinkUserData;
    friend struct QtJambiMetaObjectThreadLinkUserData;
};

struct QtJambiMetaObjectLinkUserData : public QtJambiLinkUserData{
    QtJambiMetaObjectLinkUserData(const QWeakPointer<QtJambiLink> & link, const QMetaObject* metaObject);
    inline const QMetaObject* metaObject() const override {return m_metaObject; }
private:
    const QMetaObject* m_metaObject;
    Q_DISABLE_COPY_MOVE(QtJambiMetaObjectLinkUserData)
};

struct ThreadLinkUserDataImpl{
private:
    ThreadLinkUserDataImpl(QThread* thread);
    void clear(QSharedPointer<QtJambiLink>&& link);
    Qt::HANDLE m_threadId;
    Q_DISABLE_COPY_MOVE(ThreadLinkUserDataImpl)
    friend struct QtJambiThreadLinkUserData;
    friend struct QtJambiMetaObjectThreadLinkUserData;
};

struct QtJambiThreadLinkUserData : public QtJambiLinkUserData, public ThreadLinkUserDataImpl{
    QtJambiThreadLinkUserData(QThread* thread, const QWeakPointer<QtJambiLink> & link);
    ~QtJambiThreadLinkUserData() override;
private:
    Q_DISABLE_COPY_MOVE(QtJambiThreadLinkUserData)
};

struct QtJambiMetaObjectThreadLinkUserData : public QtJambiMetaObjectLinkUserData, public ThreadLinkUserDataImpl{
    QtJambiMetaObjectThreadLinkUserData(QThread* thread, const QWeakPointer<QtJambiLink> & link, const QMetaObject* metaObject);
    ~QtJambiMetaObjectThreadLinkUserData() override;
private:
    Q_DISABLE_COPY_MOVE(QtJambiMetaObjectThreadLinkUserData)
};

class QtJambiLink{
public:
    enum class Ownership : quint8 {
        None =                      0x00000000,
        Java =                      0x00000001,   // Weak ref to java object, deleteNativeObject deletes c++ object
        Cpp =                       0x00000002,    // Strong ref to java object until c++ object is deleted, deleteNativeObject does *not* delete c++ obj.
        Split =                     0x00000004  // Weak ref to java object, deleteNativeObject does *not* delete c++ object. Only for objects not created by Java.
    };

    enum class Flag : quint32{
        None = 0,
        JavaOwnership = quint32(Ownership::Java),   // Weak ref to java object, deleteNativeObject deletes c++ object
        CppOwnership = quint32(Ownership::Cpp),    // Strong ref to java object until c++ object is deleted, deleteNativeObject does *not* delete c++ obj.
        SplitOwnership = quint32(Ownership::Split),  // Weak ref to java object, deleteNativeObject does *not* delete c++ object. Only for objects not created by Java.
        OwnershipMask = JavaOwnership | CppOwnership | SplitOwnership,
        IsInitialized =             0x00000008,
        JavaObjectIsReleased =      0x00000010,
        IsOffsetsUnregistered =     0x00000020,
        CreatedByJava =             0x00000040,
        CreatedByQml =              0x00000080,
        IsShell =                   0x00000100,
        IsJavaLinkDetached =        0x00000200,
        HasBeenCleaned =            0x00000400,
        BlockDeletion =             0x00000800,
        IsDisposed =                0x00001000,
        IsInDestructor =            0x00002000,
        IsDeleteLater =             0x00004000,
        NoNativeDeletion =          0x00008000,
        IsDependent =               0x00010000,
        HasDependencies =           0x00020000,
        IsManaged =                 0x00040000,
        HasDisposedSignal =         0x00080000,
        IsPendingValueOwner =       0x00100000,
        NoDebugMessaging =          0x00200000,
        IsPointerRegistered =       0x00400000,
        NeedsReferenceCounting =    0x00800000,
        shared_ptr =                0x01000000,
        QSharedPointer =            0x02000000,
        weak_ptr =                  0x04000000,
        QWeakPointer =              0x08000000,
        SmartPointerMask =          0x0f000000,
        IsQThread =                 0x10000000,
        IsAdoptedThread =           0x20000000,
        NoGlobalRef =               0x40000000,
#if defined(QTJAMBI_DEBUG_TOOLS)
        IsListed =                  0x80000000,
#endif
    };
    typedef QFlags<Flag> Flags;

    static QSharedPointer<QtJambiLink> createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool isCopy, PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess);
    static QSharedPointer<QtJambiLink> createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool isCopy, AbstractContainerAccess* containerAccess);
    static QSharedPointer<QtJambiLink> createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool isCopy, PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess,
                                                                        QtJambiLink::Ownership ownership);
    static QSharedPointer<QtJambiLink> createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool isCopy, PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess,
                                                                        QtJambiLink::Ownership ownership,
                                                                        const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool isCopy, AbstractContainerAccess* containerAccess, QtJambiLink::Ownership ownership);
    static QSharedPointer<QtJambiLink> createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool isCopy, AbstractContainerAccess* containerAccess, QtJambiLink::Ownership ownership,
                                                                        const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, QtJambiLink::Ownership ownership);
    static QSharedPointer<QtJambiLink> createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, QtJambiLink::Ownership ownership);
    static QSharedPointer<QtJambiLink> createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool is_shell, QtJambiLink::Ownership ownership);
    static QSharedPointer<QtJambiLink> createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr, const QMetaType& metaType,
                                                                  bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, QtJambiLink::Ownership ownership);
    static QSharedPointer<QtJambiLink> createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr, const QMetaType& metaType,
                                                                  bool created_by_java, bool is_shell, QtJambiLink::Ownership ownership);
    static QSharedPointer<QtJambiLink> createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, QtJambiLink::Ownership ownership,
                                                                        const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, QtJambiLink::Ownership ownership,
                                                                        const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool is_shell, QtJambiLink::Ownership ownership,
                                                                        const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr, const QMetaType& metaType,
                                                                        bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, QtJambiLink::Ownership ownership,
                                                                        const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr, const QMetaType& metaType,
                                                                        bool created_by_java, bool is_shell, QtJambiLink::Ownership ownership,
                                                                        const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForDependentObject(JNIEnv *env, jobject java, void *ptr, const QMetaType& metaType,
                                                                           bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, const QObject* dependsOn, QtJambiLink::Ownership ownership);
    static QSharedPointer<QtJambiLink> createLinkForDependentObject(JNIEnv *env, jobject java, void *ptr, const QMetaType& metaType,
                                                                           bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, const QObject* dependsOn, QtJambiLink::Ownership ownership,
                                                                           const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForDependentObject(JNIEnv *env, jobject java, void *ptr,
                                                                           LINK_NAME_ARG(const char* qt_name)
                                                                           bool created_by_java, bool is_shell,
                                                                           PtrDeleterFunction destructor_function,
                                                                           const QObject* dependsOn, QtJambiLink::Ownership ownership);
    static QSharedPointer<QtJambiLink> createExtendedLinkForObject(JNIEnv *env, jobject java, void *ptr,
                                                                    LINK_NAME_ARG(const char* qt_name)
                                                                    bool created_by_java, bool is_shell,
                                                                    PtrDeleterFunction destructor_function,
                                                                    const QObject* extension, QtJambiLink::Ownership ownership);
    static QSharedPointer<QtJambiLink> createLinkForDependentObject(JNIEnv *env, jobject java, void *ptr, const QMetaType& metaType,
                                                                           bool created_by_java, bool is_shell, const QObject* dependsOn, QtJambiLink::Ownership ownership);
    static QSharedPointer<QtJambiLink> createLinkForDependentObject(JNIEnv *env, jobject java, void *ptr, const QMetaType& metaType,
                                                                           bool created_by_java, bool is_shell, const QObject* dependsOn, QtJambiLink::Ownership ownership,
                                                                           const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForOwnedObject(JNIEnv *env, jobject java, void *ptr,
                                                                       LINK_NAME_ARG(const char* qt_name)
                                                                       QtJambiNativeID owner, PtrDeleterFunction destructor_function);
    static QSharedPointer<QtJambiLink> createLinkForOwnedObject(JNIEnv *env, jobject java, void *ptr,
                                                                       LINK_NAME_ARG(const char* qt_name)
                                                                       QtJambiNativeID owner);
    static QSharedPointer<QtJambiLink> createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                       LINK_NAME_ARG(const char* qt_name)
                                                                       QtJambiNativeID owner, AbstractContainerAccess* containerAccess);
    static QSharedPointer<QtJambiLink> createLinkForNativeObject(JNIEnv *env, jobject java, void *ptr,
                                                                       LINK_NAME_ARG(const char* qt_name)
                                                                       QtJambiNativeID owner, PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess);
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                LINK_NAME_ARG(const char* qt_name)
                                                                                bool created_by_java, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                const QSharedPointer<char>& smartPointer);
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                LINK_NAME_ARG(const char* qt_name)
                                                                                bool created_by_java, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                const std::shared_ptr<char>& smartPointer);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                void* pointer,
                                                                                QSharedPointer<char>& smartPointer,
                                                                                const QMetaType& elementMetaType);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                void* pointer,
                                                                                QWeakPointer<char>& smartPointer,
                                                                                const QMetaType& elementMetaType);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                void* pointer,
                                                                                std::shared_ptr<char>& smartPointer,
                                                                                const QMetaType& elementMetaType);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                    bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                void* pointer,
                                                                                std::weak_ptr<char>& smartPointer,
                                                                                const QMetaType& elementMetaType);

    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                LINK_NAME_ARG(const char* qt_name)
                                                                                bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                void* pointer,
                                                                                QSharedPointer<char>& smartPointer,
                                                                                PtrDeleterFunction deleterFunction);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                LINK_NAME_ARG(const char* qt_name)
                                                                                bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                void* pointer,
                                                                                std::shared_ptr<char>& smartPointer,
                                                                                PtrDeleterFunction deleterFunction);

    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                LINK_NAME_ARG(const char* qt_name)
                                                                                bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                void* pointer,
                                                                                QWeakPointer<char>& smartPointer,
                                                                                PtrDeleterFunction deleterFunction);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                LINK_NAME_ARG(const char* qt_name)
                                                                                bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                void* pointer,
                                                                                std::weak_ptr<char>& smartPointer,
                                                                                PtrDeleterFunction deleterFunction);

    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                LINK_NAME_ARG(const char* qt_name)
                                                                                bool created_by_java, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                const QSharedPointer<char>& smartPointer,
                                                                                const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                void* pointer,
                                                                                QSharedPointer<char>& smartPointer,
                                                                                const QMetaType& elementMetaType,
                                                                                const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                void* pointer,
                                                                                std::shared_ptr<char>& smartPointer,
                                                                                const QMetaType& elementMetaType,
                                                                                const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                void* pointer,
                                                                                QWeakPointer<char>& smartPointer,
                                                                                const QMetaType& elementMetaType,
                                                                                const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                void* pointer,
                                                                                std::weak_ptr<char>& smartPointer,
                                                                                const QMetaType& elementMetaType,
                                                                                const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                LINK_NAME_ARG(const char* qt_name)
                                                                                bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                void* pointer,
                                                                                QSharedPointer<char>& smartPointer,
                                                                                PtrDeleterFunction deleterFunction,
                                                                                const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                LINK_NAME_ARG(const char* qt_name)
                                                                                bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                void* pointer,
                                                                                QWeakPointer<char>& smartPointer,
                                                                                PtrDeleterFunction deleterFunction,
                                                                                const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                LINK_NAME_ARG(const char* qt_name)
                                                                                bool created_by_java, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                const std::shared_ptr<char>& smartPointer,
                                                                                const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                LINK_NAME_ARG(const char* qt_name)
                                                                                bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                void* pointer,
                                                                                std::shared_ptr<char>& smartPointer,
                                                                                PtrDeleterFunction deleterFunction,
                                                                                const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                LINK_NAME_ARG(const char* qt_name)
                                                                                bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                void* pointer,
                                                                                std::weak_ptr<char>& smartPointer,
                                                                                PtrDeleterFunction deleterFunction,
                                                                                const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createExtendedLinkForSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                                        bool created_by_java, bool is_shell,
                                                                                        const QObject* extension,
                                                                                        PtrOwnerFunction ownerFunction,
                                                                                        const QSharedPointer<char>& smartPointer);
    static QSharedPointer<QtJambiLink> createExtendedLinkForSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                                        bool created_by_java, bool is_shell,
                                                                                        const QObject* extension,
                                                                                        PtrOwnerFunction ownerFunction,
                                                                                        const std::shared_ptr<char>& smartPointer);
    static QSharedPointer<QtJambiLink> createLinkForNativeQObject(JNIEnv *env, jobject& java, jclass clazz, QObject *object);
    static QSharedPointer<QtJambiLink> createLinkForNativeQObject(JNIEnv *env, jobject& java, QObject *object, bool isQThread);
    static QSharedPointer<QtJambiLink> createLinkForNativeQObject(JNIEnv *env, jobject& java, QObject *object){
        return createLinkForNativeQObject(env, java, object, false);
    }
    static QSharedPointer<QtJambiLink> createLinkForNativeQObject(JNIEnv *env, jobject& java, QThread *object){
        return createLinkForNativeQObject(env, java, object, true);
    }
    static QSharedPointer<QtJambiLink> createLinkForNativeQObject(JNIEnv *env, jobject& java, QObject *object, bool isQThread, /*nonnull*/ const QMetaObject* superTypeForCustomMetaObject);
    static QSharedPointer<QtJambiLink> createLinkForNativeQObject(JNIEnv *env, jobject& java, QObject *object, bool isQThread, const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForNativeQObject(JNIEnv *env, jobject& java, QObject *object, bool isQThread, const InterfaceOffsetInfo& interfaceOffsetInfo, /*nonnull*/ const QMetaObject* superTypeForCustomMetaObject);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, QObject* object, bool isQThread, std::weak_ptr<QObject>& smartPointer, const QMetaObject* superTypeForCustomMetaObject = nullptr);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, QObject* object, bool isQThread, std::weak_ptr<QObject>& smartPointer,
                                                                                 const InterfaceOffsetInfo& interfaceOffsetInfo, const QMetaObject* superTypeForCustomMetaObject = nullptr);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, QObject* object, bool isQThread, QWeakPointer<QObject>& smartPointer, const QMetaObject* superTypeForCustomMetaObject = nullptr);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, QObject* object, bool isQThread, QWeakPointer<QObject>& smartPointer,
                                                                                 const InterfaceOffsetInfo& interfaceOffsetInfo, const QMetaObject* superTypeForCustomMetaObject = nullptr);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, QObject* object, bool isQThread, std::shared_ptr<QObject>& smartPointer, const QMetaObject* superTypeForCustomMetaObject = nullptr);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, QObject* object, bool isQThread, std::shared_ptr<QObject>& smartPointer,
                                                                                 const InterfaceOffsetInfo& interfaceOffsetInfo, const QMetaObject* superTypeForCustomMetaObject = nullptr);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, QObject* object, bool isQThread, QSharedPointer<QObject>& smartPointer, const QMetaObject* superTypeForCustomMetaObject = nullptr);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, QObject* object, bool isQThread, QSharedPointer<QObject>& smartPointer,
                                                                                 const InterfaceOffsetInfo& interfaceOffsetInfo, const QMetaObject* superTypeForCustomMetaObject = nullptr);
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, bool is_shell, const std::shared_ptr<QObject>& smartPointer, bool isQThread);
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, bool is_shell, const std::shared_ptr<QObject>& smartPointer, bool isQThread, /*nonnull*/const QMetaObject* superTypeForCustomMetaObject);
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, bool is_shell, const std::shared_ptr<QObject>& smartPointer, bool isQThread,
                                                                                 const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, bool is_shell, const std::shared_ptr<QObject>& smartPointer, bool isQThread,
                                                                                 const InterfaceOffsetInfo& interfaceOffsetInfo, /*nonnull*/const QMetaObject* superTypeForCustomMetaObject);
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, bool is_shell, const QSharedPointer<QObject>& smartPointer, bool isQThread);
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, bool is_shell, const QSharedPointer<QObject>& smartPointer, bool isQThread, /*nonnull*/const QMetaObject* superTypeForCustomMetaObject);
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, bool is_shell, const QSharedPointer<QObject>& smartPointer, bool isQThread,
                                                                                 const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, bool is_shell, const QSharedPointer<QObject>& smartPointer, bool isQThread,
                                                                                 const InterfaceOffsetInfo& interfaceOffsetInfo, /*nonnull*/const QMetaObject* superTypeForCustomMetaObject);
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                                   const QSharedPointer<char>& smartPointer, AbstractContainerAccess* containerAccess);
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                                   const QSharedPointer<char>& smartPointer, AbstractContainerAccess* containerAccess,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                                   const std::shared_ptr<char>& smartPointer, AbstractContainerAccess* containerAccess);
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                                   const std::shared_ptr<char>& smartPointer, AbstractContainerAccess* containerAccess,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                                   QtJambiLink::Ownership ownership,
                                                                                   void* pointer, QSharedPointer<char>& smartPointer, AbstractContainerAccess* containerAccess);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                                   QtJambiLink::Ownership ownership,
                                                                                   void* pointer, QSharedPointer<char>& smartPointer, AbstractContainerAccess* containerAccess,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                                   QtJambiLink::Ownership ownership,
                                                                                   void* pointer, std::shared_ptr<char>& smartPointer, AbstractContainerAccess* containerAccess);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                                   QtJambiLink::Ownership ownership,
                                                                                   void* pointer, std::shared_ptr<char>& smartPointer, AbstractContainerAccess* containerAccess,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                                   QtJambiLink::Ownership ownership,
                                                                                   void* pointer, QWeakPointer<char>& smartPointer, AbstractContainerAccess* containerAccess);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                                   QtJambiLink::Ownership ownership,
                                                                                   void* pointer, QWeakPointer<char>& smartPointer, AbstractContainerAccess* containerAccess,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                                   QtJambiLink::Ownership ownership,
                                                                                   void* pointer, std::weak_ptr<char>& smartPointer, AbstractContainerAccess* containerAccess);
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                                   QtJambiLink::Ownership ownership,
                                                                                   void* pointer, std::weak_ptr<char>& smartPointer, AbstractContainerAccess* containerAccess,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo);
    static QSharedPointer<QtJambiLink> createLinkForNewQObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject java, const std::type_info& typeId, const QMetaObject* metaObject, QObject *object, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool isDeclarativeCall, bool is_shell, bool hasCustomMetaObject, JavaException& ocurredException);
    static QSharedPointer<QtJambiLink> createLinkForNewQObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject java, const std::type_info& typeId, const QMetaObject* metaObject, QThread *object, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool isDeclarativeCall, bool is_shell, bool hasCustomMetaObject, JavaException& ocurredException);
    static QSharedPointer<QtJambiLink> createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject java, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, const QMetaType& metaType, bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    static QSharedPointer<QtJambiLink> createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject java, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, const QMetaType& metaType, bool created_by_java, bool is_shell, JavaException& ocurredException);
    static QSharedPointer<QtJambiLink> createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject java, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    static QSharedPointer<QtJambiLink> createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject java, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, JavaException& ocurredException);
    static QSharedPointer<QtJambiLink> createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject java, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, JavaException& ocurredException);
    static QSharedPointer<QtJambiLink> createLinkForNewObject(JNIEnv *env,
                                                                        jclass objectClass,
                                                                        jobject nativeLink,
                                                                        jobject java,
                                                                        const std::type_info& typeId,
                                                                        void *ptr,
                                                                        const SuperTypeInfos* superTypeInfos,
                                                                        bool created_by_java,
                                                                        bool is_shell,
                                                                        AbstractContainerAccess* containerAccess,
                                                                        JavaException& ocurredException);
    static QSharedPointer<QtJambiLink> createLinkForNewObject(JNIEnv *env,
                                                                        jclass objectClass,
                                                                        jobject nativeLink,
                                                                        jobject java,
                                                                        const std::type_info& typeId,
                                                                        void *ptr,
                                                                        const SuperTypeInfos* superTypeInfos,
                                                                        bool created_by_java,
                                                                        bool is_shell,
                                                                        AbstractContainerAccess* containerAccess,
                                                                        PtrDeleterFunction destructor_function,
                                                                        JavaException& ocurredException);
    static QSharedPointer<QtJambiLink> createLinkForNewObject(JNIEnv *env,
                                                                        jclass objectClass,
                                                                        jobject nativeLink,
                                                                        jobject java,
                                                                        const std::type_info& typeId,
                                                                        void *ptr,
                                                                        const SuperTypeInfos* superTypeInfos,
                                                                        bool created_by_java,
                                                                        bool is_shell,
                                                                        AbstractContainerAccess* containerAccess,
                                                                        PtrDeleterFunction destructor_function,
                                                                        PtrOwnerFunction ownerFunction,
                                                                        JavaException& ocurredException);
    static QSharedPointer<QtJambiLink> fromNativeId(QtJambiNativeID native_id);
    static QSharedPointer<QtJambiLink> findLinkForQObject(const QObject *qobject);
    static QList<QSharedPointer<QtJambiLink>> findLinksForPointer(const void *ptr);
    static jobject findObjectForPointer(JNIEnv* env, jclass clazz, const void *ptr);
    static QSharedPointer<QtJambiLink> findLinkForJavaObject(JNIEnv *env, jobject java);
    static QSharedPointer<QtJambiLink> findLinkForJavaInterface(JNIEnv *env, jobject java);
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
    template<template<typename> class SmartPointer>
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, bool is_shell,
                                                                                 const SmartPointer<QObject>& smartPointer, bool isQThread);
    template<template<typename> class SmartPointer>
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, bool is_shell,
                                                                                 const SmartPointer<QObject>& smartPointer, bool isQThread,
                                                                                 const QMetaObject* superTypeForCustomMetaObject);
    template<template<typename> class SmartPointer>
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, bool is_shell,
                                                                                 const SmartPointer<QObject>& smartPointer, bool isQThread,
                                                                                 const InterfaceOffsetInfo& interfaceOffsetInfo);
    template<template<typename> class SmartPointer>
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, bool is_shell,
                                                                                 const SmartPointer<QObject>& smartPointer, bool isQThread,
                                                                                 const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                 const QMetaObject* superTypeForCustomMetaObject);
    template<template<typename> class SmartPointer>
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                 QObject* object, bool isQThread, SmartPointer<QObject>& smartPointer,
                                                                                 const QMetaObject* superTypeForCustomMetaObject = nullptr);
    template<template<typename> class SmartPointer>
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                 QObject* object, bool isQThread, SmartPointer<QObject>& smartPointer,
                                                                                 const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                 const QMetaObject* superTypeForCustomMetaObject = nullptr);
    template<template<typename> class SmartPointer>
    static QSharedPointer<QtJambiLink> createExtendedLinkForSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                                        bool created_by_java, bool is_shell,
                                                                                        const QObject* extension,
                                                                                        PtrOwnerFunction ownerFunction,
                                                                                        const SmartPointer<char>& smartPointer);
    template<template<typename> class SmartPointer>
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                LINK_NAME_ARG(const char* qt_name)
                                                                                bool created_by_java, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                const SmartPointer<char>& smartPointer,
                                                                                const InterfaceOffsetInfo& interfaceOffsetInfo);
    template<template<typename> class SmartPointer>
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                void* pointer,
                                                                                SmartPointer<char>& smartPointer,
                                                                                const QMetaType& elementMetaType,
                                                                                const InterfaceOffsetInfo& interfaceOffsetInfo);
    template<template<typename> class SmartPointer>
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                LINK_NAME_ARG(const char* qt_name)
                                                                                bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                void* pointer,
                                                                                SmartPointer<char>& smartPointer,
                                                                                PtrDeleterFunction deleterFunction,
                                                                                const InterfaceOffsetInfo& interfaceOffsetInfo);
    template<template<typename> class SmartPointer>
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                LINK_NAME_ARG(const char* qt_name)
                                                                                bool created_by_java, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                const SmartPointer<char>& smartPointer);
    template<template<typename> class SmartPointer>
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                void* pointer,
                                                                                SmartPointer<char>& smartPointer,
                                                                                const QMetaType& elementMetaType);
    template<template<typename> class SmartPointer>
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                LINK_NAME_ARG(const char* qt_name)
                                                                                bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                PtrOwnerFunction ownerFunction,
                                                                                void* pointer,
                                                                                SmartPointer<char>& smartPointer,
                                                                                PtrDeleterFunction deleterFunction);
    template<template<typename> class SmartPointer>
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                                   const SmartPointer<char>& smartPointer,
                                                                                   AbstractContainerAccess* containerAccess);
    template<template<typename> class SmartPointer>
    static QSharedPointer<QtJambiLink> createLinkForSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                                   const SmartPointer<char>& smartPointer,
                                                                                   AbstractContainerAccess* containerAccess,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo);
    template<template<typename> class SmartPointer>
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                                   QtJambiLink::Ownership ownership,
                                                                                   void* pointer, SmartPointer<char>& smartPointer,
                                                                                   AbstractContainerAccess* containerAccess);
    template<template<typename> class SmartPointer>
    static QSharedPointer<QtJambiLink> createLinkForNewSmartPointerToObject(JNIEnv *env, jobject java,
                                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                                   QtJambiLink::Ownership ownership,
                                                                                   void* pointer, SmartPointer<char>& smartPointer,
                                                                                   AbstractContainerAccess* containerAccess,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo);
public:
    QtJambiLink(JNIEnv *env, jobject nativeLink,
                    LINK_NAME_ARG(const char* qt_name)
                    bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException);
protected:
    virtual ~QtJambiLink();
public:
    bool hasDisposedSignal() const;
    void setHasDisposedSignal();
    jobject getJavaObjectLocalRef(JNIEnv *env) const;
    bool createdByJava() const;
    bool createdByQml() const;
    bool isShell() const;
    bool needsReferenceCounting() const;
    void setNeedsReferenceCounting(bool);
    bool isDebugMessagingDisabled()const;
    void disableDebugMessaging();
    virtual void setCppOwnership(JNIEnv *env);
    virtual void setSplitOwnership(JNIEnv *env);
    virtual void setJavaOwnership(JNIEnv *env);
    virtual void setDefaultOwnership(JNIEnv *env);
    virtual Ownership ownership() const;
    void reset(JNIEnv *env);
    void detachJavaLink(JNIEnv *env);
    virtual void invalidate(JNIEnv *env) = 0;
    virtual QObject *qobject() const = 0;
    virtual bool isQObject() const = 0;
    bool isQThread() const;
    void setIsQThread(bool n);
    bool isAdoptedThread() const;
    void setAdoptedThread(bool n);
    virtual bool qobjectDeleted() const = 0;
    virtual bool isSmartPointer() const = 0;
    virtual QString describe() const = 0;
    virtual bool isMultiInheritanceType() const;

    bool isInDestructor() const;
    QtJambiLink::Flag smartPointerType() const;

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
    inline virtual const InterfaceOffsetInfo* interfaceOffsetInfo() const { return nullptr; }
    inline virtual const QMetaType* metaType() const { return nullptr; }
    inline virtual PtrOwnerFunction ownerFunction() const{return nullptr;}
    inline virtual PtrDeleterFunction deleterFunction() const{return nullptr;}
    inline virtual jobject getExtraSignal(JNIEnv *, const QMetaMethod&) const{
        return nullptr;
    }

    virtual void *pointer() const = 0;
    virtual void addInterface(const std::type_info& qtInterfaceType);
    virtual void removeInterface(const std::type_info& qtInterfaceType);
    virtual bool isInterfaceAvailable(const std::type_info& qtInterfaceType) const;

    virtual void* typedPointer(const std::type_info& qtType) const;
    void addFinalization(void* data, FinalizationExecutor executor, FinalizationDeleter deleter);
    void addFinalization(JNIEnv * env, jobject data, FinalizationExecutor executor, FinalizationDeleter deleter);
    void removeFinalization(JNIEnv * env, void* data, bool execute = false);
    void removeFinalization(JNIEnv * env, jobject data, bool execute = false);
    void registerDependentObject(const QSharedPointer<QtJambiLink>& link);
    void unregisterDependentObject(const QSharedPointer<QtJambiLink>& link);
    void registerConDestroyedObject(JNIEnv * env, const QSharedPointer<QtJambiLink>& link);
    void unregisterConDestroyedObject(JNIEnv * env, const QSharedPointer<QtJambiLink>& link);
    static void registerDependentObject(const QObject* object, const QSharedPointer<QtJambiLink>& link);
    static void unregisterDependentObject(const QObject* object, const QSharedPointer<QtJambiLink>& link);
    static void registerConDestroyedObject(JNIEnv * env, const QObject* object, const QSharedPointer<QtJambiLink>& link);
    static void unregisterConDestroyedObject(JNIEnv * env, const QObject* object, const QSharedPointer<QtJambiLink>& link);
    static void addFinalization(const QObject* object, void* data, FinalizationExecutor executor, FinalizationDeleter deleter);
    static void addFinalization(const QObject* object, JNIEnv * env, jobject data, FinalizationExecutor executor, FinalizationDeleter deleter);
    static void removeFinalization(const QObject* object, JNIEnv * env, void* data, bool execute = false);
    static void removeFinalization(const QObject* object, JNIEnv * env, jobject data, bool execute = false);

#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
    const char* qtTypeName() const;
#endif
#if defined(QTJAMBI_DEBUG_TOOLS)
    const char* debugFlagsToString(char* buf)const;
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
    bool isDeleteLater() const;
    bool isCleanedUp() const;
    QSharedPointer<QtJambiLink> getStrongPointer() const;
    inline QWeakPointer<QtJambiLink> getWeakPointer() const { return getStrongPointer(); }
    inline operator QSharedPointer<QtJambiLink>() const { return getStrongPointer(); }
    inline operator QWeakPointer<QtJambiLink>() const { return getStrongPointer(); }
protected:
    void resetDataLink(QtJambiLinkUserData* data);
    void dispose();
    void setDeleteLater();
    void invalidateDependentObjects(JNIEnv *env);
    void clearAllDependencies(JNIEnv *env);
    void clearAllDependencies();
    void setInDestructor();
    virtual void unregisterOffsets();
    void registerPointer(void*);
    void unregisterPointer(void*);
    void unregisterPointers(const QVector<void*>& pointers);
private:
    struct LinkDeleter{
        void operator()(QtJambiLink*);
    };
    typedef QtSharedPointer::ExternalRefCountWithCustomDeleter<QtJambiLink, LinkDeleter> ExternalRefCountData;
    QtSharedPointer::ExternalRefCountData *const m_refCount;
protected:
    jobject m_nativeLink;
    Flags m_flags;
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
    const char* m_qtTypeName;
#endif
private:
    void disposeManaged(JNIEnv *env);
    friend class QtJambiShellImpl;
    friend struct ThreadLinkUserDataImpl;
    friend struct QtJambiLinkUserData;
    friend struct DependencyManagerUserData;
    friend class PointerToQObjectLink;
    friend class PointerContainerWithPendingExtraSignals;
    friend QtSharedPointer::ExternalRefCountWithCustomDeleter<QtJambiLink, QtSharedPointer::NormalDeleter>;
};

template<>
struct QtSharedPointer::ExternalRefCountWithCustomDeleter<QtJambiLink, QtSharedPointer::NormalDeleter>{
    typedef const void* DestroyerFn;
    static constexpr char safetyCheckDeleter = 0;
    static constexpr char deleter = 0;
    template<typename... Args>
    static inline ExternalRefCountData* create(QtJambiLink* link, Args...){
        Q_STATIC_ASSERT_X(sizeof...(Args)!=2, "Cannot create QSharedPointer<QtJambiLink>");
        return fromLink(link);
    }
    static ExternalRefCountData* fromLink(QtJambiLink*);
};

class PointerToObjectLink : public QtJambiLink{
protected:
    PointerToObjectLink(JNIEnv *env, jobject nativeLink,
                        LINK_NAME_ARG(const char* qt_name)
                        void *ptr, bool created_by_java, bool is_shell, JavaException& ocurredException);
    ~PointerToObjectLink() override;
public:
    void removeInterface(const std::type_info&) override;
    void *pointer() const override;
    QObject *qobject() const override;
    bool isQObject() const override;
    virtual void deleteNativeObject(JNIEnv *env = nullptr, bool forced = false);
    inline bool qobjectDeleted() const override { return false; }
    void onDispose(JNIEnv *env) override;
    void onClean(JNIEnv *env) override;
    void invalidate(JNIEnv *env) override;
    QString describe() const override;
    bool isSmartPointer() const override final;
    void init(JNIEnv* env) override;
protected:
    void *m_pointer;
    friend QtJambiLink;
};

class MetaTypedPointerToObjectLink : public PointerToObjectLink{
protected:
    MetaTypedPointerToObjectLink(JNIEnv *env, jobject nativeLink,
                                 void *ptr, const QMetaType& metaType,
                                 bool created_by_java, bool is_shell,
                                 JavaException& ocurredException);
public:
    void deleteNativeObject(JNIEnv *env, bool forced = false) override;
    inline const QMetaType* metaType() const override {return &m_meta_type;}
protected:
    QMetaType m_meta_type;
    friend QtJambiLink;
};

class DependentMetaTypedPointerToObjectLink : public MetaTypedPointerToObjectLink, public DependentLink{
protected:
    DependentMetaTypedPointerToObjectLink(JNIEnv *env, jobject nativeLink,
                                          void *ptr, const QMetaType& metaType,
                                          bool created_by_java, bool is_shell,
                                          JavaException& ocurredException);
    void setDependencyManager(DependencyManagerUserData*) override;
    void deleteNativeObject(JNIEnv *env, bool forced) override;
    void invalidate(JNIEnv *env) override;
private:
    DependencyManagerUserData* m_dependencyManager;
    friend QtJambiLink;
};

class PointerToContainerLink : public PointerToObjectLink{
protected:
    PointerToContainerLink(JNIEnv *env, jobject nativeLink,
                           LINK_NAME_ARG(const char* qt_name)
                           void *ptr, bool created_by_java, bool is_shell,
                           AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    ~PointerToContainerLink() override;
public:
    void init(JNIEnv* env) override;
    void deleteNativeObject(JNIEnv *env, bool forced = false) override;
    inline AbstractContainerAccess* containerAccess() const override { return m_containerAccess; }
private:
    AbstractContainerAccess * m_containerAccess;
    friend QtJambiLink;
    friend class DeletablePointerToContainerLink;
    friend class DeletableOwnedPointerToContainerLink;
};

class OwnedMetaTypedPointerToObjectLink : public MetaTypedPointerToObjectLink{
protected:
    OwnedMetaTypedPointerToObjectLink(JNIEnv *env, jobject nativeLink,
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
    OwnedDependentMetaTypedPointerToObjectLink(JNIEnv *env, jobject nativeLink,
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
    DeletableOwnedPointerToObjectLink(JNIEnv *env, jobject nativeLink,
                        LINK_NAME_ARG(const char* qt_name)
                        void *ptr, bool created_by_java, bool is_shell,
                        PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
public:
    inline PtrDeleterFunction deleterFunction() const override {return m_deleter_function;}
    inline PtrOwnerFunction ownerFunction() const override {return m_owner_function;}
private:
    void init(JNIEnv* env) override;
    void deleteNativeObject(JNIEnv *env, bool forced = false) override;
    PtrOwnerFunction m_owner_function;
    PtrDeleterFunction m_deleter_function;
    friend QtJambiLink;
};

class DeletablePointerToObjectLink : public PointerToObjectLink{
public:
    inline PtrDeleterFunction deleterFunction() const override {return m_deleter_function;}
protected:
    DeletablePointerToObjectLink(JNIEnv *env, jobject nativeLink,
                        LINK_NAME_ARG(const char* qt_name)
                        void *ptr, bool created_by_java, bool is_shell,
                        PtrDeleterFunction destructor_function, JavaException& ocurredException);
    void deleteNativeObject(JNIEnv *env, bool forced = false) override;
private:
    PtrDeleterFunction m_deleter_function;
    friend QtJambiLink;
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
    ExtendedDeletablePointerToObjectLink(JNIEnv *env, jobject nativeLink,
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
    DependentDeletablePointerToObjectLink(JNIEnv *env, jobject nativeLink,
                                 LINK_NAME_ARG(const char* qt_name)
                                 void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, JavaException& ocurredException);
    void setDependencyManager(DependencyManagerUserData*) override;
    void deleteNativeObject(JNIEnv *env, bool forced) override;
    void invalidate(JNIEnv *env) override;
private:
    DependencyManagerUserData* m_dependencyManager;
    friend QtJambiLink;
};

class OwnedPointerToContainerLink : public PointerToContainerLink{
protected:
    OwnedPointerToContainerLink(JNIEnv *env, jobject nativeLink,
                                         LINK_NAME_ARG(const char* qt_name)
                                         void *ptr, bool created_by_java, bool is_shell,
                                         PtrOwnerFunction ownerFunction, AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    ~OwnedPointerToContainerLink() override;
public:
    void deleteNativeObject(JNIEnv *env, bool forced = false) override;
    void init(JNIEnv* env) override;
    inline PtrOwnerFunction ownerFunction() const override {return m_owner_function;}
private:
    PtrOwnerFunction m_owner_function;
    friend QtJambiLink;
};

class DeletablePointerToContainerLink : public PointerToContainerLink{
protected:
    DeletablePointerToContainerLink(JNIEnv *env, jobject nativeLink,
                                    LINK_NAME_ARG(const char* qt_name)
                                    void *ptr, bool created_by_java, bool is_shell,
                                    PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess, JavaException& ocurredException);
public:
    void deleteNativeObject(JNIEnv *env, bool forced = false) override;
    inline PtrDeleterFunction deleterFunction() const override {return m_deleter_function;}
private:
    PtrDeleterFunction m_deleter_function;
    friend QtJambiLink;
    friend class DeletableOwnedPointerToContainerLink;
};

class DeletableOwnedPointerToContainerLink : public DeletablePointerToContainerLink{
protected:
    DeletableOwnedPointerToContainerLink(JNIEnv *env, jobject nativeLink,
                                         LINK_NAME_ARG(const char* qt_name)
                                         void *ptr, bool created_by_java, bool is_shell,
                                         PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, AbstractContainerAccess* containerAccess, JavaException& ocurredException);
public:
    void deleteNativeObject(JNIEnv *env, bool forced = false) override;
    inline PtrOwnerFunction ownerFunction() const override {return m_owner_function;}
private:
    PtrOwnerFunction m_owner_function;
    friend QtJambiLink;
};

#define INTERFACE_DECL(LINKTYPE)\
protected:\
    ~LINKTYPE() override;\
public:\
    void unregisterOffsets() override;\
    void addInterface(const std::type_info& qtInterfaceType) override;\
    void removeInterface(const std::type_info& qtInterfaceType) override;\
    bool isInterfaceAvailable(const std::type_info& qtInterfaceType) const override;\
    void* typedPointer(const std::type_info& qtType) const override;\
    bool isMultiInheritanceType() const override;\
    const InterfaceOffsetInfo* interfaceOffsetInfo() const override;\
private:\
    const InterfaceOffsetInfo m_interfaceOffsetInfo;\
    QSet<size_t> m_availableInterfaces;\
    friend QtJambiLink;

class PointerToObjectInterfaceLink : public PointerToObjectLink{
protected:
    PointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                           LINK_NAME_ARG(const char* qt_name)
                                 void *ptr, bool created_by_java, bool is_shell, JavaException& ocurredException);
    INTERFACE_DECL(PointerToObjectInterfaceLink)
};

class MetaTypedPointerToObjectInterfaceLink : public MetaTypedPointerToObjectLink{
protected:
    MetaTypedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
                                 void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, JavaException& ocurredException);
    INTERFACE_DECL(MetaTypedPointerToObjectInterfaceLink)
};

class DependentMetaTypedPointerToObjectInterfaceLink : public MetaTypedPointerToObjectInterfaceLink, public DependentLink{
protected:
    DependentMetaTypedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
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
    OwnedMetaTypedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
                                 void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    INTERFACE_DECL(OwnedMetaTypedPointerToObjectInterfaceLink)
};

class OwnedDependentMetaTypedPointerToObjectInterfaceLink : public OwnedMetaTypedPointerToObjectInterfaceLink, public DependentLink{
protected:
    OwnedDependentMetaTypedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
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
    PointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                           LINK_NAME_ARG(const char* qt_name)
                                 void *ptr, bool created_by_java, bool is_shell, AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    INTERFACE_DECL(PointerToContainerInterfaceLink)
};

class DeletablePointerToObjectInterfaceLink : public DeletablePointerToObjectLink{
protected:
    DeletablePointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                           LINK_NAME_ARG(const char* qt_name)
                                 void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, JavaException& ocurredException);
    INTERFACE_DECL(DeletablePointerToObjectInterfaceLink)
};

class DeletablePointerToContainerInterfaceLink : public DeletablePointerToContainerLink{
protected:
    DeletablePointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
                                          LINK_NAME_ARG(const char* qt_name)
                                          void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    INTERFACE_DECL(DeletablePointerToContainerInterfaceLink)
};

class DeletableOwnedPointerToObjectInterfaceLink : public DeletableOwnedPointerToObjectLink{
protected:
    DeletableOwnedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                           LINK_NAME_ARG(const char* qt_name)
                                 void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    INTERFACE_DECL(DeletableOwnedPointerToObjectInterfaceLink)
};

class DeletableOwnedPointerToContainerInterfaceLink : public DeletableOwnedPointerToContainerLink{
protected:
    DeletableOwnedPointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
                                               LINK_NAME_ARG(const char* qt_name)
                                               void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    INTERFACE_DECL(DeletableOwnedPointerToContainerInterfaceLink)
};

class OwnedPointerToContainerInterfaceLink : public DeletableOwnedPointerToContainerLink{
protected:
    OwnedPointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                  LINK_NAME_ARG(const char* qt_name)
                                                  void *ptr, bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    INTERFACE_DECL(OwnedPointerToContainerInterfaceLink)
};

class PointerToQObjectLink : public QtJambiLink{
protected:
    PointerToQObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMetaObject* metaObject, QObject *ptr, bool isQThread, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException);
    ~PointerToQObjectLink() override;
public:
    void *pointer() const override;
    QObject *qobject() const override;
    bool isQObject() const override;
    void setAsQObjectDeleted();
    inline bool qobjectDeleted() const override { return !m_pointer; }
    void deleteNativeObject(JNIEnv *env = nullptr, bool forced = false);
    void onDispose(JNIEnv *env) override;
    void onClean(JNIEnv *env) override;
    void invalidate(JNIEnv *env) override;
    QString describe() const override;
    void init(JNIEnv* env) final override;
    virtual void init(JNIEnv* env, QSharedPointer<QtJambiLink>* replacement);
    void removeInterface(const std::type_info&) override;
    bool isSmartPointer() const override final;
private:
    QObject* m_pointer;
    friend class QtJambiLink;
};

class PointerToQObjectWithExtraSignalsLink : public PointerToQObjectLink{
protected:
    PointerToQObjectWithExtraSignalsLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMetaObject* metaObject, QObject *ptr, bool isQThread, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException);
    ~PointerToQObjectWithExtraSignalsLink() override = default;
public:
    jobject getExtraSignal(JNIEnv * env, const QMetaMethod& method) const override;
private:
    mutable QHash<int,JObjectWrapper> m_extraSignals;
    friend QtJambiLink;
};

class PointerToPendingQObjectLink : public PointerToQObjectLink{
protected:
    PointerToPendingQObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMetaObject* metaObject, QObject *ptr, bool isQThread, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException);
    ~PointerToPendingQObjectLink() override = default;
public:
    jobject getExtraSignal(JNIEnv * env, const QMetaMethod& method) const override;
    using PointerToQObjectLink::init;
    void init(JNIEnv* env, QSharedPointer<QtJambiLink>* replacement) override;
private:
    mutable QHash<int,JObjectWrapper> m_extraSignals;
    friend QtJambiLink;
};

class PointerToQObjectInterfaceLink : public PointerToQObjectLink{
protected:
    PointerToQObjectInterfaceLink(JNIEnv *env,
                                  jobject nativeLink,
                                  jobject jobj,
                                  const InterfaceOffsetInfo& interfaceOffsetInfo,
                                  const QMetaObject* metaObject,
                                  QObject *ptr,
                                  bool isQThread,
                                  bool created_by_java,
                                  bool isDeclarativeCall,
                                  bool is_shell,
                                  JavaException& ocurredException);
    ~PointerToQObjectInterfaceLink() override;
public:
    void unregisterOffsets() override;
    void addInterface(const std::type_info& qtInterfaceType) override;
    void removeInterface(const std::type_info& qtInterfaceType) override;
    bool isInterfaceAvailable(const std::type_info& qtInterfaceType) const override;
    void* typedPointer(const std::type_info& qtType) const override;
    bool isMultiInheritanceType() const override;
    const InterfaceOffsetInfo* interfaceOffsetInfo() const override;
private:
    const InterfaceOffsetInfo m_interfaceOffsetInfo;
    QSet<size_t> m_availableInterfaces;
    friend QtJambiLink;
};

class PointerToPendingQObjectInterfaceLink : public PointerToQObjectInterfaceLink{
protected:
    PointerToPendingQObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const InterfaceOffsetInfo& interfaceOffsetInfo, const QMetaObject* metaObject, QObject *ptr, bool isQThread, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException);
    ~PointerToPendingQObjectInterfaceLink() override = default;
public:
    jobject getExtraSignal(JNIEnv * env, const QMetaMethod& method) const override;
    using PointerToQObjectLink::init;
    void init(JNIEnv* env, QSharedPointer<QtJambiLink>* replacement) override;
private:
    mutable QHash<int,JObjectWrapper> m_extraSignals;
    friend QtJambiLink;
};

class PointerToQObjectInterfaceWithExtraSignalsLink : public PointerToQObjectInterfaceLink{
protected:
    PointerToQObjectInterfaceWithExtraSignalsLink(JNIEnv *env, jobject nativeLink, jobject jobj, const InterfaceOffsetInfo& interfaceOffsetInfo, const QMetaObject* metaObject, QObject *ptr, bool isQThread, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException);
    ~PointerToQObjectInterfaceWithExtraSignalsLink() override = default;
public:
    jobject getExtraSignal(JNIEnv * env, const QMetaMethod& method) const override;
private:
    friend QtJambiLink;
    friend class PointerToQObjectInterfaceWithDetectedExtraSignalsLink;
    mutable QHash<int,JObjectWrapper> m_extraSignals;
};

#define DECL_USING \
public:\
using QtJambiLink::isQThread;\
using QtJambiLink::setJavaOwnership;\
using QtJambiLink::isShell;\
using QtJambiLink::isInitialized;\
using QtJambiLink::dispose;\
using QtJambiLink::releaseJavaObject;\
using QtJambiLink::unregisterOffsets;\
using QtJambiLink::registerPointer;\
using QtJambiLink::unregisterPointer;\
using QtJambiLink::invalidateDependentObjects;\
using QtJambiLink::clearAllDependencies;\
using QtJambiLink::detachJavaLink;\
using QtJambiLink::setDeleteLater;\
using QtJambiLink::getJavaObjectLocalRef;\
using QtJambiLink::pointer;\
using QtJambiLink::setInDestructor;\
using QtJambiLink::containerAccess;\
using QtJambiLink::deleterFunction;\
using QtJambiLink::metaType;\

template<template<typename> class SmartPointer, typename T>
class SmartPointerLink : public QtJambiLink{
    DECL_USING
protected:
    SmartPointerLink(JNIEnv *env, jobject nativeLink,
                     LINK_NAME_ARG(const char *qt_name)
                     bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                     SmartPointer<T>*& smartPointer,
                     JavaException& ocurredException);
    ~SmartPointerLink() override{}
    template<typename Link>
    inline operator QSharedPointer<Link>() const {
        return QtJambiLink::getStrongPointer().template staticCast<Link>();
    }
    template<typename Link>
    inline operator QWeakPointer<Link>() const {
        return QtJambiLink::getStrongPointer().template staticCast<Link>();
    }
public:
    typedef typename std::conditional<std::is_same<SmartPointer<T>,QWeakPointer<T>>::value, QSharedPointer<T>,
                                      typename std::conditional<std::is_same<SmartPointer<T>,std::weak_ptr<T>>::value, std::shared_ptr<T>, SmartPointer<T>>::type>::type SharedPointerT;
    void setCppOwnership(JNIEnv *env) override final;
    void setSplitOwnership(JNIEnv *env) override final;
    void setJavaOwnership(JNIEnv *env) override final;
    void setDefaultOwnership(JNIEnv *env) override final;
    Ownership ownership() const override final;

    bool isSmartPointer() const override final{ return true; }
    virtual SharedPointerT getSmartPointer() const;
private:
    friend QtJambiLink;
    template<template<typename> class, typename>
    friend struct SmartPointerDeleter;
protected:
    SmartPointer<T> m_smartPointer;
    QtJambiLink::Ownership m_hiddenOwnership;
};

template<template<typename> class SmartPointer>
class SmartPointerToObjectLink : public SmartPointerLink<SmartPointer,char>{
    DECL_USING
    using SmartPointerLink<SmartPointer,char>::m_smartPointer;
protected:
    SmartPointerToObjectLink(JNIEnv *env, jobject nativeLink,
                             LINK_NAME_ARG(const char* qt_name)
                             bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                             PtrOwnerFunction ownerFunction,
                             void* pointer,
                             SmartPointer<char>*& smartPointer,
                             JavaException& ocurredException);
    ~SmartPointerToObjectLink() override;
public:
    virtual QObject *qobject() const override { Q_ASSERT_X(false, "QtJambiLink", "Pointer is not QObject"); return nullptr; }
    virtual bool isQObject() const override{
        return false;
    }
    typename SmartPointerLink<SmartPointer,char>::SharedPointerT getSmartPointer() const override;
    virtual void *pointer() const override { return m_pointer; }
    inline bool qobjectDeleted() const override { return false; }
    void deleteNativeObject(JNIEnv *env = nullptr, bool forced = false);
    void onDispose(JNIEnv *env) override;
    void onClean(JNIEnv *env) override;
    void invalidate(JNIEnv *env) override;
    QString describe() const override;
    bool isPointerZeroed() const { return this->m_flags.testFlag(QtJambiLink::Flag::IsDeleteLater); }
    void setPointerZeroed(){ this->m_flags.setFlag(QtJambiLink::Flag::IsDeleteLater); }
    void init(JNIEnv* env) override;
public:
    inline PtrOwnerFunction ownerFunction() const override {return m_owner_function;}
private:
    void* m_pointer;
    PtrOwnerFunction m_owner_function;
    friend QtJambiLink;
};

template<template<typename> class SmartPointer>
class MetaTypedSmartPointerToObjectLink : public SmartPointerToObjectLink<SmartPointer>{
public:
    MetaTypedSmartPointerToObjectLink(JNIEnv *env, jobject nativeLink,
                                      const QMetaType& metaType,
                                      bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                      PtrOwnerFunction ownerFunction,
                                      void* pointer,
                                      SmartPointer<char>*& smartPointerTarget,
                                      JavaException& ocurredException)
        : SmartPointerToObjectLink<SmartPointer>(env, nativeLink,
                                                 LINK_NAME_ARG(metaType.name())
                                                 created_by_java, ownership, is_shell, ownerFunction, pointer, smartPointerTarget, ocurredException),
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        m_metaType(metaType)
#else
        m_metaType(metaType.id())
#endif
    {}
    const QMetaType* metaType() const override { return &m_metaType; }
private:
    QMetaType m_metaType;
};

template<template<typename> class SmartPointer>
class ExtendedSmartPointerToObjectLink : public SmartPointerToObjectLink<SmartPointer>, public ExtendedLink{
protected:
    ExtendedSmartPointerToObjectLink(JNIEnv *env, jobject nativeLink,
                                     LINK_NAME_ARG(const char* qt_name)
                                     bool created_by_java, QtJambiLink::Ownership ownership,
                                     bool is_shell,
                                     const QObject* extension,
                                     PtrOwnerFunction ownerFunction,
                                     void* pointer,
                                     SmartPointer<char>*& smartPointer,
                                     JavaException& ocurredException);
public:
    inline bool hasExtension()const override{return !m_extension.isNull(); }
private:
    QPointer<const QObject> m_extension;
    friend QtJambiLink;
};

template<template<typename> class SmartPointer>
class SmartPointerToContainerLink : public SmartPointerToObjectLink<SmartPointer>{
    DECL_USING
protected:
    SmartPointerToContainerLink(JNIEnv *env, jobject nativeLink,
                                LINK_NAME_ARG(const char* qt_name)
                                bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                PtrOwnerFunction ownerFunction,
                                void* pointer,
                                SmartPointer<char>*& smartPointer,
                                AbstractContainerAccess* containerAccess,
                                JavaException& ocurredException);
    ~SmartPointerToContainerLink() override;
public:
    inline AbstractContainerAccess* containerAccess() const override { return m_containerAccess; }
    void init(JNIEnv* env) override;
private:
    AbstractContainerAccess *const m_containerAccess;
    friend QtJambiLink;
};

template<template<typename> class SmartPointer>
class SmartPointerToQObjectLink : public SmartPointerLink<SmartPointer,QObject>{
    DECL_USING
    using SmartPointerLink<SmartPointer,QObject>::m_smartPointer;
protected:
    SmartPointerToQObjectLink(JNIEnv *env, jobject nativeLink,
                              LINK_NAME_ARG(const char *qt_name)
                              bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                              QObject* pointer,
                              SmartPointer<QObject>*& smartPointer,
                              JavaException& ocurredException);
    ~SmartPointerToQObjectLink() override;
public:
    virtual QObject *qobject() const override {
        return m_pointer;
    }
    virtual bool isQObject() const override{
        return true;
    }
    typename SmartPointerLink<SmartPointer,QObject>::SharedPointerT getSmartPointer() const override;
    virtual void *pointer() const override {
        return qobject();
    }
    void deleteNativeObject(JNIEnv *env = nullptr, bool forced = false);
    void setAsQObjectDeleted(){
        m_pointer = nullptr;
    }
    inline bool qobjectDeleted() const override { return !m_pointer; }
    void onDispose(JNIEnv *env) override;
    void onClean(JNIEnv *env) override;
    void invalidate(JNIEnv *env) override;
    QString describe() const override;
    void removeInterface(const std::type_info&) override{
        QtJambiLink::setInDestructor();
    }
    bool isDeleteLater() const { return this->m_flags.testFlag(QtJambiLink::Flag::IsDeleteLater); }
private:
    void setDeleteLater(){ this->m_flags.setFlag(QtJambiLink::Flag::IsDeleteLater); }
    QObject* m_pointer;
    friend QtJambiLink;
};

template<template<typename> class SmartPointer>
class SmartPointerToPlainQObjectLink : public SmartPointerToQObjectLink<SmartPointer>{
    DECL_USING
protected:
    SmartPointerToPlainQObjectLink(JNIEnv *env,
                                   jobject nativeLink,
                                   jobject jobj,
                                   bool created_by_java,
                                   QtJambiLink::Ownership ownership,
                                   bool is_shell,
                                   QObject* pointer,
                                   bool isQThread,
                                   SmartPointer<QObject>*& smartPointer,
                                   JavaException& ocurredException);
    ~SmartPointerToPlainQObjectLink() override = default;
private:
    friend QtJambiLink;
};

template<template<typename> class SmartPointer>
class SmartPointerToQObjectWithPendingExtraSignalsLink : public SmartPointerToQObjectLink<SmartPointer>{
    DECL_USING
protected:
    SmartPointerToQObjectWithPendingExtraSignalsLink(JNIEnv *env, jobject nativeLink,
                                                     LINK_NAME_ARG(const char *qt_name)
                                                     bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                     QObject* pointer,
                                                     SmartPointer<QObject>*& smartPointer,
                                                     JavaException& ocurredException);
    SmartPointerToQObjectWithPendingExtraSignalsLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                                                     bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                     QObject* pointer,
                                                     bool isQThread,
                                                     SmartPointer<QObject>*& smartPointer,
                                                     const QMetaObject* superTypeForCustomMetaObject,
                                                     JavaException& ocurredException);
    ~SmartPointerToQObjectWithPendingExtraSignalsLink() override = default;
public:
    jobject getExtraSignal(JNIEnv * env, const QMetaMethod& method) const override;
private:
    mutable QHash<int,JObjectWrapper> m_extraSignals;
    friend QtJambiLink;
};

template<template<typename> class SmartPointer>
class SmartPointerToQObjectWithExtraSignalsLink : public SmartPointerToQObjectWithPendingExtraSignalsLink<SmartPointer>{
    DECL_USING
protected:
    SmartPointerToQObjectWithExtraSignalsLink(JNIEnv *env,
                                              jobject nativeLink,
                                              jobject jobj,
                                              bool created_by_java,
                                              QtJambiLink::Ownership ownership,
                                              bool is_shell,
                                              QObject* pointer,
                                              bool isQThread,
                                              SmartPointer<QObject>*& smartPointer,
                                              JavaException& ocurredException);
    ~SmartPointerToQObjectWithExtraSignalsLink() override = default;
private:
    friend QtJambiLink;
};

template<template<typename> class SmartPointer>
class SmartPointerToObjectInterfaceLink : public SmartPointerToObjectLink<SmartPointer>{
    DECL_USING
public:
    SmartPointerToObjectInterfaceLink(JNIEnv *env,
                                      jobject nativeLink,
                                      const InterfaceOffsetInfo& interfaceOffsetInfo,
                                      LINK_NAME_ARG(const char* qt_name)
                                      bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                      PtrOwnerFunction ownerFunction,
                                      void* pointer,
                                      SmartPointer<char>*& smartPointer,
                                      JavaException& ocurredException);
    INTERFACE_DECL(SmartPointerToObjectInterfaceLink)
};

template<template<typename> class SmartPointer>
class MetaTypedSmartPointerToObjectInterfaceLink : public SmartPointerToObjectInterfaceLink<SmartPointer>{
public:
    MetaTypedSmartPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink,
                                      const InterfaceOffsetInfo& interfaceOffsetInfo,
                                      const QMetaType& metaType,
                                      bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                      PtrOwnerFunction ownerFunction,
                                      void* pointer,
                                      SmartPointer<char>*& smartPointerTarget,
                                      JavaException& ocurredException)
        : SmartPointerToObjectInterfaceLink<SmartPointer>(env, nativeLink,
                                                 interfaceOffsetInfo,
                                                 LINK_NAME_ARG(metaType.name())
                                                 created_by_java, ownership, is_shell, ownerFunction, pointer, smartPointerTarget, ocurredException),
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        m_metaType(metaType)
#else
        m_metaType(metaType.id())
#endif
    {}
    const QMetaType* metaType() const override { return &m_metaType; }
private:
    QMetaType m_metaType;
};

template<template<typename> class SmartPointer>
class SmartPointerToContainerInterfaceLink : public SmartPointerToContainerLink<SmartPointer>{
    DECL_USING
public:
    SmartPointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink,
                                         const InterfaceOffsetInfo& interfaceOffsetInfo,
                                         LINK_NAME_ARG(const char* qt_name)
                                         bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                         PtrOwnerFunction ownerFunction,
                                         void* pointer,
                                         SmartPointer<char>*& smartPointer,
                                         AbstractContainerAccess* containerAccess,
                                         JavaException& ocurredException);
    INTERFACE_DECL(SmartPointerToContainerInterfaceLink)
};

template<template<typename> class SmartPointer>
class SmartPointerToPlainQObjectInterfaceLink : public SmartPointerToPlainQObjectLink<SmartPointer>{
    DECL_USING
public:
    SmartPointerToPlainQObjectInterfaceLink(JNIEnv *env,
                                            jobject nativeLink,
                                            jobject jobj,
                                            const InterfaceOffsetInfo& interfaceOffsetInfo,
                                            bool created_by_java,
                                            QtJambiLink::Ownership ownership,
                                            bool is_shell,
                                            QObject* pointer,
                                            bool isQThread,
                                            SmartPointer<QObject>*& smartPointer,
                                            JavaException& ocurredException);
    INTERFACE_DECL(SmartPointerToPlainQObjectInterfaceLink)
};

template<template<typename> class SmartPointer>
class SmartPointerToQObjectInterfaceWithPendingExtraSignalsLink : public SmartPointerToQObjectWithPendingExtraSignalsLink<SmartPointer>{
    DECL_USING
public:
    SmartPointerToQObjectInterfaceWithPendingExtraSignalsLink(JNIEnv *env,
                                                              jobject nativeLink,
                                                              jobject jobj,
                                                              const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                              bool created_by_java,
                                                              QtJambiLink::Ownership ownership,
                                                              bool is_shell,
                                                              QObject* pointer,
                                                              bool isQThread,
                                                              SmartPointer<QObject>*& smartPointer,
                                                              const QMetaObject* superTypeForCustomMetaObject,
                                                              JavaException& ocurredException);
    INTERFACE_DECL(SmartPointerToQObjectInterfaceWithPendingExtraSignalsLink)
};

template<template<typename> class SmartPointer>
class SmartPointerToQObjectInterfaceWithExtraSignalsLink : public SmartPointerToQObjectWithExtraSignalsLink<SmartPointer>{
    DECL_USING
public:
    SmartPointerToQObjectInterfaceWithExtraSignalsLink(JNIEnv *env,
                                                       jobject nativeLink,
                                                       jobject jobj,
                                                       const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                       bool created_by_java,
                                                       QtJambiLink::Ownership ownership,
                                                       bool is_shell,
                                                       QObject* pointer,
                                                       bool isQThread,
                                                       SmartPointer<QObject>*& smartPointer,
                                                       JavaException& ocurredException);
    INTERFACE_DECL(SmartPointerToQObjectInterfaceWithExtraSignalsLink)
};

template<template<typename> class SmartPointer>
struct SmartPointerUtility{
};

template<>
struct SmartPointerUtility<QSharedPointer>{
    typedef SmartPointerLink<QSharedPointer,QObject> SharedQObjectLink;
    typedef SmartPointerLink<QSharedPointer,char> SharedLink;
    typedef SmartPointerLink<QWeakPointer,QObject> WeakQObjectLink;
    typedef SmartPointerLink<QWeakPointer,char> WeakLink;
    template<typename T, typename... Args>
    static constexpr QSharedPointer<T> SharedPointer(Args... args){
        return QSharedPointer<T>(args...);
    }
    template<typename T>
    static constexpr QSharedPointer<T> SharedPointer(const QSharedPointer<T>& other, T* ptr){
        return QtSharedPointer::copyAndSetPointer(ptr, other);
    }
    template<typename T, typename X>
    static constexpr QSharedPointer<T> SharedPointer(const QSharedPointer<X>& other, T* ptr){
        return QtSharedPointer::copyAndSetPointer(ptr, reinterpret_cast<const QSharedPointer<T>&>(other));
    }
    template<typename T, typename... Args>
    static constexpr QWeakPointer<T> WeakPointer(Args... args){
        return QWeakPointer<T>(args...);
    }
    static constexpr auto SharedPointerFlag = QtJambiLink::Flag::QSharedPointer;
    static constexpr auto WeakPointerFlag = QtJambiLink::Flag::QWeakPointer;
};

template<>
struct SmartPointerUtility<std::shared_ptr>{
    typedef SmartPointerLink<std::shared_ptr,QObject> SharedQObjectLink;
    typedef SmartPointerLink<std::shared_ptr,char> SharedLink;
    typedef SmartPointerLink<std::weak_ptr,QObject> WeakQObjectLink;
    typedef SmartPointerLink<std::weak_ptr,char> WeakLink;
    template<typename T, typename... Args>
    static constexpr std::shared_ptr<T> SharedPointer(Args... args){
        return std::shared_ptr<T>(args...);
    }
    template<typename T>
    static constexpr std::shared_ptr<T> SharedPointer(const std::shared_ptr<T>& other, T* ptr){
        return std::shared_ptr<T>(other, ptr);
    }
    template<typename T, typename X>
    static constexpr std::shared_ptr<T> SharedPointer(const std::shared_ptr<X>& other, T* ptr){
        return std::shared_ptr<T>(reinterpret_cast<const std::shared_ptr<X>&>(other), ptr);
    }
    template<typename T, typename... Args>
    static constexpr std::weak_ptr<T> WeakPointer(Args... args){
        return std::weak_ptr<T>(args...);
    }
    static constexpr auto SharedPointerFlag = QtJambiLink::Flag::shared_ptr;
    static constexpr auto WeakPointerFlag = QtJambiLink::Flag::weak_ptr;
};

template<>
struct SmartPointerUtility<QWeakPointer>{
    typedef SmartPointerLink<QSharedPointer,QObject> SharedQObjectLink;
    typedef SmartPointerLink<QSharedPointer,char> SharedLink;
    typedef SmartPointerLink<QWeakPointer,QObject> WeakQObjectLink;
    typedef SmartPointerLink<QWeakPointer,char> WeakLink;
    template<typename T, typename... Args>
    static constexpr QSharedPointer<T> SharedPointer(Args... args){
        return QSharedPointer<T>(args...);
    }
    template<typename T>
    static constexpr QSharedPointer<T> SharedPointer(const QSharedPointer<T>& other, T* ptr){
        return QtSharedPointer::copyAndSetPointer(ptr, other);
    }
    template<typename T, typename X>
    static constexpr QSharedPointer<T> SharedPointer(const QSharedPointer<X>& other, T* ptr){
        return QtSharedPointer::copyAndSetPointer(ptr, reinterpret_cast<const QSharedPointer<T>&>(other));
    }
    template<typename T, typename... Args>
    static constexpr QWeakPointer<T> WeakPointer(Args... args){
        return QWeakPointer<T>(args...);
    }
    static constexpr auto SharedPointerFlag = QtJambiLink::Flag::QSharedPointer;
    static constexpr auto WeakPointerFlag = QtJambiLink::Flag::QWeakPointer;
};

template<>
struct SmartPointerUtility<std::weak_ptr>{
    typedef SmartPointerLink<std::shared_ptr,QObject> SharedQObjectLink;
    typedef SmartPointerLink<std::shared_ptr,char> SharedLink;
    typedef SmartPointerLink<std::weak_ptr,QObject> WeakQObjectLink;
    typedef SmartPointerLink<std::weak_ptr,char> WeakLink;
    template<typename T, typename... Args>
    static constexpr std::shared_ptr<T> SharedPointer(Args... args){
        return std::shared_ptr<T>(args...);
    }
    template<typename T>
    static constexpr std::shared_ptr<T> SharedPointer(const std::shared_ptr<T>& other, T* ptr){
        return std::shared_ptr<T>(other, ptr);
    }
    template<typename T, typename X>
    static constexpr std::shared_ptr<T> SharedPointer(const std::shared_ptr<X>& other, T* ptr){
        return std::shared_ptr<T>(reinterpret_cast<const std::shared_ptr<X>&>(other), ptr);
    }
    template<typename T, typename... Args>
    static constexpr std::shared_ptr<T> WeakPointer(Args... args){
        return std::weak_ptr<T>(args...);
    }
    static constexpr auto SharedPointerFlag = QtJambiLink::Flag::shared_ptr;
    static constexpr auto WeakPointerFlag = QtJambiLink::Flag::weak_ptr;
};

class QtJambiLinkScope : public QtJambiScope{
public:
    QtJambiLinkScope(const QSharedPointer<QtJambiLink>& _link);
    Q_DISABLE_COPY_MOVE(QtJambiLinkScope)

    QtJambiLink * link() const;
};

#endif // QTJAMBILINK_P_H
