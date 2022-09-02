/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/qcompilerdetection.h>

QT_WARNING_DISABLE_DEPRECATED
#include "qtjambi_core.h"
#include "qtjambi_repository_p.h"
#include "qtjambilink_p.h"
#include "qtjambi_interfaces_p.h"

#include "qtjambi_registry_p.h"
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
#include "qtjambi_cast.h"

// #define DEBUG_REFCOUNTING

#ifdef QT_NO_DEBUG
#define QTJAMBI_DEBUG_TRACE_WITH_THREAD(location)
#define QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(string)
#else

void qtjambi_debug_trace2(const char *message, const char* qtTypeName, const char *file, int line)
{
    static bool should = getenv("QTJAMBI_DEBUG_TRACE") != nullptr;
    if (should) {
        if(qtTypeName)
            fprintf(stderr, "%s of %s in THREAD %p ( %s:%d )\n", message, qtTypeName, reinterpret_cast<void*>(QThread::currentThreadId()), file, line);
        else
            fprintf(stderr, "%s in THREAD %p ( %s:%d )\n", message, reinterpret_cast<void*>(QThread::currentThreadId()), file, line);
        fflush(stderr);
    }
}

class QtJambiDebugMethodPrint2 {
public:
    QtJambiDebugMethodPrint2(const char* enterMessage, const char* leaveMethod, const char* qtTypeName, const char* file, int line);
    ~QtJambiDebugMethodPrint2();
private:
    const char* m_enterMessage;
    const char* m_leaveMethod;
    const char* m_qtTypeName;
    const char* m_file;
    int m_line;
};

QtJambiDebugMethodPrint2::QtJambiDebugMethodPrint2(const char* enterMessage, const char* leaveMethod, const char* qtTypeName, const char* file, int line) :
    m_enterMessage(enterMessage),
    m_leaveMethod(leaveMethod),
    m_qtTypeName(qtTypeName),
    m_file(file),
    m_line(line) {
    qtjambi_debug_trace2(m_enterMessage, m_qtTypeName, m_file, m_line);
}
QtJambiDebugMethodPrint2::~QtJambiDebugMethodPrint2(){
    qtjambi_debug_trace2(m_leaveMethod, m_qtTypeName, m_file, m_line);
}

#define QTJAMBI_DEBUG_TRACE_WITH_THREAD(location) QTJAMBI_DEBUG_TRACE(location)
#define QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(string)\
    QtJambiDebugMethodPrint2 __qt_qtjambi_debug_method_print("(native) entering: " string, "(native) leaving:  " string, qtTypeName(), __FILE__, __LINE__);\
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

Q_GLOBAL_STATIC(const QSharedPointer<QtJambiLink>, gDefaultPointer);

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
            jlong native_link = _link->m_nativeLink ? Java::QtJambi::QtJambiInternal$NativeLink::native__id(env, _link->m_nativeLink) : 0;
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
Q_GLOBAL_STATIC(QRecursiveMutex, g_magicLock);

#endif

#ifdef DEBUG_REFCOUNTING
typedef QHash<int, int> RefCountingHash;
Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gRefCountStaticLock, (QReadWriteLock::Recursive));
Q_GLOBAL_STATIC(RefCountingHash, gRefCountHash);
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
            QWriteLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
        }
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
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
        QWriteLocker lock(gLinkAccessLock());
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
        QWriteLocker lock(gLinkAccessLock());
        Q_UNUSED(lock)
        count = int(gUserObjectCache()->count());
    }
    return count;
}

void qtjambi_register_qobject_initialization(void *ptr, const QSharedPointer<QtJambiLink>& link) {
    if(qtjambi_object_cache_mode_get() != 0) {
        QWriteLocker locker(gLinkAccessLock());
        Q_UNUSED(locker)
        Q_ASSERT(gQObjectCache());
        gQObjectCache->insert(ptr, link.toWeakRef());
    }
}

void qtjambi_unregister_qobject_initialization(void *ptr) {
    if(qtjambi_object_cache_mode_get() != 0) {
        QWriteLocker locker(gLinkAccessLock());
        Q_UNUSED(locker)
        if(LinkHash* h = gQObjectCache())
            h->remove(ptr);
    }
}

bool is_finished(JNIEnv *env, QThread * thread){
    if(thread->isFinished()){
        return true;
    }
    Q_UNUSED(env)
    return false;
}

jobject getNativeLink(JNIEnv *env, jobject java){
    jobject nativeLink;
    if(java && Java::QtJambi::QtObjectInterface::isInstanceOf(env, java)){
        nativeLink = Java::QtJambi::QtJambiInternal::findInterfaceLink(env, java, true, false);
    }else{
        nativeLink = nullptr;
    }
    Q_ASSERT(nativeLink);
    return nativeLink;
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForQObject(JNIEnv *env, jobject javaObject, QObject *object, bool created_by_java, bool is_shell, const QMetaObject* superTypeForCustomMetaObject)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QtJambiLink::createLinkForQObject(JNIEnv *env, jobject java, QObject *object, bool created_by_java, bool hasCustomMetaObject)")
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewQObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, const QMetaObject* metaObject, QObject *object, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool isDeclarativeCall, bool is_shell, bool hasCustomMetaObject, JavaException& ocurredException)
{
    if(!ocurredException.object()){
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
        if(ocurredException.object()){
            qtJambiLink->dispose();
        }else{
            return qtJambiLink->getStrongPointer();
        }
    }
    return *gDefaultPointer;
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSharedPointerToQObject(JNIEnv *env, jobject javaObject, bool created_by_java, bool is_shell, void* ptr_shared_pointer, PointerDeleter pointerDeleter, PointerQObjectGetterFunction pointerGetter, const QMetaObject* superTypeForCustomMetaObject)
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
        qtJambiLink = new QSharedPointerToQObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces, object, created_by_java, is_shell, ptr_shared_pointer, pointerDeleter, pointerGetter, superTypeForCustomMetaObject, ocurredException);
    }else{
        qtJambiLink = new QSharedPointerToQObjectLink(env, nativeLink, javaObject, object, created_by_java, is_shell, ptr_shared_pointer, pointerDeleter, pointerGetter, superTypeForCustomMetaObject, ocurredException);
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSharedPointerToObject(JNIEnv *env, jobject javaObject,
                                              LINK_NAME_ARG(const char* qt_name)
                                              bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction,
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
                                                              LINK_NAME_ARG(qt_name)
                                                              created_by_java, is_shell, ownerFunction, ptr_shared_pointer, pointerDeleter, pointerGetter, ocurredException);
    }else{
        qtJambiLink = new QSharedPointerToObjectLink(env, nativeLink, javaObject,
                                                     LINK_NAME_ARG(qt_name)
                                                     created_by_java, is_shell, ownerFunction, ptr_shared_pointer, pointerDeleter, pointerGetter, ocurredException);
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForSharedPointerToContainer(JNIEnv *env, jobject javaObject,
                                              LINK_NAME_ARG(const char* qt_name)
                                              void* ptr_shared_pointer, PointerDeleter pointerDeleter, PointerGetterFunction pointerGetter, AbstractContainerAccess* containerAccess)
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
        qtJambiLink = new QSharedPointerToContainerInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
                                                              LINK_NAME_ARG(qt_name)
                                                              false, false, nullptr, ptr_shared_pointer, pointerDeleter, pointerGetter, containerAccess, ocurredException);
    }else{
        qtJambiLink = new QSharedPointerToContainerLink(env, nativeLink, javaObject,
                                                     LINK_NAME_ARG(qt_name)
                                                     false, false, nullptr, ptr_shared_pointer, pointerDeleter, pointerGetter, containerAccess, ocurredException);
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForContainer(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                        bool created_by_java, bool isCopy, AbstractContainerAccess* containerAccess)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(containerAccess);
    Q_ASSERT(metaType.isValid());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(javaObject));
    JavaException ocurredException;
    QtJambiLink* qtJambiLink;
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        qtJambiLink = new MetaTypedPointerToContainerInterfaceLink(env, nativeLink, javaObject,
                                                                     interfaceOffsets->offsets, interfaceOffsets->interfaces,
                                                                     interfaceOffsets->inheritedInterfaces,
                                                                     ptr, metaType, created_by_java, false,
                                                                     containerAccess, ocurredException);
    }else{
        qtJambiLink = new MetaTypedPointerToContainerLink(env, nativeLink, javaObject,
                                                            ptr, metaType, created_by_java, false,
                                                            containerAccess, ocurredException);
    }
    if(ocurredException.object()){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    if(created_by_java || isCopy){
        qtJambiLink->setJavaOwnership(env);
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

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(javaObject));
    JavaException ocurredException;
    QtJambiLink* qtJambiLink;
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        qtJambiLink = new DeletablePointerToContainerInterfaceLink(env, nativeLink, javaObject,
                                                                     interfaceOffsets->offsets, interfaceOffsets->interfaces,
                                                                     interfaceOffsets->inheritedInterfaces,
                                                                     LINK_NAME_ARG(qt_name)
                                                                     ptr, created_by_java,
                                                                     destructor_function, containerAccess, ocurredException);
    }else{
        qtJambiLink = new DeletablePointerToContainerLink(env, nativeLink, javaObject,
                                                            LINK_NAME_ARG(qt_name)
                                                            ptr, created_by_java,
                                                            destructor_function, containerAccess, ocurredException);
    }
    if(ocurredException.object()){
        qtJambiLink->dispose();
        ocurredException.raise();
        Q_ASSERT(false);// should not reach this
    }else{
        qtJambiLink->init(env);
    }
    if(created_by_java || isCopy){
        qtJambiLink->setJavaOwnership(env);
    }
    return qtJambiLink->getStrongPointer();
}


