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

#include <algorithm>
#include <cstring>

#include <QtCore/private/qobject_p.h>
#include <QtCore/private/qmetaobject_p.h>
#include <QtCore/private/qobject_p.h>

#if defined(QTJAMBI_DEBUG_TOOLS)
#define ON_DEBUG_TOOLS_ENABLED(expression)expression;
#else
#define ON_DEBUG_TOOLS_ENABLED(expression)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
#define qAsConst std::as_const
#endif

#define AVAILABLE_IN_DELETION

#ifdef QTJAMBI_NO_METHOD_TRACE
#define QTJAMBI_DEBUG_PRINT_WITH_ARGS(...)
#define QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(...)
#else
#define QTJAMBI_DEBUG_PRINT_WITH_ARGS(...)\
if(!QtJambiLink::isDebugMessagingDisabled())\
    DebugAPI::printCleanupArgs(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(link, methodname)\
    DebugAPI::MethodPrintFromLink __debug_method_print(link, methodname, __FILE__, __LINE__, __FUNCTION__);
#endif

namespace DebugAPI{
void printCleanupArgs(const char *file, int line, const char *function, const char *format,...);
}

class SelfDeletingThread : public QThread
{
    Q_DECLARE_PRIVATE(QThread)
public:
    inline bool deleteLaterIfIsInFinish(){
        QMutexLocker<QMutex> locker(&d_func()->mutex);
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
        if(d_func()->isInFinish){
#else
        if(d_func()->threadState==QThreadPrivate::Finishing
                || d_func()->threadState==QThreadPrivate::Finished){
#endif
            this->deleteLater();
            return true;
        }else{
            return false;
        }
    }
};

struct QtJambiLinkWriteLock{
    QtJambiLinkWriteLock() : storage(), locker(storage ? storage->linkLock() : nullptr) {
    }
    void unlock(){
        locker.unlock();
    }
    void relock(){
        locker.relock();
    }
private:
    QtJambiStorageOptional storage;
    QWriteLocker locker;
};

struct QtJambiLinkReadLock{
    QtJambiLinkReadLock() : storage(), locker(storage ? storage->linkLock() : nullptr) {
    }
    void unlock(){
        locker.unlock();
    }
    void relock(){
        locker.relock();
    }
private:
    QtJambiStorageOptional storage;
    QReadLocker locker;
};

template<bool is_qobject = true, bool is_smart_pointer = false>
class QtJambiLinkImpl : public QtJambiLink{
protected:
    typedef std::conditional_t<is_qobject,QObject,void> T;
    QtJambiLinkImpl(JNIEnv *env, jobject nativeLink,
                LINK_NAME_ARG(const char* qt_name)
                T* pointer,
                SafeBool created_by_java,
                SafeBool is_shell,
                JavaException& ocurredException)
        : QtJambiLink(env, nativeLink,
                      LINK_NAME_ARG(qt_name)
                      created_by_java, is_shell, ocurredException),
          m_pointer(pointer)
    {
        if(ocurredException){
            m_pointer = nullptr;
            QtJambiLink::m_flags.setFlag(QtJambiLink::Flag::IsJavaLinkDetached);
        }else if constexpr(!is_qobject){
            if(created_by_java){
                registerPointer(m_pointer);
            }
        }
    }
public:
    static constexpr bool IS_QOBJECT = is_qobject;
    static constexpr bool IS_SMART_POINTER = is_smart_pointer;
    bool isSmartPointer() const override final{
        if constexpr(is_smart_pointer){
            return true;
        }else{
            return false;
        }
    }
    void *pointer() const override{
        if constexpr(is_qobject){
            return
#ifndef AVAILABLE_IN_DELETION
                isInDestructor() ||
#endif
                !isInitialized() ? nullptr : m_pointer;
        }else{
            return m_pointer;
        }
    }
    void *plainPointer() const override final{
        return m_pointer;
    }
    QObject *qobject() const override{
        if constexpr(is_qobject){
            return m_pointer;
        }else{
            Q_ASSERT_X(false, "QtJambiLink", "Pointer is not QObject");
            return nullptr;
        }
    }
    bool isQObject() const override final{
        if constexpr(is_qobject){
            return true;
        }else{
            return false;
        }
    }
    bool qobjectDeleted() const override final {
        if constexpr(is_qobject){
            return !m_pointer;
        }else{
            return false;
        }
    }
    void removeInterface(const std::type_info&) override{
        clearAllDependencies();
        setInDestructor();
    }
    void invalidate(JNIEnv *env) override {
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this, "QtJambiLink::invalidate(JNIEnv *)")
        invalidateDependentObjects(env);
        releaseJavaObject(env);
        void* pointer;
        {
            QtJambiLinkWriteLock locker;
            pointer = m_pointer;
            if constexpr(!is_qobject){
                m_flags.setFlag(QtJambiLink::Flag::IsPointerRegistered, false);
            }
            m_pointer = nullptr;
        }
        if(pointer){
            QTJAMBI_DEBUG_PRINT_WITH_ARGS("invalidate object %p of link %p", pointer, this)
            if constexpr(!is_qobject){
                unregisterPointer(pointer);
            }
            unregisterOffsets(pointer);
        }
        dispose();
    }
    template<bool enabled = is_qobject>
    std::enable_if_t<enabled,void> setAsQObjectDeleted() {
        if constexpr(is_qobject){
            QTJAMBI_DEBUG_PRINT_WITH_ARGS("QtJambiLink::setAsQObjectDeleted() object %p of link %p", m_pointer, this)
            {
                QtJambiLinkWriteLock locker;
                m_pointer = nullptr;
            }
        }
    }
    void setDefaultOwnership(JNIEnv *env) override
    {
        if (createdByJava()){
            setJavaOwnership(env);
        }else {
            if constexpr(is_qobject){
                if(m_pointer && m_pointer->parent())
                    setCppOwnership(env);
                else
                    setSplitOwnership(env);
            }else
                setSplitOwnership(env);
        }
    }

    void setJavaOwnership(JNIEnv *env) override
    {
        if(!m_flags.testFlag(Flag::JavaOwnership) && env){
            if(m_flags.testFlag(Flag::NoGlobalRef)){
                if(!Java::QtJambi::NativeUtility$NativeLink::releaseOwnershipForID(env, jlong(reinterpret_cast<void*>(this))))
                    return;
            }else{
                Java::QtJambi::NativeUtility$NativeLink::releaseOwnership(env, m_nativeLink);
            }
            m_flags.setFlag(Flag::OwnershipMask, false);
            m_flags.setFlag(Flag::JavaOwnership);
            if constexpr(!is_qobject)
                registerPointer(m_pointer);
        }
    }
protected:
    using QtJambiLink::m_flags;
    using QtJambiLink::setJavaOwnership;
    using QtJambiLink::detachJavaLink;
    using QtJambiLink::releaseJavaObject;
    using QtJambiLink::clearAllDependencies;
    using QtJambiLink::setInDestructor;
    using QtJambiLink::isInDestructor;
    using QtJambiLink::isInitialized;
    using QtJambiLink::invalidateDependentObjects;
    using QtJambiLink::unregisterOffsets;
    using QtJambiLink::dispose;
    using QtJambiLink::unregisterPointer;
    using QtJambiLink::createdByJava;
    using QtJambiLink::metaType;
    T *m_pointer;
};

template<bool is_qobject = false>
class ObjectLink : public QtJambiLinkImpl<is_qobject>{
    using Super = QtJambiLinkImpl<is_qobject>;
protected:
    virtual void deleteNow(JNIEnv *env, void*){
        env->Throw(Java::Runtime::RuntimeException::newInstanceWithMessage(env, "Unable to delete object due to missing deleter or meta type information."));
    }
    virtual void addDeleter(const QExplicitlySharedDataPointer<ValueOwnerObjectDataPrivate>&, void*){}
    virtual void postDeletion(const QPointer<const QObject>&, QThread*, QThreadObjectData*, void*){}
    using Super::Super;
    using Super::m_pointer;
    using QtJambiLink::m_flags;
    using QtJambiLink::isQThread;
    using QtJambiLink::releaseJavaObject;
    using QtJambiLink::detachJavaLink;
    ~ObjectLink() override{
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this, "ObjectLink::~ObjectLink()")
        if (!m_flags.testFlag(QtJambiLink::Flag::IsJavaLinkDetached) || !m_flags.testFlag(QtJambiLink::Flag::JavaObjectIsReleased) || m_pointer) {
            try{
                if(JniEnvironment env = isQThread() ? JniEnvironment{300} : DefaultJniEnvironment{300}){
                    releaseJavaObject(env);
                    detachJavaLink(env);
                }
            } catch (const std::exception& e) {
                qCWarning(DebugAPI::internalCategory, "%s", e.what());
            } catch (...) {
            }
        }
    }
    friend QtJambiLink;
};

template<>
class ObjectLink<true> : public QtJambiLinkImpl<true>{
    using Super = QtJambiLinkImpl<true>;
public:
    void init(JNIEnv* env) final override{
        init(env, nullptr);
    }
protected:
    ~ObjectLink() override{
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this, "QObjectLink::~QObjectLink()")
        if (!m_flags.testFlag(Flag::IsJavaLinkDetached) || !m_flags.testFlag(Flag::JavaObjectIsReleased) || m_pointer) {
            try{
                if(JniEnvironment env = isQThread() ? JniEnvironment{300} : DefaultJniEnvironment{300}){
                    releaseJavaObject(env);
                    if(isInitialized())
                        deleteNativeObject(env);
                    detachJavaLink(env);
                }else if(isInitialized()){
                    deleteNativeObject();
                }
            } catch (const std::exception& e) {
                qCWarning(DebugAPI::internalCategory, "%s", e.what());
            } catch (...) {
            }
        }
    }
    void deleteNativeObject(JNIEnv *env = nullptr, bool forced = false) final override{
        if(m_pointer && !isDeleteLater()){
            QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this, "QtJambiLink::deleteNativeObject(JNIEnv *, bool)")
            unregisterOffsets(m_pointer);
            if(!isShell())
                clearAllDependencies(env);

            if (!m_flags.testFlag(Flag::NoNativeDeletion) && (m_flags.testFlag(Flag::JavaOwnership) || forced)) {
                bool isCurrentThread = false;
                QThread *objectThread = m_pointer->thread();
                // Explicit dispose from current thread, delete object
                if(!objectThread){
                    if(m_pointer->parent()){
                        qCWarning(DebugAPI::internalCategory, "Skip deletion of QObject '%s' [%s] (thread=none, parent='%s' [%s])",
                                qPrintable(m_pointer->objectName()),
                                m_pointer->metaObject()->className(),
                                qPrintable(m_pointer->parent()->objectName()),
                                m_pointer->parent()->metaObject()->className());
                    }else{
                        QtJambiExceptionBlocker __qt_exceptionBlocker;
                        delete m_pointer;
                        m_pointer = nullptr;
                        __qt_exceptionBlocker.release(env);
                    }
                }else if ((isCurrentThread =
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
                                objectThread->isCurrentThread()
#else
                                objectThread == QThread::currentThread()
#endif
                            ) || !objectThread->isRunning() || objectThread==m_pointer) {
                    QTJAMBI_DEBUG_PRINT_WITH_ARGS("call delete on object %p of type %s", m_pointer, QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QObject>::trySupplyType(m_pointer)).constData())
                    if(m_pointer==objectThread || qobject_cast<QThread*>(m_pointer)){
                        QThread* myThread;
                        if(m_pointer==objectThread){
                            myThread = objectThread;
                        }else{
                            myThread = qobject_cast<QThread*>(m_pointer);
                        }
                        QThreadData * threadData = QThreadData::get2(myThread);
                        if(threadData->isAdopted){
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("Adopted thread expected to be automatically deleted")
                            return;
                        }
                        if(static_cast<SelfDeletingThread*>(myThread)->deleteLaterIfIsInFinish()){
                            setDeleteLater();
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("call deleteLater() on object %p of type %s", m_pointer, QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QObject>::trySupplyType(m_pointer)).constData())
                            return;
                        }
                    }
                    if(isCurrentThread){
                        QtJambiExceptionBlocker __qt_exceptionBlocker;
                        delete m_pointer;
                        m_pointer = nullptr;
                        __qt_exceptionBlocker.release(env);
                    }else if(!objectThread->isRunning() && !m_pointer->parent() && !m_pointer->isWidgetType()){
                        QtJambiExceptionBlocker __qt_exceptionBlocker;
                        delete m_pointer;
                        m_pointer = nullptr;
                        __qt_exceptionBlocker.release(env);
                    }else if(objectThread==m_pointer){
                        if(objectThread->isRunning()){
                            qCWarning(DebugAPI::internalCategory, "Trying to delete a running QThread '%s'",
                                    qPrintable(m_pointer->objectName()));
                        }
                        QtJambiExceptionBlocker __qt_exceptionBlocker;
                        delete m_pointer;
                        m_pointer = nullptr;
                        __qt_exceptionBlocker.release(env);
                    }else if(m_pointer->parent()){
                        qCWarning(DebugAPI::internalCategory, "Skip deletion of QObject '%s' [%s] (thread='%s', parent='%s' [%s])",
                                qPrintable(objectThread->objectName()),
                                qPrintable(m_pointer->objectName()),
                                m_pointer->metaObject()->className(),
                                qPrintable(m_pointer->parent()->objectName()),
                                m_pointer->parent()->metaObject()->className());
                    }else{
                        qCWarning(DebugAPI::internalCategory, "Skip deletion of QObject '%s' [%s] (thread='%s', no parent)",
                                qPrintable(objectThread->objectName()),
                                qPrintable(m_pointer->objectName()),
                                m_pointer->metaObject()->className());
                    }

                // We're in the main thread and we'll have an event loop
                // running, so its safe to call delete later.
                } else if (QAbstractEventDispatcher::instance(objectThread)) {
                    setDeleteLater();
                    QTJAMBI_DEBUG_PRINT_WITH_ARGS("call deleteLater() on object %p of type %s", m_pointer, QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QObject>::trySupplyType(m_pointer)).constData())
                    m_pointer->deleteLater();

                // If the QObject is in a non-main thread, check if that
                // thread is a QThread, in which case it will run an eventloop
                // and thus, do cleanup, hence deleteLater() is safe;
                // Otherwise issue a warning.
                }else{
                    // currentThread is not objectThread
                    // && objectThread not finished
                    // && objectThread not pointer
                    // && no QAbstractEventDispatcher
                    QThreadObjectData* qthreadData = QThreadObjectData::ensureThreadUserData(objectThread);
                    setDeleteLater();
                    QTJAMBI_DEBUG_PRINT_WITH_ARGS("attach QObject to thread shutdown procedure.")
                    qthreadData->deleteAtThreadEnd(m_pointer);
                }
            }else{
                // trust me it is possible that pointer is null
                m_pointer = nullptr;
                dispose();
            }
        }
    }
private:
    virtual void init(JNIEnv* env, QSharedPointer<QtJambiLink>* replacement) = 0;
    friend class QtJambiLink;
    template<bool>
    friend class QObjectPlainPointerLink;
    friend LinkObjectData;
    friend ThreadLinkObjectData;
    using Super::Super;
protected:
    using Super::m_pointer;
    using QtJambiLink::m_flags;
    using QtJambiLink::releaseJavaObject;
    using QtJambiLink::detachJavaLink;
};

template<typename Super>
class InterfaceLink : public Super{
protected:
    ~InterfaceLink() override{
        InterfaceLink::unregisterOffsets(plainPointer());
    }
public:
    void unregisterOffsets(void *pointer) override{
        if(pointer && !this->m_flags.testFlag(QtJambiLink::Flag::IsOffsetsUnregistered)){
            QVector<void*> pointers;
            for(auto i = m_interfaceOffsetInfo.offsets.keyValueBegin(); i!=m_interfaceOffsetInfo.offsets.keyValueEnd(); ++i){
                pointers << reinterpret_cast<void*>( quintptr(pointer) + i.base().value() );
            }
            m_flags.setFlag(QtJambiLink::Flag::IsOffsetsUnregistered);
            unregisterPointers(pointers);
        }
    }
    void addInterface(const std::type_info& qtInterfaceType) override{
        m_availableInterfaces << unique_id(qtInterfaceType);
        size_t offset = m_interfaceOffsetInfo.offsets.value(unique_id(qtInterfaceType));
        void* pointer = plainPointer();
        if(offset>0){
            registerPointer(reinterpret_cast<void*>( quintptr(pointer) + offset ));
        }else{
            const QSet<const std::type_info*> inheritedInterfaces = m_interfaceOffsetInfo.inheritedInterfaces[unique_id(qtInterfaceType)];
            for(const std::type_info* ifaces : inheritedInterfaces){
                Q_ASSERT(ifaces);
                addInterface(*ifaces);
            }
        }
    }
    void removeInterface(const std::type_info& qtInterfaceType) override{
        if(m_availableInterfaces.contains(unique_id(qtInterfaceType))){
            m_availableInterfaces.remove(unique_id(qtInterfaceType));
            void* pointer = plainPointer();
            if(m_availableInterfaces.isEmpty())
                clearAllDependencies();
            if(m_interfaceOffsetInfo.offsets.contains(unique_id(qtInterfaceType))){
                size_t offset = m_interfaceOffsetInfo.offsets.value(unique_id(qtInterfaceType));
                if(offset>0)
                    unregisterPointer(reinterpret_cast<void*>( quintptr(pointer) + offset ));
                else
                    setInDestructor();
            }else{
                const QSet<const std::type_info*> inheritedInterfaces = m_interfaceOffsetInfo.inheritedInterfaces[unique_id(qtInterfaceType)];
                for(const std::type_info* ifaces : inheritedInterfaces){
                    Q_ASSERT(ifaces);
                    removeInterface(*ifaces);
                }
                setInDestructor();
            }
        }
    }
    bool isInterfaceAvailable(const std::type_info& qtInterfaceType) const override{
        return m_availableInterfaces.contains(unique_id(qtInterfaceType));
    }
    void* typedPointer(const std::type_info& qtInterfaceType) const override{
        void* ptr = pointer();
        if(m_interfaceOffsetInfo.offsets.contains(unique_id(qtInterfaceType))){
            if(!m_availableInterfaces.contains(unique_id(qtInterfaceType)))
                return nullptr;
            size_t offset = m_interfaceOffsetInfo.offsets.value(unique_id(qtInterfaceType));
            if(offset>0)
                return reinterpret_cast<void*>( quintptr(ptr) + offset );
        }
        return ptr;
    }
    bool isMultiInheritanceType() const override{
        return true;
    }
    const InterfaceOffsetInfo* interfaceOffsetInfo() const override{
        return &m_interfaceOffsetInfo;
    }
protected:
    void setInterfaceOffsetInfo(const InterfaceOffsetInfo& interfaceOffsetInfo){
        if(m_interfaceOffsetInfo.inheritedInterfaces.isEmpty()){
            m_interfaceOffsetInfo = interfaceOffsetInfo;
            m_availableInterfaces = interfaceOffsetInfo.interfaces;
            quintptr pointer = quintptr(plainPointer());
            for(auto i = m_interfaceOffsetInfo.offsets.keyValueBegin(); i!=m_interfaceOffsetInfo.offsets.keyValueEnd(); ++i){
                QtJambiLink::registerPointer(reinterpret_cast<void*>( pointer + i.base().value() ));
            }
        }
    }
    using Super::plainPointer;
    using Super::registerPointer;
    using Super::unregisterPointer;
    using Super::setInDestructor;
    using Super::pointer;
    using Super::clearAllDependencies;
    using Super::unregisterPointers;
    using QtJambiLink::m_flags;
private:
    InterfaceOffsetInfo m_interfaceOffsetInfo;
    QSet<size_t> m_availableInterfaces;
    using Super::Super;
    friend QtJambiLink;
};

class DependentLinkInterface{
protected:
    DependentLinkInterface() = default;
    virtual ~DependentLinkInterface();
    virtual void setDependencyManager(DependencyManagerObjectData*) = 0;
    friend QtJambiLink;
    friend DependencyManagerObjectData;
};

DependentLinkInterface::~DependentLinkInterface(){}

template<typename Super>
class DependentLink : public Super, public DependentLinkInterface{
protected:
    void setDependencyManager(DependencyManagerObjectData* dependencyManager) override{
        m_dependencyManager = dependencyManager;
    }
    void deleteNativeObject(JNIEnv *env, bool forced) override{
        if(m_dependencyManager)
            m_dependencyManager->removeDependentObject(*this);
        Super::deleteNativeObject(env, forced);
    }
    void invalidate(JNIEnv *env) override{
        if(m_dependencyManager)
            m_dependencyManager->removeDependentObject(*this);
        Super::invalidate(env);
    }
private:
    using Super::Super;
    DependencyManagerObjectData* m_dependencyManager;
    friend QtJambiLink;
};

ExtendedLinkInterface::~ExtendedLinkInterface(){}

template<typename Super>
class ExtendedLink : public Super, public ExtendedLinkInterface{
protected:
public:
    inline bool hasExtension()const override{return !m_extension.isNull(); }
private:
    using Super::Super;
    QPointer<const QObject> m_extension;
    friend QtJambiLink;
};

template<typename T, class = decltype(std::declval<T>().getOwner(std::declval<void*>()) )>
std::true_type  hasGetOwner_test(const T&);
std::false_type hasGetOwner_test(...);
template<typename T> struct hasGetOwner : decltype(hasGetOwner_test(std::declval<T>())){};

template<typename Super>
class ContainerLink : public Super{
protected:
    ~ContainerLink() override {
        if(m_containerAccess)
            m_containerAccess->dispose();
    }
public:
    inline AbstractContainerAccess* containerAccess() const override { return m_containerAccess; }
protected:
    const QObject* getOwner(void* pointer) {
        Q_ASSERT(m_containerAccess);
        const QObject* result = m_containerAccess->getOwner(pointer);
        if constexpr(hasGetOwner<Super>::value){
            if(!result)
                result = Super::getOwner(pointer);
        }
        return result;
    }
    void deleteNow(JNIEnv *, void* pointer) override;
    void addDeleter(const QExplicitlySharedDataPointer<ValueOwnerObjectDataPrivate>& vud, void* pointer) override;
    void postDeletion(const QPointer<const QObject>& parent, QThread* ownerThread, QThreadObjectData* threadUserData, void* pointer) override;
    void init(JNIEnv* env) override {
        if(!Super::isInitialized()){
            Super::init(env);
            if(AbstractReferenceCountingContainer* rc = dynamic_cast<AbstractReferenceCountingContainer*>(m_containerAccess)){
                jobject object = getJavaObjectLocalRef(env);
                rc->updateRC(env, {object, plainPointer()});
                env->DeleteLocalRef(object);
            }
        }
    }
    void dispose(){
        if(m_containerAccess){
            m_containerAccess->dispose();
            m_containerAccess = nullptr;
        }
        Super::dispose();
    }
    AbstractContainerAccess * m_containerAccess = nullptr;
    using Super::Super;
    using Super::plainPointer;
    using Super::getJavaObjectLocalRef;
    friend class QtJambiLink;
};

template<typename Super>
class MetaTypedLink : public Super{
public:
    inline const QMetaType* metaType() const override {return &m_meta_type;}
protected:
    void addDeleter(const QExplicitlySharedDataPointer<ValueOwnerObjectDataPrivate>& vud, void* pointer) override;
    void postDeletion(const QPointer<const QObject>& parent, QThread* ownerThread, QThreadObjectData* threadUserData, void* pointer) override;
    void deleteNow(JNIEnv *env, void* pointer) override;
    QMetaType m_meta_type;
    using Super::Super;
    friend QtJambiLink;
};

template<typename Super>
class OwnedLink : public Super{
public:
    inline PtrOwnerFunction ownerFunction() const override {return m_owner_function;}
protected:
    const QObject* getOwner(void* pointer) {
        const QObject* result = m_owner_function(pointer);
        if constexpr(hasGetOwner<Super>::value){
            if(!result)
                result = Super::getOwner(pointer);
        }
        return result;
    }
    PtrOwnerFunction m_owner_function;
private:
    using Super::Super;
    friend QtJambiLink;
};

template<typename Super>
class DeletableLink : public Super{
public:
    inline PtrDeleterFunction deleterFunction() const override {return m_deleter_function;}
protected:
    void addDeleter(const QExplicitlySharedDataPointer<ValueOwnerObjectDataPrivate>& vud, void* pointer) override;
    void postDeletion(const QPointer<const QObject>& parent, QThread* ownerThread, QThreadObjectData* threadUserData, void* pointer) override;
    void deleteNow(JNIEnv *env, void* pointer) override;
    PtrDeleterFunction m_deleter_function;
private:
    using Super::Super;
    friend QtJambiLink;
};

template<typename Super, bool is_shared_pointer = Super::IS_SMART_POINTER>
struct OwnerInfo{
    void* pointer{nullptr};
    QPointer<const QObject> ownerPointer;
    QThread* ownerThread{nullptr};
    QExplicitlySharedDataPointer<ValueOwnerObjectDataPrivate> vud;
};
template<typename Super>
struct OwnerInfo<Super,true> : OwnerInfo<Super,false>{
    typedef typename std::conditional<std::is_same<typename Super::SmartPointerType,QWeakPointer<char>>::value, QSharedPointer<char>,
                                      typename std::conditional<std::is_same<typename Super::SmartPointerType,std::weak_ptr<char>>::value, std::shared_ptr<char>, typename Super::SmartPointerType>::type>::type SharedPointerT;
    SharedPointerT shared_pointer;
};

template<typename Super>
class OwnedObjectLink : public Super {
public:
    void deleteNativeObject(JNIEnv *env = nullptr, bool forced = false) override;
private:
    OwnerInfo<Super> clearPointer(bool prepareDeletion){
        OwnerInfo<Super> ownerInfo;
        QtJambiLinkWriteLock locker;
        if constexpr(Super::IS_SMART_POINTER){
            if constexpr(std::is_same<typename Super::SmartPointerType,QWeakPointer<char>>::value
                    || std::is_same<typename Super::SmartPointerType,std::weak_ptr<char>>::value){
                ownerInfo.shared_pointer = this->m_smartPointer.lock();
            }else{
                ownerInfo.shared_pointer = this->m_smartPointer;
            }
            this->m_smartPointer = typename Super::SmartPointerType();
        }
        ownerInfo.pointer = this->m_pointer;
        this->m_pointer = nullptr;
        if(prepareDeletion){
            if(const QObject* owner = getOwner(ownerInfo.pointer)){
                const QObjectPrivate* p = QObjectPrivate::get(owner);
                Q_ASSERT(p);
                QThread* thread = p->threadData.loadAcquire()->thread.loadAcquire();
                if (thread && thread->isRunning()
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
                        && !thread->isCurrentThread()
#else
                        && thread != QThread::currentThread()
#endif
                    ) {
                    if(ValueOwnerObjectData* d = QtJambiObjectData::userData<ValueOwnerObjectData>(owner)){
                        ownerInfo.vud = d->d;
                    }
                    if(!p->wasDeleted)
                        ownerInfo.ownerPointer = owner;
                    ownerInfo.ownerThread = thread;
                }
            }
        }
        return ownerInfo;
    }
protected:
    using Super::getOwner;
    using QtJambiLink::m_flags;
    using QtJambiLink::isInitialized;
private:
    using Super::Super;
    friend QtJambiLink;
};

class PlainObjectLink : public ObjectLink<false>{
    using Super = ObjectLink<false>;
public:
    void deleteNativeObject(JNIEnv *env, bool forced = false) override;
private:
    using Super::Super;
    friend QtJambiLink;
};

using PointerToContainerLink = OwnedObjectLink<ContainerLink<ObjectLink<false>>>;
using DeletableObjectLink = DeletableLink<PlainObjectLink>;
using MetaTypedObjectLink = MetaTypedLink<PlainObjectLink>;
using OwnedMetaTypedObjectLink = OwnedObjectLink<OwnedLink<MetaTypedObjectLink>>;
using DeletableOwnedObjectLink = OwnedObjectLink<DeletableLink<OwnedLink<ObjectLink<false>>>>;
using DeletablePointerToContainerLink = OwnedObjectLink<DeletableLink<ContainerLink<ObjectLink<false>>>>;
using DeletableOwnedPointerToContainerLink = OwnedObjectLink<OwnedLink<DeletableLink<ContainerLink<ObjectLink<false>>>>>;

template<template<typename> class SmartPointer, typename T>
class SmartPointerLink : public QtJambiLinkImpl<std::is_same_v<T,QObject>,true>{
    using Super = QtJambiLinkImpl<std::is_same_v<T,QObject>,true>;
public:
    typedef SmartPointer<T> SmartPointerType;
protected:
    typedef std::conditional_t<std::is_same_v<T,QObject>, QObject, void> PointerType;
    SmartPointerLink(JNIEnv *env, jobject nativeLink,
                     LINK_NAME_ARG(const char *qt_name)
                     SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                     PointerType* pointer,
                     SmartPointerType*& smartPointer,
                     JavaException& ocurredException);
    ~SmartPointerLink() override{}
    template<template<typename> class SmartPointer_, typename T_>
    inline operator QSharedPointer<SmartPointerLink<SmartPointer_,T_>>() const {
        return QtJambiLink::getStrongPointer().template staticCast<SmartPointerLink<SmartPointer_,T_>>();
    }
    template<template<typename> class SmartPointer_, typename T_>
    inline operator QWeakPointer<SmartPointerLink<SmartPointer_,T_>>() const {
        return QtJambiLink::getStrongPointer().template staticCast<SmartPointerLink<SmartPointer_,T_>>();
    }
public:
    typedef typename std::conditional<std::is_same<SmartPointerType,QWeakPointer<T>>::value, QSharedPointer<T>,
                                      typename std::conditional<std::is_same<SmartPointerType,std::weak_ptr<T>>::value, std::shared_ptr<T>, SmartPointerType>::type>::type SharedPointerT;
    void setCppOwnership(JNIEnv *env) override final;
    void setSplitOwnership(JNIEnv *env) override final;
    void setJavaOwnership(JNIEnv *env) override final;
    void setDefaultOwnership(JNIEnv *env) override final;
    QtJambiLink::Ownership ownership() const override final;
    virtual SharedPointerT getSmartPointer() const;
private:
    friend QtJambiLink;
    using QtJambiLink::qobject;
    using QtJambiLink::createdByJava;
    template<template<typename> class, typename>
    friend struct SmartPointerDeleter;
protected:
    using Super::m_pointer;
    SmartPointerType m_smartPointer;
    QtJambiLink::Ownership m_hiddenOwnership;
};

template<template<typename> class SmartPointer>
class ObjectSmartPointerLink : public SmartPointerLink<SmartPointer,char>{
    using Super = SmartPointerLink<SmartPointer,char>;
protected:
    using Super::Super;
    ~ObjectSmartPointerLink() override;
public:
    typename Super::SharedPointerT getSmartPointer() const override;
    bool isPointerZeroed() const { return m_flags.testFlag(QtJambiLink::Flag::IsDeleteLater); }
    void setPointerZeroed(){ m_flags.setFlag(QtJambiLink::Flag::IsDeleteLater); }
protected:
    virtual void deleteNow(JNIEnv *, void*){}
    virtual void addDeleter(const QExplicitlySharedDataPointer<ValueOwnerObjectDataPrivate>&, void*){}
    virtual void postDeletion(const QPointer<const QObject>&, QThread*, QThreadObjectData*, void*){}
    using Super::m_pointer;
    friend QtJambiLink;
    using QtJambiLink::m_flags;
};

template<template<typename> class SmartPointer>
class PlainObjectSmartPointerLink : public ObjectSmartPointerLink<SmartPointer>{
    using Super = ObjectSmartPointerLink<SmartPointer>;
public:
    void deleteNativeObject(JNIEnv *env = nullptr, bool forced = false) override;
private:
    using Super::Super;
    friend QtJambiLink;
};

template<template<typename> class SmartPointer>
using MetaTypedObjectSmartPointerLink = MetaTypedLink<PlainObjectSmartPointerLink<SmartPointer>>;
template<template<typename> class SmartPointer>
using DeletableObjectSmartPointerLink = DeletableLink<PlainObjectSmartPointerLink<SmartPointer>>;
template<template<typename> class SmartPointer>
using PlainOwnedObjectSmartPointerLink = OwnedObjectLink<OwnedLink<ObjectSmartPointerLink<SmartPointer>>>;
template<template<typename> class SmartPointer>
using OwnedMetaTypedObjectSmartPointerLink = MetaTypedLink<PlainOwnedObjectSmartPointerLink<SmartPointer>>;
template<template<typename> class SmartPointer>
using OwnedDeletableObjectSmartPointerLink = DeletableLink<PlainOwnedObjectSmartPointerLink<SmartPointer>>;
template<template<typename> class SmartPointer>
using ContainerObjectSmartPointerLink = OwnedObjectLink<ContainerLink<ObjectSmartPointerLink<SmartPointer>>>;

template<typename Super>
class ExtraSignalsLink : public Super{
public:
    jobject getExtraSignal(JNIEnv * env, const QMetaMethod& method) const override{
        if(QObject* object = Super::m_pointer){
            QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(object->metaObject());
            if(extraSignals.contains(method)){
                bool contains;
                {
                    auto locker = QtJambiObjectData::readLock();
                    contains = m_extraSignals.contains(method.methodIndex());
                    if(contains){
                        return m_extraSignals.value(method.methodIndex()).object(env);
                    }
                }
                if(!contains){
                    JObjectWrapper signal = QtJambiMetaObject::resolveExtraSignal(env, QtJambiLink::getJavaObjectLocalRef(env), method);
                    {
                        auto locker = QtJambiObjectData::writeLock();
                        if(!m_extraSignals.contains(method.methodIndex()))
                            m_extraSignals[method.methodIndex()] = signal;
                        else return m_extraSignals.value(method.methodIndex()).object(env);
                    }
                    return signal.object(env);
                }
            }
        }
        return nullptr;
    }
protected:
    using Super::m_pointer;
private:
    using Super::Super;
    mutable QHash<int,JObjectWrapper> m_extraSignals;
    friend QtJambiLink;
};

using QObjectPointerLink = ObjectLink<true>;

template<bool pending = false>
class QObjectPlainPointerLink : public std::conditional_t<pending, ExtraSignalsLink<QObjectPointerLink>, QObjectPointerLink> {
    using Super = std::conditional_t<pending, ExtraSignalsLink<QObjectPointerLink>, QObjectPointerLink>;
    QObjectPlainPointerLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                              const QMetaObject* metaObject, QObject *ptr, SafeBool isQThread,
                              SafeBool created_by_java, SafeBool is_shell,
                              JavaException& ocurredException);
    using QObjectPointerLink::init;
    void init(JNIEnv* env, QSharedPointer<QtJambiLink>* replacement) override;
    friend QtJambiLink;
protected:
    using QObjectPointerLink::m_pointer;
};

using PendingQObjectPointerLink = QObjectPlainPointerLink<true>;
using PendingInterfaceQObjectPointerLink = InterfaceLink<PendingQObjectPointerLink>;
using PlainQObjectPointerLink = QObjectPlainPointerLink<false>;
using InterfaceQObjectPointerLink = InterfaceLink<PlainQObjectPointerLink>;
using ExtraSignalsInterfaceQObjectPointerLink = ExtraSignalsLink<InterfaceQObjectPointerLink>;
using ExtraSignalsQObjectPointerLink = ExtraSignalsLink<PlainQObjectPointerLink>;

template<template<typename> class SmartPointer>
class QObjectSmartPointerLink : public SmartPointerLink<SmartPointer,QObject>{
    using Super = SmartPointerLink<SmartPointer,QObject>;
protected:
    QObjectSmartPointerLink(JNIEnv *env, jobject nativeLink,
                              LINK_NAME_ARG(const char *qt_name)
                              SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                              QObject* pointer,
                              SmartPointer<QObject>*& smartPointer,
                              JavaException& ocurredException);
    ~QObjectSmartPointerLink() override;
public:
    typename Super::SharedPointerT getSmartPointer() const override;
    void removeInterface(const std::type_info&) override{
        setInDestructor();
    }
    bool isDeleteLater() const { return m_flags.testFlag(QtJambiLink::Flag::IsDeleteLater); }
private:
    void deleteNativeObject(JNIEnv *env = nullptr, bool forced = false) override final;
    void setDeleteLater(){ m_flags.setFlag(QtJambiLink::Flag::IsDeleteLater); }
    friend QtJambiLink;
    friend LinkObjectData;
    friend ThreadLinkObjectData;
protected:
    using Super::m_pointer;
    using QtJambiLink::m_flags;
    using QtJambiLink::setInDestructor;
};

