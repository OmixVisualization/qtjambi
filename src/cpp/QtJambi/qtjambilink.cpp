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

namespace Java{
namespace QtJambi{
QTJAMBI_REPOSITORY_DECLARE_CLASS(NativeUtility$NativeLink,
              QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(initialize)
              QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(detach)
              QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(reset)
              QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(assignNativeId)
              QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(nativeId)
              QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(get)
              QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(takeOwnership)
              QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(releaseOwnership))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,NativeUtility$NativeLink,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(initialize,(J)V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(detach,(JZ)V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(reset,(JZ)V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(assignNativeId,(J)V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(nativeId,()J)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(get,()Lio/qt/QtObjectInterface;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(takeOwnership,()V)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(releaseOwnership,()V)
)
namespace Private{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(NativeUtility$Object,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(nativeLink)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(assignNativeLink))
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,NativeUtility$Object,
        QTJAMBI_REPOSITORY_DEFINE_FIELD(nativeLink,Lio/qt/internal/NativeUtility$NativeLink;)
        QTJAMBI_REPOSITORY_DEFINE_METHOD(assignNativeLink,(J)V)
    )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(NativeUtility,
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(findInterfaceLink)
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(findAndAssignInterfaceLink))
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,NativeUtility,
                                    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(findInterfaceLink,(Lio/qt/QtObjectInterface;ZZ)Lio/qt/internal/NativeUtility$NativeLink;)
                                    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(findAndAssignInterfaceLink,(Lio/qt/QtObjectInterface;ZZJ)V))
}
}
}

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
if(!isDebugMessagingDisabled())\
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

class PointerToObjectLink;
class PointerToQObjectLink;
class SmartPointerToObjectLink;
class SmartPointerToQObjectLink;

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
    m_meta_type.destroy(pointer);
}

#define THREAD_ID() reinterpret_cast<void*>(QThread::currentThreadId())

#if defined(QTJAMBI_DEBUG_TOOLS)
typedef QSet<QtJambiLink*> QtJambiLinkList;
Q_GLOBAL_STATIC(QtJambiLinkList, gQtJambiLinkList);
Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gStaticQtJambiLinkListLock, (QReadWriteLock::Recursive));

