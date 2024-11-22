/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include "qtjambiapi.h"
#include "debugapi.h"
#include "java_p.h"
#include "qtjambilink_p.h"

#include "registryutil_p.h"
#include "threadutils_p.h"
#include "qtjambimetaobject_p.h"
#include "jnienvironment.h"
#include "containeraccess_p.h"

#include <algorithm>
#include <list>
#include <cstring>
#include <QDebug>
#include <QHash>
#include <QReadWriteLock>
#include <QByteArray>
#include <QThread>
#include <QWriteLocker>
#include <QCoreApplication>
#include <QAbstractEventDispatcher>
#include <QtCore/private/qobject_p.h>
#include <QtCore/private/qmetaobject_p.h>
#include <QtCore/private/qobject_p.h>

#if defined(QTJAMBI_DEBUG_TOOLS)
// Allow useful truncation on 32bit
#define QTJAMBI_LINK_MAGIC (long(0xf6f700fff5f42f04ll))

#define QTJAMBI_REGISTER_LINK registerLink(this);
#define QTJAMBI_UNREGISTER_LINK unregisterLink(this);
#define QTJAMBI_UNREGISTER_LINK_FINALLY if(this->m_flags.testFlag(QtJambiLink::Flag::IsListed)) unregisterLink(this);
#else
#define QTJAMBI_REGISTER_LINK
#define QTJAMBI_UNREGISTER_LINK
#define QTJAMBI_UNREGISTER_LINK_FINALLY
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
#define qAsConst std::as_const
#endif

#define AVAILABLE_IN_DELETION

struct QObjectInDeletion{
    QObjectInDeletion(JNIEnv* env, QtJambiLink* link);
    ~QObjectInDeletion();
    static void disposeQObject(QObject* pointer);
private:
#ifdef AVAILABLE_IN_DELETION
#endif
};

QObjectInDeletion::QObjectInDeletion(JNIEnv* env, QtJambiLink* link)
{
    Q_UNUSED(env)
    Q_UNUSED(link)
}

QObjectInDeletion::~QObjectInDeletion(){
}

void QObjectInDeletion::disposeQObject(QObject* pointer){
    Q_UNUSED(pointer)
}

namespace DebugAPI{
void printCleanupArgs(const char *file, int line, const char *function, const char *format,...);
}

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
#include "qtjambi_cast.h"

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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#define MutexLocker QMutexLocker
#else
#define MutexLocker QMutexLocker<QMutex>
#endif

class SelfDeletingThread : public QThread
{
    Q_DECLARE_PRIVATE(QThread)
public:
    inline bool deleteLaterIfIsInFinish(){
        MutexLocker locker(&d_func()->mutex);
        if(d_func()->isInFinish){
            this->deleteLater();
            return true;
        }else{
            return false;
        }
    }
};

Q_GLOBAL_STATIC(const QSharedPointer<QtJambiLink>, gDefaultPointer);

class AbstractDestructionEvent : public AbstractThreadEvent
{
protected:
    AbstractDestructionEvent(const QPointer<const QObject>& parent, const QSharedPointer<QtJambiLink>& link, void *pointer);
    AbstractDestructionEvent(const AbstractDestructionEvent& clone);
private:
    void execute() override;
    virtual void destruct(void * pointer, const QSharedPointer<QtJambiLink>& link) = 0;
private:
    QPointer<const QObject> m_parent;
    QWeakPointer<QtJambiLink> m_link;
    void * m_pointer;
};

AbstractDestructionEvent::AbstractDestructionEvent(const QPointer<const QObject>& parent, const QSharedPointer<QtJambiLink>& link, void *pointer)
    : AbstractThreadEvent(),
      m_parent(parent),
      m_link(link.toWeakRef()),
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
        if(m_parent && (parentThread = m_parent->thread())!=QThread::currentThread()){
            QThreadUserData* threadData = QThreadUserData::ensureThreadUserData(parentThread);
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
    DestructorEvent(const QPointer<const QObject>& parent, const QSharedPointer<QtJambiLink>& link, void *pointer, PtrDeleterFunction deleter_function, bool isShell);
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
    ContainerDestructionEvent(const QPointer<const QObject>& parent, const QSharedPointer<QtJambiLink>& link, void *pointer, AbstractContainerAccess * containerAccess);
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
    MetaTypeDestructionEvent(const QPointer<const QObject>& parent, const QSharedPointer<QtJambiLink>& link, void *pointer, const QMetaType& meta_type);
private:
    MetaTypeDestructionEvent(const MetaTypeDestructionEvent& clone);
    void destruct(void * pointer, const QSharedPointer<QtJambiLink>& link) override;
    MetaTypeDestructionEvent* clone() const override;
    QMetaType m_meta_type;
};

DestructorEvent::DestructorEvent(const QPointer<const QObject>& parent, const QSharedPointer<QtJambiLink>& link, void *pointer, PtrDeleterFunction destructor_function, bool isShell)
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

MetaTypeDestructionEvent::MetaTypeDestructionEvent(const QPointer<const QObject>& parent, const QSharedPointer<QtJambiLink>& link, void *pointer, const QMetaType& meta_type)
    : AbstractDestructionEvent(parent, link, pointer),
      m_meta_type(meta_type
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                  .id()
#endif
                  )
{
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(link, "new MetaTypeDestructionEvent()")
}

MetaTypeDestructionEvent::MetaTypeDestructionEvent(const MetaTypeDestructionEvent& clone)
    : AbstractDestructionEvent(clone),
      m_meta_type(clone.m_meta_type
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                  .id()
#endif
                  )
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

ContainerDestructionEvent::ContainerDestructionEvent(const QPointer<const QObject>& parent, const QSharedPointer<QtJambiLink>& link, void *pointer, AbstractContainerAccess * containerAccess)
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

#define THREAD_ID() reinterpret_cast<void*>(QThread::currentThreadId())

#if defined(QTJAMBI_DEBUG_TOOLS)
Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gStaticQtJambiLinkListLock, (QReadWriteLock::Recursive));
QReadWriteLock* staticQtJambiLinkListLock(){
    return gStaticQtJambiLinkListLock();
}
typedef SecureContainer<QSet<QtJambiLink*>,QReadWriteLock,&staticQtJambiLinkListLock> QtJambiLinkList;
Q_GLOBAL_STATIC(QtJambiLinkList, gQtJambiLinkList);

void QtJambiLink::registerLink(QtJambiLink* link)
{
    if(link->m_flags.testFlag(Flag::IsListed)) {
        char buf[24];
        fprintf(stderr, "QtJambiLink::registerLink(%p) ERROR: LINK ALREADY LISTED, flags=%s, qtType=%s\n", reinterpret_cast<void*>(link), link->debugFlagsToString(buf), link->qtTypeName());
        return;
    }

    {
        QWriteLocker lock(gStaticQtJambiLinkListLock());
        gQtJambiLinkList->insert(link);
        link->m_flags.setFlag(Flag::IsListed);
    }
}

void QtJambiLink::unregisterLink(QtJambiLink* link)
{
    if(!link->m_flags.testFlag(Flag::IsListed)) {
        char buf[24];
        fprintf(stderr, "QtJambiLink::unregisterLink(%p) ERROR: LINK NOT LISTED, flags=%s, qtType=%s\n", reinterpret_cast<void*>(link), link->debugFlagsToString(buf), link->qtTypeName());
        return;
    }

    if(!gQtJambiLinkList.isDestroyed()){
        QWriteLocker lock(gStaticQtJambiLinkListLock());
        gQtJambiLinkList->remove(link);
        link->m_flags.setFlag(Flag::IsListed, false);
    }
}

/* static */
int QtJambiLink::linkCount()
{
    QReadLocker lock(gStaticQtJambiLinkListLock());
    return gQtJambiLinkList->size();
}

/* static */
int QtJambiLink::dumpLinks(JNIEnv * env)
{
    int count = 0;
    QtJambiLinkList set;
    {
        QReadLocker lock(gStaticQtJambiLinkListLock());
        set = *gQtJambiLinkList;
    }
    for(QtJambiLink* _link : qAsConst(set)) {
        QSharedPointer<QtJambiLink> sharedPointer = _link->m_this;
        if(!sharedPointer)
            continue;
        jobject obj = _link->getJavaObjectLocalRef(env);
        QString jclassName = obj ? QtJambiAPI::getObjectClassName(env, obj) : QLatin1String("null");
        jobject nativeLink = _link->nativeLink(env);
        jlong native_link = nativeLink ? Java::QtJambi::NativeUtility$NativeLink::nativeId(env, nativeLink) : 0;
        char buf[24];
        if(_link->isQObject()){
            QString objectName;
            bool hasParent = false;
            QThread* objectThread = nullptr;
            if(_link->qobject()){
                objectName = _link->qobject()->objectName();
                objectThread = _link->qobject()->thread();
                hasParent = _link->qobject()->parent();
                QThread* objectAsThread = qobject_cast<QThread*>(_link->qobject());
                if(objectAsThread && objectAsThread->isRunning())
                    continue;
            }
            fprintf(stderr, "QtJambiLink(%p) ALIVE: { java_object=%p (%s), java_native_link=%p (%s), ownership=%s, is_qobject=true, pointer=%p, delete_later=%s, qtType=%s, objectName='%s', hasParent=%s, object_thread='%s', flags=%s }\n",
                    reinterpret_cast<void*>(_link),
                    static_cast<void*>(obj), qPrintable(jclassName),
                    static_cast<void*>(nativeLink),
                    nativeLink ? qPrintable(QString("%1").arg((native_link==jlong(_link) ? "attached" : (native_link==0 ? "detached" : "dangled")))) : "detached",
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
                    nativeLink ? qPrintable(QString("%1").arg((native_link==jlong(_link) ? "attached" : (native_link==0 ? "detached" : "dangled")))) : "detached",
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

bool QtJambiLink::hasDeleteLaterEvents()
{
    QtJambiLinkList set;
    {
        QReadLocker lock(gStaticQtJambiLinkListLock());
        set = *gQtJambiLinkList;
    }
    for(QtJambiLink* _link : qAsConst(set)) {
        QSharedPointer<QtJambiLink> linkPtr = _link->m_this;
        if(!linkPtr)
            continue;
        if(_link->isQObject()){
            if(_link->pointer()==QCoreApplicationPrivate::theMainThread.loadRelaxed())
                continue;
            if(_link->isDeleteLater() && _link->qobject() && _link->qobject()->thread()==QThread::currentThread()){
                return true;
            }
        }
    }
    return false;
}

/* static */
bool QtJambiLink::checkLink(QtJambiLink* find)
{
    bool found = false;
    if(find) {
        QReadLocker lock(gStaticQtJambiLinkListLock());
        return gQtJambiLinkList->contains(find);
    }
    if(!found)
        fprintf(stderr, "QtJambiLink::QtJambiLinkList_check(%p): FAILED\n", static_cast<void*>(find));
    return found;
}

#endif

Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gLinkAccessLock, (QReadWriteLock::Recursive))
QReadWriteLock* linkAccessLock(){
    return gLinkAccessLock();
}
typedef SecureContainer<QHash<const void *, QWeakPointer<QtJambiLink> >,QReadWriteLock,&linkAccessLock> LinkHash;
typedef SecureContainer<QMultiHash<const void *, QWeakPointer<QtJambiLink> >,QReadWriteLock,&linkAccessLock> MultiLinkHash;
Q_GLOBAL_STATIC(MultiLinkHash, gUserObjectCache)
Q_GLOBAL_STATIC(LinkHash, gQObjectCache)

QtJambiLinkUserData::QtJambiLinkUserData(const QWeakPointer<QtJambiLink> & link)
  :
    m_link(link)
 { }

void QtJambiLink::resetDataLink(QtJambiLinkUserData* data){
    data->resetLink(getWeakPointer());
}

QtJambiMetaObjectLinkUserData::QtJambiMetaObjectLinkUserData(const QWeakPointer<QtJambiLink> & link, const QMetaObject* metaObject)
  :
    QtJambiLinkUserData(link),
    m_metaObject(metaObject)
 { }

QReadWriteLock* QtJambiLinkUserData::lock()
{
    return gLinkAccessLock;
}

QtJambiThreadLinkUserData::~QtJambiThreadLinkUserData(){
#ifdef Q_OS_ANDROID
    QSharedPointer<QtJambiLink> link = m_link.toStrongRef();
    m_link.clear();
    if (link && link->isQObject()) {
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(link, "QtJambiLinkUserData::~QtJambiLinkUserData()")
        QTJAMBI_INCREASE_COUNTER(userDataDestroyedCount, link)
        if(link->isSmartPointer()){
            switch(link->smartPointerType()){
            case QtJambiLink::Flag::QSharedPointer:
                reinterpret_cast<SmartPointerToQObjectLink<QSharedPointer>*>(link.data())->setAsQObjectDeleted();
                break;
            case QtJambiLink::Flag::QWeakPointer:
                reinterpret_cast<SmartPointerToQObjectLink<QWeakPointer>*>(link.data())->setAsQObjectDeleted();
                break;
            case QtJambiLink::Flag::shared_ptr:
                reinterpret_cast<SmartPointerToQObjectLink<std::shared_ptr>*>(link.data())->setAsQObjectDeleted();
                break;
            case QtJambiLink::Flag::weak_ptr:
                reinterpret_cast<SmartPointerToQObjectLink<std::weak_ptr>*>(link.data())->setAsQObjectDeleted();
                break;
            default: break;
            }
        }else{
            reinterpret_cast<PointerToQObjectLink*>(link.data())->setAsQObjectDeleted();
        }
        if(QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed()){
            class LinkDeleter : public QObject{
            public:
                LinkDeleter(QSharedPointer<QtJambiLink>&& link) : QObject(), m_link(std::move(link)) {}
                bool event(QEvent* event) override {
                    m_link.clear();
                    return QObject::event(event);
                }
                QSharedPointer<QtJambiLink> m_link;
            };
            LinkDeleter* deleter = new LinkDeleter(std::move(link));
            deleter->moveToThread(mainThread);
            deleter->deleteLater();
        }
    }
#endif
}

QTJAMBI_OBJECTUSERDATA_ID_IMPL(,QtJambiLinkUserData::)

QtJambiLinkUserData::~QtJambiLinkUserData()
{
    try{
        QSharedPointer<QtJambiLink> link = m_link.toStrongRef();
        m_link.clear();
        if (link && link->isQObject()) {
            QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(link, "QtJambiLinkUserData::~QtJambiLinkUserData()")
            QTJAMBI_INCREASE_COUNTER(userDataDestroyedCount, link)
            if(link->isSmartPointer()){
                switch(link->smartPointerType()){
                case QtJambiLink::Flag::QSharedPointer:
                    reinterpret_cast<SmartPointerToQObjectLink<QSharedPointer>*>(link.data())->setAsQObjectDeleted();
                    break;
                case QtJambiLink::Flag::QWeakPointer:
                    reinterpret_cast<SmartPointerToQObjectLink<QWeakPointer>*>(link.data())->setAsQObjectDeleted();
                    break;
                case QtJambiLink::Flag::shared_ptr:
                    reinterpret_cast<SmartPointerToQObjectLink<std::shared_ptr>*>(link.data())->setAsQObjectDeleted();
                    break;
                case QtJambiLink::Flag::weak_ptr:
                    reinterpret_cast<SmartPointerToQObjectLink<std::weak_ptr>*>(link.data())->setAsQObjectDeleted();
                    break;
                default: break;
                }
            }else{
                reinterpret_cast<PointerToQObjectLink*>(link.data())->setAsQObjectDeleted();
            }
            // only reset the link when not a shell. Shells do this operation during shell destruction.
            // => QtJambiShellImpl::deleteShell()
            if(!link->isShell()){
                if(link->noThreadInitializationOnPurge()){
                    if(DefaultJniEnvironment env{200})
                        link->invalidate(env);
                }else{
                    if(JniEnvironment env{200})
                        link->invalidate(env);
                }
            }
        }else{
            QWriteLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
        }
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
    } catch (...) {
    }
}

void QtJambiLinkUserData::clear(){
    QSharedPointer<QtJambiLink> link = m_link.toStrongRef();
    m_link.clear();
    if (link && link->isQObject()) {
        QTJAMBI_INCREASE_COUNTER(userDataDestroyedCount, link)
    }
}

QTJAMBI_OBJECTUSERDATA_ID_IMPL(,ValueOwnerUserData::)

ValueOwnerUserData::ValueOwnerUserData(const QObject* object) : QtJambiObjectData(), m_object(object) {

}

ValueOwnerUserData::~ValueOwnerUserData(){
    while(!m_deleters.isEmpty()){
        try{
            m_deleters.takeFirst()();
        } catch (const std::exception& e) {
            qCWarning(DebugAPI::internalCategory, "%s", e.what());
        } catch (...) {
        }
    }
}

void ValueOwnerUserData::addDeleter(const QtJambiUtils::Runnable& deleter){
    m_deleters << deleter;
}

void ValueOwnerUserData::addDeleter(QtJambiUtils::Runnable&& deleter){
    m_deleters << std::move(deleter);
}

static volatile int sObjectCacheMode = 2;
int QtJambiLink::getObjectCacheMode()
{
    return sObjectCacheMode;
}

void QtJambiLink::setObjectCacheMode(int object_cache_mode)
{
    sObjectCacheMode = object_cache_mode;
}

int QtJambiLink::getObjectCacheOperationCount()
{
    int count;
    {
        QWriteLocker lock(gLinkAccessLock());
        Q_UNUSED(lock)
        count = int(gUserObjectCache()->count());
    }
    return count;
}

void QtJambiLink::registerQObjectInitialization(void *ptr, const QSharedPointer<QtJambiLink>& link) {
    if(QtJambiLink::getObjectCacheMode() != 0) {
        QWriteLocker locker(gLinkAccessLock());
        Q_UNUSED(locker)
        Q_ASSERT(gQObjectCache());
        gQObjectCache->insert(ptr, link.toWeakRef());
    }
}

void QtJambiLink::unregisterQObjectInitialization(void *ptr) {
    if(QtJambiLink::getObjectCacheMode() != 0) {
        QWriteLocker locker(gLinkAccessLock());
        Q_UNUSED(locker)
        if(LinkHash* h = gQObjectCache())
            h->remove(ptr);
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForQObject(JNIEnv *env, jobject javaObject, QObject *object, bool created_by_java, bool is_shell, const QMetaObject* superTypeForCustomMetaObject)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(object);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(javaObject));
    JavaException ocurredException;
    QtJambiLink* qtJambiLink;
    if(superTypeForCustomMetaObject){
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            qtJambiLink = new PointerToPendingQObjectInterfaceLink(env, nativeLink, javaObject, *interfaceOffsets, superTypeForCustomMetaObject, object, created_by_java, false, is_shell, ocurredException);
        }else{
            qtJambiLink = new PointerToPendingQObjectLink(env, nativeLink, javaObject, superTypeForCustomMetaObject, object, created_by_java, false, is_shell, ocurredException);
        }
    }else{
        const QMetaObject* metaObject = object->metaObject();
        QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(metaObject);
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            if(extraSignals.isEmpty())
                qtJambiLink = new PointerToQObjectInterfaceLink(env, nativeLink, javaObject, *interfaceOffsets, metaObject, object, created_by_java, false, is_shell, ocurredException);
            else
                qtJambiLink = new PointerToQObjectInterfaceWithExtraSignalsLink(env, nativeLink, javaObject, *interfaceOffsets, metaObject, object, created_by_java, false, is_shell, ocurredException);
        }else{
            if(extraSignals.isEmpty())
                qtJambiLink = new PointerToQObjectLink(env, nativeLink, javaObject, metaObject, object, created_by_java, false, is_shell, ocurredException);
            else
                qtJambiLink = new PointerToQObjectWithExtraSignalsLink(env, nativeLink, javaObject, metaObject, object, created_by_java, false, is_shell, ocurredException);
        }
    }
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    //qtJambiLink->setCppOwnership(env);

    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeQObject(JNIEnv *env, jobject javaObject, QObject *object, bool created_by_java, bool is_shell, const QMetaObject* superTypeForCustomMetaObject)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(object);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink;
    if(superTypeForCustomMetaObject){
            qtJambiLink = new PointerToPendingQObjectLink(env, nativeLink, javaObject, superTypeForCustomMetaObject, object, created_by_java, false, is_shell, ocurredException);
    }else{
        const QMetaObject* metaObject = object->metaObject();
        QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(metaObject);
        if(extraSignals.isEmpty())
            qtJambiLink = new PointerToQObjectLink(env, nativeLink, javaObject, metaObject, object, created_by_java, false, is_shell, ocurredException);
        else
            qtJambiLink = new PointerToQObjectWithExtraSignalsLink(env, nativeLink, javaObject, metaObject, object, created_by_java, false, is_shell, ocurredException);
    }
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeQObject(JNIEnv *env, jobject javaObject, QObject *object, bool created_by_java,
                                                                           bool is_shell, const QMetaObject* superTypeForCustomMetaObject,
                                                                           const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(object);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink;
    if(superTypeForCustomMetaObject){
        qtJambiLink = new PointerToPendingQObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsetInfo, superTypeForCustomMetaObject, object, created_by_java, false, is_shell, ocurredException);
    }else{
        const QMetaObject* metaObject = object->metaObject();
        QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(metaObject);
        if(extraSignals.isEmpty())
            qtJambiLink = new PointerToQObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsetInfo, metaObject, object, created_by_java, false, is_shell, ocurredException);
        else
            qtJambiLink = new PointerToQObjectInterfaceWithExtraSignalsLink(env, nativeLink, javaObject, interfaceOffsetInfo, metaObject, object, created_by_java, false, is_shell, ocurredException);
    }
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewQObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, const QMetaObject* metaObject, QObject *object, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool isDeclarativeCall, bool is_shell, bool hasCustomMetaObject, JavaException& ocurredException)
{
    if(!ocurredException){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(object);
        Q_ASSERT(metaObject);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        QtJambiLink* qtJambiLink;
        if(hasCustomMetaObject){
            if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
                qtJambiLink = new PointerToPendingQObjectInterfaceLink(env, nativeLink, javaObject, *interfaceOffsets, metaObject, object, created_by_java, isDeclarativeCall, is_shell, ocurredException);
            }else{
                qtJambiLink = new PointerToPendingQObjectLink(env, nativeLink, javaObject, metaObject, object, created_by_java, isDeclarativeCall, is_shell, ocurredException);
            }
        }else{
            QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(metaObject);
            if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
                if(extraSignals.isEmpty()){
                    qtJambiLink = new PointerToQObjectInterfaceLink(env, nativeLink, javaObject, *interfaceOffsets, metaObject, object, created_by_java, isDeclarativeCall, is_shell, ocurredException);
                }else{
                    qtJambiLink = new PointerToQObjectInterfaceWithExtraSignalsLink(env, nativeLink, javaObject, *interfaceOffsets, metaObject, object, created_by_java, isDeclarativeCall, is_shell, ocurredException);
                }
            }else{
                if(extraSignals.isEmpty()){
                    qtJambiLink = new PointerToQObjectLink(env, nativeLink, javaObject, metaObject, object, created_by_java, isDeclarativeCall, is_shell, ocurredException);
                }else{
                    qtJambiLink = new PointerToQObjectWithExtraSignalsLink(env, nativeLink, javaObject, metaObject, object, created_by_java, isDeclarativeCall, is_shell, ocurredException);
                }
            }
        }
        if(ocurredException){
            qtJambiLink->dispose();
        }else{
            return qtJambiLink->getStrongPointer();
        }
    }
    return *gDefaultPointer;
}

template<template<typename> class SmartPointer>
const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject, bool created_by_java, bool is_shell, const SmartPointer<QObject>& smartPointer,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                   const QMetaObject* superTypeForCustomMetaObject)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(smartPointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    QObject* object = smartPointer.get();
    Q_ASSERT(object);
    JavaException ocurredException;
    SmartPointerToQObjectLink<SmartPointer>* qtJambiLink;
    SmartPointer<QObject>* smartPointerTarget{nullptr};
    if(superTypeForCustomMetaObject){
        qtJambiLink = new SmartPointerToQObjectInterfaceWithPendingExtraSignalsLink(env, nativeLink, javaObject, interfaceOffsetInfo, created_by_java, QtJambiLink::Ownership::None, is_shell, object, smartPointerTarget, superTypeForCustomMetaObject, ocurredException);
    }else{
        const QMetaObject* metaObject = object->metaObject();
        QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(metaObject);
        if(extraSignals.isEmpty())
            qtJambiLink = new SmartPointerToPlainQObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsetInfo, created_by_java, QtJambiLink::Ownership::None, is_shell, object, smartPointerTarget, ocurredException);
        else
            qtJambiLink = new SmartPointerToQObjectInterfaceWithExtraSignalsLink(env, nativeLink, javaObject, interfaceOffsetInfo, created_by_java, QtJambiLink::Ownership::None, is_shell, object, smartPointerTarget, ocurredException);
    }
    *smartPointerTarget = smartPointer;
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    if(qtJambiLink->qobject()->parent())
            qtJambiLink->setCppOwnership(env);
        else
            qtJambiLink->setJavaOwnership(env);
    return qtJambiLink->getStrongPointer();
}

template<template<typename> class SmartPointer>
const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject, bool created_by_java, bool is_shell, const SmartPointer<QObject>& smartPointer, const QMetaObject* superTypeForCustomMetaObject)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(smartPointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    QObject* object = smartPointer.get();
    Q_ASSERT(object);
    JavaException ocurredException;
    SmartPointerToQObjectLink<SmartPointer>* qtJambiLink;
    SmartPointer<QObject>* smartPointerTarget{nullptr};
    if(superTypeForCustomMetaObject){
        qtJambiLink = new SmartPointerToQObjectWithPendingExtraSignalsLink(env, nativeLink, javaObject, created_by_java, QtJambiLink::Ownership::None, is_shell, object, smartPointerTarget, superTypeForCustomMetaObject, ocurredException);
    }else{
        QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(object->metaObject());
        if(extraSignals.isEmpty())
            qtJambiLink = new SmartPointerToPlainQObjectLink(env, nativeLink, javaObject, created_by_java, QtJambiLink::Ownership::None, is_shell, object, smartPointerTarget, ocurredException);
        else
            qtJambiLink = new SmartPointerToQObjectWithExtraSignalsLink(env, nativeLink, javaObject, created_by_java, QtJambiLink::Ownership::None, is_shell, object, smartPointerTarget, ocurredException);
    }
    *smartPointerTarget = smartPointer;
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    if(qtJambiLink->qobject()->parent())
            qtJambiLink->setCppOwnership(env);
        else
            qtJambiLink->setJavaOwnership(env);
    return qtJambiLink->getStrongPointer();
}