const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                        bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction)
{
    if(!ownerFunction){
        return createLinkForObject(env, javaObject, ptr, metaType,
                                   created_by_java, is_shell);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(ownerFunction);
    Q_ASSERT(metaType.isValid());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(javaObject));
    JavaException ocurredException;
    QtJambiLink* qtJambiLink;
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        qtJambiLink = new OwnedMetaTypedPointerToObjectInterfaceLink(env, nativeLink, javaObject,
                                                                     interfaceOffsets->offsets, interfaceOffsets->interfaces,
                                                                     interfaceOffsets->inheritedInterfaces,
                                                                     ptr, metaType, created_by_java,
                                                                     is_shell, ownerFunction, ocurredException);
    }else{
        qtJambiLink = new OwnedMetaTypedPointerToObjectLink(env, nativeLink, javaObject,
                                                            ptr, metaType, created_by_java,
                                                            is_shell, ownerFunction, ocurredException);
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForObject(JNIEnv *env, jobject javaObject, void *ptr, const QMetaType& metaType,
                                                                        bool created_by_java, bool is_shell)
{
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(metaType.isValid());

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(javaObject));
    JavaException ocurredException;
    QtJambiLink* qtJambiLink;
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        qtJambiLink = new MetaTypedPointerToObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
                                                       ptr, metaType, created_by_java, is_shell, ocurredException);
    }else{
        qtJambiLink = new MetaTypedPointerToObjectLink(env, nativeLink, javaObject,
                                              ptr, metaType, created_by_java, is_shell, ocurredException);
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool is_shell, PtrDeleterFunction deleter_function, PtrOwnerFunction ownerFunction)
{
    if(!ownerFunction){
        return createLinkForObject(env, javaObject, ptr,
                                   LINK_NAME_ARG(qt_name)
                                   created_by_java, is_shell, deleter_function);
    }
    if(!deleter_function){
        return createLinkForObject(env, javaObject, ptr,
                                   LINK_NAME_ARG(qt_name)
                                   created_by_java, is_shell);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(ownerFunction);
    Q_ASSERT(deleter_function);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(javaObject));
    JavaException ocurredException;
    QtJambiLink* qtJambiLink;
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        qtJambiLink = new DeletableOwnedPointerToObjectInterfaceLink(env, nativeLink, javaObject,
                                                                              interfaceOffsets->offsets,
                                                                              interfaceOffsets->interfaces,
                                                                              interfaceOffsets->inheritedInterfaces,
                                                                              LINK_NAME_ARG(qt_name)
                                                                              ptr, created_by_java,
                                                                              is_shell, deleter_function,
                                                                              ownerFunction, ocurredException);
    }else{
        qtJambiLink = new DeletableOwnedPointerToObjectLink(env, nativeLink, javaObject,
                                                                     LINK_NAME_ARG(qt_name)
                                                                     ptr, created_by_java,
                                                                     is_shell, deleter_function,
                                                                     ownerFunction, ocurredException);
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool is_shell, PtrDeleterFunction deleter_function)
{
    if(!deleter_function){
        return createLinkForObject(env, javaObject, ptr,
                                   LINK_NAME_ARG(qt_name)
                                   created_by_java, is_shell);
    }
    Q_ASSERT(env);
    Q_ASSERT(javaObject);
    Q_ASSERT(ptr);
    Q_ASSERT(deleter_function);

    // Initialize the link
    jobject nativeLink = getNativeLink(env, javaObject);
    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(javaObject));
    JavaException ocurredException;
    QtJambiLink* qtJambiLink;
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        qtJambiLink = new DeletablePointerToObjectInterfaceLink(env, nativeLink, javaObject,
                                                                         interfaceOffsets->offsets, interfaceOffsets->interfaces,
                                                                         interfaceOffsets->inheritedInterfaces,
                                                                         LINK_NAME_ARG(qt_name)
                                                                         ptr, created_by_java,
                                                                         is_shell, deleter_function, ocurredException);
    }else{
        qtJambiLink = new DeletablePointerToObjectLink(env, nativeLink, javaObject,
                                                                LINK_NAME_ARG(qt_name)
                                                                ptr, created_by_java,
                                                                is_shell, deleter_function, ocurredException);
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForObject(JNIEnv *env, jobject javaObject, void *ptr,
                                                                        LINK_NAME_ARG(const char* qt_name)
                                                                        bool created_by_java, bool is_shell)
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
                                                       LINK_NAME_ARG(qt_name)
                                                       ptr, created_by_java, is_shell, ocurredException);
    }else{
        qtJambiLink = new PointerToObjectLink(env, nativeLink, javaObject,
                                              LINK_NAME_ARG(qt_name)
                                              ptr, created_by_java, is_shell, ocurredException);
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, const QMetaType& metaType, bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
{
    if(!ownerFunction){
        return createLinkForNewObject(env, objectClass, nativeLink, javaObject, typeId, ptr, superTypeInfos, metaType, created_by_java, is_shell, ocurredException);
    }
    if(!ocurredException.object()){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);
        Q_ASSERT(ownerFunction);
        Q_ASSERT(metaType.isValid());

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        QtJambiLink* qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            qtJambiLink = new OwnedMetaTypedPointerToObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
                                                           ptr, metaType, created_by_java, is_shell, ownerFunction, ocurredException);
        }else{
            qtJambiLink = new OwnedMetaTypedPointerToObjectLink(env, nativeLink, javaObject,
                                                  ptr, metaType, created_by_java, is_shell, ownerFunction, ocurredException);
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
    return *gDefaultPointer;
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewContainer(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, const QMetaType& metaType, bool created_by_java, bool is_shell, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
{
    if(!ocurredException.object()){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);
        Q_ASSERT(containerAccess);
        Q_ASSERT(metaType.isValid());

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        QtJambiLink* qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            qtJambiLink = new MetaTypedPointerToContainerInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
                                                           ptr, metaType, created_by_java, is_shell, containerAccess, ocurredException);
        }else{
            qtJambiLink = new MetaTypedPointerToContainerLink(env, nativeLink, javaObject,
                                                  ptr, metaType, created_by_java, is_shell, containerAccess, ocurredException);
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
    return *gDefaultPointer;
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, const QMetaType& metaType, bool created_by_java, bool is_shell, JavaException& ocurredException)
{
    if(!ocurredException.object()){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);
        Q_ASSERT(metaType.isValid());

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        QtJambiLink* qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            qtJambiLink = new MetaTypedPointerToObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
                                                           ptr, metaType, created_by_java, is_shell, ocurredException);
        }else{
            qtJambiLink = new MetaTypedPointerToObjectLink(env, nativeLink, javaObject,
                                                  ptr, metaType, created_by_java, is_shell, ocurredException);
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
    return *gDefaultPointer;
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, PtrDeleterFunction deleter_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
{
    if(!ownerFunction){
        return createLinkForNewObject(env, objectClass, nativeLink, javaObject, typeId, ptr, superTypeInfos, created_by_java, is_shell, deleter_function, ocurredException);
    }
    if(!ocurredException.object()){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);
        Q_ASSERT(ownerFunction);
        Q_ASSERT(deleter_function);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        QtJambiLink* qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            qtJambiLink = new DeletableOwnedPointerToObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
                                                           LINK_NAME_ARG(getQtName(typeId))
                                                           ptr, created_by_java, is_shell, deleter_function, ownerFunction, ocurredException);
        }else{
            qtJambiLink = new DeletableOwnedPointerToObjectLink(env, nativeLink, javaObject,
                                                  LINK_NAME_ARG(getQtName(typeId))
                                                  ptr, created_by_java, is_shell, deleter_function, ownerFunction, ocurredException);
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
    return *gDefaultPointer;
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, PtrDeleterFunction deleter_function, JavaException& ocurredException)
{
    if(!deleter_function){
        return createLinkForNewObject(env, objectClass, nativeLink, javaObject, typeId, ptr, superTypeInfos, created_by_java, is_shell, ocurredException);
    }
    if(!ocurredException.object()){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);
        Q_ASSERT(deleter_function);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        QtJambiLink* qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            qtJambiLink = new DeletablePointerToObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
                                                           LINK_NAME_ARG(getQtName(typeId))
                                                           ptr, created_by_java, is_shell, deleter_function, ocurredException);
        }else{
            qtJambiLink = new DeletablePointerToObjectLink(env, nativeLink, javaObject,
                                                  LINK_NAME_ARG(getQtName(typeId))
                                                  ptr, created_by_java, is_shell, deleter_function, ocurredException);
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
    return *gDefaultPointer;
}

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewObject(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, JavaException& ocurredException)
{
    if(!ocurredException.object()){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        QtJambiLink* qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            qtJambiLink = new PointerToObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets,
                                                           interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
                                                           LINK_NAME_ARG(getQtName(typeId))
                                                           ptr, created_by_java, is_shell, ocurredException);
        }else{
            qtJambiLink = new PointerToObjectLink(env, nativeLink, javaObject,
                                                  LINK_NAME_ARG(getQtName(typeId))
                                                  ptr, created_by_java, is_shell, ocurredException);
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
            qtJambiLink = new DeletablePointerToObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
                                                           LINK_NAME_ARG(qt_name)
                                                           ptr, false, false, deleter_function, ocurredException);
        }else{
            qtJambiLink = new PointerToObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
                                                           LINK_NAME_ARG(qt_name)
                                                           ptr, false, false, ocurredException);
        }
    }else{
        if(deleter_function){
                qtJambiLink = new DeletablePointerToObjectLink(env, nativeLink, javaObject,
                                                      LINK_NAME_ARG(qt_name)
                                                      ptr, false, false, deleter_function, ocurredException);
        }else{
                qtJambiLink = new PointerToObjectLink(env, nativeLink, javaObject,
                                                      LINK_NAME_ARG(qt_name)
                                                      ptr, false, false, ocurredException);
        }
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
        qtJambiLink = new PointerToObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
                                                       LINK_NAME_ARG(qt_name)
                                                       ptr, false, false, ocurredException);
    }else{
        qtJambiLink = new PointerToObjectLink(env, nativeLink, javaObject,
                                              LINK_NAME_ARG(qt_name)
                                              ptr, false, false, ocurredException);
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
    qtJambiLink->setSplitOwnership(env);
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
        qtJambiLink = new PointerToContainerInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
                                                       LINK_NAME_ARG(qt_name)
                                                       ptr, false, containerAccess, ocurredException);
    }else{
        qtJambiLink = new PointerToContainerLink(env, nativeLink, javaObject,
                                              LINK_NAME_ARG(qt_name)
                                              ptr, false, containerAccess, ocurredException);
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
    qtJambiLink->setSplitOwnership(env);
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
        qtJambiLink = new DeletablePointerToContainerInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
                                                       LINK_NAME_ARG(qt_name)
                                                       ptr, false, destructor_function, containerAccess, ocurredException);
    }else{
        qtJambiLink = new DeletablePointerToContainerLink(env, nativeLink, javaObject,
                                              LINK_NAME_ARG(qt_name)
                                              ptr, false, destructor_function, containerAccess, ocurredException);
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
    qtJambiLink->setSplitOwnership(env);
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

QSharedPointer<QtJambiLink> QtJambiLink::findLinkForQObject(QObject *o)
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

const QSharedPointer<QtJambiLink>& QtJambiLink::fromNativeId(QtJambiNativeID native_id){
    if(!native_id){
        return *gDefaultPointer;
    }else{
        return reinterpret_cast<QtJambiLink*>(native_id)->getStrongPointer();
    }
}

const QSharedPointer<QtJambiLink>& QtJambiLink::findLinkForJavaInterface(JNIEnv *env, jobject java)
{
    if (env->IsSameObject(nullptr, java))
        return *gDefaultPointer;
    if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java)){
        if(jobject nativeLink = Java::QtJambi::QtJambiInternal::findInterfaceLink(env, java, true, true)){
            JObjectSynchronizer sync(env, nativeLink);
            Q_UNUSED(sync)
            QtJambiNativeID nativeId = QtJambiNativeID(Java::QtJambi::QtJambiInternal$NativeLink::native__id(env, nativeLink));
            const QSharedPointer<QtJambiLink>& qtJambiLink = fromNativeId(nativeId);
#if defined(QTJAMBI_DEBUG_TOOLS)
            Q_ASSERT(!qtJambiLink || QtJambiLink::checkLink(qtJambiLink.data()));  // check the C++ pointer is valid
#endif
            return qtJambiLink;
        }
    }
    return *gDefaultPointer;
}

