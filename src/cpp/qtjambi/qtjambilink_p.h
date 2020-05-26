/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBILINK_P_H
#define QTJAMBILINK_P_H

#include "qtjambi_registry_p.h"
#include "qtjambi_core.h"
#include "qtjambi_interfaces.h"
#include "qtdynamicmetaobject_p.h"
#include <typeindex>

#if defined(QTJAMBI_DEBUG_TOOLS) || !defined(QT_NO_DEBUG)
#define QTJAMBI_INCREASE_COUNTER_THIS(counter) QtJambiMemoryDebugTool::increase_##counter(qtTypeName());
#define QTJAMBI_INCREASE_COUNTER(counter, _this) if(_this) QtJambiMemoryDebugTool::increase_##counter(_this->qtTypeName());
#define QTJAMBI_INCREASE_COUNTER2(counter, qtTypeName) QtJambiMemoryDebugTool::increase_##counter(qtTypeName);
#else
#define QTJAMBI_INCREASE_COUNTER_THIS(counter)
#define QTJAMBI_INCREASE_COUNTER(counter, _this)
#define QTJAMBI_INCREASE_COUNTER2(counter, qtTypeName)
#endif

#if defined(QTJAMBI_DEBUG_TOOLS)
// Allow useful truncation on 32bit
#define QTJAMBI_LINK_USER_DATA_MAGIC (long(0xf6f700fff5f42f03ll))
#define QTJAMBI_LINK_MAGIC (long(0xf6f700fff5f42f04ll))

#define QTJAMBI_REGISTER_LINK registerLink(this);
#define QTJAMBI_UNREGISTER_LINK unregisterLink(this);
#define QTJAMBI_UNREGISTER_LINK_FINALLY if(m_flags.testFlag(Flag::IsListed)) unregisterLink(this);
#else
#define QTJAMBI_REGISTER_LINK
#define QTJAMBI_UNREGISTER_LINK
#define QTJAMBI_UNREGISTER_LINK_FINALLY
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
#else
#define QTJAMBI_ACQUIRE_MAGIC(what, _this)
#define QTJAMBI_RELEASE_MAGIC(what, _this)
#endif

const InterfaceOffsetInfo* getInterfaceOffsets(JNIEnv *env, jclass clazz);

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

class QtJambiSignalInfo
{
public:
    QtJambiSignalInfo(const QtJambiSignalInfo & info);
    QtJambiSignalInfo(QtJambiSignalInfo && info);
    QtJambiSignalInfo();
    QtJambiSignalInfo(JNIEnv* env, jobject object, bool global
#ifdef QT_QTJAMBI_PORT
                      , CallEmitMethod _CallEmitMethod
#endif
                      );

    QtJambiSignalInfo & operator =(const QtJambiSignalInfo & info);
    jobject signalObject() const;
#ifdef QT_QTJAMBI_PORT
    void callEmitMethod(JNIEnv* env, jobject signal, jvalue *arguments) const;
#endif
private:
    QSharedPointer<JObjectWrapperData> m_signalObject;
#ifdef QT_QTJAMBI_PORT
    CallEmitMethod m_CallEmitMethod;
#endif
};

typedef std::function<QObject*(void *)> PointerQObjectGetterFunction;

class QtJambiLink;

struct ValueOwnerUserData : public QObjectUserData
{
    ValueOwnerUserData(const QObject* object);
    ~ValueOwnerUserData() override;
    void addDeleter(const std::function<void()>& deleter);
    inline const QPointer<const QObject>& pointer() const { return m_object; }
    static uint id();
private:
    QPointer<const QObject> m_object;
    QList<std::function<void()>> m_deleters;
};

struct QtJambiLinkUserData : public QObjectUserData
{
    QtJambiLinkUserData(const QWeakPointer<QtJambiLink> & link, const QMetaObject* metaObject);
    ~QtJambiLinkUserData() override;

    inline const QMetaObject* metaObject() const {return m_metaObject; }
    inline const QWeakPointer<QtJambiLink>& link() const { return m_link; }

    static uint id();
    static QMutex* lock();

private:
    QWeakPointer<QtJambiLink> m_link;
    const QMetaObject* m_metaObject;
};

class PointerContainer : public QObject{
public:
    explicit PointerContainer(JNIEnv* env, jobject jobj, const QMetaObject* metaObject, const QSharedPointer<QtJambiLink>& link, void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, PointerQObjectGetterFunction pointerGetter, JavaException& ocurredException);