template<template<typename> class SmartPointer>
const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject javaObject, bool created_by_java,
                                                                                   QtJambiLink::Ownership ownership, bool is_shell, QObject* object,
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
    SmartPointerToQObjectLink<SmartPointer>* qtJambiLink;
    SmartPointer<QObject>* smartPointerTarget{nullptr};
    if(superTypeForCustomMetaObject){
        qtJambiLink = new SmartPointerToQObjectInterfaceWithPendingExtraSignalsLink(env, nativeLink, javaObject, interfaceOffsetInfo, created_by_java, ownership, is_shell, object, smartPointerTarget, superTypeForCustomMetaObject, ocurredException);
    }else{
        const QMetaObject* metaObject = object->metaObject();
        QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(metaObject);
        if(extraSignals.isEmpty())
            qtJambiLink = new SmartPointerToPlainQObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsetInfo, created_by_java, ownership, is_shell, object, smartPointerTarget, ocurredException);
        else
            qtJambiLink = new SmartPointerToQObjectInterfaceWithExtraSignalsLink(env, nativeLink, javaObject, interfaceOffsetInfo, created_by_java, ownership, is_shell, object, smartPointerTarget, ocurredException);
    }
    *smartPointerTarget = smartPointer;
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    if constexpr(std::is_same<SmartPointer<QObject>,QWeakPointer<QObject>>::value || std::is_same<SmartPointer<QObject>,std::weak_ptr<QObject>>::value){
        qtJambiLink->QtJambiLink::setJavaOwnership(env);
    }else{
        qtJambiLink->QtJambiLink::setCppOwnership(env);
    }
    return qtJambiLink->getStrongPointer();
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
                    link->QtJambiLink::setJavaOwnership(env);
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    int elementMetaType;
#else
    QMetaType elementMetaType;
#endif
    SmartMetaTypeDeleter(const QMetaType& _elementMetaType, QWeakPointer<SmartPointerLink<SmartPointer,char>>&& link)
        : SmartPointerDeleter<SmartPointer,char>(std::move(link)), elementMetaType(_elementMetaType
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                          .id()
                      #endif
                          )
    {
    }
    void operator()(char* ptr){
        if(SmartPointerDeleter<SmartPointer,char>::pushToJava())
            return;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        if(elementMetaType!=QMetaType::UnknownType)
            QMetaType::destroy(elementMetaType, ptr);
#else
        if(elementMetaType.isValid())
            elementMetaType.destroy(ptr);
#endif
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
const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject javaObject, bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, QObject* object, SmartPointer<QObject>& smartPointer, const QMetaObject* superTypeForCustomMetaObject)
{
    typedef typename std::conditional<std::is_same<SmartPointer<QObject>,QWeakPointer<QObject>>::value, QSharedPointer<QObject>,
                                      typename std::conditional<std::is_same<SmartPointer<QObject>,std::weak_ptr<QObject>>::value, std::shared_ptr<QObject>, SmartPointer<QObject>>::type>::type SharedPointerT;
    typedef typename std::conditional</*if*/std::is_same<SmartPointer<QObject>,QWeakPointer<QObject>>::value,
                                      /*then*/SmartPointerToQObjectLink<QSharedPointer>,
                                      /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<QObject>,std::weak_ptr<QObject>>::value,
                                                                        /*then*/SmartPointerToQObjectLink<std::shared_ptr>,
                                                                        /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<QObject>,QSharedPointer<QObject>>::value,
                                                                                                          /*then*/SmartPointerToQObjectLink<QWeakPointer>,
                                                                                                          /*else*/SmartPointerToQObjectLink<std::weak_ptr>>::type>::type>::type LinkType;
    typedef typename std::conditional</*if*/std::is_same<SmartPointer<QObject>,QWeakPointer<QObject>>::value,
                                      /*then*/QSharedPointer<QObject>,
                                      /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<QObject>,std::weak_ptr<QObject>>::value,
                                                                        /*then*/std::shared_ptr<QObject>,
                                                                        /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<QObject>,QSharedPointer<QObject>>::value,
                                                                                                          /*then*/QWeakPointer<QObject>,
                                                                                                          /*else*/std::weak_ptr<QObject>>::type>::type>::type TargetSmartPointer;
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(object);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    LinkType* qtJambiLink;
    TargetSmartPointer* smartPointerTarget{nullptr};
    if(superTypeForCustomMetaObject){
        qtJambiLink = new SmartPointerToQObjectWithPendingExtraSignalsLink(env, nativeLink, javaObject, created_by_java, ownership, is_shell, object, smartPointerTarget, superTypeForCustomMetaObject, ocurredException);
    }else{
        QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(object->metaObject());
        if(extraSignals.isEmpty())
            qtJambiLink = new SmartPointerToPlainQObjectLink(env, nativeLink, javaObject, created_by_java, ownership, is_shell, object, smartPointerTarget, ocurredException);
        else
            qtJambiLink = new SmartPointerToQObjectWithExtraSignalsLink(env, nativeLink, javaObject, created_by_java, ownership, is_shell, object, smartPointerTarget, ocurredException);
    }
    SharedPointerT sharedPointer;
    sharedPointer = SharedPointerT(object, LinkedQObjectDeleter<SmartPointer>(qtJambiLink->getStrongPointer().template staticCast<SmartPointerLink<SmartPointer,QObject>>()));
    smartPointer = sharedPointer;
    *smartPointerTarget = TargetSmartPointer(sharedPointer);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    if constexpr(std::is_same<SmartPointer<QObject>,QWeakPointer<QObject>>::value || std::is_same<SmartPointer<QObject>,std::weak_ptr<QObject>>::value){
        qtJambiLink->QtJambiLink::setJavaOwnership(env);
    }else{
        qtJambiLink->QtJambiLink::setCppOwnership(env);
    }
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   bool created_by_java, bool is_shell,
                                                                                   const QSharedPointer<QObject>& smartPointer,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForSmartPointerToQObject<QSharedPointer>(env, javaObject, created_by_java, is_shell,
                                                              smartPointer,
                                                              interfaceOffsetInfo,
                                                              superTypeForCustomMetaObject);
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   bool created_by_java, bool is_shell,
                                                                                   const std::shared_ptr<QObject>& smartPointer,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForSmartPointerToQObject<std::shared_ptr>(env, javaObject, created_by_java, is_shell,
                                                              smartPointer,
                                                              interfaceOffsetInfo,
                                                              superTypeForCustomMetaObject);
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   bool created_by_java, bool is_shell,
                                                                                   const QSharedPointer<QObject>& smartPointer,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForSmartPointerToQObject<QSharedPointer>(env, javaObject, created_by_java, is_shell,
                                                              smartPointer,
                                                              superTypeForCustomMetaObject);
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   bool created_by_java, bool is_shell,
                                                                                   const std::shared_ptr<QObject>& smartPointer,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForSmartPointerToQObject<std::shared_ptr>(env, javaObject, created_by_java, is_shell,
                                                              smartPointer,
                                                              superTypeForCustomMetaObject);
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                   QObject* object, QSharedPointer<QObject>& smartPointer,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForNewSmartPointerToQObject<QSharedPointer>(env, javaObject, created_by_java, ownership, is_shell,
                                                              object, smartPointer,
                                                              interfaceOffsetInfo,
                                                              superTypeForCustomMetaObject);
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                   QObject* object, std::shared_ptr<QObject>& smartPointer,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForNewSmartPointerToQObject<std::shared_ptr>(env, javaObject, created_by_java, ownership, is_shell,
                                                              object, smartPointer,
                                                              interfaceOffsetInfo,
                                                              superTypeForCustomMetaObject);
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                   QObject* object, QSharedPointer<QObject>& smartPointer,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForNewSmartPointerToQObject<QSharedPointer>(env, javaObject, created_by_java, ownership, is_shell,
                                                              object, smartPointer,
                                                              superTypeForCustomMetaObject);
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                   QObject* object, std::shared_ptr<QObject>& smartPointer,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForNewSmartPointerToQObject<std::shared_ptr>(env, javaObject, created_by_java, ownership, is_shell,
                                                              object, smartPointer,
                                                              superTypeForCustomMetaObject);
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                   QObject* object, QWeakPointer<QObject>& smartPointer,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForNewSmartPointerToQObject<QWeakPointer>(env, javaObject, created_by_java, ownership, is_shell,
                                                              object, smartPointer,
                                                              interfaceOffsetInfo,
                                                              superTypeForCustomMetaObject);
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                   QObject* object, std::weak_ptr<QObject>& smartPointer,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForNewSmartPointerToQObject<std::weak_ptr>(env, javaObject, created_by_java, ownership, is_shell,
                                                              object, smartPointer,
                                                              interfaceOffsetInfo,
                                                              superTypeForCustomMetaObject);
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                   QObject* object, QWeakPointer<QObject>& smartPointer,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForNewSmartPointerToQObject<QWeakPointer>(env, javaObject, created_by_java, ownership, is_shell,
                                                              object, smartPointer,
                                                              superTypeForCustomMetaObject);
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToQObject(JNIEnv *env, jobject javaObject,
                                                                                   bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                   QObject* object, std::weak_ptr<QObject>& smartPointer,
                                                                                   const QMetaObject* superTypeForCustomMetaObject){
    return createLinkForNewSmartPointerToQObject<std::weak_ptr>(env, javaObject, created_by_java, ownership, is_shell,
                                                              object, smartPointer,
                                                              superTypeForCustomMetaObject);
}

template<template<typename> class SmartPointer>
const QSharedPointer<QtJambiLink>& QtJambiLink::createExtendedLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                          LINK_NAME_ARG(const char* qt_name)
                                                                                          bool created_by_java, bool is_shell,
                                                                                          const QObject* extension,
                                                                                          PtrOwnerFunction ownerFunction,
                                                                                          const SmartPointer<char>& smartPointer)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(smartPointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    SmartPointer<char>* smartPointerTarget{nullptr};
    auto qtJambiLink = new ExtendedSmartPointerToObjectLink<SmartPointer>(env, nativeLink,
                                                                                  LINK_NAME_ARG(qt_name)
                                                                                  created_by_java, QtJambiLink::Ownership::None, is_shell, extension,
                                                                                  ownerFunction, smartPointer.get(),
                                                                                  smartPointerTarget,
                                                                                  ocurredException);
    *smartPointerTarget = smartPointer;
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    qtJambiLink->setJavaOwnership(env);
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createExtendedLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                          LINK_NAME_ARG(const char* qt_name)
                                                                                          bool created_by_java, bool is_shell,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createExtendedLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                          LINK_NAME_ARG(const char* qt_name)
                                                                                          bool created_by_java, bool is_shell,
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
const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  SmartPointer<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    typedef typename std::conditional<std::is_same<SmartPointer<char>,QWeakPointer<char>>::value, QSharedPointer<char>,
                                      typename std::conditional<std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value, std::shared_ptr<char>, SmartPointer<char>>::type>::type SharedPointerT;
    typedef typename std::conditional</*if*/std::is_same<SmartPointer<char>,QWeakPointer<char>>::value,
                                      /*then*/QSharedPointer<char>,
                                      /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value,
                                                                        /*then*/std::shared_ptr<char>,
                                                                        /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,QSharedPointer<char>>::value,
                                                                                                          /*then*/QWeakPointer<char>,
                                                                                                          /*else*/std::weak_ptr<char>>::type>::type>::type TargetSmartPointer;
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(pointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    TargetSmartPointer* smartPointerTarget{nullptr};
    auto qtJambiLink = new MetaTypedSmartPointerToObjectInterfaceLink(env, nativeLink, interfaceOffsetInfo,
                                                                      elementMetaType,
                                                                      created_by_java, ownership, is_shell, ownerFunction, pointer, smartPointerTarget, ocurredException);
    SharedPointerT sharedPointer(reinterpret_cast<char*>(pointer), SmartMetaTypeDeleter<SmartPointer>(elementMetaType, qtJambiLink->getStrongPointer().template staticCast<SmartPointerLink<SmartPointer,char>>()));
    smartPointer = sharedPointer;
    *smartPointerTarget = TargetSmartPointer(sharedPointer);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    qtJambiLink->setJavaOwnership(env);
    return qtJambiLink->getStrongPointer();
}

template<template<typename> class SmartPointer>
const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction,
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
    auto qtJambiLink = new SmartPointerToObjectInterfaceLink(env, nativeLink, interfaceOffsetInfo,
                                                            LINK_NAME_ARG(qt_name)
                                                            created_by_java, QtJambiLink::Ownership::None, is_shell, ownerFunction, smartPointer.get(), smartPointerTarget, ocurredException);
    *smartPointerTarget = smartPointer;
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    qtJambiLink->setJavaOwnership(env);
    return qtJambiLink->getStrongPointer();
}

template<template<typename> class SmartPointer>
const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                  PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  SmartPointer<char>& smartPointer,
                                                                                  const QMetaType& elementMetaType)
{
    typedef typename std::conditional<std::is_same<SmartPointer<char>,QWeakPointer<char>>::value, QSharedPointer<char>,
                                      typename std::conditional<std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value, std::shared_ptr<char>, SmartPointer<char>>::type>::type SharedPointerT;
    typedef typename std::conditional</*if*/std::is_same<SmartPointer<char>,QWeakPointer<char>>::value,
                                      /*then*/QSharedPointer<char>,
                                      /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value,
                                                                        /*then*/std::shared_ptr<char>,
                                                                        /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,QSharedPointer<char>>::value,
                                                                                                          /*then*/QWeakPointer<char>,
                                                                                                          /*else*/std::weak_ptr<char>>::type>::type>::type TargetSmartPointer;
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(pointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    TargetSmartPointer* smartPointerTarget{nullptr};
    auto qtJambiLink = new MetaTypedSmartPointerToObjectLink(env, nativeLink,
                                                             elementMetaType,
                                                             created_by_java, ownership, is_shell,
                                                             ownerFunction, pointer,
                                                             smartPointerTarget, ocurredException);
    SharedPointerT sharedPointer(reinterpret_cast<char*>(pointer), SmartMetaTypeDeleter<SmartPointer>(elementMetaType, qtJambiLink->getStrongPointer().template staticCast<SmartPointerLink<SmartPointer,char>>()));
    smartPointer = sharedPointer;
    *smartPointerTarget = TargetSmartPointer(sharedPointer);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    if constexpr(std::is_same<SmartPointer<char>,QWeakPointer<char>>::value || std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value){
        qtJambiLink->QtJambiLink::setJavaOwnership(env);
    }else{
        qtJambiLink->QtJambiLink::setCppOwnership(env);
    }
    return qtJambiLink->getStrongPointer();
}

template<template<typename> class SmartPointer>
const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  SmartPointer<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction,
                                                                                  const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    typedef typename std::conditional<std::is_same<SmartPointer<char>,QWeakPointer<char>>::value, QSharedPointer<char>,
                                      typename std::conditional<std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value, std::shared_ptr<char>, SmartPointer<char>>::type>::type SharedPointerT;
    typedef typename std::conditional</*if*/std::is_same<SmartPointer<char>,QWeakPointer<char>>::value,
                                      /*then*/QSharedPointer<char>,
                                      /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value,
                                                                        /*then*/std::shared_ptr<char>,
                                                                        /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,QSharedPointer<char>>::value,
                                                                                                          /*then*/QWeakPointer<char>,
                                                                                                          /*else*/std::weak_ptr<char>>::type>::type>::type TargetSmartPointer;
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(pointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    TargetSmartPointer* smartPointerTarget{nullptr};
    auto qtJambiLink = new SmartPointerToObjectInterfaceLink(env, nativeLink, interfaceOffsetInfo,
                                                            LINK_NAME_ARG(qt_name)
                                                            created_by_java, ownership, is_shell, ownerFunction, pointer, smartPointerTarget, ocurredException);
    SharedPointerT sharedPointer(reinterpret_cast<char*>(pointer), SmartDeleterFunctionDeleter<SmartPointer>{deleterFunction, qtJambiLink->getStrongPointer().template staticCast<SmartPointerLink<SmartPointer,char>>()});
    smartPointer = sharedPointer;
    *smartPointerTarget = TargetSmartPointer(sharedPointer);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    if constexpr(std::is_same<SmartPointer<char>,QWeakPointer<char>>::value || std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value){
        qtJambiLink->QtJambiLink::setJavaOwnership(env);
    }else{
        qtJambiLink->QtJambiLink::setCppOwnership(env);
    }
    return qtJambiLink->getStrongPointer();
}

template<template<typename> class SmartPointer>
const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                  PtrOwnerFunction ownerFunction,
                                                                                  void* pointer,
                                                                                  SmartPointer<char>& smartPointer,
                                                                                  PtrDeleterFunction deleterFunction)
{
    typedef typename std::conditional<std::is_same<SmartPointer<char>,QWeakPointer<char>>::value, QSharedPointer<char>,
                                      typename std::conditional<std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value, std::shared_ptr<char>, SmartPointer<char>>::type>::type SharedPointerT;
    typedef typename std::conditional</*if*/std::is_same<SmartPointer<char>,QWeakPointer<char>>::value,
                                      /*then*/QSharedPointer<char>,
                                      /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value,
                                                                        /*then*/std::shared_ptr<char>,
                                                                        /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,QSharedPointer<char>>::value,
                                                                                                          /*then*/QWeakPointer<char>,
                                                                                                          /*else*/std::weak_ptr<char>>::type>::type>::type TargetSmartPointer;
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(pointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    TargetSmartPointer* smartPointerTarget{nullptr};
    auto qtJambiLink = new SmartPointerToObjectLink(env, nativeLink,
                                                 LINK_NAME_ARG(qt_name)
                                                 created_by_java, ownership, is_shell, ownerFunction, pointer, smartPointerTarget, ocurredException);
    SharedPointerT sharedPointer(reinterpret_cast<char*>(pointer), SmartDeleterFunctionDeleter<SmartPointer>{deleterFunction, qtJambiLink->getStrongPointer().template staticCast<SmartPointerLink<SmartPointer,char>>()});
    smartPointer = sharedPointer;
    *smartPointerTarget = TargetSmartPointer(sharedPointer);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    if constexpr(std::is_same<SmartPointer<char>,QWeakPointer<char>>::value || std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value){
        qtJambiLink->QtJambiLink::setJavaOwnership(env);
    }else{
        qtJambiLink->QtJambiLink::setCppOwnership(env);
    }
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, PtrOwnerFunction ownerFunction,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, PtrOwnerFunction ownerFunction,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, PtrOwnerFunction ownerFunction,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, PtrOwnerFunction ownerFunction,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, PtrOwnerFunction ownerFunction,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, PtrOwnerFunction ownerFunction,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, PtrOwnerFunction ownerFunction,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, PtrOwnerFunction ownerFunction,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction,
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
const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                              LINK_NAME_ARG(const char* qt_name)
                                              bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction,
                                              const SmartPointer<char>& smartPointer)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(smartPointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    SmartPointer<char>* smartPointerTarget{nullptr};
    auto qtJambiLink = new SmartPointerToObjectLink(env, nativeLink,
                                                 LINK_NAME_ARG(qt_name)
                                                 created_by_java, QtJambiLink::Ownership::None, is_shell, ownerFunction, smartPointer.get(), smartPointerTarget, ocurredException);
    *smartPointerTarget = smartPointer;
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    qtJambiLink->setJavaOwnership(env);
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                              LINK_NAME_ARG(const char* qt_name)
                                              bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction,
                                              const QSharedPointer<char>& smartPointer)
{
    return createLinkForSmartPointerToObject<QSharedPointer>(env, javaObject,
                                                      LINK_NAME_ARG(qt_name)
                                                      created_by_java,
                                                      is_shell,
                                                      ownerFunction,
                                                      smartPointer);
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                              LINK_NAME_ARG(const char* qt_name)
                                              bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction,
                                              const std::shared_ptr<char>& smartPointer)
{
    return createLinkForSmartPointerToObject<std::shared_ptr>(env, javaObject,
                                                      LINK_NAME_ARG(qt_name)
                                                      created_by_java,
                                                      is_shell,
                                                      ownerFunction,
                                                      smartPointer);
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, PtrOwnerFunction ownerFunction,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, PtrOwnerFunction ownerFunction,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, PtrOwnerFunction ownerFunction,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, PtrOwnerFunction ownerFunction,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, PtrOwnerFunction ownerFunction,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, PtrOwnerFunction ownerFunction,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, PtrOwnerFunction ownerFunction,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, PtrOwnerFunction ownerFunction,
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
const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
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
    auto qtJambiLink = new SmartPointerToContainerInterfaceLink<SmartPointer>(env, nativeLink, interfaceOffsetInfo,
                                                                                      LINK_NAME_ARG(qt_name)
                                                                                      false, QtJambiLink::Ownership::None, false, nullptr,
                                                                                      smartPointer.get(), smartPointerTarget, containerAccess, ocurredException);
    *smartPointerTarget = smartPointer;
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    qtJambiLink->setJavaOwnership(env);
    return qtJambiLink->getStrongPointer();
}

template<template<typename> class SmartPointer>
const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
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
    auto qtJambiLink = new SmartPointerToContainerLink(env, nativeLink,
                                                    LINK_NAME_ARG(qt_name)
                                                    false, QtJambiLink::Ownership::None, false, nullptr,
                                                       smartPointer.get(), smartPointerTarget, containerAccess, ocurredException);
    *smartPointerTarget = smartPointer;
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    qtJambiLink->setJavaOwnership(env);
    return qtJambiLink->getStrongPointer();
}

template<template<typename> class SmartPointer>
const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                     LINK_NAME_ARG(const char* qt_name)
                                                                                     QtJambiLink::Ownership ownership,
                                                                                     void* pointer, SmartPointer<char>& smartPointer,
                                                                                     AbstractContainerAccess* containerAccess,
                                                                                     const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    typedef typename std::conditional<std::is_same<SmartPointer<char>,QWeakPointer<char>>::value, QSharedPointer<char>,
                                      typename std::conditional<std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value, std::shared_ptr<char>, SmartPointer<char>>::type>::type SharedPointerT;
    typedef typename std::conditional</*if*/std::is_same<SmartPointer<char>,QWeakPointer<char>>::value,
                                      /*then*/QSharedPointer<char>,
                                      /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value,
                                                                        /*then*/std::shared_ptr<char>,
                                                                        /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,QSharedPointer<char>>::value,
                                                                                                          /*then*/QWeakPointer<char>,
                                                                                                          /*else*/std::weak_ptr<char>>::type>::type>::type TargetSmartPointer;
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(pointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    TargetSmartPointer* smartPointerTarget{nullptr};
    auto qtJambiLink = new SmartPointerToContainerInterfaceLink(env, nativeLink, interfaceOffsetInfo,
                                                                                      LINK_NAME_ARG(qt_name)
                                                                                      false, ownership, false, nullptr,
                                                                                      pointer, smartPointerTarget, containerAccess, ocurredException);

    SharedPointerT sharedPointer(reinterpret_cast<char*>(pointer), SmartContainerDeleter<SmartPointer>(containerAccess, qtJambiLink->getStrongPointer().template staticCast<SmartPointerLink<SmartPointer,char>>()));
    smartPointer = sharedPointer;
    *smartPointerTarget = TargetSmartPointer(sharedPointer);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    if constexpr(std::is_same<SmartPointer<char>,QWeakPointer<char>>::value || std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value){
        qtJambiLink->QtJambiLink::setJavaOwnership(env);
    }else{
        qtJambiLink->QtJambiLink::setCppOwnership(env);
    }
    return qtJambiLink->getStrongPointer();
}

