/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
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

#include "qtjambi_repository_p.h"
#include "qtjambilink_p.h"
#include "qtjambi_interfaces.h"

#include "qtjambi_registry_p.h"
#include "qtjambi_core.h"
#include "qtjambi_cast.h"
#include "qtjambitypemanager_p.h"
#include "qtjambi_thread_p.h"
#include "qtjambidebugtools_p.h"

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

#if defined(QTJAMBI_DEBUG_TOOLS)
#  include "qtjambidebugtools_p.h"
#  include <QStringList>
#endif

// #define DEBUG_REFCOUNTING

#ifdef QT_NO_DEBUG
#define QTJAMBI_DEBUG_TRACE_WITH_THREAD(location)
#define QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(context, string)
#else
#define QTJAMBI_DEBUG_TRACE_WITH_THREAD(location) QTJAMBI_DEBUG_TRACE(location)
#define QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(context, string)\
    QByteArray __qt_qtjambi_debug_method_print_BEGIN(QString("(%1) entering: %2 of %3").arg(context).arg(string).arg(qtTypeName()).toUtf8());\
    QByteArray __qt_qtjambi_debug_method_print_END(QString("(%1) leaving:  %2 of %3").arg(context).arg(string).arg(qtTypeName()).toUtf8());\
    QtJambiDebugMethodPrint __qt_qtjambi_debug_method_print(__qt_qtjambi_debug_method_print_BEGIN, __qt_qtjambi_debug_method_print_END, __FILE__, __LINE__);\
    Q_UNUSED(__qt_qtjambi_debug_method_print)
#endif

JObjectSynchronizer::JObjectSynchronizer(JNIEnv* _env, jobject _object)
    : env(_env),
      object(env ? env->NewLocalRef(_object) : nullptr),
      monitor(object && env->MonitorEnter(object)==0)
{}

JObjectSynchronizer::~JObjectSynchronizer()
{
    if(env){
        if(monitor)
            env->MonitorExit(object);
        env->DeleteLocalRef(object);
    }
}

class PointerToObjectLink;
class PointerToQObjectLink;
class QSharedPointerToObjectLink;
class QSharedPointerToQObjectLink;

static const QSharedPointer<QtJambiLink> defaultPointer = QSharedPointer<QtJambiLink>();
static const QWeakPointer<QtJambiLink> defaultWeakPointer = QWeakPointer<QtJambiLink>();

#ifdef QT_QTJAMBI_PORT
class SlotObject : public QtPrivate::QSlotObjectBase
{
public:
    SlotObject(const QSharedPointer<QtJambiLink>& _this, QMetaMethod signal);
private:
    static void impl(int which, QSlotObjectBase *this_, QObject *r, void **a, bool *ret);
    QMetaMethod m_signal;
    QWeakPointer<QtJambiLink> m_link;
};
#endif // QT_QTJAMBI_PORT

#define THREAD_ID() reinterpret_cast<void*>(QThread::currentThreadId())

JNIEnv *qtjambi_current_environment(bool initializeJavaThread);

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

#include <QtCore/private/qabstractfileengine_p.h>
/* static */
int QtJambiLink::dumpLinks(JNIEnv * env)
{
    QList<QSharedPointer<QtJambiLink>> sharedPointers;
    char xb[24];
    int count = 0;
    {
        QReadLocker lock(gStaticQtJambiLinkListLock());
        for(QtJambiLink* _link : *gQtJambiLinkList) {
            sharedPointers << _link->m_this;
            if(!_link->m_this)
                continue;
            jobject obj = env->NewLocalRef(_link->m_java.object);
            QString jclassName = obj ? qtjambi_object_class_name(env, obj) : QLatin1String("null");
            jlong native_link = _link->m_nativeLink ? Java::Private::QtJambi::QtJambiInternal$NativeLink.native__id(env, _link->m_nativeLink) : 0;
            if(_link->isQObject()){
                if(_link->pointer()==QCoreApplicationPrivate::theMainThread.loadRelaxed())
                    continue;
                QString objectName;
                bool hasParent = false;
                QThread* objectThread = nullptr;
                if(_link->qobject()){
                    objectName = _link->qobject()->objectName();
                    objectThread = _link->qobject()->thread();
                    hasParent = _link->qobject()->parent();
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
    }
    fflush(stderr);
    return count;
}

bool QtJambiLink::hasDeleteLaterEvents()
{
    QReadLocker lock(gStaticQtJambiLinkListLock());
    for(QtJambiLink* _link : *gQtJambiLinkList) {
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

// used for memory checking
Q_GLOBAL_STATIC_WITH_ARGS(QMutex, g_magicLock, (QMutex::Recursive));

#endif

#ifdef DEBUG_REFCOUNTING
typedef QHash<int, int> RefCountingHash;
Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gRefCountStaticLock, (QReadWriteLock::Recursive));
Q_GLOBAL_STATIC(RefCountingHash, gRefCountHash);
#endif

typedef QHash<const void *, QWeakPointer<QtJambiLink> > LinkHash;
Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gUserObjectCacheLock, (QReadWriteLock::Recursive))
Q_GLOBAL_STATIC(LinkHash, gUserObjectCache)
Q_GLOBAL_STATIC(LinkHash, gQObjectCache)
Q_GLOBAL_STATIC_WITH_ARGS(QMutex, gObjectAccessLock, (QMutex::Recursive))

QT_WARNING_DISABLE_DEPRECATED

QtJambiLinkUserData::QtJambiLinkUserData(const QWeakPointer<QtJambiLink> & link, const QMetaObject* metaObject)
  :
    m_link(link),
    m_metaObject(metaObject)
 { }

QMutex* QtJambiLinkUserData::lock()
{
    return gObjectAccessLock;
}

uint QtJambiLinkUserData::id()
{
    static const uint qtjambi_user_data_id = QObject::registerUserData();
    return qtjambi_user_data_id;
}

QtJambiLinkUserData::~QtJambiLinkUserData()
{
    try{
        QSharedPointer<QtJambiLink> link = m_link.toStrongRef();
        m_link.clear();
        if (link && link->isQObject()) {
            QTJAMBI_DEBUG_METHOD_PRINT("native", "QtJambiLinkUserData::~QtJambiLinkUserData()")
            QTJAMBI_INCREASE_COUNTER(userDataDestroyedCount, link)
            if(link->isSharedPointer()){
                reinterpret_cast<QSharedPointerToQObjectLink*>(link.data())->setAsQObjectDeleted();
            }else{
                reinterpret_cast<PointerToQObjectLink*>(link.data())->setAsQObjectDeleted();
            }
            // only reset the link when not a shell. Shells do this operation during shell destruction.
            // => QtJambiShellImpl::deleteShell()
            if(!link->isShell()){
                link->invalidate(qtjambi_current_environment(!link->noThreadInitializationOnPurge()));
            }
        }else{
            QMutexLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
        }
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

uint ValueOwnerUserData::id()
{
    static const uint user_data_id = []() -> uint {
            QtJambiLinkUserData::id(); // let ValueOwnerUserData be after QtJambiLinkUserData
            return QObject::registerUserData();
    }();
    return user_data_id;
}

ValueOwnerUserData::ValueOwnerUserData(const QObject* object) : QObjectUserData(), m_object(object) {

}

ValueOwnerUserData::~ValueOwnerUserData(){
    while(!m_deleters.isEmpty()){
        try{
            m_deleters.takeFirst()();
        } catch (const std::exception& e) {
            qWarning("%s", e.what());
        } catch (...) {
        }
    }
}

void ValueOwnerUserData::addDeleter(const std::function<void()>& deleter){
    m_deleters << deleter;
}

static volatile int qtjambi_object_cache_mode = 2;
int qtjambi_object_cache_mode_get()
{
    return qtjambi_object_cache_mode;
}

void qtjambi_object_cache_mode_set(int object_cache_mode)
{
    qtjambi_object_cache_mode = object_cache_mode;
}

int qtjambi_object_cache_operation_flush()
{
    {
        QWriteLocker lock(gUserObjectCacheLock());
        Q_UNUSED(lock)
        gUserObjectCache()->clear();
    }
    return 0;
}

int qtjambi_object_count_get(int, int){return 0;}

int qtjambi_object_cache_operation_count()
{
    int count;
    {
        QWriteLocker lock(gUserObjectCacheLock());
        Q_UNUSED(lock)
        count = gUserObjectCache()->count();
    }
    return count;
}

void qtjambi_register_qobject_initialization(void *ptr, const QSharedPointer<QtJambiLink>& link) {
    if(qtjambi_object_cache_mode_get() != 0) {
        QWriteLocker locker(gUserObjectCacheLock());
        Q_UNUSED(locker)
        Q_ASSERT(gQObjectCache());
        gQObjectCache->insert(ptr, link.toWeakRef());
    }
}

void qtjambi_unregister_qobject_initialization(void *ptr) {
    if(qtjambi_object_cache_mode_get() != 0) {
        QWriteLocker locker(gUserObjectCacheLock());
        Q_UNUSED(locker)
        if(LinkHash* h = gQObjectCache())
            h->remove(ptr);
    }
}

bool is_finished(JNIEnv *env, QThread * thread){
    if(thread->isFinished()){
        return true;
    }
#ifdef QT_QTJAMBI_PORT
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(thread).toStrongRef()){
        jobject threadWrapper = link->getJavaObjectLocalRef(env);
        if(threadWrapper && Java::Private::QtCore::QThread$Wrapper.isInstanceOf(env, threadWrapper)){
            jobject java_thread = Java::Private::QtCore::QThread$Wrapper.thread(env, threadWrapper);
            bool finished = Java::Private::Runtime::Thread.isAlive(env, java_thread);
            if(!finished){
                return true;
            }
        }
    }
#else
    Q_UNUSED(env)
#endif
    return false;
}

jobject getNativeLink(JNIEnv *env, jobject java){
    jobject nativeLink;
    if(java && Java::Private::QtJambi::QtJambiObject.isInstanceOf(env, java)){
        nativeLink = Java::Private::QtJambi::QtJambiObject.nativeLink(env,java);
    }else if(java && Java::Private::QtJambi::QtObjectInterface.isInstanceOf(env, java)){
        nativeLink = Java::Private::QtJambi::QtJambiInternal.findInterfaceLink(env, java, false);
        if(nativeLink==nullptr){
            nativeLink = Java::Private::QtJambi::QtJambiInternal.createNativeLinkInterface(env, java);
        }
    }else{
        nativeLink = nullptr;
    }
    Q_ASSERT(nativeLink);
    return nativeLink;
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForQObject(JNIEnv *env, jobject javaObject, QObject *object, bool created_by_java, bool is_shell)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QtJambiLink::createLinkForQObject(JNIEnv *env, jobject java, QObject *object, bool created_by_java)")
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(object);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(javaObject));
    JavaException ocurredException;
    QtJambiLink* qtJambiLink;
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        qtJambiLink = new PointerToQObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces, object->metaObject(), object, created_by_java, is_shell, ocurredException);
    }else{
        qtJambiLink = new PointerToQObjectLink(env, nativeLink, javaObject, object->metaObject(), object, created_by_java, is_shell, ocurredException);
    }
    if(ocurredException.object()){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    //qtJambiLink->setCppOwnership(env);

    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewQObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, const QMetaObject* metaObject, QObject *object, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, JavaException& ocurredException)
{
    if(!ocurredException.object()){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(object);
        Q_ASSERT(metaObject);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        QtJambiLink* qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            qtJambiLink = new PointerToQObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces, metaObject, object, created_by_java, is_shell, ocurredException);
        }else{
            qtJambiLink = new PointerToQObjectLink(env, nativeLink, javaObject, metaObject, object, created_by_java, is_shell, ocurredException);
        }
        if(ocurredException.object()){
            qtJambiLink->dispose();
        }else{
            return qtJambiLink->getStrongPointer();
        }
    }
    return defaultPointer;
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSharedPointerToQObject(JNIEnv *env, jobject javaObject, bool created_by_java, bool is_shell, void* ptr_shared_pointer, PointerDeleter pointerDeleter, PointerQObjectGetterFunction pointerGetter)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr_shared_pointer);
    Q_ASSERT(pointerDeleter);
    Q_ASSERT(pointerGetter);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(javaObject));
    QtJambiLink* qtJambiLink;
    QObject* object = pointerGetter(ptr_shared_pointer);
    Q_ASSERT(object);
    JavaException ocurredException;
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        qtJambiLink = new QSharedPointerToQObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces, object, created_by_java, is_shell, ptr_shared_pointer, pointerDeleter, pointerGetter, ocurredException);
    }else{
        qtJambiLink = new QSharedPointerToQObjectLink(env, nativeLink, javaObject, object, created_by_java, is_shell, ptr_shared_pointer, pointerDeleter, pointerGetter, ocurredException);
    }
    if(ocurredException.object()){
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSharedPointerToObject(JNIEnv *env, jobject javaObject, int metaType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                              const char* qt_name,
#endif
                                              bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction,
                                              void* ptr_shared_pointer, PointerDeleter pointerDeleter, PointerGetterFunction pointerGetter)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr_shared_pointer);
    Q_ASSERT(pointerDeleter);
    Q_ASSERT(pointerGetter);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(javaObject));
    JavaException ocurredException;
    QtJambiLink* qtJambiLink;
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        qtJambiLink = new QSharedPointerToObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                              qt_name,
#endif
                                                              metaType, created_by_java, is_shell, destructor_function, ownerFunction, ptr_shared_pointer, pointerDeleter, pointerGetter, ocurredException);
    }else{
        qtJambiLink = new QSharedPointerToObjectLink(env, nativeLink, javaObject,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                     qt_name,
#endif
                                                     metaType, created_by_java, is_shell, destructor_function, ownerFunction, ptr_shared_pointer, pointerDeleter, pointerGetter, ocurredException);
    }
    if(ocurredException.object()){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    qtJambiLink->setJavaOwnership(env);
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForObject(JNIEnv *env, jobject javaObject, void *ptr, int metaType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                        const char* qt_name,
#endif
                                                                        bool created_by_java, bool is_shell, PtrDeleterFunction deleter_function, PtrOwnerFunction ownerFunction)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(javaObject));
    JavaException ocurredException;
    QtJambiLink* qtJambiLink;
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        qtJambiLink = new PointerToObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                       qt_name,
#endif
                                                       ptr, metaType, created_by_java, is_shell, deleter_function, ownerFunction, ocurredException);
    }else{
        qtJambiLink = new PointerToObjectLink(env, nativeLink, javaObject,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                              qt_name,
#endif
                                              ptr, metaType, created_by_java, is_shell, deleter_function, ownerFunction, ocurredException);
    }
    if(ocurredException.object()){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    if(created_by_java){
        qtJambiLink->setJavaOwnership(env);
    }
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, int metaType, bool created_by_java, bool is_shell, PtrDeleterFunction deleter_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
{
    if(!ocurredException.object()){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        QtJambiLink* qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            qtJambiLink = new PointerToObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                           getQtName(typeId),
#endif
                                                       ptr, metaType, created_by_java, is_shell, deleter_function, ownerFunction, ocurredException);
        }else{
            qtJambiLink = new PointerToObjectLink(env, nativeLink, javaObject,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                  getQtName(typeId),
#endif
                                                  ptr, metaType, created_by_java, is_shell, deleter_function, ownerFunction, ocurredException);
        }
        if(ocurredException.object()){
            qtJambiLink->dispose();
        }else{
            if(created_by_java){
                qtJambiLink->setJavaOwnership(env);
            }
            return qtJambiLink->getStrongPointer();
        }
    }
    return defaultPointer;
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForInterface(JNIEnv *env, jobject javaObject, void *ptr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                           const char* qt_name,
#endif
                                                                           const QSharedPointer<QtJambiLink>& owner)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(owner);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    JavaException ocurredException;
    PointerToObjectLink* qtJambiLink = new PointerToObjectLink(env, nativeLink, javaObject,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                               qt_name,
#endif
                                                               ptr, QMetaType::UnknownType, false, false, nullptr, nullptr, ocurredException);
    if(ocurredException.object()){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    owner->registerDependentObject(qtJambiLink->getStrongPointer());
    qtJambiLink->registerPointer(ptr);
    qtJambiLink->setSplitOwnership(env);
    return qtJambiLink->getStrongPointer();
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForOwnedObject(JNIEnv *env, jobject javaObject, void *ptr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                             const char* qt_name,
#endif
                                                                             QtJambiNativeID owner, PtrDeleterFunction destructor_function)
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
        qtJambiLink = new PointerToObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                       qt_name,
#endif
                                                       ptr, QMetaType::UnknownType, false, false, destructor_function, nullptr, ocurredException);
    }else{
        qtJambiLink = new PointerToObjectLink(env, nativeLink, javaObject,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                              qt_name,
#endif
                                              ptr, QMetaType::UnknownType, false, false, destructor_function, nullptr, ocurredException);
    }
    if(ocurredException.object()){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
        _owner->registerDependentObject(qtJambiLink->getStrongPointer());
    }
    if(destructor_function){
        qtJambiLink->setJavaOwnership(env);
    }else{
        qtJambiLink->setSplitOwnership(env);
    }
    return qtJambiLink->getStrongPointer();
}