    virtual ~PointerContainer();
    void* getSharedPointer() const;
    QObject *qobject() const;
#ifdef QT_QTJAMBI_PORT
    void emitSignal(JNIEnv * env, SignalMethodIndexProvider signalMethodIndexProvider, jvalue *arguments) const;
    void emitExtraSignal(JNIEnv * env, int i, jvalue *arguments) const;
#endif //QT_QTJAMBI_PORT
    jobject getListOfExtraSignal(JNIEnv * env) const;
    void setAsQObjectDeleted();
#ifdef QT_QTJAMBI_PORT
    void reconnect() const;
#endif
private:
    void* m_ptr_shared_pointer;
    PointerDeleter m_shared_pointer_deleter;
    PointerQObjectGetterFunction m_shared_pointer_getter;
    QWeakPointer<QtJambiLink> m_link;
    mutable QHash<int,QMetaObject::Connection> m_connections;
    const QHash<int,QtJambiSignalInfo> m_signals;
    const QHash<int,QtJambiSignalInfo> m_extraSignals;
    QMetaObject::Connection m_destroyedConnection;
#ifdef QT_QTJAMBI_PORT
    friend class SlotObject;
#endif // QT_QTJAMBI_PORT
};

class QtJambiLink{
public:
    static const QSharedPointer<QtJambiLink>& createLinkForObject(JNIEnv *env, jobject java, void *ptr, int metaType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                  const char* qt_name,
#endif
                                                                  bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction);
    static const QSharedPointer<QtJambiLink>& createLinkForOwnedObject(JNIEnv *env, jobject java, void *ptr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                       const char* qt_name,
#endif
                                                                       QtJambiNativeID owner, PtrDeleterFunction destructor_function);
    static const QSharedPointer<QtJambiLink>& createLinkForInterface(JNIEnv *env, jobject java, void *ptr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                     const char* qt_name,
#endif
                                                                     const QSharedPointer<QtJambiLink>& owner);
    static const QSharedPointer<QtJambiLink>& createLinkForSharedPointerToObject(JNIEnv *env, jobject java, int metaType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                 const char* qt_name,
#endif
                                                                                 bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, void* ptr_shared_pointer, PointerDeleter pointerDeleter, PointerGetterFunction pointerGetter);
    static const QSharedPointer<QtJambiLink>& createLinkForQObject(JNIEnv *env, jobject java, QObject *object, bool created_by_java, bool is_shell);
    static const QSharedPointer<QtJambiLink>& createLinkForSharedPointerToQObject(JNIEnv *env, jobject java, bool created_by_java, bool is_shell, void* ptr_shared_pointer, PointerDeleter pointerDeleter, PointerQObjectGetterFunction pointerGetter);
    static const QSharedPointer<QtJambiLink>& createLinkForNewQObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject java, const std::type_info& typeId, const QMetaObject* metaObject, QObject *object, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, JavaException& ocurredException);
    static const QSharedPointer<QtJambiLink>& createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject java, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, int metaType, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    static const QSharedPointer<QtJambiLink>& fromNativeId(QtJambiNativeID native_id);
    static const QWeakPointer<QtJambiLink>& findLinkForQObject(QObject *qobject);
    static const QWeakPointer<QtJambiLink>& findLinkForPointer(const void *ptr);
    static const QSharedPointer<QtJambiLink>& findLinkForJavaObject(JNIEnv *env, jobject java);
public:
    enum class Ownership : quint8 {
        None = 0,
        Java = 0x001,   // Weak ref to java object, deleteNativeObject deletes c++ object
        Cpp = 0x002,    // Strong ref to java object until c++ object is deleted, deleteNativeObject does *not* delete c++ obj.
        Split = 0x004  // Weak ref to java object, deleteNativeObject does *not* delete c++ object. Only for objects not created by Java.
    };

    enum class Flag : quint16 {
        None = 0,
        JavaOwnership = quint16(Ownership::Java),   // Weak ref to java object, deleteNativeObject deletes c++ object
        CppOwnership = quint16(Ownership::Cpp),    // Strong ref to java object until c++ object is deleted, deleteNativeObject does *not* delete c++ obj.
        SplitOwnership = quint16(Ownership::Split),  // Weak ref to java object, deleteNativeObject does *not* delete c++ object. Only for objects not created by Java.
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
        IsInDestructor = 0x1000,
        IsDeleteLater = 0x2000,
#ifdef QT_QTJAMBI_PORT
        BlockReconnect = 0x4000,
#else
        NoThreadInitializationOnPurge = 0x4000,
#endif
#if defined(QTJAMBI_DEBUG_TOOLS)
        IsListed = 0x8000,   // Weak ref to java object, deleteNativeObject deletes c++ object
#endif
    };
    typedef QFlags<Flag> Flags;

    QtJambiLink(JNIEnv *env, jobject nativeLink, jobject jobj,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                    const char* qt_name,
#endif
                    bool created_by_java, bool is_shell, JavaException& ocurredException);
    virtual ~QtJambiLink();

    jobject getJavaObjectLocalRef(JNIEnv *env) const;
    bool createdByJava() const;
    bool isShell() const;
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
    virtual int metaType() const = 0;
    virtual bool isSharedPointer() const = 0;
    virtual QString describe() const = 0;

    bool isInDestructor() const;