template<template<typename> class SmartPointer>
const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                     LINK_NAME_ARG(const char* qt_name)
                                                                                     QtJambiLink::Ownership ownership,
                                                                                     void* pointer, SmartPointer<char>& smartPointer,
                                                                                     AbstractContainerAccess* containerAccess)
{
    typedef typename std::conditional<std::is_same<SmartPointer<char>,QWeakPointer<char>>::value, QSharedPointer<char>,
                                      typename std::conditional<std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value, std::shared_ptr<char>, SmartPointer<char>>::type>::type SharedPointerT;
    typedef typename std::conditional</*if*/std::is_same<SmartPointer<char>,QWeakPointer<char>>::value,
                                      /*then*/QSharedPointer<char>,
                                      /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value,
                                                                        /*then*/std::shared_ptr<char>,
                                                                        /*else*/typename std::conditional</*if*/std::is_same<SmartPointer<char>,QSharedPointer<char>>::value,
                                                                                                          /*then*/QWeakPointer<char>,
                                                                                                          /*else*/std::weak_ptr<char>>::type>::type>::type TargetSmartPointer;
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(pointer);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    TargetSmartPointer* smartPointerTarget{nullptr};
    auto qtJambiLink = new SmartPointerToContainerLink(env, nativeLink,
                                                    LINK_NAME_ARG(qt_name)
                                                    false, ownership, false, nullptr, pointer, smartPointerTarget, containerAccess, ocurredException);
    SharedPointerT sharedPointer(reinterpret_cast<char*>(pointer), SmartContainerDeleter<SmartPointer>(containerAccess, qtJambiLink->getStrongPointer().template staticCast<SmartPointerLink<SmartPointer,char>>()));
    smartPointer = sharedPointer;
    *smartPointerTarget = TargetSmartPointer(sharedPointer);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    if constexpr(std::is_same<SmartPointer<char>,QWeakPointer<char>>::value || std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value){
        qtJambiLink->QtJambiLink::setJavaOwnership(env);
    }else{
        qtJambiLink->QtJambiLink::setCppOwnership(env);
    }
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewSmartPointerToObject(JNIEnv *env, jobject javaObject,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                       LINK_NAME_ARG(const char* qt_name)
                                                                       bool created_by_java, bool isCopy, AbstractContainerAccess* containerAccess)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(containerAccess);
    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(javaObject));
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        return createLinkForNativeObject(env, javaObject, ptr,
                                         LINK_NAME_ARG(qt_name)
                                         created_by_java, isCopy, containerAccess, QtJambiLink::Ownership::None,
                                         *interfaceOffsets);
    }else{
        return createLinkForNativeObject(env, javaObject, ptr,
                                         LINK_NAME_ARG(qt_name)
                                         created_by_java, isCopy, containerAccess, QtJambiLink::Ownership::None);
    }
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                          LINK_NAME_ARG(const char* qt_name)
                                                                          bool created_by_java, bool isCopy, AbstractContainerAccess* containerAccess, QtJambiLink::Ownership ownership)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(containerAccess);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new PointerToContainerLink(env, nativeLink,
                                                            LINK_NAME_ARG(qt_name)
                                                            ptr,
                                                            created_by_java, false,
                                                            containerAccess, ocurredException);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
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
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                          LINK_NAME_ARG(const char* qt_name)
                                                                          bool created_by_java, bool isCopy, AbstractContainerAccess* containerAccess, QtJambiLink::Ownership ownership,
                                                                          const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(containerAccess);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    //const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(javaObject));
    JavaException ocurredException;
    QtJambiLink* qtJambiLink;
    qtJambiLink = new PointerToContainerInterfaceLink(env,
                                                      nativeLink,
                                                      interfaceOffsetInfo,
                                                      LINK_NAME_ARG(qt_name)
                                                      ptr, created_by_java, false,
                                                      containerAccess, ocurredException);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
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
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool isCopy, PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(containerAccess);
    Q_ASSERT(destructor_function);
    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(javaObject));
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        return createLinkForNativeObject(env, javaObject, ptr,
                                         LINK_NAME_ARG(qt_name)
                                         created_by_java, isCopy, destructor_function, containerAccess, QtJambiLink::Ownership::None,
                                         *interfaceOffsets);
    }else{
        return createLinkForNativeObject(env, javaObject, ptr,
                                         LINK_NAME_ARG(qt_name)
                                         created_by_java, isCopy, destructor_function, containerAccess, QtJambiLink::Ownership::None);
    }
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool isCopy, PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess, QtJambiLink::Ownership ownership)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(containerAccess);
    Q_ASSERT(destructor_function);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new DeletablePointerToContainerLink(env, nativeLink,
                                                            LINK_NAME_ARG(qt_name)
                                                            ptr, created_by_java, false,
                                                            destructor_function, containerAccess, ocurredException);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
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
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool isCopy, PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess, QtJambiLink::Ownership ownership,
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
    QtJambiLink* qtJambiLink = new DeletablePointerToContainerInterfaceLink(env, nativeLink,
                                                                   interfaceOffsetInfo,
                                                                   LINK_NAME_ARG(qt_name)
                                                                   ptr, created_by_java, false,
                                                                   destructor_function, containerAccess, ocurredException);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
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
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                        bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, QtJambiLink::Ownership ownership)
{
    if(!ownerFunction){
        return createLinkForNativeObject(env, javaObject, ptr, metaType,
                                   created_by_java, is_shell, ownership);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(ownerFunction);
    Q_ASSERT(metaType.isValid());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new OwnedMetaTypedPointerToObjectLink(env, nativeLink,
                                                                     ptr, metaType, created_by_java,
                                                                     is_shell, ownerFunction, ocurredException);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
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
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                          bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, QtJambiLink::Ownership ownership,
                                                                          const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    if(!ownerFunction){
        return createLinkForNativeObject(env, javaObject, ptr, metaType,
                                         created_by_java, is_shell, ownership,
                                         interfaceOffsetInfo);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(ownerFunction);
    Q_ASSERT(metaType.isValid());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new OwnedMetaTypedPointerToObjectInterfaceLink(env, nativeLink,
                                                                              interfaceOffsetInfo,
                                                                              ptr, metaType, created_by_java,
                                                                              is_shell, ownerFunction, ocurredException);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
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
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                          bool created_by_java, bool is_shell, QtJambiLink::Ownership ownership)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(metaType.isValid());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new MetaTypedPointerToObjectLink(env, nativeLink,
                                                                ptr, metaType, created_by_java, is_shell, ocurredException);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
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
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                          bool created_by_java, bool is_shell, QtJambiLink::Ownership ownership,
                                                                          const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(metaType.isValid());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new MetaTypedPointerToObjectInterfaceLink(env, nativeLink, interfaceOffsetInfo,
                                                                         ptr, metaType, created_by_java, is_shell, ocurredException);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
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
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForDependentObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                        bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, const QObject* dependsOn, QtJambiLink::Ownership ownership)
{
    if(!ownerFunction){
        return createLinkForDependentObject(env, javaObject, ptr, metaType,
                                   created_by_java, is_shell, dependsOn, ownership);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(ownerFunction);
    Q_ASSERT(metaType.isValid());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new OwnedDependentMetaTypedPointerToObjectLink(env, nativeLink,
                                                                              ptr, metaType, created_by_java,
                                                                              is_shell, ownerFunction, ocurredException);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
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
    QtJambiLink::registerDependentObject(dependsOn, qtJambiLink->getStrongPointer());
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForDependentObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                        bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, const QObject* dependsOn, QtJambiLink::Ownership ownership,
                                                                             const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    if(!ownerFunction){
        return createLinkForDependentObject(env, javaObject, ptr, metaType,
                                   created_by_java, is_shell, dependsOn, ownership);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(ownerFunction);
    Q_ASSERT(metaType.isValid());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new OwnedDependentMetaTypedPointerToObjectInterfaceLink(env, nativeLink,
                                                                                       interfaceOffsetInfo,
                                                                                       ptr, metaType, created_by_java,
                                                                                       is_shell, ownerFunction, ocurredException);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
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
    QtJambiLink::registerDependentObject(dependsOn, qtJambiLink->getStrongPointer());
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForDependentObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                             LINK_NAME_ARG(const char* qt_name)
                                                                             bool created_by_java, bool is_shell,
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
    QtJambiLink* qtJambiLink = new DependentDeletablePointerToObjectLink(env, nativeLink,
                                                                         LINK_NAME_ARG(qt_name)
                                                                         ptr, created_by_java, is_shell, destructor_function, ocurredException);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
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
    QtJambiLink::registerDependentObject(dependsOn, qtJambiLink->getStrongPointer());
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createExtendedLinkForObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                      LINK_NAME_ARG(const char* qt_name)
                                                                      bool created_by_java, bool is_shell,
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
    QtJambiLink* qtJambiLink = new ExtendedDeletablePointerToObjectLink(env, nativeLink,
                                                                         LINK_NAME_ARG(qt_name)
                                                                         ptr, created_by_java, is_shell, destructor_function, extension, ocurredException);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
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
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForDependentObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                        bool created_by_java, bool is_shell, const QObject* dependsOn, QtJambiLink::Ownership ownership)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(metaType.isValid());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new DependentMetaTypedPointerToObjectLink(env, nativeLink,
                                                                         ptr, metaType, created_by_java, is_shell, ocurredException);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
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
    QtJambiLink::registerDependentObject(dependsOn, qtJambiLink->getStrongPointer());
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForDependentObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                             bool created_by_java, bool is_shell, const QObject* dependsOn, QtJambiLink::Ownership ownership,
                                                                             const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(metaType.isValid());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new DependentMetaTypedPointerToObjectInterfaceLink(env, nativeLink,
                                                                                  interfaceOffsetInfo,
                                                                                  ptr, metaType, created_by_java, is_shell, ocurredException);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
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
    QtJambiLink::registerDependentObject(dependsOn, qtJambiLink->getStrongPointer());
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool is_shell, PtrDeleterFunction deleter_function, PtrOwnerFunction ownerFunction, QtJambiLink::Ownership ownership)
{
    if(!ownerFunction){
        return createLinkForNativeObject(env, javaObject, ptr,
                                   LINK_NAME_ARG(qt_name)
                                   created_by_java, is_shell, deleter_function, ownership);
    }
    if(!deleter_function){
        return createLinkForNativeObject(env, javaObject, ptr,
                                   LINK_NAME_ARG(qt_name)
                                   created_by_java, is_shell, ownership);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(ownerFunction);
    Q_ASSERT(deleter_function);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new DeletableOwnedPointerToObjectLink(env, nativeLink,
                                                                     LINK_NAME_ARG(qt_name)
                                                                     ptr, created_by_java,
                                                                     is_shell, deleter_function,
                                                                     ownerFunction, ocurredException);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
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
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                          LINK_NAME_ARG(const char* qt_name)
                                                                          bool created_by_java, bool is_shell, PtrDeleterFunction deleter_function, PtrOwnerFunction ownerFunction, QtJambiLink::Ownership ownership,
                                                                          const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    if(!ownerFunction){
        return createLinkForNativeObject(env, javaObject, ptr,
                                   LINK_NAME_ARG(qt_name)
                                   created_by_java, is_shell, deleter_function, ownership,
                                   interfaceOffsetInfo);
    }
    if(!deleter_function){
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

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new DeletableOwnedPointerToObjectInterfaceLink(env, nativeLink,
                                                                              interfaceOffsetInfo,
                                                                              LINK_NAME_ARG(qt_name)
                                                                              ptr, created_by_java,
                                                                              is_shell, deleter_function,
                                                                              ownerFunction, ocurredException);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
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
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool is_shell, PtrDeleterFunction deleter_function, QtJambiLink::Ownership ownership)
{
    if(!deleter_function){
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
    QtJambiLink* qtJambiLink = new DeletablePointerToObjectLink(env, nativeLink,
                                                                LINK_NAME_ARG(qt_name)
                                                                ptr, created_by_java,
                                                                is_shell, deleter_function, ocurredException);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
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
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                          LINK_NAME_ARG(const char* qt_name)
                                                                          bool created_by_java, bool is_shell, PtrDeleterFunction deleter_function, QtJambiLink::Ownership ownership,
                                                                          const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    if(!deleter_function){
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
    QtJambiLink* qtJambiLink = new DeletablePointerToObjectInterfaceLink(env, nativeLink,
                                                                         interfaceOffsetInfo,
                                                                         LINK_NAME_ARG(qt_name)
                                                                         ptr, created_by_java,
                                                                         is_shell, deleter_function, ocurredException);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
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
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                          LINK_NAME_ARG(const char* qt_name)
                                                                          bool created_by_java, bool is_shell, QtJambiLink::Ownership ownership)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new PointerToObjectLink(env, nativeLink,
                                              LINK_NAME_ARG(qt_name)
                                              ptr, created_by_java, is_shell, ocurredException);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
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
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                          LINK_NAME_ARG(const char* qt_name)
                                                                          bool created_by_java, bool is_shell, QtJambiLink::Ownership ownership,
                                                                          const InterfaceOffsetInfo& interfaceOffsetInfo)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new PointerToObjectInterfaceLink(env, nativeLink,
                                                                interfaceOffsetInfo,
                                                                LINK_NAME_ARG(qt_name)
                                                                ptr, created_by_java, is_shell, ocurredException);
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
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
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, const QMetaType& metaType, bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
{
    if(!ownerFunction){
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
        QtJambiLink* qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            qtJambiLink = new OwnedMetaTypedPointerToObjectInterfaceLink(env, nativeLink, *interfaceOffsets,
                                                           ptr, metaType, created_by_java, is_shell, ownerFunction, ocurredException);
        }else{
            qtJambiLink = new OwnedMetaTypedPointerToObjectLink(env, nativeLink,
                                                  ptr, metaType, created_by_java, is_shell, ownerFunction, ocurredException);
        }
        if(ocurredException){
            qtJambiLink->dispose();
        }else{
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            return qtJambiLink->getStrongPointer();
        }
    }
    return *gDefaultPointer;
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos,
                                                                          bool created_by_java, bool is_shell, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
{
    if(!ocurredException){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);
        Q_ASSERT(containerAccess);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        QtJambiLink* qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            qtJambiLink = new PointerToContainerInterfaceLink(env, nativeLink, *interfaceOffsets,
                                                              LINK_NAME_ARG(getQtName(typeId))
                                                              ptr, created_by_java, is_shell, containerAccess, ocurredException);
        }else{
            qtJambiLink = new PointerToContainerLink(env, nativeLink,
                                                     LINK_NAME_ARG(getQtName(typeId))
                                                     ptr, created_by_java, is_shell, containerAccess, ocurredException);
        }
        if(ocurredException){
            qtJambiLink->dispose();
        }else{
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            return qtJambiLink->getStrongPointer();
        }
    }
    return *gDefaultPointer;
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, JavaException& ocurredException)
{
    if(!ocurredException){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);
        Q_ASSERT(containerAccess);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        QtJambiLink* qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            qtJambiLink = new DeletablePointerToContainerInterfaceLink(env, nativeLink, *interfaceOffsets,
                                                                       LINK_NAME_ARG(getQtName(typeId))
                                                                       ptr, created_by_java, is_shell, destructor_function, containerAccess, ocurredException);
        }else{
            qtJambiLink = new DeletablePointerToContainerLink(env, nativeLink,
                                                              LINK_NAME_ARG(getQtName(typeId))
                                                              ptr, created_by_java, is_shell, destructor_function, containerAccess, ocurredException);
        }
        if(ocurredException){
            qtJambiLink->dispose();
        }else{
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            return qtJambiLink->getStrongPointer();
        }
    }
    return *gDefaultPointer;
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
{
    if(!ocurredException){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);
        Q_ASSERT(containerAccess);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        QtJambiLink* qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            qtJambiLink = new DeletableOwnedPointerToContainerInterfaceLink(env, nativeLink, *interfaceOffsets,
                                                                       LINK_NAME_ARG(getQtName(typeId))
                                                                       ptr, created_by_java, is_shell, destructor_function, ownerFunction, containerAccess, ocurredException);
        }else{
            qtJambiLink = new DeletableOwnedPointerToContainerLink(env, nativeLink,
                                                              LINK_NAME_ARG(getQtName(typeId))
                                                              ptr, created_by_java, is_shell, destructor_function, ownerFunction, containerAccess, ocurredException);
        }
        if(ocurredException){
            qtJambiLink->dispose();
        }else{
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            return qtJambiLink->getStrongPointer();
        }
    }
    return *gDefaultPointer;
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, const QMetaType& metaType, bool created_by_java, bool is_shell, JavaException& ocurredException)
{
    if(!ocurredException){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);
        Q_ASSERT(metaType.isValid());

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        QtJambiLink* qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            qtJambiLink = new MetaTypedPointerToObjectInterfaceLink(env, nativeLink, *interfaceOffsets,
                                                           ptr, metaType, created_by_java, is_shell, ocurredException);
        }else{
            qtJambiLink = new MetaTypedPointerToObjectLink(env, nativeLink,
                                                  ptr, metaType, created_by_java, is_shell, ocurredException);
        }
        if(ocurredException){
            qtJambiLink->dispose();
        }else{
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            return qtJambiLink->getStrongPointer();
        }
    }
    return *gDefaultPointer;
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, PtrDeleterFunction deleter_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
{
    if(!ownerFunction){
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
        QtJambiLink* qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            qtJambiLink = new DeletableOwnedPointerToObjectInterfaceLink(env, nativeLink, *interfaceOffsets,
                                                           LINK_NAME_ARG(getQtName(typeId))
                                                           ptr, created_by_java, is_shell, deleter_function, ownerFunction, ocurredException);
        }else{
            qtJambiLink = new DeletableOwnedPointerToObjectLink(env, nativeLink,
                                                  LINK_NAME_ARG(getQtName(typeId))
                                                  ptr, created_by_java, is_shell, deleter_function, ownerFunction, ocurredException);
        }
        if(ocurredException){
            qtJambiLink->dispose();
        }else{
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            return qtJambiLink->getStrongPointer();
        }
    }
    return *gDefaultPointer;
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, PtrDeleterFunction deleter_function, JavaException& ocurredException)
{
    if(!deleter_function){
        return createLinkForNewObject(env, objectClass, nativeLink, javaObject, typeId, ptr, superTypeInfos, created_by_java, is_shell, ocurredException);
    }
    if(!ocurredException){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);
        Q_ASSERT(deleter_function);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        QtJambiLink* qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            qtJambiLink = new DeletablePointerToObjectInterfaceLink(env, nativeLink, *interfaceOffsets,
                                                           LINK_NAME_ARG(getQtName(typeId))
                                                           ptr, created_by_java, is_shell, deleter_function, ocurredException);
        }else{
            qtJambiLink = new DeletablePointerToObjectLink(env, nativeLink,
                                                  LINK_NAME_ARG(getQtName(typeId))
                                                  ptr, created_by_java, is_shell, deleter_function, ocurredException);
        }
        if(ocurredException){
            qtJambiLink->dispose();
        }else{
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            return qtJambiLink->getStrongPointer();
        }
    }
    return *gDefaultPointer;
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, JavaException& ocurredException)
{
    if(!ocurredException){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        QtJambiLink* qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            qtJambiLink = new PointerToObjectInterfaceLink(env, nativeLink, *interfaceOffsets,
                                                           LINK_NAME_ARG(getQtName(typeId))
                                                           ptr, created_by_java, is_shell, ocurredException);
        }else{
            qtJambiLink = new PointerToObjectLink(env, nativeLink,
                                                  LINK_NAME_ARG(getQtName(typeId))
                                                  ptr, created_by_java, is_shell, ocurredException);
        }
        if(ocurredException){
            qtJambiLink->dispose();
        }else{
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            return qtJambiLink->getStrongPointer();
        }
    }
    return *gDefaultPointer;
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForOwnedObject(JNIEnv *env, jobject javaObject, void *ptr,
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
    PointerToObjectLink* qtJambiLink;
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        if(deleter_function){
            qtJambiLink = new DeletablePointerToObjectInterfaceLink(env, nativeLink, *interfaceOffsets,
                                                           LINK_NAME_ARG(qt_name)
                                                           ptr, false, false, deleter_function, ocurredException);
        }else{
            qtJambiLink = new PointerToObjectInterfaceLink(env, nativeLink, *interfaceOffsets,
                                                           LINK_NAME_ARG(qt_name)
                                                           ptr, false, false, ocurredException);
        }
    }else{
        if(deleter_function){
                qtJambiLink = new DeletablePointerToObjectLink(env, nativeLink,
                                                      LINK_NAME_ARG(qt_name)
                                                      ptr, false, false, deleter_function, ocurredException);
        }else{
                qtJambiLink = new PointerToObjectLink(env, nativeLink,
                                                      LINK_NAME_ARG(qt_name)
                                                      ptr, false, false, ocurredException);
        }
    }
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
        _owner->registerDependentObject(qtJambiLink->getStrongPointer());
    }
    if(deleter_function){
        qtJambiLink->setJavaOwnership(env);
    }else{
        qtJambiLink->setSplitOwnership(env);
    }
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForOwnedObject(JNIEnv *env, jobject javaObject, void *ptr,
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
    PointerToObjectLink* qtJambiLink;
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        qtJambiLink = new PointerToObjectInterfaceLink(env, nativeLink, *interfaceOffsets,
                                                       LINK_NAME_ARG(qt_name)
                                                       ptr, false, false, ocurredException);
    }else{
        qtJambiLink = new PointerToObjectLink(env, nativeLink,
                                              LINK_NAME_ARG(qt_name)
                                              ptr, false, false, ocurredException);
    }
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    qtJambiLink->setSplitOwnership(env);
    if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
        _owner->registerDependentObject(qtJambiLink->getStrongPointer());
    }
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
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
    PointerToObjectLink* qtJambiLink;
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        qtJambiLink = new PointerToContainerInterfaceLink(env, nativeLink, *interfaceOffsets,
                                                       LINK_NAME_ARG(qt_name)
                                                       ptr, false, false, containerAccess, ocurredException);
    }else{
        qtJambiLink = new PointerToContainerLink(env, nativeLink,
                                              LINK_NAME_ARG(qt_name)
                                              ptr, false, false, containerAccess, ocurredException);
    }
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    qtJambiLink->setSplitOwnership(env);
    if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
        _owner->registerDependentObject(qtJambiLink->getStrongPointer());
    }
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr,
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
    PointerToObjectLink* qtJambiLink;
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        qtJambiLink = new DeletablePointerToContainerInterfaceLink(env, nativeLink, *interfaceOffsets,
                                                       LINK_NAME_ARG(qt_name)
                                                       ptr, false, false, destructor_function, containerAccess, ocurredException);
    }else{
        qtJambiLink = new DeletablePointerToContainerLink(env, nativeLink,
                                              LINK_NAME_ARG(qt_name)
                                              ptr, false, false, destructor_function, containerAccess, ocurredException);
    }
    if(ocurredException){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    if(destructor_function){
        qtJambiLink->setJavaOwnership(env);
    }else{
        qtJambiLink->setSplitOwnership(env);
    }
    if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
        _owner->registerDependentObject(qtJambiLink->getStrongPointer());
    }
    return qtJambiLink->getStrongPointer();
}

QList<QSharedPointer<QtJambiLink>> QtJambiLink::findLinksForPointer(const void *ptr)
{
    if (!ptr)
        return {};
    QReadLocker locker(gLinkAccessLock());
    Q_UNUSED(locker)
    Q_ASSERT(gUserObjectCache());
    if(!gUserObjectCache->contains(ptr))
        return {};
    QList<QWeakPointer<QtJambiLink>> values = gUserObjectCache->values(ptr);
    QList<QSharedPointer<QtJambiLink>> result;
    for(const QWeakPointer<QtJambiLink>& link : values){
        result << link;
    }
    return result;
}

QSharedPointer<QtJambiLink> QtJambiLink::findLinkForQObject(const QObject *o)
{
    if (!o)
        return {};
    {
        QReadLocker locker(QtJambiLinkUserData::lock());
        if(QtJambiLinkUserData *p = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, o)){
            QSharedPointer<QtJambiLink> link = p->link();
            return link;
        }
    }
    {
        QReadLocker locker(gLinkAccessLock());
        Q_UNUSED(locker)
        Q_ASSERT(gQObjectCache());
        if(!gQObjectCache->contains(o))
            return {};
        QSharedPointer<QtJambiLink> link = (*gQObjectCache())[o];
        return link;
    }
}

QSharedPointer<QtJambiLink> QtJambiLink::fromNativeId(QtJambiNativeID native_id){
    if(!native_id){
        return {};
    }else{
        return reinterpret_cast<QtJambiLink*>(native_id)->getStrongPointer();
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
        if(link->pointer())
            return link->pointer();
    }else if (!env->IsSameObject(nullptr, java))
        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, java).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    return nullptr;
}

QWeakPointer<QtJambiLink> QtJambiLink::getWeakPointer() const{
    return m_this.toWeakRef();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::getStrongPointer() const{
    return m_this;
}

bool QtJambiLink::noThreadInitializationOnPurge() const{ return m_flags.testFlag(Flag::NoThreadInitializationOnPurge); }

void QtJambiLink::setNoThreadInitializationOnPurge(bool n) { m_flags.setFlag(Flag::NoThreadInitializationOnPurge, n); }

void QtJambiLink::detachJavaLink(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::IsJavaLinkDetached) && env) {
        m_flags.setFlag(Flag::IsJavaLinkDetached);
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "QtJambiLink::detachJavaLink(JNIEnv *env)")
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
            registerPointer(pointer());
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

bool QtJambiLink::createdByQml() const {
    return m_flags.testFlag(Flag::CreatedByQml);
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
    m_this.clear();
}

// ### BEGIN #######################  QtJambiLink  ######################### BEGIN ####

QtJambiLink::QtJambiLink(JNIEnv *env, jobject nativeLink,
                                 LINK_NAME_ARG(const char *qt_name)
                                 bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException) :
    m_this(this),
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
    if(isDeclarativeCall){
        m_flags.setFlag(QtJambiLink::Flag::CreatedByQml);
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
    QTJAMBI_REGISTER_LINK
    QTJAMBI_INCREASE_COUNTER_THIS(linkConstructedCount)
}

QtJambiLink::~QtJambiLink(){
    QTJAMBI_UNREGISTER_LINK
    QTJAMBI_INCREASE_COUNTER_THIS(linkDestroyedCount)
    if(m_flags.testFlag(Flag::NoGlobalRef)){
        if(JniEnvironment env = noThreadInitializationOnPurge() ? JniEnvironment{300} : DefaultJniEnvironment{300}){
            try{
                Java::QtJambi::NativeUtility$NativeLink::resetForID(env, jlong(reinterpret_cast<void*>(this)), hasDisposedSignal());
            } catch (const JavaException& exn) {
                exn.report(env);
            }
            try{
                Java::QtJambi::NativeUtility::releaseGlobalReference(env, jlong(reinterpret_cast<void*>(this)));
            } catch (const JavaException& exn) {
                exn.report(env);
            }
        }
    }else{
        if(m_nativeLink){
            if(JniEnvironment env = noThreadInitializationOnPurge() ? JniEnvironment{300} : DefaultJniEnvironment{300}){
                try{
                    Java::QtJambi::NativeUtility$NativeLink::reset(env, m_nativeLink, jlong(reinterpret_cast<void*>(this)), hasDisposedSignal());
                } catch (const JavaException& exn) {
                    exn.report(env);
                }
                env->DeleteGlobalRef(m_nativeLink);
                m_nativeLink = nullptr;
            }
        }
    }
}

void QtJambiLink::init(JNIEnv*){
    m_flags.setFlag(Flag::IsInitialized, true);
}