const QWeakPointer<QtJambiLink>& QtJambiLink::findLinkForPointer(const void *ptr)
{
    if (!ptr)
        return defaultWeakPointer;
    QReadLocker locker(gUserObjectCacheLock());
    Q_UNUSED(locker)
    Q_ASSERT(gUserObjectCache());
    if(!gUserObjectCache()->contains(ptr))
        return defaultWeakPointer;
    return (*gUserObjectCache())[ptr];
}

const QWeakPointer<QtJambiLink>& QtJambiLink::findLinkForQObject(QObject *o)
{
    if (!o)
        return defaultWeakPointer;
    {
        QMutexLocker locker(QtJambiLinkUserData::lock());
        Q_UNUSED(locker)
        if(QtJambiLinkUserData *p = static_cast<QtJambiLinkUserData *>(o->userData(QtJambiLinkUserData::id())))
            return p->link();
    }
    {
        QReadLocker locker(gUserObjectCacheLock());
        Q_UNUSED(locker)
        Q_ASSERT(gQObjectCache());
        if(!gQObjectCache()->contains(o))
            return defaultWeakPointer;
        return (*gQObjectCache())[o];
    }
}

const QSharedPointer<QtJambiLink>& QtJambiLink::fromNativeId(QtJambiNativeID native_id){
    if(!!native_id){
        return reinterpret_cast<QtJambiLink*>(native_id)->getStrongPointer();
    }else{
        return defaultPointer;
    }
}

const QSharedPointer<QtJambiLink>& QtJambiLink::findLinkForJavaObject(JNIEnv *env, jobject java)
{
    if (!java)
        return defaultPointer;
    jobject nativeLink(nullptr);
    if(Java::Private::QtJambi::QtJambiObject.isInstanceOf(env,java)){
        nativeLink = Java::Private::QtJambi::QtJambiObject.nativeLink(env, java);
    }else if(Java::Private::QtJambi::QtObjectInterface.isInstanceOf(env,java)){
        nativeLink = Java::Private::QtJambi::QtJambiInternal.findInterfaceLink(env, java, true);
    }
    if(nativeLink){
        JObjectSynchronizer sync(env, nativeLink);
        Q_UNUSED(sync)
        QtJambiNativeID nativeId = QtJambiNativeID(Java::Private::QtJambi::QtJambiInternal$NativeLink.native__id(env, nativeLink));
        const QSharedPointer<QtJambiLink>& qtJambiLink = fromNativeId(nativeId);
#if defined(QTJAMBI_DEBUG_TOOLS)
        Q_ASSERT(!qtJambiLink || QtJambiLink::checkLink(qtJambiLink.data()));  // check the C++ pointer is valid
#endif
        return qtJambiLink;
    }
    return defaultPointer;
}

#if defined(QTJAMBI_DEBUG_TOOLS)
void QtJambiLink::validateMagic_unlocked(const char *prefix)
{
    if(m_magic != QTJAMBI_LINK_MAGIC) {
        char xb[24];
        fprintf(stderr, "QtJambiLink(%p %s) INVALID MAGIC %s found:0x%lx expected:0x%lx\n", reinterpret_cast<void*>(this), debugFlagsToString(xb), (!prefix ? "" : prefix), m_magic, QTJAMBI_LINK_MAGIC);
        fflush(stderr);
    }
}

void QtJambiLink::validateMagic()
{
    QMutexLocker locker(g_magicLock());
    Q_UNUSED(locker)
    validateMagic_unlocked(nullptr);
}

const char * QtJambiLink::acquire_magic_source_id_to_string(AquireSources source_id)
{
    const char *kp = nullptr;
    switch(source_id) {
    case AquireSources::LinkDestructor:
        kp = "~QtJambiLink";
        break;

    case AquireSources::OnClean:
        kp = "QtJambiLink::onClean";
        break;

    case AquireSources::OnDispose:
        kp = "QtJambiLink::onDispose";
        break;

    case AquireSources::UserdataDestructor:
        kp = "~QtJambiLinkUserData";
        break;
    default:
        break;
    }
    return kp;
}

int QtJambiLink::acquireMagic_unlocked(AquireSources source_id)
{
    int i = m_atomic_int.fetchAndAddAcquire(1);
    if(i != 0) {   // was 0, now 1
        // TID_SAME (usually means we reentered)
        char tidbuf[256];
        char xb[24];
        snprintf(tidbuf, sizeof(tidbuf), " tid=%p mytid=%p %s", m_pthread_id, THREAD_ID(), ((m_pthread_id == THREAD_ID()) ? "TID_SAME" : "TID_DIFF"));
        fprintf(stderr, "QtJambiLink(%p %s) INVALID ACQUIRE found=%d, expected=%d%s, them=%s, us=%s, qtType=%s\n", static_cast<void*>(this), debugFlagsToString(xb), i, 0, tidbuf,
         acquire_magic_source_id_to_string(m_magic_source_id), acquire_magic_source_id_to_string(source_id),
                !m_qtTypeName ? "<unknown>" : m_qtTypeName);
        fflush(stderr);
    }
    return i;
}

void QtJambiLink::acquireMagic(AquireSources source_id)
{
    QMutexLocker locker(g_magicLock());
    Q_UNUSED(locker)
    validateMagic_unlocked(nullptr);
    if(acquireMagic_unlocked(source_id) == 0) {
        m_pthread_id = THREAD_ID();
        m_magic_source_id = source_id;
    }
}

int QtJambiLink::releaseMagic_unlocked(AquireSources source_id)
{
    int i = m_atomic_int.fetchAndAddRelease(-1);
    if(i != 1) {   // was 1, now 0
        // TID_SAME (usually means we reentered)
        char tidbuf[256];
        char xb[24];
        snprintf(tidbuf, sizeof(tidbuf), " tid=%p mytid=%p %s", m_pthread_id, THREAD_ID(), ((m_pthread_id == THREAD_ID()) ? "TID_SAME" : "TID_DIFF"));
        fprintf(stderr, "QtJambiLink(%p %s) INVALID RELEASE found=%d, expected=%d%s, them=%s, us=%s, qtType=%s\n", static_cast<void*>(this), debugFlagsToString(xb), i, 1, tidbuf,
         acquire_magic_source_id_to_string(m_magic_source_id), acquire_magic_source_id_to_string(source_id),
                !m_qtTypeName ? "<unknown>" : m_qtTypeName);
        fflush(stderr);
    }
    return i;
}

void QtJambiLink::releaseMagic(AquireSources source_id)
{
    QMutexLocker locker(g_magicLock());
    Q_UNUSED(locker)
    validateMagic_unlocked(nullptr);
    releaseMagic_unlocked(source_id);
    if(m_pthread_id == THREAD_ID())
        m_pthread_id = nullptr;
}
#endif

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
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "QtJambiLink::detachJavaLink(JNIEnv *env)")
        QTJAMBI_INCREASE_COUNTER_THIS(objectInvalidatedCount)
        Java::Private::QtJambi::QtJambiInternal$NativeLink.detach(env, m_nativeLink, jlong(reinterpret_cast<void*>(this)));
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

void QtJambiLink::javaObjectInvalidated(JNIEnv *env)
{
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "QtJambiLink::javaObjectInvalidated(JNIEnv *)")
    releaseJavaObject(env);
    detachJavaLink(env);
    dispose();
}

void QtJambiLink::setCppOwnership(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::CppOwnership)){
        if (!m_flags.testFlag(Flag::GlobalReference) && env) {
            JObjectSynchronizer sync(env, m_nativeLink);
            Q_UNUSED(sync)
            jobject ref = env->NewLocalRef(m_java.object);
            jobject globalRef = env->NewGlobalRef(ref);

            if (m_java.weak) {
                env->DeleteWeakGlobalRef(m_java.weak);
                m_java.weak = nullptr;  // PARANOIA
            }

            m_java.object = globalRef;
            m_flags.setFlag(Flag::GlobalReference, true);
            env->DeleteLocalRef(ref);
        }
#if defined(QTJAMBI_DEBUG_TOOLS)
        m_last_ownership = ownership();
#endif
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
    if(!m_flags.testFlag(Flag::JavaOwnership)){
        if (m_flags.testFlag(Flag::GlobalReference) && env) {
            JObjectSynchronizer sync(env, m_nativeLink);
            Q_UNUSED(sync)
            jobject ref = env->NewLocalRef(m_java.object);
            jobject weakGlobalRef = env->NewWeakGlobalRef(ref);

            if (m_java.object) {
                env->DeleteGlobalRef(m_java.object);
                m_java.object = nullptr;  // PARANOIA
            }

            m_java.weak = weakGlobalRef;
            m_flags.setFlag(Flag::GlobalReference, false);
            env->DeleteLocalRef(ref);
        }
#if defined(QTJAMBI_DEBUG_TOOLS)
        m_last_ownership = ownership();
#endif
        m_flags.setFlag(Flag::OwnershipMask, false);
        m_flags.setFlag(Flag::JavaOwnership);
    }
}

void QtJambiLink::setSplitOwnership(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::SplitOwnership)){
        if (m_flags.testFlag(Flag::GlobalReference) && env) {
            JObjectSynchronizer sync(env, m_nativeLink);
            Q_UNUSED(sync)
            jobject ref = env->NewLocalRef(m_java.object);
            jobject weakGlobalRef = env->NewWeakGlobalRef(ref);

            if (m_java.object) {
                env->DeleteGlobalRef(m_java.object);
                m_java.object = nullptr;  // PARANOIA
            }

            m_java.weak = weakGlobalRef;
            m_flags.setFlag(Flag::GlobalReference, false);
            env->DeleteLocalRef(ref);
        }
#if defined(QTJAMBI_DEBUG_TOOLS)
        m_last_ownership = ownership();
#endif
        m_flags.setFlag(Flag::OwnershipMask, false);
        m_flags.setFlag(Flag::SplitOwnership);
    }
}

bool QtJambiLink::createdByJava() const {
    return m_flags.testFlag(Flag::CreatedByJava);
}

bool QtJambiLink::isShell() const {
    return m_flags.testFlag(Flag::IsShell);
}

QtJambiLink::Ownership QtJambiLink::ownership() const {
    return Ownership(int(m_flags & Flag::OwnershipMask));
}

jobject QtJambiLink::getJavaObjectLocalRef(JNIEnv *env) const
{
    JObjectSynchronizer sync(env, m_nativeLink);
    Q_UNUSED(sync)
    return env->NewLocalRef(m_java.object);	// this is null-safe
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

QtJambiLink::QtJambiLink(JNIEnv *env, jobject nativeLink, jobject jobj,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                 const char *qt_name,
#endif
                                 bool created_by_java, bool is_shell, JavaException& ocurredException) :
    m_this(this),
    m_nativeLink(env->NewGlobalRef(nativeLink)),
    m_flags(QtJambiLink::Flag::None) // Default to split, because it's safest
#if defined(QTJAMBI_DEBUG_TOOLS)
    ,m_magic(QTJAMBI_LINK_MAGIC)
    ,m_last_ownership(QtJambiLink::Ownership::None)
    ,m_atomic_int()
    ,m_pthread_id(nullptr)
    ,m_magic_source_id(AquireSources::None)
#endif
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

    m_java.weak = env->NewWeakGlobalRef(jobj);
    // Set the native__id field of the java object
    // We continue to use the hard local reference to java object in preference to the weak one we just created
    try{
        Java::Private::QtJambi::QtJambiInternal$NativeLink.set_native__id(env, m_nativeLink, jlong(reinterpret_cast<void*>(this)));
    } catch (const JavaException& exn) {
        env->DeleteGlobalRef(m_nativeLink);
        m_nativeLink = nullptr;
        env->DeleteWeakGlobalRef(m_java.weak);
        m_java.weak = nullptr;
        ocurredException.addSuppressed(env, exn);
    }
    QTJAMBI_REGISTER_LINK
    QTJAMBI_INCREASE_COUNTER_THIS(linkConstructedCount)
}

QtJambiLink::~QtJambiLink(){
    QTJAMBI_UNREGISTER_LINK
    QTJAMBI_INCREASE_COUNTER_THIS(linkDestroyedCount)
    JNIEnv *env = qtjambi_current_environment(!noThreadInitializationOnPurge());
    if(m_nativeLink && env){
        try{
            Java::Private::QtJambi::QtJambiInternal$NativeLink.reset(env, m_nativeLink);
        } catch (const JavaException& exn) {
            exn.report(env);
        }
        env->DeleteGlobalRef(m_nativeLink);
        m_nativeLink = nullptr;
    }
    if(m_java.object && env){
        if (m_flags.testFlag(Flag::GlobalReference)){
            env->DeleteGlobalRef(m_java.object);
        }else{
            env->DeleteWeakGlobalRef(m_java.weak);
        }
        m_java.object = nullptr;
    }
}

void QtJambiLink::init(JNIEnv*){
    m_flags.setFlag(Flag::IsInitialized, true);
}

// This method will always cleanup QtJambiLink's m_java.object.  It will also fire the disposed() callback
//  and set the native_id=0 via detachJavaLink().
void QtJambiLink::releaseJavaObject(JNIEnv *env)
{
    JObjectSynchronizer sync(env, m_nativeLink);
    Q_UNUSED(sync)
    if(m_java.object && env){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "QtJambiLink::releaseJavaObject(JNIEnv *env)")
        if (m_flags.testFlag(Flag::GlobalReference)) {
            jobject object = m_java.object;
            m_java.object = nullptr;
            if(object)
                env->DeleteGlobalRef(object);
        } else {
            jweak weak = m_java.weak;
            m_java.weak = nullptr;
            if(weak)
                env->DeleteWeakGlobalRef(weak);
        }
    }
}

uint qHash(const char *p);

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