const QSharedPointer<QtJambiLink>& QtJambiLink::findLinkForJavaObject(JNIEnv *env, jobject java)
{
    if (env->IsSameObject(nullptr, java))
        return *gDefaultPointer;
    if(jobject nativeLink = Java::QtJambi::QtJambiObject::nativeLink(env, java)){
        JObjectSynchronizer sync(env, nativeLink);
        Q_UNUSED(sync)
        QtJambiNativeID nativeId = QtJambiNativeID(Java::QtJambi::QtJambiInternal$NativeLink::native__id(env, nativeLink));
        const QSharedPointer<QtJambiLink>& qtJambiLink = fromNativeId(nativeId);
#if defined(QTJAMBI_DEBUG_TOOLS)
        Q_ASSERT(!qtJambiLink || QtJambiLink::checkLink(qtJambiLink.data()));  // check the C++ pointer is valid
#endif
        return qtJambiLink;
    }
    return *gDefaultPointer;
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
    QRecursiveMutexLocker locker(g_magicLock());
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
    QRecursiveMutexLocker locker(g_magicLock());
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
    QRecursiveMutexLocker locker(g_magicLock());
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
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("QtJambiLink::detachJavaLink(JNIEnv *env)")
        QTJAMBI_INCREASE_COUNTER_THIS(objectInvalidatedCount)
        Java::QtJambi::QtJambiInternal$NativeLink::detach(env, m_nativeLink, jlong(reinterpret_cast<void*>(this)), hasDisposedSignal());
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
        if(!isQObject())
            registerPointer(pointer());
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

bool QtJambiLink::createdByQml() const {
    return m_flags.testFlag(Flag::CreatedByQml);
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

QtJambiLink::QtJambiLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                                 LINK_NAME_ARG(const char *qt_name)
                                 bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException) :
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
    if(isDeclarativeCall){
        m_flags.setFlag(QtJambiLink::Flag::CreatedByQml);
    }

    m_java.weak = env->NewWeakGlobalRef(jobj);
    // Set the native__id field of the java object
    // We continue to use the hard local reference to java object in preference to the weak one we just created
    try{
        Java::QtJambi::QtJambiInternal$NativeLink::set_native__id(env, m_nativeLink, jlong(reinterpret_cast<void*>(this)));
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
        QTJAMBI_JNI_LOCAL_FRAME(env, 300)
        try{
            Java::QtJambi::QtJambiInternal$NativeLink::reset(env, m_nativeLink);
        } catch (const JavaException& exn) {
            exn.report(env);
        }
        env->DeleteGlobalRef(m_nativeLink);
        m_nativeLink = nullptr;
    }
    if(m_java.object && env){
        QTJAMBI_JNI_LOCAL_FRAME(env, 300)
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

// This method will always cleanup QtJambiLink's m_java.object.
void QtJambiLink::releaseJavaObject(JNIEnv *env)
{
    JObjectSynchronizer sync(env, m_nativeLink);
    Q_UNUSED(sync)
    if(m_java.object && env){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("QtJambiLink::releaseJavaObject(JNIEnv *env)")
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

void qtjambi_register_pointer(const QSharedPointer<QtJambiLink>& link) {
    if(qtjambi_object_cache_mode_get() != 0 && link && link->pointer()) {
        QWriteLocker locker(gLinkAccessLock());
        Q_UNUSED(locker)
        Q_ASSERT(gUserObjectCache());
        QWeakPointer<QtJambiLink> wlink = link.toWeakRef();
        if(!gUserObjectCache->contains(link->pointer(), wlink))
            gUserObjectCache->insert(link->pointer(), wlink);
    }
}

void QtJambiLink::registerPointer(void *ptr) {
    if(qtjambi_object_cache_mode_get() != 0) {
        QWriteLocker locker(gLinkAccessLock());
        Q_UNUSED(locker)
        Q_ASSERT(gUserObjectCache());
        QWeakPointer<QtJambiLink> wlink = m_this.toWeakRef();
        if(!gUserObjectCache->contains(ptr, wlink))
            gUserObjectCache->insert(ptr, wlink);
    }
}

void QtJambiLink::unregisterPointer(void *ptr) {
    if(qtjambi_object_cache_mode_get() != 0) {
        QWriteLocker locker(gLinkAccessLock());
        Q_UNUSED(locker)
        if(MultiLinkHash* h = gUserObjectCache()){
            QList<QWeakPointer<QtJambiLink>> values = h->values(ptr);
            for(QWeakPointer<QtJambiLink> wlink : values){
                QSharedPointer<QtJambiLink> link(wlink);
                if(!link || link==m_this){
                    h->remove(ptr, link.toWeakRef());
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

MetaTypedPointerToObjectLink::MetaTypedPointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                                                                                   void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, JavaException& ocurredException)
    :  PointerToObjectLink(env, nativeLink, jobj,
                                           LINK_NAME_META_TYPE_ARG(metaType)
                                           ptr, created_by_java, is_shell, ocurredException), m_meta_type(metaType
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                                                                            .id()
#endif
                                                                                               ) {}

PointerToContainerLink::PointerToContainerLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                    LINK_NAME_ARG(const char* qt_name)
                    void *ptr, bool created_by_java,
                    AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : PointerToObjectLink(env, nativeLink, jobj,
                                                           LINK_NAME_ARG(qt_name)
                                                           ptr, created_by_java, false, ocurredException), m_containerAccess(containerAccess) {}

PointerToContainerLink::~PointerToContainerLink(){
    m_containerAccess->dispose();
}

MetaTypedPointerToContainerLink::MetaTypedPointerToContainerLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                    void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell,
                    AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : MetaTypedPointerToObjectLink(env, nativeLink, jobj,
                                   ptr, metaType, created_by_java, is_shell, ocurredException), m_containerAccess(containerAccess) {}

MetaTypedPointerToContainerLink::~MetaTypedPointerToContainerLink(){
    m_containerAccess->dispose();
}

void MetaTypedPointerToContainerLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if (m_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("MetaTypedPointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
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
                            QTJAMBI_DEBUG_TRACE_WITH_THREAD("call QMetaType::destroy()")
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
                            QTJAMBI_DEBUG_TRACE_WITH_THREAD("use QtJambiDestructor")
                            setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                            QtJambiMetaTypeDestructor* destructor = new QtJambiMetaTypeDestructor(vud->pointer(), m_this, pointer, m_meta_type);
                            destructor->moveToThread(ownerThread->thread());
                            destructor->deleteLater();
                        }
                        pointer = nullptr;
                    }else if(!p->wasDeleted){
                        QTJAMBI_DEBUG_TRACE_WITH_THREAD("use QtJambiDestructor")
                        setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                        QtJambiMetaTypeDestructor* destructor = new QtJambiMetaTypeDestructor(obj, m_this, pointer, m_meta_type);
                        destructor->moveToThread(ownerThread->thread());
                        destructor->deleteLater();
                        pointer = nullptr;
                    }
                }
            }
            if(pointer){
                QTJAMBI_DEBUG_TRACE_WITH_THREAD("call QMetaType::destroy()")
                locker.unlock();
                metaType().destroy(pointer);
                locker.relock();
                dispose();
            }
        }else{
            dispose();
        }
    }
}

OwnedMetaTypedPointerToObjectLink::OwnedMetaTypedPointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                    void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell,
                    PtrOwnerFunction ownerFunction, JavaException& ocurredException)
    : MetaTypedPointerToObjectLink(env, nativeLink, jobj,
                                                           ptr, metaType, created_by_java, is_shell, ocurredException), m_owner_function(ownerFunction) {}

DeletableOwnedPointerToObjectLink::DeletableOwnedPointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                    LINK_NAME_ARG(const char* qt_name)
                    void *ptr, bool created_by_java, bool is_shell,
                    PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
    : PointerToObjectLink(env, nativeLink, jobj,
                                                            LINK_NAME_ARG(qt_name)
                                                            ptr, created_by_java, is_shell,
                                                            ocurredException), m_owner_function(ownerFunction), m_deleter_function(destructor_function) {}

DeletablePointerToObjectLink::DeletablePointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                    LINK_NAME_ARG(const char* qt_name)
                    void *ptr, bool created_by_java, bool is_shell,
                    PtrDeleterFunction destructor_function, JavaException& ocurredException)
    : PointerToObjectLink(env, nativeLink, jobj,
                                                            LINK_NAME_ARG(qt_name)
                                                            ptr, created_by_java, is_shell,
                                                            ocurredException), m_deleter_function(destructor_function) {}

DeletablePointerToContainerLink::DeletablePointerToContainerLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                    LINK_NAME_ARG(const char* qt_name)
                    void *ptr, bool created_by_java,
                    PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : DeletablePointerToObjectLink(env, nativeLink, jobj,
                                                           LINK_NAME_ARG(qt_name)
                                                           ptr, created_by_java, false, destructor_function, ocurredException), m_containerAccess(containerAccess) {}

DeletablePointerToContainerLink::~DeletablePointerToContainerLink(){
    m_containerAccess->dispose();
}

PointerToObjectLink::PointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                                         LINK_NAME_ARG(const char *qt_name)
                                         void *ptr, bool created_by_java, bool is_shell, JavaException& ocurredException) :
    QtJambiLink(env, nativeLink, jobj,
                LINK_NAME_ARG(qt_name)
                created_by_java, false, is_shell, ocurredException),
    m_pointer(ptr)
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
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("PointerToObjectLink::~PointerToObjectLink()")
    try{
        if(JNIEnv *env = qtjambi_current_environment(!noThreadInitializationOnPurge())){
            QTJAMBI_JNI_LOCAL_FRAME(env, 300)
            releaseJavaObject(env);
            detachJavaLink(env);
            deleteNativeObject(env);
        }else{
            releaseJavaObject(nullptr);
            detachJavaLink(nullptr);
            deleteNativeObject(nullptr);
        }
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

bool PointerToObjectLink::isSharedPointer() const { return false; }

Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gDependencyLock, (QReadWriteLock::Recursive))
typedef QMultiHash<QSharedPointer<QtJambiLink>, QWeakPointer<QtJambiLink>> DependencyHash;
Q_GLOBAL_STATIC(DependencyHash, gDependencies)

void QtJambiLink::registerDependentObject(const QSharedPointer<QtJambiLink>& link)
{
    if(!link->m_flags.testFlag(Flag::IsDependent)){
        if(!m_flags.testFlag(Flag::HasDependencies))
            m_flags.setFlag(Flag::HasDependencies);
        QWriteLocker locker(gDependencyLock());
        Q_UNUSED(locker)
        QWeakPointer<QtJambiLink> weakLink(link);
        if(!gDependencies->contains(m_this, weakLink))
            gDependencies->insert(m_this, weakLink);
        link->m_flags.setFlag(Flag::IsDependent);
    }
}