// This method will always cleanup QtJambiLink's m_java.object.
void QtJambiLink::releaseJavaObject(JNIEnv *env)
{
    if (!m_flags.testFlag(Flag::JavaObjectIsReleased)) {
        if(env){
            if(m_flags.testFlag(Flag::NoGlobalRef)){
                Java::QtJambi::NativeUtility$NativeLink::releaseOwnershipForID(env, jlong(reinterpret_cast<void*>(this)));
            }else{
                Java::QtJambi::NativeUtility$NativeLink::releaseOwnership(env, m_nativeLink);
            }
            m_flags.setFlag(Flag::JavaObjectIsReleased);
        }
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

void QtJambiLink::unregisterOffsets() {
}

void QtJambiLink::registerPointer(const QSharedPointer<QtJambiLink>& link) {
    if(link && link->pointer()) {
        link->registerPointer(link->pointer());
    }
}

void QtJambiLink::registerPointer(void *ptr) {
    if(QtJambiLink::getObjectCacheMode() != 0) {
        QWriteLocker locker(gLinkAccessLock());
        Q_UNUSED(locker)
        Q_ASSERT(gUserObjectCache());
        QWeakPointer<QtJambiLink> wlink = m_this.toWeakRef();
        if(!gUserObjectCache->contains(ptr, wlink)){
            gUserObjectCache->insert(ptr, wlink);
            m_flags.setFlag(Flag::IsPointerRegistered);
        }
    }
}

void QtJambiLink::unregisterPointer(void *ptr) {
    if(QtJambiLink::getObjectCacheMode() != 0
            && m_flags.testFlag(Flag::IsPointerRegistered)
            && !gLinkAccessLock.isDestroyed()
            && !gUserObjectCache.isDestroyed()) {
        QWriteLocker locker(gLinkAccessLock());
        Q_UNUSED(locker)
        if(MultiLinkHash* h = gUserObjectCache()){
            const QList<QWeakPointer<QtJambiLink>> values = h->values(ptr);
            for(const QWeakPointer<QtJambiLink>& wlink : values){
                QSharedPointer<QtJambiLink> link(wlink);
                if(!link || link==m_this){
                    h->remove(ptr, link.toWeakRef());
                }
            }
        }
    }
}

void QtJambiLink::unregisterPointers(const QVector<void*>& pointers){
    if(QtJambiLink::getObjectCacheMode() != 0
            && !gLinkAccessLock.isDestroyed()
            && !gUserObjectCache.isDestroyed()) {
        QWriteLocker locker(gLinkAccessLock());
        Q_UNUSED(locker)
        if(MultiLinkHash* h = gUserObjectCache()){
            for(void* ptr : pointers){
                const QList<QWeakPointer<QtJambiLink>> values = h->values(ptr);
                for(const QWeakPointer<QtJambiLink>& wlink : values){
                    QSharedPointer<QtJambiLink> link(wlink);
                    if(!link || link==m_this){
                        h->remove(ptr, link.toWeakRef());
                    }
                }
            }
        }
    }
}

#if defined(QTJAMBI_DEBUG_TOOLS)
const char* QtJambiLink::debugFlagsToString(char *s) const {
    *s++ = (char(int(ownership()) + '0'));
    *s++ = ((m_flags.testFlag(Flag::JavaObjectIsReleased)) ? 'R' : '.');
    *s++ = ((m_flags.testFlag(Flag::HasDependencies)) ? 'H' : '.');
    *s++ = ((m_flags.testFlag(Flag::IsDependent)) ? 'd' : '.');
    *s++ = ((m_flags.testFlag(Flag::IsDeleteLater)) ? 'e' : '.');
    *s++ = ((m_flags.testFlag(Flag::CreatedByQml)) ? 'Q' : '.');
    *s++ = ((m_flags.testFlag(Flag::HasBeenCleaned))     ? 'C' : '.');
    *s++ = ((createdByJava())                              ? 'J' : '.');
    *s++ = ((isShell())                              ? 'S' : '.');
    *s++ = ((m_flags.testFlag(Flag::IsJavaLinkDetached))        ? 'D' : '.');
    *s = 0;
    return s;
}
#endif

// ### END #######################  QtJambiLink  ######################### END ####

// ### BEGIN ####################  PointerToObjectLink  ###################### BEGIN ####

MetaTypedPointerToObjectLink::MetaTypedPointerToObjectLink(JNIEnv *env, jobject nativeLink,
                                                                                   void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, JavaException& ocurredException)
    :  PointerToObjectLink(env, nativeLink,
                                           LINK_NAME_META_TYPE_ARG(metaType)
                                           ptr, created_by_java, is_shell, ocurredException), m_meta_type(metaType
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                                                                            .id()
#endif
                                                                                               ) {}

PointerToContainerLink::PointerToContainerLink(JNIEnv *env, jobject nativeLink,
                    LINK_NAME_ARG(const char* qt_name)
                    void *ptr, bool created_by_java, bool is_shell,
                    AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : PointerToObjectLink(env, nativeLink,
                                                           LINK_NAME_ARG(qt_name)
                                                           ptr, created_by_java, is_shell, ocurredException), m_containerAccess(containerAccess) {}

PointerToContainerLink::~PointerToContainerLink(){
    if(m_containerAccess)
        m_containerAccess->dispose();
}

void PointerToContainerLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if (m_pointer && m_containerAccess){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "PointerToContainerLink::deleteNativeObject(JNIEnv *, bool)")
        if(!isShell())
            clearAllDependencies(env);
        unregisterPointer(m_pointer);
        unregisterOffsets();
        m_flags.setFlag(Flag::IsPointerRegistered, false);
        if(!m_flags.testFlag(Flag::NoNativeDeletion) && (m_flags.testFlag(Flag::JavaOwnership) || forced)){
            void* pointer = m_pointer;
            m_pointer = nullptr;
            QWriteLocker locker(QtJambiLinkUserData::lock());
            if(const QObject* obj = m_containerAccess->getOwner(pointer)){
                const QObjectPrivate* p = QObjectPrivate::get(obj);
                Q_ASSERT(p);
                QThread* ownerThread =
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
                        p->threadData->thread;
#else
                        p->threadData.loadAcquire()->thread.loadAcquire();
#endif
                if (ownerThread->isRunning() && ownerThread != QThread::currentThread()) {
                    if(ValueOwnerUserData* vud = QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, obj)){
                        if(p->wasDeleted){
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("delete container %p of type %s", pointer, static_cast<const char*>(m_qtTypeName))
#else
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("delete container %p", pointer)
#endif
                            void* _pointer = pointer;
                            AbstractContainerAccess *const containerAccess = m_containerAccess;
                            m_containerAccess = nullptr;
                            vud->addDeleter([containerAccess,_pointer](){
                                containerAccess->deleteContainer(_pointer);
                                containerAccess->dispose();
                            });
                            dispose();
                        }else{
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor for object %p of type %s", pointer, static_cast<const char*>(m_qtTypeName))
#else
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor for object %p", pointer)
#endif
                            setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                            QThreadUserData::Result threadDataResult = QThreadUserData::ensureThreadUserDataLocked(ownerThread);
                            if (QAbstractEventDispatcher::instance(ownerThread)) {
                                ContainerDestructionEvent* event = new ContainerDestructionEvent(vud->pointer(), m_this, pointer, m_containerAccess);
                                m_containerAccess = nullptr;
                                if(threadDataResult.initRequired){
                                    locker.unlock();
                                    threadDataResult.threadUserData->initialize(ownerThread);
                                    locker.relock();
                                }
                                QCoreApplication::postEvent(threadDataResult.threadUserData->threadDeleter(), event);
                            }else{
                                void* _pointer = pointer;
                                AbstractContainerAccess * containerAccess = m_containerAccess;
                                m_containerAccess = nullptr;
                                threadDataResult.threadUserData->doAtThreadEnd([containerAccess,_pointer](){
                                    containerAccess->deleteContainer(_pointer);
                                    containerAccess->dispose();
                                });
                                if(threadDataResult.initRequired){
                                    locker.unlock();
                                    threadDataResult.threadUserData->initialize(ownerThread);
                                    locker.relock();
                                }
                            }
                            pointer = nullptr;
                        }
                    }else if(!p->wasDeleted){
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                        QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor for object %p of type %s", pointer, static_cast<const char*>(m_qtTypeName))
#else
                        QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor for object %p", pointer)
#endif
                        setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                        QThreadUserData::Result threadDataResult = QThreadUserData::ensureThreadUserDataLocked(ownerThread);
                        if (QAbstractEventDispatcher::instance(ownerThread)) {
                            ContainerDestructionEvent* event = new ContainerDestructionEvent(obj, m_this, pointer, m_containerAccess);
                            m_containerAccess = nullptr;
                            if(threadDataResult.initRequired){
                                locker.unlock();
                                threadDataResult.threadUserData->initialize(ownerThread);
                                locker.relock();
                            }
                            QCoreApplication::postEvent(threadDataResult.threadUserData->threadDeleter(), event);
                        }else{
                            void* _pointer = pointer;
                            AbstractContainerAccess * containerAccess = m_containerAccess;
                            m_containerAccess = nullptr;
                            threadDataResult.threadUserData->doAtThreadEnd([containerAccess,_pointer](){
                                containerAccess->deleteContainer(_pointer);
                                containerAccess->dispose();
                            });
                            if(threadDataResult.initRequired){
                                locker.unlock();
                                threadDataResult.threadUserData->initialize(ownerThread);
                                locker.relock();
                            }
                        }
                        pointer = nullptr;
                    }
                }
            }
            if(pointer && m_containerAccess){
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                QTJAMBI_DEBUG_PRINT_WITH_ARGS("delete container %p of type %s", pointer, static_cast<const char*>(m_qtTypeName))
#else
                QTJAMBI_DEBUG_PRINT_WITH_ARGS("delete container %p", pointer)
#endif
                locker.unlock();
                m_containerAccess->deleteContainer(pointer);
                locker.relock();
                m_containerAccess->dispose();
                m_containerAccess = nullptr;
                dispose();
            }
        }else{
            if(m_containerAccess){
                m_containerAccess->dispose();
                m_containerAccess = nullptr;
            }
            dispose();
        }
    }
}

DeletablePointerToContainerLink::DeletablePointerToContainerLink(JNIEnv *env, jobject nativeLink,
                    LINK_NAME_ARG(const char* qt_name)
                    void *ptr, bool created_by_java, bool is_shell,
                    PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : PointerToContainerLink(env, nativeLink,
                   LINK_NAME_ARG(qt_name)
                   ptr, created_by_java, is_shell, containerAccess, ocurredException), m_deleter_function(destructor_function) {
    Q_ASSERT(destructor_function);
}

void DeletablePointerToContainerLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if (m_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "DeletablePointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
        if(!isShell())
            clearAllDependencies(env);
        unregisterPointer(m_pointer);
        unregisterOffsets();
        m_flags.setFlag(Flag::IsPointerRegistered, false);
        if(!m_flags.testFlag(Flag::NoNativeDeletion) && (m_flags.testFlag(Flag::JavaOwnership) || forced)){
            void* pointer = m_pointer;
            m_pointer = nullptr;
            QWriteLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            if(const QObject* obj = m_containerAccess->getOwner(pointer)){
                const QObjectPrivate* p = QObjectPrivate::get(obj);
                Q_ASSERT(p);
                QThread* ownerThread =
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
                        p->threadData->thread;
#else
                        p->threadData.loadAcquire()->thread.loadAcquire();
#endif
                if (ownerThread->isRunning() && ownerThread != QThread::currentThread()) {
                    if(ValueOwnerUserData* vud = QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, obj)){
                        if(p->wasDeleted){
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("call destructor_function on object %p", pointer)
                            QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
                            PtrDeleterFunction _deleter_function = m_deleter_function;
                            void* _pointer = pointer;
                            bool isShell = this->isShell();
                            vud->addDeleter([_deleter_function,_pointer,isShell](){
                                _deleter_function(_pointer, isShell);
                            });
                            dispose();
                        }else{
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor for object %p", pointer)
                            setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                            QThreadUserData::Result threadDataResult = QThreadUserData::ensureThreadUserDataLocked(ownerThread);
                            if (QAbstractEventDispatcher::instance(ownerThread)) {
                                DestructorEvent* event = new DestructorEvent(vud->pointer(), m_this, pointer, m_deleter_function, isShell());
                                if(threadDataResult.initRequired){
                                    locker.unlock();
                                    threadDataResult.threadUserData->initialize(ownerThread);
                                    locker.relock();
                                }
                                QCoreApplication::postEvent(threadDataResult.threadUserData->threadDeleter(), event);
                            }else{
                                PtrDeleterFunction _deleter_function = m_deleter_function;
                                void* _pointer = pointer;
                                bool isShell = this->isShell();
                                threadDataResult.threadUserData->doAtThreadEnd([_deleter_function, _pointer, isShell](){
                                    _deleter_function(_pointer, isShell);
                                });
                                if(threadDataResult.initRequired){
                                    locker.unlock();
                                    threadDataResult.threadUserData->initialize(ownerThread);
                                    locker.relock();
                                }
                            }
                        }
                        pointer = nullptr;
                    }else if(!p->wasDeleted){
                        QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor for object %p", pointer)
                        setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                        QThreadUserData::Result threadDataResult = QThreadUserData::ensureThreadUserDataLocked(ownerThread);
                        if (QAbstractEventDispatcher::instance(ownerThread)) {
                            DestructorEvent* event = new DestructorEvent(obj, m_this, pointer, m_deleter_function, isShell());
                            if(threadDataResult.initRequired){
                                locker.unlock();
                                threadDataResult.threadUserData->initialize(ownerThread);
                                locker.relock();
                            }
                            QCoreApplication::postEvent(threadDataResult.threadUserData->threadDeleter(), event);
                        }else{
                            PtrDeleterFunction _deleter_function = m_deleter_function;
                            void* _pointer = pointer;
                            bool isShell = this->isShell();
                            threadDataResult.threadUserData->doAtThreadEnd([_deleter_function, _pointer, isShell](){
                                _deleter_function(_pointer, isShell);
                            });
                            if(threadDataResult.initRequired){
                                locker.unlock();
                                threadDataResult.threadUserData->initialize(ownerThread);
                                locker.relock();
                            }
                        }
                        pointer = nullptr;
                    }
                }
            }
            if(pointer){
                QTJAMBI_DEBUG_PRINT_WITH_ARGS("call destructor_function on object %p", pointer)
                QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
                locker.unlock();
                m_deleter_function(pointer, isShell());
                locker.relock();
                dispose();
            }
        }else{
            dispose();
        }
    }
}

DeletableOwnedPointerToContainerLink::DeletableOwnedPointerToContainerLink(JNIEnv *env, jobject nativeLink,
                    LINK_NAME_ARG(const char* qt_name)
                    void *ptr, bool created_by_java, bool is_shell,
                    PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : DeletablePointerToContainerLink(env, nativeLink,
                                                           LINK_NAME_ARG(qt_name)
                                                           ptr, created_by_java, is_shell, destructor_function, containerAccess, ocurredException), m_owner_function(ownerFunction) {
    Q_ASSERT(ownerFunction);
}

void DeletableOwnedPointerToContainerLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if (m_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "DeletablePointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
        if(!isShell())
            clearAllDependencies(env);
        unregisterPointer(m_pointer);
        unregisterOffsets();
        m_flags.setFlag(Flag::IsPointerRegistered, false);
        if(!m_flags.testFlag(Flag::NoNativeDeletion) && (m_flags.testFlag(Flag::JavaOwnership) || forced)){
            void* pointer = m_pointer;
            m_pointer = nullptr;
            QWriteLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            const QObject* obj = m_owner_function(pointer);
            if(!obj)
                obj = m_containerAccess->getOwner(pointer);
            if(obj){
                const QObjectPrivate* p = QObjectPrivate::get(obj);
                Q_ASSERT(p);
                QThread* ownerThread =
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
                        p->threadData->thread;
#else
                        p->threadData.loadAcquire()->thread.loadAcquire();
#endif
                if (ownerThread->isRunning() && ownerThread != QThread::currentThread()) {
                    if(ValueOwnerUserData* vud = QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, obj)){
                        if(p->wasDeleted){
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("call destructor_function on object %p", pointer)
                            QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
                            PtrDeleterFunction _deleter_function = m_deleter_function;
                            void* _pointer = pointer;
                            bool isShell = this->isShell();
                            vud->addDeleter([_deleter_function,_pointer,isShell](){
                                _deleter_function(_pointer, isShell);
                            });
                            dispose();
                        }else{
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor for object %p", pointer)
                            setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                            QThreadUserData::Result threadDataResult = QThreadUserData::ensureThreadUserDataLocked(ownerThread);
                            if (QAbstractEventDispatcher::instance(ownerThread)) {
                                DestructorEvent* event = new DestructorEvent(vud->pointer(), m_this, pointer, m_deleter_function, isShell());
                                if(threadDataResult.initRequired){
                                    locker.unlock();
                                    threadDataResult.threadUserData->initialize(ownerThread);
                                    locker.relock();
                                }
                                QCoreApplication::postEvent(threadDataResult.threadUserData->threadDeleter(), event);
                            }else{
                                PtrDeleterFunction _deleter_function = m_deleter_function;
                                void* _pointer = pointer;
                                bool isShell = this->isShell();
                                threadDataResult.threadUserData->doAtThreadEnd([_deleter_function, _pointer, isShell](){
                                    _deleter_function(_pointer, isShell);
                                });
                                if(threadDataResult.initRequired){
                                    locker.unlock();
                                    threadDataResult.threadUserData->initialize(ownerThread);
                                    locker.relock();
                                }
                            }
                        }
                        pointer = nullptr;
                    }else if(!p->wasDeleted){
                        QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor for object %p", pointer)
                        setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                        QThreadUserData::Result threadDataResult = QThreadUserData::ensureThreadUserDataLocked(ownerThread);
                        if (QAbstractEventDispatcher::instance(ownerThread)) {
                            DestructorEvent* event = new DestructorEvent(obj, m_this, pointer, m_deleter_function, isShell());
                            if(threadDataResult.initRequired){
                                locker.unlock();
                                threadDataResult.threadUserData->initialize(ownerThread);
                                locker.relock();
                            }
                            QCoreApplication::postEvent(threadDataResult.threadUserData->threadDeleter(), event);
                        }else{
                            PtrDeleterFunction _deleter_function = m_deleter_function;
                            void* _pointer = pointer;
                            bool isShell = this->isShell();
                            threadDataResult.threadUserData->doAtThreadEnd([_deleter_function, _pointer, isShell](){
                                _deleter_function(_pointer, isShell);
                            });
                            if(threadDataResult.initRequired){
                                locker.unlock();
                                threadDataResult.threadUserData->initialize(ownerThread);
                                locker.relock();
                            }
                        }
                        pointer = nullptr;
                    }
                }
            }
            if(pointer){
                QTJAMBI_DEBUG_PRINT_WITH_ARGS("call destructor_function on object %p", pointer)
                QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
                locker.unlock();
                m_deleter_function(pointer, isShell());
                locker.relock();
                dispose();
            }
        }else{
            dispose();
        }
    }
}

OwnedMetaTypedPointerToObjectLink::OwnedMetaTypedPointerToObjectLink(JNIEnv *env, jobject nativeLink,
                    void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell,
                    PtrOwnerFunction ownerFunction, JavaException& ocurredException)
    : MetaTypedPointerToObjectLink(env, nativeLink,
                                                           ptr, metaType, created_by_java, is_shell, ocurredException), m_owner_function(ownerFunction) {
    Q_ASSERT(m_owner_function);
}

DeletableOwnedPointerToObjectLink::DeletableOwnedPointerToObjectLink(JNIEnv *env, jobject nativeLink,
                    LINK_NAME_ARG(const char* qt_name)
                    void *ptr, bool created_by_java, bool is_shell,
                    PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
    : PointerToObjectLink(env, nativeLink,
                                                            LINK_NAME_ARG(qt_name)
                                                            ptr, created_by_java, is_shell,
                                                            ocurredException), m_owner_function(ownerFunction), m_deleter_function(destructor_function) {
    Q_ASSERT(m_owner_function);
    Q_ASSERT(m_deleter_function);
}

DeletablePointerToObjectLink::DeletablePointerToObjectLink(JNIEnv *env, jobject nativeLink,
                    LINK_NAME_ARG(const char* qt_name)
                    void *ptr, bool created_by_java, bool is_shell,
                    PtrDeleterFunction destructor_function, JavaException& ocurredException)
    : PointerToObjectLink(env, nativeLink,
                                                            LINK_NAME_ARG(qt_name)
                                                            ptr, created_by_java, is_shell,
                                                            ocurredException), m_deleter_function(destructor_function) {
    Q_ASSERT(m_deleter_function);
}

PointerToObjectLink::PointerToObjectLink(JNIEnv *env, jobject nativeLink,
                                         LINK_NAME_ARG(const char *qt_name)
                                         void *ptr, bool created_by_java, bool is_shell, JavaException& ocurredException) :
    QtJambiLink(env, nativeLink,
                LINK_NAME_ARG(qt_name)
                created_by_java, false, is_shell, ocurredException),
    m_pointer(ptr)
{
    if(!ocurredException){
        if(created_by_java){
            registerPointer(m_pointer);
        }
    }else{
        m_flags.setFlag(Flag::IsJavaLinkDetached);
        m_pointer = nullptr;
    }
}

PointerToObjectLink::~PointerToObjectLink()
{
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "PointerToObjectLink::~PointerToObjectLink()")
    try{
        if(JniEnvironment env = noThreadInitializationOnPurge() ? JniEnvironment{300} : DefaultJniEnvironment{300}){
            releaseJavaObject(env);
            deleteNativeObject(env);
            detachJavaLink(env);
        }else{
            releaseJavaObject(nullptr);
            deleteNativeObject(nullptr);
            detachJavaLink(nullptr);
        }
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
    } catch (...) {
    }
}

bool PointerToObjectLink::isSmartPointer() const { return false; }

DependentLink::~DependentLink(){}

inline hash_type qHash(const QWeakPointer<QtJambiLink>& ptr, hash_type seed = 0){
    return qHash(ptr.lock().get(), seed);
}

QTJAMBI_OBJECTUSERDATA_ID_IMPL(,DependencyManagerUserData::)

DependencyManagerUserData::DependencyManagerUserData() : QtJambiObjectData() {
}