void QtJambiLink::registerPointer(void *ptr) {
    if(qtjambi_object_cache_mode_get() != 0) {
        QWriteLocker locker(gUserObjectCacheLock());
        Q_UNUSED(locker)
        Q_ASSERT(gUserObjectCache());
        gUserObjectCache->insert(ptr, m_this.toWeakRef());
    }
}

void QtJambiLink::unregisterPointer(void *ptr) {
    if(qtjambi_object_cache_mode_get() != 0) {
        QWriteLocker locker(gUserObjectCacheLock());
        Q_UNUSED(locker)
        if(LinkHash* h = gUserObjectCache())
            h->remove(ptr);
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
    *s++ = (char(int(m_last_ownership) + '0'));
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

PointerToObjectLink::PointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                         const char *qt_name,
#endif
                                         void *ptr, int meta_type, bool created_by_java, bool is_shell,
                                         PtrDeleterFunction deleter_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException) :
    QtJambiLink(env, nativeLink, jobj,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                qt_name,
#endif
                created_by_java, is_shell, ocurredException),
    m_pointer(ptr),
    m_meta_type(meta_type),
    m_deleter_function(deleter_function),
    m_owner_function(ownerFunction)
{
    if(!ocurredException.object()){
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
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "PointerToObjectLink::~PointerToObjectLink()")
    try{
        JNIEnv *env = qtjambi_current_environment(!noThreadInitializationOnPurge());
        releaseJavaObject(env);
        detachJavaLink(env);
        deleteNativeObject(env);
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

bool PointerToObjectLink::isSharedPointer() const { return false; }

void QtJambiLink::registerDependentObject(const QSharedPointer<QtJambiLink>& link)
{
    QWeakPointer<QtJambiLink> weakLink(link.toWeakRef());
    if(!link->m_flags.testFlag(Flag::IsDependent) && !m_dependentObjects.contains(weakLink)){
        m_dependentObjects.append(weakLink);
        link->m_flags.setFlag(Flag::IsDependent);
    }
}

void QtJambiLink::unregisterDependentObject(const QSharedPointer<QtJambiLink>& link)
{
    QWeakPointer<QtJambiLink> weakLink(link.toWeakRef());
    m_dependentObjects.removeAll(weakLink);
    link->m_flags.setFlag(Flag::IsDependent, false);
}

void QtJambiLink::invalidateDependentObjects(JNIEnv *env)
{
    if(m_dependentObjects.size()){
        for(const QWeakPointer<QtJambiLink>& weakLink : m_dependentObjects){
            if(QSharedPointer<QtJambiLink> link = weakLink.toStrongRef()){
                link->invalidate(env);
            }
        }
        m_dependentObjects.clear();
    }
}

void *PointerToObjectLink::pointer() const {
    return m_pointer;
}

QObject *PointerToObjectLink::qobject() const { Q_ASSERT_X(false, "QtJambiLink", "Pointer is not QObject"); return nullptr; }

void PointerToObjectLink::invalidate(JNIEnv *env) {
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "QtJambiLink::resetObject(JNIEnv *)")
    invalidateDependentObjects(env);
    releaseJavaObject(env);
    detachJavaLink(env);
    if(m_pointer) {
        unregisterPointer(m_pointer);
        unregisterOffsets();
        m_pointer = nullptr;
    }
    dispose();
}

void PointerToObjectLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if (m_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "PointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
    #ifdef DEBUG_REFCOUNTING
        {
            QWriteLocker locker(gRefCountStaticLock());
            int currentCount = gRefCountHash()->value(m_meta_type, 0) - 1;
            gRefCountHash()->insert(m_meta_type, currentCount);
            qDebug("Deleting QObject [count after: %d]", currentCount);
        }
    #endif
        invalidateDependentObjects(env);
        unregisterPointer(m_pointer);
        unregisterOffsets();
        if(!m_flags.testFlag(Flag::IsDependent) && (m_flags.testFlag(Flag::JavaOwnership) || forced)){
            void* pointer = m_pointer;
            m_pointer = nullptr;
            if(m_owner_function){
                QMutexLocker locker(QtJambiLinkUserData::lock());
                Q_UNUSED(locker)
                if(const QObject* obj = m_owner_function(pointer)){
                    const QObjectPrivate* p = QObjectPrivate::get(obj);
                    Q_ASSERT(p);
                    ValueOwnerUserData* vud = static_cast<ValueOwnerUserData*>(obj->userData(ValueOwnerUserData::id()));
                    QThread* ownerThread = p->threadData->thread;
                    if (ownerThread->isRunning() && ownerThread != QThread::currentThread()) {
                        if(vud){
                            if(p->wasDeleted){
                                if (m_meta_type != QMetaType::UnknownType) {
                                    QTJAMBI_DEBUG_TRACE_WITH_THREAD("call QMetaType::destroy()")
                                    int meta_type = m_meta_type;
                                    void* _pointer = pointer;
                                    vud->addDeleter([meta_type,_pointer](){
                                        QMetaType::destroy(meta_type, _pointer);
                                    });
                                    dispose();
                                } else if (m_deleter_function) {
                                    QTJAMBI_DEBUG_TRACE_WITH_THREAD("call destructor_function")
                                    QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
                                    PtrDeleterFunction _deleter_function = m_deleter_function;
                                    void* _pointer = pointer;
                                    vud->addDeleter([_deleter_function,_pointer](){
                                        _deleter_function(_pointer);
                                    });
                                    dispose();
                                }else{
                                    qWarning("Unable to delete object due to missing deleter or meta type information.");
                                }
                            }else{
                                QTJAMBI_DEBUG_TRACE_WITH_THREAD("use QtJambiDestructor")
                                setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                                QtJambiDestructor* destructor = new QtJambiDestructor(vud->pointer(), m_this, pointer, m_meta_type, ownership(), m_deleter_function);
                                destructor->moveToThread(ownerThread->thread());
                                destructor->deleteLater();
                            }
                        }else{
                            qWarning("Unable to delete object due to missing user data in the owning QObject.");
                        }
                        pointer = nullptr;
                    }
                }
            }
            if(pointer){
                if (m_meta_type != QMetaType::UnknownType) {
                    QTJAMBI_DEBUG_TRACE_WITH_THREAD("call QMetaType::destroy()")
                    QMetaType::destroy(m_meta_type, pointer);
                    dispose();
                } else if (m_deleter_function) {
                    QTJAMBI_DEBUG_TRACE_WITH_THREAD("call destructor_function")
                    QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
                    m_deleter_function(pointer);
                    dispose();
                }else{
                    qWarning("Unable to delete object due to missing deleter or meta type information.");
                }
            }
        }else{
            dispose();
        }
    }
}
// Entry point for JVM finalization QtJambiObject#finalize()
void PointerToObjectLink::onClean(JNIEnv *env)
{
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "QtJambiLink::onClean(JNIEnv *)")
    QTJAMBI_INCREASE_COUNTER_THIS(cleanCallerCount)
    releaseJavaObject(env);
    detachJavaLink(env);
    deleteNativeObject(env);
    m_flags.setFlag(Flag::HasBeenCleaned);
}

// This allows the Java language to attempt to expedite the disposal of the C++ object
//  (when that this possible), however its possible the C++ object will not be destroyed
//  as the Java side does not own it.
// Entry point for QtJambiObject#dispose() method
void PointerToObjectLink::onDispose(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::BlockDeletion)){
        m_flags.setFlag(Flag::BlockDeletion, true);
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "QtJambiLink::onDispose(JNIEnv *)")
        setJavaOwnership(env);
        detachJavaLink(env);
        deleteNativeObject(env, true);
        QTJAMBI_INCREASE_COUNTER_THIS(disposeCalledCount)
        m_flags.setFlag(Flag::BlockDeletion, false);
    }
}

void PointerToObjectLink::init(JNIEnv* env){
    if(!isInitialized()){
        QtJambiLink::init(env);
        if(m_owner_function){
            QMutexLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            if(const QObject* obj = m_owner_function(pointer())){
                if(!obj->userData(ValueOwnerUserData::id()))
                    const_cast<QObject*>(obj)->setUserData(ValueOwnerUserData::id(), new ValueOwnerUserData(obj));
            }
        }
    }
}

// ### END ######################  PointerToObjectLink  ######################## END ####

// ### BEGIN ###################  PointerToQObjectLink  ###################### BEGIN ####

const QHash<int,QtJambiSignalInfo> qtjambi_resolve_signals(JNIEnv *env, jobject java_object, const QMetaObject* metaObject, JavaException& ocurredException);
const QHash<int,QtJambiSignalInfo> qtjambi_resolve_extra_signals(JNIEnv *env, jobject java_object, const QMetaObject* originalMetaObject, JavaException& ocurredException);

PointerToQObjectLink::PointerToQObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMetaObject* metaObject, QObject *object, bool created_by_java, bool is_shell, JavaException& ocurredException) :
    QtJambiLink(env, nativeLink, jobj,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                metaObject->className(),
#endif
                created_by_java, is_shell, ocurredException),
    m_pointer(object),
    m_connections(),
    m_signals(qtjambi_resolve_signals(env, jobj, metaObject, ocurredException)),
    m_extraSignals(qtjambi_resolve_extra_signals(env, jobj, metaObject, ocurredException))
{
    // Fetch the user data id
    if(ocurredException.object()){
        m_flags.setFlag(Flag::IsJavaLinkDetached);
    }
}

void PointerToQObjectLink::init(JNIEnv* env){
    if(!isInitialized()){
        QtJambiLink::init(env);
        QObject* object = qobject();
        Q_ASSERT(object);
        {
            QMutexLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            object->setUserData(QtJambiLinkUserData::id(), new QtJambiLinkUserData(getWeakPointer(), object->metaObject()));
            if(isValueOwner(object->metaObject()) && !object->userData(ValueOwnerUserData::id())){
                object->setUserData(ValueOwnerUserData::id(), new ValueOwnerUserData(object));
            }
        }
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
        m_qtTypeName = object->metaObject()->className();
#endif
#ifdef QT_QTJAMBI_PORT
        reconnect();
#endif
        if(createdByJava()){
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
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "PointerToQObjectLink::~PointerToQObjectLink()")
    JNIEnv *env = qtjambi_current_environment(!noThreadInitializationOnPurge());
    releaseJavaObject(env);
    detachJavaLink(env);
    deleteNativeObject(env);
}

bool PointerToQObjectLink::isSharedPointer() const { return false; }

void PointerToQObjectLink::removeInterface(const std::type_info&){
    setInDestructor();
}

void *PointerToQObjectLink::pointer() const {
    return m_pointer;
}

QObject *PointerToQObjectLink::qobject() const {
    return isInDestructor() || !isInitialized() ? nullptr : m_pointer;
}

void PointerToQObjectLink::setAsQObjectDeleted() {
    QMutexLocker locker(QtJambiLinkUserData::lock());
    Q_UNUSED(locker)
    m_pointer = nullptr;
}

int PointerToQObjectLink::metaType() const{
    return QMetaType::UnknownType;
}

void PointerToQObjectLink::invalidate(JNIEnv *env) {
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "QtJambiLink::resetObject(JNIEnv *)")
    invalidateDependentObjects(env);
    releaseJavaObject(env);
    QMutexLocker locker(QtJambiLinkUserData::lock());
    Q_UNUSED(locker)
    if(m_pointer) {
        unregisterOffsets();
        m_pointer = nullptr;
    }
    dispose();
}

void PointerToQObjectLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if(m_pointer && !isDeleteLater()){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "PointerToQObjectLink::deleteNativeObject(JNIEnv *, bool)")
#ifdef DEBUG_REFCOUNTING
        {
            QWriteLocker locker(gRefCountStaticLock());
            int currentCount = gRefCountHash()->value(m_meta_type, 0) - 1;
            gRefCountHash()->insert(m_meta_type, currentCount);
            qDebug("Deleting '%s' [count after: %d]", QMetaType::typeName(m_meta_type), currentCount);
        }
#endif
        unregisterOffsets();
        invalidateDependentObjects(env);

        if(isInitialized()){
            if (!m_flags.testFlag(Flag::IsDependent) && (m_flags.testFlag(Flag::JavaOwnership) || forced)) {
                QThread *currentThread = QThread::currentThread();
                QThread *objectThread = m_pointer->thread();
                // Explicit dispose from current thread, delete object
                if(!objectThread){
                    if(m_pointer->parent()){
                        qWarning("Skip deletion of QObject '%s' [%s] (thread=none, parent='%s' [%s])",
                                qPrintable(m_pointer->objectName()),
                                m_pointer->metaObject()->className(),
                                qPrintable(m_pointer->parent()->objectName()),
                                m_pointer->parent()->metaObject()->className());
                    }else{
                        delete m_pointer;
                    }
                }else if (currentThread == objectThread || !objectThread->isRunning() || objectThread==m_pointer) {
                    QTJAMBI_DEBUG_TRACE_WITH_THREAD("call delete QObject")
                    if(m_pointer==objectThread || qobject_cast<QThread*>(m_pointer)){
                        QThread* myThread;
                        if(m_pointer==objectThread){
                            myThread = objectThread;
                        }else{
                            myThread = qobject_cast<QThread*>(m_pointer);
                        }
                        QThreadData * threadData = QThreadData::get2(myThread);
                        if(threadData->isAdopted){
                            QTJAMBI_DEBUG_TRACE_WITH_THREAD("Adopted thread expected to be automatically deleted")
                            return;
                        }
                        if(static_cast<SelfDeletingThread*>(myThread)->deleteLaterIfIsInFinish()){
                            setDeleteLater();
                            QTJAMBI_DEBUG_TRACE_WITH_THREAD("call QObject::deleteLater()")
                            return;
                        }
                    }
                    if(currentThread == objectThread){
                        delete m_pointer;
                    }else if(!objectThread->isRunning() && !m_pointer->parent()){
                        delete m_pointer;
                    }else if(objectThread==m_pointer){
                        if(objectThread->isRunning()){
                            qWarning("Trying to delete a running QThread '%s'",
                                    qPrintable(m_pointer->objectName()));
                        }
                        delete m_pointer;
                    }else if(m_pointer->parent()){
                        qWarning("Skip deletion of QObject '%s' [%s] (thread='%s', parent='%s' [%s])",
                                qPrintable(objectThread->objectName()),
                                qPrintable(m_pointer->objectName()),
                                m_pointer->metaObject()->className(),
                                qPrintable(m_pointer->parent()->objectName()),
                                m_pointer->parent()->metaObject()->className());
                    }else{
                        qWarning("Skip deletion of QObject '%s' [%s] (thread='%s', no parent)",
                                qPrintable(objectThread->objectName()),
                                qPrintable(m_pointer->objectName()),
                                m_pointer->metaObject()->className());
                    }

                // We're in the main thread and we'll have an event loop
                // running, so its safe to call delete later.
                } else if (QAbstractEventDispatcher::instance(objectThread)) {
                    setDeleteLater();
                    QTJAMBI_DEBUG_TRACE_WITH_THREAD("call QObject::deleteLater()")
                    m_pointer->deleteLater();

                // If the QObject is in a non-main thread, check if that
                // thread is a QThread, in which case it will run an eventloop
                // and thus, do cleanup, hence deleteLater() is safe;
                // Otherwise issue a warning.
                }else if(QThreadUserData* qthreadData = [objectThread]() -> QThreadUserData* {
                         QThreadUserData* threadData = static_cast<QThreadUserData*>(objectThread->userData(QThreadUserData::id()));
                         if(!threadData){
                            objectThread->setUserData(QThreadUserData::id(), threadData = new QThreadUserData());
                         }
                         return threadData;
                    }()){
                    setDeleteLater();
                    QTJAMBI_DEBUG_TRACE_WITH_THREAD("attach QObject to thread shutdown procedure.")
                    qthreadData->deleteAtThreadEnd(m_pointer);
                    QSharedPointer<QPointer<QObject>> qspointer(new QPointer<QObject>(m_pointer));
                    QSharedPointer<QMetaObject::Connection> connection(new QMetaObject::Connection());
                    *connection = QObject::connect(objectThread, &QThread::finished, [connection,qspointer](){
                        if(qspointer->data()){
                            delete qspointer->data();
                        }
                        QObject::disconnect(QMetaObject::Connection(*connection));
                    });
                } else {
                    // currentThread is not objectThread
                    // && objectThread not finished
                    // && objectThread not pointer
                    // && no QAbstractEventDispatcher
                    if(QThread* tpointer = qobject_cast<QThread*>(m_pointer)){
                        if(!tpointer->parent())
                            delete tpointer;
                    }else{
                        qWarning("QObjects can only be implicitly garbage collected when owned"
                                " by a thread with event dispatcher. Try to delete later. Otherwise, native resource is leaked: '%s' [%s]",
                                qPrintable(m_pointer->objectName()),
                                m_pointer->metaObject()->className());
                        setDeleteLater();
                        m_pointer->deleteLater();
                    }
                }
            }else{
                QMutexLocker locker(QtJambiLinkUserData::lock());
                Q_UNUSED(locker)
                // trust me it is possible that pointer is null
                if(m_pointer){
                    if(QtJambiLinkUserData* data = static_cast<QtJambiLinkUserData*>(m_pointer->userData(QtJambiLinkUserData::id()))){
                        m_pointer->setUserData(QtJambiLinkUserData::id(), nullptr);
                        Q_ASSERT(data);
                        delete data;
                    }
                    m_pointer = nullptr;
                }
                dispose();
            }
        }
    }
}
// Entry point for JVM finalization QtJambiObject#finalize()
void PointerToQObjectLink::onClean(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::BlockDeletion)){
        m_flags.setFlag(Flag::BlockDeletion, true);
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "QtJambiLink::onClean(JNIEnv *)")
        QTJAMBI_INCREASE_COUNTER_THIS(cleanCallerCount)
        releaseJavaObject(env);
        detachJavaLink(env);
        deleteNativeObject(env);
        m_flags.setFlag(Flag::HasBeenCleaned);
        m_flags.setFlag(Flag::BlockDeletion, false);
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
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "QtJambiLink::onDispose(JNIEnv *)")
        setJavaOwnership(env);
        detachJavaLink(env);
        deleteNativeObject(env, true);
        QTJAMBI_INCREASE_COUNTER_THIS(disposeCalledCount)
        m_flags.setFlag(Flag::BlockDeletion, false);
    }
}