#ifdef QT_QTJAMBI_PORT
    bool reconnectBlocked() const { return m_flags.testFlag(Flag::BlockReconnect); }
    void blockReconnect() { m_flags.setFlag(Flag::BlockReconnect, true); }
    void unblockReconnect() { m_flags.setFlag(Flag::BlockReconnect, true); }
#endif

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

    /* Called when java object is invalidated */
    void javaObjectInvalidated(JNIEnv *env);

    jobject nativeLink(JNIEnv* env) const;

    virtual void init(JNIEnv* env);

    virtual void *pointer() const = 0;
    virtual void addInterface(const std::type_info& qtInterfaceType);
    virtual void removeInterface(const std::type_info& qtInterfaceType);
    virtual bool isInterfaceAvailable(const std::type_info& qtInterfaceType) const;

    virtual void* typedPointer(const std::type_info& qtType) const;
    void registerDependentObject(const QSharedPointer<QtJambiLink>& link);
    void unregisterDependentObject(const QSharedPointer<QtJambiLink>& link);

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
protected:
    void dispose();
    bool isDeleteLater() const { return m_flags.testFlag(Flag::IsDeleteLater); }
    void setDeleteLater(){ m_flags.setFlag(Flag::IsDeleteLater); }
    void invalidateDependentObjects(JNIEnv *env);
    void setInDestructor();
    virtual void unregisterOffsets();
    void registerPointer(void*);
    void unregisterPointer(void*);

    QSharedPointer<QtJambiLink> m_this;
    QVector<QWeakPointer<QtJambiLink>> m_dependentObjects;

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
};

class PointerToObjectLink : public QtJambiLink{
protected:
    PointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                        const char* qt_name,
#endif
                        void *ptr, int metaType, bool created_by_java, bool is_shell,
                        PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
public:
    ~PointerToObjectLink() override;
    void *pointer() const override;
    QObject *qobject() const override;
    bool isQObject() const override;
    void deleteNativeObject(JNIEnv *env, bool forced = false);
    inline bool qobjectDeleted() const override { return false; }
    inline int metaType() const override {return m_meta_type;}
    void onDispose(JNIEnv *env) override;
    void onClean(JNIEnv *env) override;
    void invalidate(JNIEnv *env) override;
    QString describe() const override;
    PtrDeleterFunction destructorFunction() const;
    PtrOwnerFunction ownerFunction() const;
    bool isSharedPointer() const override final;
    void init(JNIEnv* env) override;
private:
    void *m_pointer;
    int m_meta_type;
    PtrDeleterFunction m_deleter_function;
    PtrOwnerFunction m_owner_function;
    QVector<QWeakPointer<QtJambiLink>> m_dependencies;
    friend QtJambiLink;
};

class PointerToQObjectLink : public QtJambiLink{
protected:
    PointerToQObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMetaObject* metaObject, QObject *ptr, bool created_by_java, bool is_shell, JavaException& ocurredException);
public:
    ~PointerToQObjectLink() override;
    void *pointer() const override;
    QObject *qobject() const override;
    bool isQObject() const override;
    void setAsQObjectDeleted();
    virtual int metaType() const override;
    inline bool qobjectDeleted() const override { return !m_pointer; }
    void deleteNativeObject(JNIEnv *env, bool forced = false);
    void onDispose(JNIEnv *env) override;
    void onClean(JNIEnv *env) override;
    void invalidate(JNIEnv *env) override;
#ifdef QT_QTJAMBI_PORT
    void emitSignal(JNIEnv * env, SignalMethodIndexProvider signalendMethodIndexProvider, jvalue *arguments);
    void emitExtraSignal(JNIEnv * env, int i, jvalue *arguments) const;
#endif //QT_QTJAMBI_PORT
    jobject getListOfExtraSignal(JNIEnv * env) const;