template<template<typename> class SmartPointer>
class PlainQObjectSmartPointerLink : public QObjectSmartPointerLink<SmartPointer>{
    PlainQObjectSmartPointerLink(JNIEnv *env,
                                   jobject nativeLink,
                                   jobject jobj,
                                   SafeBool created_by_java,
                                   QtJambiLink::Ownership ownership,
                                   SafeBool is_shell,
                                   QObject* pointer,
                                   SafeBool isQThread,
                                   SmartPointer<QObject>*& smartPointer,
                                   JavaException& ocurredException);
    friend QtJambiLink;
};

template<template<typename> class SmartPointer>
class QObjectWithPendingExtraSignalsSmartPointerLink : public ExtraSignalsLink<QObjectSmartPointerLink<SmartPointer>>{
    QObjectWithPendingExtraSignalsSmartPointerLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                                                     SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                     QObject* pointer,
                                                     SafeBool isQThread,
                                                     SmartPointer<QObject>*& smartPointer,
                                                     const QMetaObject* superTypeForCustomMetaObject,
                                                     JavaException& ocurredException);
    friend QtJambiLink;
};

template<template<typename> class SmartPointer>
using QObjectWithExtraSignalsSmartPointerLink = ExtraSignalsLink<PlainQObjectSmartPointerLink<SmartPointer>>;

bool operator !(QtJambiNativeID nativeId) { return nativeId == InvalidNativeID; }

bool operator &&(QtJambiNativeID nativeId, QtJambiNativeID nativeId2) { return nativeId != InvalidNativeID && nativeId2 != InvalidNativeID; }
bool operator &&(QtJambiNativeID nativeId, bool b2) { return nativeId != InvalidNativeID && b2; }
bool operator &&(bool b1, QtJambiNativeID nativeId) { return b1 && nativeId != InvalidNativeID; }
bool operator ||(QtJambiNativeID nativeId, QtJambiNativeID nativeId2) { return nativeId != InvalidNativeID || nativeId2 != InvalidNativeID; }
bool operator ||(QtJambiNativeID nativeId, bool b2) { return nativeId != InvalidNativeID || b2; }
bool operator ||(bool b1, QtJambiNativeID nativeId) { return b1 || nativeId != InvalidNativeID; }

#if defined(QTJAMBI_DEBUG_TOOLS) || !defined(QT_NO_DEBUG)
static TestAPI::DebugCounter staticIncreaser = nullptr;
namespace TestAPI{
void registerDebugCounter(DebugCounter increaser){
    staticIncreaser = increaser;
}
void increaseDebugCounter(TestAPI::DebugCounters debugCounter, const char*className){
    if(staticIncreaser)
        staticIncreaser(debugCounter,className);
}
}
#else
namespace TestAPI{
void registerDebugCounter(DebugCounter){}
}
#endif

class AbstractDestructionEvent : public AbstractThreadEvent
{
protected:
    AbstractDestructionEvent(const QPointer<const QObject>& parent, const QWeakPointer<QtJambiLink>& link, void *pointer);
    AbstractDestructionEvent(const AbstractDestructionEvent& clone);
private:
    void execute() override;
    virtual void destruct(void * pointer, const QSharedPointer<QtJambiLink>& link) = 0;
private:
    QPointer<const QObject> m_parent;
    QWeakPointer<QtJambiLink> m_link;
    void * m_pointer;
};

AbstractDestructionEvent::AbstractDestructionEvent(const QPointer<const QObject>& parent, const QWeakPointer<QtJambiLink>& link, void *pointer)
    : AbstractThreadEvent(),
      m_parent(parent),
      m_link(link),
      m_pointer(pointer)
{
}

AbstractDestructionEvent::AbstractDestructionEvent(const AbstractDestructionEvent& clone)
    : AbstractThreadEvent(clone),
      m_parent(clone.m_parent),
      m_link(clone.m_link),
      m_pointer(clone.m_pointer)
{
}

void AbstractDestructionEvent::execute(){
    if(m_pointer != nullptr){
        QThread* parentThread{nullptr};
        if(m_parent && (parentThread = m_parent->thread()) &&
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
                !parentThread->isCurrentThread()
#else
                parentThread!=QThread::currentThread()
#endif
        ){
            QThreadObjectData* threadData = QThreadObjectData::ensureThreadUserData(parentThread);
            QCoreApplication::postEvent(threadData->threadDeleter(), clone());
            return;
        }
        QSharedPointer<QtJambiLink> link = m_link.toStrongRef();
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(link, "AbstractDestructionEvent::execute()")
        destruct(m_pointer, link);
        m_pointer = nullptr;
        // This cannot be deleted before now, since the type may have a virtual destructor and may be a shell class object,
        // which means it will try to access its link. But everything is ready for
        // deletion, as this was done when the java object was finalized.
        if(link){
            JniEnvironment env{200};
            link->invalidate(env);
        }
        // continue to delete
    }
}

class DestructorEvent : public AbstractDestructionEvent
{
public:
    DestructorEvent(const QPointer<const QObject>& parent, const QWeakPointer<QtJambiLink>& link, void *pointer, PtrDeleterFunction deleter_function, bool isShell);
private:
    DestructorEvent(const DestructorEvent& clone);
    void destruct(void * pointer, const QSharedPointer<QtJambiLink>& link) override;
    DestructorEvent* clone() const override;
    PtrDeleterFunction m_deleter_function;
    bool m_isShell;
};

class ContainerDestructionEvent : public AbstractDestructionEvent
{
public:
    ContainerDestructionEvent(const QPointer<const QObject>& parent, const QWeakPointer<QtJambiLink>& link, void *pointer, AbstractContainerAccess * containerAccess);
private:
    ContainerDestructionEvent(const ContainerDestructionEvent& clone);
    ~ContainerDestructionEvent();
    void destruct(void * pointer, const QSharedPointer<QtJambiLink>& link) override;
    ContainerDestructionEvent* clone() const override;
    AbstractContainerAccess * m_containerAccess;
};

class MetaTypeDestructionEvent : public AbstractDestructionEvent
{
public:
    MetaTypeDestructionEvent(const QPointer<const QObject>& parent, const QWeakPointer<QtJambiLink>& link, void *pointer, const QMetaType& meta_type);
private:
    MetaTypeDestructionEvent(const MetaTypeDestructionEvent& clone);
    void destruct(void * pointer, const QSharedPointer<QtJambiLink>& link) override;
    MetaTypeDestructionEvent* clone() const override;
    QMetaType m_meta_type;
};

DestructorEvent::DestructorEvent(const QPointer<const QObject>& parent, const QWeakPointer<QtJambiLink>& link, void *pointer, PtrDeleterFunction destructor_function, bool isShell)
    : AbstractDestructionEvent(parent, link, pointer),
      m_deleter_function(destructor_function),
      m_isShell(isShell)
{
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(link, "new DestructorEvent()")
    Q_ASSERT(m_deleter_function);
}

DestructorEvent::DestructorEvent(const DestructorEvent& clone)
    : AbstractDestructionEvent(clone),
      m_deleter_function(clone.m_deleter_function),
      m_isShell(clone.m_isShell)
{
}

DestructorEvent* DestructorEvent::clone() const {
    return new DestructorEvent(*this);
}

void DestructorEvent::destruct(void * pointer, const QSharedPointer<QtJambiLink>& link){
    Q_UNUSED(link)
    QTJAMBI_INCREASE_COUNTER(destructorFunctionCalledCount, link)
    m_deleter_function(pointer, m_isShell);
}

MetaTypeDestructionEvent::MetaTypeDestructionEvent(const QPointer<const QObject>& parent, const QWeakPointer<QtJambiLink>& link, void *pointer, const QMetaType& meta_type)
    : AbstractDestructionEvent(parent, link, pointer),
      m_meta_type(meta_type)
{
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(link, "new MetaTypeDestructionEvent()")
}

MetaTypeDestructionEvent::MetaTypeDestructionEvent(const MetaTypeDestructionEvent& clone)
    : AbstractDestructionEvent(clone),
      m_meta_type(clone.m_meta_type)
{
}

MetaTypeDestructionEvent* MetaTypeDestructionEvent::clone() const {
    return new MetaTypeDestructionEvent(*this);
}

void MetaTypeDestructionEvent::destruct(void * pointer, const QSharedPointer<QtJambiLink>& link){
    Q_UNUSED(link)
    QTJAMBI_INCREASE_COUNTER(destructorFunctionCalledCount, link)
    m_meta_type.destroy(pointer);
}

ContainerDestructionEvent::ContainerDestructionEvent(const QPointer<const QObject>& parent, const QWeakPointer<QtJambiLink>& link, void *pointer, AbstractContainerAccess * containerAccess)
     : AbstractDestructionEvent(parent, link, pointer),
       m_containerAccess(containerAccess)
{
     QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(link, "new ContainerDestructionEvent()")
     Q_ASSERT(m_containerAccess);
}

ContainerDestructionEvent::ContainerDestructionEvent(const ContainerDestructionEvent& clone)
    : AbstractDestructionEvent(clone),
      m_containerAccess(clone.m_containerAccess->clone())
{
}

ContainerDestructionEvent::~ContainerDestructionEvent(){
    if(m_containerAccess)
        m_containerAccess->dispose();
}

void ContainerDestructionEvent::destruct(void * pointer, const QSharedPointer<QtJambiLink>& link){
    Q_UNUSED(link)
    QTJAMBI_INCREASE_COUNTER(destructorFunctionCalledCount, link)
    m_containerAccess->deleteContainer(pointer);
    m_containerAccess->dispose();
    m_containerAccess = nullptr;
}

ContainerDestructionEvent* ContainerDestructionEvent::clone() const{
    return new ContainerDestructionEvent(*this);
}

template<typename Super>
class DanglingPointerCheckingLink : public Super{
public:
    void *pointer() const override{
        void* ptr = m_pointer;
        if(ptr){
            if(checkedGetTypeInfo(m_typeInfoSupplier, ptr)==nullptr){
                if(JniEnvironment env{128}){
                    if(jobject obj = getJavaObjectLocalRef(env)){
                        QString className = QtJambiAPI::getObjectClassName(env, obj).replace(QLatin1Char('/'), QLatin1Char('.')).replace(QLatin1Char('$'), QLatin1Char('.'));
                        Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Object of type %s points to dangling pointer %p", qPrintable(className), ptr) QTJAMBI_STACKTRACEINFO );
                    }else{
                        Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Dangling pointer %p detected", ptr) QTJAMBI_STACKTRACEINFO );
                    }
                }else{
                    ptr = nullptr;
                }
            }
        }
        return ptr;
    }
private:
    using Super::Super;
    using Super::m_pointer;
    TypeInfoSupplier m_typeInfoSupplier = nullptr;
    using QtJambiLink::getJavaObjectLocalRef;
    friend QtJambiLink;
};

template<bool checkDanglingPointer = true>
void checkValueOwner(JNIEnv* env, PtrOwnerFunction ownerFunction, void* ptr){
    if(const QObject* obj = ownerFunction(ptr)){
        if constexpr(checkDanglingPointer){
            QtJambiAPI::checkDanglingPointer(env, obj);
        }
        const QObjectPrivate* p = QObjectPrivate::get(obj);
        if(p && !p->wasDeleted){
            ValueOwnerObjectData* vod{nullptr};
            {
                auto locker = QtJambiObjectData::readLock();
                vod = QtJambiObjectData::userData<ValueOwnerObjectData>(obj);
            }
            if(!vod){
                vod = new ValueOwnerObjectData(obj);
                {
                    auto locker = QtJambiObjectData::writeLock();
                    ValueOwnerObjectData* _vod = QtJambiObjectData::userData<ValueOwnerObjectData>(obj);
                    if(!_vod)
                        vod = QtJambiObjectData::setUserData<ValueOwnerObjectData>(const_cast<QObject*>(obj), vod);
                }
                if(vod)
                    delete vod;
            }
        }
    }
}

template<typename Super>
class PendingLink : public Super{
    void init(JNIEnv* env) override {
        if(!Super::isInitialized()){
            Super::init(env);
            checkValueOwner<false>(env, Super::ownerFunction(), Super::pointer());
        }
    }
    using Super::Super;
    friend class QtJambiLink;
};

#define THREAD_ID() reinterpret_cast<void*>(QThread::currentThreadId())

#if defined(QTJAMBI_DEBUG_TOOLS)
/*defined QTJAMBI_DEBUG_TOOLS*/
void QtJambiLink::registerLink(QtJambiLink* link)
{
    if(link->m_flags.testFlag(Flag::IsListed)) {
        char buf[24];
        fprintf(stderr, "QtJambiLink::registerLink(%p) ERROR: LINK ALREADY LISTED, flags=%s, qtType=%s\n", reinterpret_cast<void*>(link), link->debugFlagsToString(buf), link->qtTypeName());
        return;
    }

    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker lock(storage->lock());
        storage->linkSet().insert(link);
        link->m_flags.setFlag(Flag::IsListed);
    }
}

/*defined QTJAMBI_DEBUG_TOOLS*/
void QtJambiLink::unregisterLink(QtJambiLink* link)
{
    if(!link->m_flags.testFlag(Flag::IsListed)) {
        char buf[24];
        fprintf(stderr, "QtJambiLink::unregisterLink(%p) ERROR: LINK NOT LISTED, flags=%s, qtType=%s\n", reinterpret_cast<void*>(link), link->debugFlagsToString(buf), link->qtTypeName());
        return;
    }

    if(QtJambiStorageOptional storage{}){
        QWriteLocker lock(storage->lock());
        storage->linkSet().remove(link);
        link->m_flags.setFlag(Flag::IsListed, false);
    }
}

/*defined QTJAMBI_DEBUG_TOOLS*/
/* static */
int QtJambiLink::dumpLinks(JNIEnv * env)
{
    int count = 0;
    QtJambiStorage::LinkSet set;
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker lock(storage->lock());
        set = storage->linkSet();
    }
    for(QtJambiLink* _link : qAsConst(set)) {
        QSharedPointer<QtJambiLink> sharedPointer = *_link;
        if(!sharedPointer)
            continue;
        jobject obj = _link->getJavaObjectLocalRef(env);
        QString jclassName = obj ? QtJambiAPI::getObjectClassName(env, obj) : QStringLiteral(u"null");
        jobject nativeLink = _link->nativeLink(env);
        jlong native_link = nativeLink ? Java::QtJambi::NativeUtility$NativeLink::nativeId(env, nativeLink) : 0;
        char buf[24];
        if(_link->isQObject()){
            QString objectName;
            bool hasParent = false;
            QThread* objectThread = nullptr;
            if(QObject* qobj = _link->qobject()){
                objectName = qobj->objectName();
                objectThread = qobj->thread();
                hasParent = qobj->parent();
                QThread* objectAsThread = qobject_cast<QThread*>(qobj);
                if(objectAsThread && objectAsThread->isRunning())
                    continue;
            }
            fprintf(stderr, "QtJambiLink(%p) ALIVE: { java_object=%p (%s), java_native_link=%p (%s), ownership=%s, is_qobject=true, pointer=%p, delete_later=%s, qtType=%s, objectName='%s', hasParent=%s, object_thread='%s', flags=%s }\n",
                    reinterpret_cast<void*>(_link),
                    static_cast<void*>(obj), qPrintable(jclassName),
                    static_cast<void*>(nativeLink),
                    nativeLink ? qPrintable(QStringLiteral(u"%1").arg((native_link==jlong(_link) ? "attached" : (native_link==0 ? "detached" : "dangled")))) : "detached",
                    _link->m_flags.testFlag(Flag::CppOwnership) ? "Cpp" : (_link->m_flags.testFlag(Flag::SplitOwnership) ? "Split" : "Java"),
                    static_cast<void*>(_link->pointer()), _link->isDeleteLater() ? "true" : "false",
                    !_link->m_qtTypeName ? "<unknown>" : _link->m_qtTypeName,
                    qPrintable(objectName),
                    hasParent ? "true" : "false",
                    objectThread ? qPrintable(objectThread->objectName()) : "unknown",
                    _link->debugFlagsToString(buf));
        }else{
            fprintf(stderr, "QtJambiLink(%p) ALIVE: { java_object=%p (%s), java_native_link=%p (%s), ownership=%s, is_qobject=false, pointer=%p, delete_later=%s, qtType=%s, flags=%s }\n",
                    reinterpret_cast<void*>(_link),
                    static_cast<void*>(obj), qPrintable(jclassName),
                    static_cast<void*>(nativeLink),
                    nativeLink ? qPrintable(QStringLiteral(u"%1").arg((native_link==jlong(_link) ? "attached" : (native_link==0 ? "detached" : "dangled")))) : "detached",
                    _link->m_flags.testFlag(Flag::CppOwnership) ? "Cpp" : (_link->m_flags.testFlag(Flag::SplitOwnership) ? "Split" : "Java"),
                    _link->pointer(), _link->isDeleteLater() ? "true" : "false",
                    !_link->m_qtTypeName ? "<unknown>" : _link->m_qtTypeName,
                    _link->debugFlagsToString(buf));
        }
        count++;
    }
    fflush(stderr);
    return count;
}

/*defined QTJAMBI_DEBUG_TOOLS*/
bool QtJambiLink::hasDeleteLaterEvents()
{
    QtJambiStorage::LinkSet set;
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker lock(storage->lock());
        set = storage->linkSet();
    }
    for(QtJambiLink* _link : qAsConst(set)) {
        QSharedPointer<QtJambiLink> linkPtr = *_link;
        if(!linkPtr)
            continue;
        if(_link->isQObject()){
            if(_link->plainPointer()==QCoreApplicationPrivate::theMainThread.loadRelaxed())
                continue;
            QObject* qobj;
            QThread* thread;
            if(_link->isDeleteLater() && (qobj = _link->qobject())
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
                    && (thread = qobj->thread()) && thread->isCurrentThread()){
#else
                    && qobj->thread()==QThread::currentThread()){
#endif
                return true;
            }
        }
    }
    return false;
}

#endif // defined(QTJAMBI_DEBUG_TOOLS)

QtJambiLinkObjectData::QtJambiLinkObjectData(const QWeakPointer<QtJambiLink> & link)
  : m_link(link) {}

void QtJambiLink::resetDataLink(QtJambiLinkObjectData* data){
    data->resetLink(*this);
}

MetaObjectLinkObjectData::MetaObjectLinkObjectData(const QWeakPointer<QtJambiLink> & link, const QMetaObject* metaObject)
  : LinkObjectData(link),
    m_metaObject(metaObject){}

ThreadLinkObjectDataImpl::ThreadLinkObjectDataImpl(QThread* thread)
    : m_threadId(nullptr){
    if(thread){
        if(QThreadData* td = QThreadData::get2(thread)){
            m_threadId = td->threadId.loadRelaxed();
        }
    }
}

namespace ThreadPrivate{
void purgeLink(QSharedPointer<QtJambiLink>&& link);
}

ThreadLinkObjectData::ThreadLinkObjectData(QThread* thread, const QWeakPointer<QtJambiLink> & link)
    : QtJambiLinkObjectData(link), ThreadLinkObjectDataImpl(thread) {
}

ThreadLinkObjectData::~ThreadLinkObjectData(){
    QSharedPointer<QtJambiLink> link = m_link.toStrongRef();
    m_link.clear();
    if (link && link->isQObject()) {
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(link, "QtJambiLinkObjectData::~QtJambiLinkObjectData()")
        QTJAMBI_INCREASE_COUNTER(userDataDestroyedCount, link)
        if(link->isSmartPointer()){
            switch(link->smartPointerType()){
            case QtJambiLink::Flag::QSharedPointer:
                reinterpret_cast<QObjectSmartPointerLink<QSharedPointer>*>(link.data())->setAsQObjectDeleted();
                break;
            case QtJambiLink::Flag::QWeakPointer:
                reinterpret_cast<QObjectSmartPointerLink<QWeakPointer>*>(link.data())->setAsQObjectDeleted();
                break;
            case QtJambiLink::Flag::shared_ptr:
                reinterpret_cast<QObjectSmartPointerLink<std::shared_ptr>*>(link.data())->setAsQObjectDeleted();
                break;
            case QtJambiLink::Flag::weak_ptr:
                reinterpret_cast<QObjectSmartPointerLink<std::weak_ptr>*>(link.data())->setAsQObjectDeleted();
                break;
            default: break;
            }
        }else{
            reinterpret_cast<QObjectPointerLink*>(link.data())->setAsQObjectDeleted();
        }
        // only reset the link when not a shell. Shells do this operation during shell destruction.
        // => QtJambiShellImpl::deleteShell()
        if(!link->isShell()){
            if(link->isAdoptedThread() || (m_threadId && QThread::currentThreadId()!=m_threadId)){
                if(DefaultJniEnvironment env{200}){
                    QTJAMBI_TRY{
                        link->invalidate(env);
                        link.reset();
                    }QTJAMBI_CATCH(const JavaException& exn){
                        exn.report(env);
                    }QTJAMBI_TRY_END
                }
            }else{
                struct FinalJniEnvironment : JniEnvironment{
                    FinalJniEnvironment()
                        : JniEnvironment(int(JniEnvironmentFlag::NoAttachCurrentThread), 0){}
                };
                if(FinalJniEnvironment env{}){
                    QTJAMBI_TRY{
                        link->invalidate(env);
                        link.reset();
                    }QTJAMBI_CATCH(const JavaException& exn){
                        exn.report(env);
                    }QTJAMBI_TRY_END
                }
            }
            if(link)
                ThreadPrivate::purgeLink(std::move(link));
        }
    }
}

MetaObjectThreadLinkObjectData::MetaObjectThreadLinkObjectData(QThread* thread, const QWeakPointer<QtJambiLink> & link, const QMetaObject* metaObject)
    : ThreadLinkObjectData(thread, link), m_metaObject(metaObject) {
}

LinkObjectData::~LinkObjectData()
{
    try{
        QSharedPointer<QtJambiLink> link = m_link.toStrongRef();
        m_link.clear();
        if (link && link->isQObject()) {
            QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(link, "QtJambiLinkObjectData::~QtJambiLinkObjectData()")
            QTJAMBI_INCREASE_COUNTER(userDataDestroyedCount, link)
            if(link->isSmartPointer()){
                switch(link->smartPointerType()){
                case QtJambiLink::Flag::QSharedPointer:
                    reinterpret_cast<QObjectSmartPointerLink<QSharedPointer>*>(link.data())->setAsQObjectDeleted();
                    break;
                case QtJambiLink::Flag::QWeakPointer:
                    reinterpret_cast<QObjectSmartPointerLink<QWeakPointer>*>(link.data())->setAsQObjectDeleted();
                    break;
                case QtJambiLink::Flag::shared_ptr:
                    reinterpret_cast<QObjectSmartPointerLink<std::shared_ptr>*>(link.data())->setAsQObjectDeleted();
                    break;
                case QtJambiLink::Flag::weak_ptr:
                    reinterpret_cast<QObjectSmartPointerLink<std::weak_ptr>*>(link.data())->setAsQObjectDeleted();
                    break;
                default: break;
                }
            }else{
                reinterpret_cast<QObjectPointerLink*>(link.data())->setAsQObjectDeleted();
            }
            // only reset the link when not a shell. Shells do this operation during shell destruction.
            // => QtJambiShellImpl::deleteShell()
            if(!link->isShell()){
                if(link->isQThread()){
                    if(DefaultJniEnvironment env{200}){
                        QTJAMBI_TRY{
                            link->invalidate(env);
                        }QTJAMBI_CATCH(const JavaException& exn){
                            exn.report(env);
                        }QTJAMBI_TRY_END
                        link.reset();
                    }
                }else{
                    if(JniEnvironment env{200}){
                        QTJAMBI_TRY{
                            link->invalidate(env);
                        }QTJAMBI_CATCH(const JavaException& exn){
                            exn.report(env);
                        }QTJAMBI_TRY_END
                        link.reset();
                    }
                }
                if(link)
                    ThreadPrivate::purgeLink(std::move(link));
            }
        }else{
            QtJambiLinkWriteLock locker;
        }
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
    } catch (...) {
    }
}

void QtJambiLinkObjectData::clear(JNIEnv * env){
    QSharedPointer<QtJambiLink> link = m_link.toStrongRef();
    m_link.clear();
    if(link){
        if(env && !link->isShell()){
            link->invalidate(env);
        }
        if (link->isQObject()) {
            QTJAMBI_INCREASE_COUNTER(userDataDestroyedCount, link)
        }
    }
}

struct ValueOwnerObjectDataPrivate : QSharedData{
    ValueOwnerObjectDataPrivate(const QObject* object) : QSharedData(), m_object(object){}
    ~ValueOwnerObjectDataPrivate(){
        while(!m_deleters.isEmpty()){
            try{
                m_deleters.takeFirst()();
            } catch (const std::exception& e) {
                qCWarning(DebugAPI::internalCategory, "%s", e.what());
            } catch (...) {
            }
        }
    }
    void addDeleter(const QtJambiUtils::Runnable& deleter){
        QtJambiLinkWriteLock locker;
        m_deleters << deleter;
    }
    void addDeleter(QtJambiUtils::Runnable&& deleter){
        QtJambiLinkWriteLock locker;
        m_deleters << std::move(deleter);
    }
    inline const QPointer<const QObject>& pointer() const { return m_object; }
    QPointer<const QObject> m_object;
    QList<QtJambiUtils::Runnable> m_deleters;
};

ValueOwnerObjectData::ValueOwnerObjectData(const QObject* object) : QtJambiObjectData(), d(new ValueOwnerObjectDataPrivate(object)) {

}

ValueOwnerObjectData::~ValueOwnerObjectData(){
    QExplicitlySharedDataPointer<ValueOwnerObjectDataPrivate> _d;
    {
        QtJambiLinkWriteLock locker;
        d.swap(_d);
    }
}

void ValueOwnerObjectData::addDeleter(const QtJambiUtils::Runnable& deleter){
    d->addDeleter(deleter);
}

void ValueOwnerObjectData::addDeleter(QtJambiUtils::Runnable&& deleter){
    d->addDeleter(std::move(deleter));
}

const QPointer<const QObject>& ValueOwnerObjectData::pointer() const {
    return d->pointer();
}

void QtJambiLink::registerQObjectInitialization(void *ptr, const QSharedPointer<QtJambiLink>& link) {
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker lock(storage->linkLock());
        storage->qObjectCache().insert(ptr, link.toWeakRef());
    }
}

void QtJambiLink::unregisterQObjectInitialization(void *ptr) {
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker lock(storage->linkLock());
        storage->qObjectCache().remove(ptr);
    }
}

jobject QtJambiLink::getNativeLink(JNIEnv *env, jobject java){
    jobject nativeLink;
    if(Java::QtJambi::NativeUtility$Object::isInstanceOf(env, java)){
        nativeLink = Java::QtJambi::NativeUtility$Object::nativeLink(env, java);
    }else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java)){
        nativeLink = Java::QtJambi::NativeUtility::findInterfaceLink(env, java, true, false);
    }else{
        nativeLink = nullptr;
    }
    Q_ASSERT(nativeLink);
    return nativeLink;
}