// ### END #####################  PointerToQObjectLink  ######################## END ####

// ### BEGIN ####################  QSharedPointerLink  ####################### BEGIN ####

QSharedPointerLink::QSharedPointerLink(JNIEnv *env, jobject nativeLink, jobject jobj,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                       const char *qt_name,
#endif
                                       bool created_by_java, bool is_shell, JavaException& ocurredException)
    : QtJambiLink(env, nativeLink, jobj,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                      qt_name,
#endif
                      created_by_java, is_shell, ocurredException){
}

QSharedPointerLink::~QSharedPointerLink(){}

bool QSharedPointerLink::isSharedPointer() const {
    return true;
}

void QSharedPointerLink::setCppOwnership(JNIEnv *){}
void QSharedPointerLink::setSplitOwnership(JNIEnv *){}

// #### END ######################  QSharedPointerLink  ######################## END ####

// ### BEGIN #################  QSharedPointerToObjectLink  ################### BEGIN ###

QSharedPointerToObjectLink::QSharedPointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                       const char *qt_name,
#endif
                                                       int meta_type, bool created_by_java, bool is_shell,
                                                       PtrDeleterFunction deleter_function,
                                                       PtrOwnerFunction ownerFunction, void* ptr_shared_pointer,
                                                       PointerDeleter shared_pointer_deleter, PointerGetterFunction pointerGetter, JavaException& ocurredException) :
    QSharedPointerLink(env, nativeLink, jobj,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                       qt_name,
#endif
                       created_by_java, is_shell, ocurredException),
    m_meta_type(meta_type),
    m_ptr_shared_pointer(ptr_shared_pointer),
    m_shared_pointer_deleter(shared_pointer_deleter),
    m_shared_pointer_getter(pointerGetter),
    m_deleter_function(deleter_function),
    m_owner_function(ownerFunction)
{
    if(!ocurredException.object()){
        registerPointer(m_shared_pointer_getter(m_ptr_shared_pointer));
    }else{
        m_flags.setFlag(Flag::IsJavaLinkDetached);
        m_ptr_shared_pointer = nullptr;
    }
}

QSharedPointerToObjectLink::~QSharedPointerToObjectLink(){
    try{
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "QSharedPointerToObjectLink::~QSharedPointerToObjectLink()")
        JNIEnv *env = qtjambi_current_environment(!noThreadInitializationOnPurge());
        releaseJavaObject(env);
        detachJavaLink(env);
        deleteNativeObject(env);
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

void QSharedPointerToObjectLink::init(JNIEnv* env){
    if(!isInitialized()){
        QtJambiLink::init(env);
        if(m_owner_function){
            QMutexLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            if(const QObject* obj = m_owner_function(pointer())){
                if(!obj->userData(ValueOwnerUserData::id()))
                    const_cast<QObject*>(obj)->setUserData(ValueOwnerUserData::id(), new ValueOwnerUserData(obj));
            }
        }
    }
}
void *QSharedPointerToObjectLink::pointer() const { return m_ptr_shared_pointer ? m_shared_pointer_getter(m_ptr_shared_pointer) : nullptr; }

QObject *QSharedPointerToObjectLink::qobject() const { Q_ASSERT_X(false, "QtJambiLink", "Pointer is not QObject"); return nullptr; }

void* QSharedPointerToObjectLink::getSharedPointer() const{
    return m_ptr_shared_pointer;
}

int QSharedPointerToObjectLink::metaType() const {
    return m_meta_type;
}

void QSharedPointerToObjectLink::invalidate(JNIEnv *env) {
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "QtJambiLink::resetObject(JNIEnv *)")
    invalidateDependentObjects(env);
    releaseJavaObject(env);
    detachJavaLink(env);
    dispose();
}

void QSharedPointerToObjectLink::deleteNativeObject(JNIEnv *env, bool){
    if(m_ptr_shared_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "QSharedPointerToObjectLink::deleteNativeObject(JNIEnv *env, bool)")
    #ifdef DEBUG_REFCOUNTING
        {
            QWriteLocker locker(gRefCountStaticLock());
            int currentCount = gRefCountHash()->value(m_meta_type, 0) - 1;
            gRefCountHash()->insert(m_meta_type, currentCount);
            qDebug("Deleting '%s' [count after: %d]", QMetaType::typeName(m_meta_type), currentCount);
        }
    #endif
        unregisterOffsets();
        unregisterPointer(pointer());
        invalidateDependentObjects(env);

        void* shared_pointer = m_ptr_shared_pointer;
        m_ptr_shared_pointer = nullptr;
        if(m_owner_function){
            QMutexLocker locker(QtJambiLinkUserData::lock());
            if(const QObject* obj = m_owner_function(m_shared_pointer_getter(shared_pointer))){
                const QObjectPrivate* p = QObjectPrivate::get(obj);
                Q_ASSERT(p);
                ValueOwnerUserData* vud = static_cast<ValueOwnerUserData*>(obj->userData(ValueOwnerUserData::id()));
                QThread* ownerThread = p->threadData->thread;
                if (ownerThread->isRunning() && ownerThread != QThread::currentThread()) {
                    if(vud){
                        if(p->wasDeleted){
                            QTJAMBI_DEBUG_TRACE_WITH_THREAD("call shared pointer deleter function")
                            QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
                            PointerDeleter _shared_pointer_deleter = m_shared_pointer_deleter;
                            void* _shared_pointer = shared_pointer;
                            vud->addDeleter([_shared_pointer_deleter,_shared_pointer](){
                                _shared_pointer_deleter(_shared_pointer);
                            });
                            dispose();
                        }else{
                            QTJAMBI_DEBUG_TRACE_WITH_THREAD("use QtJambiDestructor (calling shared pointer deleter function)")
                            setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                            QtJambiDestructor* destructor = new QtJambiDestructor(
                                        vud->pointer(),
                                        m_this,
                                        shared_pointer,
                                        QMetaType::UnknownType,
                                        QtJambiLink::Ownership::Java,
                                        m_shared_pointer_deleter);
                            destructor->moveToThread(ownerThread->thread());
                            destructor->deleteLater();
                        }
                    }else{
                        qWarning("Unable to delete object due to missing user data in the owning QObject.");
                    }
                    shared_pointer = nullptr;
                }
            }
        }

        if(shared_pointer){
            QTJAMBI_DEBUG_TRACE_WITH_THREAD("call shared pointer deleter function")
            QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
            m_shared_pointer_deleter(shared_pointer);
            dispose();
        }
    }
}
// Entry point for JVM finalization QtJambiObject#finalize()
void QSharedPointerToObjectLink::onClean(JNIEnv *env)
{
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "QtJambiLink::onClean(JNIEnv *)")
    QTJAMBI_INCREASE_COUNTER_THIS(cleanCallerCount)
    releaseJavaObject(env);
    detachJavaLink(env);
    deleteNativeObject(env);
    m_flags.setFlag(Flag::HasBeenCleaned);
}

// This allows the Java language to attempt to expedite the disposal of the C++ object
//  (when that this possible), however its possible the C++ object will not be destroyed
//  as the Java side does not own it.
// Entry point for QtJambiObject#dispose() method
void QSharedPointerToObjectLink::onDispose(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::BlockDeletion)){
        m_flags.setFlag(Flag::BlockDeletion, true);
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "QtJambiLink::onDispose(JNIEnv *)")
        setJavaOwnership(env);
        detachJavaLink(env);
        deleteNativeObject(env, true);
        QTJAMBI_INCREASE_COUNTER_THIS(disposeCalledCount)
        m_flags.setFlag(Flag::BlockDeletion, false);
    }
}

// #### END ##################  QSharedPointerToObjectLink  ##################### END ####

// ### BEGIN #################  QSharedPointerToQObjectLink  ################### BEGIN ###

QSharedPointerToQObjectLink::QSharedPointerToQObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj, QObject* object, bool created_by_java, bool is_shell, void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, PointerQObjectGetterFunction pointerGetter, JavaException& ocurredException)
    : QSharedPointerLink(env, nativeLink, jobj,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                         object->metaObject()->className(),
#endif
                         created_by_java, is_shell, ocurredException),
      m_pointerContainer(nullptr)
{
    jobject local = env->NewLocalRef(jobj);
    if(!ocurredException.object()){
        m_pointerContainer = new PointerContainer(env, local, object->metaObject(), this->getStrongPointer(), ptr_shared_pointer, shared_pointer_deleter, pointerGetter, ocurredException);
    }
    if(!ocurredException.object()){
        {
            QMutexLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            object->setUserData(QtJambiLinkUserData::id(), new QtJambiLinkUserData(getWeakPointer(), object->metaObject()));
            if(isValueOwner(object->metaObject()) && !object->userData(ValueOwnerUserData::id())){
                object->setUserData(ValueOwnerUserData::id(), new ValueOwnerUserData(object));
            }
        }
#ifdef QT_QTJAMBI_PORT
        reconnect();
#endif
    }else{
        m_flags.setFlag(Flag::IsJavaLinkDetached);
    }
    env->DeleteLocalRef(local);
}

QSharedPointerToQObjectLink::~QSharedPointerToQObjectLink(){
    try{
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "QSharedPointerToQObjectLink::~QSharedPointerToQObjectLink()")
        JNIEnv *env = qtjambi_current_environment(!noThreadInitializationOnPurge());
        releaseJavaObject(env);
        detachJavaLink(env);
        deleteNativeObject(env);
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
    if(m_pointerContainer){
        delete m_pointerContainer;
        m_pointerContainer = nullptr;
    }
}

void *QSharedPointerToQObjectLink::pointer() const {
    return qobject();
}

QObject *QSharedPointerToQObjectLink::qobject() const {
    return m_pointerContainer ? m_pointerContainer->qobject() : nullptr;
}

void* QSharedPointerToQObjectLink::getSharedPointer() const{
    return m_pointerContainer ? m_pointerContainer->getSharedPointer() : nullptr;
}

void QSharedPointerToQObjectLink::setAsQObjectDeleted() {
    if(m_pointerContainer) m_pointerContainer->setAsQObjectDeleted();
}

void QSharedPointerToQObjectLink::removePointerContainer() {
    m_pointerContainer = nullptr;
}

int QSharedPointerToQObjectLink::metaType() const{
    return QMetaType::UnknownType;
}

void QSharedPointerToQObjectLink::invalidate(JNIEnv *env) {
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "QtJambiLink::resetObject(JNIEnv *)")
    invalidateDependentObjects(env);
    releaseJavaObject(env);
    dispose();
}

void QSharedPointerToQObjectLink::deleteNativeObject(JNIEnv *env, bool){
    if (m_pointerContainer && m_pointerContainer->qobject() && !isDeleteLater()){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "QSharedPointerToQObjectLink::deleteNativeObject(JNIEnv *env, bool)")
        if(m_pointerContainer){
            Q_ASSERT(m_pointerContainer->qobject());	// must be non-null
            unregisterOffsets();
            invalidateDependentObjects(env);

            QThread *objectThread = m_pointerContainer->thread();

            QThread *currentThread = QThread::currentThread();
            // Explicit dispose from current thread, delete object
            if (!objectThread || currentThread == objectThread || !objectThread->isRunning()) {
    //             printf(" - straight up delete %s [%s]\n",
    //                    qPrintable(qobj->objectName()),
    //                    qobj->metaObject()->className());
                QTJAMBI_DEBUG_TRACE_WITH_THREAD("call delete PointerContainer")
                if(objectThread && m_pointerContainer->qobject()==objectThread){
                    if(QThreadData::get2(objectThread)->isAdopted){
                        QTJAMBI_DEBUG_TRACE_WITH_THREAD("Adopted thread expected to be automatically deleted")
                        return;
                    }
                    if(static_cast<SelfDeletingThread*>(objectThread)->deleteLaterIfIsInFinish()){
                        setDeleteLater();
                        QTJAMBI_DEBUG_TRACE_WITH_THREAD("call QObject::deleteLater()")
                        return;
                    }
                }
                delete m_pointerContainer;
                m_pointerContainer = nullptr;
            // We're in the main thread and we'll have an event loop
            // running, so its safe to call delete later.
            } else if (QAbstractEventDispatcher::instance(objectThread)) {
    //             printf(" - deleteLater in main thread %s [%s]\n",
    //                    qPrintable(qobj->objectName()),
    //                    qobj->metaObject()->className());
                setDeleteLater();
                QTJAMBI_DEBUG_TRACE_WITH_THREAD("call PointerContainer::deleteLater()")
                m_pointerContainer->deleteLater();
            // If the QObject is in a non-main thread, check if that
            // thread is a QThread, in which case it will run an eventloop
            // and thus, do cleanup, hence deleteLater() is safe;
            // Otherwise issue a warning.
            }else if(QThreadUserData* qthreadData = [objectThread]() -> QThreadUserData* {
                     QThreadUserData* threadData = static_cast<QThreadUserData*>(objectThread->userData(QThreadUserData::id()));
                     if(!threadData){
                        objectThread->setUserData(QThreadUserData::id(), threadData = new QThreadUserData());
                     }
                     return threadData;
                }()){
                setDeleteLater();
                QTJAMBI_DEBUG_TRACE_WITH_THREAD("attach QObject to thread shutdown procedure.")
                qthreadData->deleteAtThreadEnd(m_pointerContainer);
                QSharedPointer<QPointer<QObject>> qspointer(new QPointer<QObject>(m_pointerContainer));
                QSharedPointer<QMetaObject::Connection> connection(new QMetaObject::Connection());
                *connection = QObject::connect(objectThread, &QThread::finished, [connection,qspointer](){
                    if(qspointer->data()){
                        delete qspointer->data();
                    }
                    QObject::disconnect(QMetaObject::Connection(*connection));
                });
            } else if(objectThread){
                if(qobject_cast<QThread*>(m_pointerContainer->qobject())){
                    delete m_pointerContainer;
                    m_pointerContainer = nullptr;
                }else{
                    qWarning("QObjects can only be implicitly garbage collected when owned"
                            " by a QThread, native resource ('%s' [%s]) is leaked",
                            qPrintable(m_pointerContainer->qobject()->objectName()),
                            m_pointerContainer->qobject()->metaObject()->className());
        //            PointerContainer* pointerContainer = m_pointerContainer;
        //            QObject::connect(objectThread, &QThread::finished, pointerContainer, [pointerContainer](){delete pointerContainer;});
                    {
                        QMutexLocker locker(QtJambiLinkUserData::lock());
                        Q_UNUSED(locker)
                        // trust me it is possible that pointer is null
                        if(m_pointerContainer){
                            if(QtJambiLinkUserData* data = static_cast<QtJambiLinkUserData*>(m_pointerContainer->qobject()->userData(QtJambiLinkUserData::id()))){
                                m_pointerContainer->qobject()->setUserData(QtJambiLinkUserData::id(), nullptr);
                                delete data;
                            }
                        }
                    }
                    m_pointerContainer = nullptr;
                    dispose();
                }
            }else{
                delete m_pointerContainer;
                m_pointerContainer = nullptr;
            }
        }
    }
}