void QtJambiLink::unregisterDependentObject(const QSharedPointer<QtJambiLink>& link)
{
    if(link->m_flags.testFlag(Flag::IsDependent)){
        QWriteLocker locker(gDependencyLock());
        Q_UNUSED(locker)
        gDependencies->remove(m_this, link);
        if(!gDependencies->contains(m_this))
            m_flags.setFlag(Flag::HasDependencies, false);
        link->m_flags.setFlag(Flag::IsDependent, false);
    }
}

void QtJambiLink::invalidateDependentObjects(JNIEnv *env)
{
    if(m_flags.testFlag(Flag::HasDependencies)){
        QList<QWeakPointer<QtJambiLink>> dependentObjects;
        {
            QWriteLocker locker(gDependencyLock());
            Q_UNUSED(locker)
            dependentObjects = gDependencies->values(m_this);
            gDependencies->remove(m_this);
        }
        for(const QWeakPointer<QtJambiLink>& weakLink : dependentObjects){
            if(QSharedPointer<QtJambiLink> link = weakLink.toStrongRef()){
                link->invalidate(env);
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
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("QtJambiLink::resetObject(JNIEnv *)")
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
        invalidateDependentObjects(env);
        unregisterPointer(m_pointer);
        unregisterOffsets();
        if(!m_flags.testFlag(Flag::IsDependent) && (m_flags.testFlag(Flag::JavaOwnership) || forced)){
            m_pointer = nullptr;
            dispose();
            Java::Runtime::RuntimeException::throwNew(env, "Unable to delete object due to missing deleter or meta type information." QTJAMBI_STACKTRACEINFO );
        }else{
            dispose();
        }
    }
}
// Entry point for JVM finalization QtJambiObject#finalize()
void PointerToObjectLink::onClean(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::HasBeenCleaned)){
        if(!m_flags.testFlag(Flag::BlockDeletion)){
            m_flags.setFlag(Flag::BlockDeletion, true);
            QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("QtJambiLink::onClean(JNIEnv *)")
            QTJAMBI_INCREASE_COUNTER_THIS(cleanCallerCount)
            releaseJavaObject(env);
            detachJavaLink(env);
            deleteNativeObject(env);
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
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("QtJambiLink::onDispose(JNIEnv *)")
        setJavaOwnership(env);
        detachJavaLink(env);
        deleteNativeObject(env, true);
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
        if(m_owner_function){
            QWriteLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            if(const QObject* obj = m_owner_function(pointer())){
                const QObjectPrivate* p = QObjectPrivate::get(obj);
                if(p && !p->wasDeleted && !QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, obj)){
                    QTJAMBI_SET_OBJECTUSERDATA(ValueOwnerUserData, const_cast<QObject*>(obj), new ValueOwnerUserData(obj));
                }
            }
        }
    }
}

void DeletableOwnedPointerToObjectLink::init(JNIEnv* env){
    if(!isInitialized()){
        PointerToObjectLink::init(env);
        if(m_owner_function){
            QWriteLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            if(const QObject* obj = m_owner_function(pointer())){
                const QObjectPrivate* p = QObjectPrivate::get(obj);
                if(p && !p->wasDeleted && !QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, obj)){
                    QTJAMBI_SET_OBJECTUSERDATA(ValueOwnerUserData, const_cast<QObject*>(obj), new ValueOwnerUserData(obj));
                }
            }
        }
    }
}

void MetaTypedPointerToObjectLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if (m_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("MetaTypedPointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
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
            QTJAMBI_DEBUG_TRACE_WITH_THREAD("call QMetaType::destroy()")
            metaType().destroy(pointer);
            dispose();
        }else{
            dispose();
        }
    }
}

void OwnedMetaTypedPointerToObjectLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if (m_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("MetaTypedPointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
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
                            QTJAMBI_DEBUG_TRACE_WITH_THREAD("call QMetaType::destroy()")
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
                            QTJAMBI_DEBUG_TRACE_WITH_THREAD("use QtJambiDestructor")
                            setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                            QtJambiMetaTypeDestructor* destructor = new QtJambiMetaTypeDestructor(vud->pointer(), m_this, pointer, m_meta_type);
                            destructor->moveToThread(ownerThread->thread());
                            destructor->deleteLater();
                        }
                        pointer = nullptr;
                    }else if(!p->wasDeleted){
                        QTJAMBI_DEBUG_TRACE_WITH_THREAD("use QtJambiDestructor")
                        setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                        QtJambiMetaTypeDestructor* destructor = new QtJambiMetaTypeDestructor(obj, m_this, pointer, m_meta_type);
                        destructor->moveToThread(ownerThread->thread());
                        destructor->deleteLater();
                        pointer = nullptr;
                    }
                }
            }
            if(pointer){
                QTJAMBI_DEBUG_TRACE_WITH_THREAD("call QMetaType::destroy()")
                locker.unlock();
                metaType().destroy(pointer);
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
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("DeletablePointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
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
                            QTJAMBI_DEBUG_TRACE_WITH_THREAD("call destructor_function")
                            QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
                            PtrDeleterFunction _deleter_function = m_deleter_function;
                            void* _pointer = pointer;
                            bool isShell = this->isShell();
                            vud->addDeleter([_deleter_function,_pointer,isShell](){
                                _deleter_function(_pointer, isShell);
                            });
                            dispose();
                        }else{
                            QTJAMBI_DEBUG_TRACE_WITH_THREAD("use QtJambiDestructor")
                            setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                            QtJambiDestructor* destructor = new QtJambiDestructor(vud->pointer(), m_this, pointer, m_deleter_function, isShell());
                            destructor->moveToThread(ownerThread->thread());
                            destructor->deleteLater();
                        }
                        pointer = nullptr;
                    }else if(!p->wasDeleted){
                        QTJAMBI_DEBUG_TRACE_WITH_THREAD("use QtJambiDestructor")
                        setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                        QtJambiDestructor* destructor = new QtJambiDestructor(obj, m_this, pointer, m_deleter_function, isShell());
                        destructor->moveToThread(ownerThread->thread());
                        destructor->deleteLater();
                        pointer = nullptr;
                    }
                }
            }
            if(pointer){
                QTJAMBI_DEBUG_TRACE_WITH_THREAD("call destructor_function")
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

void DeletablePointerToContainerLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if (m_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("DeletablePointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
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
                            QTJAMBI_DEBUG_TRACE_WITH_THREAD("call destructor_function")
                            QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
                            PtrDeleterFunction _deleter_function = m_deleter_function;
                            void* _pointer = pointer;
                            bool isShell = this->isShell();
                            vud->addDeleter([_deleter_function,_pointer, isShell](){
                                _deleter_function(_pointer, isShell);
                            });
                            dispose();
                        }else{
                            QTJAMBI_DEBUG_TRACE_WITH_THREAD("use QtJambiDestructor")
                            setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                            QtJambiDestructor* destructor = new QtJambiDestructor(vud->pointer(), m_this, pointer, m_deleter_function, isShell());
                            destructor->moveToThread(ownerThread->thread());
                            destructor->deleteLater();
                        }
                        pointer = nullptr;
                    }else if(!p->wasDeleted){
                        QTJAMBI_DEBUG_TRACE_WITH_THREAD("use QtJambiDestructor")
                        setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                        QtJambiDestructor* destructor = new QtJambiDestructor(obj, m_this, pointer, m_deleter_function, isShell());
                        destructor->moveToThread(ownerThread->thread());
                        destructor->deleteLater();
                        pointer = nullptr;
                    }
                }
            }
            if(pointer){
                QTJAMBI_DEBUG_TRACE_WITH_THREAD("call destructor_function")
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
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("DeletablePointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
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
            QTJAMBI_DEBUG_TRACE_WITH_THREAD("call destructor_function")
            QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
            m_deleter_function(pointer, isShell());
        }
        dispose();
    }
}

// ### END ######################  PointerToObjectLink  ######################## END ####

// ### BEGIN ###################  PointerToQObjectLink  ###################### BEGIN ####

void qtjambi_resolve_signals(JNIEnv *env, jobject java_object, const QMetaObject* metaObject, JavaException& ocurredException);
JObjectWrapper qtjambi_resolve_extra_signal(JNIEnv *env, jobject java_object, const QMetaMethod& method);

PointerToQObjectWithExtraSignalsLink::PointerToQObjectWithExtraSignalsLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMetaObject* metaObject, QObject *object, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException)
    : PointerToQObjectLink(env, nativeLink, jobj, metaObject, object, created_by_java, isDeclarativeCall, is_shell, ocurredException)
{
}

PointerToQObjectLink::PointerToQObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMetaObject* metaObject, QObject *object, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException) :
    QtJambiLink(env, nativeLink, jobj,
                LINK_NAME_ARG(metaObject->className())
                created_by_java, isDeclarativeCall, is_shell, ocurredException),
    m_pointer(object)
{
    qtjambi_resolve_signals(env, jobj, metaObject, ocurredException);
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
        const QMetaObject* metaObject = object->metaObject();
        /*
        //if we use dynamic meta object:
        if(QMetaObjectPrivate::get(metaObject)->flags & DynamicMetaObject){
            QObjectPrivate* p = QObjectPrivate::get(object);
            if(p && !p->metaObject)
                p->metaObject = const_cast<QAbstractDynamicMetaObject *>(static_cast<const QAbstractDynamicMetaObject *>(metaObject));
        }
        */
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
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("PointerToQObjectLink::~PointerToQObjectLink()")
    try{
        if(JNIEnv *env = qtjambi_current_environment(!noThreadInitializationOnPurge())){
            QTJAMBI_JNI_LOCAL_FRAME(env, 300)
            releaseJavaObject(env);
            detachJavaLink(env);
            deleteNativeObject(env);
        }else{
            releaseJavaObject(nullptr);
            detachJavaLink(nullptr);
            deleteNativeObject(nullptr);
        }
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
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
    QWriteLocker locker(QtJambiLinkUserData::lock());
    Q_UNUSED(locker)
    m_pointer = nullptr;
}

void PointerToQObjectLink::invalidate(JNIEnv *env) {
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("QtJambiLink::resetObject(JNIEnv *)")
    invalidateDependentObjects(env);
    releaseJavaObject(env);
    QWriteLocker locker(QtJambiLinkUserData::lock());
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
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("PointerToQObjectLink::deleteNativeObject(JNIEnv *, bool)")
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
                        QtJambiExceptionBlocker __qt_exceptionBlocker;
                        delete m_pointer;
                        __qt_exceptionBlocker.release(env);
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
                        QtJambiExceptionBlocker __qt_exceptionBlocker;
                        delete m_pointer;
                        __qt_exceptionBlocker.release(env);
                    }else if(!objectThread->isRunning() && !m_pointer->parent() && !m_pointer->isWidgetType()){
                        QtJambiExceptionBlocker __qt_exceptionBlocker;
                        delete m_pointer;
                        __qt_exceptionBlocker.release(env);
                    }else if(objectThread==m_pointer){
                        if(objectThread->isRunning()){
                            qWarning("Trying to delete a running QThread '%s'",
                                    qPrintable(m_pointer->objectName()));
                        }
                        QtJambiExceptionBlocker __qt_exceptionBlocker;
                        delete m_pointer;
                        __qt_exceptionBlocker.release(env);
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
                         QWriteLocker locker(QtJambiLinkUserData::lock());
                         QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, objectThread);
                         if(!threadData){
                            QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, objectThread, threadData = new QThreadUserData(objectThread));
                         }
                         return threadData;
                    }()){
                    setDeleteLater();
                    QTJAMBI_DEBUG_TRACE_WITH_THREAD("attach QObject to thread shutdown procedure.")
                    qthreadData->deleteAtThreadEnd(m_pointer);
                } else {
                    // currentThread is not objectThread
                    // && objectThread not finished
                    // && objectThread not pointer
                    // && no QAbstractEventDispatcher
                    if(QThread* tpointer = qobject_cast<QThread*>(m_pointer)){
                        if(!tpointer->parent()){
                            QtJambiExceptionBlocker __qt_exceptionBlocker;
                            delete tpointer;
                            __qt_exceptionBlocker.release(env);
                        }
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
                // trust me it is possible that pointer is null
                {
                    QWriteLocker locker(QtJambiLinkUserData::lock());
                    Q_UNUSED(locker)
                    if(m_pointer){
                        if(QtJambiLinkUserData* data = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, m_pointer)){
                            QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, m_pointer, nullptr);
                            locker.unlock();
                            delete data;
                            locker.relock();
                        }
                        m_pointer = nullptr;
                    }
                }
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
            QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("QtJambiLink::onClean(JNIEnv *)")
            QTJAMBI_INCREASE_COUNTER_THIS(cleanCallerCount)
            releaseJavaObject(env);
            detachJavaLink(env);
            deleteNativeObject(env);
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
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("QtJambiLink::onDispose(JNIEnv *)")
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
                                       LINK_NAME_ARG(const char *qt_name)
                                       bool created_by_java, bool is_shell, JavaException& ocurredException)
    : QtJambiLink(env, nativeLink, jobj,
                      LINK_NAME_ARG(qt_name)
                      created_by_java, false, is_shell, ocurredException){
}