template<typename Ptr>
QSharedPointer<QtJambiLink> asSharedPointer(Ptr&& qtJambiLink){
    return *qtJambiLink.release();
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeQObject(JNIEnv *env, jobject& javaObject, QObject *object, SafeBool isQThread, const QMetaObject* superTypeForCustomMetaObject)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(object);
    Q_ASSERT(superTypeForCustomMetaObject);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    typedef PendingQObjectPointerLink Link;
    std::unique_ptr<Link, QtJambiLink::LinkDisposer> qtJambiLink{new Link(env, nativeLink, javaObject, superTypeForCustomMetaObject, object, isQThread, false, false, ocurredException)};
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        QSharedPointer<QtJambiLink> replacement;
        qtJambiLink->init(env, &replacement);
        if(replacement){
            javaObject = replacement->getJavaObjectLocalRef(env);
            return replacement;
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeQObject(JNIEnv *env, jobject& javaObject, QObject *object, SafeBool isQThread)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(object);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<PlainQObjectPointerLink, QtJambiLink::LinkDisposer> qtJambiLink;
    const QMetaObject* metaObject = object->metaObject();
    QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(metaObject);
    if(extraSignals.isEmpty())
        qtJambiLink.reset(new QObjectPlainPointerLink(env, nativeLink, javaObject, metaObject, object, isQThread, false, false, ocurredException));
    else
        qtJambiLink.reset(new ExtraSignalsQObjectPointerLink(env, nativeLink, javaObject, metaObject, object, isQThread, false, false, ocurredException));
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        QSharedPointer<QtJambiLink> replacement;
        qtJambiLink->init(env, &replacement);
        if(replacement){
            javaObject = replacement->getJavaObjectLocalRef(env);
            return replacement;
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeQObject(JNIEnv *env, jobject& javaObject, QObject *object, SafeBool isQThread,
                                                                           const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(object);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<PlainQObjectPointerLink, QtJambiLink::LinkDisposer> qtJambiLink;
    const QMetaObject* metaObject = object->metaObject();
    QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(metaObject);
    if(extraSignals.isEmpty()){
        InterfaceQObjectPointerLink* link;
        qtJambiLink.reset(link = new InterfaceQObjectPointerLink(env, nativeLink, javaObject, metaObject, object, isQThread, false, false, ocurredException));
        link->setInterfaceOffsetInfo(interfaceOffsetInfo);
    }else{
        ExtraSignalsInterfaceQObjectPointerLink* link;
        qtJambiLink.reset(link = new ExtraSignalsInterfaceQObjectPointerLink(env, nativeLink, javaObject, metaObject, object, isQThread, false, false, ocurredException));
        link->setInterfaceOffsetInfo(interfaceOffsetInfo);
    }
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        QSharedPointer<QtJambiLink> replacement;
        qtJambiLink->init(env, &replacement);
        if(replacement){
            javaObject = replacement->getJavaObjectLocalRef(env);
            return replacement;
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeQObject(JNIEnv *env, jobject& javaObject, QObject *object, SafeBool isQThread,
                                                                           const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                           const QMetaObject* superTypeForCustomMetaObject)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(object);
    Q_ASSERT(superTypeForCustomMetaObject);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<PendingInterfaceQObjectPointerLink, QtJambiLink::LinkDisposer> qtJambiLink{new PendingInterfaceQObjectPointerLink(env, nativeLink, javaObject, superTypeForCustomMetaObject, object, isQThread, false, false, ocurredException)};
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        QSharedPointer<QtJambiLink> replacement;
        qtJambiLink->init(env, &replacement);
        if(replacement){
            javaObject = replacement->getJavaObjectLocalRef(env);
            return replacement;
        }
    }
    return asSharedPointer(std::move(qtJambiLink));
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewQObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, const QMetaObject* metaObject, QObject *object, const SuperTypeInfos* superTypeInfos, SafeBool created_by_java, SafeBool is_shell, SafeBool hasCustomMetaObject, JavaException& ocurredException)
{
    if(!ocurredException){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(object);
        Q_ASSERT(metaObject);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        std::unique_ptr<QObjectPointerLink, QtJambiLink::LinkDisposer> qtJambiLink;
        if(hasCustomMetaObject){
            if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
                PendingInterfaceQObjectPointerLink* link;
                qtJambiLink.reset(link = new PendingInterfaceQObjectPointerLink(env, nativeLink, javaObject, metaObject, object, false, created_by_java, is_shell, ocurredException));
                link->setInterfaceOffsetInfo(*interfaceOffsets);
            }else{
                qtJambiLink.reset(new PendingQObjectPointerLink(env, nativeLink, javaObject, metaObject, object, false, created_by_java, is_shell, ocurredException));
            }
        }else{
            QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(metaObject);
            if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
                if(extraSignals.isEmpty()){
                    InterfaceQObjectPointerLink* link;
                    qtJambiLink.reset(link = new InterfaceQObjectPointerLink(env, nativeLink, javaObject, metaObject, object, false, created_by_java, is_shell, ocurredException));
                    link->setInterfaceOffsetInfo(*interfaceOffsets);
                }else{
                    ExtraSignalsInterfaceQObjectPointerLink* link;
                    qtJambiLink.reset(link = new ExtraSignalsInterfaceQObjectPointerLink(env, nativeLink, javaObject, metaObject, object, false, created_by_java, is_shell, ocurredException));
                    link->setInterfaceOffsetInfo(*interfaceOffsets);
                }
            }else{
                if(extraSignals.isEmpty()){
                    qtJambiLink.reset(new PlainQObjectPointerLink(env, nativeLink, javaObject, metaObject, object, false, created_by_java, is_shell, ocurredException));
                }else{
                    qtJambiLink.reset(new ExtraSignalsQObjectPointerLink(env, nativeLink, javaObject, metaObject, object, false, created_by_java, is_shell, ocurredException));
                }
            }
        }
        if(Q_LIKELY(!ocurredException))
            return asSharedPointer(std::move(qtJambiLink));
    }
    return {};
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewQObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, const QMetaObject* metaObject, QThread *object, const SuperTypeInfos* superTypeInfos, SafeBool created_by_java, SafeBool is_shell, SafeBool hasCustomMetaObject, JavaException& ocurredException)
{
    if(!ocurredException){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(object);
        Q_ASSERT(metaObject);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        std::unique_ptr<QObjectPointerLink, QtJambiLink::LinkDisposer> qtJambiLink;
        if(hasCustomMetaObject){
            if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
                PendingInterfaceQObjectPointerLink* link;
                qtJambiLink.reset(link = new PendingInterfaceQObjectPointerLink(env, nativeLink, javaObject, metaObject, object, true, created_by_java, is_shell, ocurredException));
                link->setInterfaceOffsetInfo(*interfaceOffsets);
            }else{
                qtJambiLink.reset(new PendingQObjectPointerLink(env, nativeLink, javaObject, metaObject, object, true, created_by_java, is_shell, ocurredException));
            }
        }else{
            QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(metaObject);
            if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
                if(extraSignals.isEmpty()){
                    InterfaceQObjectPointerLink* link;
                    qtJambiLink.reset(link = new InterfaceQObjectPointerLink(env, nativeLink, javaObject, metaObject, object, true, created_by_java, is_shell, ocurredException));
                    link->setInterfaceOffsetInfo(*interfaceOffsets);
                }else{
                    ExtraSignalsInterfaceQObjectPointerLink* link;
                    qtJambiLink.reset(link = new ExtraSignalsInterfaceQObjectPointerLink(env, nativeLink, javaObject, metaObject, object, true, created_by_java, is_shell, ocurredException));
                    link->setInterfaceOffsetInfo(*interfaceOffsets);
                }
            }else{
                if(extraSignals.isEmpty()){
                    qtJambiLink.reset(new PlainQObjectPointerLink(env, nativeLink, javaObject, metaObject, object, true, created_by_java, is_shell, ocurredException));
                }else{
                    qtJambiLink.reset(new ExtraSignalsQObjectPointerLink(env, nativeLink, javaObject, metaObject, object, true, created_by_java, is_shell, ocurredException));
                }
            }
        }
        if(Q_LIKELY(!ocurredException))
            return asSharedPointer(std::move(qtJambiLink));
    }
    return {};
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToQObject(JNIEnv *env,
                                                                                   jobject javaObject,
                                                                                   SafeBool created_by_java,
                                                                                   SafeBool is_shell,
                                                                                   const SmartPointer<QObject>& smartPointer,
                                                                                   SafeBool isQThread,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(smartPointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    QObject* object = smartPointer.get();
    Q_ASSERT(object);
    JavaException ocurredException;
    std::unique_ptr<QObjectSmartPointerLink<SmartPointer>, QtJambiLink::LinkDisposer> qtJambiLink;
    SmartPointer<QObject>* smartPointerTarget{nullptr};
    const QMetaObject* metaObject = object->metaObject();
    QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(metaObject);
    if(extraSignals.isEmpty()){
        InterfaceLink<PlainQObjectSmartPointerLink<SmartPointer>>* link;
        qtJambiLink.reset(link = new InterfaceLink<PlainQObjectSmartPointerLink<SmartPointer>>(env, nativeLink, javaObject, created_by_java, QtJambiLink::Ownership::None, is_shell, object, isQThread, smartPointerTarget, ocurredException));
        link->setInterfaceOffsetInfo(interfaceOffsetInfo);
    }else{
        InterfaceLink<QObjectWithExtraSignalsSmartPointerLink<SmartPointer>>* link;
        qtJambiLink.reset(link = new InterfaceLink<QObjectWithExtraSignalsSmartPointerLink<SmartPointer>>(env, nativeLink, javaObject, created_by_java, QtJambiLink::Ownership::None, is_shell, object, isQThread, smartPointerTarget, ocurredException));
        link->setInterfaceOffsetInfo(interfaceOffsetInfo);
    }
    *smartPointerTarget = smartPointer;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if(object->parent())
            qtJambiLink->setCppOwnership(env);
        else
            qtJambiLink->setJavaOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToQObject(JNIEnv *env,
                                                                                   jobject javaObject,
                                                                                   SafeBool created_by_java,
                                                                                   SafeBool is_shell,
                                                                                   const SmartPointer<QObject>& smartPointer,
                                                                                   SafeBool isQThread,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                   const QMetaObject* superTypeForCustomMetaObject)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(smartPointer);
    Q_ASSERT(superTypeForCustomMetaObject);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    QObject* object = smartPointer.get();
    Q_ASSERT(object);
    JavaException ocurredException;
    SmartPointer<QObject>* smartPointerTarget{nullptr};
    typedef InterfaceLink<QObjectWithPendingExtraSignalsSmartPointerLink<SmartPointer>> Link;
    std::unique_ptr<Link, QtJambiLink::LinkDisposer> qtJambiLink{
        new Link(env, nativeLink, javaObject, created_by_java, QtJambiLink::Ownership::None, is_shell, object, isQThread, smartPointerTarget, superTypeForCustomMetaObject, ocurredException)};
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    *smartPointerTarget = smartPointer;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if(object->parent())
            qtJambiLink->setCppOwnership(env);
        else
            qtJambiLink->setJavaOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject, SafeBool created_by_java, SafeBool is_shell, const SmartPointer<QObject>& smartPointer, SafeBool isQThread, const QMetaObject* superTypeForCustomMetaObject)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(smartPointer);
    Q_ASSERT(superTypeForCustomMetaObject);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    QObject* object = smartPointer.get();
    Q_ASSERT(object);
    JavaException ocurredException;
    SmartPointer<QObject>* smartPointerTarget{nullptr};
    std::unique_ptr<QObjectSmartPointerLink<SmartPointer>, QtJambiLink::LinkDisposer> qtJambiLink{new QObjectWithPendingExtraSignalsSmartPointerLink(env, nativeLink, javaObject, created_by_java, QtJambiLink::Ownership::None, is_shell, object, isQThread, smartPointerTarget, superTypeForCustomMetaObject, ocurredException)};
    *smartPointerTarget = smartPointer;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if(object->parent())
            qtJambiLink->setCppOwnership(env);
        else
            qtJambiLink->setJavaOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject, SafeBool created_by_java, SafeBool is_shell, const SmartPointer<QObject>& smartPointer, SafeBool isQThread)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(smartPointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    QObject* object = smartPointer.get();
    Q_ASSERT(object);
    JavaException ocurredException;
    SmartPointer<QObject>* smartPointerTarget{nullptr};
    QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(object->metaObject());
    std::unique_ptr<PlainQObjectSmartPointerLink<SmartPointer>, QtJambiLink::LinkDisposer> qtJambiLink;
    if(extraSignals.isEmpty())
        qtJambiLink.reset(new PlainQObjectSmartPointerLink<SmartPointer>(env, nativeLink, javaObject, created_by_java, QtJambiLink::Ownership::None, is_shell, object, isQThread, smartPointerTarget, ocurredException));
    else
        qtJambiLink.reset(new QObjectWithExtraSignalsSmartPointerLink<SmartPointer>(env, nativeLink, javaObject, created_by_java, QtJambiLink::Ownership::None, is_shell, object, isQThread, smartPointerTarget, ocurredException));
    *smartPointerTarget = smartPointer;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if(object->parent())
            qtJambiLink->setCppOwnership(env);
        else
            qtJambiLink->setJavaOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject javaObject, SafeBool created_by_java,
                                                                                      QtJambiLink::Ownership ownership, SafeBool is_shell, QObject* object,
                                                                                      SafeBool isQThread,
                                                                                      SmartPointer<QObject>& smartPointer,
                                                                                      const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                      const QMetaObject* superTypeForCustomMetaObject)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(object);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    SmartPointer<QObject>* smartPointerTarget{nullptr};
    std::unique_ptr<QObjectSmartPointerLink<SmartPointer>, QtJambiLink::LinkDisposer> qtJambiLink;
    if(superTypeForCustomMetaObject){
        InterfaceLink<QObjectWithPendingExtraSignalsSmartPointerLink<SmartPointer>>* link;
        qtJambiLink.reset(link = new InterfaceLink<QObjectWithPendingExtraSignalsSmartPointerLink<SmartPointer>>(env, nativeLink, javaObject, created_by_java, ownership, is_shell, object, isQThread, smartPointerTarget, superTypeForCustomMetaObject, ocurredException));
        link->setInterfaceOffsetInfo(interfaceOffsetInfo);
    }else{
        const QMetaObject* metaObject = object->metaObject();
        QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(metaObject);
        if(extraSignals.isEmpty()){
            InterfaceLink<PlainQObjectSmartPointerLink<SmartPointer>>* link;
            qtJambiLink.reset(link = new InterfaceLink<PlainQObjectSmartPointerLink<SmartPointer>>(env, nativeLink, javaObject, created_by_java, ownership, is_shell, object, isQThread, smartPointerTarget, ocurredException));
            link->setInterfaceOffsetInfo(interfaceOffsetInfo);
        }else{
            InterfaceLink<QObjectWithExtraSignalsSmartPointerLink<SmartPointer>>* link;
            qtJambiLink.reset(link = new InterfaceLink<QObjectWithExtraSignalsSmartPointerLink<SmartPointer>>(env, nativeLink, javaObject, created_by_java, ownership, is_shell, object, isQThread, smartPointerTarget, ocurredException));
            link->setInterfaceOffsetInfo(interfaceOffsetInfo);
        }
    }
    *smartPointerTarget = smartPointer;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if constexpr(std::is_same<SmartPointer<QObject>,QWeakPointer<QObject>>::value || std::is_same<SmartPointer<QObject>,std::weak_ptr<QObject>>::value){
            qtJambiLink->QtJambiLink::setJavaOwnership(env);
        }else{
            qtJambiLink->QtJambiLink::setCppOwnership(env);
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

template<template<typename> class SmartPointer, typename T>
struct SmartPointerDeleter{
    QWeakPointer<SmartPointerLink<SmartPointer,T>> m_link;
    SmartPointerDeleter(QWeakPointer<SmartPointerLink<SmartPointer,T>>&& link)
        : m_link(std::move(link))
    {
    }
    bool pushToJava(){
        if(QSharedPointer<SmartPointerLink<SmartPointer,T>> link = m_link){
            QtJambiLink::Ownership ownership = link->ownership();
            switch(ownership){
            case QtJambiLink::Ownership::Java:
                if(JniEnvironment env{200}){
                    QTJAMBI_TRY{
                        link->QtJambiLink::setJavaOwnership(env);
                    }QTJAMBI_CATCH(const JavaException& exn){
                        exn.report(env);
                    }QTJAMBI_TRY_END
                    return true;
                }
                break;
            default:
                break;
            }
        }
        return false;
    }
};

template<template<typename> class SmartPointer>
struct LinkedQObjectDeleter : SmartPointerDeleter<SmartPointer,QObject>{
    LinkedQObjectDeleter(QWeakPointer<SmartPointerLink<SmartPointer,QObject>>&& link)
        : SmartPointerDeleter<SmartPointer,QObject>(std::move(link))
    {
    }
    void operator()(QObject* ptr){
        if(SmartPointerDeleter<SmartPointer,QObject>::pushToJava())
            return;
        delete ptr;
    }
};

template<template<typename> class SmartPointer>
struct SmartDeleterFunctionDeleter : SmartPointerDeleter<SmartPointer,char>{
    PtrDeleterFunction deleterFunction;
    SmartDeleterFunctionDeleter(PtrDeleterFunction _deleterFunction, QWeakPointer<SmartPointerLink<SmartPointer,char>>&& link)
        : SmartPointerDeleter<SmartPointer,char>(std::move(link)), deleterFunction(_deleterFunction)
    {
    }
    void operator()(char* ptr){
        if(SmartPointerDeleter<SmartPointer,char>::pushToJava())
            return;
        deleterFunction(ptr, false);
    }
};

template<template<typename> class SmartPointer>
struct SmartMetaTypeDeleter : SmartPointerDeleter<SmartPointer,char>{
    QMetaType elementMetaType;
    SmartMetaTypeDeleter(const QMetaType& _elementMetaType, QWeakPointer<SmartPointerLink<SmartPointer,char>>&& link)
        : SmartPointerDeleter<SmartPointer,char>(std::move(link)), elementMetaType(_elementMetaType)
    {
    }
    void operator()(char* ptr){
        if(SmartPointerDeleter<SmartPointer,char>::pushToJava())
            return;
        if(elementMetaType.isValid())
            elementMetaType.destroy(ptr);
    }
};

template<template<typename> class SmartPointer>
struct SmartContainerDeleter : SmartPointerDeleter<SmartPointer,char>{
    AbstractContainerAccess* containerAccess;
    SmartContainerDeleter(AbstractContainerAccess* _containerAccess, QWeakPointer<SmartPointerLink<SmartPointer,char>>&& link)
        : SmartPointerDeleter<SmartPointer,char>(std::move(link)),
          containerAccess(_containerAccess->clone()){

    }
    ~SmartContainerDeleter(){containerAccess->dispose();}
    void operator()(char* ptr){
        if(SmartPointerDeleter<SmartPointer,char>::pushToJava())
            return;
        containerAccess->deleteContainer(ptr);
    }
};

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject javaObject, SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell, QObject* object, SafeBool isQThread, SmartPointer<QObject>& smartPointer, const QMetaObject* superTypeForCustomMetaObject)
{
    typedef typename std::conditional<std::is_same<SmartPointer<QObject>,QWeakPointer<QObject>>::value, QSharedPointer<QObject>,
                                      typename std::conditional<std::is_same<SmartPointer<QObject>,std::weak_ptr<QObject>>::value, std::shared_ptr<QObject>, SmartPointer<QObject>>::type>::type SharedPointerT;
    typedef typename std::conditional</*if*/std::is_same<SmartPointer<QObject>,QWeakPointer<QObject>>::value,
                                      /*then*/QObjectSmartPointerLink<QSharedPointer>,
                                      /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<QObject>,std::weak_ptr<QObject>>::value,
                                                                        /*then*/QObjectSmartPointerLink<std::shared_ptr>,
                                                                        /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<QObject>,QSharedPointer<QObject>>::value,
                                                                                                          /*then*/QObjectSmartPointerLink<QWeakPointer>,
                                                                                                          /*else*/QObjectSmartPointerLink<std::weak_ptr>>::type>::type>::type Link;
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(object);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    typename Link::SmartPointerType* smartPointerTarget{nullptr};
    std::unique_ptr<Link, QtJambiLink::LinkDisposer> qtJambiLink;
    if(superTypeForCustomMetaObject){
        qtJambiLink.reset(new QObjectWithPendingExtraSignalsSmartPointerLink(env, nativeLink, javaObject, created_by_java, ownership, is_shell, object, isQThread, smartPointerTarget, superTypeForCustomMetaObject, ocurredException));
    }else{
        QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(object->metaObject());
        if(extraSignals.isEmpty())
            qtJambiLink.reset(new PlainQObjectSmartPointerLink(env, nativeLink, javaObject, created_by_java, ownership, is_shell, object, isQThread, smartPointerTarget, ocurredException));
        else{
            typedef typename std::conditional</*if*/std::is_same<SmartPointer<QObject>,QWeakPointer<QObject>>::value,
                                              /*then*/QObjectWithExtraSignalsSmartPointerLink<QSharedPointer>,
                                              /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<QObject>,std::weak_ptr<QObject>>::value,
                                                                                /*then*/QObjectWithExtraSignalsSmartPointerLink<std::shared_ptr>,
                                                                                /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<QObject>,QSharedPointer<QObject>>::value,
                                                                                                                  /*then*/QObjectWithExtraSignalsSmartPointerLink<QWeakPointer>,
                                                                                                                  /*else*/QObjectWithExtraSignalsSmartPointerLink<std::weak_ptr>>::type>::type>::type ExtraLink;
            qtJambiLink.reset(new ExtraLink(env, nativeLink, javaObject, created_by_java, ownership, is_shell, object, isQThread, smartPointerTarget, ocurredException));
        }
    }
    SharedPointerT sharedPointer;
    sharedPointer = SharedPointerT(object, LinkedQObjectDeleter<SmartPointer>(*qtJambiLink));
    smartPointer = sharedPointer;
    *smartPointerTarget = typename Link::SmartPointerType(sharedPointer);
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if constexpr(std::is_same<SmartPointer<QObject>,QWeakPointer<QObject>>::value || std::is_same<SmartPointer<QObject>,std::weak_ptr<QObject>>::value){
            qtJambiLink->QtJambiLink::setJavaOwnership(env);
        }else{
            qtJambiLink->QtJambiLink::setCppOwnership(env);
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   SafeBool created_by_java, SafeBool is_shell,
                                                                                   const QSharedPointer<QObject>& smartPointer,
                                                                                   SafeBool isQThread,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo){
    return createLinkForSmartPointerToQObject<QSharedPointer>(env, javaObject, created_by_java, is_shell,
                                                              smartPointer, isQThread,
                                                              interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   SafeBool created_by_java, SafeBool is_shell,
                                                                                   const std::shared_ptr<QObject>& smartPointer,
                                                                                   SafeBool isQThread,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo){
    return createLinkForSmartPointerToQObject<std::shared_ptr>(env, javaObject, created_by_java, is_shell,
                                                              smartPointer, isQThread,
                                                              interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   SafeBool created_by_java, SafeBool is_shell,
                                                                                   const QSharedPointer<QObject>& smartPointer,
                                                                                   SafeBool isQThread,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForSmartPointerToQObject<QSharedPointer>(env, javaObject, created_by_java, is_shell,
                                                              smartPointer, isQThread,
                                                              interfaceOffsetInfo,
                                                              superTypeForCustomMetaObject);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   SafeBool created_by_java, SafeBool is_shell,
                                                                                   const std::shared_ptr<QObject>& smartPointer,
                                                                                   SafeBool isQThread,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForSmartPointerToQObject<std::shared_ptr>(env, javaObject, created_by_java, is_shell,
                                                              smartPointer, isQThread,
                                                              interfaceOffsetInfo,
                                                              superTypeForCustomMetaObject);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   SafeBool created_by_java, SafeBool is_shell,
                                                                                   const QSharedPointer<QObject>& smartPointer,
                                                                                   SafeBool isQThread){
    return createLinkForSmartPointerToQObject<QSharedPointer>(env, javaObject, created_by_java, is_shell,
                                                              smartPointer, isQThread);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   SafeBool created_by_java, SafeBool is_shell,
                                                                                   const std::shared_ptr<QObject>& smartPointer,
                                                                                   SafeBool isQThread){
    return createLinkForSmartPointerToQObject<std::shared_ptr>(env, javaObject, created_by_java, is_shell,
                                                              smartPointer, isQThread);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   SafeBool created_by_java, SafeBool is_shell,
                                                                                   const QSharedPointer<QObject>& smartPointer,
                                                                                   SafeBool isQThread,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForSmartPointerToQObject<QSharedPointer>(env, javaObject, created_by_java, is_shell,
                                                              smartPointer, isQThread,
                                                              superTypeForCustomMetaObject);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   SafeBool created_by_java, SafeBool is_shell,
                                                                                   const std::shared_ptr<QObject>& smartPointer,
                                                                                   SafeBool isQThread,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForSmartPointerToQObject<std::shared_ptr>(env, javaObject, created_by_java, is_shell,
                                                              smartPointer, isQThread,
                                                              superTypeForCustomMetaObject);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                   QObject* object, SafeBool isQThread, QSharedPointer<QObject>& smartPointer,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForNewSmartPointerToQObject<QSharedPointer>(env, javaObject, created_by_java, ownership, is_shell,
                                                              object, isQThread, smartPointer,
                                                              interfaceOffsetInfo,
                                                              superTypeForCustomMetaObject);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                   QObject* object, SafeBool isQThread, std::shared_ptr<QObject>& smartPointer,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForNewSmartPointerToQObject<std::shared_ptr>(env, javaObject, created_by_java, ownership, is_shell,
                                                              object, isQThread, smartPointer,
                                                              interfaceOffsetInfo,
                                                              superTypeForCustomMetaObject);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                   QObject* object, SafeBool isQThread, QSharedPointer<QObject>& smartPointer,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForNewSmartPointerToQObject<QSharedPointer>(env, javaObject, created_by_java, ownership, is_shell,
                                                              object, isQThread, smartPointer,
                                                              superTypeForCustomMetaObject);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                   QObject* object, SafeBool isQThread, std::shared_ptr<QObject>& smartPointer,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForNewSmartPointerToQObject<std::shared_ptr>(env, javaObject, created_by_java, ownership, is_shell,
                                                              object, isQThread, smartPointer,
                                                              superTypeForCustomMetaObject);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                   QObject* object, SafeBool isQThread, QWeakPointer<QObject>& smartPointer,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForNewSmartPointerToQObject<QWeakPointer>(env, javaObject, created_by_java, ownership, is_shell,
                                                              object, isQThread, smartPointer,
                                                              interfaceOffsetInfo,
                                                              superTypeForCustomMetaObject);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                   QObject* object, SafeBool isQThread, std::weak_ptr<QObject>& smartPointer,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForNewSmartPointerToQObject<std::weak_ptr>(env, javaObject, created_by_java, ownership, is_shell,
                                                              object, isQThread, smartPointer,
                                                              interfaceOffsetInfo,
                                                              superTypeForCustomMetaObject);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                   QObject* object, SafeBool isQThread, QWeakPointer<QObject>& smartPointer,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForNewSmartPointerToQObject<QWeakPointer>(env, javaObject, created_by_java, ownership, is_shell,
                                                              object, isQThread, smartPointer,
                                                              superTypeForCustomMetaObject);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                   QObject* object, SafeBool isQThread, std::weak_ptr<QObject>& smartPointer,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForNewSmartPointerToQObject<std::weak_ptr>(env, javaObject, created_by_java, ownership, is_shell,
                                                              object, isQThread, smartPointer,
                                                              superTypeForCustomMetaObject);
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createExtendedLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                          LINK_NAME_ARG(const char* qt_name)
                                                                                          SafeBool created_by_java, SafeBool is_shell,
                                                                                          const QObject* extension,
                                                                                          const SmartPointer<char>& smartPointer)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(smartPointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    SmartPointer<char>* smartPointerTarget{nullptr};
    typedef ExtendedLink<PlainObjectSmartPointerLink<SmartPointer>> Link;
    std::unique_ptr<Link, QtJambiLink::LinkDisposer> qtJambiLink{new Link(env, nativeLink,
                                                                   LINK_NAME_ARG(qt_name)
                                                                   created_by_java, QtJambiLink::Ownership::None, is_shell,
                                                                   smartPointer.get(),
                                                                   smartPointerTarget,
                                                                   ocurredException)};
    qtJambiLink->m_extension = extension;
    *smartPointerTarget = smartPointer;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        qtJambiLink->setJavaOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createExtendedLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                          LINK_NAME_ARG(const char* qt_name)
                                                                                          SafeBool created_by_java, SafeBool is_shell,
                                                                                          const QObject* extension,
                                                                                          PtrOwnerFunction ownerFunction,
                                                                                          const SmartPointer<char>& smartPointer)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(smartPointer);
    if(ownerFunction)
        checkValueOwner(env, ownerFunction, smartPointer.get());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    SmartPointer<char>* smartPointerTarget{nullptr};
    std::unique_ptr<ExtendedLink<PlainOwnedObjectSmartPointerLink<SmartPointer>>, QtJambiLink::LinkDisposer> qtJambiLink{
        new ExtendedLink<PlainOwnedObjectSmartPointerLink<SmartPointer>>(env, nativeLink,
                                                                   LINK_NAME_ARG(qt_name)
                                                                   created_by_java, QtJambiLink::Ownership::None, is_shell,
                                                                   smartPointer.get(),
                                                                   smartPointerTarget,
                                                                   ocurredException)};
    qtJambiLink->m_owner_function = ownerFunction;
    qtJambiLink->m_extension = extension;
    *smartPointerTarget = smartPointer;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        qtJambiLink->setJavaOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createExtendedLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                          LINK_NAME_ARG(const char* qt_name)
                                                                                          SafeBool created_by_java, SafeBool is_shell,
                                                                                          const QObject* extension,
                                                                                          const std::shared_ptr<char>& smartPointer){
    return createExtendedLinkForSmartPointerToObject<std::shared_ptr>(env, javaObject,
                                                                     LINK_NAME_ARG(qt_name)
                                                                     created_by_java, is_shell,
                                                                     extension,
                                                                     smartPointer);
}

QSharedPointer<QtJambiLink> QtJambiLink::createExtendedLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                          LINK_NAME_ARG(const char* qt_name)
                                                                                          SafeBool created_by_java, SafeBool is_shell,
                                                                                          const QObject* extension,
                                                                                          const QSharedPointer<char>& smartPointer){
    return createExtendedLinkForSmartPointerToObject<QSharedPointer>(env, javaObject,
                                                                     LINK_NAME_ARG(qt_name)
                                                                     created_by_java, is_shell,
                                                                     extension,
                                                                     smartPointer);
}

QSharedPointer<QtJambiLink> QtJambiLink::createExtendedLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                          LINK_NAME_ARG(const char* qt_name)
                                                                                          SafeBool created_by_java, SafeBool is_shell,
                                                                                          const QObject* extension,
                                                                                          PtrOwnerFunction ownerFunction,
                                                                                          const std::shared_ptr<char>& smartPointer){
    return createExtendedLinkForSmartPointerToObject<std::shared_ptr>(env, javaObject,
                                                                     LINK_NAME_ARG(qt_name)
                                                                     created_by_java, is_shell,
                                                                     extension,
                                                                     ownerFunction,
                                                                     smartPointer);
}