// Entry point for JVM finalization QtJambiObject#finalize()
void QSharedPointerToQObjectLink::onClean(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::BlockDeletion)){
        m_flags.setFlag(Flag::BlockDeletion, true);
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "QtJambiLink::onClean(JNIEnv *)")
        QTJAMBI_INCREASE_COUNTER_THIS(cleanCallerCount)
        releaseJavaObject(env);
        detachJavaLink(env);
        deleteNativeObject(env);
        m_flags.setFlag(Flag::HasBeenCleaned);
        m_flags.setFlag(Flag::BlockDeletion, false);
    }
}

// This allows the Java language to attempt to expedite the disposal of the C++ object
//  (when that this possible), however its possible the C++ object will not be destroyed
//  as the Java side does not own it.
// Entry point for QtJambiObject#dispose() method
void QSharedPointerToQObjectLink::onDispose(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::BlockDeletion)){
        m_flags.setFlag(Flag::BlockDeletion, true);
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "QtJambiLink::onDispose(JNIEnv *)")
        setJavaOwnership(env);
        detachJavaLink(env);
        deleteNativeObject(env, true);
        QTJAMBI_INCREASE_COUNTER_THIS(disposeCalledCount)
        m_flags.setFlag(Flag::BlockDeletion, false);
    }
}

bool QSharedPointerToQObjectLink::isQObject() const {
    return true;
}

bool PointerToQObjectLink::isQObject() const {
    return true;
}

bool PointerToObjectLink::isQObject() const {
    return false;
}

bool QSharedPointerToObjectLink::isQObject() const {
    return false;
}

#ifdef QT_QTJAMBI_PORT

void QSharedPointerToQObjectLink::emitSignal(JNIEnv * env, SignalMethodIndexProvider signalMethodIndexProvider, jvalue *arguments) const
{
    if(m_pointerContainer)
        m_pointerContainer->emitSignal(env, signalMethodIndexProvider, arguments);
}

void qtjambi_emit_signal(const QtJambiSignalInfo& signal, JNIEnv * env, jvalue *arguments)
{
    // Check if signal has since been collected
    jobject signalObject = env->NewLocalRef(signal.signalObject());
    if (env->IsSameObject(signalObject, nullptr) != JNI_FALSE) {    // it can be non-null (in C++) but null (in Java)
        goto done;     // Checks for non-null and for lost weak-reference
    }
    Q_ASSERT(signalObject);	// redundant?
    Q_ASSERT(Java::Runtime::QtJambiSignals$AbstractSignal.isInstanceOf(env, signalObject));  // check the java object is right type

    // Don't recurse
    if (Java::Runtime::QtJambiSignals$AbstractSignal.inJavaEmission(env, signalObject)) {
        goto done;
    }

    Java::Runtime::QtJambiSignals$AbstractSignal.setInCppEmission(env, signalObject, true);

    {
        QTJAMBI_DEBUG_METHOD_PRINT("java", "QtJambiSignals$AbstractSignal::emit(...)")
        signal.callEmitMethod(env, signalObject, arguments);
    }

    Java::Runtime::QtJambiSignals$AbstractSignal.setInCppEmission(env, signalObject, false);

done:
    if(signalObject)
        env->DeleteLocalRef(signalObject);
}

void PointerToQObjectLink::emitSignal(JNIEnv * env, SignalMethodIndexProvider signalMethodIndexProvider, jvalue *arguments) const
{
    QTJAMBI_DEBUG_METHOD_PRINT("core", "QtJambiLink::emitSignal(JNIEnv * env, SignalMethodIndexProvider signalMethodIndexProvider, jvalue *arguments)")
    if(signalMethodIndexProvider){
        int i = signalMethodIndexProvider();
        if(m_signals.contains(i))
            qtjambi_emit_signal(m_signals[i], env, arguments);
    }
}

void PointerToQObjectLink::emitExtraSignal(JNIEnv * env, int i, jvalue *arguments) const
{
    QTJAMBI_DEBUG_METHOD_PRINT("core", "QtJambiLink::emitExtraSignal(JNIEnv * env, int i, jvalue *arguments)")
    if(m_extraSignals.contains(i))
        qtjambi_emit_signal(m_extraSignals[i], env, arguments);
}
#endif //QT_QTJAMBI_PORT

jobject PointerToQObjectLink::getListOfExtraSignal(JNIEnv * env) const{
    jobject result = qtjambi_arraylist_new(env, 0);
    for(const QtJambiSignalInfo& info : m_extraSignals){
        if(info.signalObject())
            qtjambi_collection_add(env, result, env->NewLocalRef(info.signalObject()));
    }
    return result;
}

jobject PointerContainer::getListOfExtraSignal(JNIEnv * env) const{
    jobject result = qtjambi_arraylist_new(env, 0);
    for(const QtJambiSignalInfo& info : m_extraSignals){
        if(info.signalObject())
            qtjambi_collection_add(env, result, env->NewLocalRef(info.signalObject()));
    }
    return result;
}

#ifdef QT_QTJAMBI_PORT
void PointerContainer::emitSignal(JNIEnv * env, SignalMethodIndexProvider signalMethodIndexProvider, jvalue *arguments) const
{
    QTJAMBI_DEBUG_METHOD_PRINT("core", "QtJambiLink::emitSignal(JNIEnv * env, SignalMethodIndexProvider signalMethodIndexProvider, jvalue *arguments)")
    if(signalMethodIndexProvider){
        int i = signalMethodIndexProvider();
        if(m_signals.contains(i))
            qtjambi_emit_signal(m_signals[i], env, arguments);
    }
}

void PointerContainer::emitExtraSignal(JNIEnv * env, int i, jvalue *arguments) const
{
    QTJAMBI_DEBUG_METHOD_PRINT("core", "QtJambiLink::emitExtraSignal(JNIEnv * env, int i, jvalue *arguments)")
    if(m_extraSignals.contains(i))
        qtjambi_emit_signal(m_extraSignals[i], env, arguments);
}
#endif //QT_QTJAMBI_PORT

PtrDeleterFunction PointerToObjectLink::destructorFunction() const {
    return m_deleter_function;
}

PtrOwnerFunction PointerToObjectLink::ownerFunction() const {
    return m_owner_function;
}

PtrDeleterFunction QSharedPointerToObjectLink::destructorFunction() const {
    return m_deleter_function;
}

PtrOwnerFunction QSharedPointerToObjectLink::ownerFunction() const {
    return m_owner_function;
}

QString PointerToObjectLink::describe() const{
    QString strg = "[deletionPolicy=%1, createdByJava=%2, hasBeenCleaned=%3, isJavaObjectReleased=%4, isJavaLinkDetached=%5, isDeleteLater=%6, ownership=%7, metaType=%8, metaTypeId=%9]";
    if(m_owner_function) strg = strg.arg("InSpecificThread");
    else strg = strg.arg("Normal");
    strg = strg
            .arg(createdByJava() ? "true" : "false")
            .arg(m_flags.testFlag(Flag::HasBeenCleaned) ? "true" : "false")
            .arg(m_java.object==nullptr ? "true" : "false")
            .arg(m_flags.testFlag(Flag::IsJavaLinkDetached) ? "true" : "false")
            .arg(isDeleteLater() ? "true" : "false");
    if(m_flags.testFlag(Flag::JavaOwnership)) strg = strg.arg("Java");
    else if(m_flags.testFlag(Flag::CppOwnership)) strg = strg.arg("Cpp");
    else if(m_flags.testFlag(Flag::SplitOwnership)) strg = strg.arg("Split");
    else strg = strg.arg("InvalidOwnership");
    strg = strg.arg(QMetaType::typeName(m_meta_type));
    strg = strg.arg(m_meta_type);
    return strg;
}

QString PointerToQObjectLink::describe() const{
    QString strg;
    if(qobject()){
        strg += QString("class=%1, objectName=%2, ")
                .arg(qobject()->metaObject()->className())
                .arg(qobject()->objectName());
    }
    strg += QString("createdByJava=%1, hasBeenCleaned=%2, isJavaObjectReleased=%3, isJavaLinkDetached=%4, ownership=%5")
            .arg(createdByJava() ? "true" : "false")
            .arg(m_flags.testFlag(Flag::HasBeenCleaned) ? "true" : "false")
            .arg(m_java.object==nullptr ? "true" : "false")
            .arg(m_flags.testFlag(Flag::IsJavaLinkDetached) ? "true" : "false");
    if(m_flags.testFlag(Flag::JavaOwnership)) strg = strg.arg("Java");
    else if(m_flags.testFlag(Flag::CppOwnership)) strg = strg.arg("Cpp");
    else if(m_flags.testFlag(Flag::SplitOwnership)) strg = strg.arg("Split");
    else strg = strg.arg("InvalidOwnership");
    return QString("[%1]").arg(strg);
}

QString QSharedPointerToObjectLink::describe() const{
    QString strg = "[deletionPolicy=%1, createdByJava=%2, hasBeenCleaned=%3, linkRemoved=%4, objectInvalid=%5, pointerZeroed=%6, ownership=%7, metaType=%8, metaTypeId=%9]";
    if(m_owner_function) strg = strg.arg("InSpecificThread");
    else strg = strg.arg("Normal");
    strg = strg
            .arg(createdByJava() ? "true" : "false")
            .arg(m_flags.testFlag(Flag::HasBeenCleaned) ? "true" : "false")
            .arg(m_java.object==nullptr ? "true" : "false")
            .arg(m_flags.testFlag(Flag::IsJavaLinkDetached) ? "true" : "false")
            .arg(isPointerZeroed() ? "true" : "false");
    if(m_flags.testFlag(Flag::JavaOwnership)) strg = strg.arg("JavaOwnership");
    else if(m_flags.testFlag(Flag::CppOwnership)) strg = strg.arg("CppOwnership");
    else if(m_flags.testFlag(Flag::SplitOwnership)) strg = strg.arg("SplitOwnership");
    else strg = strg.arg("InvalidOwnership");
    strg = strg.arg(QMetaType::typeName(m_meta_type));
    strg = strg.arg(m_meta_type);
    return strg;
}

void QSharedPointerToQObjectLink::removeInterface(const std::type_info&){
    setInDestructor();
}

QString QSharedPointerToQObjectLink::describe() const{
    QString strg;
    if(qobject()){
        strg += QString("class=%1, objectName=%2, ")
                .arg(qobject()->metaObject()->className())
                .arg(qobject()->objectName());
    }
    strg += QString("createdByJava=%1, hasBeenCleaned=%2, linkRemoved=%3, objectInvalid=%4, ownership=%6")
            .arg(createdByJava() ? "true" : "false")
            .arg(m_flags.testFlag(Flag::HasBeenCleaned) ? "true" : "false")
            .arg(m_java.object==nullptr ? "true" : "false")
            .arg(m_flags.testFlag(Flag::IsJavaLinkDetached) ? "true" : "false");
    if(m_flags.testFlag(Flag::JavaOwnership)) strg = strg.arg("JavaOwnership");
    else if(m_flags.testFlag(Flag::CppOwnership)) strg = strg.arg("CppOwnership");
    else if(m_flags.testFlag(Flag::SplitOwnership)) strg = strg.arg("SplitOwnership");
    else strg = strg.arg("InvalidOwnership");
    return QString("[%1]").arg(strg);
}

#ifdef QT_QTJAMBI_PORT
WeakNativeID qtjambi_to_Weak_native_id(const QSharedPointer<QtJambiLink>& link);

void PointerToQObjectLink::reconnect() const{
    if(m_this
            && !m_this->qobjectDeleted()
            && !m_this->isInDestructor()){
        QObject* qobj = qobject();
        const QMetaObject* metaObject = qobj->metaObject();
        SignalConnector connector = nullptr;
        QSet<const QMetaObject*> knownMetaObjects;
        do{
            connector = signalConnector(metaObject);
            if(connector){
                connector(qtjambi_to_Weak_native_id(m_this), qobj, &m_connections);
                knownMetaObjects << metaObject;
            }
            metaObject = metaObject->superClass();
        }while(metaObject);

        if(metaObject != qobj->metaObject()){
            metaObject = qobj->metaObject();
            if (!qtjambi_metaobject_is_dynamic(metaObject)){
                const int methodCount = metaObject->methodCount();
                for(int i=0; i<methodCount; ++i){
                    QMetaMethod method = metaObject->method(i);
                    if(method.isValid() && method.methodType()==QMetaMethod::Signal){
                        if(!knownMetaObjects.contains(method.enclosingMetaObject())){
                            if(!m_connections.value(method.methodIndex())){
                                QtPrivate::QSlotObjectBase *slotObj = new SlotObject(m_this, method);
                                m_connections.insert(method.methodIndex(), QObjectPrivate::connectImpl(qobj, QMetaObjectPrivate::signalIndex(method), qobj, nullptr, slotObj, Qt::AutoConnection, nullptr, metaObject));
                            }
                        }
                    }
                }
            }
        }
    }
}

void QSharedPointerToQObjectLink::reconnect() const{
    m_pointerContainer->reconnect();
}