QSharedPointerLink::~QSharedPointerLink(){}

bool QSharedPointerLink::isSharedPointer() const {
    return true;
}

void QSharedPointerLink::setCppOwnership(JNIEnv *){}
void QSharedPointerLink::setSplitOwnership(JNIEnv *){}

// #### END ######################  QSharedPointerLink  ######################## END ####

// ### BEGIN #################  QSharedPointerToObjectLink  ################### BEGIN ###

QSharedPointerToContainerLink::QSharedPointerToContainerLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                                                             LINK_NAME_ARG(const char* qt_name)
                                                             bool created_by_java, bool is_shell,
                                                             PtrOwnerFunction ownerFunction, void* ptr_shared_pointer,
                                                             PointerDeleter shared_pointer_deleter, PointerGetterFunction pointerGetter,
                                                             AbstractContainerAccess* containerAccess, JavaException& ocurredException) :
    QSharedPointerToObjectLink(env, nativeLink, jobj,
                               LINK_NAME_ARG(qt_name)
                               created_by_java, is_shell,
                               ownerFunction, ptr_shared_pointer,
                               shared_pointer_deleter, pointerGetter,
                               ocurredException),
    m_containerAccess(containerAccess)
{
}

QSharedPointerToContainerLink::~QSharedPointerToContainerLink(){
    m_containerAccess->dispose();
}

QSharedPointerToObjectLink::QSharedPointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                                                       LINK_NAME_ARG(const char *qt_name)
                                                       bool created_by_java, bool is_shell,
                                                       PtrOwnerFunction ownerFunction, void* ptr_shared_pointer,
                                                       PointerDeleter shared_pointer_deleter, PointerGetterFunction pointerGetter, JavaException& ocurredException) :
    QSharedPointerLink(env, nativeLink, jobj,
                       LINK_NAME_ARG(qt_name)
                       created_by_java, is_shell, ocurredException),
    m_ptr_shared_pointer(ptr_shared_pointer),
    m_shared_pointer_deleter(shared_pointer_deleter),
    m_shared_pointer_getter(pointerGetter),
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
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("QSharedPointerToObjectLink::~QSharedPointerToObjectLink()")
        if(JNIEnv *env = qtjambi_current_environment(!noThreadInitializationOnPurge())){
            QTJAMBI_JNI_LOCAL_FRAME(env, 300)
            releaseJavaObject(env);
            detachJavaLink(env);
            deleteNativeObject(env);
        }else{
            releaseJavaObject(nullptr);
            detachJavaLink(nullptr);
            deleteNativeObject(nullptr);
        }
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

void QSharedPointerToObjectLink::init(JNIEnv* env){
    if(!isInitialized()){
        QtJambiLink::init(env);
        if(m_owner_function){
            QWriteLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            if(const QObject* obj = m_owner_function(pointer())){
                const QObjectPrivate* p = QObjectPrivate::get(obj);
                if(p && !p->wasDeleted && !QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, obj))
                    QTJAMBI_SET_OBJECTUSERDATA(ValueOwnerUserData, const_cast<QObject*>(obj), new ValueOwnerUserData(obj));
            }
        }
    }
}
void *QSharedPointerToObjectLink::pointer() const { return m_ptr_shared_pointer ? m_shared_pointer_getter(m_ptr_shared_pointer) : nullptr; }

QObject *QSharedPointerToObjectLink::qobject() const { Q_ASSERT_X(false, "QtJambiLink", "Pointer is not QObject"); return nullptr; }

void* QSharedPointerToObjectLink::getSharedPointer() const{
    return m_ptr_shared_pointer;
}

void QSharedPointerToObjectLink::invalidate(JNIEnv *env) {
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("QtJambiLink::resetObject(JNIEnv *)")
    invalidateDependentObjects(env);
    releaseJavaObject(env);
    detachJavaLink(env);
    dispose();
}

void QSharedPointerToObjectLink::deleteNativeObject(JNIEnv *env, bool){
    if(m_ptr_shared_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("QSharedPointerToObjectLink::deleteNativeObject(JNIEnv *env, bool)")
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
                            QTJAMBI_DEBUG_TRACE_WITH_THREAD("call shared pointer deleter function")
                            QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
                            PointerDeleter _shared_pointer_deleter = m_shared_pointer_deleter;
                            void* _shared_pointer = shared_pointer;
                            bool isShell = this->isShell();
                            vud->addDeleter([_shared_pointer_deleter,_shared_pointer, isShell](){
                                _shared_pointer_deleter(_shared_pointer, isShell);
                            });
                            dispose();
                        }else{
                            QTJAMBI_DEBUG_TRACE_WITH_THREAD("use QtJambiDestructor (calling shared pointer deleter function)")
                            setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                            QtJambiDestructor* destructor = new QtJambiDestructor(
                                        vud->pointer(),
                                        m_this,
                                        shared_pointer,
                                        m_shared_pointer_deleter,
                                        isShell());
                            destructor->moveToThread(ownerThread->thread());
                            destructor->deleteLater();
                        }
                        shared_pointer = nullptr;
                    }else if(!p->wasDeleted){
                        QTJAMBI_DEBUG_TRACE_WITH_THREAD("use QtJambiDestructor (calling shared pointer deleter function)")
                        setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                        QtJambiDestructor* destructor = new QtJambiDestructor(
                                    obj,
                                    m_this,
                                    shared_pointer,
                                    m_shared_pointer_deleter,
                                    isShell());
                        destructor->moveToThread(ownerThread->thread());
                        destructor->deleteLater();
                        shared_pointer = nullptr;
                    }
                }
            }
        }

        if(shared_pointer){
            QTJAMBI_DEBUG_TRACE_WITH_THREAD("call shared pointer deleter function")
            QTJAMBI_INCREASE_COUNTER_THIS(destructorFunctionCalledCount)
            m_shared_pointer_deleter(shared_pointer, isShell());
            dispose();
        }
    }
}
// Entry point for JVM finalization QtJambiObject#finalize()
void QSharedPointerToObjectLink::onClean(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::HasBeenCleaned)){
        if(!m_flags.testFlag(Flag::BlockDeletion)){
            m_flags.setFlag(Flag::BlockDeletion, true);
            QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("QtJambiLink::onClean(JNIEnv *)")
            QTJAMBI_INCREASE_COUNTER_THIS(cleanCallerCount)
            releaseJavaObject(env);
            detachJavaLink(env);
            deleteNativeObject(env);
            m_flags.setFlag(Flag::HasBeenCleaned);
            m_flags.setFlag(Flag::BlockDeletion, false);
        }
    }
}

// This allows the Java language to attempt to expedite the disposal of the C++ object
//  (when that this possible), however its possible the C++ object will not be destroyed
//  as the Java side does not own it.
// Entry point for QtJambiObject#dispose() method
void QSharedPointerToObjectLink::onDispose(JNIEnv *env)
{
    if(!m_flags.testFlag(Flag::BlockDeletion)){
        m_flags.setFlag(Flag::BlockDeletion, true);
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("QtJambiLink::onDispose(JNIEnv *)")
        setJavaOwnership(env);
        detachJavaLink(env);
        deleteNativeObject(env, true);
        QTJAMBI_INCREASE_COUNTER_THIS(disposeCalledCount)
        m_flags.setFlag(Flag::BlockDeletion, false);
    }
}

// #### END ##################  QSharedPointerToObjectLink  ##################### END ####

// ### BEGIN #################  QSharedPointerToQObjectLink  ################### BEGIN ###

QSharedPointerToQObjectLink::QSharedPointerToQObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj, QObject* object, bool created_by_java, bool is_shell, void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, PointerQObjectGetterFunction pointerGetter, const QMetaObject* superTypeForCustomMetaObject, JavaException& ocurredException)
    : QSharedPointerLink(env, nativeLink, jobj,
                         LINK_NAME_ARG(superTypeForCustomMetaObject ? superTypeForCustomMetaObject->className() : object->metaObject()->className())
                         created_by_java, is_shell, ocurredException),
      m_pointerContainer(nullptr)
{
    jobject local = env->NewLocalRef(jobj);
    const QMetaObject* metaObject = superTypeForCustomMetaObject ? superTypeForCustomMetaObject : object->metaObject();
    /* //if we use dynamic meta object:
    if(QMetaObjectPrivate::get(metaObject)->flags & DynamicMetaObject){
        QObjectPrivate* p = QObjectPrivate::get(object);
        if(p && !p->metaObject)
            p->metaObject = const_cast<QAbstractDynamicMetaObject *>(static_cast<const QAbstractDynamicMetaObject *>(metaObject));
    }*/
    if(!ocurredException.object()){
        if(superTypeForCustomMetaObject){
            m_pointerContainer = new PointerContainerWithPendingExtraSignals(env, local, superTypeForCustomMetaObject, this->getStrongPointer(), ptr_shared_pointer, isShell(), shared_pointer_deleter, pointerGetter, ocurredException);
        }else{
            QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(metaObject);
            if(extraSignals.isEmpty())
                m_pointerContainer = new PointerContainer(env, local, metaObject, this->getStrongPointer(), ptr_shared_pointer, isShell(), shared_pointer_deleter, pointerGetter, ocurredException);
            else
                m_pointerContainer = new PointerContainerWithExtraSignals(env, local, metaObject, this->getStrongPointer(), ptr_shared_pointer, isShell(), shared_pointer_deleter, pointerGetter, ocurredException);
        }
    }
    if(!ocurredException.object()){
        bool _isValueOwner = isValueOwner(metaObject);
        QWriteLocker locker(QtJambiLinkUserData::lock());
        Q_UNUSED(locker)
        QtJambiLinkUserData* lud;
        if(superTypeForCustomMetaObject)
            lud = new QtJambiLinkUserData(getWeakPointer());
        else
            lud = new QtJambiMetaObjectLinkUserData(getWeakPointer(), metaObject);
        QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, object, lud);
        if(_isValueOwner && !QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, object)){
            QTJAMBI_SET_OBJECTUSERDATA(ValueOwnerUserData, object, new ValueOwnerUserData(object));
        }
    }else{
        m_flags.setFlag(Flag::IsJavaLinkDetached);
    }
    env->DeleteLocalRef(local);
}