DependencyManagerUserData::~DependencyManagerUserData(){
    QSet<QWeakPointer<QtJambiLink>> dependentObjects;
    QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>> finalizations;
    QSet<QWeakPointer<QtJambiLink>> qpropertyObjects;
    {
        QWriteLocker locker(QtJambiLinkUserData::lock());
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

void DependencyManagerUserData::invalidateDependentObjects(QObject* obj, JNIEnv* env){
    QSet<QWeakPointer<QtJambiLink>> dependentObjects;
    bool deleteThis = false;
    {
        QWriteLocker locker(QtJambiLinkUserData::lock());
        dependentObjects.swap(m_dependentObjects);
        if(m_condestroyedObjects.isEmpty() && m_finalizations.isEmpty()){
            QTJAMBI_SET_OBJECTUSERDATA(DependencyManagerUserData, obj, nullptr);
            deleteThis = true;
        }
    }
    for(const QWeakPointer<QtJambiLink>& weakLink : qAsConst(dependentObjects)){
        if(QSharedPointer<QtJambiLink> link = weakLink.toStrongRef()){
            link->invalidate(env);
        }
    }
    dependentObjects.clear();
    if(deleteThis)
        delete this;
}

void DependencyManagerUserData::clear(QObject* obj){
    QSet<QWeakPointer<QtJambiLink>> dependentObjects;
    QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>> finalizations;
    QSet<QWeakPointer<QtJambiLink>> condestroyedObjects;
    {
        QWriteLocker locker(QtJambiLinkUserData::lock());
        dependentObjects.swap(m_dependentObjects);
        finalizations.swap(m_finalizations);
        condestroyedObjects.swap(m_condestroyedObjects);
        QTJAMBI_SET_OBJECTUSERDATA(DependencyManagerUserData, obj, nullptr);
    }
    if(!dependentObjects.isEmpty()
            || !finalizations.isEmpty()
            || !condestroyedObjects.isEmpty()){
        if(JniEnvironment env{200}){
            clear(env, dependentObjects, finalizations, condestroyedObjects);
        }
    }
    delete this;
}

void DependencyManagerUserData::clear(QObject* obj, JNIEnv* env){
    QSet<QWeakPointer<QtJambiLink>> dependentObjects;
    QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>> finalizations;
    QSet<QWeakPointer<QtJambiLink>> condestroyedObjects;
    {
        QWriteLocker locker(QtJambiLinkUserData::lock());
        dependentObjects.swap(m_dependentObjects);
        finalizations.swap(m_finalizations);
        condestroyedObjects.swap(m_condestroyedObjects);
        QTJAMBI_SET_OBJECTUSERDATA(DependencyManagerUserData, obj, nullptr);
    }
    clear(env, dependentObjects, finalizations, condestroyedObjects);
    delete this;
}

void DependencyManagerUserData::clear(JNIEnv* env,
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

void DependencyManagerUserData::addDependentObject(const QSharedPointer<QtJambiLink>& dependent){
    QWriteLocker locker(QtJambiLinkUserData::lock());
    m_dependentObjects.insert(dependent.toWeakRef());
    if(DependentLink* dlink = dynamic_cast<DependentLink*>(dependent.data())){
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

void DependencyManagerUserData::removeDependentObject(const QSharedPointer<QtJambiLink>& dependent){
    QWriteLocker locker(QtJambiLinkUserData::lock());
    m_dependentObjects.remove(dependent.toWeakRef());
    if(DependentLink* dlink = dynamic_cast<DependentLink*>(dependent.data())){
        dlink->setDependencyManager(nullptr);
    }
}

void DependencyManagerUserData::addConDestroyedObject(const QSharedPointer<QtJambiLink>& link){
    QWriteLocker locker(QtJambiLinkUserData::lock());
    m_condestroyedObjects.insert(link.toWeakRef());
}

void DependencyManagerUserData::removeConDestroyedObject(const QSharedPointer<QtJambiLink>& link){
    QWriteLocker locker(QtJambiLinkUserData::lock());
    m_condestroyedObjects.remove(link.toWeakRef());
}

void DependencyManagerUserData::addFinalization(void* data, FinalizationExecutor executor, FinalizationDeleter deleter){
    m_finalizations[data] = {executor, deleter};
}

void DependencyManagerUserData::addFinalization(JNIEnv* env, jobject data, FinalizationExecutor executor, FinalizationDeleter deleter){
    m_finalizations[data] = {executor, deleter};
    m_finalizationData[Java::Runtime::System::identityHashCode(env, data)] = data;
}

void DependencyManagerUserData::removeFinalization(JNIEnv* env, void* data, bool execute){
    QPair<FinalizationExecutor,FinalizationDeleter> fun = m_finalizations[data];
    if(m_finalizations.remove(data)){
        if(execute && fun.first)
            fun.first(env, data);
        if(fun.second)
            fun.second(env, data);
    }
}

void DependencyManagerUserData::removeFinalization(JNIEnv* env, jobject data, bool execute){
    QPair<FinalizationExecutor,FinalizationDeleter> fun = m_finalizations[data];
    if(m_finalizations.remove(data)){
        if(execute && fun.first)
            fun.first(env, data);
        if(fun.second)
            fun.second(env, data);
    }
}

bool DependencyManagerUserData::hasDependencies() const{
    return !m_dependentObjects.empty() && !m_condestroyedObjects.empty();
}

DependencyManagerUserData* DependencyManagerUserData::instance(const QObject* object, bool forceConstruction){
    DependencyManagerUserData* dm{nullptr};
    {
        QReadLocker locker(QtJambiLinkUserData::lock());
        dm = QTJAMBI_GET_OBJECTUSERDATA(DependencyManagerUserData, object);
    }
    if(!dm && forceConstruction){
        QWriteLocker locker(QtJambiLinkUserData::lock());
        if(!(dm = QTJAMBI_GET_OBJECTUSERDATA(DependencyManagerUserData, object))){
            QTJAMBI_SET_OBJECTUSERDATA(DependencyManagerUserData, const_cast<QObject*>(object), dm = new DependencyManagerUserData());
        }
    }
    return dm;
}

Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gDependencyLock, (QReadWriteLock::Recursive))
QReadWriteLock* dependencyLock(){
    return gDependencyLock();
}
typedef SecureContainer<QMultiHash<QSharedPointer<QtJambiLink>, QWeakPointer<QtJambiLink>>,QReadWriteLock,&dependencyLock> DependencyHash;
Q_GLOBAL_STATIC(DependencyHash, gDependencies)
Q_GLOBAL_STATIC(DependencyHash, gConDestroyedDependencies)
typedef SecureContainer<QHash<jint,void*>,QReadWriteLock,&dependencyLock> FinalizationDataHash;
typedef SecureContainer<QHash<QSharedPointer<QtJambiLink>, QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>>>,QReadWriteLock,&dependencyLock> FinalizationHash;
Q_GLOBAL_STATIC(FinalizationHash, gFinalizations)
Q_GLOBAL_STATIC(FinalizationDataHash, gFinalizationData)

void QtJambiLink::registerConDestroyedObject(JNIEnv * env, const QObject* object, const QSharedPointer<QtJambiLink>& link){
    if(!link->m_flags.testFlag(Flag::IsManaged)){
        DependencyManagerUserData* dm = DependencyManagerUserData::instance(object);
        dm->addConDestroyedObject(link);
        link->setCppOwnership(env);
        link->m_flags.setFlag(Flag::NoNativeDeletion);
        link->m_flags.setFlag(Flag::IsManaged);
    }
}
void QtJambiLink::unregisterConDestroyedObject(JNIEnv * env, const QObject* object, const QSharedPointer<QtJambiLink>& link){
    if(link->m_flags.testFlag(Flag::IsManaged)){
        DependencyManagerUserData* dm = DependencyManagerUserData::instance(object);
        dm->removeConDestroyedObject(link);
        link->setJavaOwnership(env);
        link->m_flags.setFlag(Flag::NoNativeDeletion, false);
        link->m_flags.setFlag(Flag::IsManaged, false);
    }
}

void QtJambiLink::registerDependentObject(const QObject* object, const QSharedPointer<QtJambiLink>& link){
    if(object && !link->isShell() && !link->isSmartPointer() && !link->m_flags.testFlag(Flag::IsDependent)){
        if(!link->isQObject()){
            DependencyManagerUserData* dm = DependencyManagerUserData::instance(object);
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
            if(DependencyManagerUserData* dm = DependencyManagerUserData::instance(object, false)){
                dm->removeDependentObject(link);
            }
        }
        link->m_flags.setFlag(Flag::NoNativeDeletion, false);
        link->m_flags.setFlag(Flag::IsDependent, false);
    }
}

void QtJambiLink::addFinalization(const QObject* object, void* data, FinalizationExecutor executor, FinalizationDeleter deleter){
    if(object){
        if(DependencyManagerUserData* dm = DependencyManagerUserData::instance(object)){
            dm->addFinalization(data, executor, deleter);
        }
    }
}

void QtJambiLink::addFinalization(const QObject* object, JNIEnv * env, jobject data, FinalizationExecutor executor, FinalizationDeleter deleter){
    if(object){
        if(DependencyManagerUserData* dm = DependencyManagerUserData::instance(object)){
            dm->addFinalization(env, data, executor, deleter);
        }
    }
}

void QtJambiLink::addFinalization(void* data, FinalizationExecutor executor, FinalizationDeleter deleter){
    if(isQObject()){
        addFinalization(qobject(), data, executor, deleter);
    }else{
        if(!m_flags.testFlag(Flag::HasDependencies))
            m_flags.setFlag(Flag::HasDependencies);
        QWriteLocker locker(gDependencyLock());
        Q_UNUSED(locker)
        QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>>& container = (*gFinalizations)[m_this];
        container[data] = {executor, deleter};
    }
}

void QtJambiLink::addFinalization(JNIEnv * env, jobject data, FinalizationExecutor executor, FinalizationDeleter deleter){
    if(isQObject()){
        addFinalization(qobject(), env, data, executor, deleter);
    }else{
        if(!m_flags.testFlag(Flag::HasDependencies))
            m_flags.setFlag(Flag::HasDependencies);
        QWriteLocker locker(gDependencyLock());
        Q_UNUSED(locker)
        QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>>& container = (*gFinalizations)[m_this];
        container[data] = {executor, deleter};
        (*gFinalizationData)[Java::Runtime::System::identityHashCode(env, data)] = data;
    }
}

void QtJambiLink::removeFinalization(JNIEnv * env, void* data, bool execute){
    if(isQObject()){
        removeFinalization(qobject(), env, data);
    }else{
        if(!m_flags.testFlag(Flag::HasDependencies))
            m_flags.setFlag(Flag::HasDependencies);
        QWriteLocker locker(gDependencyLock());
        Q_UNUSED(locker)
        QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>>& container = (*gFinalizations)[m_this];
        QPair<FinalizationExecutor,FinalizationDeleter> fun = container.take(data);
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
        QWriteLocker locker(gDependencyLock());
        Q_UNUSED(locker)
        QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>>& container = (*gFinalizations)[m_this];
        void* ptr = gFinalizationData->take(Java::Runtime::System::identityHashCode(env, data));
        QPair<FinalizationExecutor,FinalizationDeleter> fun = container.take(ptr);
        if(execute && fun.first)
            fun.first(env, data);
        if(fun.second)
            fun.second(env, data);
    }
}

void QtJambiLink::removeFinalization(const QObject* object, JNIEnv * env, void* data, bool execute){
    if(object){
        if(DependencyManagerUserData* ddata = DependencyManagerUserData::instance(object, false)){
            ddata->removeFinalization(env, data, execute);
        }
    }
}

void QtJambiLink::removeFinalization(const QObject* object, JNIEnv * env, jobject data, bool execute){
    if(object){
        if(DependencyManagerUserData* ddata = DependencyManagerUserData::instance(object, false)){
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
            QWriteLocker locker(gDependencyLock());
            Q_UNUSED(locker)
            gDependencies->insert(m_this, link);
            gDependencies->remove(m_this, QWeakPointer<QtJambiLink>{});
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
            QWriteLocker locker(gDependencyLock());
            Q_UNUSED(locker)
            gDependencies->remove(m_this, link);
            gDependencies->remove(m_this, QWeakPointer<QtJambiLink>{});
            if(!gDependencies->contains(m_this))
                m_flags.setFlag(Flag::HasDependencies, false);
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
        if(!m_flags.testFlag(Flag::HasDependencies))
            m_flags.setFlag(Flag::HasDependencies);
        QWriteLocker locker(gDependencyLock());
        Q_UNUSED(locker)
        gConDestroyedDependencies->insert(m_this, link);
        gConDestroyedDependencies->remove(m_this, QWeakPointer<QtJambiLink>{});
        link->setCppOwnership(env);
        link->m_flags.setFlag(Flag::NoNativeDeletion);
        link->m_flags.setFlag(Flag::IsManaged);
    }
}

void QtJambiLink::unregisterConDestroyedObject(JNIEnv * env, const QSharedPointer<QtJambiLink>& link)
{
    if(isQObject()){
        unregisterConDestroyedObject(env, qobject(), link);
    }else if(link->m_flags.testFlag(Flag::IsManaged)){
        QWriteLocker locker(gDependencyLock());
        Q_UNUSED(locker)
        gConDestroyedDependencies->remove(m_this, link);
        gConDestroyedDependencies->remove(m_this, QWeakPointer<QtJambiLink>{});
        if(!gConDestroyedDependencies->contains(m_this))
            m_flags.setFlag(Flag::HasDependencies, false);
        link->setJavaOwnership(env);
        link->m_flags.setFlag(Flag::IsManaged, false);
        link->m_flags.setFlag(Flag::NoNativeDeletion, false);
    }
}

void QtJambiLink::invalidateDependentObjects(JNIEnv *env)
{
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "QtJambiLink::invalidateDependentObjects(JNIEnv *)")
    if(isQObject()){
#ifdef AVAILABLE_IN_DELETION
        if(!isInDestructor())
#endif
        if(QObject* obj = qobject()){
            if(DependencyManagerUserData* dm = DependencyManagerUserData::instance(obj, false)){
                dm->invalidateDependentObjects(obj, env);
            }
        }
    }else{
        if(m_flags.testFlag(Flag::HasDependencies)){
            QList<QWeakPointer<QtJambiLink>> dependentObjects;
            {
                QWriteLocker locker(gDependencyLock());
                Q_UNUSED(locker)
                dependentObjects = gDependencies->values(m_this);
                gDependencies->remove(m_this);
            }
            for(const QWeakPointer<QtJambiLink>& weakLink : qAsConst(dependentObjects)){
                if(QSharedPointer<QtJambiLink> link = weakLink.toStrongRef()){
                    link->invalidate(env);
                }
            }
        }
    }
}

void QtJambiLink::clearAllDependencies(JNIEnv *env)
{
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "QtJambiLink::clearAllDependencies(JNIEnv *)")
    if(isQObject()){
#ifdef AVAILABLE_IN_DELETION
        if(!isInDestructor())
#endif
        if(QObject* obj = qobject()){
            if(DependencyManagerUserData* dm = DependencyManagerUserData::instance(obj, false)){
                dm->clear(obj, env);
            }
        }
    }else{
        if(m_flags.testFlag(Flag::HasDependencies)){
            QList<QWeakPointer<QtJambiLink>> dependentObjects;
            QList<QWeakPointer<QtJambiLink>> conDestroyedObjects;
            QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>> finalizations;
            {
                QWriteLocker locker(gDependencyLock());
                Q_UNUSED(locker)
                dependentObjects = gDependencies->values(m_this);
                gDependencies->remove(m_this);
                finalizations = gFinalizations->take(m_this);
                conDestroyedObjects = gConDestroyedDependencies->values(m_this);
                gConDestroyedDependencies->remove(m_this);
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
}

void QtJambiLink::clearAllDependencies(){
    if(!isInDestructor()){
        if(isQObject()){
            if(QObject* obj = qobject()){
                if(DependencyManagerUserData* dm = DependencyManagerUserData::instance(obj, false)){
                    dm->clear(obj);
                }
            }
        }else{
            if(m_flags.testFlag(Flag::HasDependencies)){
                QList<QWeakPointer<QtJambiLink>> dependentObjects;
                QList<QWeakPointer<QtJambiLink>> conDestroyedObjects;
                QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>> finalizations;
                {
                    QWriteLocker locker(gDependencyLock());
                    Q_UNUSED(locker)
                    dependentObjects = gDependencies->values(m_this);
                    gDependencies->remove(m_this);
                    finalizations = gFinalizations->take(m_this);
                    conDestroyedObjects = gConDestroyedDependencies->values(m_this);
                    gConDestroyedDependencies->remove(m_this);
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

void *PointerToObjectLink::pointer() const {
    return m_pointer;
}

QObject *PointerToObjectLink::qobject() const { Q_ASSERT_X(false, "QtJambiLink", "Pointer is not QObject"); return nullptr; }

void PointerToObjectLink::invalidate(JNIEnv *env) {
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "PointerToObjectLink::invalidate(JNIEnv *)")
    invalidateDependentObjects(env);
    releaseJavaObject(env);
    detachJavaLink(env);
    if(m_pointer) {
        QTJAMBI_DEBUG_PRINT_WITH_ARGS("invalidate object %p of link %p", m_pointer, this)
        unregisterPointer(m_pointer);
        unregisterOffsets();
        m_flags.setFlag(Flag::IsPointerRegistered, false);
        m_pointer = nullptr;
    }
    dispose();
}

void PointerToObjectLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if (m_pointer){
        if(!isShell())
            clearAllDependencies(env);
        unregisterPointer(m_pointer);
        unregisterOffsets();
        m_flags.setFlag(Flag::IsPointerRegistered, false);
        if(!m_flags.testFlag(Flag::NoNativeDeletion) && (m_flags.testFlag(Flag::JavaOwnership) || forced)){
            m_pointer = nullptr;
            dispose();
            Java::Runtime::RuntimeException::throwNew(env, "Unable to delete object due to missing deleter or meta type information." QTJAMBI_STACKTRACEINFO );
        }else{
            dispose();
        }
    }
}

void PointerToObjectLink::onClean(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::HasBeenCleaned)){
        if(!m_flags.testFlag(Flag::BlockDeletion)){
            JavaException throwable;
            m_flags.setFlag(Flag::BlockDeletion, true);
            QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "QtJambiLink::onClean(JNIEnv *)")
            QTJAMBI_INCREASE_COUNTER_THIS(cleanCallerCount)
            releaseJavaObject(env);
            deleteNativeObject(env);
            if(env->ExceptionCheck()){
                jthrowable t = env->ExceptionOccurred();
                env->ExceptionClear();
                throwable = JavaException(env, t);
            }
            detachJavaLink(env);
            m_flags.setFlag(Flag::HasBeenCleaned);
            m_flags.setFlag(Flag::BlockDeletion, false);
            throwable.raise();
        }
    }
}

// This allows the Java language to attempt to expedite the disposal of the C++ object
//  (when that this possible), however its possible the C++ object will not be destroyed
//  as the Java side does not own it.
// Entry point for QtJambiObject#dispose() method
void PointerToObjectLink::onDispose(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::IsManaged)){
        if(!m_flags.testFlag(Flag::BlockDeletion)){
            JavaException throwable;
            m_flags.setFlag(Flag::BlockDeletion, true);
            QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "QtJambiLink::onDispose(JNIEnv *)")
            setJavaOwnership(env);
            deleteNativeObject(env, true);
            if(env->ExceptionCheck()){
                jthrowable t = env->ExceptionOccurred();
                env->ExceptionClear();
                throwable = JavaException(env, t);
            }
            detachJavaLink(env);
            QTJAMBI_INCREASE_COUNTER_THIS(disposeCalledCount)
            m_flags.setFlag(Flag::BlockDeletion, false);
            throwable.raise();
        }
    }
}

void PointerToObjectLink::init(JNIEnv* env){
    //if(!isInitialized()){
        QtJambiLink::init(env);
    //}
}

void OwnedMetaTypedPointerToObjectLink::init(JNIEnv* env){
    if(!isInitialized()){
        PointerToObjectLink::init(env);
        if(const QObject* obj = m_owner_function(pointer())){
            const QObjectPrivate* p = QObjectPrivate::get(obj);
            if(p && !p->wasDeleted){
                ValueOwnerUserData* vod;
                {
                    QReadLocker locker(QtJambiLinkUserData::lock());
                    Q_UNUSED(locker)
                    vod = QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, obj);
                }
                if(!vod){
                    QWriteLocker locker(QtJambiLinkUserData::lock());
                    Q_UNUSED(locker)
                    vod = QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, obj);
                    if(!vod)
                        QTJAMBI_SET_OBJECTUSERDATA(ValueOwnerUserData, const_cast<QObject*>(obj), new ValueOwnerUserData(obj));
                }
            }
        }
    }
}

void DeletableOwnedPointerToObjectLink::init(JNIEnv* env){
    if(!isInitialized()){
        PointerToObjectLink::init(env);
        if(const QObject* obj = m_owner_function(pointer())){
            const QObjectPrivate* p = QObjectPrivate::get(obj);
            if(p && !p->wasDeleted){
                ValueOwnerUserData* vod;
                {
                    QReadLocker locker(QtJambiLinkUserData::lock());
                    Q_UNUSED(locker)
                    vod = QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, obj);
                }
                if(!vod){
                    QWriteLocker locker(QtJambiLinkUserData::lock());
                    Q_UNUSED(locker)
                    vod = QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, obj);
                    if(!vod)
                        QTJAMBI_SET_OBJECTUSERDATA(ValueOwnerUserData, const_cast<QObject*>(obj), new ValueOwnerUserData(obj));
                }
            }
        }
    }
}

void MetaTypedPointerToObjectLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if (m_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "MetaTypedPointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
        if(!isShell())
            clearAllDependencies(env);
        unregisterPointer(m_pointer);
        unregisterOffsets();
        m_flags.setFlag(Flag::IsPointerRegistered, false);
        if(!m_flags.testFlag(Flag::NoNativeDeletion) && (m_flags.testFlag(Flag::JavaOwnership) || forced)){
            void* pointer = m_pointer;
            m_pointer = nullptr;
            QTJAMBI_DEBUG_PRINT_WITH_ARGS("call QMetaType::destroy() on object %p of type %s", pointer, static_cast<const char*>(m_meta_type.name()))
            m_meta_type.destroy(pointer);
            dispose();
        }else{
            dispose();
        }
    }
}

void OwnedMetaTypedPointerToObjectLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if (m_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "MetaTypedPointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
        if(!isShell())
            clearAllDependencies(env);
        unregisterPointer(m_pointer);
        unregisterOffsets();
        m_flags.setFlag(Flag::IsPointerRegistered, false);
        if(!m_flags.testFlag(Flag::NoNativeDeletion) && (m_flags.testFlag(Flag::JavaOwnership) || forced)){
            void* pointer = m_pointer;
            m_pointer = nullptr;
            QWriteLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            if(const QObject* obj = m_owner_function(pointer)){
                const QObjectPrivate* p = QObjectPrivate::get(obj);
                Q_ASSERT(p);
                QThread* ownerThread =
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
                        p->threadData->thread;
#else
                        p->threadData.loadAcquire()->thread.loadAcquire();
#endif
                if (ownerThread->isRunning() && ownerThread != QThread::currentThread()) {
                    if(ValueOwnerUserData* vud = QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, obj)){
                        if(p->wasDeleted){
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("call QMetaType::destroy() on object %p of type %s", pointer, static_cast<const char*>(m_meta_type.name()))
                            void* _pointer = pointer;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                            QMetaType meta_type = m_meta_type;
                            vud->addDeleter([meta_type,_pointer](){
                                meta_type.destroy(_pointer);
                            });
#else
                            int meta_type = m_meta_type.id();
                            vud->addDeleter([meta_type,_pointer](){
                                QMetaType::destroy(meta_type, _pointer);
                            });
#endif
                            dispose();
                        }else{
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor for object %p of type %s", pointer, static_cast<const char*>(m_meta_type.name()))
                            setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                            QThreadUserData::Result threadDataResult = QThreadUserData::ensureThreadUserDataLocked(ownerThread);
                            if (QAbstractEventDispatcher::instance(ownerThread)) {
                                MetaTypeDestructionEvent* event = new MetaTypeDestructionEvent(vud->pointer(), m_this, pointer, m_meta_type);
                                if(threadDataResult.initRequired){
                                    locker.unlock();
                                    threadDataResult.threadUserData->initialize(ownerThread);
                                    locker.relock();
                                }
                                QCoreApplication::postEvent(threadDataResult.threadUserData->threadDeleter(), event);
                            }else{
                                void* _pointer = pointer;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                                QMetaType meta_type = m_meta_type;
                                threadDataResult.threadUserData->doAtThreadEnd([meta_type,_pointer](){
                                    meta_type.destroy(_pointer);
                                });
#else
                                int meta_type = m_meta_type.id();
                                threadDataResult.threadUserData->doAtThreadEnd([meta_type,_pointer](){
                                    QMetaType::destroy(meta_type, _pointer);
                                });
#endif
                                if(threadDataResult.initRequired){
                                    locker.unlock();
                                    threadDataResult.threadUserData->initialize(ownerThread);
                                    locker.relock();
                                }
                            }
                        }
                        pointer = nullptr;
                    }else if(!p->wasDeleted){
                        QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor for object %p of type %s", pointer, static_cast<const char*>(m_meta_type.name()))
                        setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                        QThreadUserData::Result threadDataResult = QThreadUserData::ensureThreadUserDataLocked(ownerThread);
                        if (QAbstractEventDispatcher::instance(ownerThread)) {
                            MetaTypeDestructionEvent* event = new MetaTypeDestructionEvent(obj, m_this, pointer, m_meta_type);
                            if(threadDataResult.initRequired){
                                locker.unlock();
                                threadDataResult.threadUserData->initialize(ownerThread);
                                locker.relock();
                            }
                            QCoreApplication::postEvent(threadDataResult.threadUserData->threadDeleter(), event);
                        }else{
                            void* _pointer = pointer;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                            QMetaType meta_type = m_meta_type;
                            threadDataResult.threadUserData->doAtThreadEnd([meta_type,_pointer](){
                                meta_type.destroy(_pointer);
                            });
#else
                            int meta_type = m_meta_type.id();
                            threadDataResult.threadUserData->doAtThreadEnd([meta_type,_pointer](){
                                QMetaType::destroy(meta_type, _pointer);
                            });
#endif
                            if(threadDataResult.initRequired){
                                locker.unlock();
                                threadDataResult.threadUserData->initialize(ownerThread);
                                locker.relock();
                            }
                        }
                        pointer = nullptr;
                    }
                }
            }
            if(pointer){
                QTJAMBI_DEBUG_PRINT_WITH_ARGS("call QMetaType::destroy() on object %p of type %s", pointer, static_cast<const char*>(m_meta_type.name()))
                locker.unlock();
                m_meta_type.destroy(pointer);
                locker.relock();
                dispose();
            }
        }else{
            dispose();
        }
    }
}

void DeletableOwnedPointerToObjectLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if (m_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "DeletablePointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
        if(!isShell())
            clearAllDependencies(env);
        unregisterPointer(m_pointer);
        unregisterOffsets();
        m_flags.setFlag(Flag::IsPointerRegistered, false);
        if(!m_flags.testFlag(Flag::NoNativeDeletion) && (m_flags.testFlag(Flag::JavaOwnership) || forced)){
            void* pointer = m_pointer;
            m_pointer = nullptr;
            QWriteLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            if(const QObject* obj = m_owner_function(pointer)){
                const QObjectPrivate* p = QObjectPrivate::get(obj);
                Q_ASSERT(p);
                QThread* ownerThread =
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
                        p->threadData->thread;
#else
                        p->threadData.loadAcquire()->thread.loadAcquire();
#endif
                if (ownerThread->isRunning() && ownerThread != QThread::currentThread()) {
                    if(ValueOwnerUserData* vud = QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, obj)){
                        if(p->wasDeleted){
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("call destructor_function on object %p", pointer)
                            QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
                            PtrDeleterFunction _deleter_function = m_deleter_function;
                            void* _pointer = pointer;
                            bool isShell = this->isShell();
                            vud->addDeleter([_deleter_function,_pointer,isShell](){
                                _deleter_function(_pointer, isShell);
                            });
                            dispose();
                        }else{
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor for object %p", pointer)
                            setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                            QThreadUserData::Result threadDataResult = QThreadUserData::ensureThreadUserDataLocked(ownerThread);
                            if (QAbstractEventDispatcher::instance(ownerThread)) {
                                DestructorEvent* event = new DestructorEvent(vud->pointer(), m_this, pointer, m_deleter_function, isShell());
                                if(threadDataResult.initRequired){
                                    locker.unlock();
                                    threadDataResult.threadUserData->initialize(ownerThread);
                                    locker.relock();
                                }
                                QCoreApplication::postEvent(threadDataResult.threadUserData->threadDeleter(), event);
                            }else{
                                PtrDeleterFunction _deleter_function = m_deleter_function;
                                void* _pointer = pointer;
                                bool isShell = this->isShell();
                                threadDataResult.threadUserData->doAtThreadEnd([_deleter_function, _pointer, isShell](){
                                    _deleter_function(_pointer, isShell);
                                });
                                if(threadDataResult.initRequired){
                                    locker.unlock();
                                    threadDataResult.threadUserData->initialize(ownerThread);
                                    locker.relock();
                                }
                            }
                        }
                        pointer = nullptr;
                    }else if(!p->wasDeleted){
                        QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor for object %p", pointer)
                        setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                        QThreadUserData::Result threadDataResult = QThreadUserData::ensureThreadUserDataLocked(ownerThread);
                        if (QAbstractEventDispatcher::instance(ownerThread)) {
                            DestructorEvent* event = new DestructorEvent(obj, m_this, pointer, m_deleter_function, isShell());
                            if(threadDataResult.initRequired){
                                locker.unlock();
                                threadDataResult.threadUserData->initialize(ownerThread);
                                locker.relock();
                            }
                            QCoreApplication::postEvent(threadDataResult.threadUserData->threadDeleter(), event);
                        }else{
                            PtrDeleterFunction _deleter_function = m_deleter_function;
                            void* _pointer = pointer;
                            bool isShell = this->isShell();
                            threadDataResult.threadUserData->doAtThreadEnd([_deleter_function, _pointer, isShell](){
                                _deleter_function(_pointer, isShell);
                            });
                            if(threadDataResult.initRequired){
                                locker.unlock();
                                threadDataResult.threadUserData->initialize(ownerThread);
                                locker.relock();
                            }
                        }
                        pointer = nullptr;
                    }
                }
            }
            if(pointer){
                QTJAMBI_DEBUG_PRINT_WITH_ARGS("call destructor_function on object %p", pointer)
                QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
                locker.unlock();
                m_deleter_function(pointer, isShell());
                locker.relock();
                dispose();
            }
        }else{
            dispose();
        }
    }
}

template<template<typename> class SmartPointer>
void SmartPointerToContainerLink<SmartPointer>::init(JNIEnv* env){
    SmartPointerToObjectLink<SmartPointer>::init(env);
    if(AbstractReferenceCountingContainer* rc = dynamic_cast<AbstractReferenceCountingContainer*>(m_containerAccess)){
        jobject object = getJavaObjectLocalRef(env);
        rc->updateRC(env, {object, pointer()});
        env->DeleteLocalRef(object);
    }
}

void PointerToContainerLink::init(JNIEnv* env){
    PointerToObjectLink::init(env);
    if(AbstractReferenceCountingContainer* rc = dynamic_cast<AbstractReferenceCountingContainer*>(m_containerAccess)){
        jobject object = getJavaObjectLocalRef(env);
        rc->updateRC(env, {object, m_pointer});
        env->DeleteLocalRef(object);
    }
}

void DeletablePointerToObjectLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if (m_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "DeletablePointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
        if(!isShell())
            clearAllDependencies(env);
        unregisterPointer(m_pointer);
        unregisterOffsets();
        m_flags.setFlag(Flag::IsPointerRegistered, false);
        if(!m_flags.testFlag(Flag::NoNativeDeletion) && (m_flags.testFlag(Flag::JavaOwnership) || forced)){
            void* pointer = m_pointer;
            m_pointer = nullptr;
            QTJAMBI_DEBUG_PRINT_WITH_ARGS("call destructor_function on object %p", pointer)
            QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
            m_deleter_function(pointer, isShell());
        }
        dispose();
    }
}

// ### END ######################  PointerToObjectLink  ######################## END ####

// ### BEGIN ###################  PointerToQObjectLink  ###################### BEGIN ####

PointerToQObjectWithExtraSignalsLink::PointerToQObjectWithExtraSignalsLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMetaObject* metaObject, QObject *object, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException)
    : PointerToQObjectLink(env, nativeLink, jobj, metaObject, object, created_by_java, isDeclarativeCall, is_shell, ocurredException)
{
}

PointerToQObjectLink::PointerToQObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMetaObject* metaObject, QObject *object, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException) :
    QtJambiLink(env, nativeLink,
                LINK_NAME_ARG(metaObject->className())
                created_by_java, isDeclarativeCall, is_shell, ocurredException),
    m_pointer(object)
{
    QtJambiMetaObject::resolveSignals(env, jobj, metaObject, ocurredException);
    // Fetch the user data id
    if(ocurredException){
        m_flags.setFlag(Flag::IsJavaLinkDetached);
    }
}