void PointerContainer::reconnect() const
{
    if(QSharedPointer<QtJambiLink> link = m_link.toStrongRef()){
        if(!link->qobjectDeleted() && !link->isInDestructor()){
            QObject* qobj = qobject();
            const QMetaObject* metaObject = qobj->metaObject();
            SignalConnector connector = nullptr;
            QSet<const QMetaObject*> knownMetaObjects;
            do{
                connector = signalConnector(metaObject);
                if(connector){
                    connector(qtjambi_to_Weak_native_id(link), qobj, &m_connections);
                    knownMetaObjects << metaObject;
                }
                metaObject = metaObject->superClass();
            }while(metaObject);

            if(metaObject != qobj->metaObject()){
                metaObject = qobj->metaObject();
                if (!qtjambi_metaobject_is_dynamic(metaObject)){
                    const int methodCount = metaObject->methodCount();
                    for(int i=0; i<methodCount; ++i){
                        QMetaMethod method = metaObject->method(i);
                        if(method.isValid()
                                && !knownMetaObjects.contains(method.enclosingMetaObject())
                                && method.methodType()==QMetaMethod::Signal){
                            if(!m_connections.value(method.methodIndex())){
                                QtPrivate::QSlotObjectBase *slotObj = new SlotObject(link, method);
                                m_connections.insert(method.methodIndex(), QObjectPrivate::connectImpl(qobj, QMetaObjectPrivate::signalIndex(method), qobj, nullptr, slotObj, Qt::AutoConnection, nullptr, metaObject));
                            }
                        }
                    }
                }
            }
        }
    }
}
#endif

// ### END #################  QSharedPointerToQObjectLink  ################### END ###

PointerContainer::PointerContainer(JNIEnv* env, jobject jobj, const QMetaObject* metaObject, const QSharedPointer<QtJambiLink>& link, void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, PointerQObjectGetterFunction pointerGetter, JavaException& ocurredException):
    QObject(),
    m_ptr_shared_pointer(ptr_shared_pointer),
    m_shared_pointer_deleter(shared_pointer_deleter),
    m_shared_pointer_getter(pointerGetter),
    m_link(link.toWeakRef()),
    m_connections(),
    m_signals(qtjambi_resolve_signals(env, jobj, metaObject, ocurredException)),
    m_extraSignals(qtjambi_resolve_extra_signals(env, jobj, metaObject, ocurredException)),
    m_destroyedConnection()
{
    if(!ocurredException.object()){
        QObject* object = qobject();
        this->setObjectName(QString("PointerContainer-for-%1").arg(object->objectName()));
        if(object){
            if(object->thread())
                this->moveToThread(object->thread());
        }
    }
}

PointerContainer::~PointerContainer(){
    QTJAMBI_DEBUG_METHOD_PRINT("native", "PointerContainer::~PointerContainer()")
    try{
        QSharedPointer<QtJambiLink> link = m_link.toStrongRef();
        QSharedPointerToQObjectLink* _link = reinterpret_cast<QSharedPointerToQObjectLink*>(link.data());
        if(m_ptr_shared_pointer){
            QTJAMBI_DEBUG_TRACE_WITH_THREAD("calling shared pointer deleter function")
            QTJAMBI_INCREASE_COUNTER(pointerContainerDestroyedCount, _link)
            m_shared_pointer_deleter(m_ptr_shared_pointer);
        }
        if(_link){
            _link->removePointerContainer();
        }
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

void PointerContainer::setAsQObjectDeleted() {
    m_ptr_shared_pointer = nullptr;
}

void* PointerContainer::getSharedPointer() const{
    return m_ptr_shared_pointer;
}

PointerToQObjectInterfaceLink::PointerToQObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces, const QMetaObject* metaObject, QObject *ptr, bool created_by_java, bool is_shell, JavaException& ocurredException)
    : PointerToQObjectLink(env, nativeLink, jobj, metaObject, ptr, created_by_java, is_shell, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

PointerToQObjectInterfaceLink::~PointerToQObjectInterfaceLink()
{
    unregisterOffsets();
    m_flags.setFlag(Flag::IsOffsetsUnregistered);
}

PointerToObjectInterfaceLink::PointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                           const char *qt_name,
#endif
                                                           void *ptr, int metaType, bool created_by_java, bool is_shell, PtrDeleterFunction deleter_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
    : PointerToObjectLink(env, nativeLink, jobj,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                          qt_name,
#endif
                          ptr, metaType, created_by_java, is_shell, deleter_function, ownerFunction, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

PointerToObjectInterfaceLink::~PointerToObjectInterfaceLink()
{
    unregisterOffsets();
    m_flags.setFlag(Flag::IsOffsetsUnregistered);
}

QSharedPointerToObjectInterfaceLink::QSharedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                         const char *qt_name,
#endif
                                                                         int metaType, bool created_by_java, bool is_shell, PtrDeleterFunction deleter_function, PtrOwnerFunction ownerFunction, void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, PointerGetterFunction pointerGetter, JavaException& ocurredException)
    : QSharedPointerToObjectLink(env, nativeLink, jobj,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                 qt_name,
#endif
                                 metaType, created_by_java, is_shell, deleter_function, ownerFunction, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
    for(QMap<size_t, uint>::key_value_iterator i = m_interfaceOffsets.keyValueBegin(); i!=m_interfaceOffsets.keyValueEnd(); ++i){
        registerPointer(reinterpret_cast<void*>( quintptr(QSharedPointerToObjectLink::pointer()) + i.base().value() ));
    }
}

QSharedPointerToObjectInterfaceLink::~QSharedPointerToObjectInterfaceLink()
{
    unregisterOffsets();
    m_flags.setFlag(Flag::IsOffsetsUnregistered);
}

QSharedPointerToQObjectInterfaceLink::QSharedPointerToQObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces, QObject* object, bool created_by_java, bool is_shell, void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, PointerQObjectGetterFunction pointerGetter, JavaException& ocurredException)
    : QSharedPointerToQObjectLink(env, nativeLink, jobj, object, created_by_java, is_shell, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
    for(QMap<size_t, uint>::key_value_iterator i = m_interfaceOffsets.keyValueBegin(); i!=m_interfaceOffsets.keyValueEnd(); ++i){
        registerPointer(reinterpret_cast<void*>( quintptr(QSharedPointerToQObjectLink::pointer()) + i.base().value() ));
    }
}

QSharedPointerToQObjectInterfaceLink::~QSharedPointerToQObjectInterfaceLink()
{
    unregisterOffsets();
    m_flags.setFlag(Flag::IsOffsetsUnregistered);
}

void PointerToQObjectInterfaceLink::unregisterOffsets() {
    if(!m_flags.testFlag(Flag::IsOffsetsUnregistered)){
        for(QMap<size_t, uint>::key_value_iterator i = m_interfaceOffsets.keyValueBegin(); i!=m_interfaceOffsets.keyValueEnd(); ++i){
            unregisterPointer(reinterpret_cast<void*>( quintptr(PointerToQObjectLink::pointer()) + i.base().value() ));
        }
    }
}

void PointerToObjectInterfaceLink::unregisterOffsets() {
    if(!m_flags.testFlag(Flag::IsOffsetsUnregistered)){
        for(QMap<size_t, uint>::key_value_iterator i = m_interfaceOffsets.keyValueBegin(); i!=m_interfaceOffsets.keyValueEnd(); ++i){
            unregisterPointer(reinterpret_cast<void*>( quintptr(PointerToObjectLink::pointer()) + i.base().value() ));
        }
    }
}

void QSharedPointerToQObjectInterfaceLink::unregisterOffsets() {
    if(!m_flags.testFlag(Flag::IsOffsetsUnregistered)){
        for(QMap<size_t, uint>::key_value_iterator i = m_interfaceOffsets.keyValueBegin(); i!=m_interfaceOffsets.keyValueEnd(); ++i){
            unregisterPointer(reinterpret_cast<void*>( quintptr(QSharedPointerToQObjectLink::pointer()) + i.base().value() ));
        }
    }
}

void QSharedPointerToObjectInterfaceLink::unregisterOffsets() {
    if(!m_flags.testFlag(Flag::IsOffsetsUnregistered)){
        for(QMap<size_t, uint>::key_value_iterator i = m_interfaceOffsets.keyValueBegin(); i!=m_interfaceOffsets.keyValueEnd(); ++i){
            unregisterPointer(reinterpret_cast<void*>( quintptr(QSharedPointerToObjectLink::pointer()) + i.base().value() ));
        }
    }
}

void PointerToQObjectInterfaceLink::addInterface(const std::type_info& qtInterfaceType){
    m_availableInterfaces << qtInterfaceType.hash_code();
    size_t offset = m_interfaceOffsets.value(qtInterfaceType.hash_code());
    if(offset>0){
        registerPointer(reinterpret_cast<void*>( quintptr(PointerToQObjectLink::pointer()) + offset ));
    }else{
        for(const std::type_info* ifaces : m_inheritedInterfaces[qtInterfaceType.hash_code()]){
            Q_ASSERT(ifaces);
            addInterface(*ifaces);
        }
    }
}

void PointerToQObjectInterfaceLink::removeInterface(const std::type_info& qtInterfaceType){
    if(m_availableInterfaces.contains(qtInterfaceType.hash_code())){
        m_availableInterfaces.remove(qtInterfaceType.hash_code());
        if(m_interfaceOffsets.contains(qtInterfaceType.hash_code())){
            size_t offset = m_interfaceOffsets.value(qtInterfaceType.hash_code());
            if(offset>0){
                unregisterPointer(reinterpret_cast<void*>( quintptr(PointerToQObjectLink::pointer()) + offset ));
            }else{
                setInDestructor();
            }
        }else{
            for(const std::type_info* ifaces : m_inheritedInterfaces[qtInterfaceType.hash_code()]){
                Q_ASSERT(ifaces);
                removeInterface(*ifaces);
            }
            setInDestructor();
        }
    }
}

void* PointerToQObjectInterfaceLink::typedPointer(const std::type_info& qtInterfaceType) const{
    if(m_interfaceOffsets.contains(qtInterfaceType.hash_code())){
        if(!m_availableInterfaces.contains(qtInterfaceType.hash_code()))
            return nullptr;
        size_t offset = m_interfaceOffsets.value(qtInterfaceType.hash_code());
        if(offset>0){
            return reinterpret_cast<void*>( quintptr(PointerToQObjectLink::pointer()) + offset );
        }
    }
    return PointerToQObjectLink::pointer();
}

void PointerToObjectInterfaceLink::addInterface(const std::type_info& qtInterfaceType){
    m_availableInterfaces << qtInterfaceType.hash_code();
    size_t offset = m_interfaceOffsets.value(qtInterfaceType.hash_code());
    if(offset>0){
        registerPointer(reinterpret_cast<void*>( quintptr(PointerToObjectLink::pointer()) + offset ));
    }else{
        for(const std::type_info* ifaces : m_inheritedInterfaces[qtInterfaceType.hash_code()]){
            Q_ASSERT(ifaces);
            addInterface(*ifaces);
        }
    }
}

void PointerToObjectInterfaceLink::removeInterface(const std::type_info& qtInterfaceType){
    if(m_availableInterfaces.contains(qtInterfaceType.hash_code())){
        m_availableInterfaces.remove(qtInterfaceType.hash_code());
        if(m_interfaceOffsets.contains(qtInterfaceType.hash_code())){
            size_t offset = m_interfaceOffsets.value(qtInterfaceType.hash_code());
            if(offset>0){
                unregisterPointer(reinterpret_cast<void*>( quintptr(PointerToObjectLink::pointer()) + offset ));
            }else{
                setInDestructor();
            }
        }else{
            for(const std::type_info* ifaces : m_inheritedInterfaces[qtInterfaceType.hash_code()]){
                Q_ASSERT(ifaces);
                removeInterface(*ifaces);
            }
            setInDestructor();
        }
    }
}

void* PointerToObjectInterfaceLink::typedPointer(const std::type_info& qtInterfaceType) const{
    if(m_interfaceOffsets.contains(qtInterfaceType.hash_code())){
        if(!m_availableInterfaces.contains(qtInterfaceType.hash_code()))
            return nullptr;
        size_t offset = m_interfaceOffsets.value(qtInterfaceType.hash_code());
        if(offset>0){
            return reinterpret_cast<void*>( quintptr(PointerToObjectLink::pointer()) + offset );
        }
    }
    return PointerToObjectLink::pointer();
}

void QSharedPointerToObjectInterfaceLink::addInterface(const std::type_info& qtInterfaceType){
    m_availableInterfaces << qtInterfaceType.hash_code();
    size_t offset = m_interfaceOffsets.value(qtInterfaceType.hash_code());
    if(offset>0){
        registerPointer(reinterpret_cast<void*>( quintptr(QSharedPointerToObjectLink::pointer()) + offset ));
    }else{
        for(const std::type_info* ifaces : m_inheritedInterfaces[qtInterfaceType.hash_code()]){
            Q_ASSERT(ifaces);
            addInterface(*ifaces);
        }
    }
}

void QSharedPointerToObjectInterfaceLink::removeInterface(const std::type_info& qtInterfaceType){
    if(m_availableInterfaces.contains(qtInterfaceType.hash_code())){
        m_availableInterfaces.remove(qtInterfaceType.hash_code());
        if(m_interfaceOffsets.contains(qtInterfaceType.hash_code())){
            size_t offset = m_interfaceOffsets.value(qtInterfaceType.hash_code());
            if(offset>0){
                unregisterPointer(reinterpret_cast<void*>( quintptr(QSharedPointerToObjectLink::pointer()) + offset ));
            }else{
                setInDestructor();
            }
        }else{
            for(const std::type_info* ifaces : m_inheritedInterfaces[qtInterfaceType.hash_code()]){
                Q_ASSERT(ifaces);
                removeInterface(*ifaces);
            }
            setInDestructor();
        }
    }
}

void* QSharedPointerToObjectInterfaceLink::typedPointer(const std::type_info& qtInterfaceType) const{
    if(m_interfaceOffsets.contains(qtInterfaceType.hash_code())){
        if(!m_availableInterfaces.contains(qtInterfaceType.hash_code()))
            return nullptr;
        size_t offset = m_interfaceOffsets.value(qtInterfaceType.hash_code());
        if(offset>0){
            return reinterpret_cast<void*>( quintptr(QSharedPointerToObjectLink::pointer()) + offset );
        }
    }
    return QSharedPointerToObjectLink::pointer();
}

void QSharedPointerToQObjectInterfaceLink::addInterface(const std::type_info& qtInterfaceType){
    m_availableInterfaces << qtInterfaceType.hash_code();
    size_t offset = m_interfaceOffsets.value(qtInterfaceType.hash_code());
    if(offset>0){
        registerPointer(reinterpret_cast<void*>( quintptr(QSharedPointerToQObjectLink::pointer()) + offset ));
    }else{
        for(const std::type_info* ifaces : m_inheritedInterfaces[qtInterfaceType.hash_code()]){
            Q_ASSERT(ifaces);
            addInterface(*ifaces);
        }
    }
}

void QSharedPointerToQObjectInterfaceLink::removeInterface(const std::type_info& qtInterfaceType){
    if(m_availableInterfaces.contains(qtInterfaceType.hash_code())){
        m_availableInterfaces.remove(qtInterfaceType.hash_code());
        if(m_interfaceOffsets.contains(qtInterfaceType.hash_code())){
            size_t offset = m_interfaceOffsets.value(qtInterfaceType.hash_code());
            if(offset>0){
                unregisterPointer(reinterpret_cast<void*>( quintptr(QSharedPointerToQObjectLink::pointer()) + offset ));
            }else{
                setInDestructor();
            }
        }else{
            for(const std::type_info* ifaces : m_inheritedInterfaces[qtInterfaceType.hash_code()]){
                Q_ASSERT(ifaces);
                removeInterface(*ifaces);
            }
            setInDestructor();
        }
    }
}

void* QSharedPointerToQObjectInterfaceLink::typedPointer(const std::type_info& qtInterfaceType) const{
    if(m_interfaceOffsets.contains(qtInterfaceType.hash_code())){
        if(!m_availableInterfaces.contains(qtInterfaceType.hash_code()))
            return nullptr;
        size_t offset = m_interfaceOffsets.value(qtInterfaceType.hash_code());
        if(offset>0){
            return reinterpret_cast<void*>( quintptr(QSharedPointerToQObjectLink::pointer()) + offset );
        }
    }
    return QSharedPointerToQObjectLink::pointer();
}

bool PointerToQObjectInterfaceLink::isInterfaceAvailable(const std::type_info& qtInterfaceType) const{
    return m_availableInterfaces.contains(qtInterfaceType.hash_code());
}

bool QSharedPointerToQObjectInterfaceLink::isInterfaceAvailable(const std::type_info& qtInterfaceType) const{
    return m_availableInterfaces.contains(qtInterfaceType.hash_code());
}

bool PointerToObjectInterfaceLink::isInterfaceAvailable(const std::type_info& qtInterfaceType) const{
    return m_availableInterfaces.contains(qtInterfaceType.hash_code());
}

bool QSharedPointerToObjectInterfaceLink::isInterfaceAvailable(const std::type_info& qtInterfaceType) const{
    return m_availableInterfaces.contains(qtInterfaceType.hash_code());
}


QObject* PointerContainer::qobject() const{
    return m_ptr_shared_pointer && m_shared_pointer_getter ? m_shared_pointer_getter(m_ptr_shared_pointer) : nullptr;
}

#ifdef QT_QTJAMBI_PORT
SlotObject::SlotObject(const QSharedPointer<QtJambiLink>& link, QMetaMethod signal)
    : QSlotObjectBase(&SlotObject::impl),
      m_signal(signal),
      m_link(link.toWeakRef())
{

}

void SlotObject::impl(int which, QSlotObjectBase *this_, QObject *, void **a, bool *ret){
    switch (which) {
    case Destroy:
        delete static_cast<SlotObject*>(this_);
        break;
    case Call:
    if(JNIEnv* env = qtjambi_current_environment()){
        SlotObject* _this = static_cast<SlotObject*>(this_);
        if(QSharedPointer<QtJambiLink> link = _this->m_link.toStrongRef()){
            QtJambiScope scope(link.data());
            if(link->isQObject()){
                const QVector<ParameterTypeInfo>* parameterTypeInfos = QtDynamicMetaObject::methodParameterInfo(env, _this->m_signal);
                Q_ASSERT(parameterTypeInfos);
                if(parameterTypeInfos->size()){
                    QVector<jvalue> converted_arguments(parameterTypeInfos->size()-1);

                    bool success = true;
                    for (int i = 0; i < converted_arguments.size(); ++i) {
                        const ParameterTypeInfo& parameterTypeInfo = parameterTypeInfos->at(i+1);
                        converted_arguments[i].l = nullptr;
                        if(!parameterTypeInfo.convertInternalToExternal(env, &scope, a[i+1], &converted_arguments[i], true)){
                            success = false;
                            break;
                        }
                    }
                    if (success) {
                        jvalue *args = converted_arguments.isEmpty() ? nullptr : converted_arguments.data();
                        if(link->isSharedPointer()){
                            QSharedPointerToQObjectLink* slink = static_cast<QSharedPointerToQObjectLink*>(link.data());
                            slink->pointerContainer()->emitExtraSignal(env, _this->m_signal.methodIndex(), args);
                        }else{
                            PointerToQObjectLink* plink = static_cast<PointerToQObjectLink*>(link.data());
                            plink->emitExtraSignal(env, _this->m_signal.methodIndex(), args);
                        }
                    } else {
                        qWarning("SlotObject::CallSignal: Failed to convert arguments");
                    }
                }else{
                    qWarning()<< "SlotObject::CallSignal: Failed to convert method types for signal " << _this->m_signal.methodSignature();
                }
            }
        }
    }
        break;
    case Compare:
        *ret = false;
        break;
    case NumOperations: ;
    }
}
#endif // QT_QTJAMBI_PORT

QtJambiSignalInfo::QtJambiSignalInfo(const QtJambiSignalInfo & info):
    m_signalObject(info.m_signalObject)
#ifdef QT_QTJAMBI_PORT
    ,m_CallEmitMethod(info.m_CallEmitMethod)
#endif
{}

QtJambiSignalInfo::QtJambiSignalInfo(QtJambiSignalInfo && info):
        m_signalObject(info.m_signalObject)
#ifdef QT_QTJAMBI_PORT
        ,m_CallEmitMethod(info.m_CallEmitMethod)
#endif
{}

QtJambiSignalInfo::QtJambiSignalInfo(): m_signalObject()
#ifdef QT_QTJAMBI_PORT
  , m_CallEmitMethod(nullptr)
#endif
{}

QtJambiSignalInfo::QtJambiSignalInfo(JNIEnv* env, jobject object, bool global
#ifdef QT_QTJAMBI_PORT
                                     , CallEmitMethod _CallEmitMethod
#endif
                                    )
  : m_signalObject(global ? static_cast<JObjectWrapperData*>(new JObjectGlobalWrapperData(env, object)) : static_cast<JObjectWrapperData*>(new JObjectWeakWrapperData(env, object)))
#ifdef QT_QTJAMBI_PORT
    ,m_CallEmitMethod(_CallEmitMethod)
#endif
{}

QtJambiSignalInfo & QtJambiSignalInfo::operator =(const QtJambiSignalInfo & info)
{
    m_signalObject = info.m_signalObject;
#ifdef QT_QTJAMBI_PORT
    m_CallEmitMethod = info.m_CallEmitMethod;
#endif
    return *this;
}

jobject QtJambiSignalInfo::signalObject() const{
    return m_signalObject ? m_signalObject->data() : nullptr;
}

#ifdef QT_QTJAMBI_PORT
void QtJambiSignalInfo::callEmitMethod(JNIEnv* env, jobject signal, jvalue *arguments) const{
    if(m_CallEmitMethod)
        m_CallEmitMethod(env, signal, arguments);
}
#endif

void qtjambi_add_dependency(QtJambiNativeID dependentLink, QtJambiNativeID ownerLink)
{
    QSharedPointer<QtJambiLink> _dependentLink = QtJambiLink::fromNativeId(dependentLink);
    QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::fromNativeId(ownerLink);
    if(_dependentLink && _ownerLink){
        _ownerLink->registerDependentObject(_dependentLink);
    }
}

void qtjambi_remove_dependency(QtJambiNativeID dependentLink, QtJambiNativeID ownerLink)
{
    QSharedPointer<QtJambiLink> _dependentLink = QtJambiLink::fromNativeId(dependentLink);
    QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::fromNativeId(ownerLink);
    if(_dependentLink && _ownerLink){
        _ownerLink->unregisterDependentObject(_dependentLink);
    }
}

void qtjambi_set_java_ownership_for_toplevel_object(JNIEnv *env, QObject* qobject)
{
    if(qobject && !qobject->parent()){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qobject).toStrongRef()){
            link->setJavaOwnership(env);
        }
    }
}