QSharedPointerToQObjectLink::~QSharedPointerToQObjectLink(){
    try{
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("QSharedPointerToQObjectLink::~QSharedPointerToQObjectLink()")
        if(JNIEnv *env = qtjambi_current_environment(!noThreadInitializationOnPurge())){
            QTJAMBI_JNI_LOCAL_FRAME(env, 300)
            releaseJavaObject(env);
            detachJavaLink(env);
            deleteNativeObject(env);
        }else{
            releaseJavaObject(nullptr);
            detachJavaLink(nullptr);
            deleteNativeObject(nullptr);
        }
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

void QSharedPointerToQObjectLink::invalidate(JNIEnv *env) {
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("QtJambiLink::resetObject(JNIEnv *)")
    invalidateDependentObjects(env);
    releaseJavaObject(env);
    dispose();
}

void QSharedPointerToQObjectLink::deleteNativeObject(JNIEnv *env, bool){
    if (m_pointerContainer && m_pointerContainer->qobject() && !isDeleteLater()){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("QSharedPointerToQObjectLink::deleteNativeObject(JNIEnv *env, bool)")
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
                     QWriteLocker locker(QtJambiLinkUserData::lock());
                     QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, objectThread);
                     if(!threadData){
                        QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, objectThread, threadData = new QThreadUserData(objectThread));
                     }
                     return threadData;
                }()){
                setDeleteLater();
                QTJAMBI_DEBUG_TRACE_WITH_THREAD("attach QObject to thread shutdown procedure.")
                qthreadData->deleteAtThreadEnd(m_pointerContainer);
            } else if(objectThread){
                if(qobject_cast<QThread*>(m_pointerContainer->qobject())){
                    delete m_pointerContainer;
                    m_pointerContainer = nullptr;
                }else{
                    qWarning("QObjects can only be implicitly garbage collected when owned"
                            " by a QThread, native resource ('%s' [%s]) is leaked",
                            qPrintable(m_pointerContainer->qobject()->objectName()),
                            m_pointerContainer->qobject()->metaObject()->className());
                    // trust me it is possible that pointer is null
                    {
                        QWriteLocker locker(QtJambiLinkUserData::lock());
                        Q_UNUSED(locker)
                        if(m_pointerContainer){
                            if(QtJambiLinkUserData* data = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, m_pointerContainer->qobject())){
                                QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, m_pointerContainer->qobject(), nullptr);
                                locker.unlock();
                                delete data;
                                locker.relock();
                            }
                        }
                        m_pointerContainer = nullptr;
                    }
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
    if(!m_flags.testFlag(Flag::HasBeenCleaned)){
        if(!m_flags.testFlag(Flag::BlockDeletion)){
            m_flags.setFlag(Flag::BlockDeletion, true);
            QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("QtJambiLink::onClean(JNIEnv *)")
            QTJAMBI_INCREASE_COUNTER_THIS(cleanCallerCount)
            releaseJavaObject(env);
            detachJavaLink(env);
            deleteNativeObject(env);
            m_flags.setFlag(Flag::HasBeenCleaned);
            m_flags.setFlag(Flag::BlockDeletion, false);
        }
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
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("QtJambiLink::onDispose(JNIEnv *)")
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

jobject PointerToQObjectLink::getExtraSignal(JNIEnv*, const QMetaMethod&) const{
    return nullptr;
}

PointerToPendingQObjectLink::PointerToPendingQObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMetaObject* metaObject, QObject *ptr, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException)
    : PointerToQObjectLink(env, nativeLink, jobj, metaObject, ptr, created_by_java, isDeclarativeCall, is_shell, ocurredException),
      m_metaObject(metaObject)
{
}