void PointerToQObjectLink::init(JNIEnv* env){
    if(!isInitialized()){
        QtJambiLink::init(env);
        QObject* object = qobject();
        Q_ASSERT(object);
        const QMetaObject* metaObject = object->metaObject();
        {
            bool _isValueOwner = isValueOwner(metaObject);
            QWriteLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            if(metaObject->inherits(&QThread::staticMetaObject) && object!=QCoreApplicationPrivate::theMainThread.loadRelaxed())
                QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, object, new QtJambiThreadLinkUserData(getWeakPointer(), metaObject));
            else
                QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, object, new QtJambiMetaObjectLinkUserData(getWeakPointer(), metaObject));
            if(_isValueOwner && !QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, object)){
                QTJAMBI_SET_OBJECTUSERDATA(ValueOwnerUserData, object, new ValueOwnerUserData(object));
            }
        }
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
        m_qtTypeName = metaObject->className();
#endif
        if(createdByQml()){
            setCppOwnership(env);
        }else if(createdByJava()){
            if(object->parent())
                setCppOwnership(env);
            else
                setJavaOwnership(env);
        }else{
            if(object->parent())
                setCppOwnership(env);
            else
                setSplitOwnership(env);
        }
    }
}

PointerToQObjectLink::~PointerToQObjectLink()
{
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "PointerToQObjectLink::~PointerToQObjectLink()")
    try{
        if(JniEnvironment env = noThreadInitializationOnPurge() ? JniEnvironment{300} : DefaultJniEnvironment{300}){
            releaseJavaObject(env);
            deleteNativeObject(env);
            detachJavaLink(env);
        }else{
            releaseJavaObject(nullptr);
            deleteNativeObject(nullptr);
            detachJavaLink(nullptr);
        }
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
    } catch (...) {
    }
}

bool PointerToQObjectLink::isSmartPointer() const { return false; }

void PointerToQObjectLink::removeInterface(const std::type_info&){
    clearAllDependencies();
    setInDestructor();
}

void PointerToObjectLink::removeInterface(const std::type_info&){
    clearAllDependencies();
    setInDestructor();
}

void *PointerToQObjectLink::pointer() const {
    return m_pointer;
}

QObject *PointerToQObjectLink::qobject() const {
    return
#ifndef AVAILABLE_IN_DELETION
            isInDestructor() ||
#endif
            !isInitialized() ? nullptr : m_pointer;
}

void PointerToQObjectLink::setAsQObjectDeleted() {
    QWriteLocker locker(QtJambiLinkUserData::lock());
    Q_UNUSED(locker)
    QTJAMBI_DEBUG_PRINT_WITH_ARGS("PointerToQObjectLink::setAsQObjectDeleted() object %p of link %p", m_pointer, this)
    m_pointer = nullptr;
}

void PointerToQObjectLink::invalidate(JNIEnv *env) {
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "PointerToQObjectLink::invalidate(JNIEnv *)")
    invalidateDependentObjects(env);
    releaseJavaObject(env);
    QWriteLocker locker(QtJambiLinkUserData::lock());
    Q_UNUSED(locker)
    if(m_pointer) {
        QTJAMBI_DEBUG_PRINT_WITH_ARGS("invalidate object %p of link %p", m_pointer, this)
        unregisterOffsets();
        m_pointer = nullptr;
    }
    dispose();
}

void PointerToQObjectLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if(m_pointer && !isDeleteLater()){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "PointerToQObjectLink::deleteNativeObject(JNIEnv *, bool)")
        unregisterOffsets();
        if(!isShell())
            clearAllDependencies(env);

        if(isInitialized()){
            if (!m_flags.testFlag(Flag::NoNativeDeletion) && (m_flags.testFlag(Flag::JavaOwnership) || forced)) {
                QThread *currentThread = QThread::currentThread();
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
                        {
                            QObjectInDeletion d(env, this);
                            delete m_pointer;
                        }
                        __qt_exceptionBlocker.release(env);
                    }
                }else if (currentThread == objectThread || !objectThread->isRunning() || objectThread==m_pointer) {
                    QTJAMBI_DEBUG_PRINT_WITH_ARGS("call delete on object %p of type %s", m_pointer, QtJambiAPI::typeName(typeid(*m_pointer)).constData())
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
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("call deleteLater() on object %p of type %s", m_pointer, QtJambiAPI::typeName(typeid(*m_pointer)).constData())
                            return;
                        }
                    }
                    if(currentThread == objectThread){
                        QtJambiExceptionBlocker __qt_exceptionBlocker;
                        {
                            QObjectInDeletion d(env, this);
                            delete m_pointer;
                        }
                        __qt_exceptionBlocker.release(env);
                    }else if(!objectThread->isRunning() && !m_pointer->parent() && !m_pointer->isWidgetType()){
                        QtJambiExceptionBlocker __qt_exceptionBlocker;
                        {
                            QObjectInDeletion d(env, this);
                            delete m_pointer;
                        }
                        __qt_exceptionBlocker.release(env);
                    }else if(objectThread==m_pointer){
                        if(objectThread->isRunning()){
                            qCWarning(DebugAPI::internalCategory, "Trying to delete a running QThread '%s'",
                                    qPrintable(m_pointer->objectName()));
                        }
                        QtJambiExceptionBlocker __qt_exceptionBlocker;
                        {
                            QObjectInDeletion d(env, this);
                            delete m_pointer;
                        }
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
                    QTJAMBI_DEBUG_PRINT_WITH_ARGS("call deleteLater() on object %p of type %s", m_pointer, QtJambiAPI::typeName(typeid(*m_pointer)).constData())
                    QObjectInDeletion::disposeQObject(m_pointer);
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
                    QThreadUserData* qthreadData = QThreadUserData::ensureThreadUserData(objectThread);
                    setDeleteLater();
                    QObjectInDeletion::disposeQObject(m_pointer);
                    QTJAMBI_DEBUG_PRINT_WITH_ARGS("attach QObject to thread shutdown procedure.")
                    qthreadData->deleteAtThreadEnd(m_pointer);
                }
            }else{
                // trust me it is possible that pointer is null
                QObjectInDeletion::disposeQObject(m_pointer);
                m_pointer = nullptr;
                dispose();
            }
        }
    }
}
// Entry point for JVM finalization QtJambiObject#finalize()
void PointerToQObjectLink::onClean(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::HasBeenCleaned)){
        if(!m_flags.testFlag(Flag::BlockDeletion)){
            m_flags.setFlag(Flag::BlockDeletion, true);
            QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "QtJambiLink::onClean(JNIEnv *)")
            QTJAMBI_INCREASE_COUNTER_THIS(cleanCallerCount)
            releaseJavaObject(env);
            deleteNativeObject(env);
            detachJavaLink(env);
            m_flags.setFlag(Flag::HasBeenCleaned);
            m_flags.setFlag(Flag::BlockDeletion, false);
        }
    }
}

// This allows the Java language to attempt to expedite the disposal of the C++ object
//  (when that this possible), however its possible the C++ object will not be destroyed
//  as the Java side does not own it.
// Entry point for QtJambiObject#dispose() method
void PointerToQObjectLink::onDispose(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::IsManaged)){
        if(!m_flags.testFlag(Flag::BlockDeletion)){
            m_flags.setFlag(Flag::BlockDeletion, true);
            QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "QtJambiLink::onDispose(JNIEnv *)")
            setJavaOwnership(env);
            deleteNativeObject(env, true);
            detachJavaLink(env);
            QTJAMBI_INCREASE_COUNTER_THIS(disposeCalledCount)
            m_flags.setFlag(Flag::BlockDeletion, false);
        }
    }
}

// ### END #####################  PointerToQObjectLink  ######################## END ####

template<template<typename> class SmartPointer>
ExtendedSmartPointerToObjectLink<SmartPointer>::ExtendedSmartPointerToObjectLink(JNIEnv *env, jobject nativeLink,
                                                                                 LINK_NAME_ARG(const char* qt_name)
                                                                                 bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, const QObject* extension,
                                                                                 PtrOwnerFunction ownerFunction,
                                                                                 void* pointer,
                                                                                 SmartPointer<char>*& smartPointer,
                                                                                 JavaException& ocurredException)
    : SmartPointerToObjectLink<SmartPointer>(env, nativeLink,
                                             LINK_NAME_ARG(qt_name)
                                             created_by_java, ownership, is_shell,
                                             ownerFunction,
                                             pointer,
                                             smartPointer,
                                             ocurredException), ExtendedLink(), m_extension(extension){}

// ### BEGIN #################  SmartPointerToObjectLink  ################### BEGIN ###

template<template<typename> class SmartPointer>
SmartPointerToContainerLink<SmartPointer>::SmartPointerToContainerLink(JNIEnv *env, jobject nativeLink,
                                                                       LINK_NAME_ARG(const char* qt_name)
                                                                       bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                       PtrOwnerFunction ownerFunction,
                                                                       void* pointer,
                                                                       SmartPointer<char>*& smartPointer,
                                                                       AbstractContainerAccess* containerAccess,
                                                                       JavaException& ocurredException) :
    SmartPointerToObjectLink<SmartPointer>(env, nativeLink,
                                           LINK_NAME_ARG(qt_name)
                                           created_by_java, ownership, is_shell,
                                           ownerFunction,
                                           pointer,
                                           smartPointer,
                                           ocurredException),
    m_containerAccess(containerAccess)
{
}

template<template<typename> class SmartPointer>
SmartPointerToContainerLink<SmartPointer>::~SmartPointerToContainerLink(){
    m_containerAccess->dispose();
}

template<template<typename> class SmartPointer>
SmartPointerToObjectLink<SmartPointer>::SmartPointerToObjectLink(JNIEnv *env, jobject nativeLink,
                                                                 LINK_NAME_ARG(const char *qt_name)
                                                                 bool created_by_java, QtJambiLink::Ownership ownership,
                                                                 bool is_shell,
                                                                 PtrOwnerFunction ownerFunction,
                                                                 void* pointer,
                                                                 SmartPointer<char>*& smartPointer,
                                                                 JavaException& ocurredException) :
    SmartPointerLink<SmartPointer,char>(env, nativeLink,
                       LINK_NAME_ARG(qt_name)
                       created_by_java, ownership, is_shell, smartPointer, ocurredException),
    m_pointer(pointer),
    m_owner_function(ownerFunction)
{
    if(!ocurredException){
        this->registerPointer(pointer);
    }else{
        this->m_flags.setFlag(QtJambiLink::Flag::IsJavaLinkDetached);
        m_pointer = nullptr;
    }
}

template<template<typename> class SmartPointer>
SmartPointerToObjectLink<SmartPointer>::~SmartPointerToObjectLink(){
    try{
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this->m_this, "SmartPointerToObjectLink::~SmartPointerToObjectLink()")
        if(JniEnvironment env = noThreadInitializationOnPurge() ? JniEnvironment{300} : DefaultJniEnvironment{300}){
            releaseJavaObject(env);
            deleteNativeObject(env);
            detachJavaLink(env);
        }else{
            releaseJavaObject(nullptr);
            deleteNativeObject(nullptr);
            detachJavaLink(nullptr);
        }
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
    } catch (...) {
    }
}

template<template<typename> class SmartPointer>
typename SmartPointerLink<SmartPointer,char>::SharedPointerT SmartPointerToObjectLink<SmartPointer>::getSmartPointer() const {
    if(m_pointer)
        return SmartPointerLink<SmartPointer,char>::getSmartPointer();
    else return{};
}

template<>
QSharedPointer<char> SmartPointerToObjectLink<QWeakPointer>::getSmartPointer() const {
    if(m_pointer){
        QSharedPointer<char> result = SmartPointerLink<QWeakPointer,char>::getSmartPointer();
        if(!result){
            if(containerAccess()){
                result = QSharedPointer<char>(reinterpret_cast<char*>(m_pointer), SmartContainerDeleter<QWeakPointer>(containerAccess(), getStrongPointer().template staticCast<SmartPointerLink<QWeakPointer,char>>()));
            }else if(deleterFunction()){
                result = QSharedPointer<char>(reinterpret_cast<char*>(m_pointer), SmartDeleterFunctionDeleter<QWeakPointer>(*deleterFunction(), getStrongPointer().template staticCast<SmartPointerLink<QWeakPointer,char>>()));
            }else if(metaType()){
                result = QSharedPointer<char>(reinterpret_cast<char*>(m_pointer), SmartMetaTypeDeleter<QWeakPointer>(*metaType(), getStrongPointer().template staticCast<SmartPointerLink<QWeakPointer,char>>()));
            }
        }
        return result;
    }else return{};
}

template<>
std::shared_ptr<char> SmartPointerToObjectLink<std::weak_ptr>::getSmartPointer() const {
    if(m_pointer){
        std::shared_ptr<char> result = SmartPointerLink<std::weak_ptr,char>::getSmartPointer();
        if(!result){
            if(containerAccess()){
                result = std::shared_ptr<char>(reinterpret_cast<char*>(m_pointer), SmartContainerDeleter<std::weak_ptr>(containerAccess(), getStrongPointer().template staticCast<SmartPointerLink<std::weak_ptr,char>>()));
            }else if(deleterFunction()){
                result = std::shared_ptr<char>(reinterpret_cast<char*>(m_pointer), SmartDeleterFunctionDeleter<std::weak_ptr>(*deleterFunction(), getStrongPointer().template staticCast<SmartPointerLink<std::weak_ptr,char>>()));
            }else if(metaType()){
                result = std::shared_ptr<char>(reinterpret_cast<char*>(m_pointer), SmartMetaTypeDeleter<std::weak_ptr>(*metaType(), getStrongPointer().template staticCast<SmartPointerLink<std::weak_ptr,char>>()));
            }
        }
        return result;
    }else return{};
}

template<template<typename> class SmartPointer>
void SmartPointerToObjectLink<SmartPointer>::init(JNIEnv* env){
    if(!isInitialized()){
        QtJambiLink::init(env);
        if(m_owner_function){
            if(const QObject* obj = m_owner_function(pointer())){
                const QObjectPrivate* p = QObjectPrivate::get(obj);
                if(p && !p->wasDeleted){
                    ValueOwnerUserData* vod;
                    {
                        QReadLocker locker(QtJambiLinkUserData::lock());
                        Q_UNUSED(locker)
                        vod = QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, obj);
                    }
                    if(!vod){
                        QWriteLocker locker(QtJambiLinkUserData::lock());
                        Q_UNUSED(locker)
                        vod = QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, obj);
                        if(!vod)
                            QTJAMBI_SET_OBJECTUSERDATA(ValueOwnerUserData, const_cast<QObject*>(obj), new ValueOwnerUserData(obj));
                    }
                }
            }
        }
    }
}

template<template<typename> class SmartPointer>
void SmartPointerToObjectLink<SmartPointer>::invalidate(JNIEnv *env) {
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this->m_this, "SmartPointerToObjectLink::invalidate(JNIEnv *)")
    invalidateDependentObjects(env);
    releaseJavaObject(env);
    detachJavaLink(env);
    if(m_pointer) {
        QTJAMBI_DEBUG_PRINT_WITH_ARGS("invalidate object %p of link %p", m_pointer, this)
        unregisterPointer(m_pointer);
        unregisterOffsets();
        this->m_flags.setFlag(QtJambiLink::Flag::IsPointerRegistered, false);
        m_pointer = nullptr;
    }
    dispose();
}

template<template<typename> class SmartPointer>
void SmartPointerToObjectLink<SmartPointer>::deleteNativeObject(JNIEnv *env, bool){
    typedef typename std::conditional<std::is_same<SmartPointer<char>,QWeakPointer<char>>::value, QSharedPointer<char>,
                                      typename std::conditional<std::is_same<SmartPointer<char>,std::weak_ptr<char>>::value, std::shared_ptr<char>, SmartPointer<char>>::type>::type SharedPointerT;
    if(m_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this->m_this, "SmartPointerToObjectLink::deleteNativeObject(JNIEnv *env, bool)")
        unregisterOffsets();
        unregisterPointer(m_pointer);
        this->m_flags.setFlag(QtJambiLink::Flag::IsPointerRegistered, false);
        if(!isShell())
            clearAllDependencies(env);

        SharedPointerT shared_pointer;
        if constexpr(std::is_same<SmartPointer<QObject>,QWeakPointer<QObject>>::value
                || std::is_same<SmartPointer<QObject>,std::weak_ptr<QObject>>::value){
            shared_pointer = m_smartPointer.lock();
        }else{
            shared_pointer = m_smartPointer;
        }
        m_smartPointer = SmartPointer<char>();
        void* pointer = m_pointer;
        m_pointer = nullptr;
        PtrDeleterFunction deleter_function = deleterFunction();
        const QMetaType* metaTypePtr = metaType();
        AbstractContainerAccess* _containerAccess = containerAccess();
        if(m_owner_function){
            QWriteLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            if(const QObject* obj = m_owner_function(pointer)){
                const QObjectPrivate* p = QObjectPrivate::get(obj);
                Q_ASSERT(p);
                QThread* ownerThread =
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
                            p->threadData->thread;
#else
                            p->threadData.loadAcquire()->thread.loadAcquire();
#endif
                if (ownerThread->isRunning() && ownerThread != QThread::currentThread()) {
                    if(ValueOwnerUserData* vud = QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, obj)){
                        if(p->wasDeleted){
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("call shared pointer deleter function")
                            QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
                            if(shared_pointer){
                                vud->addDeleter([_shared_pointer = SharedPointerT(shared_pointer)]()mutable{
                                    _shared_pointer = SharedPointerT();
                                });
                            }else{
                                void* _pointer = pointer;
                                if(PtrDeleterFunction fn = deleterFunction()){
                                    bool isShell = this->isShell();
                                    vud->addDeleter([fn,_pointer, isShell](){
                                        fn(_pointer, isShell);
                                    });
                                }else if(const QMetaType* mt = metaType()){
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                                    QMetaType meta_type = *mt;
                                    vud->addDeleter([meta_type,_pointer](){
                                        meta_type.destroy(_pointer);
                                    });
#else
                                    int meta_type = mt->id();
                                    vud->addDeleter([meta_type,_pointer](){
                                        QMetaType::destroy(meta_type, _pointer);
                                    });
#endif
                                }
                                pointer = nullptr;
                            }
                            dispose();
                        }else{
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor (calling shared pointer deleter function)")
                            setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                            QThreadUserData::Result threadDataResult = QThreadUserData::ensureThreadUserDataLocked(ownerThread);
                            if(shared_pointer){
                                if (QAbstractEventDispatcher::instance(ownerThread)) {
                                    DestructorEvent* event = new DestructorEvent(vud->pointer(), this->m_this, new SharedPointerT(shared_pointer), [](void * ptr,bool){delete reinterpret_cast<SharedPointerT*>(ptr);}, isShell());
                                    if(threadDataResult.initRequired){
                                        locker.unlock();
                                        threadDataResult.threadUserData->initialize(ownerThread);
                                        locker.relock();
                                    }
                                    QCoreApplication::postEvent(threadDataResult.threadUserData->threadDeleter(), event);
                                }else{
                                    threadDataResult.threadUserData->doAtThreadEnd([_shared_pointer = SharedPointerT(shared_pointer)]() mutable {
                                        _shared_pointer = SharedPointerT();
                                    });
                                    if(threadDataResult.initRequired){
                                        locker.unlock();
                                        threadDataResult.threadUserData->initialize(ownerThread);
                                        locker.relock();
                                    }
                                }
                                shared_pointer = SharedPointerT();
                            }else{
                                if(_containerAccess){
                                    if (QAbstractEventDispatcher::instance(ownerThread)) {
                                        ContainerDestructionEvent* event = new ContainerDestructionEvent(obj, this->m_this, pointer, _containerAccess->clone());
                                        if(threadDataResult.initRequired){
                                            locker.unlock();
                                            threadDataResult.threadUserData->initialize(ownerThread);
                                            locker.relock();
                                        }
                                        QCoreApplication::postEvent(threadDataResult.threadUserData->threadDeleter(), event);
                                    }else{
                                        void* _pointer = pointer;
                                        threadDataResult.threadUserData->doAtThreadEnd([_containerAccess = QSharedPointer<AbstractContainerAccess>(_containerAccess->clone(), &containerDisposer), _pointer](){
                                            _containerAccess->deleteContainer(_pointer);
                                        });
                                        if(threadDataResult.initRequired){
                                            locker.unlock();
                                            threadDataResult.threadUserData->initialize(ownerThread);
                                            locker.relock();
                                        }
                                    }
                                }else if(deleter_function){
                                    if (QAbstractEventDispatcher::instance(ownerThread)) {
                                        DestructorEvent* event = new DestructorEvent(vud->pointer(), this->m_this, pointer, deleter_function, isShell());
                                        if(threadDataResult.initRequired){
                                            locker.unlock();
                                            threadDataResult.threadUserData->initialize(ownerThread);
                                            locker.relock();
                                        }
                                        QCoreApplication::postEvent(threadDataResult.threadUserData->threadDeleter(), event);
                                    }else{
                                        void* _pointer = pointer;
                                        bool isShell = this->isShell();
                                        threadDataResult.threadUserData->doAtThreadEnd([deleter_function, _pointer, isShell](){
                                            deleter_function(_pointer, isShell);
                                        });
                                        if(threadDataResult.initRequired){
                                            locker.unlock();
                                            threadDataResult.threadUserData->initialize(ownerThread);
                                            locker.relock();
                                        }
                                    }
                                }else if(metaTypePtr){
                                    if (QAbstractEventDispatcher::instance(ownerThread)) {
                                        MetaTypeDestructionEvent* event = new MetaTypeDestructionEvent(vud->pointer(), this->m_this, pointer, *metaTypePtr);
                                        if(threadDataResult.initRequired){
                                            locker.unlock();
                                            threadDataResult.threadUserData->initialize(ownerThread);
                                            locker.relock();
                                        }
                                        QCoreApplication::postEvent(threadDataResult.threadUserData->threadDeleter(), event);
                                    }else{
                                        void* _pointer = pointer;
        #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                                        QMetaType meta_type = *metaTypePtr;
                                        threadDataResult.threadUserData->doAtThreadEnd([meta_type,_pointer](){
                                            meta_type.destroy(_pointer);
                                        });
        #else
                                        int meta_type = metaTypePtr->id();
                                        threadDataResult.threadUserData->doAtThreadEnd([meta_type,_pointer](){
                                            QMetaType::destroy(meta_type, _pointer);
                                        });
        #endif
                                        if(threadDataResult.initRequired){
                                            locker.unlock();
                                            threadDataResult.threadUserData->initialize(ownerThread);
                                            locker.relock();
                                        }
                                    }
                                }
                                pointer = nullptr;
                            }
                        }
                    }else if(!p->wasDeleted){
                        QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor (calling shared pointer deleter function)")
                        setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                        QThreadUserData::Result threadDataResult = QThreadUserData::ensureThreadUserDataLocked(ownerThread);
                        if(shared_pointer){
                            if (QAbstractEventDispatcher::instance(ownerThread)) {
                                DestructorEvent* event = new DestructorEvent(obj, this->m_this, new SharedPointerT(shared_pointer), [](void * ptr,bool){delete reinterpret_cast<SharedPointerT*>(ptr);}, isShell());
                                if(threadDataResult.initRequired){
                                    locker.unlock();
                                    threadDataResult.threadUserData->initialize(ownerThread);
                                    locker.relock();
                                }
                                QCoreApplication::postEvent(threadDataResult.threadUserData->threadDeleter(), event);
                            }else{
                                threadDataResult.threadUserData->doAtThreadEnd([_shared_pointer = SharedPointerT(shared_pointer)]() mutable {
                                    _shared_pointer = SharedPointerT();
                                });
                                if(threadDataResult.initRequired){
                                    locker.unlock();
                                    threadDataResult.threadUserData->initialize(ownerThread);
                                    locker.relock();
                                }
                            }
                            shared_pointer = SharedPointerT();
                        }else{
                            if(_containerAccess){
                                if (QAbstractEventDispatcher::instance(ownerThread)) {
                                    ContainerDestructionEvent* event = new ContainerDestructionEvent(obj, this->m_this, pointer, _containerAccess->clone());
                                    if(threadDataResult.initRequired){
                                        locker.unlock();
                                        threadDataResult.threadUserData->initialize(ownerThread);
                                        locker.relock();
                                    }
                                    QCoreApplication::postEvent(threadDataResult.threadUserData->threadDeleter(), event);
                                }else{
                                    void* _pointer = pointer;
                                    threadDataResult.threadUserData->doAtThreadEnd([_containerAccess = QSharedPointer<AbstractContainerAccess>(_containerAccess->clone(), &containerDisposer), _pointer](){
                                        _containerAccess->deleteContainer(_pointer);
                                    });
                                    if(threadDataResult.initRequired){
                                        locker.unlock();
                                        threadDataResult.threadUserData->initialize(ownerThread);
                                        locker.relock();
                                    }
                                }
                            }else if(deleter_function){
                                if (QAbstractEventDispatcher::instance(ownerThread)) {
                                    DestructorEvent* event = new DestructorEvent(obj, this->m_this, pointer, deleter_function, isShell());
                                    if(threadDataResult.initRequired){
                                        locker.unlock();
                                        threadDataResult.threadUserData->initialize(ownerThread);
                                        locker.relock();
                                    }
                                    QCoreApplication::postEvent(threadDataResult.threadUserData->threadDeleter(), event);
                                }else{
                                    void* _pointer = pointer;
                                    bool isShell = this->isShell();
                                    threadDataResult.threadUserData->doAtThreadEnd([deleter_function, _pointer, isShell](){
                                        deleter_function(_pointer, isShell);
                                    });
                                    if(threadDataResult.initRequired){
                                        locker.unlock();
                                        threadDataResult.threadUserData->initialize(ownerThread);
                                        locker.relock();
                                    }
                                }
                            }else if(metaTypePtr){
                                if (QAbstractEventDispatcher::instance(ownerThread)) {
                                    MetaTypeDestructionEvent* event = new MetaTypeDestructionEvent(obj, this->m_this, pointer, *metaTypePtr);
                                    if(threadDataResult.initRequired){
                                        locker.unlock();
                                        threadDataResult.threadUserData->initialize(ownerThread);
                                        locker.relock();
                                    }
                                    QCoreApplication::postEvent(threadDataResult.threadUserData->threadDeleter(), event);
                                }else{
                                    void* _pointer = pointer;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                                    QMetaType meta_type = *metaTypePtr;
                                    threadDataResult.threadUserData->doAtThreadEnd([meta_type,_pointer](){
                                        meta_type.destroy(_pointer);
                                    });
#else
                                    int meta_type = metaTypePtr->id();
                                    threadDataResult.threadUserData->doAtThreadEnd([meta_type,_pointer](){
                                        QMetaType::destroy(meta_type, _pointer);
                                    });
#endif
                                    if(threadDataResult.initRequired){
                                        locker.unlock();
                                        threadDataResult.threadUserData->initialize(ownerThread);
                                        locker.relock();
                                    }
                                }
                            }
                            pointer = nullptr;
                        }
                    }
                }
            }
        }

        if(shared_pointer){
            QTJAMBI_DEBUG_PRINT_WITH_ARGS("call shared pointer deleter function")
            QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
            shared_pointer = SharedPointerT();
            dispose();
        }else if(pointer){
            QTJAMBI_DEBUG_PRINT_WITH_ARGS("call shared pointer deleter function")
            QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
            if(_containerAccess){
                _containerAccess->deleteContainer(pointer);
            }else if(deleter_function){
                deleter_function(pointer, isShell());
            }else if(metaTypePtr){
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                metaTypePtr->destroy(pointer);
#else
                QMetaType::destroy(metaTypePtr->id(), pointer);
#endif
            }
            dispose();
        }
    }
}