void QtJambiLink::registerLink(QtJambiLink* link)
{
    if(link->m_flags.testFlag(Flag::IsListed)) {
        char xb[24];
        fprintf(stderr, "QtJambiLink::registerLink(%p %s) ERROR: LINK ALREADY LISTED, qtType=%s\n", reinterpret_cast<void*>(link), link->debugFlagsToString(xb), link->qtTypeName());
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
        char xb[24];
        fprintf(stderr, "QtJambiLink::unregisterLink(%p %s) ERROR: LINK NOT LISTED, qtType=%s\n", reinterpret_cast<void*>(link), link->debugFlagsToString(xb), link->qtTypeName());
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
    QList<QSharedPointer<QtJambiLink>> sharedPointers;
    char xb[24];
    int count = 0;
    QtJambiLinkList set;
    {
        QReadLocker lock(gStaticQtJambiLinkListLock());
        set = *gQtJambiLinkList;
    }
    for(QtJambiLink* _link : qAsConst(set)) {
        sharedPointers << _link->m_this;
        if(!_link->m_this)
            continue;
        jobject obj = env->NewLocalRef(_link->m_java.object);
        QString jclassName = obj ? QtJambiAPI::getObjectClassName(env, obj) : QLatin1String("null");
        jlong native_link = _link->m_nativeLink ? Java::QtJambi::NativeUtility$NativeLink::nativeId(env, _link->m_nativeLink) : 0;
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
            fprintf(stderr, "QtJambiLink(%p %s) ALIVE: { java_object=%p (%s), java_native_link=%p (%s), global_ref=%s, is_qobject=true, pointer=%p, delete_later=%s, qtType=%s, objectName='%s', hasParent=%s, object_thread='%s' }\n",
                    reinterpret_cast<void*>(_link), _link->debugFlagsToString(xb),
                    static_cast<void*>(obj), qPrintable(jclassName),
                    static_cast<void*>(_link->m_nativeLink),
                    _link->m_nativeLink ? qPrintable(QString("%1").arg((native_link==jlong(_link) ? "attached" : (native_link==0 ? "detached" : "dangled")))) : "detached",
                    _link->m_flags.testFlag(Flag::GlobalReference) ? "true" : "false",
                    static_cast<void*>(_link->pointer()), _link->isDeleteLater() ? "true" : "false",
                    !_link->m_qtTypeName ? "<unknown>" : _link->m_qtTypeName,
                    qPrintable(objectName),
                    hasParent ? "true" : "false",
                    objectThread ? qPrintable(objectThread->objectName()) : "unknown");
        }else{
            fprintf(stderr, "QtJambiLink(%p %s) ALIVE: { java_object=%p (%s), java_native_link=%p (%s), global_ref=%s, is_qobject=false, pointer=%p, delete_later=%s, qtType=%s }\n",
                    reinterpret_cast<void*>(_link), _link->debugFlagsToString(xb),
                    static_cast<void*>(obj), qPrintable(jclassName),
                    static_cast<void*>(_link->m_nativeLink),
                    _link->m_nativeLink ? qPrintable(QString("%1").arg((native_link==jlong(_link) ? "attached" : (native_link==0 ? "detached" : "dangled")))) : "detached",
                    _link->m_flags.testFlag(Flag::GlobalReference) ? "true" : "false",
                    _link->pointer(), _link->isDeleteLater() ? "true" : "false",
                    !_link->m_qtTypeName ? "<unknown>" : _link->m_qtTypeName);
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

typedef QHash<const void *, QWeakPointer<QtJambiLink> > LinkHash;
typedef QMultiHash<const void *, QWeakPointer<QtJambiLink> > MultiLinkHash;
Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gLinkAccessLock, (QReadWriteLock::Recursive))
Q_GLOBAL_STATIC(MultiLinkHash, gUserObjectCache)
Q_GLOBAL_STATIC(LinkHash, gQObjectCache)

QtJambiLinkUserData::QtJambiLinkUserData(const QWeakPointer<QtJambiLink> & link)
  :
    m_link(link)
 { }

QtJambiMetaObjectLinkUserData::QtJambiMetaObjectLinkUserData(const QWeakPointer<QtJambiLink> & link, const QMetaObject* metaObject)
  :
    QtJambiLinkUserData(link),
    m_metaObject(metaObject)
 { }

QReadWriteLock* QtJambiLinkUserData::lock()
{
    return gLinkAccessLock;
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
                reinterpret_cast<SmartPointerToQObjectLink*>(link.data())->setAsQObjectDeleted();
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
    if(Java::QtJambi::Private::NativeUtility$Object::isInstanceOf(env, java)){
        nativeLink = Java::QtJambi::Private::NativeUtility$Object::nativeLink(env, java);
    }else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java)){
        nativeLink = Java::QtJambi::Private::NativeUtility::findInterfaceLink(env, java, true, false);
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
            qtJambiLink = new PointerToPendingQObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces, superTypeForCustomMetaObject, object, created_by_java, false, is_shell, ocurredException);
        }else{
            qtJambiLink = new PointerToPendingQObjectLink(env, nativeLink, javaObject, superTypeForCustomMetaObject, object, created_by_java, false, is_shell, ocurredException);
        }
    }else{
        const QMetaObject* metaObject = object->metaObject();
        QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(metaObject);
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            if(extraSignals.isEmpty())
                qtJambiLink = new PointerToQObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces, metaObject, object, created_by_java, false, is_shell, ocurredException);
            else
                qtJambiLink = new PointerToQObjectInterfaceWithExtraSignalsLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces, metaObject, object, created_by_java, false, is_shell, ocurredException);
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
                                                                           const QMap<size_t,uint>& interfaceOffsets,
                                                                           const QSet<size_t>& interfaceTypes,
                                                                           const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(object);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink;
    if(superTypeForCustomMetaObject){
        qtJambiLink = new PointerToPendingQObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets, interfaceTypes, inheritedInterfaces, superTypeForCustomMetaObject, object, created_by_java, false, is_shell, ocurredException);
    }else{
        const QMetaObject* metaObject = object->metaObject();
        QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(metaObject);
        if(extraSignals.isEmpty())
            qtJambiLink = new PointerToQObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets, interfaceTypes, inheritedInterfaces, metaObject, object, created_by_java, false, is_shell, ocurredException);
        else
            qtJambiLink = new PointerToQObjectInterfaceWithExtraSignalsLink(env, nativeLink, javaObject, interfaceOffsets, interfaceTypes, inheritedInterfaces, metaObject, object, created_by_java, false, is_shell, ocurredException);
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
                qtJambiLink = new PointerToPendingQObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces, metaObject, object, created_by_java, isDeclarativeCall, is_shell, ocurredException);
            }else{
                qtJambiLink = new PointerToPendingQObjectLink(env, nativeLink, javaObject, metaObject, object, created_by_java, isDeclarativeCall, is_shell, ocurredException);
            }
        }else{
            QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(metaObject);
            if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
                if(extraSignals.isEmpty()){
                    qtJambiLink = new PointerToQObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces, metaObject, object, created_by_java, isDeclarativeCall, is_shell, ocurredException);
                }else{
                    qtJambiLink = new PointerToQObjectInterfaceWithExtraSignalsLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces, metaObject, object, created_by_java, isDeclarativeCall, is_shell, ocurredException);
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject, bool created_by_java, bool is_shell, void* ptr_shared_pointer, SmartPointerDeleter pointerDeleter, SmartPointerGetterFunction pointerGetter,
                                                                                   const QMap<size_t,uint>& offsets,
                                                                                   const QSet<size_t>& interfaces,
                                                                                   const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                                                   const QMetaObject* superTypeForCustomMetaObject)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr_shared_pointer);
    Q_ASSERT(pointerDeleter);
    Q_ASSERT(pointerGetter);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    QObject* object = reinterpret_cast<QObject*>(pointerGetter(ptr_shared_pointer));
    Q_ASSERT(object);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink;
    if(superTypeForCustomMetaObject){
        qtJambiLink = new SmartPointerToQObjectInterfaceWithPendingExtraSignalsLink(env, nativeLink, javaObject, offsets, interfaces, inheritedInterfaces, object, created_by_java, is_shell, ptr_shared_pointer, pointerDeleter, pointerGetter, superTypeForCustomMetaObject, ocurredException);
    }else{
        const QMetaObject* metaObject = object->metaObject();
        QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(metaObject);
        if(extraSignals.isEmpty())
            qtJambiLink = new SmartPointerToPlainQObjectInterfaceLink(env, nativeLink, javaObject, offsets, interfaces, inheritedInterfaces, object, created_by_java, is_shell, ptr_shared_pointer, pointerDeleter, pointerGetter, ocurredException);
        else
            qtJambiLink = new SmartPointerToQObjectInterfaceWithExtraSignalsLink(env, nativeLink, javaObject, offsets, interfaces, inheritedInterfaces, object, created_by_java, is_shell, ptr_shared_pointer, pointerDeleter, pointerGetter, ocurredException);
    }
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToQObject(JNIEnv *env, jobject javaObject, bool created_by_java, bool is_shell, void* ptr_shared_pointer, SmartPointerDeleter pointerDeleter, SmartPointerGetterFunction pointerGetter, const QMetaObject* superTypeForCustomMetaObject)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr_shared_pointer);
    Q_ASSERT(pointerDeleter);
    Q_ASSERT(pointerGetter);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    QObject* object = reinterpret_cast<QObject*>(pointerGetter(ptr_shared_pointer));
    Q_ASSERT(object);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink;
    if(superTypeForCustomMetaObject){
        qtJambiLink = new SmartPointerToQObjectWithPendingExtraSignalsLink(env, nativeLink, javaObject, object, created_by_java, is_shell, ptr_shared_pointer, pointerDeleter, pointerGetter, superTypeForCustomMetaObject, ocurredException);
    }else{
        QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(object->metaObject());
        if(extraSignals.isEmpty())
            qtJambiLink = new SmartPointerToPlainQObjectLink(env, nativeLink, javaObject, object, created_by_java, is_shell, ptr_shared_pointer, pointerDeleter, pointerGetter, ocurredException);
        else
            qtJambiLink = new SmartPointerToQObjectWithExtraSignalsLink(env, nativeLink, javaObject, object, created_by_java, is_shell, ptr_shared_pointer, pointerDeleter, pointerGetter, ocurredException);
    }
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createExtendedLinkForSmartPointerToObject(JNIEnv *env, jobject javaObject,
                                                                                  LINK_NAME_ARG(const char* qt_name)
                                                                                  bool created_by_java, bool is_shell, const QObject* extension, PtrOwnerFunction ownerFunction,
                                                                                  void* ptr_shared_pointer, SmartPointerDeleter pointerDeleter, SmartPointerGetterFunction pointerGetter)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr_shared_pointer);
    Q_ASSERT(pointerDeleter);
    Q_ASSERT(pointerGetter);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new ExtendedSmartPointerToObjectLink(env, nativeLink,
                                               LINK_NAME_ARG(qt_name)
                                               created_by_java, is_shell, extension, ownerFunction, ptr_shared_pointer, pointerDeleter, pointerGetter, ocurredException);
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
                                                                                  void* ptr_shared_pointer, SmartPointerDeleter pointerDeleter, SmartPointerGetterFunction pointerGetter,
                                                                                  const QMap<size_t,uint>& offsets,
                                                                                  const QSet<size_t>& interfaces,
                                                                                  const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr_shared_pointer);
    Q_ASSERT(pointerDeleter);
    Q_ASSERT(pointerGetter);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new SmartPointerToObjectInterfaceLink(env, nativeLink, offsets, interfaces, inheritedInterfaces,
                                                            LINK_NAME_ARG(qt_name)
                                                            created_by_java, is_shell, ownerFunction, ptr_shared_pointer, pointerDeleter, pointerGetter, ocurredException);
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
                                              void* ptr_shared_pointer, SmartPointerDeleter pointerDeleter, SmartPointerGetterFunction pointerGetter)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr_shared_pointer);
    Q_ASSERT(pointerDeleter);
    Q_ASSERT(pointerGetter);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new SmartPointerToObjectLink(env, nativeLink,
                                                 LINK_NAME_ARG(qt_name)
                                                 created_by_java, is_shell, ownerFunction, ptr_shared_pointer, pointerDeleter, pointerGetter, ocurredException);
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToContainer(JNIEnv *env, jobject javaObject,
                                                                                     LINK_NAME_ARG(const char* qt_name)
                                                                                     void* ptr_shared_pointer, SmartPointerDeleter pointerDeleter, SmartPointerGetterFunction pointerGetter, AbstractContainerAccess* containerAccess,
                                                                                     const QMap<size_t,uint>& offsets,
                                                                                     const QSet<size_t>& interfaces,
                                                                                     const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr_shared_pointer);
    Q_ASSERT(pointerDeleter);
    Q_ASSERT(pointerGetter);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new SmartPointerToContainerInterfaceLink(env, nativeLink, offsets, interfaces, inheritedInterfaces,
                                                             LINK_NAME_ARG(qt_name)
                                                             false, false, nullptr, ptr_shared_pointer, pointerDeleter, pointerGetter, containerAccess, ocurredException);
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSmartPointerToContainer(JNIEnv *env, jobject javaObject,
                                              LINK_NAME_ARG(const char* qt_name)
                                              void* ptr_shared_pointer, SmartPointerDeleter pointerDeleter, SmartPointerGetterFunction pointerGetter, AbstractContainerAccess* containerAccess)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr_shared_pointer);
    Q_ASSERT(pointerDeleter);
    Q_ASSERT(pointerGetter);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new SmartPointerToContainerLink(env, nativeLink,
                                                    LINK_NAME_ARG(qt_name)
                                                    false, false, nullptr, ptr_shared_pointer, pointerDeleter, pointerGetter, containerAccess, ocurredException);
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForContainer(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                        bool created_by_java, bool isCopy, AbstractContainerAccess* containerAccess)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(containerAccess);
    Q_ASSERT(metaType.isValid());
    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(javaObject));
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        return createLinkForNativeObject(env, javaObject, ptr, metaType,
                                         created_by_java, isCopy, containerAccess, QtJambiLink::Ownership::None,
                                         interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces);
    }else{
        return createLinkForNativeObject(env, javaObject, ptr, metaType,
                                         created_by_java, isCopy, containerAccess, QtJambiLink::Ownership::None);
    }
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                        bool created_by_java, bool isCopy, AbstractContainerAccess* containerAccess, QtJambiLink::Ownership ownership)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(containerAccess);
    Q_ASSERT(metaType.isValid());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new MetaTypedPointerToContainerLink(env, nativeLink,
                                                            ptr, metaType, created_by_java, false,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNativeObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                       bool created_by_java, bool isCopy, AbstractContainerAccess* containerAccess, QtJambiLink::Ownership ownership,
                                                                       const QMap<size_t,uint>& interfaceOffsets,
                                                                       const QSet<size_t>& interfaceTypes,
                                                                       const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(containerAccess);
    Q_ASSERT(metaType.isValid());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    //const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(javaObject));
    JavaException ocurredException;
    QtJambiLink* qtJambiLink;
    qtJambiLink = new MetaTypedPointerToContainerInterfaceLink(env, nativeLink,
                                                                 interfaceOffsets, interfaceTypes,
                                                                 inheritedInterfaces,
                                                                 ptr, metaType, created_by_java, false,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForContainer(JNIEnv *env, jobject javaObject, void *ptr,
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
                                         interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces);
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
                                                                          const QMap<size_t,uint>& interfaceOffsets,
                                                                          const QSet<size_t>& interfaceTypes,
                                                                          const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces)
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
                                                                     interfaceOffsets, interfaceTypes,
                                                                     inheritedInterfaces,
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
                                                                          const QMap<size_t,uint>& interfaceOffsets,
                                                                          const QSet<size_t>& interfaceTypes,
                                                                          const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces)
{
    if(!ownerFunction){
        return createLinkForNativeObject(env, javaObject, ptr, metaType,
                                         created_by_java, is_shell, ownership,
                                         interfaceOffsets, interfaceTypes, inheritedInterfaces);
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
                                                                              interfaceOffsets, interfaceTypes,
                                                                              inheritedInterfaces,
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
                                                                          const QMap<size_t,uint>& interfaceOffsets,
                                                                          const QSet<size_t>& interfaceTypes,
                                                                          const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(metaType.isValid());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new MetaTypedPointerToObjectInterfaceLink(env, nativeLink, interfaceOffsets, interfaceTypes, inheritedInterfaces,
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
                                                                             const QMap<size_t,uint>& interfaceOffsets,
                                                                             const QSet<size_t>& interfaceTypes,
                                                                             const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces)
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
                                                                                       interfaceOffsets, interfaceTypes,
                                                                                       inheritedInterfaces,
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
                                                                             const QMap<size_t,uint>& interfaceOffsets,
                                                                             const QSet<size_t>& interfaceTypes,
                                                                             const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(metaType.isValid());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new DependentMetaTypedPointerToObjectInterfaceLink(env, nativeLink,
                                                                                  interfaceOffsets, interfaceTypes, inheritedInterfaces,
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
                                                                          const QMap<size_t,uint>& interfaceOffsets,
                                                                          const QSet<size_t>& interfaceTypes,
                                                                          const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces)
{
    if(!ownerFunction){
        return createLinkForNativeObject(env, javaObject, ptr,
                                   LINK_NAME_ARG(qt_name)
                                   created_by_java, is_shell, deleter_function, ownership,
                                   interfaceOffsets,
                                   interfaceTypes,
                                   inheritedInterfaces);
    }
    if(!deleter_function){
        return createLinkForNativeObject(env, javaObject, ptr,
                                   LINK_NAME_ARG(qt_name)
                                   created_by_java, is_shell, ownership,
                                   interfaceOffsets,
                                   interfaceTypes,
                                   inheritedInterfaces);
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
                                                                              interfaceOffsets,
                                                                              interfaceTypes,
                                                                              inheritedInterfaces,
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
                                                                          const QMap<size_t,uint>& interfaceOffsets,
                                                                          const QSet<size_t>& interfaceTypes,
                                                                          const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces)
{
    if(!deleter_function){
        return createLinkForNativeObject(env, javaObject, ptr,
                                         LINK_NAME_ARG(qt_name)
                                         created_by_java, is_shell, ownership,
                                         interfaceOffsets,
                                         interfaceTypes,
                                         inheritedInterfaces
                                    );
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(deleter_function);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new DeletablePointerToObjectInterfaceLink(env, nativeLink,
                                                                interfaceOffsets,
                                                                interfaceTypes,
                                                                inheritedInterfaces,
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
                                                                          const QMap<size_t,uint>& interfaceOffsets,
                                                                          const QSet<size_t>& interfaceTypes,
                                                                          const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    QtJambiLink* qtJambiLink = new PointerToObjectInterfaceLink(env, nativeLink,
                                                       interfaceOffsets,
                                                       interfaceTypes,
                                                       inheritedInterfaces,
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
            qtJambiLink = new OwnedMetaTypedPointerToObjectInterfaceLink(env, nativeLink, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewContainer(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, const QMetaType& metaType, bool created_by_java, bool is_shell, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
{
    if(!ocurredException){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);
        Q_ASSERT(containerAccess);
        Q_ASSERT(metaType.isValid());

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        QtJambiLink* qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            qtJambiLink = new MetaTypedPointerToContainerInterfaceLink(env, nativeLink, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
                                                           ptr, metaType, created_by_java, is_shell, containerAccess, ocurredException);
        }else{
            qtJambiLink = new MetaTypedPointerToContainerLink(env, nativeLink,
                                                  ptr, metaType, created_by_java, is_shell, containerAccess, ocurredException);
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewContainer(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, JavaException& ocurredException)
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
            qtJambiLink = new DeletablePointerToContainerInterfaceLink(env, nativeLink, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewContainer(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
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
            qtJambiLink = new DeletableOwnedPointerToContainerInterfaceLink(env, nativeLink, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
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
            qtJambiLink = new MetaTypedPointerToObjectInterfaceLink(env, nativeLink, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
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
            qtJambiLink = new DeletableOwnedPointerToObjectInterfaceLink(env, nativeLink, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
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
            qtJambiLink = new DeletablePointerToObjectInterfaceLink(env, nativeLink, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
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
            qtJambiLink = new PointerToObjectInterfaceLink(env, nativeLink, interfaceOffsets->offsets,
                                                           interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
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
            qtJambiLink = new DeletablePointerToObjectInterfaceLink(env, nativeLink, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
                                                           LINK_NAME_ARG(qt_name)
                                                           ptr, false, false, deleter_function, ocurredException);
        }else{
            qtJambiLink = new PointerToObjectInterfaceLink(env, nativeLink, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
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
        qtJambiLink = new PointerToObjectInterfaceLink(env, nativeLink, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForOwnedContainer(JNIEnv *env, jobject javaObject, void *ptr,
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
        qtJambiLink = new PointerToContainerInterfaceLink(env, nativeLink, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForOwnedContainer(JNIEnv *env, jobject javaObject, void *ptr,
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
        qtJambiLink = new DeletablePointerToContainerInterfaceLink(env, nativeLink, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
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
    if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java))
        Java::QtJambi::Private::NativeUtility::findAndAssignInterfaceLink(env, java, true, true, jlong(&ptr));
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
        Java::QtJambi::Private::NativeUtility$Object::assignNativeLink(env, java, jlong(&ptr));
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
        if(m_nativeLink)
            Java::QtJambi::NativeUtility$NativeLink::detach(env, m_nativeLink, jlong(reinterpret_cast<void*>(this)), hasDisposedSignal());
    }
}

#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
const char* QtJambiLink::qtTypeName() const{
    return m_qtTypeName;
}
#endif

jobject QtJambiLink::nativeLink(JNIEnv* env) const {
    return m_nativeLink ? env->NewLocalRef(m_nativeLink) : nullptr;
}

void QtJambiLink::setCppOwnership(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::CppOwnership) && env){
        Java::QtJambi::NativeUtility$NativeLink::takeOwnership(env, m_nativeLink);
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
        Java::QtJambi::NativeUtility$NativeLink::releaseOwnership(env, m_nativeLink);
        m_flags.setFlag(Flag::OwnershipMask, false);
        m_flags.setFlag(Flag::JavaOwnership);
        if(!isQObject())
            registerPointer(pointer());
    }
}

void QtJambiLink::setSplitOwnership(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::SplitOwnership) && env){
        Java::QtJambi::NativeUtility$NativeLink::releaseOwnership(env, m_nativeLink);
        m_flags.setFlag(Flag::OwnershipMask, false);
        m_flags.setFlag(Flag::SplitOwnership);
    }
}

void QtJambiLink::reset(JNIEnv *env)
{
    Java::QtJambi::NativeUtility$NativeLink::reset(env, m_nativeLink, jlong(reinterpret_cast<void*>(this)), hasDisposedSignal());
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
    return Java::QtJambi::NativeUtility$NativeLink::get(env, m_nativeLink);
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
    m_nativeLink(env->NewGlobalRef(nativeLink)),
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

    // Set the native__id field of the java object
    // We continue to use the hard local reference to java object in preference to the weak one we just created
    try{
        Java::QtJambi::NativeUtility$NativeLink::initialize(env, m_nativeLink, jlong(reinterpret_cast<void*>(this)));
    } catch (const JavaException& exn) {
        env->DeleteGlobalRef(m_nativeLink);
        m_nativeLink = nullptr;
        ocurredException.addSuppressed(env, exn);
    }
    QTJAMBI_REGISTER_LINK
    QTJAMBI_INCREASE_COUNTER_THIS(linkConstructedCount)
}

QtJambiLink::~QtJambiLink(){
    QTJAMBI_UNREGISTER_LINK
    QTJAMBI_INCREASE_COUNTER_THIS(linkDestroyedCount)
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

void QtJambiLink::init(JNIEnv*){
    m_flags.setFlag(Flag::IsInitialized, true);
}

// This method will always cleanup QtJambiLink's m_java.object.
void QtJambiLink::releaseJavaObject(JNIEnv *env)
{
    if (!m_flags.testFlag(Flag::JavaObjectIsReleased)) {
        if(env){
            Java::QtJambi::NativeUtility$NativeLink::releaseOwnership(env, m_nativeLink);
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
const char *QtJambiLink::debugFlagsToString(char *buf) const {
    char *s = buf;
    if(m_flags.testFlag(Flag::GlobalReference))
        *s++ = ((m_java.object)            ? 'O' : '.');
    else
        *s++ = ((m_java.weak)              ? 'W' : '.');
    *s++ = (char(int(ownership()) + '0'));
    *s++ = ((m_flags.testFlag(Flag::GlobalReference)) ? 'G' : '.');
    *s++ = ((m_flags.testFlag(Flag::HasBeenCleaned))     ? 'C' : '.');
    *s++ = ((createdByJava())                              ? 'J' : '.');
    *s++ = ((isShell())                              ? 'S' : '.');
    *s++ = ((m_flags.testFlag(Flag::IsJavaLinkDetached))        ? 'D' : '.');
    *s = 0;
    return buf;
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
    m_containerAccess->dispose();
}

MetaTypedPointerToContainerLink::MetaTypedPointerToContainerLink(JNIEnv *env, jobject nativeLink,
                    void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell,
                    AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : MetaTypedPointerToObjectLink(env, nativeLink,
                                   ptr, metaType, created_by_java, is_shell, ocurredException), m_containerAccess(containerAccess) {}

MetaTypedPointerToContainerLink::~MetaTypedPointerToContainerLink(){
    m_containerAccess->dispose();
}

void MetaTypedPointerToContainerLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if (m_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "MetaTypedPointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
        invalidateDependentObjects(env);
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
                            pointer = nullptr;
                        }
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

DeletablePointerToContainerLink::DeletablePointerToContainerLink(JNIEnv *env, jobject nativeLink,
                    LINK_NAME_ARG(const char* qt_name)
                    void *ptr, bool created_by_java, bool is_shell,
                    PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : DeletablePointerToObjectLink(env, nativeLink,
                                                           LINK_NAME_ARG(qt_name)
                                                           ptr, created_by_java, is_shell, destructor_function, ocurredException), m_containerAccess(containerAccess) {
    Q_ASSERT(m_containerAccess);
}

DeletablePointerToContainerLink::~DeletablePointerToContainerLink(){
    m_containerAccess->dispose();
}

DeletableOwnedPointerToContainerLink::DeletableOwnedPointerToContainerLink(JNIEnv *env, jobject nativeLink,
                    LINK_NAME_ARG(const char* qt_name)
                    void *ptr, bool created_by_java, bool is_shell,
                    PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : DeletableOwnedPointerToObjectLink(env, nativeLink,
                                                           LINK_NAME_ARG(qt_name)
                                                           ptr, created_by_java, is_shell, destructor_function, ownerFunction, ocurredException), m_containerAccess(containerAccess) {
    Q_ASSERT(m_containerAccess);
}

DeletableOwnedPointerToContainerLink::~DeletableOwnedPointerToContainerLink(){
    m_containerAccess->dispose();
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
    {
        QWriteLocker locker(QtJambiLinkUserData::lock());
        dependentObjects.swap(m_dependentObjects);
        finalizations.swap(m_finalizations);
    }
    if(!dependentObjects.empty() || !finalizations.isEmpty()){
        if(JniEnvironment env{1024}){
            clear(env, dependentObjects, finalizations);
        }
    }
}

void DependencyManagerUserData::clear(JNIEnv* env){
    QSet<QWeakPointer<QtJambiLink>> dependentObjects;
    QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>> finalizations;
    {
        QWriteLocker locker(QtJambiLinkUserData::lock());
        dependentObjects.swap(m_dependentObjects);
        finalizations.swap(m_finalizations);
    }
    clear(env, dependentObjects, finalizations);
}

void DependencyManagerUserData::clear(JNIEnv* env, QSet<QWeakPointer<QtJambiLink>>& dependentObjects, QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>>& finalizations){
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
    return !m_dependentObjects.empty();
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
typedef QMultiHash<QSharedPointer<QtJambiLink>, QWeakPointer<QtJambiLink>> DependencyHash;
Q_GLOBAL_STATIC(DependencyHash, gDependencies)
typedef QHash<jint,void*> FinalizationDataHash;
typedef QHash<QSharedPointer<QtJambiLink>, QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>>> FinalizationHash;
Q_GLOBAL_STATIC(FinalizationHash, gFinalizations)
Q_GLOBAL_STATIC(FinalizationDataHash, gFinalizationData)

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

void QtJambiLink::invalidateDependentObjects(JNIEnv *env)
{
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "QtJambiLink::invalidateDependentObjects(JNIEnv *)")
    if(isQObject()){
#ifdef AVAILABLE_IN_DELETION
        if(!isInDestructor())
#endif
        if(QObject* obj = qobject()){
            if(DependencyManagerUserData* dm = DependencyManagerUserData::instance(obj, false)){
                dm->clear(env);
            }
        }
    }else{
        if(m_flags.testFlag(Flag::HasDependencies)){
            QList<QWeakPointer<QtJambiLink>> dependentObjects;
            QHash<void*,QPair<FinalizationExecutor,FinalizationDeleter>> finalizations;
            {
                QWriteLocker locker(gDependencyLock());
                Q_UNUSED(locker)
                dependentObjects = gDependencies->values(m_this);
                gDependencies->remove(m_this);
                finalizations = (*gFinalizations)[m_this];
                gFinalizations->remove(m_this);
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

const QMetaType& PointerToObjectLink::metaType() const{
    static QMetaType invalid(QMetaType::UnknownType);
    return invalid;
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
        invalidateDependentObjects(env);
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
void PointerToObjectLink::onDispose(JNIEnv *env)
{
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
        invalidateDependentObjects(env);
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
        invalidateDependentObjects(env);
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
        invalidateDependentObjects(env);
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

void DeletablePointerToContainerLink::init(JNIEnv* env){
    DeletablePointerToObjectLink::init(env);
    if(AbstractReferenceCountingContainer* rc = dynamic_cast<AbstractReferenceCountingContainer*>(m_containerAccess)){
        jobject object = getJavaObjectLocalRef(env);
        rc->updateRC(env, {object, m_pointer});
        env->DeleteLocalRef(object);
    }
}

void SmartPointerToContainerLink::init(JNIEnv* env){
    SmartPointerToObjectLink::init(env);
    if(AbstractReferenceCountingContainer* rc = dynamic_cast<AbstractReferenceCountingContainer*>(m_containerAccess)){
        jobject object = getJavaObjectLocalRef(env);
        rc->updateRC(env, {object, pointer()});
        env->DeleteLocalRef(object);
    }
}

void DeletableOwnedPointerToContainerLink::init(JNIEnv* env){
    DeletableOwnedPointerToObjectLink::init(env);
    if(AbstractReferenceCountingContainer* rc = dynamic_cast<AbstractReferenceCountingContainer*>(m_containerAccess)){
        jobject object = getJavaObjectLocalRef(env);
        rc->updateRC(env, {object, m_pointer});
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

void MetaTypedPointerToContainerLink::init(JNIEnv* env){
    MetaTypedPointerToObjectLink::init(env);
    if(AbstractReferenceCountingContainer* rc = dynamic_cast<AbstractReferenceCountingContainer*>(m_containerAccess)){
        jobject object = getJavaObjectLocalRef(env);
        rc->updateRC(env, {object, m_pointer});
        env->DeleteLocalRef(object);
    }
}

void DeletablePointerToContainerLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if (m_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "DeletablePointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
        invalidateDependentObjects(env);
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
                            vud->addDeleter([_deleter_function,_pointer, isShell](){
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

void DeletablePointerToObjectLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if (m_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "DeletablePointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
        invalidateDependentObjects(env);
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

void DeletableOwnedPointerToContainerLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if (m_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "DeletablePointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
        invalidateDependentObjects(env);
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
                            vud->addDeleter([_deleter_function,_pointer, isShell](){
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
        invalidateDependentObjects(env);

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

// ### END #####################  PointerToQObjectLink  ######################## END ####

// ### BEGIN ####################  SmartPointerLink  ####################### BEGIN ####

SmartPointerLink::SmartPointerLink(JNIEnv *env, jobject nativeLink,
                                       LINK_NAME_ARG(const char *qt_name)
                                       bool created_by_java, bool is_shell, JavaException& ocurredException)
    : QtJambiLink(env, nativeLink,
                      LINK_NAME_ARG(qt_name)
                      created_by_java, false, is_shell, ocurredException){
}

SmartPointerLink::~SmartPointerLink(){}

bool SmartPointerLink::isSmartPointer() const {
    return true;
}

void SmartPointerLink::setCppOwnership(JNIEnv *){}
void SmartPointerLink::setSplitOwnership(JNIEnv *){}

// #### END ######################  SmartPointerLink  ######################## END ####

ExtendedSmartPointerToObjectLink::ExtendedSmartPointerToObjectLink(JNIEnv *env, jobject nativeLink,
                                 LINK_NAME_ARG(const char* qt_name)
                                 bool created_by_java, bool is_shell, const QObject* extension,
                                 PtrOwnerFunction ownerFunction, void* ptr_shared_pointer,
                                 SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException)
    : SmartPointerToObjectLink(env, nativeLink,
                               LINK_NAME_ARG(qt_name)
                               created_by_java, is_shell,
                               ownerFunction, ptr_shared_pointer,
                               shared_pointer_deleter, pointerGetter,
                               ocurredException), ExtendedLink(), m_extension(extension){}

// ### BEGIN #################  SmartPointerToObjectLink  ################### BEGIN ###

SmartPointerToContainerLink::SmartPointerToContainerLink(JNIEnv *env, jobject nativeLink,
                                                             LINK_NAME_ARG(const char* qt_name)
                                                             bool created_by_java, bool is_shell,
                                                             PtrOwnerFunction ownerFunction, void* ptr_shared_pointer,
                                                             SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter,
                                                             AbstractContainerAccess* containerAccess, JavaException& ocurredException) :
    SmartPointerToObjectLink(env, nativeLink,
                               LINK_NAME_ARG(qt_name)
                               created_by_java, is_shell,
                               ownerFunction, ptr_shared_pointer,
                               shared_pointer_deleter, pointerGetter,
                               ocurredException),
    m_containerAccess(containerAccess)
{
}

SmartPointerToContainerLink::~SmartPointerToContainerLink(){
    m_containerAccess->dispose();
}

SmartPointerToObjectLink::SmartPointerToObjectLink(JNIEnv *env, jobject nativeLink,
                                                       LINK_NAME_ARG(const char *qt_name)
                                                       bool created_by_java, bool is_shell,
                                                       PtrOwnerFunction ownerFunction, void* ptr_shared_pointer,
                                                       SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException) :
    SmartPointerLink(env, nativeLink,
                       LINK_NAME_ARG(qt_name)
                       created_by_java, is_shell, ocurredException),
    m_ptr_shared_pointer(ptr_shared_pointer),
    m_shared_pointer_deleter(shared_pointer_deleter),
    m_shared_pointer_getter(pointerGetter),
    m_owner_function(ownerFunction)
{
    if(!ocurredException){
        registerPointer(m_shared_pointer_getter(m_ptr_shared_pointer));
    }else{
        m_flags.setFlag(Flag::IsJavaLinkDetached);
        m_ptr_shared_pointer = nullptr;
    }
}

SmartPointerToObjectLink::~SmartPointerToObjectLink(){
    try{
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "SmartPointerToObjectLink::~SmartPointerToObjectLink()")
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

void SmartPointerToObjectLink::init(JNIEnv* env){
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
void *SmartPointerToObjectLink::pointer() const { return m_ptr_shared_pointer ? m_shared_pointer_getter(m_ptr_shared_pointer) : nullptr; }

QObject *SmartPointerToObjectLink::qobject() const { Q_ASSERT_X(false, "QtJambiLink", "Pointer is not QObject"); return nullptr; }

void* SmartPointerToObjectLink::getSmartPointer() const{
    return m_ptr_shared_pointer;
}

void SmartPointerToObjectLink::invalidate(JNIEnv *env) {
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "SmartPointerToObjectLink::invalidate(JNIEnv *)")
    invalidateDependentObjects(env);
    releaseJavaObject(env);
    detachJavaLink(env);
    dispose();
}

void SmartPointerToObjectLink::deleteNativeObject(JNIEnv *env, bool){
    if(m_ptr_shared_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "SmartPointerToObjectLink::deleteNativeObject(JNIEnv *env, bool)")
        unregisterOffsets();
        unregisterPointer(pointer());
        m_flags.setFlag(Flag::IsPointerRegistered, false);
        invalidateDependentObjects(env);

        void* shared_pointer = m_ptr_shared_pointer;
        m_ptr_shared_pointer = nullptr;
        if(m_owner_function){
            QWriteLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            if(const QObject* obj = m_owner_function(m_shared_pointer_getter(shared_pointer))){
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
                            SmartPointerDeleter _shared_pointer_deleter = m_shared_pointer_deleter;
                            void* _shared_pointer = shared_pointer;
                            bool isShell = this->isShell();
                            vud->addDeleter([_shared_pointer_deleter,_shared_pointer, isShell](){
                                _shared_pointer_deleter(_shared_pointer, isShell);
                            });
                            dispose();
                        }else{
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor (calling shared pointer deleter function)")
                            setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                            QThreadUserData::Result threadDataResult = QThreadUserData::ensureThreadUserDataLocked(ownerThread);
                            if (QAbstractEventDispatcher::instance(ownerThread)) {
                                DestructorEvent* event = new DestructorEvent(vud->pointer(), m_this, shared_pointer, m_shared_pointer_deleter, isShell());
                                if(threadDataResult.initRequired){
                                    locker.unlock();
                                    threadDataResult.threadUserData->initialize(ownerThread);
                                    locker.relock();
                                }
                                QCoreApplication::postEvent(threadDataResult.threadUserData->threadDeleter(), event);
                            }else{
                                bool is_shell = isShell();
                                SmartPointerDeleter shared_pointer_deleter = m_shared_pointer_deleter;
                                threadDataResult.threadUserData->doAtThreadEnd([shared_pointer, shared_pointer_deleter, is_shell](){
                                    shared_pointer_deleter(shared_pointer, is_shell);
                                });
                                if(threadDataResult.initRequired){
                                    locker.unlock();
                                    threadDataResult.threadUserData->initialize(ownerThread);
                                    locker.relock();
                                }
                            }
                        }
                        shared_pointer = nullptr;
                    }else if(!p->wasDeleted){
                        QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor (calling shared pointer deleter function)")
                        setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                        QThreadUserData::Result threadDataResult = QThreadUserData::ensureThreadUserDataLocked(ownerThread);
                        if (QAbstractEventDispatcher::instance(ownerThread)) {
                            DestructorEvent* event = new DestructorEvent(obj, m_this, shared_pointer, m_shared_pointer_deleter, isShell());
                            if(threadDataResult.initRequired){
                                locker.unlock();
                                threadDataResult.threadUserData->initialize(ownerThread);
                                locker.relock();
                            }
                            QCoreApplication::postEvent(threadDataResult.threadUserData->threadDeleter(), event);
                        }else{
                            bool is_shell = isShell();
                            SmartPointerDeleter shared_pointer_deleter = m_shared_pointer_deleter;
                            threadDataResult.threadUserData->doAtThreadEnd([shared_pointer, shared_pointer_deleter, is_shell](){
                                shared_pointer_deleter(shared_pointer, is_shell);
                            });
                            if(threadDataResult.initRequired){
                                locker.unlock();
                                threadDataResult.threadUserData->initialize(ownerThread);
                                locker.relock();
                            }
                        }
                        shared_pointer = nullptr;
                    }
                }
            }
        }

        if(shared_pointer){
            QTJAMBI_DEBUG_PRINT_WITH_ARGS("call shared pointer deleter function")
            QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
            m_shared_pointer_deleter(shared_pointer, isShell());
            dispose();
        }
    }
}
// Entry point for JVM finalization QtJambiObject#finalize()
void SmartPointerToObjectLink::onClean(JNIEnv *env)
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
void SmartPointerToObjectLink::onDispose(JNIEnv *env)
{
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

// #### END ##################  SmartPointerToObjectLink  ##################### END ####

// ### BEGIN #################  SmartPointerToQObjectLink  ################### BEGIN ###

SmartPointerToQObjectLink::SmartPointerToQObjectLink(JNIEnv *env, jobject nativeLink,
                                                     LINK_NAME_ARG(const char *qt_name)
                                                     bool created_by_java, bool is_shell, void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException)
    : SmartPointerLink(env, nativeLink,
                         LINK_NAME_ARG(qt_name)
                         created_by_java, is_shell, ocurredException),
      m_ptr_shared_pointer(ptr_shared_pointer),
      m_isShell(is_shell),
      m_shared_pointer_deleter(shared_pointer_deleter),
      m_shared_pointer_getter(pointerGetter)
{
}

SmartPointerToQObjectLink::~SmartPointerToQObjectLink(){
    try{
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "SmartPointerToQObjectLink::~SmartPointerToQObjectLink()")
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
    if(m_ptr_shared_pointer && m_shared_pointer_deleter)
        m_shared_pointer_deleter(m_ptr_shared_pointer, m_isShell);
}

void *SmartPointerToQObjectLink::pointer() const {
    return qobject();
}

QObject *SmartPointerToQObjectLink::qobject() const {
    return m_ptr_shared_pointer && m_shared_pointer_getter ? reinterpret_cast<QObject*>(m_shared_pointer_getter(m_ptr_shared_pointer)) : nullptr;
}

void* SmartPointerToQObjectLink::getSmartPointer() const{
    return m_ptr_shared_pointer;
}

void SmartPointerToQObjectLink::setAsQObjectDeleted() {
    m_ptr_shared_pointer = nullptr;
}

void SmartPointerToQObjectLink::invalidate(JNIEnv *env) {
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "SmartPointerToQObjectLink::invalidate(JNIEnv *)")
    invalidateDependentObjects(env);
    releaseJavaObject(env);
    dispose();
}

void SmartPointerToQObjectLink::deleteNativeObject(JNIEnv *env, bool){
    QObject* obj = qobject();
    if (obj && !isDeleteLater()){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_this, "SmartPointerToQObjectLink::deleteNativeObject(JNIEnv *env, bool)")
        unregisterOffsets();
        invalidateDependentObjects(env);

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
            SmartPointerDeleter shared_pointer_deleter = m_shared_pointer_deleter;
            void* ptr_shared_pointer = m_ptr_shared_pointer;
            m_shared_pointer_deleter = nullptr;
            m_ptr_shared_pointer = nullptr;
            if(ptr_shared_pointer && shared_pointer_deleter){
                QtJambiExceptionBlocker __qt_exceptionBlocker;
                {
                    QObjectInDeletion d(env, this);
                    shared_pointer_deleter(ptr_shared_pointer, m_isShell);
                }
                __qt_exceptionBlocker.release(env);
            }
        // We're in the main thread and we'll have an event loop
        // running, so its safe to call delete later.
        }else{
            if(qobject_cast<QThread*>(obj)){
                SmartPointerDeleter shared_pointer_deleter = m_shared_pointer_deleter;
                void* ptr_shared_pointer = m_ptr_shared_pointer;
                m_shared_pointer_deleter = nullptr;
                m_ptr_shared_pointer = nullptr;
                if(ptr_shared_pointer && shared_pointer_deleter){
                    QtJambiExceptionBlocker __qt_exceptionBlocker;
                    {
                        QObjectInDeletion d(env, this);
                        shared_pointer_deleter(ptr_shared_pointer, m_isShell);
                    }
                    __qt_exceptionBlocker.release(env);
                }
            }else{
                QThreadUserData* threadData = QThreadUserData::ensureThreadUserData(objectThread);
                QObjectInDeletion::disposeQObject(obj);
                setDeleteLater();
                QTJAMBI_DEBUG_PRINT_WITH_ARGS("attach QObject to thread shutdown procedure.")
                SmartPointerDeleter shared_pointer_deleter = m_shared_pointer_deleter;
                void* ptr_shared_pointer = m_ptr_shared_pointer;
                m_shared_pointer_deleter = nullptr;
                m_ptr_shared_pointer = nullptr;
                if(ptr_shared_pointer && shared_pointer_deleter){
                    if (QAbstractEventDispatcher::instance(objectThread)) {
                        QCoreApplication::postEvent(threadData->threadDeleter(), new DestructorEvent(obj, m_this, ptr_shared_pointer, shared_pointer_deleter, m_isShell));
                    }else{
                        bool is_shell = m_isShell;
                        threadData->doAtThreadEnd([ptr_shared_pointer, shared_pointer_deleter, is_shell](){
                            shared_pointer_deleter(ptr_shared_pointer, is_shell);
                        });
                    }
                }
            }
        }
    }
}

// Entry point for JVM finalization QtJambiObject#finalize()
void SmartPointerToQObjectLink::onClean(JNIEnv *env)
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
void SmartPointerToQObjectLink::onDispose(JNIEnv *env)
{
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

bool SmartPointerToQObjectLink::isQObject() const {
    return true;
}

bool PointerToQObjectLink::isQObject() const {
    return true;
}

bool PointerToObjectLink::isQObject() const {
    return false;
}

bool SmartPointerToObjectLink::isQObject() const {
    return false;
}

jobject PointerToQObjectLink::getExtraSignal(JNIEnv*, const QMetaMethod&) const{
    return nullptr;
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
            return m_extraSignals.value(method.methodIndex()).object();
        }
    }
    return nullptr;
}

PointerToPendingQObjectInterfaceLink::PointerToPendingQObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces, const QMetaObject* metaObject, QObject *ptr, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException)
    : PointerToQObjectInterfaceLink(env, nativeLink, jobj, interfaceOffsets, interfaces, inheritedInterfaces, metaObject, ptr, created_by_java, isDeclarativeCall, is_shell, ocurredException)
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
            return m_extraSignals.value(method.methodIndex()).object();
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
            return m_extraSignals.value(method.methodIndex()).object();
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
            return m_extraSignals.value(method.methodIndex()).object();
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
    strg = strg.arg(QLatin1String(metaType().name()));
    strg = strg.arg(metaType().id());
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

QString SmartPointerToObjectLink::describe() const{
    QString strg = "[deletionPolicy=%1, createdByJava=%2, hasBeenCleaned=%3, objectInvalid=%4, pointerZeroed=%5, ownership=%6, metaType=%7, metaTypeId=%8]";
    if(m_owner_function) strg = strg.arg("InSpecificThread");
    else strg = strg.arg("Normal");
    strg = strg
            .arg(createdByJava() ? "true" : "false",
                    m_flags.testFlag(Flag::HasBeenCleaned) ? "true" : "false",
                    m_flags.testFlag(Flag::IsJavaLinkDetached) ? "true" : "false",
                    isPointerZeroed() ? "true" : "false");
    if(m_flags.testFlag(Flag::JavaOwnership)) strg = strg.arg("JavaOwnership");
    else if(m_flags.testFlag(Flag::CppOwnership)) strg = strg.arg("CppOwnership");
    else if(m_flags.testFlag(Flag::SplitOwnership)) strg = strg.arg("SplitOwnership");
    else strg = strg.arg("InvalidOwnership");
    return strg;
}

jobject SmartPointerToQObjectLink::getExtraSignal(JNIEnv *, const QMetaMethod&) const{
    return nullptr;
}

void SmartPointerToQObjectLink::removeInterface(const std::type_info&){
    setInDestructor();
}

QString SmartPointerToQObjectLink::describe() const{
    QString strg;
    if(qobject()){
        strg += QString("class=%1, objectName=%2, ").arg(QLatin1String(qobject()->metaObject()->className()), qobject()->objectName());
    }
    strg += QString("createdByJava=%1, hasBeenCleaned=%2, objectInvalid=%3, ownership=%4")
            .arg(createdByJava() ? "true" : "false",
                     m_flags.testFlag(Flag::HasBeenCleaned) ? "true" : "false",
                     m_flags.testFlag(Flag::IsJavaLinkDetached) ? "true" : "false");
    if(m_flags.testFlag(Flag::JavaOwnership)) strg = strg.arg("JavaOwnership");
    else if(m_flags.testFlag(Flag::CppOwnership)) strg = strg.arg("CppOwnership");
    else if(m_flags.testFlag(Flag::SplitOwnership)) strg = strg.arg("SplitOwnership");
    else strg = strg.arg("InvalidOwnership");
    return QString("[%1]").arg(strg);
}

SmartPointerToQObjectWithPendingExtraSignalsLink::SmartPointerToQObjectWithPendingExtraSignalsLink(JNIEnv *env, jobject nativeLink, jobject jobj, QObject* object, bool created_by_java, bool is_shell, void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, const QMetaObject* superTypeForCustomMetaObject, JavaException& ocurredException)
    : SmartPointerToQObjectLink(env, nativeLink,
                                LINK_NAME_ARG(superTypeForCustomMetaObject->className())
                                created_by_java, is_shell, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, ocurredException),
      m_extraSignals()
{
    jobject local = env->NewLocalRef(jobj);
    if(!ocurredException){
        QtJambiMetaObject::resolveSignals(env, jobj, superTypeForCustomMetaObject, ocurredException);
    }
    if(!ocurredException){
        bool _isValueOwner = isValueOwner(superTypeForCustomMetaObject);
        QWriteLocker locker(QtJambiLinkUserData::lock());
        Q_UNUSED(locker)
        QtJambiLinkUserData* lud = new QtJambiLinkUserData(getWeakPointer());
        QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, object, lud);
        if(_isValueOwner && !QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, object)){
            QTJAMBI_SET_OBJECTUSERDATA(ValueOwnerUserData, object, new ValueOwnerUserData(object));
        }
    }else{
        m_flags.setFlag(Flag::IsJavaLinkDetached);
    }
    env->DeleteLocalRef(local);
}

SmartPointerToQObjectWithPendingExtraSignalsLink::SmartPointerToQObjectWithPendingExtraSignalsLink(JNIEnv *env, jobject nativeLink,
                                                                                                   LINK_NAME_ARG(const char *qt_name)
                                                                                                   bool created_by_java, bool is_shell, void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException)
    : SmartPointerToQObjectLink(env, nativeLink,
                                LINK_NAME_ARG(qt_name)
                                created_by_java, is_shell, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, ocurredException),
      m_extraSignals()
{
}

jobject SmartPointerToQObjectWithPendingExtraSignalsLink::getExtraSignal(JNIEnv * env, const QMetaMethod& method) const{
    if(QObject* object = qobject()){
        QList<QMetaMethod> _extraSignals = getExtraSignalsOfMetaObject(object->metaObject());
        if(_extraSignals.contains(method)){
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
            return m_extraSignals.value(method.methodIndex()).object();
        }
    }
    return nullptr;
}

SmartPointerToPlainQObjectLink::SmartPointerToPlainQObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj, QObject* object, bool created_by_java, bool is_shell, void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException)
    : SmartPointerToQObjectLink(env, nativeLink,
                                LINK_NAME_ARG(object->metaObject()->className())
                                created_by_java, is_shell, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, ocurredException)
{
    jobject local = env->NewLocalRef(jobj);
    const QMetaObject* metaObject = object->metaObject();
    if(!ocurredException){
        QtJambiMetaObject::resolveSignals(env, jobj, metaObject, ocurredException);
    }
    if(!ocurredException){
        bool _isValueOwner = isValueOwner(metaObject);
        QWriteLocker locker(QtJambiLinkUserData::lock());
        Q_UNUSED(locker)
        QtJambiLinkUserData* lud = new QtJambiMetaObjectLinkUserData(getWeakPointer(), metaObject);
        QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, object, lud);
        if(_isValueOwner && !QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, object)){
            QTJAMBI_SET_OBJECTUSERDATA(ValueOwnerUserData, object, new ValueOwnerUserData(object));
        }
    }else{
        m_flags.setFlag(Flag::IsJavaLinkDetached);
    }
    env->DeleteLocalRef(local);
}

SmartPointerToQObjectWithExtraSignalsLink::SmartPointerToQObjectWithExtraSignalsLink(JNIEnv *env, jobject nativeLink, jobject jobj, QObject* object, bool created_by_java, bool is_shell, void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException)
    : SmartPointerToQObjectWithPendingExtraSignalsLink(env, nativeLink,
                                                       LINK_NAME_ARG(object->metaObject()->className())
                                                       created_by_java, is_shell, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, ocurredException)
{
    jobject local = env->NewLocalRef(jobj);
    const QMetaObject* metaObject = object->metaObject();
    if(!ocurredException){
        QtJambiMetaObject::resolveSignals(env, jobj, metaObject, ocurredException);
    }
    if(!ocurredException){
        bool _isValueOwner = isValueOwner(metaObject);
        QWriteLocker locker(QtJambiLinkUserData::lock());
        Q_UNUSED(locker)
        QtJambiLinkUserData* lud = new QtJambiMetaObjectLinkUserData(getWeakPointer(), metaObject);
        QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, object, lud);
        if(_isValueOwner && !QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, object)){
            QTJAMBI_SET_OBJECTUSERDATA(ValueOwnerUserData, object, new ValueOwnerUserData(object));
        }
    }else{
        m_flags.setFlag(Flag::IsJavaLinkDetached);
    }
    env->DeleteLocalRef(local);
}

// ### END #################  SmartPointerToQObjectLink  ################### END ###

PointerToQObjectInterfaceWithExtraSignalsLink::PointerToQObjectInterfaceWithExtraSignalsLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces, const QMetaObject* metaObject, QObject *ptr, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException)
    : PointerToQObjectInterfaceLink(env, nativeLink, jobj, interfaceOffsets, interfaces, inheritedInterfaces, metaObject, ptr, created_by_java, isDeclarativeCall, is_shell, ocurredException)
{
}

PointerToQObjectInterfaceLink::PointerToQObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces, const QMetaObject* metaObject, QObject *ptr, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException)
    : PointerToQObjectLink(env, nativeLink, jobj, metaObject, ptr, created_by_java, isDeclarativeCall, is_shell, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

PointerToObjectInterfaceLink::PointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                           LINK_NAME_ARG(const char *qt_name)
                                                           void *ptr, bool created_by_java, bool is_shell, JavaException& ocurredException)
    : PointerToObjectLink(env, nativeLink,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, is_shell, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

OwnedMetaTypedPointerToObjectInterfaceLink::OwnedMetaTypedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                           void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
    : OwnedMetaTypedPointerToObjectLink(env, nativeLink,
                          ptr, metaType, created_by_java, is_shell, ownerFunction, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

PointerToContainerInterfaceLink::PointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                           LINK_NAME_ARG(const char *qt_name)
                                                           void *ptr, bool created_by_java, bool is_shell, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : PointerToContainerLink(env, nativeLink,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, is_shell, containerAccess, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

MetaTypedPointerToContainerInterfaceLink::MetaTypedPointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                           void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : MetaTypedPointerToContainerLink(env, nativeLink,
                          ptr, metaType, created_by_java, is_shell, containerAccess, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

DeletablePointerToContainerInterfaceLink::DeletablePointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                           LINK_NAME_ARG(const char *qt_name)
                                                           void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : DeletablePointerToContainerLink(env, nativeLink,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, is_shell, destructor_function, containerAccess, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

MetaTypedPointerToObjectInterfaceLink::MetaTypedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                           void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, JavaException& ocurredException)
    : MetaTypedPointerToObjectLink(env, nativeLink,
                          ptr, metaType, created_by_java, is_shell, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

DeletablePointerToObjectInterfaceLink::DeletablePointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                           LINK_NAME_ARG(const char *qt_name)
                                                           void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, JavaException& ocurredException)
    : DeletablePointerToObjectLink(env, nativeLink,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, is_shell, destructor_function, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

DeletableOwnedPointerToObjectInterfaceLink::DeletableOwnedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                           LINK_NAME_ARG(const char *qt_name)
                                                           void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
    : DeletableOwnedPointerToObjectLink(env, nativeLink,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, is_shell, destructor_function, ownerFunction, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

DeletableOwnedPointerToContainerInterfaceLink::DeletableOwnedPointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                           LINK_NAME_ARG(const char *qt_name)
                                                           void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : DeletableOwnedPointerToContainerLink(env, nativeLink,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, is_shell, destructor_function, ownerFunction, containerAccess, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

SmartPointerToObjectInterfaceLink::SmartPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                                         LINK_NAME_ARG(const char *qt_name)
                                                                         bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException)
    : SmartPointerToObjectLink(env, nativeLink,
                                 LINK_NAME_ARG(qt_name)
                                 created_by_java, is_shell, ownerFunction, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
    for(QMap<size_t, uint>::key_value_iterator i = m_interfaceOffsets.keyValueBegin(); i!=m_interfaceOffsets.keyValueEnd(); ++i){
        registerPointer(reinterpret_cast<void*>( quintptr(SmartPointerToObjectLink::pointer()) + i.base().value() ));
    }
}

SmartPointerToContainerInterfaceLink::SmartPointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                                         LINK_NAME_ARG(const char *qt_name)
                                                                         bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : SmartPointerToContainerLink(env, nativeLink,
                                          LINK_NAME_ARG(qt_name)
                                          created_by_java, is_shell, ownerFunction, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, containerAccess, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
    for(QMap<size_t, uint>::key_value_iterator i = m_interfaceOffsets.keyValueBegin(); i!=m_interfaceOffsets.keyValueEnd(); ++i){
        registerPointer(reinterpret_cast<void*>( quintptr(SmartPointerToObjectLink::pointer()) + i.base().value() ));
    }
}

SmartPointerToPlainQObjectInterfaceLink::SmartPointerToPlainQObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces, QObject* object, bool created_by_java, bool is_shell, void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException)
    : SmartPointerToPlainQObjectLink(env, nativeLink, jobj, object, created_by_java, is_shell, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
    for(QMap<size_t, uint>::key_value_iterator i = m_interfaceOffsets.keyValueBegin(); i!=m_interfaceOffsets.keyValueEnd(); ++i){
        registerPointer(reinterpret_cast<void*>( quintptr(SmartPointerToQObjectLink::pointer()) + i.base().value() ));
    }
}

SmartPointerToQObjectInterfaceWithPendingExtraSignalsLink::SmartPointerToQObjectInterfaceWithPendingExtraSignalsLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces, QObject* object, bool created_by_java, bool is_shell, void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, const QMetaObject* superTypeForCustomMetaObject, JavaException& ocurredException)
    : SmartPointerToQObjectWithPendingExtraSignalsLink(env, nativeLink, jobj, object, created_by_java, is_shell, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, superTypeForCustomMetaObject, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
    for(QMap<size_t, uint>::key_value_iterator i = m_interfaceOffsets.keyValueBegin(); i!=m_interfaceOffsets.keyValueEnd(); ++i){
        registerPointer(reinterpret_cast<void*>( quintptr(SmartPointerToQObjectLink::pointer()) + i.base().value() ));
    }
}

SmartPointerToQObjectInterfaceWithExtraSignalsLink::SmartPointerToQObjectInterfaceWithExtraSignalsLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces, QObject* object, bool created_by_java, bool is_shell, void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException)
    : SmartPointerToQObjectWithExtraSignalsLink(env, nativeLink, jobj, object, created_by_java, is_shell, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
    for(QMap<size_t, uint>::key_value_iterator i = m_interfaceOffsets.keyValueBegin(); i!=m_interfaceOffsets.keyValueEnd(); ++i){
        registerPointer(reinterpret_cast<void*>( quintptr(SmartPointerToQObjectLink::pointer()) + i.base().value() ));
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

DependentMetaTypedPointerToObjectInterfaceLink::DependentMetaTypedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                               void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, JavaException& ocurredException)
    : MetaTypedPointerToObjectInterfaceLink(env, nativeLink, interfaceOffsets, interfaces, inheritedInterfaces,
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

OwnedDependentMetaTypedPointerToObjectInterfaceLink::OwnedDependentMetaTypedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                               void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
    : OwnedMetaTypedPointerToObjectInterfaceLink(env, nativeLink, interfaceOffsets, interfaces, inheritedInterfaces,
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

#define INTERFACE_METHODS(LINKTYPE,LINKSUPERTYPE) \
LINKTYPE::~LINKTYPE()\
{\
    unregisterOffsets();\
}\
void LINKTYPE::unregisterOffsets() {\
    if(!m_flags.testFlag(Flag::IsOffsetsUnregistered)){\
        QVector<void*> pointers;\
        for(QMap<size_t, uint>::key_value_iterator i = m_interfaceOffsets.keyValueBegin(); i!=m_interfaceOffsets.keyValueEnd(); ++i){\
            pointers << reinterpret_cast<void*>( quintptr(LINKSUPERTYPE::pointer()) + i.base().value() );\
        }\
        unregisterPointers(pointers);\
        m_flags.setFlag(Flag::IsOffsetsUnregistered);\
    }\
}\
void LINKTYPE::addInterface(const std::type_info& qtInterfaceType){\
    m_availableInterfaces << unique_id(qtInterfaceType);\
    size_t offset = m_interfaceOffsets.value(unique_id(qtInterfaceType));\
    if(offset>0){\
        registerPointer(reinterpret_cast<void*>( quintptr(LINKSUPERTYPE::pointer()) + offset ));\
    }else{\
        const QSet<const std::type_info*> inheritedInterfaces = m_inheritedInterfaces[unique_id(qtInterfaceType)];\
        for(const std::type_info* ifaces : inheritedInterfaces){\
            Q_ASSERT(ifaces);\
            addInterface(*ifaces);\
        }\
    }\
}\
void LINKTYPE::removeInterface(const std::type_info& qtInterfaceType){\
    if(m_availableInterfaces.contains(unique_id(qtInterfaceType))){\
        m_availableInterfaces.remove(unique_id(qtInterfaceType));\
        if(m_interfaceOffsets.contains(unique_id(qtInterfaceType))){\
            size_t offset = m_interfaceOffsets.value(unique_id(qtInterfaceType));\
            if(offset>0)\
                unregisterPointer(reinterpret_cast<void*>( quintptr(LINKSUPERTYPE::pointer()) + offset ));\
            else\
                setInDestructor();\
        }else{\
            const QSet<const std::type_info*> inheritedInterfaces = m_inheritedInterfaces[unique_id(qtInterfaceType)];\
            for(const std::type_info* ifaces : inheritedInterfaces){\
                Q_ASSERT(ifaces);\
                removeInterface(*ifaces);\
            }\
            setInDestructor();\
        }\
    }\
}\
void* LINKTYPE::typedPointer(const std::type_info& qtInterfaceType) const{\
    if(m_interfaceOffsets.contains(unique_id(qtInterfaceType))){\
        if(!m_availableInterfaces.contains(unique_id(qtInterfaceType)))\
            return nullptr;\
        size_t offset = m_interfaceOffsets.value(unique_id(qtInterfaceType));\
        if(offset>0)\
            return reinterpret_cast<void*>( quintptr(LINKSUPERTYPE::pointer()) + offset );\
    }\
    return LINKSUPERTYPE::pointer();\
}\
bool LINKTYPE::isMultiInheritanceType() const{\
    return true;\
}\
bool LINKTYPE::isInterfaceAvailable(const std::type_info& qtInterfaceType) const{\
    return m_availableInterfaces.contains(unique_id(qtInterfaceType));\
}

INTERFACE_METHODS(DeletableOwnedPointerToObjectInterfaceLink, PointerToObjectLink)
INTERFACE_METHODS(DeletablePointerToObjectInterfaceLink, PointerToObjectLink)
INTERFACE_METHODS(OwnedMetaTypedPointerToObjectInterfaceLink, PointerToObjectLink)
INTERFACE_METHODS(MetaTypedPointerToObjectInterfaceLink, PointerToObjectLink)
INTERFACE_METHODS(PointerToObjectInterfaceLink, PointerToObjectLink)
INTERFACE_METHODS(PointerToQObjectInterfaceLink, PointerToQObjectLink)
INTERFACE_METHODS(SmartPointerToPlainQObjectInterfaceLink, SmartPointerToPlainQObjectLink)
INTERFACE_METHODS(SmartPointerToQObjectInterfaceWithPendingExtraSignalsLink, SmartPointerToQObjectWithPendingExtraSignalsLink)
INTERFACE_METHODS(SmartPointerToQObjectInterfaceWithExtraSignalsLink, SmartPointerToQObjectWithExtraSignalsLink)
INTERFACE_METHODS(SmartPointerToObjectInterfaceLink, SmartPointerToObjectLink)
INTERFACE_METHODS(SmartPointerToContainerInterfaceLink, SmartPointerToContainerLink)
INTERFACE_METHODS(PointerToContainerInterfaceLink, PointerToContainerLink)
INTERFACE_METHODS(MetaTypedPointerToContainerInterfaceLink, MetaTypedPointerToContainerLink)
INTERFACE_METHODS(DeletablePointerToContainerInterfaceLink, DeletablePointerToContainerLink)
INTERFACE_METHODS(DeletableOwnedPointerToContainerInterfaceLink, DeletableOwnedPointerToContainerLink)