QSharedPointer<QtJambiLink> QtJambiLink::createExtendedLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                          LINK_NAME_ARG(const char* qt_name)
                                                                                          SafeBool created_by_java, SafeBool is_shell,
                                                                                          const QObject* extension,
                                                                                          PtrOwnerFunction ownerFunction,
                                                                                          const QSharedPointer<char>& smartPointer){
    return createExtendedLinkForSmartPointerToObject<QSharedPointer>(env, javaObject,
                                                                     LINK_NAME_ARG(qt_name)
                                                                     created_by_java, is_shell,
                                                                     extension,
                                                                     ownerFunction,
                                                                     smartPointer);
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  SmartPointer<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    typedef typename std::conditional<std::is_same<SmartPointer<char>,QWeakPointer<char>>::value, QSharedPointer<char>,
                                      typename std::conditional<std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value, std::shared_ptr<char>, SmartPointer<char>>::type>::type SharedPointerT;
    typedef typename std::conditional</*if*/std::is_same<SmartPointer<char>,QWeakPointer<char>>::value,
                                      /*then*/InterfaceLink<OwnedMetaTypedObjectSmartPointerLink<QSharedPointer>>,
                                      /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value,
                                                                        /*then*/InterfaceLink<OwnedMetaTypedObjectSmartPointerLink<std::shared_ptr>>,
                                                                        /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,QSharedPointer<char>>::value,
                                                                                                          /*then*/InterfaceLink<OwnedMetaTypedObjectSmartPointerLink<QWeakPointer>>,
                                                                                                          /*else*/InterfaceLink<OwnedMetaTypedObjectSmartPointerLink<std::weak_ptr>>>::type>::type>::type Link;
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(pointer);
    if(ownerFunction)
        checkValueOwner(env, ownerFunction, pointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    typename Link::SmartPointerType* smartPointerTarget{nullptr};
    std::unique_ptr<Link, QtJambiLink::LinkDisposer> qtJambiLink{new Link(env, nativeLink,
                                                                          LINK_NAME_ARG(elementMetaType.name())
                                                                          created_by_java, ownership, is_shell,
                                                                          pointer, smartPointerTarget, ocurredException)};
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    qtJambiLink->m_owner_function = ownerFunction;
    SharedPointerT sharedPointer(reinterpret_cast<char*>(pointer), SmartMetaTypeDeleter<SmartPointer>(elementMetaType, *qtJambiLink.get()));
    smartPointer = sharedPointer;
    *smartPointerTarget = typename Link::SmartPointerType(sharedPointer);
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        qtJambiLink->setJavaOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  void* pointer,
                                                                                  SmartPointer<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    typedef typename std::conditional<std::is_same<SmartPointer<char>,QWeakPointer<char>>::value, QSharedPointer<char>,
                                      typename std::conditional<std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value, std::shared_ptr<char>, SmartPointer<char>>::type>::type SharedPointerT;
    typedef typename std::conditional</*if*/std::is_same<SmartPointer<char>,QWeakPointer<char>>::value,
                                      /*then*/InterfaceLink<MetaTypedObjectSmartPointerLink<QSharedPointer>>,
                                      /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value,
                                                                        /*then*/InterfaceLink<MetaTypedObjectSmartPointerLink<std::shared_ptr>>,
                                                                        /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,QSharedPointer<char>>::value,
                                                                                                          /*then*/InterfaceLink<MetaTypedObjectSmartPointerLink<QWeakPointer>>,
                                                                                                          /*else*/InterfaceLink<MetaTypedObjectSmartPointerLink<std::weak_ptr>>>::type>::type>::type Link;
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(pointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    typename Link::SmartPointerType* smartPointerTarget{nullptr};
    std::unique_ptr<Link, QtJambiLink::LinkDisposer> qtJambiLink{new Link(env, nativeLink,
                                                                          LINK_NAME_ARG(elementMetaType.name())
                                                                          created_by_java, ownership, is_shell,
                                                                          pointer, smartPointerTarget, ocurredException)};
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    SharedPointerT sharedPointer(reinterpret_cast<char*>(pointer), SmartMetaTypeDeleter<SmartPointer>(elementMetaType, *qtJambiLink.get()));
    smartPointer = sharedPointer;
    *smartPointerTarget = typename Link::SmartPointerType(sharedPointer);
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        qtJambiLink->setJavaOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, SafeBool is_shell,
                                                                                  const SmartPointer<char>& smartPointer,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(smartPointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    SmartPointer<char>* smartPointerTarget{nullptr};
    std::unique_ptr<InterfaceLink<PlainObjectSmartPointerLink<SmartPointer>>, QtJambiLink::LinkDisposer> qtJambiLink{new InterfaceLink<PlainObjectSmartPointerLink<SmartPointer>>(env, nativeLink,
                                                                            LINK_NAME_ARG(qt_name)
                                                                            created_by_java, QtJambiLink::Ownership::None, is_shell, smartPointer.get(), smartPointerTarget, ocurredException)};
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    *smartPointerTarget = smartPointer;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        qtJambiLink->setJavaOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  const SmartPointer<char>& smartPointer,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(smartPointer);
    Q_ASSERT(ownerFunction);
    checkValueOwner(env, ownerFunction, smartPointer.get());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    SmartPointer<char>* smartPointerTarget{nullptr};
    std::unique_ptr<InterfaceLink<PlainOwnedObjectSmartPointerLink<SmartPointer>>, QtJambiLink::LinkDisposer> qtJambiLink{new InterfaceLink<PlainOwnedObjectSmartPointerLink<SmartPointer>>(env, nativeLink,
                                                                            LINK_NAME_ARG(qt_name)
                                                                            created_by_java, QtJambiLink::Ownership::None, is_shell, smartPointer.get(), smartPointerTarget, ocurredException)};
    qtJambiLink->m_owner_function = ownerFunction;
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    *smartPointerTarget = smartPointer;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        qtJambiLink->setJavaOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  SmartPointer<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType)
{
    typedef typename std::conditional<std::is_same<SmartPointer<char>,QWeakPointer<char>>::value, QSharedPointer<char>,
                                      typename std::conditional<std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value, std::shared_ptr<char>, SmartPointer<char>>::type>::type SharedPointerT;
    typedef typename std::conditional</*if*/std::is_same<SmartPointer<char>,QWeakPointer<char>>::value,
                                      /*then*/OwnedMetaTypedObjectSmartPointerLink<QSharedPointer>,
                                      /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value,
                                                                        /*then*/OwnedMetaTypedObjectSmartPointerLink<std::shared_ptr>,
                                                                        /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,QSharedPointer<char>>::value,
                                                                                                          /*then*/OwnedMetaTypedObjectSmartPointerLink<QWeakPointer>,
                                                                                                          /*else*/OwnedMetaTypedObjectSmartPointerLink<std::weak_ptr>>::type>::type>::type Link;
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(pointer);
    Q_ASSERT(ownerFunction);
    checkValueOwner(env, ownerFunction, pointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    typename Link::SmartPointerType* smartPointerTarget{nullptr};

    std::unique_ptr<Link, QtJambiLink::LinkDisposer> qtJambiLink{new Link(env, nativeLink,
                                LINK_NAME_ARG(elementMetaType.name())
                                created_by_java, ownership, is_shell,
                                pointer,
                                smartPointerTarget, ocurredException)};
    qtJambiLink->m_owner_function = ownerFunction;
    qtJambiLink->m_meta_type = elementMetaType;
    SharedPointerT sharedPointer(reinterpret_cast<char*>(pointer), SmartMetaTypeDeleter<SmartPointer>(elementMetaType, *qtJambiLink.get()));
    smartPointer = sharedPointer;
    *smartPointerTarget = typename Link::SmartPointerType(sharedPointer);
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if constexpr(std::is_same<SmartPointer<char>,QWeakPointer<char>>::value || std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value){
            qtJambiLink->QtJambiLink::setJavaOwnership(env);
        }else{
            qtJambiLink->QtJambiLink::setCppOwnership(env);
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  void* pointer,
                                                                                  SmartPointer<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType)
{
    typedef typename std::conditional<std::is_same<SmartPointer<char>,QWeakPointer<char>>::value, QSharedPointer<char>,
                                      typename std::conditional<std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value, std::shared_ptr<char>, SmartPointer<char>>::type>::type SharedPointerT;
    typedef typename std::conditional</*if*/std::is_same<SmartPointer<char>,QWeakPointer<char>>::value,
                                      /*then*/MetaTypedObjectSmartPointerLink<QSharedPointer>,
                                      /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value,
                                                                        /*then*/MetaTypedObjectSmartPointerLink<std::shared_ptr>,
                                                                        /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,QSharedPointer<char>>::value,
                                                                                                          /*then*/MetaTypedObjectSmartPointerLink<QWeakPointer>,
                                                                                                          /*else*/MetaTypedObjectSmartPointerLink<std::weak_ptr>>::type>::type>::type Link;
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(pointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    typename Link::SmartPointerType* smartPointerTarget{nullptr};

    std::unique_ptr<Link, QtJambiLink::LinkDisposer> qtJambiLink{
        new Link(env, nativeLink,
                                LINK_NAME_ARG(elementMetaType.name())
                                created_by_java, ownership, is_shell,
                                pointer,
                                smartPointerTarget, ocurredException)};
    qtJambiLink->m_meta_type = elementMetaType;
    SharedPointerT sharedPointer(reinterpret_cast<char*>(pointer), SmartMetaTypeDeleter<SmartPointer>(elementMetaType, *qtJambiLink.get()));
    smartPointer = sharedPointer;
    *smartPointerTarget = typename Link::SmartPointerType(sharedPointer);
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if constexpr(std::is_same<SmartPointer<char>,QWeakPointer<char>>::value || std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value){
            qtJambiLink->QtJambiLink::setJavaOwnership(env);
        }else{
            qtJambiLink->QtJambiLink::setCppOwnership(env);
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  SmartPointer<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    typedef typename std::conditional<std::is_same<SmartPointer<char>,QWeakPointer<char>>::value, QSharedPointer<char>,
                                      typename std::conditional<std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value, std::shared_ptr<char>, SmartPointer<char>>::type>::type SharedPointerT;
    typedef typename std::conditional</*if*/std::is_same<SmartPointer<char>,QWeakPointer<char>>::value,
                                      /*then*/InterfaceLink<PlainOwnedObjectSmartPointerLink<QSharedPointer>>,
                                      /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value,
                                                                        /*then*/InterfaceLink<PlainOwnedObjectSmartPointerLink<std::shared_ptr>>,
                                                                        /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,QSharedPointer<char>>::value,
                                                                                                          /*then*/InterfaceLink<PlainOwnedObjectSmartPointerLink<QWeakPointer>>,
                                                                                                          /*else*/InterfaceLink<PlainOwnedObjectSmartPointerLink<std::weak_ptr>>>::type>::type>::type Link;
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(pointer);
    Q_ASSERT(ownerFunction);
    checkValueOwner(env, ownerFunction, pointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    typename Link::SmartPointerType* smartPointerTarget{nullptr};

    std::unique_ptr<Link, QtJambiLink::LinkDisposer> qtJambiLink{new Link(env, nativeLink,
                                                                          LINK_NAME_ARG(qt_name)
                                                                          created_by_java, ownership, is_shell, pointer, smartPointerTarget, ocurredException)};
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    qtJambiLink->m_owner_function = ownerFunction;
    SharedPointerT sharedPointer(reinterpret_cast<char*>(pointer), SmartDeleterFunctionDeleter<SmartPointer>{deleterFunction, *qtJambiLink.get()});
    smartPointer = sharedPointer;
    *smartPointerTarget = typename Link::SmartPointerType(sharedPointer);
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if constexpr(std::is_same<SmartPointer<char>,QWeakPointer<char>>::value || std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value){
            qtJambiLink->QtJambiLink::setJavaOwnership(env);
        }else{
            qtJambiLink->QtJambiLink::setCppOwnership(env);
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  void* pointer,
                                                                                  SmartPointer<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    typedef typename std::conditional<std::is_same<SmartPointer<char>,QWeakPointer<char>>::value, QSharedPointer<char>,
                                      typename std::conditional<std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value, std::shared_ptr<char>, SmartPointer<char>>::type>::type SharedPointerT;
    typedef typename std::conditional</*if*/std::is_same<SmartPointer<char>,QWeakPointer<char>>::value,
                                      /*then*/InterfaceLink<PlainObjectSmartPointerLink<QSharedPointer>>,
                                      /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value,
                                                                        /*then*/InterfaceLink<PlainObjectSmartPointerLink<std::shared_ptr>>,
                                                                        /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,QSharedPointer<char>>::value,
                                                                                                          /*then*/InterfaceLink<PlainObjectSmartPointerLink<QWeakPointer>>,
                                                                                                          /*else*/InterfaceLink<PlainObjectSmartPointerLink<std::weak_ptr>>>::type>::type>::type Link;
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(pointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    typename Link::SmartPointerType* smartPointerTarget{nullptr};

    std::unique_ptr<Link, QtJambiLink::LinkDisposer> qtJambiLink{new Link(env, nativeLink,
                                                                          LINK_NAME_ARG(qt_name)
                                                                          created_by_java, ownership, is_shell, pointer, smartPointerTarget, ocurredException)};
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    SharedPointerT sharedPointer(reinterpret_cast<char*>(pointer), SmartDeleterFunctionDeleter<SmartPointer>{deleterFunction, *qtJambiLink.get()});
    smartPointer = sharedPointer;
    *smartPointerTarget = typename Link::SmartPointerType(sharedPointer);
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if constexpr(std::is_same<SmartPointer<char>,QWeakPointer<char>>::value || std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value){
            qtJambiLink->QtJambiLink::setJavaOwnership(env);
        }else{
            qtJambiLink->QtJambiLink::setCppOwnership(env);
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  SmartPointer<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction)
{
    typedef typename std::conditional<std::is_same<SmartPointer<char>,QWeakPointer<char>>::value, QSharedPointer<char>,
                                      typename std::conditional<std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value, std::shared_ptr<char>, SmartPointer<char>>::type>::type SharedPointerT;
    typedef typename std::conditional</*if*/std::is_same<SmartPointer<char>,QWeakPointer<char>>::value,
                                      /*then*/PlainOwnedObjectSmartPointerLink<QSharedPointer>,
                                      /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value,
                                                                        /*then*/PlainOwnedObjectSmartPointerLink<std::shared_ptr>,
                                                                        /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,QSharedPointer<char>>::value,
                                                                                                          /*then*/PlainOwnedObjectSmartPointerLink<QWeakPointer>,
                                                                                                          /*else*/PlainOwnedObjectSmartPointerLink<std::weak_ptr>>::type>::type>::type Link;
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(pointer);
    Q_ASSERT(ownerFunction);
    checkValueOwner(env, ownerFunction, pointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    typename Link::SmartPointerType* smartPointerTarget{nullptr};

    std::unique_ptr<Link, QtJambiLink::LinkDisposer> qtJambiLink{new Link(env, nativeLink,
                                                                          LINK_NAME_ARG(qt_name)
                                                                          created_by_java, ownership, is_shell, pointer, smartPointerTarget, ocurredException)};
    qtJambiLink->m_owner_function = ownerFunction;
    SharedPointerT sharedPointer(reinterpret_cast<char*>(pointer), SmartDeleterFunctionDeleter<SmartPointer>{deleterFunction, *qtJambiLink.get()});
    smartPointer = sharedPointer;
    *smartPointerTarget = typename Link::SmartPointerType(sharedPointer);
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if constexpr(std::is_same<SmartPointer<char>,QWeakPointer<char>>::value || std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value){
            qtJambiLink->QtJambiLink::setJavaOwnership(env);
        }else{
            qtJambiLink->QtJambiLink::setCppOwnership(env);
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  void* pointer,
                                                                                  SmartPointer<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction)
{
    typedef typename std::conditional<std::is_same<SmartPointer<char>,QWeakPointer<char>>::value, QSharedPointer<char>,
                                      typename std::conditional<std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value, std::shared_ptr<char>, SmartPointer<char>>::type>::type SharedPointerT;
    typedef typename std::conditional</*if*/std::is_same<SmartPointer<char>,QWeakPointer<char>>::value,
                                      /*then*/PlainObjectSmartPointerLink<QSharedPointer>,
                                      /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value,
                                                                        /*then*/PlainObjectSmartPointerLink<std::shared_ptr>,
                                                                        /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,QSharedPointer<char>>::value,
                                                                                                          /*then*/PlainObjectSmartPointerLink<QWeakPointer>,
                                                                                                          /*else*/PlainObjectSmartPointerLink<std::weak_ptr>>::type>::type>::type Link;
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(pointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    typename Link::SmartPointerType* smartPointerTarget{nullptr};

    std::unique_ptr<Link, QtJambiLink::LinkDisposer> qtJambiLink{new Link(env, nativeLink,
                                                                          LINK_NAME_ARG(qt_name)
                                                                          created_by_java, ownership, is_shell, pointer, smartPointerTarget, ocurredException)};
    SharedPointerT sharedPointer(reinterpret_cast<char*>(pointer), SmartDeleterFunctionDeleter<SmartPointer>{deleterFunction, *qtJambiLink.get()});
    smartPointer = sharedPointer;
    *smartPointerTarget = typename Link::SmartPointerType(sharedPointer);
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if constexpr(std::is_same<SmartPointer<char>,QWeakPointer<char>>::value || std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value){
            qtJambiLink->QtJambiLink::setJavaOwnership(env);
        }else{
            qtJambiLink->QtJambiLink::setCppOwnership(env);
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  void* pointer,
                                                                                  QSharedPointer<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForNewSmartPointerToObject<QSharedPointer>(env, javaObject,
                                                             created_by_java, ownership, is_shell,
                                                             pointer,
                                                             smartPointer,
                                                             elementMetaType,
                                                             interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  QSharedPointer<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForNewSmartPointerToObject<QSharedPointer>(env, javaObject,
                                                             created_by_java, ownership, is_shell,
                                                             ownerFunction,
                                                             pointer,
                                                             smartPointer,
                                                             elementMetaType,
                                                             interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  void* pointer,
                                                                                  std::shared_ptr<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForNewSmartPointerToObject<std::shared_ptr>(env, javaObject,
                                                             created_by_java, ownership, is_shell,
                                                             pointer,
                                                             smartPointer,
                                                             elementMetaType,
                                                             interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  std::shared_ptr<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForNewSmartPointerToObject<std::shared_ptr>(env, javaObject,
                                                             created_by_java, ownership, is_shell,
                                                             ownerFunction,
                                                             pointer,
                                                             smartPointer,
                                                             elementMetaType,
                                                             interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  void* pointer,
                                                                                  QWeakPointer<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForNewSmartPointerToObject<QWeakPointer>(env, javaObject,
                                                             created_by_java, ownership, is_shell,
                                                             pointer,
                                                             smartPointer,
                                                             elementMetaType,
                                                             interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  QWeakPointer<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForNewSmartPointerToObject<QWeakPointer>(env, javaObject,
                                                             created_by_java, ownership, is_shell,
                                                             ownerFunction,
                                                             pointer,
                                                             smartPointer,
                                                             elementMetaType,
                                                             interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  void* pointer,
                                                                                  std::weak_ptr<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForNewSmartPointerToObject<std::weak_ptr>(env, javaObject,
                                                             created_by_java, ownership, is_shell,
                                                             pointer,
                                                             smartPointer,
                                                             elementMetaType,
                                                             interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  std::weak_ptr<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForNewSmartPointerToObject<std::weak_ptr>(env, javaObject,
                                                             created_by_java, ownership, is_shell,
                                                             ownerFunction,
                                                             pointer,
                                                             smartPointer,
                                                             elementMetaType,
                                                             interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  void* pointer,
                                                                                  QSharedPointer<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForNewSmartPointerToObject<QSharedPointer>(env, javaObject,
                                                             LINK_NAME_ARG(qt_name)
                                                             created_by_java, ownership, is_shell,
                                                             pointer,
                                                             smartPointer,
                                                             deleterFunction,
                                                             interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  QSharedPointer<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForNewSmartPointerToObject<QSharedPointer>(env, javaObject,
                                                             LINK_NAME_ARG(qt_name)
                                                             created_by_java, ownership, is_shell,
                                                             ownerFunction,
                                                             pointer,
                                                             smartPointer,
                                                             deleterFunction,
                                                             interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  void* pointer,
                                                                                  QWeakPointer<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForNewSmartPointerToObject<QWeakPointer>(env, javaObject,
                                                             LINK_NAME_ARG(qt_name)
                                                             created_by_java, ownership, is_shell,
                                                             pointer,
                                                             smartPointer,
                                                             deleterFunction,
                                                             interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  QWeakPointer<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForNewSmartPointerToObject<QWeakPointer>(env, javaObject,
                                                             LINK_NAME_ARG(qt_name)
                                                             created_by_java, ownership, is_shell,
                                                             ownerFunction,
                                                             pointer,
                                                             smartPointer,
                                                             deleterFunction,
                                                             interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  void* pointer,
                                                                                  std::shared_ptr<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForNewSmartPointerToObject<std::shared_ptr>(env, javaObject,
                                                             LINK_NAME_ARG(qt_name)
                                                             created_by_java, ownership, is_shell,
                                                             pointer,
                                                             smartPointer,
                                                             deleterFunction,
                                                             interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  std::shared_ptr<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForNewSmartPointerToObject<std::shared_ptr>(env, javaObject,
                                                             LINK_NAME_ARG(qt_name)
                                                             created_by_java, ownership, is_shell,
                                                             ownerFunction,
                                                             pointer,
                                                             smartPointer,
                                                             deleterFunction,
                                                             interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  void* pointer,
                                                                                  std::weak_ptr<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForNewSmartPointerToObject<std::weak_ptr>(env, javaObject,
                                                             LINK_NAME_ARG(qt_name)
                                                             created_by_java, ownership, is_shell,
                                                             pointer,
                                                             smartPointer,
                                                             deleterFunction,
                                                             interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  std::weak_ptr<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForNewSmartPointerToObject<std::weak_ptr>(env, javaObject,
                                                             LINK_NAME_ARG(qt_name)
                                                             created_by_java, ownership, is_shell,
                                                             ownerFunction,
                                                             pointer,
                                                             smartPointer,
                                                             deleterFunction,
                                                             interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, SafeBool is_shell,
                                                                                  const QSharedPointer<char>& smartPointer,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForSmartPointerToObject<QSharedPointer>(env, javaObject,
                                                             LINK_NAME_ARG(qt_name)
                                                             created_by_java, is_shell,
                                                             smartPointer,
                                                             interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  const QSharedPointer<char>& smartPointer,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForSmartPointerToObject<QSharedPointer>(env, javaObject,
                                                             LINK_NAME_ARG(qt_name)
                                                             created_by_java, is_shell,
                                                             ownerFunction,
                                                             smartPointer,
                                                             interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, SafeBool is_shell,
                                                                                  const std::shared_ptr<char>& smartPointer,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForSmartPointerToObject<std::shared_ptr>(env, javaObject,
                                                             LINK_NAME_ARG(qt_name)
                                                             created_by_java, is_shell,
                                                             smartPointer,
                                                             interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  const std::shared_ptr<char>& smartPointer,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForSmartPointerToObject<std::shared_ptr>(env, javaObject,
                                                             LINK_NAME_ARG(qt_name)
                                                             created_by_java, is_shell,
                                                             ownerFunction,
                                                             smartPointer,
                                                             interfaceOffsetInfo);
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                              LINK_NAME_ARG(const char* qt_name)
                                              SafeBool created_by_java, SafeBool is_shell,
                                              const SmartPointer<char>& smartPointer)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(smartPointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    SmartPointer<char>* smartPointerTarget{nullptr};
    typedef PlainObjectSmartPointerLink<SmartPointer> Link;
    std::unique_ptr<Link, QtJambiLink::LinkDisposer> qtJambiLink{new Link(env, nativeLink,
                                                                            LINK_NAME_ARG(qt_name)
                                                                            created_by_java, QtJambiLink::Ownership::None, is_shell, smartPointer.get(), smartPointerTarget, ocurredException)};
    *smartPointerTarget = smartPointer;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        qtJambiLink->setJavaOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                              LINK_NAME_ARG(const char* qt_name)
                                              SafeBool created_by_java, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                              const SmartPointer<char>& smartPointer)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(smartPointer);
    Q_ASSERT(ownerFunction);
    checkValueOwner(env, ownerFunction, smartPointer.get());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    SmartPointer<char>* smartPointerTarget{nullptr};
    typedef PlainOwnedObjectSmartPointerLink<SmartPointer> Link;
    std::unique_ptr<Link, QtJambiLink::LinkDisposer> qtJambiLink{new Link(env, nativeLink,
                                                                            LINK_NAME_ARG(qt_name)
                                                                            created_by_java, QtJambiLink::Ownership::None, is_shell, smartPointer.get(), smartPointerTarget, ocurredException)};
    qtJambiLink->m_owner_function = ownerFunction;
    *smartPointerTarget = smartPointer;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        qtJambiLink->setJavaOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                              LINK_NAME_ARG(const char* qt_name)
                                              SafeBool created_by_java, SafeBool is_shell,
                                              const QSharedPointer<char>& smartPointer)
{
    return createLinkForSmartPointerToObject<QSharedPointer>(env, javaObject,
                                                      LINK_NAME_ARG(qt_name)
                                                      created_by_java,
                                                      is_shell,
                                                      smartPointer);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                              LINK_NAME_ARG(const char* qt_name)
                                              SafeBool created_by_java, SafeBool is_shell,
                                              const std::shared_ptr<char>& smartPointer)
{
    return createLinkForSmartPointerToObject<std::shared_ptr>(env, javaObject,
                                                      LINK_NAME_ARG(qt_name)
                                                      created_by_java,
                                                      is_shell,
                                                      smartPointer);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                              LINK_NAME_ARG(const char* qt_name)
                                              SafeBool created_by_java, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                              const QSharedPointer<char>& smartPointer)
{
    return createLinkForSmartPointerToObject<QSharedPointer>(env, javaObject,
                                                      LINK_NAME_ARG(qt_name)
                                                      created_by_java,
                                                      is_shell,
                                                      ownerFunction,
                                                      smartPointer);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                              LINK_NAME_ARG(const char* qt_name)
                                              SafeBool created_by_java, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                              const std::shared_ptr<char>& smartPointer)
{
    return createLinkForSmartPointerToObject<std::shared_ptr>(env, javaObject,
                                                      LINK_NAME_ARG(qt_name)
                                                      created_by_java,
                                                      is_shell,
                                                      ownerFunction,
                                                      smartPointer);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  void* pointer,
                                                                                  QSharedPointer<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType)
{
    return createLinkForNewSmartPointerToObject<QSharedPointer>(env,
                                                             javaObject,
                                                             created_by_java,
                                                             ownership, is_shell,
                                                             pointer,
                                                             smartPointer,
                                                             elementMetaType);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  QSharedPointer<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType)
{
    return createLinkForNewSmartPointerToObject<QSharedPointer>(env,
                                                             javaObject,
                                                             created_by_java,
                                                             ownership, is_shell,
                                                             ownerFunction,
                                                             pointer,
                                                             smartPointer,
                                                             elementMetaType);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  void* pointer,
                                                                                  QWeakPointer<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType)
{
    return createLinkForNewSmartPointerToObject<QWeakPointer>(env,
                                                             javaObject,
                                                             created_by_java,
                                                             ownership, is_shell,
                                                             pointer,
                                                             smartPointer,
                                                             elementMetaType);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  QWeakPointer<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType)
{
    return createLinkForNewSmartPointerToObject<QWeakPointer>(env,
                                                             javaObject,
                                                             created_by_java,
                                                             ownership, is_shell,
                                                             ownerFunction,
                                                             pointer,
                                                             smartPointer,
                                                             elementMetaType);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  void* pointer,
                                                                                  std::shared_ptr<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType)
{
    return createLinkForNewSmartPointerToObject<std::shared_ptr>(env,
                                                             javaObject,
                                                             created_by_java,
                                                             ownership, is_shell,
                                                             pointer,
                                                             smartPointer,
                                                             elementMetaType);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  std::shared_ptr<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType)
{
    return createLinkForNewSmartPointerToObject<std::shared_ptr>(env,
                                                             javaObject,
                                                             created_by_java,
                                                             ownership, is_shell,
                                                             ownerFunction,
                                                             pointer,
                                                             smartPointer,
                                                             elementMetaType);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  void* pointer,
                                                                                  std::weak_ptr<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType)
{
    return createLinkForNewSmartPointerToObject<std::weak_ptr>(env,
                                                             javaObject,
                                                             created_by_java,
                                                             ownership, is_shell,
                                                             pointer,
                                                             smartPointer,
                                                             elementMetaType);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  std::weak_ptr<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType)
{
    return createLinkForNewSmartPointerToObject<std::weak_ptr>(env,
                                                             javaObject,
                                                             created_by_java,
                                                             ownership, is_shell,
                                                             ownerFunction,
                                                             pointer,
                                                             smartPointer,
                                                             elementMetaType);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  void* pointer,
                                                                                  QSharedPointer<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction)
{
    return createLinkForNewSmartPointerToObject<QSharedPointer>(env, javaObject,
                                                             LINK_NAME_ARG(qt_name)
                                                             created_by_java,
                                                             ownership, is_shell,
                                                             pointer,
                                                             smartPointer,
                                                             deleterFunction);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  QSharedPointer<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction)
{
    return createLinkForNewSmartPointerToObject<QSharedPointer>(env, javaObject,
                                                             LINK_NAME_ARG(qt_name)
                                                             created_by_java,
                                                             ownership, is_shell,
                                                             ownerFunction,
                                                             pointer,
                                                             smartPointer,
                                                             deleterFunction);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  void* pointer,
                                                                                  std::shared_ptr<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction)
{
    return createLinkForNewSmartPointerToObject<std::shared_ptr>(env, javaObject,
                                                              LINK_NAME_ARG(qt_name)
                                                              created_by_java,
                                                              ownership, is_shell,
                                                              pointer,
                                                              smartPointer,
                                                              deleterFunction);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  std::shared_ptr<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction)
{
    return createLinkForNewSmartPointerToObject<std::shared_ptr>(env, javaObject,
                                                              LINK_NAME_ARG(qt_name)
                                                              created_by_java,
                                                              ownership, is_shell,
                                                              ownerFunction,
                                                              pointer,
                                                              smartPointer,
                                                              deleterFunction);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  void* pointer,
                                                                                  QWeakPointer<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction)
{
    return createLinkForNewSmartPointerToObject<QWeakPointer>(env, javaObject,
                                                             LINK_NAME_ARG(qt_name)
                                                             created_by_java,
                                                             ownership, is_shell,
                                                             pointer,
                                                             smartPointer,
                                                             deleterFunction);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  QWeakPointer<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction)
{
    return createLinkForNewSmartPointerToObject<QWeakPointer>(env, javaObject,
                                                             LINK_NAME_ARG(qt_name)
                                                             created_by_java,
                                                             ownership, is_shell,
                                                             ownerFunction,
                                                             pointer,
                                                             smartPointer,
                                                             deleterFunction);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell,
                                                                                  void* pointer,
                                                                                  std::weak_ptr<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction)
{
    return createLinkForNewSmartPointerToObject<std::weak_ptr>(env, javaObject,
                                                              LINK_NAME_ARG(qt_name)
                                                              created_by_java,
                                                              ownership, is_shell,
                                                              pointer,
                                                              smartPointer,
                                                              deleterFunction);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  std::weak_ptr<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction)
{
    return createLinkForNewSmartPointerToObject<std::weak_ptr>(env, javaObject,
                                                              LINK_NAME_ARG(qt_name)
                                                              created_by_java,
                                                              ownership, is_shell,
                                                              ownerFunction,
                                                              pointer,
                                                              smartPointer,
                                                              deleterFunction);
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                     LINK_NAME_ARG(const char* qt_name)
                                                                                     const SmartPointer<char>& smartPointer,
                                                                                     AbstractContainerAccess* containerAccess,
                                                                                     const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(smartPointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    SmartPointer<char>* smartPointerTarget{nullptr};
    typedef InterfaceLink<ContainerObjectSmartPointerLink<SmartPointer>> Link;
    std::unique_ptr<Link, QtJambiLink::LinkDisposer> qtJambiLink{new Link(env, nativeLink,
                                                                                 LINK_NAME_ARG(qt_name)
                                                                                 false, QtJambiLink::Ownership::None, false,
                                                                                 smartPointer.get(), smartPointerTarget, ocurredException)};
    qtJambiLink->m_containerAccess = containerAccess;
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);

    *smartPointerTarget = smartPointer;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        qtJambiLink->setJavaOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                     LINK_NAME_ARG(const char* qt_name)
                                                                                     const SmartPointer<char>& smartPointer,
                                                                                     AbstractContainerAccess* containerAccess)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(smartPointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    SmartPointer<char>* smartPointerTarget{nullptr};
    typedef ContainerObjectSmartPointerLink<SmartPointer> Link;
    std::unique_ptr<Link, QtJambiLink::LinkDisposer> qtJambiLink{new Link(env, nativeLink,
                                                                            LINK_NAME_ARG(qt_name)
                                                                            false, QtJambiLink::Ownership::None, false,
                                                                            smartPointer.get(), smartPointerTarget, ocurredException)};
    qtJambiLink->m_containerAccess = containerAccess;
    *smartPointerTarget = smartPointer;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        qtJambiLink->setJavaOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                     LINK_NAME_ARG(const char* qt_name)
                                                                                     QtJambiLink::Ownership ownership,
                                                                                     void* pointer, SmartPointer<char>& smartPointer,
                                                                                     AbstractContainerAccess* containerAccess,
                                                                                     const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    typedef typename std::conditional<std::is_same<SmartPointer<char>,QWeakPointer<char>>::value, QSharedPointer<char>,
                                      typename std::conditional<std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value, std::shared_ptr<char>, SmartPointer<char>>::type>::type SharedPointerT;
    typedef typename std::conditional</*if*/std::is_same<SmartPointer<char>,QWeakPointer<char>>::value,
                                      /*then*/InterfaceLink<ContainerObjectSmartPointerLink<QSharedPointer>>,
                                      /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value,
                                                                        /*then*/InterfaceLink<ContainerObjectSmartPointerLink<std::shared_ptr>>,
                                                                        /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,QSharedPointer<char>>::value,
                                                                                                          /*then*/InterfaceLink<ContainerObjectSmartPointerLink<QWeakPointer>>,
                                                                                                          /*else*/InterfaceLink<ContainerObjectSmartPointerLink<std::weak_ptr>>>::type>::type>::type Link;
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(pointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    typename Link::SmartPointerType* smartPointerTarget{nullptr};

    std::unique_ptr<Link, QtJambiLink::LinkDisposer> qtJambiLink{new Link(env, nativeLink,
                                                                          LINK_NAME_ARG(qt_name)
                                                                          false, ownership, false,
                                                                          pointer, smartPointerTarget, ocurredException)};
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    qtJambiLink->m_containerAccess = containerAccess;
    SharedPointerT sharedPointer(reinterpret_cast<char*>(pointer), SmartContainerDeleter<SmartPointer>(containerAccess, *qtJambiLink.get()));
    smartPointer = sharedPointer;
    *smartPointerTarget = typename Link::SmartPointerType(sharedPointer);
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if constexpr(std::is_same<SmartPointer<char>,QWeakPointer<char>>::value || std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value){
            qtJambiLink->QtJambiLink::setJavaOwnership(env);
        }else{
            qtJambiLink->QtJambiLink::setCppOwnership(env);
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

template<template<typename> class SmartPointer>
QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                     LINK_NAME_ARG(const char* qt_name)
                                                                                     QtJambiLink::Ownership ownership,
                                                                                     void* pointer, SmartPointer<char>& smartPointer,
                                                                                     AbstractContainerAccess* containerAccess)
{
    typedef typename std::conditional<std::is_same<SmartPointer<char>,QWeakPointer<char>>::value, QSharedPointer<char>,
                                      typename std::conditional<std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value, std::shared_ptr<char>, SmartPointer<char>>::type>::type SharedPointerT;
    typedef typename std::conditional</*if*/std::is_same<SmartPointer<char>,QWeakPointer<char>>::value,
                                      /*then*/ContainerObjectSmartPointerLink<QSharedPointer>,
                                      /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value,
                                                                        /*then*/ContainerObjectSmartPointerLink<std::shared_ptr>,
                                                                        /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,QSharedPointer<char>>::value,
                                                                                                          /*then*/ContainerObjectSmartPointerLink<QWeakPointer>,
                                                                                                          /*else*/ContainerObjectSmartPointerLink<std::weak_ptr>>::type>::type>::type Link;
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(pointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    typename Link::SmartPointerType* smartPointerTarget{nullptr};

    std::unique_ptr<Link, QtJambiLink::LinkDisposer> qtJambiLink{new Link(env, nativeLink,
                                                                          LINK_NAME_ARG(qt_name)
                                                                          false, ownership, false, pointer, smartPointerTarget, ocurredException)};
    qtJambiLink->m_containerAccess = containerAccess;
    SharedPointerT sharedPointer(reinterpret_cast<char*>(pointer), SmartContainerDeleter<SmartPointer>(containerAccess, *qtJambiLink.get()));
    smartPointer = sharedPointer;
    *smartPointerTarget = typename Link::SmartPointerType(sharedPointer);
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if constexpr(std::is_same<SmartPointer<char>,QWeakPointer<char>>::value || std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value){
            qtJambiLink->QtJambiLink::setJavaOwnership(env);
        }else{
            qtJambiLink->QtJambiLink::setCppOwnership(env);
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                     LINK_NAME_ARG(const char* qt_name)
                                                                                     const QSharedPointer<char>& smartPointer,
                                                                                     AbstractContainerAccess* containerAccess,
                                                                                     const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForSmartPointerToObject<QSharedPointer>(env, javaObject,
                                                                LINK_NAME_ARG(qt_name)
                                                                smartPointer,
                                                                containerAccess,
                                                                interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                     LINK_NAME_ARG(const char* qt_name)
                                                                                     const QSharedPointer<char>& smartPointer,
                                                                                     AbstractContainerAccess* containerAccess)
{
    return createLinkForSmartPointerToObject<QSharedPointer>(env,
                                                         javaObject,
                                                         LINK_NAME_ARG(qt_name)
                                                         smartPointer,
                                                         containerAccess);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                     LINK_NAME_ARG(const char* qt_name)
                                                                                     const std::shared_ptr<char>& smartPointer,
                                                                                     AbstractContainerAccess* containerAccess,
                                                                                     const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForSmartPointerToObject<std::shared_ptr>(env, javaObject,
                                                                LINK_NAME_ARG(qt_name)
                                                                smartPointer,
                                                                containerAccess,
                                                                interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                     LINK_NAME_ARG(const char* qt_name)
                                                                                     const std::shared_ptr<char>& smartPointer,
                                                                                     AbstractContainerAccess* containerAccess)
{
    return createLinkForSmartPointerToObject<std::shared_ptr>(env,
                                                         javaObject,
                                                         LINK_NAME_ARG(qt_name)
                                                         smartPointer,
                                                         containerAccess);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                     LINK_NAME_ARG(const char* qt_name)
                                                                                     QtJambiLink::Ownership ownership,
                                                                                     void* pointer, QSharedPointer<char>& smartPointer,
                                                                                     AbstractContainerAccess* containerAccess,
                                                                                     const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForNewSmartPointerToObject<QSharedPointer>(env, javaObject,
                                                                LINK_NAME_ARG(qt_name)
                                                                ownership, pointer, smartPointer,
                                                                containerAccess,
                                                                interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                     LINK_NAME_ARG(const char* qt_name)
                                                                                     QtJambiLink::Ownership ownership,
                                                                                     void* pointer, QSharedPointer<char>& smartPointer,
                                                                                     AbstractContainerAccess* containerAccess)
{
    return createLinkForNewSmartPointerToObject<QSharedPointer>(env,
                                                         javaObject,
                                                         LINK_NAME_ARG(qt_name)
                                                         ownership, pointer, smartPointer,
                                                         containerAccess);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                     LINK_NAME_ARG(const char* qt_name)
                                                                                     QtJambiLink::Ownership ownership,
                                                                                     void* pointer, std::shared_ptr<char>& smartPointer,
                                                                                     AbstractContainerAccess* containerAccess,
                                                                                     const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForNewSmartPointerToObject<std::shared_ptr>(env, javaObject,
                                                                LINK_NAME_ARG(qt_name)
                                                                ownership, pointer, smartPointer,
                                                                containerAccess,
                                                                interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                     LINK_NAME_ARG(const char* qt_name)
                                                                                     QtJambiLink::Ownership ownership,
                                                                                     void* pointer, std::shared_ptr<char>& smartPointer,
                                                                                     AbstractContainerAccess* containerAccess)
{
    return createLinkForNewSmartPointerToObject<std::shared_ptr>(env,
                                                         javaObject,
                                                         LINK_NAME_ARG(qt_name)
                                                         ownership, pointer, smartPointer,
                                                         containerAccess);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                     LINK_NAME_ARG(const char* qt_name)
                                                                                     QtJambiLink::Ownership ownership,
                                                                                     void* pointer, QWeakPointer<char>& smartPointer,
                                                                                     AbstractContainerAccess* containerAccess,
                                                                                     const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForNewSmartPointerToObject<QWeakPointer>(env, javaObject,
                                                                LINK_NAME_ARG(qt_name)
                                                                ownership, pointer, smartPointer,
                                                                containerAccess,
                                                                interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                     LINK_NAME_ARG(const char* qt_name)
                                                                                     QtJambiLink::Ownership ownership,
                                                                                     void* pointer, QWeakPointer<char>& smartPointer,
                                                                                     AbstractContainerAccess* containerAccess)
{
    return createLinkForNewSmartPointerToObject<QWeakPointer>(env,
                                                         javaObject,
                                                         LINK_NAME_ARG(qt_name)
                                                         ownership, pointer, smartPointer,
                                                         containerAccess);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                     LINK_NAME_ARG(const char* qt_name)
                                                                                     QtJambiLink::Ownership ownership,
                                                                                     void* pointer, std::weak_ptr<char>& smartPointer,
                                                                                     AbstractContainerAccess* containerAccess,
                                                                                     const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    return createLinkForNewSmartPointerToObject<std::weak_ptr>(env, javaObject,
                                                                LINK_NAME_ARG(qt_name)
                                                                ownership, pointer, smartPointer,
                                                                containerAccess,
                                                                interfaceOffsetInfo);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                     LINK_NAME_ARG(const char* qt_name)
                                                                                     QtJambiLink::Ownership ownership,
                                                                                     void* pointer, std::weak_ptr<char>& smartPointer,
                                                                                     AbstractContainerAccess* containerAccess)
{
    return createLinkForNewSmartPointerToObject<std::weak_ptr>(env,
                                                         javaObject,
                                                         LINK_NAME_ARG(qt_name)
                                                         ownership,
                                                         pointer, smartPointer,
                                                         containerAccess);
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                   SafeBool created_by_java, SafeBool isCopy,
                                                                   AbstractContainerAccess* containerAccess,
                                                                   QtJambiLink::Ownership ownership)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(containerAccess);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    typedef PointerToContainerLink Link;
    std::unique_ptr<Link, QtJambiLink::LinkDisposer> qtJambiLink{new Link(env, nativeLink,
                                                                          LINK_NAME_ARG(qt_name)
                                                                          ptr,
                                                                          created_by_java, false,
                                                                          ocurredException)};
    qtJambiLink->m_containerAccess = containerAccess;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        switch(ownership){
        case QtJambiLink::Ownership::Java:
            qtJambiLink->setJavaOwnership(env);
            break;
        case QtJambiLink::Ownership::Cpp:
            qtJambiLink->setCppOwnership(env);
            break;
        default:
            if(created_by_java || isCopy){
                qtJambiLink->setJavaOwnership(env);
            }
            break;
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                   AbstractContainerAccess* containerAccess,
                                                                   SafeBool cppOwnership,
                                                                   TypeInfoSupplier typeInfoSupplier)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(containerAccess);
    Q_ASSERT(typeInfoSupplier);
    if(enabledDanglingPointerCheck() && checkedGetTypeInfo(typeInfoSupplier, ptr)==nullptr){
        QString className = QtJambiAPI::getObjectClassName(env, javaObject).replace(QLatin1Char('/'), QLatin1Char('.')).replace(QLatin1Char('$'), QLatin1Char('.'));
        Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Unable to convert dangling pointer %p to object of type %s", ptr, qPrintable(className)) QTJAMBI_STACKTRACEINFO );
    }

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    typedef DanglingPointerCheckingLink<PointerToContainerLink> Link;
    std::unique_ptr<Link, QtJambiLink::LinkDisposer> qtJambiLink{new Link(env, nativeLink,
                                                                          LINK_NAME_ARG(qt_name)
                                                                          ptr,
                                                                          false, false,
                                                                          ocurredException)};
    qtJambiLink->m_containerAccess = containerAccess;
    qtJambiLink->m_typeInfoSupplier = typeInfoSupplier;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if(cppOwnership)
            qtJambiLink->setCppOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                   SafeBool created_by_java, SafeBool isCopy,
                                                                   AbstractContainerAccess* containerAccess,
                                                                   QtJambiLink::Ownership ownership,
                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(containerAccess);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<InterfaceLink<PointerToContainerLink>, QtJambiLink::LinkDisposer> qtJambiLink{new InterfaceLink<PointerToContainerLink>(env,
                                                                                                                       nativeLink,
                                                                                                                       LINK_NAME_ARG(qt_name)
                                                                                                                       ptr, created_by_java, false,
                                                                                                                       ocurredException)};
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    qtJambiLink->m_containerAccess = containerAccess;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        switch(ownership){
        case QtJambiLink::Ownership::Java:
            qtJambiLink->setJavaOwnership(env);
            break;
        case QtJambiLink::Ownership::Cpp:
            qtJambiLink->setCppOwnership(env);
            break;
        default:
            if(created_by_java || isCopy){
                qtJambiLink->setJavaOwnership(env);
            }
            break;
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                   AbstractContainerAccess* containerAccess,
                                                                   SafeBool cppOwnership,
                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                   TypeInfoSupplier typeInfoSupplier)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(containerAccess);
    Q_ASSERT(typeInfoSupplier);
    if(enabledDanglingPointerCheck() && checkedGetTypeInfo(typeInfoSupplier, ptr)==nullptr){
        QString className = QtJambiAPI::getObjectClassName(env, javaObject).replace(QLatin1Char('/'), QLatin1Char('.')).replace(QLatin1Char('$'), QLatin1Char('.'));
        Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Unable to convert dangling pointer %p to object of type %s", ptr, qPrintable(className)) QTJAMBI_STACKTRACEINFO );
    }

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DanglingPointerCheckingLink<InterfaceLink<PointerToContainerLink>>, QtJambiLink::LinkDisposer> qtJambiLink{new DanglingPointerCheckingLink<InterfaceLink<PointerToContainerLink>>(env, nativeLink,
                                                                                                                                                          LINK_NAME_ARG(qt_name)
                                                                                                                                                          ptr,
                                                                                                                                                          false, false,
                                                                                                                                                          ocurredException)};
    qtJambiLink->m_containerAccess = containerAccess;
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    qtJambiLink->m_typeInfoSupplier = typeInfoSupplier;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if(cppOwnership)
            qtJambiLink->setCppOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                   PtrDeleterFunction destructor_function,
                                                                   AbstractContainerAccess* containerAccess,
                                                                   SafeBool cppOwnership,
                                                                   TypeInfoSupplier typeInfoSupplier)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(containerAccess);
    Q_ASSERT(destructor_function);
    Q_ASSERT(typeInfoSupplier);
    if(enabledDanglingPointerCheck() && checkedGetTypeInfo(typeInfoSupplier, ptr)==nullptr){
        QString className = QtJambiAPI::getObjectClassName(env, javaObject).replace(QLatin1Char('/'), QLatin1Char('.')).replace(QLatin1Char('$'), QLatin1Char('.'));
        Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Unable to convert dangling pointer %p to object of type %s", ptr, qPrintable(className)) QTJAMBI_STACKTRACEINFO );
    }

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DanglingPointerCheckingLink<DeletablePointerToContainerLink>, QtJambiLink::LinkDisposer> qtJambiLink{new DanglingPointerCheckingLink<DeletablePointerToContainerLink>(env, nativeLink,
                                                                                                                                                          LINK_NAME_ARG(qt_name)
                                                                                                                                                          ptr, false, false,
                                                                                                                                                          ocurredException)};
    qtJambiLink->m_deleter_function = destructor_function;
    qtJambiLink->m_containerAccess = containerAccess;
    qtJambiLink->m_typeInfoSupplier = typeInfoSupplier;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if(cppOwnership)
            qtJambiLink->setCppOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                   SafeBool created_by_java, SafeBool isCopy,
                                                                   PtrDeleterFunction destructor_function,
                                                                   AbstractContainerAccess* containerAccess,
                                                                   QtJambiLink::Ownership ownership)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(containerAccess);
    Q_ASSERT(destructor_function);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DeletablePointerToContainerLink, QtJambiLink::LinkDisposer> qtJambiLink{new DeletablePointerToContainerLink(env, nativeLink,
                                                                                                                                LINK_NAME_ARG(qt_name)
                                                                                                                                ptr, created_by_java,
                                                                                                                                false,
                                                                                                                                ocurredException)};
    qtJambiLink->m_deleter_function = destructor_function;
    qtJambiLink->m_containerAccess = containerAccess;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        switch(ownership){
        case QtJambiLink::Ownership::Java:
            qtJambiLink->setJavaOwnership(env);
            break;
        case QtJambiLink::Ownership::Cpp:
            qtJambiLink->setCppOwnership(env);
            break;
        default:
            if(created_by_java || isCopy){
                qtJambiLink->setJavaOwnership(env);
            }
            break;
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                   PtrDeleterFunction destructor_function,
                                                                   AbstractContainerAccess* containerAccess,
                                                                   SafeBool cppOwnership,
                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                   TypeInfoSupplier typeInfoSupplier)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(containerAccess);
    Q_ASSERT(destructor_function);
    if(enabledDanglingPointerCheck() && checkedGetTypeInfo(typeInfoSupplier, ptr)==nullptr){
        QString className = QtJambiAPI::getObjectClassName(env, javaObject).replace(QLatin1Char('/'), QLatin1Char('.')).replace(QLatin1Char('$'), QLatin1Char('.'));
        Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Unable to convert dangling pointer %p to object of type %s", ptr, qPrintable(className)) QTJAMBI_STACKTRACEINFO );
    }

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DanglingPointerCheckingLink<InterfaceLink<DeletablePointerToContainerLink>>, QtJambiLink::LinkDisposer> qtJambiLink{new DanglingPointerCheckingLink<InterfaceLink<DeletablePointerToContainerLink>>(env, nativeLink,
                                                                                                                                                                            LINK_NAME_ARG(qt_name)
                                                                                                                                                                            ptr, false, false,
                                                                                                                                                                            ocurredException)};
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    qtJambiLink->m_typeInfoSupplier = typeInfoSupplier;
    qtJambiLink->m_deleter_function = destructor_function;
    qtJambiLink->m_containerAccess = containerAccess;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if(cppOwnership)
            qtJambiLink->setCppOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                   SafeBool created_by_java, SafeBool isCopy,
                                                                   PtrDeleterFunction destructor_function,
                                                                   AbstractContainerAccess* containerAccess,
                                                                   QtJambiLink::Ownership ownership,
                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(containerAccess);
    Q_ASSERT(destructor_function);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<InterfaceLink<DeletablePointerToContainerLink>, QtJambiLink::LinkDisposer> qtJambiLink{new InterfaceLink<DeletablePointerToContainerLink>(env, nativeLink,
                                                                                                                                                    LINK_NAME_ARG(qt_name)
                                                                                                                                                    ptr, created_by_java,
                                                                                                                                                    false,
                                                                                                                                                    ocurredException)};
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    qtJambiLink->m_deleter_function = destructor_function;
    qtJambiLink->m_containerAccess = containerAccess;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        switch(ownership){
        case QtJambiLink::Ownership::Java:
            qtJambiLink->setJavaOwnership(env);
            break;
        case QtJambiLink::Ownership::Cpp:
            qtJambiLink->setCppOwnership(env);
            break;
        default:
            if(created_by_java || isCopy){
                qtJambiLink->setJavaOwnership(env);
            }
            break;
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                   SafeBool created_by_java, SafeBool is_shell,
                                                                   PtrOwnerFunction ownerFunction,
                                                                   QtJambiLink::Ownership ownership)
{
    if(Q_UNLIKELY(!ownerFunction)){
        return createLinkForNativeObject(env, javaObject, ptr, metaType,
                                   created_by_java, is_shell, ownership);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(ownerFunction);
    Q_ASSERT(metaType.isValid());
    checkValueOwner(env, ownerFunction, ptr);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<OwnedMetaTypedObjectLink, QtJambiLink::LinkDisposer> qtJambiLink{new OwnedMetaTypedObjectLink(env, nativeLink,
                                                                                                                                    LINK_NAME_ARG(metaType.name())
                                                                                                                                    ptr, created_by_java,
                                                                                                                                    is_shell, ocurredException)};
    qtJambiLink->m_meta_type = metaType;
    qtJambiLink->m_owner_function = ownerFunction;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        switch(ownership){
        case QtJambiLink::Ownership::Java:
            qtJambiLink->setJavaOwnership(env);
            break;
        case QtJambiLink::Ownership::Cpp:
            qtJambiLink->setCppOwnership(env);
            break;
        default:
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            break;
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                   PtrOwnerFunction ownerFunction,
                                                                   SafeBool cppOwnership,
                                                                   TypeInfoSupplier typeInfoSupplier)
{
    if(Q_UNLIKELY(!ownerFunction)){
        return createLinkForNativeObject(env, javaObject, ptr, metaType, cppOwnership, typeInfoSupplier);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(ownerFunction);
    Q_ASSERT(metaType.isValid());
    Q_ASSERT(typeInfoSupplier);
    if(enabledDanglingPointerCheck() && checkedGetTypeInfo(typeInfoSupplier, ptr)==nullptr){
        QString className = QtJambiAPI::getObjectClassName(env, javaObject).replace(QLatin1Char('/'), QLatin1Char('.')).replace(QLatin1Char('$'), QLatin1Char('.'));
        Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Unable to convert dangling pointer %p to object of type %s", ptr, qPrintable(className)) QTJAMBI_STACKTRACEINFO );
    }
    checkValueOwner(env, ownerFunction, ptr);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DanglingPointerCheckingLink<OwnedMetaTypedObjectLink>, QtJambiLink::LinkDisposer> qtJambiLink{new DanglingPointerCheckingLink<OwnedMetaTypedObjectLink>(env, nativeLink,
                                                                                                                                                              LINK_NAME_ARG(metaType.name())
                                                                                                                                                              ptr, false,
                                                                                                                                                              false, ocurredException)};
    qtJambiLink->m_meta_type = metaType;
    qtJambiLink->m_owner_function = ownerFunction;
    qtJambiLink->m_typeInfoSupplier = typeInfoSupplier;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if(cppOwnership)
            qtJambiLink->setCppOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                   SafeBool created_by_java, SafeBool is_shell,
                                                                   PtrOwnerFunction ownerFunction, QtJambiLink::Ownership ownership,
                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    if(Q_UNLIKELY(!ownerFunction)){
        return createLinkForNativeObject(env, javaObject, ptr, metaType,
                                         created_by_java, is_shell, ownership,
                                         interfaceOffsetInfo);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(ownerFunction);
    Q_ASSERT(metaType.isValid());
    checkValueOwner(env, ownerFunction, ptr);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<InterfaceLink<OwnedMetaTypedObjectLink>, QtJambiLink::LinkDisposer> qtJambiLink{new InterfaceLink<OwnedMetaTypedObjectLink>(env, nativeLink,
                                                                                                                                                                  LINK_NAME_ARG(metaType.name())
                                                                                                                                                                  ptr, created_by_java,
                                                                                                                                                                  is_shell, ocurredException)};
    qtJambiLink->m_meta_type = metaType;
    qtJambiLink->m_owner_function = ownerFunction;
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        switch(ownership){
        case QtJambiLink::Ownership::Java:
            qtJambiLink->setJavaOwnership(env);
            break;
        case QtJambiLink::Ownership::Cpp:
            qtJambiLink->setCppOwnership(env);
            break;
        default:
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            break;
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                   PtrOwnerFunction ownerFunction, SafeBool cppOwnership,
                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                   TypeInfoSupplier typeInfoSupplier)
{
    if(Q_UNLIKELY(!ownerFunction)){
        return createLinkForNativeObject(env, javaObject, ptr, metaType,
                                         cppOwnership, interfaceOffsetInfo, typeInfoSupplier);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(ownerFunction);
    Q_ASSERT(metaType.isValid());
    Q_ASSERT(typeInfoSupplier);
    if(enabledDanglingPointerCheck() && checkedGetTypeInfo(typeInfoSupplier, ptr)==nullptr){
        QString className = QtJambiAPI::getObjectClassName(env, javaObject).replace(QLatin1Char('/'), QLatin1Char('.')).replace(QLatin1Char('$'), QLatin1Char('.'));
        Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Unable to convert dangling pointer %p to object of type %s", ptr, qPrintable(className)) QTJAMBI_STACKTRACEINFO );
    }
    checkValueOwner(env, ownerFunction, ptr);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DanglingPointerCheckingLink<InterfaceLink<OwnedMetaTypedObjectLink>>, QtJambiLink::LinkDisposer> qtJambiLink{new DanglingPointerCheckingLink<InterfaceLink<OwnedMetaTypedObjectLink>>(env, nativeLink,
                                                                                                                                                                                            LINK_NAME_ARG(metaType.name())
                                                                                                                                                                                            ptr, false,
                                                                                                                                                                                            false, ocurredException)};
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    qtJambiLink->m_meta_type = metaType;
    qtJambiLink->m_owner_function = ownerFunction;
    qtJambiLink->m_typeInfoSupplier = typeInfoSupplier;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if(cppOwnership)
            qtJambiLink->setCppOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                          SafeBool created_by_java, SafeBool is_shell, QtJambiLink::Ownership ownership)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(metaType.isValid());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<MetaTypedObjectLink, QtJambiLink::LinkDisposer> qtJambiLink{new MetaTypedObjectLink(env, nativeLink,
                                                                                                                          LINK_NAME_ARG(metaType.name())
                                                                                                                          ptr, created_by_java, is_shell,
                                                                                                                          ocurredException)};
    qtJambiLink->m_meta_type = metaType;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        switch(ownership){
        case QtJambiLink::Ownership::Java:
            qtJambiLink->setJavaOwnership(env);
            break;
        case QtJambiLink::Ownership::Cpp:
            qtJambiLink->setCppOwnership(env);
            break;
        default:
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            break;
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   const QMetaType& metaType,
                                                                   SafeBool cppOwnership,
                                                                   TypeInfoSupplier typeInfoSupplier)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(metaType.isValid());
    Q_ASSERT(typeInfoSupplier);
    if(enabledDanglingPointerCheck() && checkedGetTypeInfo(typeInfoSupplier, ptr)==nullptr){
        QString className = QtJambiAPI::getObjectClassName(env, javaObject).replace(QLatin1Char('/'), QLatin1Char('.')).replace(QLatin1Char('$'), QLatin1Char('.'));
        Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Unable to convert dangling pointer %p to object of type %s", ptr, qPrintable(className)) QTJAMBI_STACKTRACEINFO );
    }

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DanglingPointerCheckingLink<MetaTypedObjectLink>, QtJambiLink::LinkDisposer> qtJambiLink{new DanglingPointerCheckingLink<MetaTypedObjectLink>(env, nativeLink,
                                                                                                                                                    LINK_NAME_ARG(metaType.name())
                                                                                                                                                    ptr, false, false, ocurredException)};
    qtJambiLink->m_meta_type = metaType;
    qtJambiLink->m_typeInfoSupplier = typeInfoSupplier;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if(cppOwnership)
            qtJambiLink->setCppOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                          SafeBool created_by_java, SafeBool is_shell, QtJambiLink::Ownership ownership,
                                                                          const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(metaType.isValid());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<InterfaceLink<MetaTypedObjectLink>, QtJambiLink::LinkDisposer> qtJambiLink{new InterfaceLink<MetaTypedObjectLink>(env, nativeLink,
                                                                                                                                                        LINK_NAME_ARG(metaType.name())
                                                                                                                                                        ptr, created_by_java,
                                                                                                                                                        is_shell, ocurredException)};
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    qtJambiLink->m_meta_type = metaType;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        switch(ownership){
        case QtJambiLink::Ownership::Java:
            qtJambiLink->setJavaOwnership(env);
            break;
        case QtJambiLink::Ownership::Cpp:
            qtJambiLink->setCppOwnership(env);
            break;
        default:
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            break;
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   const QMetaType& metaType,
                                                                   SafeBool cppOwnership,
                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                   TypeInfoSupplier typeInfoSupplier)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(metaType.isValid());
    Q_ASSERT(typeInfoSupplier);
    if(enabledDanglingPointerCheck() && checkedGetTypeInfo(typeInfoSupplier, ptr)==nullptr){
        QString className = QtJambiAPI::getObjectClassName(env, javaObject).replace(QLatin1Char('/'), QLatin1Char('.')).replace(QLatin1Char('$'), QLatin1Char('.'));
        Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Unable to convert dangling pointer %p to object of type %s", ptr, qPrintable(className)) QTJAMBI_STACKTRACEINFO );
    }

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DanglingPointerCheckingLink<InterfaceLink<MetaTypedObjectLink>>, QtJambiLink::LinkDisposer> qtJambiLink{new DanglingPointerCheckingLink<InterfaceLink<MetaTypedObjectLink>>(env, nativeLink,
                                                                                                                                                                                  LINK_NAME_ARG(metaType.name())
                                                                                                                                                                                  ptr, false, false,
                                                                                                                                                                                  ocurredException)};
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    qtJambiLink->m_meta_type = metaType;
    qtJambiLink->m_typeInfoSupplier = typeInfoSupplier;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if(cppOwnership)
            qtJambiLink->setCppOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForDependentObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                      SafeBool created_by_java, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                      const QObject* dependsOn, QtJambiLink::Ownership ownership)
{
    if(Q_UNLIKELY(!ownerFunction)){
        return createLinkForDependentObject(env, javaObject, ptr, metaType,
                                   created_by_java, is_shell, dependsOn, ownership);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(ownerFunction);
    Q_ASSERT(metaType.isValid());
    checkValueOwner(env, ownerFunction, ptr);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DependentLink<OwnedMetaTypedObjectLink>, QtJambiLink::LinkDisposer> qtJambiLink{new DependentLink<OwnedMetaTypedObjectLink>(env, nativeLink,
                                                                                                                                                                  LINK_NAME_ARG(metaType.name())
                                                                                                                                                                  ptr, created_by_java,
                                                                                                                                                                  is_shell, ocurredException)};
    qtJambiLink->m_meta_type = metaType;
    qtJambiLink->m_owner_function = ownerFunction;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        switch(ownership){
        case QtJambiLink::Ownership::Java:
            qtJambiLink->setJavaOwnership(env);
            break;
        case QtJambiLink::Ownership::Cpp:
            qtJambiLink->setCppOwnership(env);
            break;
        default:
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            break;
        }
        QSharedPointer<QtJambiLink> shared = asSharedPointer(std::move(qtJambiLink));
        QtJambiLink::registerDependentObject(dependsOn, shared);
        return shared;
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForDependentObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                      SafeBool created_by_java, SafeBool is_shell, PtrOwnerFunction ownerFunction,
                                                                      const QObject* dependsOn, QtJambiLink::Ownership ownership,
                                                                      const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    if(Q_UNLIKELY(!ownerFunction)){
        return createLinkForDependentObject(env, javaObject, ptr, metaType,
                                   created_by_java, is_shell, dependsOn, ownership);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(ownerFunction);
    Q_ASSERT(metaType.isValid());
    checkValueOwner(env, ownerFunction, ptr);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DependentLink<InterfaceLink<OwnedMetaTypedObjectLink>>, QtJambiLink::LinkDisposer> qtJambiLink{new DependentLink<InterfaceLink<OwnedMetaTypedObjectLink>>(env, nativeLink,
                                                                                                                                                                                                LINK_NAME_ARG(metaType.name())
                                                                                                                                                                                                ptr, created_by_java,
                                                                                                                                                                                                is_shell, ocurredException)};
    qtJambiLink->m_meta_type = metaType;
    qtJambiLink->m_owner_function = ownerFunction;
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        switch(ownership){
        case QtJambiLink::Ownership::Java:
            qtJambiLink->setJavaOwnership(env);
            break;
        case QtJambiLink::Ownership::Cpp:
            qtJambiLink->setCppOwnership(env);
            break;
        default:
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            break;
        }
        QSharedPointer<QtJambiLink> shared = asSharedPointer(std::move(qtJambiLink));
        QtJambiLink::registerDependentObject(dependsOn, shared);
        return shared;
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForDependentObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                             LINK_NAME_ARG(const char* qt_name)
                                                                             SafeBool created_by_java, SafeBool is_shell,
                                                                             PtrDeleterFunction destructor_function,
                                                                             const QObject* dependsOn, QtJambiLink::Ownership ownership)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(destructor_function);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DependentLink<DeletableObjectLink>, QtJambiLink::LinkDisposer> qtJambiLink{new DependentLink<DeletableObjectLink>(env, nativeLink,
                                                                                                                                            LINK_NAME_ARG(qt_name)
                                                                                                                                            ptr, created_by_java, is_shell, ocurredException)};
    qtJambiLink->m_deleter_function = destructor_function;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        switch(ownership){
        case QtJambiLink::Ownership::Java:
            qtJambiLink->setJavaOwnership(env);
            break;
        case QtJambiLink::Ownership::Cpp:
            qtJambiLink->setCppOwnership(env);
            break;
        default:
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            break;
        }
        QSharedPointer<QtJambiLink> shared = asSharedPointer(std::move(qtJambiLink));
        QtJambiLink::registerDependentObject(dependsOn, shared);
        return shared;
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createExtendedLinkForObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                      LINK_NAME_ARG(const char* qt_name)
                                                                      SafeBool created_by_java, SafeBool is_shell,
                                                                      PtrDeleterFunction destructor_function,
                                                                      const QObject* extension, QtJambiLink::Ownership ownership)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(destructor_function);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<ExtendedLink<DeletableObjectLink>, QtJambiLink::LinkDisposer> qtJambiLink{new ExtendedLink<DeletableObjectLink>(env, nativeLink,
                                                                                                                                          LINK_NAME_ARG(qt_name)
                                                                                                                                          ptr, created_by_java, is_shell, ocurredException)};
    qtJambiLink->m_deleter_function = destructor_function;
    qtJambiLink->m_extension = extension;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        switch(ownership){
        case QtJambiLink::Ownership::Java:
            qtJambiLink->setJavaOwnership(env);
            break;
        case QtJambiLink::Ownership::Cpp:
            qtJambiLink->setCppOwnership(env);
            break;
        default:
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            break;
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForDependentObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                        SafeBool created_by_java, SafeBool is_shell, const QObject* dependsOn, QtJambiLink::Ownership ownership)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(metaType.isValid());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DependentLink<MetaTypedObjectLink>, QtJambiLink::LinkDisposer> qtJambiLink{new DependentLink<MetaTypedObjectLink>(env, nativeLink,
                                                                                                                                                        LINK_NAME_ARG(metaType.name())
                                                                                                                                                        ptr, created_by_java,
                                                                                                                                                        is_shell, ocurredException)};
    qtJambiLink->m_meta_type = metaType;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        switch(ownership){
        case QtJambiLink::Ownership::Java:
            qtJambiLink->setJavaOwnership(env);
            break;
        case QtJambiLink::Ownership::Cpp:
            qtJambiLink->setCppOwnership(env);
            break;
        default:
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            break;
        }
        QSharedPointer<QtJambiLink> shared = asSharedPointer(std::move(qtJambiLink));
        QtJambiLink::registerDependentObject(dependsOn, shared);
        return shared;
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForDependentObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                      const QMetaType& metaType,
                                                                      SafeBool created_by_java, SafeBool is_shell,
                                                                      const QObject* dependsOn,
                                                                      QtJambiLink::Ownership ownership,
                                                                      const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(metaType.isValid());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DependentLink<InterfaceLink<MetaTypedObjectLink>>, QtJambiLink::LinkDisposer> qtJambiLink{new DependentLink<InterfaceLink<MetaTypedObjectLink>>(env, nativeLink,
                                                                                                                                                                                      LINK_NAME_ARG(metaType.name())
                                                                                                                                                                                      ptr, created_by_java,
                                                                                                                                                                                      is_shell, ocurredException)};
    qtJambiLink->m_meta_type = metaType;
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        switch(ownership){
        case QtJambiLink::Ownership::Java:
            qtJambiLink->setJavaOwnership(env);
            break;
        case QtJambiLink::Ownership::Cpp:
            qtJambiLink->setCppOwnership(env);
            break;
        default:
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            break;
        }
        QSharedPointer<QtJambiLink> shared = asSharedPointer(std::move(qtJambiLink));
        QtJambiLink::registerDependentObject(dependsOn, shared);
        return shared;
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                   SafeBool created_by_java, SafeBool is_shell,
                                                                   PtrDeleterFunction deleter_function,
                                                                   PtrOwnerFunction ownerFunction,
                                                                   QtJambiLink::Ownership ownership)
{
    if(Q_UNLIKELY(!ownerFunction)){
        return createLinkForNativeObject(env, javaObject, ptr,
                                   LINK_NAME_ARG(qt_name)
                                   created_by_java, is_shell, deleter_function, ownership);
    }
    if(Q_UNLIKELY(!deleter_function)){
        return createLinkForNativeObject(env, javaObject, ptr,
                                   LINK_NAME_ARG(qt_name)
                                   created_by_java, is_shell, ownership);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(ownerFunction);
    Q_ASSERT(deleter_function);
    checkValueOwner(env, ownerFunction, ptr);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DeletableOwnedObjectLink, QtJambiLink::LinkDisposer> qtJambiLink{new DeletableOwnedObjectLink(env, nativeLink,
                                                                                                                                    LINK_NAME_ARG(qt_name)
                                                                                                                                    ptr, created_by_java,
                                                                                                                                    is_shell,
                                                                                                                                    ocurredException)};
    qtJambiLink->m_deleter_function = deleter_function;
    qtJambiLink->m_owner_function = ownerFunction;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        switch(ownership){
        case QtJambiLink::Ownership::Java:
            qtJambiLink->setJavaOwnership(env);
            break;
        case QtJambiLink::Ownership::Cpp:
            qtJambiLink->setCppOwnership(env);
            break;
        default:
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            break;
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                   PtrDeleterFunction deleter_function,
                                                                   PtrOwnerFunction ownerFunction,
                                                                   SafeBool cppOwnership,
                                                                   TypeInfoSupplier typeInfoSupplier)
{
    if(Q_UNLIKELY(!ownerFunction)){
        return createLinkForNativeObject(env, javaObject, ptr,
                                   LINK_NAME_ARG(qt_name)
                                   deleter_function, cppOwnership, typeInfoSupplier);
    }
    if(Q_UNLIKELY(!deleter_function)){
        return createLinkForNativeObject(env, javaObject, ptr,
                                   LINK_NAME_ARG(qt_name)
                                   cppOwnership, typeInfoSupplier);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(ownerFunction);
    Q_ASSERT(deleter_function);
    Q_ASSERT(typeInfoSupplier);
    if(enabledDanglingPointerCheck() && checkedGetTypeInfo(typeInfoSupplier, ptr)==nullptr){
        QString className = QtJambiAPI::getObjectClassName(env, javaObject).replace(QLatin1Char('/'), QLatin1Char('.')).replace(QLatin1Char('$'), QLatin1Char('.'));
        Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Unable to convert dangling pointer %p to object of type %s", ptr, qPrintable(className)) QTJAMBI_STACKTRACEINFO );
    }
    checkValueOwner(env, ownerFunction, ptr);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DanglingPointerCheckingLink<DeletableOwnedObjectLink>, QtJambiLink::LinkDisposer> qtJambiLink{new DanglingPointerCheckingLink<DeletableOwnedObjectLink>(env, nativeLink,
                                                                                                                                                              LINK_NAME_ARG(qt_name)
                                                                                                                                                              ptr, false,
                                                                                                                                                              false, ocurredException)};
    qtJambiLink->m_deleter_function = deleter_function;
    qtJambiLink->m_owner_function = ownerFunction;
    qtJambiLink->m_typeInfoSupplier = typeInfoSupplier;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if(cppOwnership)
            qtJambiLink->setCppOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                   SafeBool created_by_java, SafeBool is_shell,
                                                                   PtrDeleterFunction deleter_function,
                                                                   PtrOwnerFunction ownerFunction,
                                                                   QtJambiLink::Ownership ownership,
                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    if(Q_UNLIKELY(!ownerFunction)){
        return createLinkForNativeObject(env, javaObject, ptr,
                                   LINK_NAME_ARG(qt_name)
                                   created_by_java, is_shell, deleter_function, ownership,
                                   interfaceOffsetInfo);
    }
    if(Q_UNLIKELY(!deleter_function)){
        return createLinkForNativeObject(env, javaObject, ptr,
                                   LINK_NAME_ARG(qt_name)
                                   created_by_java, is_shell, ownership,
                                   interfaceOffsetInfo);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(ownerFunction);
    Q_ASSERT(deleter_function);
    checkValueOwner(env, ownerFunction, ptr);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<InterfaceLink<DeletableOwnedObjectLink>, QtJambiLink::LinkDisposer> qtJambiLink{new InterfaceLink<DeletableOwnedObjectLink>(env, nativeLink,
                                                                                                                                                      LINK_NAME_ARG(qt_name)
                                                                                                                                                      ptr, created_by_java,
                                                                                                                                                      is_shell, ocurredException)};
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    qtJambiLink->m_deleter_function = deleter_function;
    qtJambiLink->m_owner_function = ownerFunction;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        switch(ownership){
        case QtJambiLink::Ownership::Java:
            qtJambiLink->setJavaOwnership(env);
            break;
        case QtJambiLink::Ownership::Cpp:
            qtJambiLink->setCppOwnership(env);
            break;
        default:
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            break;
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                   PtrDeleterFunction deleter_function,
                                                                   PtrOwnerFunction ownerFunction,
                                                                   SafeBool cppOwnership,
                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                   TypeInfoSupplier typeInfoSupplier)
{
    if(Q_UNLIKELY(!ownerFunction)){
        return createLinkForNativeObject(env, javaObject, ptr,
                                   LINK_NAME_ARG(qt_name)
                                   deleter_function, cppOwnership,
                                   interfaceOffsetInfo, typeInfoSupplier);
    }
    if(Q_UNLIKELY(!deleter_function)){
        return createLinkForNativeObject(env, javaObject, ptr,
                                   LINK_NAME_ARG(qt_name)
                                   cppOwnership,
                                   interfaceOffsetInfo, typeInfoSupplier);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(ownerFunction);
    Q_ASSERT(deleter_function);
    Q_ASSERT(typeInfoSupplier);
    if(enabledDanglingPointerCheck() && checkedGetTypeInfo(typeInfoSupplier, ptr)==nullptr){
        QString className = QtJambiAPI::getObjectClassName(env, javaObject).replace(QLatin1Char('/'), QLatin1Char('.')).replace(QLatin1Char('$'), QLatin1Char('.'));
        Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Unable to convert dangling pointer %p to object of type %s", ptr, qPrintable(className)) QTJAMBI_STACKTRACEINFO );
    }
    checkValueOwner(env, ownerFunction, ptr);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DanglingPointerCheckingLink<InterfaceLink<DeletableOwnedObjectLink>>, QtJambiLink::LinkDisposer> qtJambiLink{new DanglingPointerCheckingLink<InterfaceLink<DeletableOwnedObjectLink>>(env, nativeLink,
                                                                                                                                                                                LINK_NAME_ARG(qt_name)
                                                                                                                                                                                ptr, false,
                                                                                                                                                                                false, ocurredException)};
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    qtJambiLink->m_deleter_function = deleter_function;
    qtJambiLink->m_owner_function = ownerFunction;
    qtJambiLink->m_typeInfoSupplier = typeInfoSupplier;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if(cppOwnership)
            qtJambiLink->setCppOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                   SafeBool created_by_java, SafeBool is_shell,
                                                                   PtrDeleterFunction deleter_function,
                                                                   QtJambiLink::Ownership ownership)
{
    if(Q_UNLIKELY(!deleter_function)){
        return createLinkForNativeObject(env, javaObject, ptr,
                                   LINK_NAME_ARG(qt_name)
                                   created_by_java, is_shell, ownership);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(deleter_function);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DeletableObjectLink, QtJambiLink::LinkDisposer> qtJambiLink{new DeletableObjectLink(env, nativeLink,
                                                                                                                          LINK_NAME_ARG(qt_name)
                                                                                                                          ptr, created_by_java,
                                                                                                                          is_shell, ocurredException)};
    qtJambiLink->m_deleter_function = deleter_function;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        switch(ownership){
        case QtJambiLink::Ownership::Java:
            qtJambiLink->setJavaOwnership(env);
            break;
        case QtJambiLink::Ownership::Cpp:
            qtJambiLink->setCppOwnership(env);
            break;
        default:
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            break;
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                   PtrDeleterFunction deleter_function,
                                                                   SafeBool cppOwnership,
                                                                   TypeInfoSupplier typeInfoSupplier)
{
    if(Q_UNLIKELY(!deleter_function)){
        return createLinkForNativeObject(env, javaObject, ptr,
                                   LINK_NAME_ARG(qt_name)
                                   cppOwnership, typeInfoSupplier);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(deleter_function);
    Q_ASSERT(typeInfoSupplier);
    if(enabledDanglingPointerCheck() && checkedGetTypeInfo(typeInfoSupplier, ptr)==nullptr){
        QString className = QtJambiAPI::getObjectClassName(env, javaObject).replace(QLatin1Char('/'), QLatin1Char('.')).replace(QLatin1Char('$'), QLatin1Char('.'));
        Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Unable to convert dangling pointer %p to object of type %s", ptr, qPrintable(className)) QTJAMBI_STACKTRACEINFO );
    }

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DanglingPointerCheckingLink<DeletableObjectLink>, QtJambiLink::LinkDisposer> qtJambiLink{new DanglingPointerCheckingLink<DeletableObjectLink>(env, nativeLink,
                                                                                                                                                    LINK_NAME_ARG(qt_name)
                                                                                                                                                    ptr, false,
                                                                                                                                                    false,
                                                                                                                                                    ocurredException)};
    qtJambiLink->m_typeInfoSupplier = typeInfoSupplier;
    qtJambiLink->m_deleter_function = deleter_function;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if(cppOwnership)
            qtJambiLink->setCppOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                   SafeBool created_by_java, SafeBool is_shell,
                                                                   PtrDeleterFunction deleter_function,
                                                                   QtJambiLink::Ownership ownership,
                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    if(Q_UNLIKELY(!deleter_function)){
        return createLinkForNativeObject(env, javaObject, ptr,
                                         LINK_NAME_ARG(qt_name)
                                         created_by_java, is_shell, ownership,
                                         interfaceOffsetInfo);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(deleter_function);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<InterfaceLink<DeletableObjectLink>, QtJambiLink::LinkDisposer> qtJambiLink{new InterfaceLink<DeletableObjectLink>(env, nativeLink,
                                                                                                                                            LINK_NAME_ARG(qt_name)
                                                                                                                                            ptr, created_by_java,
                                                                                                                                            is_shell, ocurredException)};
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    qtJambiLink->m_deleter_function = deleter_function;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        switch(ownership){
        case QtJambiLink::Ownership::Java:
            qtJambiLink->setJavaOwnership(env);
            break;
        case QtJambiLink::Ownership::Cpp:
            qtJambiLink->setCppOwnership(env);
            break;
        default:
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            break;
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                   PtrDeleterFunction deleter_function,
                                                                   SafeBool cppOwnership,
                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                   TypeInfoSupplier typeInfoSupplier)
{
    if(Q_UNLIKELY(!deleter_function)){
        return createLinkForNativeObject(env, javaObject, ptr,
                                         LINK_NAME_ARG(qt_name)
                                         cppOwnership,
                                         interfaceOffsetInfo, typeInfoSupplier);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(deleter_function);
    Q_ASSERT(typeInfoSupplier);
    if(enabledDanglingPointerCheck() && checkedGetTypeInfo(typeInfoSupplier, ptr)==nullptr){
        QString className = QtJambiAPI::getObjectClassName(env, javaObject).replace(QLatin1Char('/'), QLatin1Char('.')).replace(QLatin1Char('$'), QLatin1Char('.'));
        Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Unable to convert dangling pointer %p to object of type %s", ptr, qPrintable(className)) QTJAMBI_STACKTRACEINFO );
    }

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DanglingPointerCheckingLink<InterfaceLink<DeletableObjectLink>>, QtJambiLink::LinkDisposer> qtJambiLink{
        new DanglingPointerCheckingLink<InterfaceLink<DeletableObjectLink>>(env, nativeLink,
                                                                    LINK_NAME_ARG(qt_name)
                                                                    ptr, false,
                                                                    false,
                                                                    ocurredException)};
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    qtJambiLink->m_typeInfoSupplier = typeInfoSupplier;
    qtJambiLink->m_deleter_function = deleter_function;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if(cppOwnership)
            qtJambiLink->setCppOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                   SafeBool created_by_java, SafeBool is_shell,
                                                                   QtJambiLink::Ownership ownership)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<PlainObjectLink, QtJambiLink::LinkDisposer> qtJambiLink{new PlainObjectLink(env, nativeLink,
                                                                                                        LINK_NAME_ARG(qt_name)
                                                                                                        ptr, created_by_java, is_shell, ocurredException)};
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        switch(ownership){
        case QtJambiLink::Ownership::Java:
            qtJambiLink->setJavaOwnership(env);
            break;
        case QtJambiLink::Ownership::Cpp:
            qtJambiLink->setCppOwnership(env);
            break;
        default:
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            break;
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                   SafeBool cppOwnership,
                                                                   TypeInfoSupplier typeInfoSupplier)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(typeInfoSupplier);
    if(enabledDanglingPointerCheck() && checkedGetTypeInfo(typeInfoSupplier, ptr)==nullptr){
        QString className = QtJambiAPI::getObjectClassName(env, javaObject).replace(QLatin1Char('/'), QLatin1Char('.')).replace(QLatin1Char('$'), QLatin1Char('.'));
        Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Unable to convert dangling pointer %p to object of type %s", ptr, qPrintable(className)) QTJAMBI_STACKTRACEINFO );
    }

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DanglingPointerCheckingLink<PlainObjectLink>, QtJambiLink::LinkDisposer> qtJambiLink{new DanglingPointerCheckingLink<PlainObjectLink>(env, nativeLink,
                                                                                                                                  LINK_NAME_ARG(qt_name)
                                                                                                                                  ptr, false, false, ocurredException)};
    qtJambiLink->m_typeInfoSupplier = typeInfoSupplier;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if(cppOwnership)
            qtJambiLink->setCppOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                   SafeBool created_by_java,
                                                                   SafeBool is_shell,
                                                                   QtJambiLink::Ownership ownership,
                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<InterfaceLink<PlainObjectLink>, QtJambiLink::LinkDisposer> qtJambiLink{new InterfaceLink<PlainObjectLink>(env, nativeLink,
                                                                                                                          LINK_NAME_ARG(qt_name)
                                                                                                                          ptr, created_by_java, is_shell, ocurredException)};
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        switch(ownership){
        case QtJambiLink::Ownership::Java:
            qtJambiLink->setJavaOwnership(env);
            break;
        case QtJambiLink::Ownership::Cpp:
            qtJambiLink->setCppOwnership(env);
            break;
        default:
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            break;
        }
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                   SafeBool cppOwnership,
                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                   TypeInfoSupplier typeInfoSupplier)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(typeInfoSupplier);
    if(enabledDanglingPointerCheck() && checkedGetTypeInfo(typeInfoSupplier, ptr)==nullptr){
        QString className = QtJambiAPI::getObjectClassName(env, javaObject).replace(QLatin1Char('/'), QLatin1Char('.')).replace(QLatin1Char('$'), QLatin1Char('.'));
        Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Unable to convert dangling pointer %p to object of type %s", ptr, qPrintable(className)) QTJAMBI_STACKTRACEINFO );
    }

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    std::unique_ptr<DanglingPointerCheckingLink<InterfaceLink<PlainObjectLink>>, QtJambiLink::LinkDisposer> qtJambiLink{new DanglingPointerCheckingLink<InterfaceLink<PlainObjectLink>>(env, nativeLink,
                                                                                                                                                    LINK_NAME_ARG(qt_name)
                                                                                                                                                    ptr, false, false, ocurredException)};
    qtJambiLink->setInterfaceOffsetInfo(interfaceOffsetInfo);
    qtJambiLink->m_typeInfoSupplier = typeInfoSupplier;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if(cppOwnership)
            qtJambiLink->setCppOwnership(env);
        return asSharedPointer(std::move(qtJambiLink));
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, const QMetaType& metaType, SafeBool created_by_java, SafeBool is_shell, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
{
    if(Q_UNLIKELY(!ownerFunction)){
        return createLinkForNewObject(env, objectClass, nativeLink, javaObject, typeId, ptr, superTypeInfos, metaType, created_by_java, is_shell, ocurredException);
    }
    if(!ocurredException){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);
        Q_ASSERT(ownerFunction);
        Q_ASSERT(metaType.isValid());

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        std::unique_ptr<OwnedMetaTypedObjectLink, QtJambiLink::LinkDisposer> qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            PendingLink<InterfaceLink<OwnedMetaTypedObjectLink>>* link;
            qtJambiLink.reset(link = new PendingLink<InterfaceLink<OwnedMetaTypedObjectLink>>(env, nativeLink,
                                                                                                       LINK_NAME_ARG(metaType.name())
                                                                                                       ptr, created_by_java, is_shell, ocurredException));
            link->setInterfaceOffsetInfo(*interfaceOffsets);
        }else{
            qtJambiLink.reset(new PendingLink<OwnedMetaTypedObjectLink>(env, nativeLink,
                                                                                 LINK_NAME_ARG(metaType.name())
                                                                                 ptr, created_by_java, is_shell, ocurredException));
        }
        qtJambiLink->m_meta_type = metaType;
        qtJambiLink->m_owner_function = ownerFunction;
        if(Q_LIKELY(!ocurredException)){
            if(created_by_java)
                qtJambiLink->setJavaOwnership(env);
            return asSharedPointer(std::move(qtJambiLink));
        }
    }
    return {};
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos,
                                                                          SafeBool created_by_java, SafeBool is_shell, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
{
    if(!ocurredException){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);
        Q_ASSERT(containerAccess);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        std::unique_ptr<PointerToContainerLink, QtJambiLink::LinkDisposer> qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            InterfaceLink<PointerToContainerLink>* link;
            qtJambiLink.reset(link = new InterfaceLink<PointerToContainerLink>(env, nativeLink,
                                                              LINK_NAME_ARG(getQtName(typeId))
                                                              ptr, created_by_java, is_shell, ocurredException));
            link->setInterfaceOffsetInfo(*interfaceOffsets);
            link->m_containerAccess = containerAccess;
        }else{
            PointerToContainerLink* link;
            qtJambiLink.reset(link = new PointerToContainerLink(env, nativeLink,
                                                     LINK_NAME_ARG(getQtName(typeId))
                                                     ptr, created_by_java, is_shell, ocurredException));
            link->m_containerAccess = containerAccess;
        }
        if(Q_LIKELY(!ocurredException)){
            if(created_by_java)
                qtJambiLink->setJavaOwnership(env);
            return asSharedPointer(std::move(qtJambiLink));
        }
    }
    return {};
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, SafeBool created_by_java, SafeBool is_shell, AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, JavaException& ocurredException)
{
    if(!ocurredException){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);
        Q_ASSERT(containerAccess);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        std::unique_ptr<DeletablePointerToContainerLink, QtJambiLink::LinkDisposer> qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            InterfaceLink<DeletablePointerToContainerLink>* link;
            qtJambiLink.reset(link = new InterfaceLink<DeletablePointerToContainerLink>(env, nativeLink,
                                                                           LINK_NAME_ARG(getQtName(typeId))
                                                                           ptr, created_by_java, is_shell, ocurredException));
            link->setInterfaceOffsetInfo(*interfaceOffsets);
        }else{
            qtJambiLink.reset(new DeletablePointerToContainerLink(env, nativeLink,
                                                                  LINK_NAME_ARG(getQtName(typeId))
                                                                  ptr, created_by_java, is_shell, ocurredException));
        }
        qtJambiLink->m_deleter_function = destructor_function;
        qtJambiLink->m_containerAccess = containerAccess;
        if(Q_LIKELY(!ocurredException)){
            if(created_by_java)
                qtJambiLink->setJavaOwnership(env);
            return asSharedPointer(std::move(qtJambiLink));
        }
    }
    return {};
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, SafeBool created_by_java, SafeBool is_shell, AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
{
    if(!ocurredException){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);
        Q_ASSERT(containerAccess);
        Q_ASSERT(ownerFunction);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        std::unique_ptr<DeletableOwnedPointerToContainerLink, QtJambiLink::LinkDisposer> qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            PendingLink<InterfaceLink<DeletableOwnedPointerToContainerLink>>* link;
            qtJambiLink.reset(link = new PendingLink<InterfaceLink<DeletableOwnedPointerToContainerLink>>(env, nativeLink,
                                                                                LINK_NAME_ARG(getQtName(typeId))
                                                                                ptr, created_by_java, is_shell, ocurredException));
            link->setInterfaceOffsetInfo(*interfaceOffsets);
        }else{
            qtJambiLink.reset(new PendingLink<DeletableOwnedPointerToContainerLink>(env, nativeLink,
                                                                       LINK_NAME_ARG(getQtName(typeId))
                                                                       ptr, created_by_java, is_shell, ocurredException));
        }
        qtJambiLink->m_owner_function = ownerFunction;
        qtJambiLink->m_deleter_function = destructor_function;
        qtJambiLink->m_containerAccess = containerAccess;
        if(Q_LIKELY(!ocurredException)){
            if(created_by_java)
                qtJambiLink->setJavaOwnership(env);
            return asSharedPointer(std::move(qtJambiLink));
        }
    }
    return {};
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, const QMetaType& metaType, SafeBool created_by_java, SafeBool is_shell, JavaException& ocurredException)
{
    if(!ocurredException){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);
        Q_ASSERT(metaType.isValid());

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        std::unique_ptr<QtJambiLink, QtJambiLink::LinkDisposer> qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            InterfaceLink<MetaTypedObjectLink>* link;
            qtJambiLink.reset(link = new InterfaceLink<MetaTypedObjectLink>(env, nativeLink,
                                                                                     LINK_NAME_ARG(metaType.name())
                                                                                     ptr, created_by_java,
                                                                                     is_shell, ocurredException));
            link->setInterfaceOffsetInfo(*interfaceOffsets);
            link->m_meta_type = metaType;
        }else{
            MetaTypedObjectLink* link;
            qtJambiLink.reset(link = new MetaTypedObjectLink(env, nativeLink,
                                                                      LINK_NAME_ARG(metaType.name())
                                                                      ptr, created_by_java,
                                                                      is_shell, ocurredException));
            link->m_meta_type = metaType;
        }
        if(Q_LIKELY(!ocurredException)){
            if(created_by_java)
                qtJambiLink->setJavaOwnership(env);
            return asSharedPointer(std::move(qtJambiLink));
        }
    }
    return {};
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, SafeBool created_by_java, SafeBool is_shell, PtrDeleterFunction deleter_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
{
    if(Q_UNLIKELY(!ownerFunction)){
        return createLinkForNewObject(env, objectClass, nativeLink, javaObject, typeId, ptr, superTypeInfos, created_by_java, is_shell, deleter_function, ocurredException);
    }
    if(!ocurredException){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);
        Q_ASSERT(ownerFunction);
        Q_ASSERT(deleter_function);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        std::unique_ptr<DeletableOwnedObjectLink, QtJambiLink::LinkDisposer> qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            PendingLink<InterfaceLink<DeletableOwnedObjectLink>>* link;
            qtJambiLink.reset(link = new PendingLink<InterfaceLink<DeletableOwnedObjectLink>>(env, nativeLink,
                                                                             LINK_NAME_ARG(getQtName(typeId))
                                                                             ptr, created_by_java, is_shell, ocurredException));
            link->setInterfaceOffsetInfo(*interfaceOffsets);
        }else{
            qtJambiLink.reset(new PendingLink<DeletableOwnedObjectLink>(env, nativeLink,
                                                                    LINK_NAME_ARG(getQtName(typeId))
                                                                    ptr, created_by_java, is_shell, ocurredException));
        }
        qtJambiLink->m_deleter_function = deleter_function;
        qtJambiLink->m_owner_function = ownerFunction;
        if(Q_LIKELY(!ocurredException)){
            if(created_by_java)
                qtJambiLink->setJavaOwnership(env);
            return asSharedPointer(std::move(qtJambiLink));
        }
    }
    return {};
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, SafeBool created_by_java, SafeBool is_shell, PtrDeleterFunction deleter_function, JavaException& ocurredException)
{
    if(Q_UNLIKELY(!deleter_function)){
        return createLinkForNewObject(env, objectClass, nativeLink, javaObject, typeId, ptr, superTypeInfos, created_by_java, is_shell, ocurredException);
    }
    if(!ocurredException){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);
        Q_ASSERT(deleter_function);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        std::unique_ptr<DeletableObjectLink, QtJambiLink::LinkDisposer> qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            InterfaceLink<DeletableObjectLink>* link;
            qtJambiLink.reset(link = new InterfaceLink<DeletableObjectLink>(env, nativeLink,
                                                                        LINK_NAME_ARG(getQtName(typeId))
                                                                        ptr, created_by_java, is_shell, ocurredException));
            link->setInterfaceOffsetInfo(*interfaceOffsets);
        }else{
            qtJambiLink.reset(new DeletableObjectLink(env, nativeLink,
                                                               LINK_NAME_ARG(getQtName(typeId))
                                                               ptr, created_by_java, is_shell, ocurredException));
        }
        qtJambiLink->m_deleter_function = deleter_function;
        if(Q_LIKELY(!ocurredException)){
            if(created_by_java)
                qtJambiLink->setJavaOwnership(env);
            return asSharedPointer(std::move(qtJambiLink));
        }
    }
    return {};
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, SafeBool created_by_java, SafeBool is_shell, JavaException& ocurredException)
{
    if(!ocurredException){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        std::unique_ptr<PlainObjectLink, QtJambiLink::LinkDisposer> qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            InterfaceLink<PlainObjectLink>* link;
            qtJambiLink.reset(link = new InterfaceLink<PlainObjectLink>(env, nativeLink,
                                                               LINK_NAME_ARG(getQtName(typeId))
                                                               ptr, created_by_java, is_shell, ocurredException));
            link->setInterfaceOffsetInfo(*interfaceOffsets);
        }else{
            qtJambiLink.reset(new PlainObjectLink(env, nativeLink,
                                                      LINK_NAME_ARG(getQtName(typeId))
                                                      ptr, created_by_java, is_shell, ocurredException));
        }
        if(Q_LIKELY(!ocurredException)){
            if(created_by_java)
                qtJambiLink->setJavaOwnership(env);
            return asSharedPointer(std::move(qtJambiLink));
        }
    }
    return {};
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForOwnedObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                             LINK_NAME_ARG(const char* qt_name)
                                                                             QtJambiNativeID owner, PtrDeleterFunction deleter_function)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(javaObject));
    JavaException ocurredException;
    std::unique_ptr<PlainObjectLink, QtJambiLink::LinkDisposer> qtJambiLink;
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        if(deleter_function){
            InterfaceLink<DeletableObjectLink>* link;
            qtJambiLink.reset(link = new InterfaceLink<DeletableObjectLink>(env, nativeLink,
                                                                        LINK_NAME_ARG(qt_name)
                                                                        ptr, false, false, ocurredException));
            link->setInterfaceOffsetInfo(*interfaceOffsets);
            link->m_deleter_function = deleter_function;
        }else{
            InterfaceLink<PlainObjectLink>* link;
            qtJambiLink.reset(link = new InterfaceLink<PlainObjectLink>(env, nativeLink,
                                                               LINK_NAME_ARG(qt_name)
                                                               ptr, false, false, ocurredException));
            link->setInterfaceOffsetInfo(*interfaceOffsets);
        }
    }else{
        if(deleter_function){
            DeletableObjectLink* link;
            qtJambiLink.reset(link = new DeletableObjectLink(env, nativeLink,
                                                               LINK_NAME_ARG(qt_name)
                                                               ptr, false, false, ocurredException));
            link->m_deleter_function = deleter_function;
        }else{
            qtJambiLink.reset(new PlainObjectLink(env, nativeLink,
                                                      LINK_NAME_ARG(qt_name)
                                                      ptr, false, false, ocurredException));
        }
    }
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        QSharedPointer<QtJambiLink> shared = asSharedPointer(std::move(qtJambiLink));
        if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
            _owner->registerDependentObject(shared);
        }
        if(deleter_function){
            shared->setJavaOwnership(env);
        }else{
            shared->setSplitOwnership(env);
        }
        return shared;
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForOwnedObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                             LINK_NAME_ARG(const char* qt_name)
                                                                             QtJambiNativeID owner)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(javaObject));
    JavaException ocurredException;
    std::unique_ptr<PlainObjectLink, QtJambiLink::LinkDisposer> qtJambiLink;
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        InterfaceLink<PlainObjectLink>* link;
        qtJambiLink.reset(link = new InterfaceLink<PlainObjectLink>(env, nativeLink,
                                                           LINK_NAME_ARG(qt_name)
                                                           ptr, false, false, ocurredException));
        link->setInterfaceOffsetInfo(*interfaceOffsets);
    }else{
        qtJambiLink.reset(new PlainObjectLink(env, nativeLink,
                                                  LINK_NAME_ARG(qt_name)
                                                  ptr, false, false, ocurredException));
    }
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        qtJambiLink->setSplitOwnership(env);
        QSharedPointer<QtJambiLink> shared = asSharedPointer(std::move(qtJambiLink));
        if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
            _owner->registerDependentObject(shared);
        }
        return shared;
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                             LINK_NAME_ARG(const char* qt_name)
                                                                             QtJambiNativeID owner, AbstractContainerAccess* containerAccess)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(javaObject));
    JavaException ocurredException;
    std::unique_ptr<PointerToContainerLink, QtJambiLink::LinkDisposer> qtJambiLink;
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        InterfaceLink<PointerToContainerLink>* link;
        qtJambiLink.reset(link = new InterfaceLink<PointerToContainerLink>(env, nativeLink,
                                                              LINK_NAME_ARG(qt_name)
                                                              ptr, false, false, ocurredException));
        link->setInterfaceOffsetInfo(*interfaceOffsets);
        link->m_containerAccess = containerAccess;
    }else{
        PointerToContainerLink* link;
        qtJambiLink.reset(link = new PointerToContainerLink(env, nativeLink,
                                                     LINK_NAME_ARG(qt_name)
                                                     ptr, false, false, ocurredException));
        link->m_containerAccess = containerAccess;
    }
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        qtJambiLink->setSplitOwnership(env);
        QSharedPointer<QtJambiLink> shared = asSharedPointer(std::move(qtJambiLink));
        if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
            _owner->registerDependentObject(shared);
        }
        return shared;
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                             LINK_NAME_ARG(const char* qt_name)
                                                                             QtJambiNativeID owner, PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(javaObject));
    JavaException ocurredException;
    std::unique_ptr<DeletablePointerToContainerLink, QtJambiLink::LinkDisposer> qtJambiLink;
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        InterfaceLink<DeletablePointerToContainerLink>* link;
        qtJambiLink.reset(link = new InterfaceLink<DeletablePointerToContainerLink>(env, nativeLink,
                                                                       LINK_NAME_ARG(qt_name)
                                                                       ptr, false, false, ocurredException));
        link->setInterfaceOffsetInfo(*interfaceOffsets);
    }else{
        qtJambiLink.reset(new DeletablePointerToContainerLink(env, nativeLink,
                                                              LINK_NAME_ARG(qt_name)
                                                              ptr, false, false, ocurredException));
    }
    qtJambiLink->m_deleter_function = destructor_function;
    qtJambiLink->m_containerAccess = containerAccess;
    if(Q_UNLIKELY(ocurredException)){
        ocurredException.raise();
        return {};
    }else{
        qtJambiLink->init(env);
        if(destructor_function){
            qtJambiLink->setJavaOwnership(env);
        }else{
            qtJambiLink->setSplitOwnership(env);
        }
        QSharedPointer<QtJambiLink> shared = asSharedPointer(std::move(qtJambiLink));
        if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
            _owner->registerDependentObject(shared);
        }
        return shared;
    }
}

jobject QtJambiLink::findObjectForPointer(JNIEnv* env, jclass clazz, const void *ptr){
    jobject result = nullptr;
    Q_ASSERT(ptr);
    QtJambiStorage* storage = getQtJambiStorage();
    {
        if(storage->linkLock()->tryLockForRead(2000)){
            const QList<QWeakPointer<QtJambiLink>> values = storage->userObjectCache().values(ptr);
            storage->linkLock()->unlock();
            for(const QWeakPointer<QtJambiLink>& wlink : values){
                if(QSharedPointer<QtJambiLink> link = wlink){
                    jobject obj = link->getJavaObjectLocalRef(env);
                    if(obj && env->IsInstanceOf(obj, clazz)){
                        result = obj;
                        break;
                    }
                }
            }
        }
    }
    return result;
}

QList<QSharedPointer<QtJambiLink>> QtJambiLink::findLinksForPointer(const void *ptr)
{
    QList<QSharedPointer<QtJambiLink>> result;
    if (Q_LIKELY(ptr)){
        QtJambiStorage* storage = getQtJambiStorage();
        {
            if(storage->linkLock()->tryLockForRead(2000)){
                const QList<QWeakPointer<QtJambiLink>> values = storage->userObjectCache().values(ptr);
                storage->linkLock()->unlock();
                for(const QWeakPointer<QtJambiLink>& wlink : values){
                    if(QSharedPointer<QtJambiLink> link = wlink)
                        result << link;
                }
            }
        }
    }
    return result;
}

QSharedPointer<QtJambiLink> QtJambiLink::findLinkForQObject(const QObject *o)
{
    if (!o)
        return {};
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->linkLock());
        if(QtJambiLinkObjectData *p = QtJambiObjectData::userData<QtJambiLinkObjectData>(o)){
            return p->link();
        }
        return storage->qObjectCache().value(o);
    }
    return {};
}

QSharedPointer<QtJambiLink> QtJambiLink::fromNativeId(QtJambiNativeID native_id){
    if(!native_id){
        return {};
    }else{
        return *reinterpret_cast<QtJambiLink*>(native_id);
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::findLinkForJavaInterface(JNIEnv *env, jobject java)
{
    if(env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    QSharedPointer<QtJambiLink> ptr;
    if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java)){
        Java::QtJambi::NativeUtility::findAndAssignInterfaceLink(env, java, true, true, jlong(&ptr));
    }else if(Java::QtCore::SmartPointer::isInstanceOf(env, java)){
        java = Java::QtCore::SmartPointer::get(env, java);
        if (!env->IsSameObject(nullptr, java)){
            if(Java::QtJambi::QtObject::isInstanceOf(env, java))
                Java::QtJambi::NativeUtility$Object::assignNativeLink(env, java, jlong(&ptr));
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java))
                Java::QtJambi::NativeUtility::findAndAssignInterfaceLink(env, java, true, true, jlong(&ptr));
        }
    }else if(Java::Runtime::Reference::isInstanceOf(env, java)){
        java = Java::Runtime::Reference::get(env, java);
        if (!env->IsSameObject(nullptr, java)){
            if(Java::QtJambi::QtObject::isInstanceOf(env, java))
                Java::QtJambi::NativeUtility$Object::assignNativeLink(env, java, jlong(&ptr));
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java))
                Java::QtJambi::NativeUtility::findAndAssignInterfaceLink(env, java, true, true, jlong(&ptr));
        }
    }
    return ptr;
}

QSharedPointer<QtJambiLink> QtJambiLink::findLinkForJavaObject(JNIEnv *env, jobject java)
{
    if(env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    QSharedPointer<QtJambiLink> ptr;
    if (!env->IsSameObject(nullptr, java))
        Java::QtJambi::NativeUtility$Object::assignNativeLink(env, java, jlong(&ptr));
    return ptr;
}

void* QtJambiLink::findPointerForJavaInterface(JNIEnv *env, jobject java, const std::type_info& typeId)
{
    if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java)){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, java)){
            void* result{nullptr};
            if(link->isMultiInheritanceType()){
                if(link->isInterfaceAvailable(typeId))
                    result = link->typedPointer(typeId);
            }else result = link->pointer();
            if(result)
                return result;
        }
    }else if(env->IsSameObject(nullptr, java))
        return nullptr;
    Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, java).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    return nullptr;
}

void* QtJambiLink::findPointerForJavaObject(JNIEnv *env, jobject java)
{
    if(env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, java)){
        if(void* ptr = link->pointer())
            return ptr;
    }else if (!env->IsSameObject(nullptr, java))
        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, java).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    return nullptr;
}

namespace QtSharedPointer{
ExternalRefCountData* ExternalRefCountWithCustomDeleter<QtJambiLink, NormalDeleter>::create(QtJambiLink * link, QtSharedPointer::NormalDeleter, DestroyerFn){
    link->m_refCount->strongref.ref();
    link->m_refCount->weakref.ref();
    return link->m_refCount;
}

template<>
struct ExternalRefCountWithCustomDeleter<QtJambiLink, ExternalRefCountData*>{
    typedef const void* DestroyerFn;
    static constexpr char safetyCheckDeleter = 0;
    static constexpr char deleter = 0;
    static constexpr inline ExternalRefCountData* create(QtJambiLink *, ExternalRefCountData* result, DestroyerFn){
        return result;
    }
};
}

QSharedPointer<QtJambiLink> QtJambiLink::getStrongPointer() const {
    if(Q_LIKELY(m_refCount->strongref.fetchAndAddAcquire(1)>0)){
        m_refCount->weakref.ref();
        return {const_cast<QtJambiLink*>(this), m_refCount};
    }else{
        m_refCount->strongref.fetchAndAddAcquire(-1);
        return {};
    }
}

bool QtJambiLink::isDeleteLater() const { return m_flags.testFlag(Flag::IsDeleteLater); }
bool QtJambiLink::isCleanedUp() const { return m_flags.testFlag(Flag::HasBeenCleaned); }
void QtJambiLink::setDeleteLater(){ m_flags.setFlag(Flag::IsDeleteLater); }

bool QtJambiLink::isQThread() const{ return m_flags.testFlag(Flag::IsQThread); }

void QtJambiLink::setIsQThread(bool n) { m_flags.setFlag(Flag::IsQThread, n); }

bool QtJambiLink::isAdoptedThread() const{ return m_flags.testFlag(Flag::IsAdoptedThread); }

void QtJambiLink::setAdoptedThread(bool n) { m_flags.setFlag(Flag::IsAdoptedThread, n); }

void QtJambiLink::detachJavaLink(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::IsJavaLinkDetached) && env) {
        m_flags.setFlag(Flag::IsJavaLinkDetached);
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this, "QtJambiLink::detachJavaLink(JNIEnv *env)")
        QTJAMBI_INCREASE_COUNTER_THIS(objectInvalidatedCount)
        if(m_flags.testFlag(Flag::NoGlobalRef)){
            Java::QtJambi::NativeUtility$NativeLink::detachForID(env, jlong(reinterpret_cast<void*>(this)), hasDisposedSignal());
        }else{
            if(m_nativeLink)
                Java::QtJambi::NativeUtility$NativeLink::detach(env, m_nativeLink, jlong(reinterpret_cast<void*>(this)), hasDisposedSignal());
        }
    }
}

#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
const char* QtJambiLink::qtTypeName() const{
    return m_qtTypeName;
}
#endif
void QtJambiLink::onDispose(JNIEnv *env){
    if(!m_flags.testFlag(QtJambiLink::Flag::IsManaged)){
        if(!m_flags.testFlag(QtJambiLink::Flag::BlockDeletion)){
            JavaException throwable;
            m_flags.setFlag(QtJambiLink::Flag::BlockDeletion, true);
            QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this, "QtJambiLink::onDispose(JNIEnv *)")
            setJavaOwnership(env);
            deleteNativeObject(env, true);
            if(env->ExceptionCheck()){
                jthrowable t = env->ExceptionOccurred();
                env->ExceptionClear();
                throwable = JavaException(env, t);
            }
            detachJavaLink(env);
            QTJAMBI_INCREASE_COUNTER_THIS(disposeCalledCount)
            m_flags.setFlag(QtJambiLink::Flag::BlockDeletion, false);
            throwable.raise();
        }
    }
}
void QtJambiLink::onClean(JNIEnv *env){
    if(!m_flags.testFlag(QtJambiLink::Flag::HasBeenCleaned)){
        if(!m_flags.testFlag(QtJambiLink::Flag::BlockDeletion)){
            JavaException throwable;
            m_flags.setFlag(QtJambiLink::Flag::BlockDeletion, true);
            QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this, "QtJambiLink::onClean(JNIEnv *)")
            QTJAMBI_INCREASE_COUNTER_THIS(cleanCallerCount)
            releaseJavaObject(env);
            deleteNativeObject(env);
            if(env->ExceptionCheck()){
                jthrowable t = env->ExceptionOccurred();
                env->ExceptionClear();
                throwable = JavaException(env, t);
            }
            detachJavaLink(env);
            m_flags.setFlag(QtJambiLink::Flag::HasBeenCleaned);
            m_flags.setFlag(QtJambiLink::Flag::BlockDeletion, false);
            throwable.raise();
        }
    }
}

jobject QtJambiLink::nativeLink(JNIEnv* env) const {
    if(m_flags.testFlag(Flag::NoGlobalRef)){
        return Java::QtJambi::NativeUtility::getGlobalReference(env, jlong(reinterpret_cast<const void*>(this)));
    }else{
        return m_nativeLink ? env->NewLocalRef(m_nativeLink) : nullptr;
    }
}

void QtJambiLink::setCppOwnership(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::CppOwnership) && env){
        if(m_flags.testFlag(Flag::NoGlobalRef)){
            if(!Java::QtJambi::NativeUtility$NativeLink::takeOwnershipForID(env, jlong(reinterpret_cast<void*>(this))))
                return;
        }else{
            Java::QtJambi::NativeUtility$NativeLink::takeOwnership(env, m_nativeLink);
        }
        m_flags.setFlag(Flag::OwnershipMask, false);
        m_flags.setFlag(Flag::CppOwnership);
    }
}

void QtJambiLink::setDefaultOwnership(JNIEnv *env)
{
    if (createdByJava())
        setJavaOwnership(env);
    else if(!isQObject() || (!qobjectDeleted() && qobject()->parent()==nullptr))
        setSplitOwnership(env);
    else
        setCppOwnership(env);
}

void QtJambiLink::setJavaOwnership(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::JavaOwnership) && env){
        if(m_flags.testFlag(Flag::NoGlobalRef)){
            if(!Java::QtJambi::NativeUtility$NativeLink::releaseOwnershipForID(env, jlong(reinterpret_cast<void*>(this))))
                return;
        }else{
            Java::QtJambi::NativeUtility$NativeLink::releaseOwnership(env, m_nativeLink);
        }
        m_flags.setFlag(Flag::OwnershipMask, false);
        m_flags.setFlag(Flag::JavaOwnership);
        if(!isQObject())
            registerPointer(plainPointer());
    }
}

void QtJambiLink::setSplitOwnership(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::SplitOwnership) && env){
        if(m_flags.testFlag(Flag::NoGlobalRef)){
            if(!Java::QtJambi::NativeUtility$NativeLink::releaseOwnershipForID(env, jlong(reinterpret_cast<void*>(this))))
                return;
        }else{
            Java::QtJambi::NativeUtility$NativeLink::releaseOwnership(env, m_nativeLink);
        }
        m_flags.setFlag(Flag::OwnershipMask, false);
        m_flags.setFlag(Flag::SplitOwnership);
    }
}

void QtJambiLink::reset(JNIEnv *env)
{
    if(m_flags.testFlag(Flag::NoGlobalRef)){
        Java::QtJambi::NativeUtility$NativeLink::resetForID(env, jlong(reinterpret_cast<void*>(this)), hasDisposedSignal());
    }else{
        Java::QtJambi::NativeUtility$NativeLink::reset(env, m_nativeLink, jlong(reinterpret_cast<void*>(this)), hasDisposedSignal());
    }
    m_flags.setFlag(Flag::OwnershipMask, false);
    m_flags.setFlag(Flag::SplitOwnership);
}

bool QtJambiLink::createdByJava() const {
    return m_flags.testFlag(Flag::CreatedByJava);
}

bool QtJambiLink::isShell() const {
    return m_flags.testFlag(Flag::IsShell);
}

bool QtJambiLink::needsReferenceCounting() const {
    return m_flags.testFlag(Flag::NeedsReferenceCounting);
}

void QtJambiLink::setNeedsReferenceCounting(bool b){
    m_flags.setFlag(Flag::NeedsReferenceCounting, b);
}

QtJambiLink::Ownership QtJambiLink::ownership() const {
    return Ownership(int(m_flags & Flag::OwnershipMask));
}

jobject QtJambiLink::getJavaObjectLocalRef(JNIEnv *env) const
{
#ifndef AVAILABLE_IN_DELETION
    if(isInDestructor())
        return nullptr;
#endif
    if (m_flags.testFlag(Flag::JavaObjectIsReleased))
        return nullptr;
    if(m_flags.testFlag(Flag::NoGlobalRef)){
        return Java::QtJambi::NativeUtility$NativeLink::getForID(env, jlong(reinterpret_cast<const void*>(this)));
    }else{
        return Java::QtJambi::NativeUtility$NativeLink::get(env, m_nativeLink);
    }
}

bool QtJambiLink::hasDisposedSignal() const
{
    return m_flags.testFlag(Flag::HasDisposedSignal);
}

void QtJambiLink::setHasDisposedSignal()
{
    m_flags.setFlag(Flag::HasDisposedSignal);
}

void QtJambiLink::setInDestructor()
{
    m_flags.setFlag(Flag::IsInDestructor);
}

bool QtJambiLink::isInDestructor() const
{
    return m_flags.testFlag(Flag::IsInDestructor);
}

QtJambiLink::Flag QtJambiLink::smartPointerType() const
{
    return QtJambiLink::Flag(int(m_flags & Flag::SmartPointerMask));
}

bool QtJambiLink::isInitialized() const
{
    return m_flags.testFlag(Flag::IsInitialized);
}

void QtJambiLink::dispose(){
    if(!m_flags.testFlag(Flag::IsDisposed)){
        m_flags.setFlag(Flag::IsDisposed);
        if (!m_refCount->strongref.deref()) {
            m_refCount->destroy();
        }
    }
}

QString QtJambiLink::describe() const{
    QString strg;
    if(isQObject()){
        if(QObject* qobj = qobject()){
            strg += QString("class=%1, objectName=%2, ")
                        .arg(QLatin1String(qobj->metaObject()->className()), qobj->objectName());
        }
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
        else if(m_qtTypeName){
            strg += QString("class=%1, ").arg(QLatin1String(m_qtTypeName));
        }
#endif
    }else{
        if(const QMetaType* m = metaType()){
            strg += QString("metaType=%1 (%2), ").arg(QLatin1String(m->name())).arg(m->id());
        }
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
        else if(m_qtTypeName){
            strg += QString("metaType=%1, ").arg(QLatin1String(m_qtTypeName));
        }
#endif
    }
    strg += QString("createdByJava=%1, hasBeenCleaned=%2, isJavaLinkDetached=%3, ownership=%4")
            .arg(createdByJava() ? "true" : "false",
                     m_flags.testFlag(QtJambiLink::Flag::HasBeenCleaned) ? "true" : "false",
                     m_flags.testFlag(QtJambiLink::Flag::IsJavaLinkDetached) ? "true" : "false");
    if(m_flags.testFlag(QtJambiLink::Flag::JavaOwnership)) strg = strg.arg("Java");
    else if(m_flags.testFlag(QtJambiLink::Flag::CppOwnership)) strg = strg.arg("Cpp");
    else if(m_flags.testFlag(QtJambiLink::Flag::SplitOwnership)) strg = strg.arg("Split");
    else strg = strg.arg("InvalidOwnership");
    return QString("[%1]").arg(strg);
}

// ### BEGIN #######################  QtJambiLink  ######################### BEGIN ####

void QtJambiLink::LinkDeleter::operator()(QtJambiLink* link){
    delete link;
}

void QtJambiLink::LinkDisposer::operator()(QtJambiLink* link){
    link->dispose();
}

QtJambiLink::QtJambiLink(JNIEnv *env, jobject nativeLink,
                                 LINK_NAME_ARG(const char *qt_name)
                                 SafeBool created_by_java, SafeBool is_shell, JavaException& ocurredException) :
# ifdef QT_SHAREDPOINTER_TRACK_POINTERS
    m_refCount(ExternalRefCountData::create(this, {}, &ExternalRefCountData::safetyCheckDeleter)),
# else
    m_refCount(ExternalRefCountData::create(this, {}, &ExternalRefCountData::deleter)),
# endif
    m_nativeLink(nullptr),
    m_flags(QtJambiLink::Flag::None) // Default to split, because it's safest
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
    ,m_qtTypeName(qt_name)
#endif
{
    m_flags.setFlag(QtJambiLink::Flag::SplitOwnership);
    if(is_shell){
        m_flags.setFlag(QtJambiLink::Flag::IsShell);
    }
    if(created_by_java){
        m_flags.setFlag(QtJambiLink::Flag::CreatedByJava);
    }
    if(avoidJNIGlobalRefs(env))
        m_flags.setFlag(Flag::NoGlobalRef);

    // Set the native__id field of the java object
    // We continue to use the hard local reference to java object in preference to the weak one we just created
    try{
        Java::QtJambi::NativeUtility$NativeLink::initialize(env, nativeLink, jlong(reinterpret_cast<void*>(this)));
    } catch (const JavaException& exn) {
        ocurredException.addSuppressed(env, exn);
        return;
    }
    if(m_flags.testFlag(Flag::NoGlobalRef)){
        try{
            Java::QtJambi::NativeUtility::pushGlobalReference(env, jlong(reinterpret_cast<void*>(this)), nativeLink);
        } catch (const JavaException& exn) {
            ocurredException.addSuppressed(env, exn);
            return;
        }
    }else{
        m_nativeLink = env->NewGlobalRef(nativeLink);
    }
    ON_DEBUG_TOOLS_ENABLED(registerLink(this))
    QTJAMBI_INCREASE_COUNTER_THIS(linkConstructedCount)
}

QtJambiLink::~QtJambiLink(){
    ON_DEBUG_TOOLS_ENABLED(unregisterLink(this))
    QTJAMBI_INCREASE_COUNTER_THIS(linkDestroyedCount)
    if(m_flags.testFlag(Flag::NoGlobalRef) || m_nativeLink){
        if(JniEnvironment env = isQThread() ? JniEnvironment{300} : DefaultJniEnvironment{300}){
            if(m_flags.testFlag(Flag::NoGlobalRef)){
                try{
                    if (!m_flags.testFlag(Flag::JavaObjectIsReleased))
                        Java::QtJambi::NativeUtility$NativeLink::releaseOwnershipForID(env, jlong(reinterpret_cast<void*>(this)));
                    if(!m_flags.testFlag(Flag::IsJavaLinkDetached))
                        Java::QtJambi::NativeUtility$NativeLink::detachForID(env, jlong(reinterpret_cast<void*>(this)), m_flags.testFlag(Flag::HasDisposedSignal));
                    Java::QtJambi::NativeUtility$NativeLink::resetForID(env, jlong(reinterpret_cast<void*>(this)), hasDisposedSignal());
                } catch (const JavaException& exn) {
                    exn.report(env);
                }
                try{
                    Java::QtJambi::NativeUtility::releaseGlobalReference(env, jlong(reinterpret_cast<void*>(this)));
                } catch (const JavaException& exn) {
                    exn.report(env);
                }
            }else{
                try{
                    if (!m_flags.testFlag(Flag::JavaObjectIsReleased))
                        Java::QtJambi::NativeUtility$NativeLink::releaseOwnership(env, m_nativeLink);
                    if(!m_flags.testFlag(Flag::IsJavaLinkDetached))
                        Java::QtJambi::NativeUtility$NativeLink::detach(env, m_nativeLink, jlong(reinterpret_cast<void*>(this)), m_flags.testFlag(Flag::HasDisposedSignal));
                    Java::QtJambi::NativeUtility$NativeLink::reset(env, m_nativeLink, jlong(reinterpret_cast<void*>(this)), hasDisposedSignal());
                } catch (const JavaException& exn) {
                    exn.report(env);
                }
                env->DeleteGlobalRef(m_nativeLink);
                m_nativeLink = nullptr;
            }
        }else{
            if(m_flags.testFlag(Flag::NoGlobalRef)){
                ThreadPrivate::doInPurgeThread([flags = m_flags, id = jlong(reinterpret_cast<void*>(this))](JNIEnv* env){
                    try{
                        if (!flags.testFlag(Flag::JavaObjectIsReleased))
                            Java::QtJambi::NativeUtility$NativeLink::releaseOwnershipForID(env, id);
                        if(!flags.testFlag(Flag::IsJavaLinkDetached))
                            Java::QtJambi::NativeUtility$NativeLink::detachForID(env, id, flags.testFlag(Flag::HasDisposedSignal));
                        Java::QtJambi::NativeUtility$NativeLink::resetForID(env, id, flags.testFlag(Flag::HasDisposedSignal));
                        Java::QtJambi::NativeUtility::releaseGlobalReference(env, id);
                    } catch (const JavaException& exn) {
                        exn.report(env);
                    }
                });
            }else{
                ThreadPrivate::doInPurgeThread([nativeLink = m_nativeLink, flags = m_flags, id = jlong(reinterpret_cast<void*>(this))](JNIEnv* env){
                    try{
                        if (!flags.testFlag(Flag::JavaObjectIsReleased))
                            Java::QtJambi::NativeUtility$NativeLink::releaseOwnership(env, nativeLink);
                        if(!flags.testFlag(Flag::IsJavaLinkDetached))
                            Java::QtJambi::NativeUtility$NativeLink::detach(env, nativeLink, id, flags.testFlag(Flag::HasDisposedSignal));
                        Java::QtJambi::NativeUtility$NativeLink::reset(env, nativeLink, id, flags.testFlag(Flag::HasDisposedSignal));
                    } catch (const JavaException& exn) {
                        exn.report(env);
                    }
                    env->DeleteGlobalRef(nativeLink);
                });
            }
            m_nativeLink = nullptr;
        }
    }
    if (!m_refCount->weakref.deref())
        delete m_refCount;
}

void QtJambiLink::init(JNIEnv*){
    m_flags.setFlag(Flag::IsInitialized, true);
}

// This method will always cleanup QtJambiLink's m_java.object.
void QtJambiLink::releaseJavaObject(JNIEnv *env)
{
    if (!m_flags.testFlag(Flag::JavaObjectIsReleased) && env) {
        if(m_flags.testFlag(Flag::NoGlobalRef)){
            Java::QtJambi::NativeUtility$NativeLink::releaseOwnershipForID(env, jlong(reinterpret_cast<void*>(this)));
        }else{
            Java::QtJambi::NativeUtility$NativeLink::releaseOwnership(env, m_nativeLink);
        }
        m_flags.setFlag(Flag::JavaObjectIsReleased);
    }
}

void QtJambiLink::disableDebugMessaging(){
    m_flags.setFlag(Flag::NoDebugMessaging, true);
}

bool QtJambiLink::isDebugMessagingDisabled() const {
    return m_flags.testFlag(Flag::NoDebugMessaging);
}

bool QtJambiLink::isMultiInheritanceType() const{
    return false;
}

void QtJambiLink::addInterface(const std::type_info&){
}

void QtJambiLink::removeInterface(const std::type_info&){
}

bool QtJambiLink::isInterfaceAvailable(const std::type_info&) const{
    return false;
}

void* QtJambiLink::typedPointer(const std::type_info&) const{
    return pointer();
}

void QtJambiLink::unregisterOffsets(void *) {
}

void QtJambiLink::registerPointer(void *ptr) {
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->linkLock());
        QWeakPointer<QtJambiLink> wp = *this;
        if(!storage->userObjectCache().contains(ptr, wp)){
            storage->userObjectCache().insert(ptr, wp);
            m_flags.setFlag(Flag::IsPointerRegistered);
        }
    }
}

void QtJambiLink::unregisterPointer(void *ptr) {
    if(m_flags.testFlag(Flag::IsPointerRegistered)) {
        QtJambiStorageRef storage;
        if(ptr){
            QWriteLocker locker(storage->linkLock());
            const QList<QWeakPointer<QtJambiLink>> values = storage->userObjectCache().values(ptr);
            QWeakPointer<QtJambiLink> wp = *this;
            for(const QWeakPointer<QtJambiLink>& wlink : values){
                QSharedPointer<QtJambiLink> link(wlink);
                if(!link || link==wp){
                    storage->userObjectCache().remove(ptr, link.toWeakRef());
                }
            }
        }
    }
}

void QtJambiLink::unregisterPointers(const QVector<void*>& pointers){
    QtJambiStorageRef storage;
    if(!pointers.isEmpty()){
        QWriteLocker locker(storage->linkLock());
        QWeakPointer<QtJambiLink> wp = *this;
        for(void* ptr : pointers){
            const QList<QWeakPointer<QtJambiLink>> values = storage->userObjectCache().values(ptr);
            for(const QWeakPointer<QtJambiLink>& wlink : values){
                QSharedPointer<QtJambiLink> link(wlink);
                if(!link || link==wp){
                    storage->userObjectCache().remove(ptr, link.toWeakRef());
                }
            }
        }
    }
}

#if defined(QTJAMBI_DEBUG_TOOLS)
const char* QtJambiLink::debugFlagsToString(char *s) const {
    *s++ = (char(int(QtJambiLink::ownership()) + '0'));
    *s++ = ((m_flags.testFlag(Flag::JavaObjectIsReleased)) ? 'R' : '.');
    *s++ = ((m_flags.testFlag(Flag::HasDependencies)) ? 'H' : '.');
    *s++ = ((m_flags.testFlag(Flag::IsDependent)) ? 'd' : '.');
    *s++ = ((m_flags.testFlag(Flag::IsDeleteLater)) ? 'e' : '.');
    *s++ = ((m_flags.testFlag(Flag::HasBeenCleaned))     ? 'C' : '.');
    *s++ = ((createdByJava())                              ? 'J' : '.');
    *s++ = ((isShell())                              ? 'S' : '.');
    *s++ = ((m_flags.testFlag(Flag::IsJavaLinkDetached))        ? 'D' : '.');
    *s = 0;
    return s;
}
#endif

inline size_t qHash(const QWeakPointer<QtJambiLink>& ptr, size_t seed = 0){
    return qHash(ptr.lock().get(), seed);
}

DependencyManagerObjectData::DependencyManagerObjectData() : QtJambiObjectData() {
}

DependencyManagerObjectData::~DependencyManagerObjectData(){
    QSet<QWeakPointer<QtJambiLink>> dependentObjects;
    QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>> finalizations;
    QSet<QWeakPointer<QtJambiLink>> qpropertyObjects;
    {
        QtJambiLinkWriteLock locker;
        dependentObjects.swap(m_dependentObjects);
        finalizations.swap(m_finalizations);
        qpropertyObjects.swap(m_condestroyedObjects);
    }
    if(!dependentObjects.isEmpty()
            || !finalizations.isEmpty()
            || !qpropertyObjects.isEmpty()
            ){
        if(JniEnvironment env{1024}){
            for(const QWeakPointer<QtJambiLink>& weakLink : qAsConst(dependentObjects)){
                if(QSharedPointer<QtJambiLink> link = weakLink.toStrongRef()){
                    link->invalidate(env);
                }
            }
        }
    }
}

void DependencyManagerObjectData::invalidateDependentObjects(QObject* obj, JNIEnv* env){
    QSet<QWeakPointer<QtJambiLink>> dependentObjects;
    DependencyManagerObjectData* d{nullptr};
    {
        QtJambiLinkWriteLock locker;
        dependentObjects.swap(m_dependentObjects);
        if(m_condestroyedObjects.isEmpty() && m_finalizations.isEmpty()){
            d = QtJambiObjectData::setUserData<DependencyManagerObjectData>(obj, nullptr);
        }
    }
    for(const QWeakPointer<QtJambiLink>& weakLink : qAsConst(dependentObjects)){
        if(QSharedPointer<QtJambiLink> link = weakLink.toStrongRef()){
            link->invalidate(env);
        }
    }
    dependentObjects.clear();
    if(d)
        delete d;
}

void DependencyManagerObjectData::clear(QObject* obj){
    QSet<QWeakPointer<QtJambiLink>> dependentObjects;
    QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>> finalizations;
    QSet<QWeakPointer<QtJambiLink>> condestroyedObjects;
    DependencyManagerObjectData* d{nullptr};
    {
        QtJambiLinkWriteLock locker;
        dependentObjects.swap(m_dependentObjects);
        finalizations.swap(m_finalizations);
        condestroyedObjects.swap(m_condestroyedObjects);
        d = QtJambiObjectData::setUserData<DependencyManagerObjectData>(obj, nullptr);
    }
    if(!dependentObjects.isEmpty()
            || !finalizations.isEmpty()
            || !condestroyedObjects.isEmpty()){
        if(JniEnvironment env{200}){
            clear(env, dependentObjects, finalizations, condestroyedObjects);
        }
    }
    if(d && d!=this)
        delete d;
    delete this;
}

void DependencyManagerObjectData::clear(QObject* obj, JNIEnv* env){
    QSet<QWeakPointer<QtJambiLink>> dependentObjects;
    QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>> finalizations;
    QSet<QWeakPointer<QtJambiLink>> condestroyedObjects;
    {
        QtJambiLinkWriteLock locker;
        dependentObjects.swap(m_dependentObjects);
        finalizations.swap(m_finalizations);
        condestroyedObjects.swap(m_condestroyedObjects);
        QtJambiObjectData::setUserData<DependencyManagerObjectData>(obj, nullptr);
    }
    clear(env, dependentObjects, finalizations, condestroyedObjects);
    delete this;
}

void DependencyManagerObjectData::clear(JNIEnv* env,
                                      QSet<QWeakPointer<QtJambiLink>>& dependentObjects,
                                      QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>>& finalizations,
                                      QSet<QWeakPointer<QtJambiLink>>& condestroyedObjects){
    for(const QWeakPointer<QtJambiLink>& weakLink : qAsConst(dependentObjects)){
        if(QSharedPointer<QtJambiLink> link = weakLink.toStrongRef()){
            link->invalidate(env);
        }
    }
    dependentObjects.clear();
    for(auto iter = finalizations.constKeyValueBegin(); iter!=finalizations.constKeyValueEnd(); ++iter){
        iter->second.first(env, iter->first);
        if(iter->second.second)
            iter->second.second(env, iter->first);
    }
    finalizations.clear();
    for(const QWeakPointer<QtJambiLink>& weakLink : qAsConst(condestroyedObjects)){
        if(QSharedPointer<QtJambiLink> link = weakLink.toStrongRef()){
            link->disposeManaged(env);
        }
    }
    condestroyedObjects.clear();
}

void DependencyManagerObjectData::addDependentObject(const QSharedPointer<QtJambiLink>& dependent){
    auto locker = QtJambiObjectData::writeLock();
    m_dependentObjects.insert(dependent.toWeakRef());
    if(DependentLinkInterface* dlink = dynamic_cast<DependentLinkInterface*>(dependent.data())){
        dlink->setDependencyManager(this);
    }else{
#if QT_VERSION >= QT_VERSION_CHECK(6,1,0)
        erase_if(m_dependentObjects, [](const QWeakPointer<QtJambiLink>& ptr)->bool{return ptr.isNull();});
#else
        auto it = m_dependentObjects.begin();
        const auto e = m_dependentObjects.end();
        while (it != e) {
            if ((*it).isNull()) {
                it = m_dependentObjects.erase(it);
            } else {
                ++it;
            }
        }
#endif
    }
}

void DependencyManagerObjectData::removeDependentObject(const QSharedPointer<QtJambiLink>& dependent){
    QtJambiLinkWriteLock locker;
    m_dependentObjects.remove(dependent.toWeakRef());
    if(DependentLinkInterface* dlink = dynamic_cast<DependentLinkInterface*>(dependent.data())){
        dlink->setDependencyManager(nullptr);
    }
}

void DependencyManagerObjectData::addConDestroyedObject(const QSharedPointer<QtJambiLink>& link){
    auto locker = QtJambiObjectData::writeLock();
    m_condestroyedObjects.insert(link.toWeakRef());
}

void DependencyManagerObjectData::removeConDestroyedObject(const QSharedPointer<QtJambiLink>& link){
    QtJambiLinkWriteLock locker;
    m_condestroyedObjects.remove(link.toWeakRef());
}

void DependencyManagerObjectData::addFinalization(void* data, FinalizationExecutor executor, FinalizationDeleter deleter){
    auto locker = QtJambiObjectData::writeLock();
    m_finalizations[data] = {executor, deleter};
}

void DependencyManagerObjectData::addFinalization(JNIEnv* env, jobject data, FinalizationExecutor executor, FinalizationDeleter deleter){
    auto id = Java::Runtime::System::identityHashCode(env, data);
    auto locker = QtJambiObjectData::writeLock();
    m_finalizations[data] = {executor, deleter};
    m_finalizationData[id] = data;
}

void DependencyManagerObjectData::removeFinalization(JNIEnv* env, void* data, bool execute){
    auto locker = QtJambiObjectData::writeLock();
    QPair<FinalizationExecutor,FinalizationDeleter> fun = m_finalizations[data];
    if(m_finalizations.remove(data)){
        locker.unlock();
        if(execute && fun.first)
            fun.first(env, data);
        if(fun.second)
            fun.second(env, data);
        locker.relock();
    }
}

void DependencyManagerObjectData::removeFinalization(JNIEnv* env, jobject data, bool execute){
    auto locker = QtJambiObjectData::writeLock();
    QPair<FinalizationExecutor,FinalizationDeleter> fun = m_finalizations[data];
    if(m_finalizations.remove(data)){
        locker.unlock();
        if(execute && fun.first)
            fun.first(env, data);
        if(fun.second)
            fun.second(env, data);
        locker.relock();
    }
}

bool DependencyManagerObjectData::hasDependencies() const{
    auto locker = QtJambiObjectData::readLock();
    return !m_dependentObjects.empty() && !m_condestroyedObjects.empty();
}

DependencyManagerObjectData* DependencyManagerObjectData::instance(const QObject* object, bool forceConstruction){
    DependencyManagerObjectData* dm{nullptr};
    {
        QtJambiLinkReadLock locker;
        dm = QtJambiObjectData::userData<DependencyManagerObjectData>(object);
    }
    if(!dm && forceConstruction){
        dm = new DependencyManagerObjectData();
        QtJambiLinkWriteLock locker;
        DependencyManagerObjectData* _dm = QtJambiObjectData::userData<DependencyManagerObjectData>(object);
        if(_dm){
            delete dm;
            dm = _dm;
        }else{
            _dm = QtJambiObjectData::setUserData<DependencyManagerObjectData>(const_cast<QObject*>(object), dm);
            if(_dm)
                delete _dm;
        }
    }
    return dm;
}

void QtJambiLink::registerConDestroyedObject(JNIEnv * env, const QObject* object, const QSharedPointer<QtJambiLink>& link){
    if(!link->m_flags.testFlag(Flag::IsManaged)){
        DependencyManagerObjectData* dm = DependencyManagerObjectData::instance(object);
        dm->addConDestroyedObject(link);
        link->setCppOwnership(env);
        link->m_flags.setFlag(Flag::NoNativeDeletion);
        link->m_flags.setFlag(Flag::IsManaged);
    }
}
void QtJambiLink::unregisterConDestroyedObject(JNIEnv * env, const QObject* object, const QSharedPointer<QtJambiLink>& link){
    if(link->m_flags.testFlag(Flag::IsManaged)){
        DependencyManagerObjectData* dm = DependencyManagerObjectData::instance(object);
        dm->removeConDestroyedObject(link);
        link->setJavaOwnership(env);
        link->m_flags.setFlag(Flag::NoNativeDeletion, false);
        link->m_flags.setFlag(Flag::IsManaged, false);
    }
}

void QtJambiLink::registerDependentObject(const QObject* object, const QSharedPointer<QtJambiLink>& link){
    if(object && !link->isShell() && !link->isSmartPointer() && !link->m_flags.testFlag(Flag::IsDependent)){
        if(!link->isQObject()){
            DependencyManagerObjectData* dm = DependencyManagerObjectData::instance(object);
            dm->addDependentObject(link);
        }
        if(link->ownership()!=Ownership::Java)
            link->m_flags.setFlag(Flag::NoNativeDeletion);
        link->m_flags.setFlag(Flag::IsDependent);
    }
}

void QtJambiLink::unregisterDependentObject(const QObject* object, const QSharedPointer<QtJambiLink>& link){
    if(object && !link->isShell() && !link->isSmartPointer() && link->m_flags.testFlag(Flag::IsDependent)){
        if(!link->isQObject()){
            if(DependencyManagerObjectData* dm = DependencyManagerObjectData::instance(object, false)){
                dm->removeDependentObject(link);
            }
        }
        link->m_flags.setFlag(Flag::NoNativeDeletion, false);
        link->m_flags.setFlag(Flag::IsDependent, false);
    }
}

void QtJambiLink::addFinalization(const QObject* object, void* data, FinalizationExecutor executor, FinalizationDeleter deleter){
    if(object){
        if(DependencyManagerObjectData* dm = DependencyManagerObjectData::instance(object)){
            dm->addFinalization(data, executor, deleter);
        }
    }
}

void QtJambiLink::addFinalization(const QObject* object, JNIEnv * env, jobject data, FinalizationExecutor executor, FinalizationDeleter deleter){
    if(object){
        if(DependencyManagerObjectData* dm = DependencyManagerObjectData::instance(object)){
            dm->addFinalization(env, data, executor, deleter);
        }
    }
}

void QtJambiLink::addFinalization(void* data, FinalizationExecutor executor, FinalizationDeleter deleter){
    if(isQObject()){
        addFinalization(qobject(), data, executor, deleter);
    }else{
        QSharedPointer<QtJambiLink> sp = *this;
        if(!m_flags.testFlag(Flag::HasDependencies))
            m_flags.setFlag(Flag::HasDependencies);
        QtJambiStorage* storage = getQtJambiStorage();
        {
            QWriteLocker locker(storage->lock());
            QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>>& container = storage->finalizations()[sp];
            container[data] = {executor, deleter};
        }
    }
}

void QtJambiLink::addFinalization(JNIEnv * env, jobject data, FinalizationExecutor executor, FinalizationDeleter deleter){
    if(isQObject()){
        addFinalization(qobject(), env, data, executor, deleter);
    }else{
        QSharedPointer<QtJambiLink> sp = *this;
        if(!m_flags.testFlag(Flag::HasDependencies))
            m_flags.setFlag(Flag::HasDependencies);
        QtJambiStorage* storage = getQtJambiStorage();
        jint hash = Java::Runtime::System::identityHashCode(env, data);
        {
            QWriteLocker locker(storage->lock());
            QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>>& container = storage->finalizations()[sp];
            container[data] = {executor, deleter};
            storage->finalizationData()[hash] = data;
        }
    }
}

void QtJambiLink::removeFinalization(JNIEnv * env, void* data, bool execute){
    if(isQObject()){
        removeFinalization(qobject(), env, data);
    }else{
        if(!m_flags.testFlag(Flag::HasDependencies))
            m_flags.setFlag(Flag::HasDependencies);
        QSharedPointer<QtJambiLink> sp = *this;
        QtJambiStorage* storage = getQtJambiStorage();
        QPair<FinalizationExecutor,FinalizationDeleter> fun;
        {
            QWriteLocker locker(storage->lock());
            QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>>& container = storage->finalizations()[sp];
            fun = container.take(data);
        }
        if(execute && fun.first)
            fun.first(env, data);
        if(fun.second)
            fun.second(env, data);
    }
}

void QtJambiLink::removeFinalization(JNIEnv * env, jobject data, bool execute){
    if(isQObject()){
        removeFinalization(qobject(), env, data);
    }else{
        if(!m_flags.testFlag(Flag::HasDependencies))
            m_flags.setFlag(Flag::HasDependencies);
        QSharedPointer<QtJambiLink> sp = *this;
        QtJambiStorage* storage = getQtJambiStorage();
        QPair<FinalizationExecutor,FinalizationDeleter> fun;
        jint hash = Java::Runtime::System::identityHashCode(env, data);
        {
            QWriteLocker locker(storage->lock());
            QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>>& container = storage->finalizations()[sp];
            void* ptr = storage->finalizationData().take(hash);
            fun = container.take(ptr);
        }
        if(execute && fun.first)
            fun.first(env, data);
        if(fun.second)
            fun.second(env, data);
    }
}

void QtJambiLink::removeFinalization(const QObject* object, JNIEnv * env, void* data, bool execute){
    if(object){
        if(DependencyManagerObjectData* ddata = DependencyManagerObjectData::instance(object, false)){
            ddata->removeFinalization(env, data, execute);
        }
    }
}

void QtJambiLink::removeFinalization(const QObject* object, JNIEnv * env, jobject data, bool execute){
    if(object){
        if(DependencyManagerObjectData* ddata = DependencyManagerObjectData::instance(object, false)){
            ddata->removeFinalization(env, data, execute);
        }
    }
}

void QtJambiLink::registerDependentObject(const QSharedPointer<QtJambiLink>& link)
{
    if(isQObject()){
        registerDependentObject(qobject(), link);
    }else if(!link->isShell() && !link->isSmartPointer() && !link->m_flags.testFlag(Flag::IsDependent)){
        if(!link->isQObject()){
            if(!m_flags.testFlag(Flag::HasDependencies))
                m_flags.setFlag(Flag::HasDependencies);
            QSharedPointer<QtJambiLink> sp = *this;
            QtJambiStorage* storage = getQtJambiStorage();
            {
                QWriteLocker locker(storage->lock());
                storage->dependencies().insert(sp, link);
                storage->dependencies().remove(sp, QWeakPointer<QtJambiLink>{});
            }
        }
        if(link->ownership()!=Ownership::Java)
            link->m_flags.setFlag(Flag::NoNativeDeletion);
        link->m_flags.setFlag(Flag::IsDependent);
    }
}

void QtJambiLink::unregisterDependentObject(const QSharedPointer<QtJambiLink>& link)
{
    if(isQObject()){
        unregisterDependentObject(qobject(), link);
    }else if(!link->isShell() && !link->isQObject() && !link->isSmartPointer() && link->m_flags.testFlag(Flag::IsDependent)){
        if(!link->isQObject()){
            QSharedPointer<QtJambiLink> sp = *this;
            QtJambiStorage* storage = getQtJambiStorage();
            {
                QWriteLocker locker(storage->lock());
                storage->dependencies().remove(sp, link);
                storage->dependencies().remove(sp, QWeakPointer<QtJambiLink>{});
                if(!storage->dependencies().contains(sp))
                    m_flags.setFlag(Flag::HasDependencies, false);
            }
        }
        link->m_flags.setFlag(Flag::IsDependent, false);
        link->m_flags.setFlag(Flag::NoNativeDeletion, false);
    }
}

void QtJambiLink::registerConDestroyedObject(JNIEnv * env, const QSharedPointer<QtJambiLink>& link)
{
    if(isQObject()){
        registerConDestroyedObject(env, qobject(), link);
    }else if(!link->m_flags.testFlag(Flag::IsManaged)){
        QSharedPointer<QtJambiLink> sp = *this;
        if(!m_flags.testFlag(Flag::HasDependencies))
            m_flags.setFlag(Flag::HasDependencies);
        QtJambiStorage* storage = getQtJambiStorage();
        {
            QWriteLocker locker(storage->lock());
            storage->conDestroyedDependencies().insert(sp, link);
            storage->conDestroyedDependencies().remove(sp, QWeakPointer<QtJambiLink>{});
            link->m_flags.setFlag(Flag::NoNativeDeletion);
            link->m_flags.setFlag(Flag::IsManaged);
        }
        link->setCppOwnership(env);
    }
}

void QtJambiLink::unregisterConDestroyedObject(JNIEnv * env, const QSharedPointer<QtJambiLink>& link)
{
    if(isQObject()){
        unregisterConDestroyedObject(env, qobject(), link);
    }else if(link->m_flags.testFlag(Flag::IsManaged)){
        QSharedPointer<QtJambiLink> sp = *this;
        QtJambiStorage* storage = getQtJambiStorage();
        {
            QWriteLocker locker(storage->lock());
            storage->conDestroyedDependencies().remove(sp, link);
            storage->conDestroyedDependencies().remove(sp, QWeakPointer<QtJambiLink>{});
            if(!storage->conDestroyedDependencies().contains(sp))
                m_flags.setFlag(Flag::HasDependencies, false);
            link->m_flags.setFlag(Flag::IsManaged, false);
            link->m_flags.setFlag(Flag::NoNativeDeletion, false);
        }
        link->setJavaOwnership(env);
    }
}

void QtJambiLink::invalidateDependentObjects(JNIEnv *env)
{
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this, "QtJambiLink::invalidateDependentObjects(JNIEnv *)")
    if(isQObject()){
#ifdef AVAILABLE_IN_DELETION
        if(!isInDestructor())
#endif
        if(QObject* obj = qobject()){
            if(DependencyManagerObjectData* dm = DependencyManagerObjectData::instance(obj, false)){
                dm->invalidateDependentObjects(obj, env);
            }
        }
    }
    if(m_flags.testFlag(Flag::HasDependencies)){
        QSharedPointer<QtJambiLink> sp = *this;
        QList<QWeakPointer<QtJambiLink>> dependentObjects;
        QtJambiStorage* storage = getQtJambiStorage();
        {
            QWriteLocker locker(storage->lock());
            dependentObjects = storage->dependencies().values(sp);
            storage->dependencies().remove(sp);
        }
        for(const QWeakPointer<QtJambiLink>& weakLink : qAsConst(dependentObjects)){
            if(QSharedPointer<QtJambiLink> link = weakLink.toStrongRef()){
                link->invalidate(env);
            }
        }
    }
}

void QtJambiLink::clearAllDependencies(JNIEnv *env)
{
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this, "QtJambiLink::clearAllDependencies(JNIEnv *)")
    if(isQObject()){
#ifdef AVAILABLE_IN_DELETION
        if(!isInDestructor())
#endif
        if(QObject* obj = qobject()){
            if(DependencyManagerObjectData* dm = DependencyManagerObjectData::instance(obj, false)){
                dm->clear(obj, env);
            }
        }
    }
    if(m_flags.testFlag(Flag::HasDependencies)){
        QList<QWeakPointer<QtJambiLink>> dependentObjects;
        QList<QWeakPointer<QtJambiLink>> conDestroyedObjects;
        QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>> finalizations;
        QSharedPointer<QtJambiLink> sp = *this;
        QtJambiStorage* storage = getQtJambiStorage();
        {
            QWriteLocker locker(storage->lock());
            dependentObjects = storage->dependencies().values(sp);
            storage->dependencies().remove(sp);
            finalizations = storage->finalizations().take(sp);
            conDestroyedObjects = storage->conDestroyedDependencies().values(sp);
            storage->conDestroyedDependencies().remove(sp);
        }
        for(const QWeakPointer<QtJambiLink>& weakLink : qAsConst(conDestroyedObjects)){
            if(QSharedPointer<QtJambiLink> link = weakLink.toStrongRef()){
                link->disposeManaged(env);
            }
        }
        for(const QWeakPointer<QtJambiLink>& weakLink : qAsConst(dependentObjects)){
            if(QSharedPointer<QtJambiLink> link = weakLink.toStrongRef()){
                link->invalidate(env);
            }
        }
        for(auto iter = finalizations.constKeyValueBegin(); iter!=finalizations.constKeyValueEnd(); ++iter){
            iter->second.first(env, iter->first);
            if(iter->second.second)
                iter->second.second(env, iter->first);
        }
    }
}

void QtJambiLink::clearAllDependencies(){
    if(!isInDestructor()){
        if(isQObject()){
            if(QObject* obj = qobject()){
                if(DependencyManagerObjectData* dm = DependencyManagerObjectData::instance(obj, false)){
                    dm->clear(obj);
                }
            }
        }else{
            if(m_flags.testFlag(Flag::HasDependencies)){
                QList<QWeakPointer<QtJambiLink>> dependentObjects;
                QList<QWeakPointer<QtJambiLink>> conDestroyedObjects;
                QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>> finalizations;
                QSharedPointer<QtJambiLink> sp = *this;
                QtJambiStorage* storage = getQtJambiStorage();
                {
                    QWriteLocker locker(storage->lock());
                    dependentObjects = storage->dependencies().values(sp);
                    storage->dependencies().remove(sp);
                    finalizations = storage->finalizations().take(sp);
                    conDestroyedObjects = storage->conDestroyedDependencies().values(sp);
                    storage->conDestroyedDependencies().remove(sp);
                }
                if(!conDestroyedObjects.isEmpty()
                        || !finalizations.isEmpty()
                        || !dependentObjects.isEmpty()){
                    if(JniEnvironment env{200}){
                        for(const QWeakPointer<QtJambiLink>& weakLink : qAsConst(conDestroyedObjects)){
                            if(QSharedPointer<QtJambiLink> link = weakLink.toStrongRef()){
                                link->disposeManaged(env);
                            }
                        }
                        for(const QWeakPointer<QtJambiLink>& weakLink : qAsConst(dependentObjects)){
                            if(QSharedPointer<QtJambiLink> link = weakLink.toStrongRef()){
                                link->invalidate(env);
                            }
                        }
                        for(auto iter = finalizations.constKeyValueBegin(); iter!=finalizations.constKeyValueEnd(); ++iter){
                            iter->second.first(env, iter->first);
                            if(iter->second.second)
                                iter->second.second(env, iter->first);
                        }
                    }
                }
            }
        }
    }
}

void QtJambiLink::disposeManaged(JNIEnv *env){
    if(m_flags.testFlag(Flag::IsManaged)){
        m_flags.setFlag(Flag::IsManaged, false);
        onDispose(env);
    }
}

void PlainObjectLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if (m_pointer){
        if(!isShell())
            clearAllDependencies(env);
        void* pointer;
        {
            QtJambiLinkWriteLock locker;
            pointer = m_pointer;
            m_pointer = nullptr;
        }
        if(pointer){
            unregisterPointer(pointer);
            unregisterOffsets(pointer);
            m_flags.setFlag(Flag::IsPointerRegistered, false);
            if(!m_flags.testFlag(Flag::NoNativeDeletion) && (m_flags.testFlag(Flag::JavaOwnership) || forced)){
                deleteNow(env, pointer);
                pointer = nullptr;
            }
        }
    }
    dispose();
}

template<typename Super>
void OwnedObjectLink<Super>::deleteNativeObject(JNIEnv *env, bool forced)
{
    if (this->m_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this, "QtJambiLink::deleteNativeObject(JNIEnv *, bool)")
        if(!this->isShell())
            this->clearAllDependencies(env);
        bool nativeDeletion = !m_flags.testFlag(QtJambiLink::Flag::NoNativeDeletion)
                && (m_flags.testFlag(QtJambiLink::Flag::JavaOwnership) || forced);
        OwnerInfo<Super> ownerInfo = clearPointer(nativeDeletion);
        this->unregisterPointer(ownerInfo.pointer);
        this->unregisterOffsets(ownerInfo.pointer);
        this->m_flags.setFlag(QtJambiLink::Flag::IsPointerRegistered, false);
        if(nativeDeletion){
            if(ownerInfo.ownerThread){
                if(ownerInfo.ownerPointer){
                    this->setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                    QThreadObjectData* threadUserData = QThreadObjectData::ensureThreadUserData(ownerInfo.ownerThread);
                    if constexpr(Super::IS_SMART_POINTER){
                        if(ownerInfo.shared_pointer){
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("use deleter event for shared pointer")
                            if (QAbstractEventDispatcher::instance(ownerInfo.ownerThread)) {
                                DestructorEvent* event = new DestructorEvent(ownerInfo.ownerPointer, *this,
                                                                             new typename OwnerInfo<Super>::SharedPointerT(ownerInfo.shared_pointer),
                                                                             [](void * ptr,bool){delete reinterpret_cast<typename OwnerInfo<Super>::SharedPointerT*>(ptr);}, this->isShell());
                                QCoreApplication::postEvent(threadUserData->threadDeleter(), event);
                            }else{
                                threadUserData->doAtThreadEnd([_shared_pointer = typename OwnerInfo<Super>::SharedPointerT(ownerInfo.shared_pointer)]() mutable {
                                    _shared_pointer = typename OwnerInfo<Super>::SharedPointerT();
                                });
                                if(!this->isShell())
                                    this->dispose();
                            }
                            ownerInfo.shared_pointer = typename OwnerInfo<Super>::SharedPointerT();
                        }else{
                            this->postDeletion(ownerInfo.ownerPointer, ownerInfo.ownerThread, threadUserData, ownerInfo.pointer);
                            ownerInfo.pointer = nullptr;
                        }
                    }else{
                        this->postDeletion(ownerInfo.ownerPointer, ownerInfo.ownerThread, threadUserData, ownerInfo.pointer);
                        ownerInfo.pointer = nullptr;
                    }
                }else if(ownerInfo.vud){
                    QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
                    if constexpr(Super::IS_SMART_POINTER){
                        if(ownerInfo.shared_pointer){
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("call shared pointer deleter function")
                            ownerInfo.vud->addDeleter([shared_pointer = typename OwnerInfo<Super>::SharedPointerT(ownerInfo.shared_pointer)]()mutable{
                                shared_pointer = typename OwnerInfo<Super>::SharedPointerT();
                            });
                            if(!this->isShell())
                                this->dispose();
                        }else{
                            this->addDeleter(ownerInfo.vud, ownerInfo.pointer);
                            ownerInfo.pointer = nullptr;
                        }
                    }else{
                        this->addDeleter(ownerInfo.vud, ownerInfo.pointer);
                        ownerInfo.pointer = nullptr;
                    }
                }
            }

            if constexpr(Super::IS_SMART_POINTER){
                if(ownerInfo.shared_pointer){
                    QTJAMBI_DEBUG_PRINT_WITH_ARGS("call shared pointer deleter function")
                    QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
                    ownerInfo.shared_pointer = typename OwnerInfo<Super>::SharedPointerT();
                    if(!this->isShell())
                        this->dispose();
                }else if(ownerInfo.pointer){
                    this->deleteNow(env, ownerInfo.pointer);
                    ownerInfo.pointer = nullptr;
                }
            }else{
                if(ownerInfo.pointer){
                    this->deleteNow(env, ownerInfo.pointer);
                    ownerInfo.pointer = nullptr;
                }
            }
        }else{
            this->dispose();
        }
    }else{
        this->dispose();
    }
}

template<bool pending>
QObjectPlainPointerLink<pending>::QObjectPlainPointerLink(JNIEnv *env, jobject nativeLink,
                                                              jobject jobj, const QMetaObject* metaObject,
                                                              QObject *ptr, SafeBool isQThread, SafeBool created_by_java,
                                                              SafeBool is_shell, JavaException& ocurredException)
    : Super(env, nativeLink,
            LINK_NAME_ARG(metaObject->className())
            ptr, created_by_java, is_shell, ocurredException)
{
    if(Q_LIKELY(!ocurredException)){
        this->setIsQThread(isQThread);
        QtJambiMetaObject::resolveSignals(env, jobj, metaObject, ocurredException);
        if constexpr(pending){
            this->m_flags.setFlag(QtJambiLink::Flag::IsPendingValueOwner, isValueOwner(metaObject));
        }
    }
}

template<bool pending>
void QObjectPlainPointerLink<pending>::init(JNIEnv* env, QSharedPointer<QtJambiLink>* replacement){
    if(!this->isInitialized()){
        QtJambiLink::init(env);
        QObject* object = m_pointer;
        QSharedPointer<QtJambiLink> _this = *this;
        Q_ASSERT(object);
        const QMetaObject* metaObject{nullptr};
        bool _isValueOwner = this->m_flags.testFlag(QtJambiLink::Flag::IsPendingValueOwner);
        if constexpr(pending){
            Q_UNUSED(metaObject)
            _isValueOwner = this->m_flags.testFlag(QtJambiLink::Flag::IsPendingValueOwner);
        }else{
            metaObject = object->metaObject();
            _isValueOwner = isValueOwner(metaObject);
        }

        QtJambiLinkObjectData* lud{nullptr};
        ValueOwnerObjectData* vud{nullptr};
        {
            auto locker = QtJambiObjectData::readLock();
            if(_isValueOwner)
                vud = QtJambiObjectData::userData<ValueOwnerObjectData>(object);
            lud = QtJambiObjectData::userData<QtJambiLinkObjectData>(object);
        }
        if(lud){
            if(replacement){
                if constexpr(pending){
                    if(QSharedPointer<QtJambiLink> other = lud->link()){
                        jobject obj1 = this->getJavaObjectLocalRef(env);
                        jobject obj2 = other->getJavaObjectLocalRef(env);
                        if(!env->IsSameObject(obj1, obj2)){
                            jclass class1 = env->GetObjectClass(obj1);
                            jclass class2 = env->GetObjectClass(obj2);
                            if(env->IsSameObject(class1, class2)){
                                *replacement = other;
                                return;
                            }
                        }
                    }
                }else{
                    *replacement = lud->link();
                    if(*replacement)
                        return;
                }
            }
            QtJambiLink::resetDataLink(lud);
        }
        if(_isValueOwner && !vud)
            vud = new ValueOwnerObjectData(object);
        if(this->isQThread() && object!=QCoreApplicationPrivate::theMainThread.loadRelaxed()){
            QThread* thread = static_cast<QThread*>(object);
            QSharedPointer<QtJambiLink> _this = *this;
            QThreadObjectData* data{nullptr};
            QtJambiLink::setIsQThread(true);
            if(lud){
                if(vud){
                    auto locker = QtJambiObjectData::writeLock();
                    data = QtJambiObjectData::userData<QThreadObjectData>(thread);
                    vud = QtJambiObjectData::setUserData<ValueOwnerObjectData>(object, vud);
                }else{
                    auto locker = QtJambiObjectData::readLock();
                    data = QtJambiObjectData::userData<QThreadObjectData>(thread);
                }
                lud = nullptr; // don't delete
            }else{
                if constexpr(pending){
                    lud = new ThreadLinkObjectData(thread, _this);
                }else{
                    lud = new MetaObjectThreadLinkObjectData(thread, _this, metaObject);
                }
                {
                    auto locker = QtJambiObjectData::writeLock();
                    data = QtJambiObjectData::userData<QThreadObjectData>(thread);
                    lud = QtJambiObjectData::setUserData<QtJambiLinkObjectData>(thread, lud);
                    if(vud)
                        vud = QtJambiObjectData::setUserData<ValueOwnerObjectData>(object, vud);
                }
            }
            if(!data)
                ThreadPrivate::fromQThread(env, this->getJavaObjectLocalRef(env), thread, _this);
        }else{
            if(lud){
                if(vud){
                    auto locker = QtJambiObjectData::writeLock();
                    vud = QtJambiObjectData::setUserData<ValueOwnerObjectData>(object, vud);
                }
                lud = nullptr; // don't delete
            }else{
                if constexpr(pending){
                    lud = new LinkObjectData(*this);
                }else{
                    lud = new MetaObjectLinkObjectData(*this, metaObject);
                }
                {
                    auto locker = QtJambiObjectData::writeLock();
                    lud = QtJambiObjectData::setUserData<QtJambiLinkObjectData>(object, lud);
                    if(vud)
                        vud = QtJambiObjectData::setUserData<ValueOwnerObjectData>(object, vud);
                }
            }
        }
        if(vud)
            delete vud;
        if(lud)
            delete lud;
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
        if constexpr(!pending){
            this->m_qtTypeName = metaObject->className();
        }
#endif
        if(this->createdByJava()){
            if(object->parent())
                this->setCppOwnership(env);
            else
                this->setJavaOwnership(env);
        }else{
            if(object->parent())
                this->setCppOwnership(env);
            else
                this->setSplitOwnership(env);
        }
    }
}

template<template<typename> class SmartPointer>
ObjectSmartPointerLink<SmartPointer>::~ObjectSmartPointerLink(){
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this, "ObjectSmartPointerLink::~ObjectSmartPointerLink()")
    if (!QtJambiLink::m_flags.testFlag(QtJambiLink::Flag::IsJavaLinkDetached) || !QtJambiLink::m_flags.testFlag(QtJambiLink::Flag::JavaObjectIsReleased) || this->m_pointer) {
        try{
            if(JniEnvironment env = this->isQThread() ? JniEnvironment{300} : DefaultJniEnvironment{300}){
                this->releaseJavaObject(env);
                this->detachJavaLink(env);
            }
        } catch (const std::exception& e) {
            qCWarning(DebugAPI::internalCategory, "%s", e.what());
        } catch (...) {
        }
    }
}