#ifdef QT_QTJAMBI_PORT
    void reconnect() const;
#endif
    QString describe() const override;
    void init(JNIEnv* env) override;
    void removeInterface(const std::type_info&) override;
    bool isSharedPointer() const override final;
private:
    QObject* m_pointer;
    mutable QHash<int,QMetaObject::Connection> m_connections;
    const QHash<int,QtJambiSignalInfo> m_signals;
    const QHash<int,QtJambiSignalInfo> m_extraSignals;
    friend class QtJambiLink;
#ifdef QT_QTJAMBI_PORT
    friend class SlotObject;
#endif // QT_QTJAMBI_PORT
};

class QSharedPointerLink : public QtJambiLink{
protected:
    QSharedPointerLink(JNIEnv *env, jobject nativeLink, jobject jobj,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                       const char *qt_name,
#endif
                       bool created_by_java, bool is_shell, JavaException& ocurredException);
public:
    ~QSharedPointerLink() override;

    void setCppOwnership(JNIEnv *env) override final;
    void setSplitOwnership(JNIEnv *env) override final;

    bool isSharedPointer() const override final;
    virtual void* getSharedPointer() const = 0;
private:
    friend QtJambiLink;
};

class QSharedPointerToObjectLink : public QSharedPointerLink{
protected:
    QSharedPointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                               const char* qt_name,
#endif
                               int metaType, bool created_by_java, bool is_shell,
                               PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, void* ptr_shared_pointer,
                               PointerDeleter shared_pointer_deleter, PointerGetterFunction pointerGetter, JavaException& ocurredException);
    friend QtJambiLink;
public:
    ~QSharedPointerToObjectLink() override;
    virtual QObject *qobject() const override;
    virtual bool isQObject() const override;
    virtual void* getSharedPointer() const override;
    virtual void *pointer() const override;
    virtual int metaType() const override;
    inline bool qobjectDeleted() const override { return false; }
    void deleteNativeObject(JNIEnv *env, bool forced = false);
    void onDispose(JNIEnv *env) override;
    void onClean(JNIEnv *env) override;
    void invalidate(JNIEnv *env) override;
    QString describe() const override;
    PtrDeleterFunction destructorFunction() const;
    PtrOwnerFunction ownerFunction() const;
    bool isPointerZeroed() const { return m_flags.testFlag(Flag::IsDeleteLater); }
    void setPointerZeroed(){ m_flags.setFlag(Flag::IsDeleteLater); }
    void init(JNIEnv* env) override;
private:
    int m_meta_type;
    void* m_ptr_shared_pointer;
    PointerDeleter m_shared_pointer_deleter;
    PointerGetterFunction m_shared_pointer_getter;
    PtrDeleterFunction m_deleter_function;
    PtrOwnerFunction m_owner_function;
    QVector<QWeakPointer<QtJambiLink>> m_dependencies;
    friend QtJambiLink;
};

class QSharedPointerToQObjectLink : public QSharedPointerLink{
protected:
    QSharedPointerToQObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj, QObject* object, bool created_by_java, bool is_shell, void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, PointerQObjectGetterFunction pointerGetter, JavaException& ocurredException);
public:
    ~QSharedPointerToQObjectLink() override;
    virtual QObject *qobject() const override;
    virtual bool isQObject() const override;
    virtual void* getSharedPointer() const override;
    virtual void *pointer() const override;
    void deleteNativeObject(JNIEnv *env, bool forced = false);
    void setAsQObjectDeleted();
    void removePointerContainer();
    inline bool qobjectDeleted() const override { return !m_pointerContainer; }
    virtual int metaType() const override;
    void onDispose(JNIEnv *env) override;
    void onClean(JNIEnv *env) override;
    void invalidate(JNIEnv *env) override;
    inline PointerContainer* pointerContainer() const {return m_pointerContainer;}
#ifdef QT_QTJAMBI_PORT
    void emitSignal(JNIEnv * env, SignalMethodIndexProvider signalMethodIndexProvider, jvalue *arguments);
    void reconnect() const;
#endif
    QString describe() const override;
    void removeInterface(const std::type_info&) override;
    bool isDeleteLater() const { return m_flags.testFlag(Flag::IsDeleteLater); }
private:
    void setDeleteLater(){ m_flags.setFlag(Flag::IsDeleteLater); }
    PointerContainer* m_pointerContainer;
    friend QtJambiLink;
};