void PointerToPendingQObjectLink::init(JNIEnv* env){
    if(!isInitialized()){
        QtJambiLink::init(env);
        QObject* object = qobject();
        Q_ASSERT(object);
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
        m_qtTypeName = m_metaObject->className();
#endif
        {
            bool _isValueOwner = isValueOwner(m_metaObject);
            QWriteLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, object, new QtJambiLinkUserData(getWeakPointer()));
            if(_isValueOwner && !QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, object)){
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
                JObjectWrapper signal = qtjambi_resolve_extra_signal(env, getJavaObjectLocalRef(env), method);
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
    : PointerToQObjectInterfaceLink(env, nativeLink, jobj, interfaceOffsets, interfaces, inheritedInterfaces, metaObject, ptr, created_by_java, isDeclarativeCall, is_shell, ocurredException),
      m_metaObject(metaObject)
{
}

void PointerToPendingQObjectInterfaceLink::init(JNIEnv* env){
    if(!isInitialized()){
        QtJambiLink::init(env);
        QObject* object = qobject();
        Q_ASSERT(object);
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
        m_qtTypeName = m_metaObject->className();
#endif
        {
            bool _isValueOwner = isValueOwner(m_metaObject);
            QWriteLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, object, new QtJambiLinkUserData(getWeakPointer()));
            if(_isValueOwner && !QTJAMBI_GET_OBJECTUSERDATA(ValueOwnerUserData, object)){
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
                JObjectWrapper signal = qtjambi_resolve_extra_signal(env, getJavaObjectLocalRef(env), method);
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
                JObjectWrapper signal = qtjambi_resolve_extra_signal(env, getJavaObjectLocalRef(env), method);
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
                JObjectWrapper signal = qtjambi_resolve_extra_signal(env, getJavaObjectLocalRef(env), method);
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

jobject PointerContainer::getExtraSignal(JNIEnv *, const QSharedPointerToQObjectLink*, const QMetaMethod&) const{
    return nullptr;
}

QString PointerToObjectLink::describe() const{
    QString strg = "[deletionPolicy=%1, createdByJava=%2, hasBeenCleaned=%3, isJavaObjectReleased=%4, isJavaLinkDetached=%5, isDeleteLater=%6, ownership=%7, metaType=%8, metaTypeId=%9]";
    strg = strg.arg("Normal");
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
    strg = strg.arg(QLatin1String(metaType().name()));
    strg = strg.arg(metaType().id());
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
    return strg;
}

jobject QSharedPointerToQObjectLink::getExtraSignal(JNIEnv * env, const QMetaMethod& method) const{
    return m_pointerContainer->getExtraSignal(env, this, method);
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

// ### END #################  QSharedPointerToQObjectLink  ################### END ###

PointerContainerWithExtraSignals::PointerContainerWithExtraSignals(JNIEnv* env, jobject jobj, const QMetaObject* metaObject, const QSharedPointer<QtJambiLink>& link, void* ptr_shared_pointer, bool isShell, PointerDeleter shared_pointer_deleter, PointerQObjectGetterFunction pointerGetter, JavaException& ocurredException)
    : PointerContainer(env, jobj, metaObject, link, ptr_shared_pointer, isShell, shared_pointer_deleter, pointerGetter, ocurredException)
{
}

jobject PointerContainerWithExtraSignals::getExtraSignal(JNIEnv * env, const QSharedPointerToQObjectLink* link, const QMetaMethod& method) const{
    if(QObject* object = qobject()){
        QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(object->metaObject());
        if(extraSignals.contains(method)){
            QReadLocker rlocker(QtJambiLinkUserData::lock());
            if(!m_extraSignals.contains(method.methodIndex())){
                rlocker.unlock();
                JObjectWrapper signal = qtjambi_resolve_extra_signal(env, link->getJavaObjectLocalRef(env), method);
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

PointerContainerWithPendingExtraSignals::PointerContainerWithPendingExtraSignals(JNIEnv* env, jobject jobj, const QMetaObject* metaObject, const QSharedPointer<QtJambiLink>& link, void* ptr_shared_pointer, bool isShell, PointerDeleter shared_pointer_deleter, PointerQObjectGetterFunction pointerGetter, JavaException& ocurredException)
    : PointerContainer(env, jobj, metaObject, link, ptr_shared_pointer, isShell, shared_pointer_deleter, pointerGetter, ocurredException),
      m_extraSignals(),
      m_metaObject(metaObject)
{

}

jobject PointerContainerWithPendingExtraSignals::getExtraSignal(JNIEnv * env, const QSharedPointerToQObjectLink* link, const QMetaMethod& method) const{
    if(QObject* object = qobject()){
        QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(object->metaObject());
        if(extraSignals.contains(method)){
            QReadLocker rlocker(QtJambiLinkUserData::lock());
            if(!m_extraSignals.contains(method.methodIndex())){
                rlocker.unlock();
                JObjectWrapper signal = qtjambi_resolve_extra_signal(env, link->getJavaObjectLocalRef(env), method);
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


PointerContainer::PointerContainer(JNIEnv* env, jobject jobj, const QMetaObject* metaObject, const QSharedPointer<QtJambiLink>& link, void* ptr_shared_pointer, bool isShell, PointerDeleter shared_pointer_deleter, PointerQObjectGetterFunction pointerGetter, JavaException& ocurredException):
    QObject(),
    m_ptr_shared_pointer(ptr_shared_pointer),
    m_isShell(isShell),
    m_shared_pointer_deleter(shared_pointer_deleter),
    m_shared_pointer_getter(pointerGetter),
    m_link(link.toWeakRef())
{
    if(!ocurredException.object()){
        qtjambi_resolve_signals(env, jobj, metaObject, ocurredException);
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
            m_shared_pointer_deleter(m_ptr_shared_pointer, m_isShell);
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

PointerToQObjectInterfaceWithExtraSignalsLink::PointerToQObjectInterfaceWithExtraSignalsLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces, const QMetaObject* metaObject, QObject *ptr, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException)
    : PointerToQObjectInterfaceLink(env, nativeLink, jobj, interfaceOffsets, interfaces, inheritedInterfaces, metaObject, ptr, created_by_java, isDeclarativeCall, is_shell, ocurredException)
{
}

PointerToQObjectInterfaceLink::PointerToQObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces, const QMetaObject* metaObject, QObject *ptr, bool created_by_java, bool isDeclarativeCall, bool is_shell, JavaException& ocurredException)
    : PointerToQObjectLink(env, nativeLink, jobj, metaObject, ptr, created_by_java, isDeclarativeCall, is_shell, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

PointerToObjectInterfaceLink::PointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                           LINK_NAME_ARG(const char *qt_name)
                                                           void *ptr, bool created_by_java, bool is_shell, JavaException& ocurredException)
    : PointerToObjectLink(env, nativeLink, jobj,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, is_shell, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

OwnedMetaTypedPointerToObjectInterfaceLink::OwnedMetaTypedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                           void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
    : OwnedMetaTypedPointerToObjectLink(env, nativeLink, jobj,
                          ptr, metaType, created_by_java, is_shell, ownerFunction, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

PointerToContainerInterfaceLink::PointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                           LINK_NAME_ARG(const char *qt_name)
                                                           void *ptr, bool created_by_java, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : PointerToContainerLink(env, nativeLink, jobj,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, containerAccess, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

MetaTypedPointerToContainerInterfaceLink::MetaTypedPointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                           void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : MetaTypedPointerToContainerLink(env, nativeLink, jobj,
                          ptr, metaType, created_by_java, is_shell, containerAccess, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

DeletablePointerToContainerInterfaceLink::DeletablePointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                           LINK_NAME_ARG(const char *qt_name)
                                                           void *ptr, bool created_by_java, PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : DeletablePointerToContainerLink(env, nativeLink, jobj,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, destructor_function, containerAccess, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

MetaTypedPointerToObjectInterfaceLink::MetaTypedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                           void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, JavaException& ocurredException)
    : MetaTypedPointerToObjectLink(env, nativeLink, jobj,
                          ptr, metaType, created_by_java, is_shell, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

DeletablePointerToObjectInterfaceLink::DeletablePointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                           LINK_NAME_ARG(const char *qt_name)
                                                           void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, JavaException& ocurredException)
    : DeletablePointerToObjectLink(env, nativeLink, jobj,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, is_shell, destructor_function, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

DeletableOwnedPointerToObjectInterfaceLink::DeletableOwnedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                           LINK_NAME_ARG(const char *qt_name)
                                                           void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
    : DeletableOwnedPointerToObjectLink(env, nativeLink, jobj,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, is_shell, destructor_function, ownerFunction, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

QSharedPointerToObjectInterfaceLink::QSharedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                                         LINK_NAME_ARG(const char *qt_name)
                                                                         bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, PointerGetterFunction pointerGetter, JavaException& ocurredException)
    : QSharedPointerToObjectLink(env, nativeLink, jobj,
                                 LINK_NAME_ARG(qt_name)
                                 created_by_java, is_shell, ownerFunction, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
    for(QMap<size_t, uint>::key_value_iterator i = m_interfaceOffsets.keyValueBegin(); i!=m_interfaceOffsets.keyValueEnd(); ++i){
        registerPointer(reinterpret_cast<void*>( quintptr(QSharedPointerToObjectLink::pointer()) + i.base().value() ));
    }
}

QSharedPointerToContainerInterfaceLink::QSharedPointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                                         LINK_NAME_ARG(const char *qt_name)
                                                                         bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, PointerGetterFunction pointerGetter, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : QSharedPointerToContainerLink(env, nativeLink, jobj,
                                          LINK_NAME_ARG(qt_name)
                                          created_by_java, is_shell, ownerFunction, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, containerAccess, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
    for(QMap<size_t, uint>::key_value_iterator i = m_interfaceOffsets.keyValueBegin(); i!=m_interfaceOffsets.keyValueEnd(); ++i){
        registerPointer(reinterpret_cast<void*>( quintptr(QSharedPointerToObjectLink::pointer()) + i.base().value() ));
    }
}

QSharedPointerToQObjectInterfaceLink::QSharedPointerToQObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces, QObject* object, bool created_by_java, bool is_shell, void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, PointerQObjectGetterFunction pointerGetter, const QMetaObject* superTypeForCustomMetaObject, JavaException& ocurredException)
    : QSharedPointerToQObjectLink(env, nativeLink, jobj, object, created_by_java, is_shell, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, superTypeForCustomMetaObject, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
    for(QMap<size_t, uint>::key_value_iterator i = m_interfaceOffsets.keyValueBegin(); i!=m_interfaceOffsets.keyValueEnd(); ++i){
        registerPointer(reinterpret_cast<void*>( quintptr(QSharedPointerToQObjectLink::pointer()) + i.base().value() ));
    }
}

#define INTERFACE_METHODS(LINKTYPE,LINKSUPERTYPE) \
LINKTYPE::~LINKTYPE()\
{\
    unregisterOffsets();\
    m_flags.setFlag(Flag::IsOffsetsUnregistered);\
}\
void LINKTYPE::unregisterOffsets() {\
    if(!m_flags.testFlag(Flag::IsOffsetsUnregistered)){\
        for(QMap<size_t, uint>::key_value_iterator i = m_interfaceOffsets.keyValueBegin(); i!=m_interfaceOffsets.keyValueEnd(); ++i){\
            unregisterPointer(reinterpret_cast<void*>( quintptr(LINKSUPERTYPE::pointer()) + i.base().value() ));\
        }\
    }\
}\
void LINKTYPE::addInterface(const std::type_info& qtInterfaceType){\
    m_availableInterfaces << unique_id(qtInterfaceType);\
    size_t offset = m_interfaceOffsets.value(unique_id(qtInterfaceType));\
    if(offset>0){\
        registerPointer(reinterpret_cast<void*>( quintptr(LINKSUPERTYPE::pointer()) + offset ));\
    }else{\
        for(const std::type_info* ifaces : m_inheritedInterfaces[unique_id(qtInterfaceType)]){\
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
            if(offset>0){\
                unregisterPointer(reinterpret_cast<void*>( quintptr(LINKSUPERTYPE::pointer()) + offset ));\
            }else{\
                setInDestructor();\
            }\
        }else{\
            for(const std::type_info* ifaces : m_inheritedInterfaces[unique_id(qtInterfaceType)]){\
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
        if(offset>0){\
            return reinterpret_cast<void*>( quintptr(LINKSUPERTYPE::pointer()) + offset );\
        }\
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
INTERFACE_METHODS(QSharedPointerToQObjectInterfaceLink, QSharedPointerToQObjectLink)
INTERFACE_METHODS(QSharedPointerToObjectInterfaceLink, QSharedPointerToObjectLink)
INTERFACE_METHODS(QSharedPointerToContainerInterfaceLink, QSharedPointerToContainerLink)
INTERFACE_METHODS(PointerToContainerInterfaceLink, PointerToContainerLink)
INTERFACE_METHODS(MetaTypedPointerToContainerInterfaceLink, MetaTypedPointerToContainerLink)
INTERFACE_METHODS(DeletablePointerToContainerInterfaceLink, DeletablePointerToContainerLink)


QObject* PointerContainer::qobject() const{
    return m_ptr_shared_pointer && m_shared_pointer_getter ? m_shared_pointer_getter(m_ptr_shared_pointer) : nullptr;
}

void qtjambi_set_java_ownership_for_toplevel_object(JNIEnv *env, QObject* qobject)
{
    if(qobject && !qobject->parent()){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qobject)){
            link->setJavaOwnership(env);
        }
    }
}

void qtjambi_set_default_ownership_for_toplevel_object(JNIEnv *env, QObject* qobject)
{
    if(qobject && !qobject->parent()){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qobject)){
            link->setDefaultOwnership(env);
        }
    }
}

void qtjambi_set_cpp_ownership_for_toplevel_object(JNIEnv *env, QObject* qobject)
{
    if(qobject && !qobject->parent()){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qobject)){
            link->setCppOwnership(env);
        }
    }
}

void qtjambi_set_java_ownership(JNIEnv *env, jobject object)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object)){
        link->setJavaOwnership(env);
    }
}

void qtjambi_set_default_ownership(JNIEnv *env, jobject object)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object)){
        link->setDefaultOwnership(env);
    }
}

void qtjambi_set_cpp_ownership(JNIEnv *env, jobject object)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object)){
        link->setCppOwnership(env);
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

bool qtjambi_is_java_ownership(JNIEnv *env, jobject object)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object)){
        return link->ownership()==QtJambiLink::Ownership::Java;
    }
    return false;
}

bool qtjambi_is_split_ownership(JNIEnv *env, jobject object)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object)){
        return link->ownership()==QtJambiLink::Ownership::Split;
    }
    return false;
}

bool qtjambi_is_cpp_ownership(JNIEnv *env, jobject object)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object)){
        return link->ownership()==QtJambiLink::Ownership::Cpp;
    }
    return false;
}

bool qtjambi_is_java_ownership(QtJambiNativeID objectId)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(objectId)){
        return link->ownership()==QtJambiLink::Ownership::Java;
    }
    return false;
}

bool qtjambi_is_split_ownership(QtJambiNativeID objectId)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(objectId)){
        return link->ownership()==QtJambiLink::Ownership::Split;
    }
    return false;
}

bool qtjambi_is_cpp_ownership(QtJambiNativeID objectId)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(objectId)){
        return link->ownership()==QtJambiLink::Ownership::Cpp;
    }
    return false;
}

void qtjambi_register_pointer_deletion(void* ptr)
{
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(ptr)){
        if(link && !link->isQObject() && !link->isShell()){
            if(JNIEnv* env = qtjambi_current_environment(!link->noThreadInitializationOnPurge())){
                QTJAMBI_JNI_LOCAL_FRAME(env, 100)
                link->invalidate(env);
            }else
                link->invalidate(nullptr);
        }
    }
}

QtJambiDestructor::QtJambiDestructor(const QPointer<const QObject>& parent, const QSharedPointer<QtJambiLink>& link, void *pointer, PtrDeleterFunction destructor_function, bool isShell)
    : QObject(),
      m_parent(parent),
      m_link(link.toWeakRef()),
      m_pointer(pointer),
      m_deleter_function(destructor_function),
      m_isShell(isShell)
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
     ,m_qtTypeName(link->qtTypeName())
#endif
{
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("new QtJambiDestructor()")
}

QtJambiDestructor::~QtJambiDestructor()
{
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("delete QtJambiDestructor")
}