template<template<typename> class SmartPointer, typename T>
SmartPointerLink<SmartPointer,T>::SmartPointerLink(JNIEnv *env, jobject nativeLink,
                   LINK_NAME_ARG(const char *qt_name)
                   bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell, SmartPointer<T>*& smartPointer, JavaException& ocurredException)
    : QtJambiLink(env, nativeLink,
                  LINK_NAME_ARG(qt_name)
                  created_by_java, false, is_shell, ocurredException), m_smartPointer(), m_hiddenOwnership(ownership) {
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
void SmartPointerLink<SmartPointer,T>::setDefaultOwnership(JNIEnv *) {
    if(m_hiddenOwnership!=QtJambiLink::Ownership::None){
        if (createdByJava())
            m_hiddenOwnership = QtJambiLink::Ownership::Java;
        else if(!isQObject() || (!qobjectDeleted() && qobject()->parent()==nullptr))
            m_hiddenOwnership = QtJambiLink::Ownership::Split;
        else
            m_hiddenOwnership = QtJambiLink::Ownership::Cpp;
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

template<template<typename> class SmartPointer>
void SmartPointerToObjectLink<SmartPointer>::onClean(JNIEnv *env)
{
    if(!this->m_flags.testFlag(QtJambiLink::Flag::HasBeenCleaned)){
        if(!this->m_flags.testFlag(QtJambiLink::Flag::BlockDeletion)){
            JavaException throwable;
            this->m_flags.setFlag(QtJambiLink::Flag::BlockDeletion, true);
            QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this->m_this, "QtJambiLink::onClean(JNIEnv *)")
            QTJAMBI_INCREASE_COUNTER_THIS(cleanCallerCount)
            releaseJavaObject(env);
            deleteNativeObject(env);
            if(env->ExceptionCheck()){
                jthrowable t = env->ExceptionOccurred();
                env->ExceptionClear();
                throwable = JavaException(env, t);
            }
            detachJavaLink(env);
            this->m_flags.setFlag(QtJambiLink::Flag::HasBeenCleaned);
            this->m_flags.setFlag(QtJambiLink::Flag::BlockDeletion, false);
            throwable.raise();
        }
    }
}

// This allows the Java language to attempt to expedite the disposal of the C++ object
//  (when that this possible), however its possible the C++ object will not be destroyed
//  as the Java side does not own it.
// Entry point for QtJambiObject#dispose() method
template<template<typename> class SmartPointer>
void SmartPointerToObjectLink<SmartPointer>::onDispose(JNIEnv *env)
{
    if(!this->m_flags.testFlag(QtJambiLink::Flag::BlockDeletion)){
        JavaException throwable;
        this->m_flags.setFlag(QtJambiLink::Flag::BlockDeletion, true);
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this->m_this, "QtJambiLink::onDispose(JNIEnv *)")
        setJavaOwnership(env);
        deleteNativeObject(env, true);
        if(env->ExceptionCheck()){
            jthrowable t = env->ExceptionOccurred();
            env->ExceptionClear();
            throwable = JavaException(env, t);
        }
        detachJavaLink(env);
        QTJAMBI_INCREASE_COUNTER_THIS(disposeCalledCount)
        this->m_flags.setFlag(QtJambiLink::Flag::BlockDeletion, false);
        throwable.raise();
    }
}

// #### END ##################  SmartPointerToObjectLink  ##################### END ####

// ### BEGIN #################  SmartPointerToQObjectLink  ################### BEGIN ###

template<template<typename> class SmartPointer>
SmartPointerToQObjectLink<SmartPointer>::SmartPointerToQObjectLink(JNIEnv *env, jobject nativeLink,
                                                                   LINK_NAME_ARG(const char *qt_name)
                                                                   bool created_by_java, QtJambiLink::Ownership ownership,
                                                                   bool is_shell,
                                                                   QObject* pointer,
                                                                   SmartPointer<QObject>*& smartPointer,
                                                                   JavaException& ocurredException)
    : SmartPointerLink<SmartPointer,QObject>(env, nativeLink,
                                             LINK_NAME_ARG(qt_name)
                                             created_by_java, ownership, is_shell, smartPointer, ocurredException),
      m_pointer(pointer)
{
}

template<template<typename> class SmartPointer>
SmartPointerToQObjectLink<SmartPointer>::~SmartPointerToQObjectLink(){
    try{
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this->m_this, "SmartPointerToQObjectLink::~SmartPointerToQObjectLink()")
        if(JniEnvironment env = QtJambiLink::noThreadInitializationOnPurge() ? JniEnvironment{300} : DefaultJniEnvironment{300}){
            QtJambiLink::releaseJavaObject(env);
            deleteNativeObject(env);
            QtJambiLink::detachJavaLink(env);
        }else{
            QtJambiLink::releaseJavaObject(nullptr);
            deleteNativeObject(nullptr);
            QtJambiLink::detachJavaLink(nullptr);
        }
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
    } catch (...) {
    }
}

template<template<typename> class SmartPointer>
typename SmartPointerLink<SmartPointer,QObject>::SharedPointerT SmartPointerToQObjectLink<SmartPointer>::getSmartPointer() const {
    if(m_pointer)
        return SmartPointerLink<SmartPointer,QObject>::getSmartPointer();
    else return{};
}

template<>
QSharedPointer<QObject> SmartPointerToQObjectLink<QWeakPointer>::getSmartPointer() const {
    if(m_pointer){
        QSharedPointer<QObject> result = SmartPointerLink<QWeakPointer,QObject>::getSmartPointer();
        if(!result)
            result = QSharedPointer<QObject>(m_pointer, LinkedQObjectDeleter<QWeakPointer>(getStrongPointer().template staticCast<SmartPointerLink<QWeakPointer,QObject>>()));
        return result;
    }else return{};
}

template<>
std::shared_ptr<QObject> SmartPointerToQObjectLink<std::weak_ptr>::getSmartPointer() const {
    if(m_pointer){
        std::shared_ptr<QObject> result = SmartPointerLink<std::weak_ptr,QObject>::getSmartPointer();
        if(!result)
            result = std::shared_ptr<QObject>(m_pointer, LinkedQObjectDeleter<std::weak_ptr>(getStrongPointer().template staticCast<SmartPointerLink<std::weak_ptr,QObject>>()));
        return result;
    }else return{};
}

template<template<typename> class SmartPointer>
void SmartPointerToQObjectLink<SmartPointer>::invalidate(JNIEnv *env) {
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this->m_this, "SmartPointerToQObjectLink::invalidate(JNIEnv *)")
    QtJambiLink::invalidateDependentObjects(env);
    QtJambiLink::releaseJavaObject(env);
    QtJambiLink::dispose();
}

template<template<typename> class SmartPointer>
void SmartPointerToQObjectLink<SmartPointer>::deleteNativeObject(JNIEnv *env, bool){
    typedef typename std::conditional<std::is_same<SmartPointer<QObject>,QWeakPointer<QObject>>::value, QSharedPointer<QObject>,
                                      typename std::conditional<std::is_same<SmartPointer<QObject>,std::weak_ptr<QObject>>::value, std::shared_ptr<QObject>, SmartPointer<QObject>>::type>::type SharedPointerT;
    QObject* obj = m_pointer;
    SharedPointerT shared_pointer;
    if constexpr(std::is_same<SmartPointer<QObject>,QWeakPointer<QObject>>::value
            || std::is_same<SmartPointer<QObject>,std::weak_ptr<QObject>>::value){
        shared_pointer = m_smartPointer.lock();
    }else{
        shared_pointer = m_smartPointer;
    }
    m_smartPointer = SmartPointer<QObject>();
    m_pointer = nullptr;
    if (obj && !isDeleteLater()){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this->m_this, "SmartPointerToQObjectLink::deleteNativeObject(JNIEnv *env, bool)")
        unregisterOffsets();
        if(!isShell())
            clearAllDependencies(env);

        QThread *objectThread = obj->thread();

        QThread *currentThread = QThread::currentThread();

        // Explicit dispose from current thread, delete object
        if (!objectThread || currentThread == objectThread || !objectThread->isRunning()) {
//             printf(" - straight up delete %s [%s]\n",
//                    qPrintable(qobj->objectName()),
//                    qobj->metaObject()->className());
            QTJAMBI_DEBUG_PRINT_WITH_ARGS("call delete PointerContainer")
            if(objectThread && obj==objectThread){
                if(QThreadData::get2(objectThread)->isAdopted){
                    QTJAMBI_DEBUG_PRINT_WITH_ARGS("Adopted thread expected to be automatically deleted")
                    QObjectInDeletion::disposeQObject(obj);
                    return;
                }
                if(static_cast<SelfDeletingThread*>(objectThread)->deleteLaterIfIsInFinish()){
                    setDeleteLater();
                    QObjectInDeletion::disposeQObject(obj);
                    QTJAMBI_DEBUG_PRINT_WITH_ARGS("call QObject::deleteLater()")
                    return;
                }
            }
            {
                QtJambiExceptionBlocker __qt_exceptionBlocker;
                {
                    QObjectInDeletion d(env, this);
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
                        QObjectInDeletion d(env, this);
                        if(shared_pointer)
                            shared_pointer = SharedPointerT();
                        else
                            delete obj;
                    }
                    __qt_exceptionBlocker.release(env);
                }
            }else{
                QThreadUserData* threadData = QThreadUserData::ensureThreadUserData(objectThread);
                QObjectInDeletion::disposeQObject(obj);
                setDeleteLater();
                QTJAMBI_DEBUG_PRINT_WITH_ARGS("attach QObject to thread shutdown procedure.")
                if(shared_pointer){
                    if (QAbstractEventDispatcher::instance(objectThread)) {
                        QCoreApplication::postEvent(threadData->threadDeleter(), new DestructorEvent(obj, this->m_this, new SharedPointerT(shared_pointer), [](void * ptr,bool){delete reinterpret_cast<SharedPointerT*>(ptr);}, false));
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

// Entry point for JVM finalization QtJambiObject#finalize()
template<template<typename> class SmartPointer>
void SmartPointerToQObjectLink<SmartPointer>::onClean(JNIEnv *env)
{
    if(!this->m_flags.testFlag(QtJambiLink::Flag::HasBeenCleaned)){
        if(!this->m_flags.testFlag(QtJambiLink::Flag::BlockDeletion)){
            this->m_flags.setFlag(QtJambiLink::Flag::BlockDeletion, true);
            QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this->m_this, "QtJambiLink::onClean(JNIEnv *)")
            QTJAMBI_INCREASE_COUNTER_THIS(cleanCallerCount)
            QtJambiLink::releaseJavaObject(env);
            deleteNativeObject(env);
            QtJambiLink::detachJavaLink(env);
            this->m_flags.setFlag(QtJambiLink::Flag::HasBeenCleaned);
            this->m_flags.setFlag(QtJambiLink::Flag::BlockDeletion, false);
        }
    }
}

// This allows the Java language to attempt to expedite the disposal of the C++ object
//  (when that this possible), however its possible the C++ object will not be destroyed
//  as the Java side does not own it.
// Entry point for QtJambiObject#dispose() method
template<template<typename> class SmartPointer>
void SmartPointerToQObjectLink<SmartPointer>::onDispose(JNIEnv *env)
{
    if(!this->m_flags.testFlag(QtJambiLink::Flag::BlockDeletion)){
        this->m_flags.setFlag(QtJambiLink::Flag::BlockDeletion, true);
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(this->m_this, "QtJambiLink::onDispose(JNIEnv *)")
        QtJambiLink::setJavaOwnership(env);
        deleteNativeObject(env, true);
        QtJambiLink::detachJavaLink(env);
        QTJAMBI_INCREASE_COUNTER_THIS(disposeCalledCount)
        this->m_flags.setFlag(QtJambiLink::Flag::BlockDeletion, false);
    }
}

bool PointerToQObjectLink::isQObject() const {
    return true;
}

bool PointerToObjectLink::isQObject() const {
    return false;
}

PointerToPendingQObjectLink::PointerToPendingQObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMetaObject* metaObject, QObject *ptr, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException)
    : PointerToQObjectLink(env, nativeLink, jobj, metaObject, ptr, created_by_java, isDeclarativeCall, is_shell, ocurredException)
{
    m_flags.setFlag(Flag::IsPendingValueOwner, isValueOwner(metaObject));
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
        m_qtTypeName = metaObject->className();
#endif
}

void PointerToPendingQObjectLink::init(JNIEnv* env){
    if(!isInitialized()){
        QtJambiLink::init(env);
        QObject* object = qobject();
        Q_ASSERT(object);
        {
            QWriteLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            QtJambiLinkUserData* lud = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, object);
            if(lud)
                QtJambiLink::resetDataLink(lud);
            else
                QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, object, new QtJambiLinkUserData(getWeakPointer()));
            if(m_flags.testFlag(Flag::IsPendingValueOwner) && !QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, object)){
                QTJAMBI_SET_OBJECTUSERDATA(ValueOwnerUserData, object, new ValueOwnerUserData(object));
            }
        }
        if(createdByQml()){
            setCppOwnership(env);
        }else if(createdByJava()){
            if(object->parent())
                setCppOwnership(env);
            else
                setJavaOwnership(env);
        }else{
            if(object->parent())
                setCppOwnership(env);
            else
                setSplitOwnership(env);
        }
    }
}

jobject PointerToPendingQObjectLink::getExtraSignal(JNIEnv * env, const QMetaMethod& method) const{
    if(QObject* object = qobject()){
        QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(object->metaObject());
        if(extraSignals.contains(method)){
            QReadLocker rlocker(QtJambiLinkUserData::lock());
            if(!m_extraSignals.contains(method.methodIndex())){
                rlocker.unlock();
                JObjectWrapper signal = QtJambiMetaObject::resolveExtraSignal(env, getJavaObjectLocalRef(env), method);
                {
                    QWriteLocker wlocker(QtJambiLinkUserData::lock());
                    if(!m_extraSignals.contains(method.methodIndex()))
                        m_extraSignals[method.methodIndex()] = signal;
                }
                rlocker.relock();
            }
            return m_extraSignals.value(method.methodIndex()).object(env);
        }
    }
    return nullptr;
}

PointerToPendingQObjectInterfaceLink::PointerToPendingQObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const InterfaceOffsetInfo& interfaceOffsetInfo, const QMetaObject* metaObject, QObject *ptr, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException)
    : PointerToQObjectInterfaceLink(env, nativeLink, jobj, interfaceOffsetInfo, metaObject, ptr, created_by_java, isDeclarativeCall, is_shell, ocurredException)
{
    m_flags.setFlag(Flag::IsPendingValueOwner, isValueOwner(metaObject));
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
        m_qtTypeName = metaObject->className();
#endif
}

void PointerToPendingQObjectInterfaceLink::init(JNIEnv* env){
    if(!isInitialized()){
        QtJambiLink::init(env);
        QObject* object = qobject();
        Q_ASSERT(object);
        {
            QWriteLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            QtJambiLinkUserData* lud = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, object);
            if(lud)
                QtJambiLink::resetDataLink(lud);
            else
                QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, object, new QtJambiLinkUserData(getWeakPointer()));
            if(m_flags.testFlag(Flag::IsPendingValueOwner) && !QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, object)){
                QTJAMBI_SET_OBJECTUSERDATA(ValueOwnerUserData, object, new ValueOwnerUserData(object));
            }
        }
        if(createdByQml()){
            setCppOwnership(env);
        }else if(createdByJava()){
            if(object->parent())
                setCppOwnership(env);
            else
                setJavaOwnership(env);
        }else{
            if(object->parent())
                setCppOwnership(env);
            else
                setSplitOwnership(env);
        }
    }
}

jobject PointerToPendingQObjectInterfaceLink::getExtraSignal(JNIEnv * env, const QMetaMethod& method) const{
    if(QObject* object = qobject()){
        QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(object->metaObject());
        if(extraSignals.contains(method)){
            QReadLocker rlocker(QtJambiLinkUserData::lock());
            if(!m_extraSignals.contains(method.methodIndex())){
                rlocker.unlock();
                JObjectWrapper signal = QtJambiMetaObject::resolveExtraSignal(env, getJavaObjectLocalRef(env), method);
                {
                    QWriteLocker wlocker(QtJambiLinkUserData::lock());
                    if(!m_extraSignals.contains(method.methodIndex()))
                        m_extraSignals[method.methodIndex()] = signal;
                }
                rlocker.relock();
            }
            return m_extraSignals.value(method.methodIndex()).object(env);
        }
    }
    return nullptr;
}

jobject PointerToQObjectWithExtraSignalsLink::getExtraSignal(JNIEnv * env, const QMetaMethod& method) const{
    if(QObject* object = qobject()){
        QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(object->metaObject());
        if(extraSignals.contains(method)){
            QReadLocker rlocker(QtJambiLinkUserData::lock());
            if(!m_extraSignals.contains(method.methodIndex())){
                rlocker.unlock();
                JObjectWrapper signal = QtJambiMetaObject::resolveExtraSignal(env, getJavaObjectLocalRef(env), method);
                {
                    QWriteLocker wlocker(QtJambiLinkUserData::lock());
                    if(!m_extraSignals.contains(method.methodIndex()))
                        m_extraSignals[method.methodIndex()] = signal;
                }
                rlocker.relock();
            }
            return m_extraSignals.value(method.methodIndex()).object(env);
        }
    }
    return nullptr;
}

jobject PointerToQObjectInterfaceWithExtraSignalsLink::getExtraSignal(JNIEnv * env, const QMetaMethod& method) const{
    if(QObject* object = qobject()){
        QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(object->metaObject());
        if(extraSignals.contains(method)){
            QReadLocker rlocker(QtJambiLinkUserData::lock());
            if(!m_extraSignals.contains(method.methodIndex())){
                rlocker.unlock();
                JObjectWrapper signal = QtJambiMetaObject::resolveExtraSignal(env, getJavaObjectLocalRef(env), method);
                {
                    QWriteLocker wlocker(QtJambiLinkUserData::lock());
                    if(!m_extraSignals.contains(method.methodIndex()))
                        m_extraSignals[method.methodIndex()] = signal;
                }
                rlocker.relock();
            }
            return m_extraSignals.value(method.methodIndex()).object(env);
        }
    }
    return nullptr;
}

QString PointerToObjectLink::describe() const{
    QString strg = "[deletionPolicy=%1, createdByJava=%2, hasBeenCleaned=%3, isJavaLinkDetached=%4, isDeleteLater=%5, ownership=%6, metaType=%7, metaTypeId=%8]";
    strg = strg.arg("Normal");
    strg = strg
            .arg(createdByJava() ? "true" : "false",
                    m_flags.testFlag(Flag::HasBeenCleaned) ? "true" : "false",
                    m_flags.testFlag(Flag::IsJavaLinkDetached) ? "true" : "false",
                    isDeleteLater() ? "true" : "false");
    if(m_flags.testFlag(Flag::JavaOwnership)) strg = strg.arg("Java");
    else if(m_flags.testFlag(Flag::CppOwnership)) strg = strg.arg("Cpp");
    else if(m_flags.testFlag(Flag::SplitOwnership)) strg = strg.arg("Split");
    else strg = strg.arg("InvalidOwnership");
    if(metaType()){
        strg = strg.arg(QLatin1String(metaType()->name()));
        strg = strg.arg(metaType()->id());
    }else{
        strg = strg.arg(QLatin1String("UnknownType"));
        strg = strg.arg(0);
    }
    return strg;
}

QString PointerToQObjectLink::describe() const{
    QString strg;
    if(qobject()){
        strg += QString("class=%1, objectName=%2, ")
                    .arg(QLatin1String(qobject()->metaObject()->className()), qobject()->objectName());
    }
    strg += QString("createdByJava=%1, hasBeenCleaned=%2, isJavaLinkDetached=%3, ownership=%4")
            .arg(createdByJava() ? "true" : "false",
                     m_flags.testFlag(Flag::HasBeenCleaned) ? "true" : "false",
                     m_flags.testFlag(Flag::IsJavaLinkDetached) ? "true" : "false");
    if(m_flags.testFlag(Flag::JavaOwnership)) strg = strg.arg("Java");
    else if(m_flags.testFlag(Flag::CppOwnership)) strg = strg.arg("Cpp");
    else if(m_flags.testFlag(Flag::SplitOwnership)) strg = strg.arg("Split");
    else strg = strg.arg("InvalidOwnership");
    return QString("[%1]").arg(strg);
}

template<template<typename> class SmartPointer>
QString SmartPointerToObjectLink<SmartPointer>::describe() const{
    QString strg = "[deletionPolicy=%1, createdByJava=%2, hasBeenCleaned=%3, objectInvalid=%4, pointerZeroed=%5, ownership=%6, metaType=%7, metaTypeId=%8]";
    if(m_owner_function) strg = strg.arg("InSpecificThread");
    else strg = strg.arg("Normal");
    strg = strg
            .arg(QtJambiLink::createdByJava() ? "true" : "false",
                    this->m_flags.testFlag(QtJambiLink::Flag::HasBeenCleaned) ? "true" : "false",
                    this->m_flags.testFlag(QtJambiLink::Flag::IsJavaLinkDetached) ? "true" : "false",
                    isPointerZeroed() ? "true" : "false");
    if(this->m_flags.testFlag(QtJambiLink::Flag::JavaOwnership)) strg = strg.arg("JavaOwnership");
    else if(this->m_flags.testFlag(QtJambiLink::Flag::CppOwnership)) strg = strg.arg("CppOwnership");
    else if(this->m_flags.testFlag(QtJambiLink::Flag::SplitOwnership)) strg = strg.arg("SplitOwnership");
    else strg = strg.arg("InvalidOwnership");
    return strg;
}

template<template<typename> class SmartPointer>
QString SmartPointerToQObjectLink<SmartPointer>::describe() const{
    QString strg;
    if(qobject()){
        strg += QString("class=%1, objectName=%2, ").arg(QLatin1String(qobject()->metaObject()->className()), qobject()->objectName());
    }
    strg += QString("createdByJava=%1, hasBeenCleaned=%2, objectInvalid=%3, ownership=%4")
            .arg(QtJambiLink::createdByJava() ? "true" : "false",
                     this->m_flags.testFlag(QtJambiLink::Flag::HasBeenCleaned) ? "true" : "false",
                     this->m_flags.testFlag(QtJambiLink::Flag::IsJavaLinkDetached) ? "true" : "false");
    if(this->m_flags.testFlag(QtJambiLink::Flag::JavaOwnership)) strg = strg.arg("JavaOwnership");
    else if(this->m_flags.testFlag(QtJambiLink::Flag::CppOwnership)) strg = strg.arg("CppOwnership");
    else if(this->m_flags.testFlag(QtJambiLink::Flag::SplitOwnership)) strg = strg.arg("SplitOwnership");
    else strg = strg.arg("InvalidOwnership");
    return QString("[%1]").arg(strg);
}

template<template<typename> class SmartPointer>
SmartPointerToQObjectWithPendingExtraSignalsLink<SmartPointer>::SmartPointerToQObjectWithPendingExtraSignalsLink(JNIEnv *env,
                                                                                                                 jobject nativeLink,
                                                                                                                 jobject jobj,
                                                                                                                 bool created_by_java,
                                                                                                                 QtJambiLink::Ownership ownership,
                                                                                                                 bool is_shell,
                                                                                                                 QObject* pointer,
                                                                                                                 SmartPointer<QObject>*& smartPointer,
                                                                                                                 const QMetaObject* superTypeForCustomMetaObject,
                                                                                                                 JavaException& ocurredException)
    : SmartPointerToQObjectLink<SmartPointer>(env, nativeLink,
                                LINK_NAME_ARG(superTypeForCustomMetaObject->className())
                                created_by_java, ownership, is_shell, pointer, smartPointer, ocurredException),
      m_extraSignals()
{
    jobject local = env->NewLocalRef(jobj);
    if(!ocurredException){
        QtJambiMetaObject::resolveSignals(env, jobj, superTypeForCustomMetaObject, ocurredException);
    }
    if(!ocurredException){
        bool _isValueOwner = isValueOwner(superTypeForCustomMetaObject);
        {
            QWriteLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            QtJambiLinkUserData* lud = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, pointer);
            if(lud)
                QtJambiLink::resetDataLink(lud);
            else
                QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, pointer, new QtJambiLinkUserData(QtJambiLink::getWeakPointer()));
            if(_isValueOwner && !QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, pointer)){
                QTJAMBI_SET_OBJECTUSERDATA(ValueOwnerUserData, pointer, new ValueOwnerUserData(pointer));
            }
        }
    }else{
        this->m_flags.setFlag(QtJambiLink::Flag::IsJavaLinkDetached);
    }
    env->DeleteLocalRef(local);
}

template<template<typename> class SmartPointer>
SmartPointerToQObjectWithPendingExtraSignalsLink<SmartPointer>::SmartPointerToQObjectWithPendingExtraSignalsLink(JNIEnv *env, jobject nativeLink,
                                                                                                                 LINK_NAME_ARG(const char *qt_name)
                                                                                                                 bool created_by_java,
                                                                                                                 QtJambiLink::Ownership ownership,
                                                                                                                 bool is_shell,
                                                                                                                 QObject* pointer,
                                                                                                                 SmartPointer<QObject>*& smartPointer,
                                                                                                                 JavaException& ocurredException)
    : SmartPointerToQObjectLink<SmartPointer>(env, nativeLink,
                                LINK_NAME_ARG(qt_name)
                                created_by_java, ownership, is_shell, pointer, smartPointer, ocurredException),
      m_extraSignals()
{
}