template<template<typename> class SmartPointer>
typename SmartPointerLink<SmartPointer,char>::SharedPointerT ObjectSmartPointerLink<SmartPointer>::getSmartPointer() const {
    if(m_pointer)
        return SmartPointerLink<SmartPointer,char>::getSmartPointer();
    else return{};
}

template<>
QSharedPointer<char> ObjectSmartPointerLink<QWeakPointer>::getSmartPointer() const {
    if(m_pointer){
        QSharedPointer<char> result = SmartPointerLink<QWeakPointer,char>::getSmartPointer();
        if(!result){
            if(containerAccess()){
                result = QSharedPointer<char>(reinterpret_cast<char*>(m_pointer), SmartContainerDeleter<QWeakPointer>(containerAccess(), *this));
            }else if(deleterFunction()){
                result = QSharedPointer<char>(reinterpret_cast<char*>(m_pointer), SmartDeleterFunctionDeleter<QWeakPointer>(*deleterFunction(), *this));
            }else if(metaType()){
                result = QSharedPointer<char>(reinterpret_cast<char*>(m_pointer), SmartMetaTypeDeleter<QWeakPointer>(*metaType(), *this));
            }
        }
        return result;
    }else return{};
}

template<>
std::shared_ptr<char> ObjectSmartPointerLink<std::weak_ptr>::getSmartPointer() const {
    if(m_pointer){
        std::shared_ptr<char> result = SmartPointerLink<std::weak_ptr,char>::getSmartPointer();
        if(!result){
            if(containerAccess()){
                result = std::shared_ptr<char>(reinterpret_cast<char*>(m_pointer), SmartContainerDeleter<std::weak_ptr>(containerAccess(), *this));
            }else if(deleterFunction()){
                result = std::shared_ptr<char>(reinterpret_cast<char*>(m_pointer), SmartDeleterFunctionDeleter<std::weak_ptr>(*deleterFunction(), *this));
            }else if(metaType()){
                result = std::shared_ptr<char>(reinterpret_cast<char*>(m_pointer), SmartMetaTypeDeleter<std::weak_ptr>(*metaType(), *this));
            }
        }
        return result;
    }else return{};
}