void qtjambi_set_default_ownership_for_toplevel_object(JNIEnv *env, QObject* qobject)
{
    if(qobject && !qobject->parent()){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qobject).toStrongRef()){
            link->setDefaultOwnership(env);
        }
    }
}

void qtjambi_set_cpp_ownership_for_toplevel_object(JNIEnv *env, QObject* qobject)
{
    if(qobject && !qobject->parent()){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qobject).toStrongRef()){
            link->setCppOwnership(env);
        }
    }
}

void qtjambi_set_java_ownership(JNIEnv *env, jobject object)
{
    if(object){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object)){
            link->setJavaOwnership(env);
        }
    }
}

void qtjambi_set_default_ownership(JNIEnv *env, jobject object)
{
    if(object){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object)){
            link->setDefaultOwnership(env);
        }
    }
}

void qtjambi_set_cpp_ownership(JNIEnv *env, jobject object)
{
    if(object){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object)){
            link->setCppOwnership(env);
        }
    }
}

void qtjambi_set_java_ownership(JNIEnv *env, QtJambiNativeID objectId)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(objectId)){
        link->setJavaOwnership(env);
    }
}

void qtjambi_set_default_ownership(JNIEnv *env, QtJambiNativeID objectId)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(objectId)){
        link->setDefaultOwnership(env);
    }
}

void qtjambi_set_cpp_ownership(JNIEnv *env, QtJambiNativeID objectId)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(objectId)){
        link->setCppOwnership(env);
    }
}

void qtjambi_register_pointer_deletion(void* ptr, bool* isShell)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForPointer(ptr)){
        if(isShell)
            *isShell = link->isShell();
        if(!link->isQObject() && !link->isShell()){
            JNIEnv* env = qtjambi_current_environment(!link->noThreadInitializationOnPurge());
            link->invalidate(env);
        }
    }
}

void qtjambi_register_pointer_deletion(void* ptr, void(*purgeFunction)(void*), bool* isShell)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForPointer(ptr)){
        if(isShell)
            *isShell = link->isShell();
        if(!link->isShell()){
            purgeFunction(ptr);
        }
        if(!link->isQObject() && !link->isShell()){
            JNIEnv* env = qtjambi_current_environment(!link->noThreadInitializationOnPurge());
            link->invalidate(env);
        }
    }
}

QtJambiDestructor::QtJambiDestructor(const QPointer<const QObject>& parent, const QSharedPointer<QtJambiLink>& link, void *pointer, int meta_type, QtJambiLink::Ownership ownership, PtrDeleterFunction destructor_function)
    : QObject(),
      m_parent(parent),
      m_link(link.toWeakRef()),
      m_pointer(pointer),
      m_meta_type(meta_type),
      m_ownership(ownership),
      m_deleter_function(destructor_function)
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
     ,m_qtTypeName(link->qtTypeName())
#endif
{
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "new QtJambiDestructor()")
}

QtJambiDestructor::~QtJambiDestructor()
{
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "delete QtJambiDestructor")
}

bool QtJambiDestructor::event(QEvent * e){
    if(m_pointer != nullptr && e->type()==QEvent::Type(QEvent::DeferredDelete)){
        if(m_parent && m_parent->thread()!=this->thread()){
            this->moveToThread(m_parent->thread());
            this->deleteLater();
            return true;
        }
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("native", "event DeferredDelete on QtJambiDestructor")
        QSharedPointer<QtJambiLink> link = m_link.toStrongRef();
#if QT_VERSION < 0x050000
        if (m_meta_type != QMetaType::Void)
#else
        if (m_meta_type != QMetaType::UnknownType)
#endif
        {
            QMetaType::destroy(m_meta_type, m_pointer);
        } else if (m_ownership == QtJambiLink::Ownership::Java && m_deleter_function) {
            QTJAMBI_INCREASE_COUNTER(destructorFunctionCalledCount, link)
            m_deleter_function(m_pointer);
            m_deleter_function = nullptr;
        }
        m_pointer = nullptr;

        // This cannot be deleted before now, since the type may have a virtual destructor and may be a shell class object,
        // which means it will try to access its link. But everything is ready for
        // deletion, as this was done when the java object was finalized.
        if(link){
            link->invalidate(qtjambi_current_environment());
        }
        // continue to delete
    }
    return QObject::event(e);
}

#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
const QString& QtJambiDestructor::qtTypeName() const
{
    return m_qtTypeName;
}
#endif


#ifdef QT_QTJAMBI_PORT
static const char *signatureTable[] = {
    /* 0 */ "()V",
    /* 1 */ "(Ljava/lang/Object;)V",
    /* 2 */ "(Ljava/lang/Object;Ljava/lang/Object;)V",
    /* 3 */ "(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V",
    /* 4 */ "(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V",
    /* 5 */ "(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V",
    /* 6 */ "(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V",
    /* 7 */ "(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V",
    /* 8 */ "(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V",
    /* 9 */ "(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V"
};
#endif