template<template<typename> class SmartPointer>
jobject SmartPointerToQObjectWithPendingExtraSignalsLink<SmartPointer>::getExtraSignal(JNIEnv * env, const QMetaMethod& method) const{
    if(QObject* object = this->qobject()){
        QList<QMetaMethod> _extraSignals = getExtraSignalsOfMetaObject(object->metaObject());
        if(_extraSignals.contains(method)){
            QReadLocker rlocker(QtJambiLinkUserData::lock());
            if(!m_extraSignals.contains(method.methodIndex())){
                rlocker.unlock();
                JObjectWrapper signal = QtJambiMetaObject::resolveExtraSignal(env, QtJambiLink::getJavaObjectLocalRef(env), method);
                {
                    QWriteLocker wlocker(QtJambiLinkUserData::lock());
                    if(!m_extraSignals.contains(method.methodIndex()))
                        m_extraSignals[method.methodIndex()] = signal;
                }
                rlocker.relock();
            }
            return m_extraSignals.value(method.methodIndex()).object(env);
        }
    }
    return nullptr;
}

template<template<typename> class SmartPointer>
SmartPointerToPlainQObjectLink<SmartPointer>::SmartPointerToPlainQObjectLink(JNIEnv *env,
                                                                             jobject nativeLink,
                                                                             jobject jobj,
                                                                             bool created_by_java,
                                                                             QtJambiLink::Ownership ownership,
                                                                             bool is_shell,
                                                                             QObject* pointer,
                                                                             SmartPointer<QObject>*& smartPointer,
                                                                             JavaException& ocurredException)
    : SmartPointerToQObjectLink<SmartPointer>(env, nativeLink,
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
        QWriteLocker locker(QtJambiLinkUserData::lock());
        Q_UNUSED(locker)
        QtJambiLinkUserData* lud = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, pointer);
        if(lud)
            QtJambiLink::resetDataLink(lud);
        else if(metaObject->inherits(&QThread::staticMetaObject) && pointer!=QCoreApplicationPrivate::theMainThread.loadRelaxed())
            QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, pointer, new QtJambiThreadLinkUserData(QtJambiLink::getWeakPointer(), metaObject));
        else
            QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, pointer, new QtJambiMetaObjectLinkUserData(QtJambiLink::getWeakPointer(), metaObject));
        if(_isValueOwner && !QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, pointer)){
            QTJAMBI_SET_OBJECTUSERDATA(ValueOwnerUserData, pointer, new ValueOwnerUserData(pointer));
        }
    }else{
        this->m_flags.setFlag(QtJambiLink::Flag::IsJavaLinkDetached);
    }
    env->DeleteLocalRef(local);
}

template<template<typename> class SmartPointer>
SmartPointerToQObjectWithExtraSignalsLink<SmartPointer>::SmartPointerToQObjectWithExtraSignalsLink(JNIEnv *env,
                                                                                                   jobject nativeLink,
                                                                                                   jobject jobj,
                                                                                                   bool created_by_java,
                                                                                                   QtJambiLink::Ownership ownership,
                                                                                                   bool is_shell,
                                                                                                   QObject* pointer,
                                                                                                   SmartPointer<QObject>*& smartPointer,
                                                                                                   JavaException& ocurredException)
    : SmartPointerToQObjectWithPendingExtraSignalsLink<SmartPointer>(env, nativeLink,
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
        QWriteLocker locker(QtJambiLinkUserData::lock());
        Q_UNUSED(locker)
        QtJambiLinkUserData* lud = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, pointer);
        if(lud)
            QtJambiLink::resetDataLink(lud);
        else if(metaObject->inherits(&QThread::staticMetaObject) && pointer!=QCoreApplicationPrivate::theMainThread.loadRelaxed())
            QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, pointer, new QtJambiThreadLinkUserData(QtJambiLink::getWeakPointer(), metaObject));
        else
            QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, pointer, new QtJambiMetaObjectLinkUserData(QtJambiLink::getWeakPointer(), metaObject));
        if(_isValueOwner && !QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, pointer)){
            QTJAMBI_SET_OBJECTUSERDATA(ValueOwnerUserData, pointer, new ValueOwnerUserData(pointer));
        }
    }else{
        this->m_flags.setFlag(QtJambiLink::Flag::IsJavaLinkDetached);
    }
    env->DeleteLocalRef(local);
}

// ### END #################  SmartPointerToQObjectLink  ################### END ###

PointerToQObjectInterfaceWithExtraSignalsLink::PointerToQObjectInterfaceWithExtraSignalsLink(JNIEnv *env, jobject nativeLink, jobject jobj, const InterfaceOffsetInfo& interfaceOffsetInfo, const QMetaObject* metaObject, QObject *ptr, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException)
    : PointerToQObjectInterfaceLink(env, nativeLink, jobj, interfaceOffsetInfo, metaObject, ptr, created_by_java, isDeclarativeCall, is_shell, ocurredException)
{
}

PointerToQObjectInterfaceLink::PointerToQObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const InterfaceOffsetInfo& interfaceOffsetInfo, const QMetaObject* metaObject, QObject *ptr, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException)
    : PointerToQObjectLink(env, nativeLink, jobj, metaObject, ptr, created_by_java, isDeclarativeCall, is_shell, ocurredException), m_interfaceOffsetInfo(interfaceOffsetInfo), m_availableInterfaces(interfaceOffsetInfo.interfaces)
{
}

PointerToObjectInterfaceLink::PointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                           LINK_NAME_ARG(const char *qt_name)
                                                           void *ptr, bool created_by_java, bool is_shell, JavaException& ocurredException)
    : PointerToObjectLink(env, nativeLink,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, is_shell, ocurredException), m_interfaceOffsetInfo(interfaceOffsetInfo), m_availableInterfaces(interfaceOffsetInfo.interfaces)
{
}

OwnedMetaTypedPointerToObjectInterfaceLink::OwnedMetaTypedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                           void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
    : OwnedMetaTypedPointerToObjectLink(env, nativeLink,
                          ptr, metaType, created_by_java, is_shell, ownerFunction, ocurredException), m_interfaceOffsetInfo(interfaceOffsetInfo), m_availableInterfaces(interfaceOffsetInfo.interfaces)
{
}

PointerToContainerInterfaceLink::PointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                           LINK_NAME_ARG(const char *qt_name)
                                                           void *ptr, bool created_by_java, bool is_shell, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : PointerToContainerLink(env, nativeLink,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, is_shell, containerAccess, ocurredException), m_interfaceOffsetInfo(interfaceOffsetInfo), m_availableInterfaces(interfaceOffsetInfo.interfaces)
{
}

MetaTypedPointerToObjectInterfaceLink::MetaTypedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                           void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, JavaException& ocurredException)
    : MetaTypedPointerToObjectLink(env, nativeLink,
                          ptr, metaType, created_by_java, is_shell, ocurredException), m_interfaceOffsetInfo(interfaceOffsetInfo), m_availableInterfaces(interfaceOffsetInfo.interfaces)
{
}

DeletablePointerToObjectInterfaceLink::DeletablePointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                           LINK_NAME_ARG(const char *qt_name)
                                                           void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, JavaException& ocurredException)
    : DeletablePointerToObjectLink(env, nativeLink,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, is_shell, destructor_function, ocurredException), m_interfaceOffsetInfo(interfaceOffsetInfo), m_availableInterfaces(interfaceOffsetInfo.interfaces)
{
}

DeletablePointerToContainerInterfaceLink::DeletablePointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                           LINK_NAME_ARG(const char *qt_name)
                                                           void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : DeletablePointerToContainerLink(env, nativeLink,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, is_shell, destructor_function, containerAccess, ocurredException), m_interfaceOffsetInfo(interfaceOffsetInfo), m_availableInterfaces(interfaceOffsetInfo.interfaces)
{
}

DeletableOwnedPointerToObjectInterfaceLink::DeletableOwnedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                           LINK_NAME_ARG(const char *qt_name)
                                                           void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
    : DeletableOwnedPointerToObjectLink(env, nativeLink,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, is_shell, destructor_function, ownerFunction, ocurredException), m_interfaceOffsetInfo(interfaceOffsetInfo), m_availableInterfaces(interfaceOffsetInfo.interfaces)
{
}

DeletableOwnedPointerToContainerInterfaceLink::DeletableOwnedPointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                           LINK_NAME_ARG(const char *qt_name)
                                                           void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : DeletableOwnedPointerToContainerLink(env, nativeLink,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, is_shell, destructor_function, ownerFunction, containerAccess, ocurredException), m_interfaceOffsetInfo(interfaceOffsetInfo), m_availableInterfaces(interfaceOffsetInfo.interfaces)
{
}

template<template<typename> class SmartPointer>
SmartPointerToObjectInterfaceLink<SmartPointer>::SmartPointerToObjectInterfaceLink(JNIEnv *env,
                                                                                   jobject nativeLink,
                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                   LINK_NAME_ARG(const char* qt_name)
                                                                                   bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                   PtrOwnerFunction ownerFunction,
                                                                                   void* pointer,
                                                                                   SmartPointer<char>*& smartPointer,
                                                                                   JavaException& ocurredException)
    : SmartPointerToObjectLink<SmartPointer>(env, nativeLink,
                                 LINK_NAME_ARG(qt_name)
                                 created_by_java, ownership, is_shell, ownerFunction, pointer, smartPointer, ocurredException),
      m_interfaceOffsetInfo(interfaceOffsetInfo), m_availableInterfaces(interfaceOffsetInfo.interfaces)
{
    for(auto i = interfaceOffsetInfo.offsets.keyValueBegin(); i!=interfaceOffsetInfo.offsets.keyValueEnd(); ++i){
        QtJambiLink::registerPointer(reinterpret_cast<void*>( quintptr(pointer) + i.base().value() ));
    }
}

template<template<typename> class SmartPointer>
SmartPointerToContainerInterfaceLink<SmartPointer>::SmartPointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink,
                                                                                         const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                         LINK_NAME_ARG(const char* qt_name)
                                                                                         bool created_by_java, QtJambiLink::Ownership ownership, bool is_shell,
                                                                                         PtrOwnerFunction ownerFunction,
                                                                                         void* pointer,
                                                                                         SmartPointer<char>*& smartPointer,
                                                                                         AbstractContainerAccess* containerAccess,
                                                                                         JavaException& ocurredException)
    : SmartPointerToContainerLink<SmartPointer>(env, nativeLink,
                                          LINK_NAME_ARG(qt_name)
                                          created_by_java, ownership, is_shell, ownerFunction, pointer, smartPointer, containerAccess, ocurredException),
      m_interfaceOffsetInfo(interfaceOffsetInfo), m_availableInterfaces(interfaceOffsetInfo.interfaces)
{
    for(auto i = interfaceOffsetInfo.offsets.keyValueBegin(); i!=interfaceOffsetInfo.offsets.keyValueEnd(); ++i){
        QtJambiLink::registerPointer(reinterpret_cast<void*>( quintptr(pointer) + i.base().value() ));
    }
}

template<template<typename> class SmartPointer>
SmartPointerToPlainQObjectInterfaceLink<SmartPointer>::SmartPointerToPlainQObjectInterfaceLink(JNIEnv *env,
                                                                                               jobject nativeLink,
                                                                                               jobject jobj,
                                                                                               const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                               bool created_by_java,
                                                                                               QtJambiLink::Ownership ownership,
                                                                                               bool is_shell,
                                                                                               QObject* pointer,
                                                                                               SmartPointer<QObject>*& smartPointer,
                                                                                               JavaException& ocurredException)
    : SmartPointerToPlainQObjectLink<SmartPointer>(env, nativeLink, jobj, created_by_java, ownership, is_shell, pointer, smartPointer, ocurredException),
      m_interfaceOffsetInfo(interfaceOffsetInfo), m_availableInterfaces(interfaceOffsetInfo.interfaces)
{
    for(auto i = interfaceOffsetInfo.offsets.keyValueBegin(); i!=interfaceOffsetInfo.offsets.keyValueEnd(); ++i){
        QtJambiLink::registerPointer(reinterpret_cast<void*>( quintptr(pointer) + i.base().value() ));
    }
}

template<template<typename> class SmartPointer>
SmartPointerToQObjectInterfaceWithPendingExtraSignalsLink<SmartPointer>::SmartPointerToQObjectInterfaceWithPendingExtraSignalsLink(JNIEnv *env,
                                                                                                                                   jobject nativeLink,
                                                                                                                                   jobject jobj,
                                                                                                                                   const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                                                                   bool created_by_java,
                                                                                                                                   QtJambiLink::Ownership ownership,
                                                                                                                                   bool is_shell,
                                                                                                                                   QObject* pointer,
                                                                                                                                   SmartPointer<QObject>*& smartPointer,
                                                                                                                                   const QMetaObject* superTypeForCustomMetaObject,
                                                                                                                                   JavaException& ocurredException)
    : SmartPointerToQObjectWithPendingExtraSignalsLink<SmartPointer>(env, nativeLink, jobj, created_by_java, ownership, is_shell, pointer, smartPointer, superTypeForCustomMetaObject, ocurredException),
      m_interfaceOffsetInfo(interfaceOffsetInfo), m_availableInterfaces(interfaceOffsetInfo.interfaces)
{
    for(auto i = interfaceOffsetInfo.offsets.keyValueBegin(); i!=interfaceOffsetInfo.offsets.keyValueEnd(); ++i){
        QtJambiLink::registerPointer(reinterpret_cast<void*>( quintptr(pointer) + i.base().value() ));
    }
}

template<template<typename> class SmartPointer>
SmartPointerToQObjectInterfaceWithExtraSignalsLink<SmartPointer>::SmartPointerToQObjectInterfaceWithExtraSignalsLink(JNIEnv *env,
                                                                                                                     jobject nativeLink,
                                                                                                                     jobject jobj,
                                                                                                                     const InterfaceOffsetInfo& interfaceOffsetInfo,
                                                                                                                     bool created_by_java,
                                                                                                                     QtJambiLink::Ownership ownership,
                                                                                                                     bool is_shell,
                                                                                                                     QObject* pointer,
                                                                                                                     SmartPointer<QObject>*& smartPointer,
                                                                                                                     JavaException& ocurredException)
    : SmartPointerToQObjectWithExtraSignalsLink<SmartPointer>(env, nativeLink, jobj, created_by_java, ownership, is_shell, pointer, smartPointer, ocurredException),
      m_interfaceOffsetInfo(interfaceOffsetInfo), m_availableInterfaces(interfaceOffsetInfo.interfaces)
{
    for(auto i = interfaceOffsetInfo.offsets.keyValueBegin(); i!=interfaceOffsetInfo.offsets.keyValueEnd(); ++i){
        QtJambiLink::registerPointer(reinterpret_cast<void*>( quintptr(pointer) + i.base().value() ));
    }
}

ExtendedLink::~ExtendedLink(){}

ExtendedDeletablePointerToObjectLink::ExtendedDeletablePointerToObjectLink(JNIEnv *env, jobject nativeLink,
                                                                             LINK_NAME_ARG(const char* qt_name)
                                                                             void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function,
                                                                             const QObject* extension, JavaException& ocurredException)
    : DeletablePointerToObjectLink(env, nativeLink,
                                   LINK_NAME_ARG(qt_name)
                                   ptr, created_by_java, is_shell, destructor_function, ocurredException),
    ExtendedLink(),
    m_extension(extension)
{}

DependentDeletablePointerToObjectLink::DependentDeletablePointerToObjectLink(JNIEnv *env, jobject nativeLink,
                                                           LINK_NAME_ARG(const char* qt_name)
                                                           void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, JavaException& ocurredException)
    : DeletablePointerToObjectLink(env, nativeLink,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, is_shell, destructor_function, ocurredException)
{}

void DependentDeletablePointerToObjectLink::setDependencyManager(DependencyManagerUserData* dependencyManager) {
    m_dependencyManager = dependencyManager;
}

void DependentDeletablePointerToObjectLink::deleteNativeObject(JNIEnv *env, bool forced){
    if(m_dependencyManager)
        m_dependencyManager->removeDependentObject(m_this);
    DeletablePointerToObjectLink::deleteNativeObject(env, forced);
}

void DependentDeletablePointerToObjectLink::invalidate(JNIEnv *env){
    if(m_dependencyManager)
        m_dependencyManager->removeDependentObject(m_this);
    DeletablePointerToObjectLink::invalidate(env);
}

DependentMetaTypedPointerToObjectLink::DependentMetaTypedPointerToObjectLink(JNIEnv *env, jobject nativeLink,
                                                                             void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, JavaException& ocurredException)
    : MetaTypedPointerToObjectLink(env, nativeLink, ptr, metaType, created_by_java, is_shell, ocurredException)
{}

void DependentMetaTypedPointerToObjectLink::setDependencyManager(DependencyManagerUserData* dependencyManager) {
    m_dependencyManager = dependencyManager;
}

void DependentMetaTypedPointerToObjectLink::deleteNativeObject(JNIEnv *env, bool forced){
    if(m_dependencyManager)
        m_dependencyManager->removeDependentObject(m_this);
    MetaTypedPointerToObjectLink::deleteNativeObject(env, forced);
}

void DependentMetaTypedPointerToObjectLink::invalidate(JNIEnv *env){
    if(m_dependencyManager)
        m_dependencyManager->removeDependentObject(m_this);
    MetaTypedPointerToObjectLink::invalidate(env);
}

OwnedDependentMetaTypedPointerToObjectLink::OwnedDependentMetaTypedPointerToObjectLink(JNIEnv *env, jobject nativeLink,
                    void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell,
                    PtrOwnerFunction ownerFunction, JavaException& ocurredException)
    : OwnedMetaTypedPointerToObjectLink(env, nativeLink,
       ptr, metaType, created_by_java, is_shell,
       ownerFunction, ocurredException){}

void OwnedDependentMetaTypedPointerToObjectLink::setDependencyManager(DependencyManagerUserData* dependencyManager) {
    m_dependencyManager = dependencyManager;
}

void OwnedDependentMetaTypedPointerToObjectLink::deleteNativeObject(JNIEnv *env, bool forced){
    if(m_dependencyManager)
        m_dependencyManager->removeDependentObject(m_this);
    OwnedMetaTypedPointerToObjectLink::deleteNativeObject(env, forced);
}

void OwnedDependentMetaTypedPointerToObjectLink::invalidate(JNIEnv *env){
    if(m_dependencyManager)
        m_dependencyManager->removeDependentObject(m_this);
    OwnedMetaTypedPointerToObjectLink::invalidate(env);
}

DependentMetaTypedPointerToObjectInterfaceLink::DependentMetaTypedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
                                               void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, JavaException& ocurredException)
    : MetaTypedPointerToObjectInterfaceLink(env, nativeLink, interfaceOffsetInfo,
                                            ptr, metaType, created_by_java, is_shell, ocurredException)
{
}


void DependentMetaTypedPointerToObjectInterfaceLink::setDependencyManager(DependencyManagerUserData* dependencyManager) {
    m_dependencyManager = dependencyManager;
}

void DependentMetaTypedPointerToObjectInterfaceLink::deleteNativeObject(JNIEnv *env, bool forced){
    if(m_dependencyManager)
        m_dependencyManager->removeDependentObject(m_this);
    MetaTypedPointerToObjectInterfaceLink::deleteNativeObject(env, forced);
}

void DependentMetaTypedPointerToObjectInterfaceLink::invalidate(JNIEnv *env){
    if(m_dependencyManager)
        m_dependencyManager->removeDependentObject(m_this);
    MetaTypedPointerToObjectInterfaceLink::invalidate(env);
}

OwnedDependentMetaTypedPointerToObjectInterfaceLink::OwnedDependentMetaTypedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const InterfaceOffsetInfo& interfaceOffsetInfo,
                                               void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
    : OwnedMetaTypedPointerToObjectInterfaceLink(env, nativeLink, interfaceOffsetInfo,
                                            ptr, metaType, created_by_java, is_shell, ownerFunction, ocurredException)
{
}


void OwnedDependentMetaTypedPointerToObjectInterfaceLink::setDependencyManager(DependencyManagerUserData* dependencyManager) {
    m_dependencyManager = dependencyManager;
}

void OwnedDependentMetaTypedPointerToObjectInterfaceLink::deleteNativeObject(JNIEnv *env, bool forced){
    if(m_dependencyManager)
        m_dependencyManager->removeDependentObject(m_this);
    OwnedMetaTypedPointerToObjectInterfaceLink::deleteNativeObject(env, forced);
}

void OwnedDependentMetaTypedPointerToObjectInterfaceLink::invalidate(JNIEnv *env){
    if(m_dependencyManager)
        m_dependencyManager->removeDependentObject(m_this);
    OwnedMetaTypedPointerToObjectInterfaceLink::invalidate(env);
}

#define INTERFACE_METHODS4(LINKTYPE,DESTRUCTOR,LINKSUPERTYPE,TEMPLATE) \
TEMPLATE \
LINKTYPE::~DESTRUCTOR()\
{\
    LINKTYPE::unregisterOffsets();\
}\
TEMPLATE \
void LINKTYPE::unregisterOffsets() {\
    if(!this->m_flags.testFlag(QtJambiLink::Flag::IsOffsetsUnregistered)){\
        QVector<void*> pointers;\
        for(auto i = m_interfaceOffsetInfo.offsets.keyValueBegin(); i!=m_interfaceOffsetInfo.offsets.keyValueEnd(); ++i){\
            pointers << reinterpret_cast<void*>( quintptr(this->pointer()) + i.base().value() );\
        }\
        this->unregisterPointers(pointers);\
        this->m_flags.setFlag(QtJambiLink::Flag::IsOffsetsUnregistered);\
    }\
}\
TEMPLATE \
void LINKTYPE::addInterface(const std::type_info& qtInterfaceType){\
    m_availableInterfaces << unique_id(qtInterfaceType);\
    size_t offset = m_interfaceOffsetInfo.offsets.value(unique_id(qtInterfaceType));\
    if(offset>0){\
        this->registerPointer(reinterpret_cast<void*>( quintptr(this->pointer()) + offset ));\
    }else{\
        const QSet<const std::type_info*> inheritedInterfaces = m_interfaceOffsetInfo.inheritedInterfaces[unique_id(qtInterfaceType)];\
        for(const std::type_info* ifaces : inheritedInterfaces){\
            Q_ASSERT(ifaces);\
            this->addInterface(*ifaces);\
        }\
    }\
}\
TEMPLATE \
void LINKTYPE::removeInterface(const std::type_info& qtInterfaceType){\
    if(m_availableInterfaces.contains(unique_id(qtInterfaceType))){\
        m_availableInterfaces.remove(unique_id(qtInterfaceType));\
        if(m_availableInterfaces.isEmpty())\
            this->clearAllDependencies(); \
        if(m_interfaceOffsetInfo.offsets.contains(unique_id(qtInterfaceType))){\
            size_t offset = m_interfaceOffsetInfo.offsets.value(unique_id(qtInterfaceType));\
            if(offset>0)\
                this->unregisterPointer(reinterpret_cast<void*>( quintptr(this->pointer()) + offset ));\
            else\
                this->setInDestructor();\
        }else{\
            const QSet<const std::type_info*> inheritedInterfaces = m_interfaceOffsetInfo.inheritedInterfaces[unique_id(qtInterfaceType)];\
            for(const std::type_info* ifaces : inheritedInterfaces){\
                Q_ASSERT(ifaces);\
                this->removeInterface(*ifaces);\
            }\
            this->setInDestructor();\
        }\
    }\
}\
TEMPLATE \
void* LINKTYPE::typedPointer(const std::type_info& qtInterfaceType) const{\
    if(m_interfaceOffsetInfo.offsets.contains(unique_id(qtInterfaceType))){\
        if(!m_availableInterfaces.contains(unique_id(qtInterfaceType)))\
            return nullptr;\
        size_t offset = m_interfaceOffsetInfo.offsets.value(unique_id(qtInterfaceType));\
        if(offset>0)\
            return reinterpret_cast<void*>( quintptr(this->pointer()) + offset );\
    }\
    return this->pointer();\
}\
TEMPLATE \
bool LINKTYPE::isMultiInheritanceType() const{\
    return true;\
}\
TEMPLATE \
bool LINKTYPE::isInterfaceAvailable(const std::type_info& qtInterfaceType) const{\
    return m_availableInterfaces.contains(unique_id(qtInterfaceType));\
}\
TEMPLATE \
const InterfaceOffsetInfo* LINKTYPE::interfaceOffsetInfo() const{\
    return &m_interfaceOffsetInfo;\
}

#define INTERFACE_METHODS(LINKTYPE,LINKSUPERTYPE) INTERFACE_METHODS4(LINKTYPE,LINKTYPE,LINKSUPERTYPE, )
#define TEMPLATE_INTERFACE_METHODS(LINKTYPE,LINKSUPERTYPE) INTERFACE_METHODS4(LINKTYPE<SmartPointer>,LINKTYPE,LINKSUPERTYPE<SmartPointer>,template<template<typename> class SmartPointer>)

INTERFACE_METHODS(DeletableOwnedPointerToObjectInterfaceLink, PointerToObjectLink)
INTERFACE_METHODS(DeletablePointerToObjectInterfaceLink, PointerToObjectLink)
INTERFACE_METHODS(OwnedMetaTypedPointerToObjectInterfaceLink, PointerToObjectLink)
INTERFACE_METHODS(MetaTypedPointerToObjectInterfaceLink, PointerToObjectLink)
INTERFACE_METHODS(PointerToObjectInterfaceLink, PointerToObjectLink)
INTERFACE_METHODS(PointerToQObjectInterfaceLink, PointerToQObjectLink)
INTERFACE_METHODS(PointerToContainerInterfaceLink, PointerToContainerLink)
INTERFACE_METHODS(DeletablePointerToContainerInterfaceLink, DeletablePointerToContainerLink)
INTERFACE_METHODS(DeletableOwnedPointerToContainerInterfaceLink, DeletableOwnedPointerToContainerLink)
TEMPLATE_INTERFACE_METHODS(SmartPointerToPlainQObjectInterfaceLink, SmartPointerToPlainQObjectLink)
TEMPLATE_INTERFACE_METHODS(SmartPointerToQObjectInterfaceWithPendingExtraSignalsLink, SmartPointerToQObjectWithPendingExtraSignalsLink)
TEMPLATE_INTERFACE_METHODS(SmartPointerToQObjectInterfaceWithExtraSignalsLink, SmartPointerToQObjectWithExtraSignalsLink)
TEMPLATE_INTERFACE_METHODS(SmartPointerToObjectInterfaceLink, SmartPointerToObjectLink)
TEMPLATE_INTERFACE_METHODS(SmartPointerToContainerInterfaceLink, SmartPointerToContainerLink)