template<typename Super>
void ContainerLink<Super>::addDeleter(const QExplicitlySharedDataPointer<ValueOwnerObjectDataPrivate>& vud, void* pointer){
    QTJAMBI_DEBUG_PRINT_WITH_ARGS("use deleter event for object %p", pointer)
    Q_ASSERT(this->m_containerAccess);
    vud->addDeleter([containerAccess = QSharedPointer<AbstractContainerAccess>(this->m_containerAccess, &containerDisposer), pointer, isShell = this->isShell()](){
        containerAccess->deleteContainer(pointer);
    });
    this->m_containerAccess = nullptr;
    if(!this->isShell())
        this->dispose();
}

template<typename Super>
void ContainerLink<Super>::postDeletion(const QPointer<const QObject>& parent, QThread* ownerThread, QThreadObjectData* threadUserData, void* pointer){
    QTJAMBI_DEBUG_PRINT_WITH_ARGS("use deleter event for object %p", pointer)
    Q_ASSERT(this->m_containerAccess);
    if (QAbstractEventDispatcher::instance(ownerThread)) {
        ContainerDestructionEvent* event = new ContainerDestructionEvent(parent, *this, pointer, this->m_containerAccess);
        this->m_containerAccess = nullptr;
        QCoreApplication::postEvent(threadUserData->threadDeleter(), event);
    }else{
        threadUserData->doAtThreadEnd([containerAccess = QSharedPointer<AbstractContainerAccess>(this->m_containerAccess, &containerDisposer), pointer, isShell = this->isShell()](){
            containerAccess->deleteContainer(pointer);
        });
        this->m_containerAccess = nullptr;
        if(!this->isShell())
            this->dispose();
    }
}