bool QtJambiDestructor::event(QEvent * e){
    if(m_pointer != nullptr && e->type()==QEvent::Type(QEvent::DeferredDelete)){
        if(m_parent && m_parent->thread()!=this->thread()){
            this->moveToThread(m_parent->thread());
            this->deleteLater();
            return true;
        }
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("event DeferredDelete on QtJambiDestructor")
        QSharedPointer<QtJambiLink> link = m_link.toStrongRef();
        QTJAMBI_INCREASE_COUNTER(destructorFunctionCalledCount, link)
        m_deleter_function(m_pointer, m_isShell);
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

QtJambiMetaTypeDestructor::QtJambiMetaTypeDestructor(const QPointer<const QObject>& parent, const QSharedPointer<QtJambiLink>& link, void *pointer, const QMetaType& meta_type)
    : QObject(),
      m_parent(parent),
      m_link(link.toWeakRef()),
      m_pointer(pointer),
      m_meta_type(meta_type
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                  .id()
#endif
                  )
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
     ,m_qtTypeName(link->qtTypeName())
#endif
{
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("new QtJambiDestructor()")
}

QtJambiMetaTypeDestructor::~QtJambiMetaTypeDestructor()
{
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("delete QtJambiDestructor")
}

bool QtJambiMetaTypeDestructor::event(QEvent * e){
    if(m_pointer != nullptr && e->type()==QEvent::Type(QEvent::DeferredDelete)){
        if(m_parent && m_parent->thread()!=this->thread()){
            this->moveToThread(m_parent->thread());
            this->deleteLater();
            return true;
        }
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("event DeferredDelete on QtJambiDestructor")
        QSharedPointer<QtJambiLink> link = m_link.toStrongRef();
        m_meta_type.destroy(m_pointer);
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
const char* QtJambiDestructor::qtTypeName() const
{
    return m_qtTypeName;
}

const char* QtJambiMetaTypeDestructor::qtTypeName() const
{
    return m_qtTypeName;
}
#endif

jobject qtjambi_get_signal_types(JNIEnv *env, jobject signalField, const QMetaMethod& metaMethod);

void qtjambi_resolve_signals(JNIEnv *env, jobject java_object, const QMetaObject* metaObject, JavaException& ocurredException){
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
                if(const QVector<const SignalMetaInfo>* infos = signalMetaInfos(metaObject)){
                    QList<QString> signalOrder;
                    QMap<QString,QList<const SignalMetaInfo*> > sortedSignals;
                    for(const SignalMetaInfo& info : *infos){
                        QString key = QLatin1String(info.signal_name);
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
                                jclass clazz(nullptr);
                                if(const std::type_info* typeId = getTypeByMetaObject(metaObject)){
                                    if(const char* javaClass = getJavaName(*typeId))
                                        clazz = resolveClass(env, javaClass);
                                }
                                if(!clazz){
                                    clazz = Java::QtJambi::QtJambiInternal::findGeneratedSuperclass(env, env->GetObjectClass(java_object));
                                }
                                Q_ASSERT_X(clazz, "qtjambi_resolve_signals", qPrintable(QString("unable to find class").arg(metaObject->className())));

                                jfieldID fieldId = resolveField(env, info.signal_name, info.signal_signature, clazz);
                                Q_ASSERT_X(fieldId, "qtjambi_resolve_signals", qPrintable(QString("unable to find signal field %1").arg(info.signal_name)));

                                jobject signal = env->GetObjectField(java_object, fieldId);
                                qtjambi_throw_java_exception(env);
                                Q_ASSERT_X(signal, "qtjambi_resolve_signals", qPrintable(QString("unable to fetch signal field %1").arg(info.signal_name)));

                                if (signal) {  // everything went well
                                    QMetaMethod metaMethod = metaMethods.take(signal_method_index);
                                    jobject signalField = env->ToReflectedField(clazz, fieldId, false);
                                    qtjambi_throw_java_exception(env);
                                    jobject signalTypes = qtjambi_get_signal_types(env, signalField, metaMethod);
                                    Java::QtJambi::QtJambiSignals$AbstractSignal::initializeSignal(env, signal, clazz, signalTypes, signal_method_index, jlong(metaMethod.enclosingMetaObject()));
                                }
                                env->DeleteLocalRef(signal);
                            }
                        }else if(signalsByName.size()>1){
                            const SignalMetaInfo& first = *signalsByName.first();
                            jclass clazz(nullptr);
                            if(const std::type_info* typeId = getTypeByMetaObject(metaObject)){
                                if(const char* javaClass = getJavaName(*typeId))
                                    clazz = resolveClass(env, javaClass);
                            }
                            if(!clazz){
                                clazz = Java::QtJambi::QtJambiInternal::findGeneratedSuperclass(env, env->GetObjectClass(java_object));
                            }
                            Q_ASSERT_X(clazz, "qtjambi_resolve_signals", qPrintable(QString("unable to find class").arg(metaObject->className())));

                            jfieldID fieldId = resolveField(env, first.signal_name, qPrintable(QString("L%1$MultiSignal_%2;").arg(qtjambi_class_name(env, clazz).replace('.', '/')).arg(first.signal_name)), clazz);
                            if(!fieldId)
                                fieldId = resolveField(env, first.signal_name, "Lio/qt/core/QObject$MultiSignal;", clazz);
                            qtjambi_throw_java_exception(env);
                            Q_ASSERT_X(fieldId, "qtjambi_resolve_signals", qPrintable(QString("unable to find signal field %1").arg(first.signal_name)));

                            jobject reflectedField = env->ToReflectedField(clazz, fieldId, false);

                            jobject multiSignal = env->GetObjectField(java_object, fieldId);
                            qtjambi_throw_java_exception(env);
                            Q_ASSERT_X(multiSignal, "qtjambi_resolve_signals", qPrintable(QString("unable to fetch signal field %1").arg(first.signal_name)));

                            jintArray methodIndexes = env->NewIntArray(jsize(signalsByName.size()));
                            jlongArray metaObjects = env->NewLongArray(jsize(signalsByName.size()));
                            jobjectArray signalParameterTypes = env->NewObjectArray(jsize(signalsByName.size()), Java::Runtime::List::getClass(env), nullptr);
                            jobjectArray signalObjectTypes = env->NewObjectArray(jsize(signalsByName.size()), Java::Runtime::Class::getClass(env), nullptr);
                            if(signalsByName.size()>0){
                                JIntArrayPointer methodIndexesPtr(env, methodIndexes, true);
                                JLongArrayPointer metaObjectsPtr(env, metaObjects, true);
                                for(int i=0; i<signalsByName.size(); i++){
                                    const SignalMetaInfo& info = *signalsByName.at(i);
                                    int signal_method_index = -1;
                                    if(info.signal_method_index_provider){
                                        signal_method_index = info.signal_method_index_provider();
                                    }
                                    QString signalSignature = QLatin1String(info.signal_signature);
                                    if(signalSignature.startsWith('L') && signalSignature.endsWith(';'))
                                        signalSignature = signalSignature.mid(1, signalSignature.length()-2);
                                    env->SetObjectArrayElement(signalObjectTypes, jsize(i), resolveClass(env, qPrintable(signalSignature), nullptr));
                                    QMetaMethod metaMethod = metaMethods.take(signal_method_index);
                                    const QList<ParameterTypeInfo>& parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, metaMethod);
                                    jobjectArray signalParameterClasses = env->NewObjectArray(jsize(parameterTypeInfos.size()-1), Java::Runtime::Class::getClass(env), nullptr);
                                    for (decltype(parameterTypeInfos.size()) i=1; i<parameterTypeInfos.size(); ++i){
                                        env->SetObjectArrayElement(signalParameterClasses, jsize(i-1), parameterTypeInfos[i].javaClass());
                                    }
                                    env->SetObjectArrayElement(signalParameterTypes, jsize(i), qtjambi_get_signal_types(env, Java::QtCore::QPair::newInstance(env, multiSignal, signalParameterClasses), metaMethod));
                                    methodIndexesPtr.pointer()[i] = signal_method_index;
                                    metaObjectsPtr.pointer()[i] = jlong(metaMethod.enclosingMetaObject());
                                }
                            }
                            Java::QtJambi::QtJambiSignals$AbstractMultiSignal::initializeSignals(env, multiSignal, reflectedField, methodIndexes, metaObjects, signalParameterTypes, signalObjectTypes);
                            env->DeleteLocalRef(reflectedField);
                        }else{
                            Q_ASSERT_X(false, "qtjambi_resolve_signals", "should never reach this");
                        }
                    }
                }
                metaObject = metaObject->superClass();
            }while(metaObject);

            if(!metaMethods.isEmpty()){
                for(QMap<int,QMetaMethod>::const_iterator i = metaMethods.cbegin(); i!=metaMethods.cend(); ++i){
                    const QMetaMethod& metaMethod = i.value();
                    metaObject = metaMethod.enclosingMetaObject();
                    if (const QtJambiMetaObject* mo = QtJambiMetaObject::cast(metaObject)){
                        jclass java_class = mo->javaClass();
                        jobject signalField = Java::Runtime::Class::getDeclaredField(env, java_class, env->NewStringUTF(metaMethod.name().data()));
                        if(signalField){
                            jfieldID signalFieldId = env->FromReflectedField(signalField);
                            jobject signalTypes = mo->signalTypes(metaMethod.methodIndex()-metaObject->methodOffset());
                            jobject signal = env->GetObjectField(java_object, signalFieldId);
                            qtjambi_throw_java_exception(env);
                            if(signal){
                                if(!signalTypes){
                                    signalTypes = qtjambi_get_signal_types(env, signalField, metaMethod);
                                }
                                Java::QtJambi::QtJambiSignals$AbstractSignal::initializeSignal(env, signal, signalTypes, metaMethod.methodIndex(), jlong(mo));
                            }
                        }
                    }
                }
            }
        }catch(const JavaException& exn){
            ocurredException.addSuppressed(env, exn);
        }
    }
}

JObjectWrapper qtjambi_resolve_extra_signal(JNIEnv *env, jobject java_object, const QMetaMethod& method){
    QSet<const QMetaObject*> convertedMetaObjects;
    jobject signalObject = nullptr;
    switch(method.parameterCount()){
    case 0:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal0::newInstance(env, java_object);
        break;
    case 1:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal1::newInstance(env, java_object);
        break;
    case 2:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal2::newInstance(env, java_object);
        break;
    case 3:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal3::newInstance(env, java_object);
        break;
    case 4:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal4::newInstance(env, java_object);
        break;
    case 5:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal5::newInstance(env, java_object);
        break;
    case 6:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal6::newInstance(env, java_object);
        break;
    case 7:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal7::newInstance(env, java_object);
        break;
    case 8:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal8::newInstance(env, java_object);
        break;
    case 9:
        signalObject = Java::QtJambi::QInstanceMemberSignals$Signal9::newInstance(env, java_object);
        break;
    default:
        break;
    }
    if(signalObject){
        {
            const QMetaObject* parentMetaObject = method.enclosingMetaObject();
            while(parentMetaObject && !convertedMetaObjects.contains(parentMetaObject)){
                qtjambi_cast<jobject>(env, parentMetaObject);
                convertedMetaObjects.insert(parentMetaObject);
                parentMetaObject = parentMetaObject->superClass();
            }
        }
        jobject signalTypes = qtjambi_get_signal_types(env, nullptr, method);
        Java::QtJambi::QtJambiSignals$AbstractSignal::initializeSignal(env, signalObject, env->GetObjectClass(java_object), signalTypes, method.methodIndex(), jlong(method.enclosingMetaObject()));
        return JObjectWrapper(env, signalObject, true);
    }else{
        return JObjectWrapper();
    }
}