class PointerToQObjectInterfaceLink : public PointerToQObjectLink{
protected:
    PointerToQObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces, const QMetaObject* metaObject, QObject *ptr, bool created_by_java, bool is_shell, JavaException& ocurredException);
public:
    ~PointerToQObjectInterfaceLink() override;
    void unregisterOffsets() override;
    void addInterface(const std::type_info& qtInterfaceType) override;
    void removeInterface(const std::type_info& qtInterfaceType) override;
    bool isInterfaceAvailable(const std::type_info& qtInterfaceType) const override;
    void* typedPointer(const std::type_info& qtType) const override;
private:
    QMap<size_t, uint> m_interfaceOffsets;
    QSet<size_t> m_availableInterfaces;
    QMap<size_t, QSet<const std::type_info*>> m_inheritedInterfaces;
    friend QtJambiLink;
};

class PointerToObjectInterfaceLink : public PointerToObjectLink{
protected:
    PointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                           const char* qt_name,
#endif
                                 void *ptr, int metaType, bool created_by_java, bool is_shell,
                                 PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
public:
    ~PointerToObjectInterfaceLink() override;
    void unregisterOffsets() override;
    void addInterface(const std::type_info& qtInterfaceType) override;
    void removeInterface(const std::type_info& qtInterfaceType) override;
    bool isInterfaceAvailable(const std::type_info& qtInterfaceType) const override;
    void* typedPointer(const std::type_info& qtType) const override;
private:
    QMap<size_t, uint> m_interfaceOffsets;
    QSet<size_t> m_availableInterfaces;
    QMap<size_t, QSet<const std::type_info*>> m_inheritedInterfaces;
    friend QtJambiLink;
};

class QSharedPointerToObjectInterfaceLink : public QSharedPointerToObjectLink{
private:
    QSharedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                        const char* qt_name,
#endif
                                        int metaType, bool created_by_java, bool is_shell,
                                        PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, void* ptr_shared_pointer,
                                        PointerDeleter shared_pointer_deleter, PointerGetterFunction pointerGetter, JavaException& ocurredException);
public:
    ~QSharedPointerToObjectInterfaceLink() override;
    void unregisterOffsets() override;
    void addInterface(const std::type_info& qtInterfaceType) override;
    void removeInterface(const std::type_info& qtInterfaceType) override;
    bool isInterfaceAvailable(const std::type_info& qtInterfaceType) const override;
    void* typedPointer(const std::type_info& qtType) const override;
private:
    QMap<size_t, uint> m_interfaceOffsets;
    QSet<size_t> m_availableInterfaces;
    QMap<size_t, QSet<const std::type_info*>> m_inheritedInterfaces;
    friend QtJambiLink;
};

class QSharedPointerToQObjectInterfaceLink : public QSharedPointerToQObjectLink{
private:
    QSharedPointerToQObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces, QObject* object, bool created_by_java, bool is_shell, void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, PointerQObjectGetterFunction pointerGetter, JavaException& ocurredException);
public:
    ~QSharedPointerToQObjectInterfaceLink() override;
    virtual void unregisterOffsets() override;
    void addInterface(const std::type_info& qtInterfaceType) override;
    void removeInterface(const std::type_info& qtInterfaceType) override;
    bool isInterfaceAvailable(const std::type_info& qtInterfaceType) const override;
    void* typedPointer(const std::type_info& qtType) const override;
private:
    QMap<size_t, uint> m_interfaceOffsets;
    QSet<size_t> m_availableInterfaces;
    QMap<size_t, QSet<const std::type_info*>> m_inheritedInterfaces;
    friend QtJambiLink;
};

class QtJambiDestructor : public QObject
{

public:
    QtJambiDestructor(const QPointer<const QObject>& parent, const QSharedPointer<QtJambiLink>& link, void *pointer, int meta_type, QtJambiLink::Ownership ownership, PtrDeleterFunction deleter_function);
    ~QtJambiDestructor();
    bool event(QEvent * e);
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
    const QString& qtTypeName() const;
#endif

private:
    QPointer<const QObject> m_parent;
    QWeakPointer<QtJambiLink> m_link;
    void * m_pointer;
    int m_meta_type;
    QtJambiLink::Ownership m_ownership;
    PtrDeleterFunction m_deleter_function;
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
    QString m_qtTypeName;
#endif
};

class QtJambiLinkScope : public QtJambiScope{
public:
    QtJambiLinkScope(const QSharedPointer<QtJambiLink>& _link);
    Q_DISABLE_COPY_MOVE(QtJambiLinkScope)
};

#endif // QTJAMBILINK_P_H