template<typename Super>
void ContainerLink<Super>::deleteNow(JNIEnv *, void* pointer) {
    QTJAMBI_DEBUG_PRINT_WITH_ARGS("call deleteContainer on object %p", pointer)
    Q_ASSERT(m_containerAccess);
    QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
    m_containerAccess->deleteContainer(pointer);
    if(!this->isShell())
        this->dispose();
}

template<typename Super>
void DeletableLink<Super>::addDeleter(const QExplicitlySharedDataPointer<ValueOwnerObjectDataPrivate>& vud, void* pointer){
    QTJAMBI_DEBUG_PRINT_WITH_ARGS("use deleter event for object %p", pointer)
    QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
    vud->addDeleter([deleter_function = this->m_deleter_function, pointer, isShell = this->isShell()](){
        deleter_function(pointer, isShell);
    });
    if(!this->isShell())
        this->dispose();
}

template<typename Super>
void DeletableLink<Super>::postDeletion(const QPointer<const QObject>& parent, QThread* ownerThread, QThreadObjectData* threadUserData, void* pointer){
    QTJAMBI_DEBUG_PRINT_WITH_ARGS("use deleter event for object %p", pointer)
    if (QAbstractEventDispatcher::instance(ownerThread)) {
        DestructorEvent* event = new DestructorEvent(parent, *this, pointer, this->m_deleter_function, this->isShell());
        QCoreApplication::postEvent(threadUserData->threadDeleter(), event);
    }else{
        threadUserData->doAtThreadEnd([deleter_function = this->m_deleter_function, pointer, isShell = this->isShell()](){
            deleter_function(pointer, isShell);
        });
        if(!this->isShell())
            this->dispose();
    }
}