const QHash<int,QtJambiSignalInfo> qtjambi_resolve_signals(JNIEnv *env, jobject java_object, const QMetaObject* metaObject, JavaException& ocurredException){
    QHash<int,QtJambiSignalInfo> sinfos;
    if(!ocurredException.object()){
        try{
            QMap<int,QMetaMethod> metaMethods;
            {
                const int methodCount = metaObject->methodCount();
                for(int i=0; i<methodCount; i++){
                    QMetaMethod method = metaObject->method(i);
                    if(method.methodType()==QMetaMethod::Signal)
                        metaMethods.insert(method.methodIndex(), method);
                }
            }
            do{
                if(const QVector<SignalMetaInfo>* infos = signalMetaInfos(metaObject)){
                    sinfos.reserve(sinfos.size() + infos->size());
                    QList<QString> signalOrder;
                    QMap<QString,QList<const SignalMetaInfo*> > sortedSignals;
                    for(const SignalMetaInfo& info : *infos){
                        QString key = QString("%1::%2").arg(info.signal_declaring_class_name).arg(info.signal_name);
                        if(!signalOrder.contains(key))
                            signalOrder.append(key);
                        sortedSignals[key].append(&info);
                    }
                    for(const QString& key : signalOrder){
                        const QList<const SignalMetaInfo*>& signalsByName = sortedSignals[key];
                        if(signalsByName.size()==1){
                            const SignalMetaInfo& info = *signalsByName.first();
                            int signal_method_index = -1;
                            if(info.signal_method_index_provider){
                                signal_method_index = info.signal_method_index_provider();
                            }
                            if(signal_method_index>=0){
                                jclass clazz = resolveClass(env, info.signal_declaring_class_name);
                                if(!clazz){
                                    clazz = Java::Private::QtJambi::QtJambiInternal.findGeneratedSuperclass(env, env->GetObjectClass(java_object));
                                }
                                Q_ASSERT_X(clazz, "qtjambi_resolve_signals", qPrintable(QString("unable to find class").arg(info.signal_declaring_class_name)));

                                jfieldID fieldId = resolveField(env, info.signal_name, info.signal_signature, clazz);
                                Q_ASSERT_X(fieldId, "qtjambi_resolve_signals", qPrintable(QString("unable to find signal field %1").arg(info.signal_name)));

                                jobject signal = env->GetObjectField(java_object, fieldId);
                                qtjambi_throw_java_exception(env)
                                Q_ASSERT_X(signal, "qtjambi_resolve_signals", qPrintable(QString("unable to fetch signal field %1").arg(info.signal_name)));

                                jmethodID signalMethodId = nullptr;
#ifdef QT_QTJAMBI_PORT
                                if(info.signal_method_signature!=nullptr){
                                    signalMethodId = resolveMethod(env, info.signal_name, info.signal_method_signature, clazz);
                                    Q_ASSERT(signalMethodId);
                                }
#endif

                                jclass cls = env->GetObjectClass(signal);
                                Q_ASSERT_X(cls, "qtjambi_resolve_signals", "unable to determine signal class");

#ifdef QT_QTJAMBI_PORT
                                jmethodID emitMethodID = resolveMethod(env, "emit", signatureTable[info.signal_argumentcount], cls);
                                Q_ASSERT_X(emitMethodID, "qtjambi_resolve_signals", qPrintable(QString("unable to find signal's method  emit%1").arg(signatureTable[info.signal_argumentcount])));
#endif
                                if (signal
#ifdef QT_QTJAMBI_PORT
                                        && emitMethodID
#endif
                                        ) {  // everything went well
                                    jobject reflectedField = env->ToReflectedField(clazz, fieldId, false);
                                    qtjambi_throw_java_exception(env)
                                    jobject reflectedMethod = signalMethodId ? env->ToReflectedMethod(clazz, signalMethodId, false) : nullptr;
                                    qtjambi_throw_java_exception(env)
                                    metaMethods.remove(signal_method_index);
                                    Java::Private::QtJambi::QtJambiSignals$AbstractSignal.initializeSignal(env, signal, reflectedField, reflectedMethod, signal_method_index);
                                    env->DeleteLocalRef(reflectedField);
                                    env->DeleteLocalRef(reflectedMethod);
                                    sinfos.insert(signal_method_index, QtJambiSignalInfo(env, signal, false
#ifdef QT_QTJAMBI_PORT
                                                                                         , [emitMethodID](JNIEnv* env, jobject signal, jvalue * arguments){
                                                                                                if(arguments){
                                                                                                    env->CallVoidMethodA(signal, emitMethodID, arguments);
                                                                                                }else{
                                                                                                    env->CallVoidMethod(signal, emitMethodID);
                                                                                                }
                                                                                            }
#endif
                                                                                        ));
                                } else {  // something went bad
                                    if(signal_method_index>=0)
                                        sinfos.insert(signal_method_index, QtJambiSignalInfo());
                                }
                                env->DeleteLocalRef(cls);
                                env->DeleteLocalRef(signal);
                            }
                        }else if(signalsByName.size()>1){
                            const SignalMetaInfo& first = *signalsByName.first();
                            jclass clazz = resolveClass(env, first.signal_declaring_class_name);
                            if(!clazz){
                                clazz = Java::Private::QtJambi::QtJambiInternal.findGeneratedSuperclass(env, env->GetObjectClass(java_object));
                            }
                            Q_ASSERT_X(clazz, "qtjambi_resolve_signals", qPrintable(QString("unable to find class").arg(first.signal_declaring_class_name)));

                            jfieldID fieldId = resolveField(env, first.signal_name, qPrintable(QString("L%1$MultiSignal_%2;").arg(first.signal_declaring_class_name).arg(first.signal_name)), clazz);
                            if(!fieldId)
                                fieldId = resolveField(env, first.signal_name, "Lio/qt/core/QObject$MultiSignal;", clazz);
                            qtjambi_throw_java_exception(env)
                            Q_ASSERT_X(fieldId, "qtjambi_resolve_signals", qPrintable(QString("unable to find signal field %1").arg(first.signal_name)));

                            jobject reflectedField = env->ToReflectedField(clazz, fieldId, false);

                            jobject multiSignal = env->GetObjectField(java_object, fieldId);
                            qtjambi_throw_java_exception(env)
                            Q_ASSERT_X(multiSignal, "qtjambi_resolve_signals", qPrintable(QString("unable to fetch signal field %1").arg(first.signal_name)));

                            jobject methodList = qtjambi_arraylist_new(env, signalsByName.size());
                            jintArray jmethodIndexes = env->NewIntArray(signalsByName.size());
                            jboolean isCopy = false;
                            jint* methodIndexes = env->GetIntArrayElements(jmethodIndexes, &isCopy);
                            for(int i=0; i<signalsByName.size(); i++){
                                const SignalMetaInfo& info = *signalsByName.at(i);
                                int signal_method_index = -1;
                                if(info.signal_method_index_provider){
                                    signal_method_index = info.signal_method_index_provider();
                                }
                                if(signal_method_index>=0){
                                    jmethodID signalMethodId = nullptr;
#ifdef QT_QTJAMBI_PORT
                                    if(info.signal_method_signature!=nullptr){
                                        signalMethodId = resolveMethod(env, info.signal_name, info.signal_method_signature, clazz);
                                        qtjambi_throw_java_exception(env)
                                        Q_ASSERT(signalMethodId);
                                    }
#endif
                                    jobject reflectedMethod = signalMethodId ? env->ToReflectedMethod(clazz, signalMethodId, false) : nullptr;
                                    qtjambi_throw_java_exception(env)
                                    qtjambi_collection_add(env, methodList, reflectedMethod);
                                    env->DeleteLocalRef(reflectedMethod);

                                    jobject signal = Java::Private::QtJambi::QtJambiSignals$MultiSignal.signal(env, multiSignal, i);
                                    Q_ASSERT_X(signal, "qtjambi_resolve_signals", qPrintable(QString("unable to fetch signal field %1").arg(info.signal_name)));

                                    jclass cls = env->GetObjectClass(signal);
                                    Q_ASSERT_X(cls, "qtjambi_resolve_signals", "unable to determine signal class");

#ifdef QT_QTJAMBI_PORT
                                    jmethodID emitMethodID = resolveMethod(env, "emit", signatureTable[info.signal_argumentcount], cls);
                                    qtjambi_throw_java_exception(env)
                                    Q_ASSERT_X(emitMethodID, "qtjambi_resolve_signals", qPrintable(QString("unable to find signal's method  emit%1").arg(signatureTable[info.signal_argumentcount])));
#endif

                                    if (signal
#ifdef QT_QTJAMBI_PORT
                                            && emitMethodID
#endif
                                            ) {  // everything went well
                                        sinfos.insert(signal_method_index, QtJambiSignalInfo(env, signal, false
#ifdef QT_QTJAMBI_PORT
                                                                              , [emitMethodID](JNIEnv* env, jobject signal, jvalue * arguments){
                                                                                     if(arguments){
                                                                                         env->CallVoidMethodA(signal, emitMethodID, arguments);
                                                                                     }else{
                                                                                         env->CallVoidMethod(signal, emitMethodID);
                                                                                     }
                                                                                 }
#endif
                                                                                             ));
                                    } else {  // something went bad
                                        if(signal_method_index>=0)
                                            sinfos.insert(signal_method_index, QtJambiSignalInfo());
                                    }
                                    env->DeleteLocalRef(cls);
                                    env->DeleteLocalRef(signal);
                                    metaMethods.remove(signal_method_index);
                                }
                                methodIndexes[i] = signal_method_index;
                            }
                            env->ReleaseIntArrayElements(jmethodIndexes, methodIndexes, 0);
                            Java::Private::QtJambi::QtJambiSignals$MultiSignal.initializeSignals(env, multiSignal, reflectedField, methodList, jmethodIndexes);
                            env->DeleteLocalRef(methodList);
                            env->DeleteLocalRef(reflectedField);
                        }else{
                            Q_ASSERT_X(false, "qtjambi_resolve_signals", "should never reach this");
                        }
                    }
                }
                metaObject = metaObject->superClass();
            }while(metaObject);

            if(!metaMethods.isEmpty()){
                for(QMap<int,QMetaMethod>::const_iterator i = metaMethods.begin(); i!=metaMethods.end(); ++i){
                    metaObject = i.value().enclosingMetaObject();
                    if (qtjambi_metaobject_is_dynamic(metaObject)){
                        jclass java_class = static_cast<const QtDynamicMetaObject*>(metaObject)->javaClass();
                        jobject signalField = Java::Private::Runtime::Class.getDeclaredField(env, java_class, env->NewStringUTF(i.value().name().data()));
                        if(signalField){
                            jfieldID signalFieldId = env->FromReflectedField(signalField);
                            jobject signal = env->GetObjectField(java_object, signalFieldId);
                            qtjambi_throw_java_exception(env)
                            if(signal){
                                Java::Private::QtJambi::QtJambiSignals$AbstractSignal.initializeSignal(env, signal, signalField, nullptr, i.key());
                            }
                        }
                    }
                }
            }
        }catch(const JavaException& exn){
            ocurredException.addSuppressed(env, exn);
        }
    }
    return sinfos;
}

const QHash<int,QtJambiSignalInfo> qtjambi_resolve_extra_signals(JNIEnv *env, jobject java_object, const QMetaObject* originalMetaObject, JavaException& ocurredException){
    QHash<int,QtJambiSignalInfo> sinfos;
    if(!ocurredException.object()){
        try{
            const QMetaObject* metaObject = originalMetaObject;
            if (!qtjambi_metaobject_is_dynamic(metaObject)){
                {
                    const QVector<SignalMetaInfo> *infos = nullptr;
                    do{
                        infos = signalMetaInfos(metaObject);
                        if(infos!=nullptr)
                            break;
                        metaObject = metaObject->superClass();
                    }while(metaObject);
                    if(infos==nullptr){
                        metaObject = originalMetaObject;
                    }
                }
                QSet<const QMetaObject*> knownMetaObjects;
                QSet<const QMetaObject*> convertedMetaObjects;
                {
                    const QMetaObject* parentMetaObject = metaObject;
                    do{
                        knownMetaObjects.insert(parentMetaObject);
                        parentMetaObject = parentMetaObject->superClass();
                    }while(parentMetaObject);
                }

                if(metaObject != originalMetaObject){
                    metaObject = originalMetaObject;
                    const int methodCount = metaObject->methodCount();
                    sinfos.reserve(methodCount);
                    for(int i=0; i<methodCount; ++i){
                        QMetaMethod method = metaObject->method(i);
                        if(method.isValid() && !knownMetaObjects.contains(method.enclosingMetaObject()) && method.methodType()==QMetaMethod::Signal){
#ifdef QT_QTJAMBI_PORT
                            CallEmitMethod callEmitMethod = nullptr;
#endif
                            jobject signalObject = nullptr;
                            switch(method.parameterCount()){
                            case 0:
                                signalObject = Java::Private::QtJambi::QInstanceMemberSignals$PrivateSignal0.newInstance(env, java_object);
#ifdef QT_QTJAMBI_PORT
                                callEmitMethod = [](JNIEnv* env, jobject signal, jvalue * arguments){
                                    Java::Private::QtJambi::QInstanceMemberSignals$PrivateSignal0.emitMethod(env, signal, arguments);
                                };
#endif
                                break;
                            case 1:
                                signalObject = Java::Private::QtJambi::QInstanceMemberSignals$PrivateSignal1.newInstance(env, java_object);
#ifdef QT_QTJAMBI_PORT
                                callEmitMethod = [](JNIEnv* env, jobject signal, jvalue * arguments){
                                    Java::Private::QtJambi::QInstanceMemberSignals$PrivateSignal1.emitMethod(env, signal, arguments);
                                };
#endif
                                break;
                            case 2:
                                signalObject = Java::Private::QtJambi::QInstanceMemberSignals$PrivateSignal2.newInstance(env, java_object);
#ifdef QT_QTJAMBI_PORT
                                callEmitMethod = [](JNIEnv* env, jobject signal, jvalue * arguments){
                                    Java::Private::QtJambi::QInstanceMemberSignals$PrivateSignal2.emitMethod(env, signal, arguments);
                                };
#endif
                                break;
                            case 3:
                                signalObject = Java::Private::QtJambi::QInstanceMemberSignals$PrivateSignal3.newInstance(env, java_object);
#ifdef QT_QTJAMBI_PORT
                                callEmitMethod = [](JNIEnv* env, jobject signal, jvalue * arguments){
                                    Java::Private::QtJambi::QInstanceMemberSignals$PrivateSignal3.emitMethod(env, signal, arguments);
                                };
#endif
                                break;
                            case 4:
                                signalObject = Java::Private::QtJambi::QInstanceMemberSignals$PrivateSignal4.newInstance(env, java_object);
#ifdef QT_QTJAMBI_PORT
                                callEmitMethod = [](JNIEnv* env, jobject signal, jvalue * arguments){
                                    Java::Private::QtJambi::QInstanceMemberSignals$PrivateSignal4.emitMethod(env, signal, arguments);
                                };
#endif
                                break;
                            case 5:
                                signalObject = Java::Private::QtJambi::QInstanceMemberSignals$PrivateSignal5.newInstance(env, java_object);
#ifdef QT_QTJAMBI_PORT
                                callEmitMethod = [](JNIEnv* env, jobject signal, jvalue * arguments){
                                    Java::Private::QtJambi::QInstanceMemberSignals$PrivateSignal5.emitMethod(env, signal, arguments);
                                };
#endif
                                break;
                            case 6:
                                signalObject = Java::Private::QtJambi::QInstanceMemberSignals$PrivateSignal6.newInstance(env, java_object);
#ifdef QT_QTJAMBI_PORT
                                callEmitMethod = [](JNIEnv* env, jobject signal, jvalue * arguments){
                                    Java::Private::QtJambi::QInstanceMemberSignals$PrivateSignal6.emitMethod(env, signal, arguments);
                                };
#endif
                                break;
                            case 7:
                                signalObject = Java::Private::QtJambi::QInstanceMemberSignals$PrivateSignal7.newInstance(env, java_object);
#ifdef QT_QTJAMBI_PORT
                                callEmitMethod = [](JNIEnv* env, jobject signal, jvalue * arguments){
                                    Java::Private::QtJambi::QInstanceMemberSignals$PrivateSignal7.emitMethod(env, signal, arguments);
                                };
#endif
                                break;
                            case 8:
                                signalObject = Java::Private::QtJambi::QInstanceMemberSignals$PrivateSignal8.newInstance(env, java_object);
#ifdef QT_QTJAMBI_PORT
                                callEmitMethod = [](JNIEnv* env, jobject signal, jvalue * arguments){
                                    Java::Private::QtJambi::QInstanceMemberSignals$PrivateSignal8.emitMethod(env, signal, arguments);
                                };
#endif
                                break;
                            case 9:
                                signalObject = Java::Private::QtJambi::QInstanceMemberSignals$PrivateSignal9.newInstance(env, java_object);
#ifdef QT_QTJAMBI_PORT
                                callEmitMethod = [](JNIEnv* env, jobject signal, jvalue * arguments){
                                    Java::Private::QtJambi::QInstanceMemberSignals$PrivateSignal9.emitMethod(env, signal, arguments);
                                };
#endif
                                break;
                            default:
                                break;
                            }
                            if(signalObject){
                                {
                                    const QMetaObject* parentMetaObject = method.enclosingMetaObject();
                                    while(parentMetaObject && !knownMetaObjects.contains(parentMetaObject) && !convertedMetaObjects.contains(parentMetaObject)){
                                        qtjambi_from_QMetaObject(env, parentMetaObject);
                                        convertedMetaObjects.insert(parentMetaObject);
                                        parentMetaObject = parentMetaObject->superClass();
                                    }
                                }
                                const QVector<ParameterTypeInfo>* parameterTypeInfos = QtDynamicMetaObject::methodParameterInfo(env, method);
                                Q_ASSERT(parameterTypeInfos);
                                jobject signalParameterTypes = qtjambi_arraylist_new(env, parameterTypeInfos->size()-1);
                                for (int i=1; i<parameterTypeInfos->size(); ++i){
                                    qtjambi_collection_add(env, signalParameterTypes, parameterTypeInfos->at(i).javaClass());
                                }
                                Java::Private::QtJambi::QtJambiSignals$AbstractSignal.initializeExtraSignal(env, signalObject,
                                                      env->GetObjectClass(java_object),
                                                      signalParameterTypes, method.methodIndex());
                                sinfos.insert(method.methodIndex(), QtJambiSignalInfo(env, signalObject, true
#ifdef QT_QTJAMBI_PORT
                                                                                      , callEmitMethod
#endif
                                                                                      ));
                            }else{
                                sinfos.insert(method.methodIndex(), QtJambiSignalInfo());
                            }
                        }
                    }
                }
            }
        }catch(const JavaException& exn){
            ocurredException.addSuppressed(env, exn);
        }
    }
    return sinfos;
}