template<typename Super>
void DeletableLink<Super>::deleteNow(JNIEnv *, void* pointer){
    QTJAMBI_DEBUG_PRINT_WITH_ARGS("call deleter_function on object %p", pointer)
    QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
    this->m_deleter_function(pointer, this->isShell());
    if(!this->isShell())
        this->dispose();
}

template<typename Super>
void MetaTypedLink<Super>::addDeleter(const QExplicitlySharedDataPointer<ValueOwnerObjectDataPrivate>& vud, void* pointer){
    QTJAMBI_DEBUG_PRINT_WITH_ARGS("use deleter event on object %p of type %s", pointer, static_cast<const char*>(m_meta_type.name()))
    QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
    vud->addDeleter([metaType = this->m_meta_type, pointer](){
        metaType.destroy(pointer);
    });
    if(!this->isShell())
        this->dispose();
}

template<typename Super>
void MetaTypedLink<Super>::postDeletion(const QPointer<const QObject>& parent, QThread* ownerThread, QThreadObjectData* threadUserData, void* pointer){
    QTJAMBI_DEBUG_PRINT_WITH_ARGS("use deleter event on object %p of type %s", pointer, static_cast<const char*>(m_meta_type.name()))
    if (QAbstractEventDispatcher::instance(ownerThread)) {
        MetaTypeDestructionEvent* event = new MetaTypeDestructionEvent(parent, *this, pointer, this->m_meta_type);
        QCoreApplication::postEvent(threadUserData->threadDeleter(), event);
    }else{
        threadUserData->doAtThreadEnd([metaType = this->m_meta_type,pointer](){
            metaType.destroy(pointer);
        });
        if(!this->isShell())
            this->dispose();
    }
}

template<typename Super>
void MetaTypedLink<Super>::deleteNow(JNIEnv *, void* pointer){
    QTJAMBI_DEBUG_PRINT_WITH_ARGS("call QMetaType::destroy() on object %p of type %s", pointer, static_cast<const char*>(m_meta_type.name()))
    QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
    this->m_meta_type.destroy(pointer);
    if(!this->isShell())
        this->dispose();
}

template<template<typename> class SmartPointer>
void PlainObjectSmartPointerLink<SmartPointer>::deleteNativeObject(JNIEnv *env, bool){
    typedef typename std::conditional<std::is_same<SmartPointer<char>,QWeakPointer<char>>::value, QSharedPointer<char>,
                                      typename std::conditional<std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value, std::shared_ptr<char>, SmartPointer<char>>::type>::type SharedPointerT;
    if(this->m_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this, "QtJambiLink::deleteNativeObject(JNIEnv *env, bool)")
        if(!this->isShell())
            this->clearAllDependencies(env);
        this->unregisterOffsets(this->m_pointer);
        this->unregisterPointer(this->m_pointer);
        this->m_flags.setFlag(QtJambiLink::Flag::IsPointerRegistered, false);

        SharedPointerT shared_pointer;
        if constexpr(std::is_same<SmartPointer<QObject>,QWeakPointer<QObject>>::value
                || std::is_same<SmartPointer<QObject>,std::weak_ptr<QObject>>::value){
            shared_pointer = this->m_smartPointer.lock();
        }else{
            shared_pointer = this->m_smartPointer;
        }
        void* pointer;
        {
            QtJambiLinkWriteLock locker;
            pointer = this->m_pointer;
            this->m_pointer = nullptr;
            this->m_smartPointer = SmartPointer<char>();
        }
        if(shared_pointer){
            QTJAMBI_DEBUG_PRINT_WITH_ARGS("call shared pointer deleter function")
            QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
            shared_pointer = SharedPointerT();
            if(!this->isShell())
                this->dispose();
        }else if(pointer){
            this->deleteNow(env, pointer);
            pointer = nullptr;
        }
    }else{
        this->dispose();
    }
}

template<template<typename> class SmartPointer, typename T>
SmartPointerLink<SmartPointer,T>::SmartPointerLink(JNIEnv *env, jobject nativeLink,
                   LINK_NAME_ARG(const char *qt_name)
                   SafeBool created_by_java, QtJambiLink::Ownership ownership, SafeBool is_shell, PointerType* pointer, SmartPointer<T>*& smartPointer, JavaException& ocurredException)
    : Super(env, nativeLink,
                  LINK_NAME_ARG(qt_name)
                  pointer, created_by_java, is_shell, ocurredException), m_smartPointer(), m_hiddenOwnership(ownership) {
    if constexpr(std::is_same<SmartPointer<T>,QSharedPointer<T>>::value){
        this->m_flags.setFlag(QtJambiLink::Flag::QSharedPointer);
    }else if constexpr(std::is_same<SmartPointer<T>,QWeakPointer<T>>::value){
        this->m_flags.setFlag(QtJambiLink::Flag::QWeakPointer);
    }else if constexpr(std::is_same<SmartPointer<T>,std::weak_ptr<T>>::value){
        this->m_flags.setFlag(QtJambiLink::Flag::weak_ptr);
    }else{
        this->m_flags.setFlag(QtJambiLink::Flag::shared_ptr);
    }
    smartPointer = &m_smartPointer;
}

template<template<typename> class SmartPointer, typename T>
void SmartPointerLink<SmartPointer,T>::setCppOwnership(JNIEnv *) {
    if(m_hiddenOwnership!=QtJambiLink::Ownership::None)
        m_hiddenOwnership = QtJambiLink::Ownership::Cpp;
}

template<template<typename> class SmartPointer, typename T>
void SmartPointerLink<SmartPointer,T>::setSplitOwnership(JNIEnv *) {
    if(m_hiddenOwnership!=QtJambiLink::Ownership::None)
        m_hiddenOwnership = QtJambiLink::Ownership::Split;
}

template<template<typename> class SmartPointer, typename T>
void SmartPointerLink<SmartPointer,T>::setJavaOwnership(JNIEnv *env) {
    if(m_hiddenOwnership==QtJambiLink::Ownership::None)
        QtJambiLink::setJavaOwnership(env);
    else
        m_hiddenOwnership = QtJambiLink::Ownership::Java;
}

template<template<typename> class SmartPointer, typename T>
void SmartPointerLink<SmartPointer,T>::setDefaultOwnership(JNIEnv *env) {
    if(m_hiddenOwnership!=QtJambiLink::Ownership::None){
        if (createdByJava())
            m_hiddenOwnership = QtJambiLink::Ownership::Java;
        else{
            if constexpr(std::is_same_v<T,void>){
                Q_UNUSED(env)
                m_hiddenOwnership = QtJambiLink::Ownership::Split;
            }else{
                if(QObject* obj = qobject()){
                    if(obj->parent()==nullptr)
                        m_hiddenOwnership = QtJambiLink::Ownership::Split;
                    else
                        m_hiddenOwnership = QtJambiLink::Ownership::Cpp;
                }else{
                    QtJambiLink::setJavaOwnership(env);
                    m_hiddenOwnership = QtJambiLink::Ownership::Java;
                }
            }
        }
    }
}

template<template<typename> class SmartPointer, typename T>
QtJambiLink::Ownership SmartPointerLink<SmartPointer,T>::ownership() const{
    if(m_hiddenOwnership==QtJambiLink::Ownership::None)
        return QtJambiLink::ownership();
    return m_hiddenOwnership;
}

template<template<typename> class SmartPointer, typename T>
typename SmartPointerLink<SmartPointer,T>::SharedPointerT SmartPointerLink<SmartPointer,T>::getSmartPointer() const{
    return SmartPointerLink<SmartPointer,T>::SharedPointerT(m_smartPointer);
}

// #### END ##################  ObjectSmartPointerLink  ##################### END ####

// ### BEGIN #################  QObjectSmartPointerLink  ################### BEGIN ###

template<template<typename> class SmartPointer>
QObjectSmartPointerLink<SmartPointer>::QObjectSmartPointerLink(JNIEnv *env, jobject nativeLink,
                                                                   LINK_NAME_ARG(const char *qt_name)
                                                                   SafeBool created_by_java, QtJambiLink::Ownership ownership,
                                                                   SafeBool is_shell,
                                                                   QObject* pointer,
                                                                   SmartPointer<QObject>*& smartPointer,
                                                                   JavaException& ocurredException)
    : SmartPointerLink<SmartPointer,QObject>(env, nativeLink,
                                             LINK_NAME_ARG(qt_name)
                                             created_by_java, ownership, is_shell, pointer, smartPointer, ocurredException)
{
}

template<template<typename> class SmartPointer>
QObjectSmartPointerLink<SmartPointer>::~QObjectSmartPointerLink(){
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this, "QObjectSmartPointerLink::~QObjectSmartPointerLink()")
    if (!QtJambiLink::m_flags.testFlag(QtJambiLink::Flag::IsJavaLinkDetached) || !QtJambiLink::m_flags.testFlag(QtJambiLink::Flag::JavaObjectIsReleased) || m_pointer) {
        try{
            if(JniEnvironment env = QtJambiLink::isQThread() ? JniEnvironment{300} : DefaultJniEnvironment{300}){
                QtJambiLink::releaseJavaObject(env);
                if(QtJambiLink::isInitialized())
                    deleteNativeObject(env);
                QtJambiLink::detachJavaLink(env);
            }else if(QtJambiLink::isInitialized()){
                deleteNativeObject();
            }
        } catch (const std::exception& e) {
            qCWarning(DebugAPI::internalCategory, "%s", e.what());
        } catch (...) {
        }
    }
}

template<template<typename> class SmartPointer>
typename SmartPointerLink<SmartPointer,QObject>::SharedPointerT QObjectSmartPointerLink<SmartPointer>::getSmartPointer() const {
    if(m_pointer)
        return SmartPointerLink<SmartPointer,QObject>::getSmartPointer();
    else return{};
}

template<>
QSharedPointer<QObject> QObjectSmartPointerLink<QWeakPointer>::getSmartPointer() const {
    if(m_pointer){
        QSharedPointer<QObject> result = SmartPointerLink<QWeakPointer,QObject>::getSmartPointer();
        if(!result)
            result = QSharedPointer<QObject>(m_pointer, LinkedQObjectDeleter<QWeakPointer>(*this));
        return result;
    }else return{};
}

template<>
std::shared_ptr<QObject> QObjectSmartPointerLink<std::weak_ptr>::getSmartPointer() const {
    if(m_pointer){
        std::shared_ptr<QObject> result = SmartPointerLink<std::weak_ptr,QObject>::getSmartPointer();
        if(!result)
            result = std::shared_ptr<QObject>(m_pointer, LinkedQObjectDeleter<std::weak_ptr>(*this));
        return result;
    }else return{};
}

template<template<typename> class SmartPointer>
void QObjectSmartPointerLink<SmartPointer>::deleteNativeObject(JNIEnv *env, bool){
    typedef typename std::conditional<std::is_same<SmartPointer<QObject>,QWeakPointer<QObject>>::value, QSharedPointer<QObject>,
                                      typename std::conditional<std::is_same<SmartPointer<QObject>,std::weak_ptr<QObject>>::value, std::shared_ptr<QObject>, SmartPointer<QObject>>::type>::type SharedPointerT;
    QObject* obj;
    SharedPointerT shared_pointer;
    {
        QtJambiLinkWriteLock locker;
        if constexpr(std::is_same<SmartPointer<QObject>,QWeakPointer<QObject>>::value
                || std::is_same<SmartPointer<QObject>,std::weak_ptr<QObject>>::value){
            shared_pointer = this->m_smartPointer.lock();
        }else{
            shared_pointer = this->m_smartPointer;
        }
        this->m_smartPointer = SmartPointer<QObject>();
        obj = m_pointer;
        m_pointer = nullptr;
    }
    if (obj && !isDeleteLater()){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this, "QtJambiLink::deleteNativeObject(JNIEnv *env, bool)")
        if(!this->isShell())
            this->clearAllDependencies(env);
        this->unregisterOffsets(obj);

        QThread *objectThread = obj->thread();

        // Explicit dispose from current thread, delete object
        if (!objectThread
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
                || objectThread->isCurrentThread()
#else
                || objectThread==QThread::currentThread()
#endif
                || !objectThread->isRunning()) {
//             printf(" - straight up delete %s [%s]\n",
//                    qPrintable(qobj->objectName()),
//                    qobj->metaObject()->className());
            QTJAMBI_DEBUG_PRINT_WITH_ARGS("call delete PointerContainer")
            if(objectThread && obj==objectThread){
                if(QThreadData::get2(objectThread)->isAdopted){
                    QTJAMBI_DEBUG_PRINT_WITH_ARGS("Adopted thread expected to be automatically deleted")
                    return;
                }
                if(static_cast<SelfDeletingThread*>(objectThread)->deleteLaterIfIsInFinish()){
                    setDeleteLater();
                    QTJAMBI_DEBUG_PRINT_WITH_ARGS("call QObject::deleteLater()")
                    return;
                }
            }
            {
                QtJambiExceptionBlocker __qt_exceptionBlocker;
                {
                    if(shared_pointer)
                        shared_pointer = SharedPointerT();
                    else
                        delete obj;
                }
                __qt_exceptionBlocker.release(env);
            }
        // We're in the main thread and we'll have an event loop
        // running, so its safe to call delete later.
        }else{
            if(qobject_cast<QThread*>(obj)){
                {
                    QtJambiExceptionBlocker __qt_exceptionBlocker;
                    {
                        if(shared_pointer)
                            shared_pointer = SharedPointerT();
                        else
                            delete obj;
                    }
                    __qt_exceptionBlocker.release(env);
                }
            }else{
                QThreadObjectData* threadData = QThreadObjectData::ensureThreadUserData(objectThread);
                setDeleteLater();
                QTJAMBI_DEBUG_PRINT_WITH_ARGS("attach QObject to thread shutdown procedure.")
                if(shared_pointer){
                    if (QAbstractEventDispatcher::instance(objectThread)) {
                        QCoreApplication::postEvent(threadData->threadDeleter(), new DestructorEvent(obj, *this, new SharedPointerT(shared_pointer), [](void * ptr,bool){delete reinterpret_cast<SharedPointerT*>(ptr);}, false));
                    }else{
                        threadData->doAtThreadEnd([shared_pointer]() mutable {
                            shared_pointer = SharedPointerT();
                        });
                    }
                }else{
                    if (QAbstractEventDispatcher::instance(objectThread)) {
                        obj->deleteLater();
                    }else{
                        threadData->doAtThreadEnd([obj]() mutable {
                            delete obj;
                        });
                    }
                }
            }
        }
    }
}

template<template<typename> class SmartPointer>
QObjectWithPendingExtraSignalsSmartPointerLink<SmartPointer>::QObjectWithPendingExtraSignalsSmartPointerLink(JNIEnv *env,
                                                                                                                 jobject nativeLink,
                                                                                                                 jobject jobj,
                                                                                                                 SafeBool created_by_java,
                                                                                                                 QtJambiLink::Ownership ownership,
                                                                                                                 SafeBool is_shell,
                                                                                                                 QObject* pointer,
                                                                                                                 SafeBool isQThread,
                                                                                                                 SmartPointer<QObject>*& smartPointer,
                                                                                                                 const QMetaObject* superTypeForCustomMetaObject,
                                                                                                                 JavaException& ocurredException)
    : ExtraSignalsLink<QObjectSmartPointerLink<SmartPointer>>(env, nativeLink,
                                LINK_NAME_ARG(superTypeForCustomMetaObject->className())
                                created_by_java, ownership, is_shell, pointer, smartPointer, ocurredException)
{
    jobject local = env->NewLocalRef(jobj);
    if(!ocurredException){
        QtJambiMetaObject::resolveSignals(env, jobj, superTypeForCustomMetaObject, ocurredException);
    }
    if(!ocurredException){
        bool _isValueOwner = isValueOwner(superTypeForCustomMetaObject);
        QtJambiLinkObjectData* lud{nullptr};
        ValueOwnerObjectData* vud{nullptr};
        {
            auto locker = QtJambiObjectData::readLock();
            if(_isValueOwner)
                vud = QtJambiObjectData::userData<ValueOwnerObjectData>(pointer);
            lud = QtJambiObjectData::userData<QtJambiLinkObjectData>(pointer);
        }
        if(_isValueOwner && !vud)
            vud = new ValueOwnerObjectData(pointer);
        if(lud) {
            QtJambiLink::resetDataLink(lud);
        }
        if(isQThread && pointer!=QCoreApplicationPrivate::theMainThread.loadRelaxed()){
            QThread* thread = static_cast<QThread*>(pointer);
            QSharedPointer<QtJambiLink> _this = *this;
            QThreadObjectData* data{nullptr};
            QtJambiLink::setIsQThread(true);
            if(lud){
                if(vud){
                    auto locker = QtJambiObjectData::writeLock();
                    data = QtJambiObjectData::userData<QThreadObjectData>(thread);
                    vud = QtJambiObjectData::setUserData<ValueOwnerObjectData>(pointer, vud);
                }else{
                    auto locker = QtJambiObjectData::readLock();
                    data = QtJambiObjectData::userData<QThreadObjectData>(thread);
                }
                lud = nullptr; // don't delete
            }else{
                lud = new ThreadLinkObjectData(thread, _this);
                {
                    auto locker = QtJambiObjectData::writeLock();
                    data = QtJambiObjectData::userData<QThreadObjectData>(thread);
                    lud = QtJambiObjectData::setUserData<QtJambiLinkObjectData>(thread, lud);
                    if(vud)
                        vud = QtJambiObjectData::setUserData<ValueOwnerObjectData>(pointer, vud);
                }
            }
            if(!data)
                ThreadPrivate::fromQThread(env, jobj, thread, _this);
        }else{
            if(lud) {
                if(vud){
                    auto locker = QtJambiObjectData::writeLock();
                    vud = QtJambiObjectData::setUserData<ValueOwnerObjectData>(pointer, vud);
                }
                lud = nullptr; // don't delete
            }else{
                lud = new LinkObjectData(*this);
                {
                    auto locker = QtJambiObjectData::writeLock();
                    lud = QtJambiObjectData::setUserData<QtJambiLinkObjectData>(pointer, lud);
                    if(vud)
                        vud = QtJambiObjectData::setUserData<ValueOwnerObjectData>(pointer, vud);
                }
            }
        }
        if(vud)
            delete vud;
        if(lud)
            delete lud;
    }else{
        this->m_flags.setFlag(QtJambiLink::Flag::IsJavaLinkDetached);
    }
    env->DeleteLocalRef(local);
}

template<template<typename> class SmartPointer>
PlainQObjectSmartPointerLink<SmartPointer>::PlainQObjectSmartPointerLink(JNIEnv *env,
                                                                             jobject nativeLink,
                                                                             jobject jobj,
                                                                             SafeBool created_by_java,
                                                                             QtJambiLink::Ownership ownership,
                                                                             SafeBool is_shell,
                                                                             QObject* pointer,
                                                                             SafeBool isQThread,
                                                                             SmartPointer<QObject>*& smartPointer,
                                                                             JavaException& ocurredException)
    : QObjectSmartPointerLink<SmartPointer>(env, nativeLink,
                                LINK_NAME_ARG(pointer->metaObject()->className())
                                created_by_java, ownership, is_shell, pointer, smartPointer, ocurredException)
{
    jobject local = env->NewLocalRef(jobj);
    const QMetaObject* metaObject = pointer->metaObject();
    if(!ocurredException){
        QtJambiMetaObject::resolveSignals(env, jobj, metaObject, ocurredException);
    }
    if(!ocurredException){
        bool _isValueOwner = isValueOwner(metaObject);
        QtJambiLinkObjectData* lud{nullptr};
        ValueOwnerObjectData* vud{nullptr};
        {
            auto locker = QtJambiObjectData::readLock();
            if(_isValueOwner)
                vud = QtJambiObjectData::userData<ValueOwnerObjectData>(pointer);
            lud = QtJambiObjectData::userData<QtJambiLinkObjectData>(pointer);
        }
        if(_isValueOwner && !vud)
            vud = new ValueOwnerObjectData(pointer);
        if(lud) {
            QtJambiLink::resetDataLink(lud);
        }
        if(isQThread && pointer!=QCoreApplicationPrivate::theMainThread.loadRelaxed()){
            QThread* thread = static_cast<QThread*>(pointer);
            QSharedPointer<QtJambiLink> _this = *this;
            QThreadObjectData* data{nullptr};
            QtJambiLink::setIsQThread(true);
            if(lud){
                if(vud){
                    auto locker = QtJambiObjectData::writeLock();
                    data = QtJambiObjectData::userData<QThreadObjectData>(thread);
                    vud = QtJambiObjectData::setUserData<ValueOwnerObjectData>(pointer, vud);
                }else{
                    auto locker = QtJambiObjectData::readLock();
                    data = QtJambiObjectData::userData<QThreadObjectData>(thread);
                }
                lud = nullptr; // don't delete
            }else{
                lud = new MetaObjectThreadLinkObjectData(thread, _this, metaObject);
                {
                    auto locker = QtJambiObjectData::writeLock();
                    data = QtJambiObjectData::userData<QThreadObjectData>(thread);
                    lud = QtJambiObjectData::setUserData<QtJambiLinkObjectData>(thread, lud);
                    if(vud)
                        vud = QtJambiObjectData::setUserData<ValueOwnerObjectData>(pointer, vud);
                }
            }
            if(!data)
                ThreadPrivate::fromQThread(env, jobj, thread, _this);
        }else{
            if(lud) {
                if(vud){
                    auto locker = QtJambiObjectData::writeLock();
                    vud = QtJambiObjectData::setUserData<ValueOwnerObjectData>(pointer, vud);
                }
                lud = nullptr; // don't delete
            }else{
                lud = new MetaObjectLinkObjectData(*this, metaObject);
                {
                    auto locker = QtJambiObjectData::writeLock();
                    lud = QtJambiObjectData::setUserData<QtJambiLinkObjectData>(pointer, lud);
                    if(vud)
                        vud = QtJambiObjectData::setUserData<ValueOwnerObjectData>(pointer, vud);
                }
            }
        }
        if(vud)
            delete vud;
        if(lud)
            delete lud;
    }else{
        this->m_flags.setFlag(QtJambiLink::Flag::IsJavaLinkDetached);
    }
    env->DeleteLocalRef(local);
}

namespace QtJambiPrivate{

template<template<typename> class SmartPointer>
struct QObjectLink{//QSharedPointer
    typedef SmartPointerLink<QSharedPointer,QObject> Shared;
    typedef SmartPointerLink<QWeakPointer,QObject> Weak;
};

template<>
struct QObjectLink<QWeakPointer>{
    typedef SmartPointerLink<QSharedPointer,QObject> Shared;
    typedef SmartPointerLink<QWeakPointer,QObject> Weak;
};

template<>
struct QObjectLink<std::shared_ptr>{
    typedef SmartPointerLink<std::shared_ptr,QObject> Shared;
    typedef SmartPointerLink<std::weak_ptr,QObject> Weak;
};

template<>
struct QObjectLink<std::weak_ptr>{
    typedef SmartPointerLink<std::shared_ptr,QObject> Shared;
    typedef SmartPointerLink<std::weak_ptr,QObject> Weak;
};

template<template<typename> class SmartPointer>
struct PointerLink{//QSharedPointer
    typedef SmartPointerLink<QSharedPointer,char> Shared;
    typedef SmartPointerLink<QWeakPointer,char> Weak;
};

template<>
struct PointerLink<std::shared_ptr>{
    typedef SmartPointerLink<std::shared_ptr,char> Shared;
    typedef SmartPointerLink<std::weak_ptr,char> Weak;
};

template<>
struct PointerLink<QWeakPointer>{
    typedef SmartPointerLink<QSharedPointer,char> Shared;
    typedef SmartPointerLink<QWeakPointer,char> Weak;
};

template<>
struct PointerLink<std::weak_ptr>{
    typedef SmartPointerLink<std::shared_ptr,char> Shared;
    typedef SmartPointerLink<std::weak_ptr,char> Weak;
};

template<template<typename> class SmartPointer>
bool convertToNativeSmartPointer(const QSharedPointer<QtJambiLink>& link, SmartPointer<QObject>& output){
    if(link->isSmartPointer()){
        switch(link->smartPointerType()){
        case SmartPointerUtility<SmartPointer>::SharedPointerFlag:
            if(link->isQObject()){
                auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<QObject>(static_cast<typename QObjectLink<SmartPointer>::Shared*>(link.data())->getSmartPointer());
                output = sp;
            }else{
                auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(static_cast<typename PointerLink<SmartPointer>::Shared*>(link.data())->getSmartPointer());
                output = SmartPointerUtility<SmartPointer>::template SharedPointer<QObject>(sp, reinterpret_cast<QObject*>(link->pointer()));
            }
            return true;
        case SmartPointerUtility<SmartPointer>::WeakPointerFlag:
            if(link->isQObject()){
                auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<QObject>(static_cast<typename QObjectLink<SmartPointer>::Weak*>(link.data())->getSmartPointer());
                output = sp;
            }else{
                auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(static_cast<typename PointerLink<SmartPointer>::Weak*>(link.data())->getSmartPointer());
                output = SmartPointerUtility<SmartPointer>::template SharedPointer<QObject>(sp, reinterpret_cast<QObject*>(link->pointer()));
            }
            return true;
        default:
            break;
        }
    }
    return false;
}

bool convertToNativeSmartPointer(const QSharedPointer<QtJambiLink>& link, QSharedPointer<QObject>& output){
    return convertToNativeSmartPointer<QSharedPointer>(link, output);
}
bool convertToNativeSmartPointer(const QSharedPointer<QtJambiLink>& link, QWeakPointer<QObject>& output){
    return convertToNativeSmartPointer<QWeakPointer>(link, output);
}
bool convertToNativeSmartPointer(const QSharedPointer<QtJambiLink>& link, std::shared_ptr<QObject>& output){
    return convertToNativeSmartPointer<std::shared_ptr>(link, output);
}
bool convertToNativeSmartPointer(const QSharedPointer<QtJambiLink>& link, std::weak_ptr<QObject>& output){
    return convertToNativeSmartPointer<std::weak_ptr>(link, output);
}

template<template<typename> class SmartPointer>
bool convertToNativeSmartPointer(const QSharedPointer<QtJambiLink>& link, SmartPointer<char>& output, const std::type_info* typeId = nullptr){
    if(link->isSmartPointer()){
        if(typeId){
            void *ptr{nullptr};
            switch(link->smartPointerType()){
            case SmartPointerUtility<SmartPointer>::SharedPointerFlag:
                if(link->isQObject()){
                    auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<QObject>(static_cast<typename QObjectLink<SmartPointer>::Shared*>(link.data())->getSmartPointer());
                    ptr = link->typedPointer(*typeId);
                    if(ptr){
                        output = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(ptr));
                    }else{
                        output = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(link->pointer()));
                    }
                }else{
                    auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(static_cast<typename PointerLink<SmartPointer>::Shared*>(link.data())->getSmartPointer());
                    ptr = link->typedPointer(*typeId);
                    if(ptr){
                        output = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(ptr));
                    }else{
                        output = sp;
                    }
                }
                return true;
            case SmartPointerUtility<SmartPointer>::WeakPointerFlag:
                if(link->isQObject()){
                    auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<QObject>(static_cast<typename QObjectLink<SmartPointer>::Weak*>(link.data())->getSmartPointer());
                    ptr = link->typedPointer(*typeId);
                    if(ptr){
                        output = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(ptr));
                    }else{
                        output = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(link->pointer()));
                    }
                }else{
                    auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(static_cast<typename PointerLink<SmartPointer>::Weak*>(link.data())->getSmartPointer());
                    ptr = link->typedPointer(*typeId);
                    if(ptr){
                        output = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(ptr));
                    }else{
                        output = sp;
                    }
                }
                return true;
            default:
                break;
            }
        }else{
            switch(link->smartPointerType()){
            case SmartPointerUtility<SmartPointer>::SharedPointerFlag:
                if(link->isQObject()){
                    auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<QObject>(static_cast<typename QObjectLink<SmartPointer>::Shared*>(link.data())->getSmartPointer());
                    output = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(link->pointer()));
                }else{
                    auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(static_cast<typename PointerLink<SmartPointer>::Shared*>(link.data())->getSmartPointer());
                    output = sp;
                }
                return true;
            case SmartPointerUtility<SmartPointer>::WeakPointerFlag:
                if(link->isQObject()){
                    auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<QObject>(static_cast<typename QObjectLink<SmartPointer>::Weak*>(link.data())->getSmartPointer());
                    output = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(link->pointer()));
                }else{
                    auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(static_cast<typename PointerLink<SmartPointer>::Weak*>(link.data())->getSmartPointer());
                    output = sp;
                }
                return true;
            default:
                break;
            }
        }
    }
    return false;
}

bool convertToNativeSmartPointer(const QSharedPointer<QtJambiLink>& link, QSharedPointer<char>& output, const std::type_info* typeId){
    return convertToNativeSmartPointer<QSharedPointer>(link, output, typeId);
}
bool convertToNativeSmartPointer(const QSharedPointer<QtJambiLink>& link, QWeakPointer<char>& output, const std::type_info* typeId){
    return convertToNativeSmartPointer<QWeakPointer>(link, output, typeId);
}
bool convertToNativeSmartPointer(const QSharedPointer<QtJambiLink>& link, std::shared_ptr<char>& output, const std::type_info* typeId){
    return convertToNativeSmartPointer<std::shared_ptr>(link, output, typeId);
}
bool convertToNativeSmartPointer(const QSharedPointer<QtJambiLink>& link, std::weak_ptr<char>& output, const std::type_info* typeId){
    return convertToNativeSmartPointer<std::weak_ptr>(link, output, typeId);
}

}
