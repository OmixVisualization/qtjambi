/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

namespace DebugAPI{
void printArgs(const char *file, int line, const char *function, const char *format,...);
}

bool operator !(QtJambiNativeID nativeId) { return nativeId == InvalidNativeID; }

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

// #define DEBUG_REFCOUNTING

#ifdef QTJAMBI_NO_METHOD_TRACE
#define QTJAMBI_DEBUG_PRINT_WITH_ARGS(...)
#define QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(string)
#else
#define QTJAMBI_DEBUG_PRINT_WITH_ARGS(...)\
if(!isDebugMessagingDisabled())\
    DebugAPI::printArgs(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME)
#define QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(methodname)\
DebugAPI::MethodPrintWithType __debug_method_print(isDebugMessagingDisabled() ? DebugAPI::MethodPrint::Disabled : DebugAPI::MethodPrint::Internal, methodname, this, qtTypeName(), __FILE__, __LINE__, __FUNCTION__);
#else
#define QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(methodname)\
DebugAPI::MethodPrint __debug_method_print(isDebugMessagingDisabled() ? DebugAPI::MethodPrint::Disabled : DebugAPI::MethodPrint::Internal, this, methodname, __FILE__, __LINE__, __FUNCTION__);
#endif

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
class SmartPointerToObjectLink;
class QSharedPointerToQObjectLink;

Q_GLOBAL_STATIC(const QSharedPointer<QtJambiLink>, gDefaultPointer);

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
            QString jclassName = obj ? QtJambiAPI::getObjectClassName(env, obj) : QLatin1String("null");
            jlong native_link = _link->m_nativeLink ? Java::QtJambi::NativeUtility$NativeLink::native__id(env, _link->m_nativeLink) : 0;
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
            QTJAMBI_INCREASE_COUNTER(userDataDestroyedCount, link)
            if(link->isSmartPointer()){
                reinterpret_cast<QSharedPointerToQObjectLink*>(link.data())->setAsQObjectDeleted();
            }else{
                reinterpret_cast<PointerToQObjectLink*>(link.data())->setAsQObjectDeleted();
            }
            // only reset the link when not a shell. Shells do this operation during shell destruction.
            // => QtJambiShellImpl::deleteShell()
            if(!link->isShell()){
                if(link->noThreadInitializationOnPurge()){
                    if(JniEnvironment env{200})
                        link->invalidate(env);
                }else{
                    if(DefaultJniEnvironment env{200})
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

void ValueOwnerUserData::addDeleter(const std::function<void()>& deleter){
    m_deleters << deleter;
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
    if(java && Java::QtJambi::QtObjectInterface::isInstanceOf(env, java)){
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
    if(ocurredException.object()){
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
    if(ocurredException.object()){
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
    QtJambiLink* qtJambiLink = new QSharedPointerToQObjectInterfaceLink(env, nativeLink, javaObject, offsets, interfaces, inheritedInterfaces, object, created_by_java, is_shell, ptr_shared_pointer, pointerDeleter, pointerGetter, superTypeForCustomMetaObject, ocurredException);
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
    QtJambiLink* qtJambiLink = new QSharedPointerToQObjectLink(env, nativeLink, javaObject, object, created_by_java, is_shell, ptr_shared_pointer, pointerDeleter, pointerGetter, superTypeForCustomMetaObject, ocurredException);
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
    QtJambiLink* qtJambiLink = new ExtendedSmartPointerToObjectLink(env, nativeLink, javaObject,
                                               LINK_NAME_ARG(qt_name)
                                               created_by_java, is_shell, extension, ownerFunction, ptr_shared_pointer, pointerDeleter, pointerGetter, ocurredException);
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
    QtJambiLink* qtJambiLink = new SmartPointerToObjectInterfaceLink(env, nativeLink, javaObject, offsets, interfaces, inheritedInterfaces,
                                                            LINK_NAME_ARG(qt_name)
                                                            created_by_java, is_shell, ownerFunction, ptr_shared_pointer, pointerDeleter, pointerGetter, ocurredException);
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
    QtJambiLink* qtJambiLink = new SmartPointerToObjectLink(env, nativeLink, javaObject,
                                                 LINK_NAME_ARG(qt_name)
                                                 created_by_java, is_shell, ownerFunction, ptr_shared_pointer, pointerDeleter, pointerGetter, ocurredException);
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
    QtJambiLink* qtJambiLink = new QSharedPointerToContainerInterfaceLink(env, nativeLink, javaObject, offsets, interfaces, inheritedInterfaces,
                                                             LINK_NAME_ARG(qt_name)
                                                             false, false, nullptr, ptr_shared_pointer, pointerDeleter, pointerGetter, containerAccess, ocurredException);
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
    QtJambiLink* qtJambiLink = new QSharedPointerToContainerLink(env, nativeLink, javaObject,
                                                    LINK_NAME_ARG(qt_name)
                                                    false, false, nullptr, ptr_shared_pointer, pointerDeleter, pointerGetter, containerAccess, ocurredException);
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
    QtJambiLink* qtJambiLink = new MetaTypedPointerToContainerLink(env, nativeLink, javaObject,
                                                            ptr, metaType, created_by_java, false,
                                                            containerAccess, ocurredException);
    if(ocurredException.object()){
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
    qtJambiLink = new MetaTypedPointerToContainerInterfaceLink(env, nativeLink, javaObject,
                                                                 interfaceOffsets, interfaceTypes,
                                                                 inheritedInterfaces,
                                                                 ptr, metaType, created_by_java, false,
                                                                 containerAccess, ocurredException);
    if(ocurredException.object()){
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
    QtJambiLink* qtJambiLink = new DeletablePointerToContainerLink(env, nativeLink, javaObject,
                                                            LINK_NAME_ARG(qt_name)
                                                            ptr, created_by_java, false,
                                                            destructor_function, containerAccess, ocurredException);
    if(ocurredException.object()){
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
    QtJambiLink* qtJambiLink = new DeletablePointerToContainerInterfaceLink(env, nativeLink, javaObject,
                                                                     interfaceOffsets, interfaceTypes,
                                                                     inheritedInterfaces,
                                                                     LINK_NAME_ARG(qt_name)
                                                                     ptr, created_by_java, false,
                                                                     destructor_function, containerAccess, ocurredException);
    if(ocurredException.object()){
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
    QtJambiLink* qtJambiLink = new OwnedMetaTypedPointerToObjectLink(env, nativeLink, javaObject,
                                                                     ptr, metaType, created_by_java,
                                                                     is_shell, ownerFunction, ocurredException);
    if(ocurredException.object()){
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
    QtJambiLink* qtJambiLink = new OwnedMetaTypedPointerToObjectInterfaceLink(env, nativeLink, javaObject,
                                                                              interfaceOffsets, interfaceTypes,
                                                                              inheritedInterfaces,
                                                                              ptr, metaType, created_by_java,
                                                                              is_shell, ownerFunction, ocurredException);
    if(ocurredException.object()){
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
    QtJambiLink* qtJambiLink = new MetaTypedPointerToObjectLink(env, nativeLink, javaObject,
                                                                ptr, metaType, created_by_java, is_shell, ocurredException);
    if(ocurredException.object()){
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
    QtJambiLink* qtJambiLink = new MetaTypedPointerToObjectInterfaceLink(env, nativeLink, javaObject, interfaceOffsets, interfaceTypes, inheritedInterfaces,
                                                                         ptr, metaType, created_by_java, is_shell, ocurredException);
    if(ocurredException.object()){
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
    QtJambiLink* qtJambiLink = new OwnedDependentMetaTypedPointerToObjectLink(env, nativeLink, javaObject,
                                                                              ptr, metaType, created_by_java,
                                                                              is_shell, ownerFunction, ocurredException);
    if(ocurredException.object()){
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
    QtJambiLink* qtJambiLink = new OwnedDependentMetaTypedPointerToObjectInterfaceLink(env, nativeLink, javaObject,
                                                                                       interfaceOffsets, interfaceTypes,
                                                                                       inheritedInterfaces,
                                                                                       ptr, metaType, created_by_java,
                                                                                       is_shell, ownerFunction, ocurredException);
    if(ocurredException.object()){
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
    QtJambiLink* qtJambiLink = new DependentDeletablePointerToObjectLink(env, nativeLink, javaObject,
                                                                         LINK_NAME_ARG(qt_name)
                                                                         ptr, created_by_java, is_shell, destructor_function, ocurredException);
    if(ocurredException.object()){
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
    QtJambiLink* qtJambiLink = new ExtendedDeletablePointerToObjectLink(env, nativeLink, javaObject,
                                                                         LINK_NAME_ARG(qt_name)
                                                                         ptr, created_by_java, is_shell, destructor_function, extension, ocurredException);
    if(ocurredException.object()){
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
    QtJambiLink* qtJambiLink = new DependentMetaTypedPointerToObjectLink(env, nativeLink, javaObject,
                                                                         ptr, metaType, created_by_java, is_shell, ocurredException);
    if(ocurredException.object()){
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
    QtJambiLink* qtJambiLink = new DependentMetaTypedPointerToObjectInterfaceLink(env, nativeLink, javaObject,
                                                                                  interfaceOffsets, interfaceTypes, inheritedInterfaces,
                                                                                  ptr, metaType, created_by_java, is_shell, ocurredException);
    if(ocurredException.object()){
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
    QtJambiLink* qtJambiLink = new DeletableOwnedPointerToObjectLink(env, nativeLink, javaObject,
                                                                     LINK_NAME_ARG(qt_name)
                                                                     ptr, created_by_java,
                                                                     is_shell, deleter_function,
                                                                     ownerFunction, ocurredException);
    if(ocurredException.object()){
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
    QtJambiLink* qtJambiLink = new DeletableOwnedPointerToObjectInterfaceLink(env, nativeLink, javaObject,
                                                                              interfaceOffsets,
                                                                              interfaceTypes,
                                                                              inheritedInterfaces,
                                                                              LINK_NAME_ARG(qt_name)
                                                                              ptr, created_by_java,
                                                                              is_shell, deleter_function,
                                                                              ownerFunction, ocurredException);
    if(ocurredException.object()){
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
    QtJambiLink* qtJambiLink = new DeletablePointerToObjectLink(env, nativeLink, javaObject,
                                                                LINK_NAME_ARG(qt_name)
                                                                ptr, created_by_java,
                                                                is_shell, deleter_function, ocurredException);
    if(ocurredException.object()){
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
    QtJambiLink* qtJambiLink = new DeletablePointerToObjectInterfaceLink(env, nativeLink, javaObject,
                                                                interfaceOffsets,
                                                                interfaceTypes,
                                                                inheritedInterfaces,
                                                                LINK_NAME_ARG(qt_name)
                                                                ptr, created_by_java,
                                                                is_shell, deleter_function, ocurredException);
    if(ocurredException.object()){
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
    QtJambiLink* qtJambiLink = new PointerToObjectLink(env, nativeLink, javaObject,
                                              LINK_NAME_ARG(qt_name)
                                              ptr, created_by_java, is_shell, ocurredException);
    if(ocurredException.object()){
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
    QtJambiLink* qtJambiLink = new PointerToObjectInterfaceLink(env, nativeLink, javaObject,
                                                       interfaceOffsets,
                                                       interfaceTypes,
                                                       inheritedInterfaces,
                                                       LINK_NAME_ARG(qt_name)
                                                       ptr, created_by_java, is_shell, ocurredException);
    if(ocurredException.object()){
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewContainer(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, JavaException& ocurredException)
{
    if(!ocurredException.object()){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);
        Q_ASSERT(containerAccess);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        QtJambiLink* qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            qtJambiLink = new DeletablePointerToContainerInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
                                                                       LINK_NAME_ARG(getQtName(typeId))
                                                                       ptr, created_by_java, is_shell, destructor_function, containerAccess, ocurredException);
        }else{
            qtJambiLink = new DeletablePointerToContainerLink(env, nativeLink, javaObject,
                                                              LINK_NAME_ARG(getQtName(typeId))
                                                              ptr, created_by_java, is_shell, destructor_function, containerAccess, ocurredException);
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

const QSharedPointer<QtJambiLink>& QtJambiLink::createLinkForNewContainer(JNIEnv *env, jclass objectClass, jobject nativeLink, jobject javaObject, const std::type_info& typeId, void *ptr, const SuperTypeInfos* superTypeInfos, bool created_by_java, bool is_shell, AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
{
    if(!ocurredException.object()){
        Q_ASSERT(env);
        Q_ASSERT(javaObject);
        Q_ASSERT(ptr);
        Q_ASSERT(containerAccess);

        // Initialize the link
        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, objectClass, typeId, superTypeInfos);
        QtJambiLink* qtJambiLink;
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            qtJambiLink = new DeletableOwnedPointerToContainerInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
                                                                       LINK_NAME_ARG(getQtName(typeId))
                                                                       ptr, created_by_java, is_shell, destructor_function, ownerFunction, containerAccess, ocurredException);
        }else{
            qtJambiLink = new DeletableOwnedPointerToContainerLink(env, nativeLink, javaObject,
                                                              LINK_NAME_ARG(getQtName(typeId))
                                                              ptr, created_by_java, is_shell, destructor_function, ownerFunction, containerAccess, ocurredException);
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
        qtJambiLink = new PointerToContainerInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
                                                       LINK_NAME_ARG(qt_name)
                                                       ptr, false, false, containerAccess, ocurredException);
    }else{
        qtJambiLink = new PointerToContainerLink(env, nativeLink, javaObject,
                                              LINK_NAME_ARG(qt_name)
                                              ptr, false, false, containerAccess, ocurredException);
    }
    if(ocurredException.object()){
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
        qtJambiLink = new DeletablePointerToContainerInterfaceLink(env, nativeLink, javaObject, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces,
                                                       LINK_NAME_ARG(qt_name)
                                                       ptr, false, false, destructor_function, containerAccess, ocurredException);
    }else{
        qtJambiLink = new DeletablePointerToContainerLink(env, nativeLink, javaObject,
                                              LINK_NAME_ARG(qt_name)
                                              ptr, false, false, destructor_function, containerAccess, ocurredException);
    }
    if(ocurredException.object()){
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
        if(jobject nativeLink = Java::QtJambi::NativeUtility::findInterfaceLink(env, java, true, true)){
            JObjectSynchronizer sync(env, nativeLink);
            Q_UNUSED(sync)
            QtJambiNativeID nativeId = QtJambiNativeID(Java::QtJambi::NativeUtility$NativeLink::native__id(env, nativeLink));
            return fromNativeId(nativeId);
        }
    }
    return *gDefaultPointer;
}

const QSharedPointer<QtJambiLink>& QtJambiLink::findLinkForJavaObject(JNIEnv *env, jobject java)
{
    if (env->IsSameObject(nullptr, java))
        return *gDefaultPointer;
    if(jobject nativeLink = Java::QtJambi::NativeUtility$Object::nativeLink(env, java)){
        JObjectSynchronizer sync(env, nativeLink);
        Q_UNUSED(sync)
        QtJambiNativeID nativeId = QtJambiNativeID(Java::QtJambi::NativeUtility$NativeLink::native__id(env, nativeLink));
        return fromNativeId(nativeId);
    }
    return *gDefaultPointer;
}

void* QtJambiLink::findPointerForJavaInterface(JNIEnv *env, jobject java, const std::type_info& typeId)
{
    if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java)){
        if(jobject nativeLink = Java::QtJambi::NativeUtility::findInterfaceLink(env, java, true, true)){
            JObjectSynchronizer sync(env, nativeLink);
            Q_UNUSED(sync)
            QtJambiNativeID nativeId = QtJambiNativeID(Java::QtJambi::NativeUtility$NativeLink::native__id(env, nativeLink));
            if(QtJambiLink * link = reinterpret_cast<QtJambiLink *>(nativeId)){
                if(link->isMultiInheritanceType()){
                    if(link->isInterfaceAvailable(typeId))
                        return link->typedPointer(typeId);
                    else return nullptr;
                }else return link->pointer();
            }
        }
    }else if(env->IsSameObject(nullptr, java))
        return nullptr;
    Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, java).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    return nullptr;
}

void* QtJambiLink::findPointerForJavaObject(JNIEnv *env, jobject java)
{
    if (env->IsSameObject(nullptr, java))
        return nullptr;
    else if(jobject nativeLink = Java::QtJambi::NativeUtility$Object::nativeLink(env, java)){
        JObjectSynchronizer sync(env, nativeLink);
        Q_UNUSED(sync)
        QtJambiNativeID nativeId = QtJambiNativeID(Java::QtJambi::NativeUtility$NativeLink::native__id(env, nativeLink));
        if(QtJambiLink * link = reinterpret_cast<QtJambiLink *>(nativeId))
            return link->pointer();
    }
    Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, java).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    return nullptr;
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
        Java::QtJambi::NativeUtility$NativeLink::set_native__id(env, m_nativeLink, jlong(reinterpret_cast<void*>(this)));
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
    if(m_nativeLink || m_java.object){
        if(JniEnvironment env = noThreadInitializationOnPurge() ? JniEnvironment{300} : DefaultJniEnvironment{300}){
            if(m_nativeLink){
                try{
                    Java::QtJambi::NativeUtility$NativeLink::reset(env, m_nativeLink);
                } catch (const JavaException& exn) {
                    exn.report(env);
                }
                env->DeleteGlobalRef(m_nativeLink);
                m_nativeLink = nullptr;
            }
            if(m_java.object){
                if (m_flags.testFlag(Flag::GlobalReference)){
                    env->DeleteGlobalRef(m_java.object);
                }else{
                    env->DeleteWeakGlobalRef(m_java.weak);
                }
                m_java.object = nullptr;
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
    if(QtJambiLink::getObjectCacheMode() != 0 && link && link->pointer()) {
        QWriteLocker locker(gLinkAccessLock());
        Q_UNUSED(locker)
        Q_ASSERT(gUserObjectCache());
        QWeakPointer<QtJambiLink> wlink = link.toWeakRef();
        if(!gUserObjectCache->contains(link->pointer(), wlink))
            gUserObjectCache->insert(link->pointer(), wlink);
    }
}

void QtJambiLink::registerPointer(void *ptr) {
    if(QtJambiLink::getObjectCacheMode() != 0) {
        QWriteLocker locker(gLinkAccessLock());
        Q_UNUSED(locker)
        Q_ASSERT(gUserObjectCache());
        QWeakPointer<QtJambiLink> wlink = m_this.toWeakRef();
        if(!gUserObjectCache->contains(ptr, wlink))
            gUserObjectCache->insert(ptr, wlink);
    }
}

void QtJambiLink::unregisterPointer(void *ptr) {
    if(QtJambiLink::getObjectCacheMode() != 0) {
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
                    void *ptr, bool created_by_java, bool is_shell,
                    AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : PointerToObjectLink(env, nativeLink, jobj,
                                                           LINK_NAME_ARG(qt_name)
                                                           ptr, created_by_java, is_shell, ocurredException), m_containerAccess(containerAccess) {}

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
            qCDebug(DebugAPI::internalCategory, "Deleting QObject [count after: %d]", currentCount);
        }
    #endif
        invalidateDependentObjects(env);
        unregisterPointer(m_pointer);
        unregisterOffsets();
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
                            QtJambiMetaTypeDestructor* destructor = new QtJambiMetaTypeDestructor(vud->pointer(), m_this, pointer, m_meta_type);
                            destructor->moveToThread(ownerThread->thread());
                            destructor->deleteLater();
                        }
                        pointer = nullptr;
                    }else if(!p->wasDeleted){
                        QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor for object %p of type %s", pointer, static_cast<const char*>(m_meta_type.name()))
                        setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                        QtJambiMetaTypeDestructor* destructor = new QtJambiMetaTypeDestructor(obj, m_this, pointer, m_meta_type);
                        destructor->moveToThread(ownerThread->thread());
                        destructor->deleteLater();
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
                    void *ptr, bool created_by_java, bool is_shell,
                    PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : DeletablePointerToObjectLink(env, nativeLink, jobj,
                                                           LINK_NAME_ARG(qt_name)
                                                           ptr, created_by_java, is_shell, destructor_function, ocurredException), m_containerAccess(containerAccess) {}

DeletablePointerToContainerLink::~DeletablePointerToContainerLink(){
    m_containerAccess->dispose();
}

DeletableOwnedPointerToContainerLink::DeletableOwnedPointerToContainerLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                    LINK_NAME_ARG(const char* qt_name)
                    void *ptr, bool created_by_java, bool is_shell,
                    PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : DeletableOwnedPointerToObjectLink(env, nativeLink, jobj,
                                                           LINK_NAME_ARG(qt_name)
                                                           ptr, created_by_java, is_shell, destructor_function, ownerFunction, ocurredException), m_containerAccess(containerAccess) {}

DeletableOwnedPointerToContainerLink::~DeletableOwnedPointerToContainerLink(){
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
        if(JniEnvironment env = noThreadInitializationOnPurge() ? JniEnvironment{300} : DefaultJniEnvironment{300}){
            releaseJavaObject(env);
            detachJavaLink(env);
            deleteNativeObject(env);
        }else{
            releaseJavaObject(nullptr);
            detachJavaLink(nullptr);
            deleteNativeObject(nullptr);
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
    QList<std::function<void(JNIEnv*)>> finalizations;
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
    QList<std::function<void(JNIEnv*)>> finalizations;
    {
        QWriteLocker locker(QtJambiLinkUserData::lock());
        dependentObjects.swap(m_dependentObjects);
        finalizations.swap(m_finalizations);
    }
    clear(env, dependentObjects, finalizations);
}

void DependencyManagerUserData::clear(JNIEnv* env, QSet<QWeakPointer<QtJambiLink>>& dependentObjects, QList<std::function<void(JNIEnv*)>>& finalizations){
    for(const QWeakPointer<QtJambiLink>& weakLink : qAsConst(dependentObjects)){
        if(QSharedPointer<QtJambiLink> link = weakLink.toStrongRef()){
            link->invalidate(env);
        }
    }
    dependentObjects.clear();
    for(const std::function<void(JNIEnv*)>& f : qAsConst(finalizations)){
        f(env);
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

void DependencyManagerUserData::addFinalization(std::function<void(JNIEnv*)>&& finalization){
    m_finalizations << std::move(finalization);
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
typedef QMultiHash<QSharedPointer<QtJambiLink>, std::function<void(JNIEnv*)>> FinalizationHash;
Q_GLOBAL_STATIC(FinalizationHash, gFinalizations)

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

void QtJambiLink::addFinalization(const QObject* object, std::function<void(JNIEnv*)>&& finalization){
    if(object){
        if(DependencyManagerUserData* dm = DependencyManagerUserData::instance(object)){
            dm->addFinalization(std::move(finalization));
        }
    }
}

void QtJambiLink::addFinalization(std::function<void(JNIEnv*)>&& finalization){
    if(isQObject()){
        addFinalization(qobject(), std::move(finalization));
    }else{
        if(!m_flags.testFlag(Flag::HasDependencies))
            m_flags.setFlag(Flag::HasDependencies);
        QWriteLocker locker(gDependencyLock());
        Q_UNUSED(locker)
        gFinalizations->insert(m_this, std::move(finalization));
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
    if(isQObject()){
        if(QObject* obj = qobject()){
            if(DependencyManagerUserData* dm = DependencyManagerUserData::instance(obj, false)){
                dm->clear(env);
            }
        }
    }else{
        if(m_flags.testFlag(Flag::HasDependencies)){
            QList<QWeakPointer<QtJambiLink>> dependentObjects;
            QList<std::function<void(JNIEnv*)>> finalizations;
            {
                QWriteLocker locker(gDependencyLock());
                Q_UNUSED(locker)
                dependentObjects = gDependencies->values(m_this);
                gDependencies->remove(m_this);
                finalizations = gFinalizations->values(m_this);
                gFinalizations->remove(m_this);
            }
            for(const QWeakPointer<QtJambiLink>& weakLink : qAsConst(dependentObjects)){
                if(QSharedPointer<QtJambiLink> link = weakLink.toStrongRef()){
                    link->invalidate(env);
                }
            }
            for(const std::function<void(JNIEnv*)>& f : qAsConst(finalizations)){
                f(env);
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
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("QtJambiLink::invalidate(JNIEnv *)")
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
            qCDebug(DebugAPI::internalCategory, "Deleting QObject [count after: %d]", currentCount);
        }
    #endif
        invalidateDependentObjects(env);
        unregisterPointer(m_pointer);
        unregisterOffsets();
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
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("MetaTypedPointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
    #ifdef DEBUG_REFCOUNTING
        {
            QWriteLocker locker(gRefCountStaticLock());
            int currentCount = gRefCountHash()->value(m_meta_type, 0) - 1;
            gRefCountHash()->insert(m_meta_type, currentCount);
            qCDebug(DebugAPI::internalCategory, "Deleting QObject [count after: %d]", currentCount);
        }
    #endif
        invalidateDependentObjects(env);
        unregisterPointer(m_pointer);
        unregisterOffsets();
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
                            QtJambiMetaTypeDestructor* destructor = new QtJambiMetaTypeDestructor(vud->pointer(), m_this, pointer, m_meta_type);
                            destructor->moveToThread(ownerThread->thread());
                            destructor->deleteLater();
                        }
                        pointer = nullptr;
                    }else if(!p->wasDeleted){
                        QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor for object %p of type %s", pointer, static_cast<const char*>(m_meta_type.name()))
                        setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                        QtJambiMetaTypeDestructor* destructor = new QtJambiMetaTypeDestructor(obj, m_this, pointer, m_meta_type);
                        destructor->moveToThread(ownerThread->thread());
                        destructor->deleteLater();
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
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("DeletablePointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
    #ifdef DEBUG_REFCOUNTING
        {
            QWriteLocker locker(gRefCountStaticLock());
            int currentCount = gRefCountHash()->value(m_meta_type, 0) - 1;
            gRefCountHash()->insert(m_meta_type, currentCount);
            qCDebug(DebugAPI::internalCategory, "Deleting QObject [count after: %d]", currentCount);
        }
    #endif
        invalidateDependentObjects(env);
        unregisterPointer(m_pointer);
        unregisterOffsets();
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
                            QtJambiDestructor* destructor = new QtJambiDestructor(vud->pointer(), m_this, pointer, m_deleter_function, isShell());
                            destructor->moveToThread(ownerThread->thread());
                            destructor->deleteLater();
                        }
                        pointer = nullptr;
                    }else if(!p->wasDeleted){
                        QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor for object %p", pointer)
                        setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                        QtJambiDestructor* destructor = new QtJambiDestructor(obj, m_this, pointer, m_deleter_function, isShell());
                        destructor->moveToThread(ownerThread->thread());
                        destructor->deleteLater();
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

void DeletablePointerToContainerLink::deleteNativeObject(JNIEnv *env, bool forced)
{
    if (m_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("DeletablePointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
    #ifdef DEBUG_REFCOUNTING
        {
            QWriteLocker locker(gRefCountStaticLock());
            int currentCount = gRefCountHash()->value(m_meta_type, 0) - 1;
            gRefCountHash()->insert(m_meta_type, currentCount);
            qCDebug(DebugAPI::internalCategory, "Deleting QObject [count after: %d]", currentCount);
        }
    #endif
        invalidateDependentObjects(env);
        unregisterPointer(m_pointer);
        unregisterOffsets();
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
                            QtJambiDestructor* destructor = new QtJambiDestructor(vud->pointer(), m_this, pointer, m_deleter_function, isShell());
                            destructor->moveToThread(ownerThread->thread());
                            destructor->deleteLater();
                        }
                        pointer = nullptr;
                    }else if(!p->wasDeleted){
                        QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor for object %p", pointer)
                        setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                        QtJambiDestructor* destructor = new QtJambiDestructor(obj, m_this, pointer, m_deleter_function, isShell());
                        destructor->moveToThread(ownerThread->thread());
                        destructor->deleteLater();
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
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("DeletablePointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
    #ifdef DEBUG_REFCOUNTING
        {
            QWriteLocker locker(gRefCountStaticLock());
            int currentCount = gRefCountHash()->value(m_meta_type, 0) - 1;
            gRefCountHash()->insert(m_meta_type, currentCount);
            qCDebug(DebugAPI::internalCategory, "Deleting QObject [count after: %d]", currentCount);
        }
    #endif
        invalidateDependentObjects(env);
        unregisterPointer(m_pointer);
        unregisterOffsets();
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
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("DeletablePointerToObjectLink::deleteNativeObject(JNIEnv *, bool)")
    #ifdef DEBUG_REFCOUNTING
        {
            QWriteLocker locker(gRefCountStaticLock());
            int currentCount = gRefCountHash()->value(m_meta_type, 0) - 1;
            gRefCountHash()->insert(m_meta_type, currentCount);
            qCDebug(DebugAPI::internalCategory, "Deleting QObject [count after: %d]", currentCount);
        }
    #endif
        invalidateDependentObjects(env);
        unregisterPointer(m_pointer);
        unregisterOffsets();
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
                            QtJambiDestructor* destructor = new QtJambiDestructor(vud->pointer(), m_this, pointer, m_deleter_function, isShell());
                            destructor->moveToThread(ownerThread->thread());
                            destructor->deleteLater();
                        }
                        pointer = nullptr;
                    }else if(!p->wasDeleted){
                        QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor for object %p", pointer)
                        setDeleteLater();    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                        QtJambiDestructor* destructor = new QtJambiDestructor(obj, m_this, pointer, m_deleter_function, isShell());
                        destructor->moveToThread(ownerThread->thread());
                        destructor->deleteLater();
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
    QtJambiLink(env, nativeLink, jobj,
                LINK_NAME_ARG(metaObject->className())
                created_by_java, isDeclarativeCall, is_shell, ocurredException),
    m_pointer(object)
{
    QtJambiMetaObject::resolveSignals(env, jobj, metaObject, ocurredException);
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
        if(JniEnvironment env = noThreadInitializationOnPurge() ? JniEnvironment{300} : DefaultJniEnvironment{300}){
            releaseJavaObject(env);
            detachJavaLink(env);
            deleteNativeObject(env);
        }else{
            releaseJavaObject(nullptr);
            detachJavaLink(nullptr);
            deleteNativeObject(nullptr);
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
            qCDebug(DebugAPI::internalCategory, "Deleting '%s' [count after: %d]", QMetaType::typeName(m_meta_type), currentCount);
        }
#endif
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
                        delete m_pointer;
                        __qt_exceptionBlocker.release(env);
                    }
                }else if (currentThread == objectThread || !objectThread->isRunning() || objectThread==m_pointer) {
                    QTJAMBI_DEBUG_PRINT_WITH_ARGS("call delete on object %p of type %s", m_pointer, typeid(*m_pointer).name())
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
                            QTJAMBI_DEBUG_PRINT_WITH_ARGS("call deleteLater() on object %p of type %s", m_pointer, typeid(*m_pointer).name())
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
                            qCWarning(DebugAPI::internalCategory, "Trying to delete a running QThread '%s'",
                                    qPrintable(m_pointer->objectName()));
                        }
                        QtJambiExceptionBlocker __qt_exceptionBlocker;
                        delete m_pointer;
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
                    QTJAMBI_DEBUG_PRINT_WITH_ARGS("call deleteLater() on object %p of type %s", m_pointer, typeid(*m_pointer).name())
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
                    QTJAMBI_DEBUG_PRINT_WITH_ARGS("attach QObject to thread shutdown procedure.")
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
                        qCWarning(DebugAPI::internalCategory, "QObjects can only be implicitly garbage collected when owned"
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

// ### BEGIN ####################  SmartPointerLink  ####################### BEGIN ####

SmartPointerLink::SmartPointerLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                                       LINK_NAME_ARG(const char *qt_name)
                                       bool created_by_java, bool is_shell, JavaException& ocurredException)
    : QtJambiLink(env, nativeLink, jobj,
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

ExtendedSmartPointerToObjectLink::ExtendedSmartPointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                                 LINK_NAME_ARG(const char* qt_name)
                                 bool created_by_java, bool is_shell, const QObject* extension,
                                 PtrOwnerFunction ownerFunction, void* ptr_shared_pointer,
                                 SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException)
    : SmartPointerToObjectLink(env, nativeLink, jobj,
                               LINK_NAME_ARG(qt_name)
                               created_by_java, is_shell,
                               ownerFunction, ptr_shared_pointer,
                               shared_pointer_deleter, pointerGetter,
                               ocurredException), ExtendedLink(), m_extension(extension){}

// ### BEGIN #################  SmartPointerToObjectLink  ################### BEGIN ###

QSharedPointerToContainerLink::QSharedPointerToContainerLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                                                             LINK_NAME_ARG(const char* qt_name)
                                                             bool created_by_java, bool is_shell,
                                                             PtrOwnerFunction ownerFunction, void* ptr_shared_pointer,
                                                             SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter,
                                                             AbstractContainerAccess* containerAccess, JavaException& ocurredException) :
    SmartPointerToObjectLink(env, nativeLink, jobj,
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

SmartPointerToObjectLink::SmartPointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                                                       LINK_NAME_ARG(const char *qt_name)
                                                       bool created_by_java, bool is_shell,
                                                       PtrOwnerFunction ownerFunction, void* ptr_shared_pointer,
                                                       SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException) :
    SmartPointerLink(env, nativeLink, jobj,
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

SmartPointerToObjectLink::~SmartPointerToObjectLink(){
    try{
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("SmartPointerToObjectLink::~SmartPointerToObjectLink()")
        if(JniEnvironment env = noThreadInitializationOnPurge() ? JniEnvironment{300} : DefaultJniEnvironment{300}){
            releaseJavaObject(env);
            detachJavaLink(env);
            deleteNativeObject(env);
        }else{
            releaseJavaObject(nullptr);
            detachJavaLink(nullptr);
            deleteNativeObject(nullptr);
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
void *SmartPointerToObjectLink::pointer() const { return m_ptr_shared_pointer ? m_shared_pointer_getter(m_ptr_shared_pointer) : nullptr; }

QObject *SmartPointerToObjectLink::qobject() const { Q_ASSERT_X(false, "QtJambiLink", "Pointer is not QObject"); return nullptr; }

void* SmartPointerToObjectLink::getSmartPointer() const{
    return m_ptr_shared_pointer;
}

void SmartPointerToObjectLink::invalidate(JNIEnv *env) {
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("QtJambiLink::resetObject(JNIEnv *)")
    invalidateDependentObjects(env);
    releaseJavaObject(env);
    detachJavaLink(env);
    dispose();
}

void SmartPointerToObjectLink::deleteNativeObject(JNIEnv *env, bool){
    if(m_ptr_shared_pointer){
        QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME("SmartPointerToObjectLink::deleteNativeObject(JNIEnv *env, bool)")
    #ifdef DEBUG_REFCOUNTING
        {
            QWriteLocker locker(gRefCountStaticLock());
            int currentCount = gRefCountHash()->value(m_meta_type, 0) - 1;
            gRefCountHash()->insert(m_meta_type, currentCount);
            qCDebug(DebugAPI::internalCategory, "Deleting '%s' [count after: %d]", QMetaType::typeName(m_meta_type), currentCount);
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
                        QTJAMBI_DEBUG_PRINT_WITH_ARGS("use QtJambiDestructor (calling shared pointer deleter function)")
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
void SmartPointerToObjectLink::onDispose(JNIEnv *env)
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

// #### END ##################  SmartPointerToObjectLink  ##################### END ####

// ### BEGIN #################  QSharedPointerToQObjectLink  ################### BEGIN ###

QSharedPointerToQObjectLink::QSharedPointerToQObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj, QObject* object, bool created_by_java, bool is_shell, void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, const QMetaObject* superTypeForCustomMetaObject, JavaException& ocurredException)
    : SmartPointerLink(env, nativeLink, jobj,
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
        if(JniEnvironment env = noThreadInitializationOnPurge() ? JniEnvironment{300} : DefaultJniEnvironment{300}){
            releaseJavaObject(env);
            detachJavaLink(env);
            deleteNativeObject(env);
        }else{
            releaseJavaObject(nullptr);
            detachJavaLink(nullptr);
            deleteNativeObject(nullptr);
        }
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
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

void* QSharedPointerToQObjectLink::getSmartPointer() const{
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
                QTJAMBI_DEBUG_PRINT_WITH_ARGS("call delete PointerContainer")
                if(objectThread && m_pointerContainer->qobject()==objectThread){
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
                delete m_pointerContainer;
                m_pointerContainer = nullptr;
            // We're in the main thread and we'll have an event loop
            // running, so its safe to call delete later.
            } else if (QAbstractEventDispatcher::instance(objectThread)) {
    //             printf(" - deleteLater in main thread %s [%s]\n",
    //                    qPrintable(qobj->objectName()),
    //                    qobj->metaObject()->className());
                setDeleteLater();
                QTJAMBI_DEBUG_PRINT_WITH_ARGS("call PointerContainer::deleteLater()")
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
                QTJAMBI_DEBUG_PRINT_WITH_ARGS("attach QObject to thread shutdown procedure.")
                qthreadData->deleteAtThreadEnd(m_pointerContainer);
            } else if(objectThread){
                if(qobject_cast<QThread*>(m_pointerContainer->qobject())){
                    delete m_pointerContainer;
                    m_pointerContainer = nullptr;
                }else{
                    qCWarning(DebugAPI::internalCategory, "QObjects can only be implicitly garbage collected when owned"
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

jobject PointerContainer::getExtraSignal(JNIEnv *, const QSharedPointerToQObjectLink*, const QMetaMethod&) const{
    return nullptr;
}

QString PointerToObjectLink::describe() const{
    QString strg = "[deletionPolicy=%1, createdByJava=%2, hasBeenCleaned=%3, isJavaObjectReleased=%4, isJavaLinkDetached=%5, isDeleteLater=%6, ownership=%7, metaType=%8, metaTypeId=%9]";
    strg = strg.arg("Normal");
    strg = strg
            .arg(createdByJava() ? "true" : "false",
                    m_flags.testFlag(Flag::HasBeenCleaned) ? "true" : "false",
                    m_java.object==nullptr ? "true" : "false",
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
    strg += QString("createdByJava=%1, hasBeenCleaned=%2, isJavaObjectReleased=%3, isJavaLinkDetached=%4, ownership=%5")
            .arg(createdByJava() ? "true" : "false",
                     m_flags.testFlag(Flag::HasBeenCleaned) ? "true" : "false",
                     m_java.object==nullptr ? "true" : "false",
                     m_flags.testFlag(Flag::IsJavaLinkDetached) ? "true" : "false");
    if(m_flags.testFlag(Flag::JavaOwnership)) strg = strg.arg("Java");
    else if(m_flags.testFlag(Flag::CppOwnership)) strg = strg.arg("Cpp");
    else if(m_flags.testFlag(Flag::SplitOwnership)) strg = strg.arg("Split");
    else strg = strg.arg("InvalidOwnership");
    return QString("[%1]").arg(strg);
}

QString SmartPointerToObjectLink::describe() const{
    QString strg = "[deletionPolicy=%1, createdByJava=%2, hasBeenCleaned=%3, linkRemoved=%4, objectInvalid=%5, pointerZeroed=%6, ownership=%7, metaType=%8, metaTypeId=%9]";
    if(m_owner_function) strg = strg.arg("InSpecificThread");
    else strg = strg.arg("Normal");
    strg = strg
            .arg(createdByJava() ? "true" : "false",
                    m_flags.testFlag(Flag::HasBeenCleaned) ? "true" : "false",
                    m_java.object==nullptr ? "true" : "false",
                    m_flags.testFlag(Flag::IsJavaLinkDetached) ? "true" : "false",
                    isPointerZeroed() ? "true" : "false");
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
        strg += QString("class=%1, objectName=%2, ").arg(QLatin1String(qobject()->metaObject()->className()), qobject()->objectName());
    }
    strg += QString("createdByJava=%1, hasBeenCleaned=%2, linkRemoved=%3, objectInvalid=%4, ownership=%6")
            .arg(createdByJava() ? "true" : "false",
                     m_flags.testFlag(Flag::HasBeenCleaned) ? "true" : "false",
                     m_java.object==nullptr ? "true" : "false",
                     m_flags.testFlag(Flag::IsJavaLinkDetached) ? "true" : "false");
    if(m_flags.testFlag(Flag::JavaOwnership)) strg = strg.arg("JavaOwnership");
    else if(m_flags.testFlag(Flag::CppOwnership)) strg = strg.arg("CppOwnership");
    else if(m_flags.testFlag(Flag::SplitOwnership)) strg = strg.arg("SplitOwnership");
    else strg = strg.arg("InvalidOwnership");
    return QString("[%1]").arg(strg);
}

// ### END #################  QSharedPointerToQObjectLink  ################### END ###

PointerContainerWithExtraSignals::PointerContainerWithExtraSignals(JNIEnv* env, jobject jobj, const QMetaObject* metaObject, const QSharedPointer<QtJambiLink>& link, void* ptr_shared_pointer, bool isShell, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException)
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
                JObjectWrapper signal = QtJambiMetaObject::resolveExtraSignal(env, link->getJavaObjectLocalRef(env), method);
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

PointerContainerWithPendingExtraSignals::PointerContainerWithPendingExtraSignals(JNIEnv* env, jobject jobj, const QMetaObject* metaObject, const QSharedPointer<QtJambiLink>& link, void* ptr_shared_pointer, bool isShell, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException)
    : PointerContainer(env, jobj, metaObject, link, ptr_shared_pointer, isShell, shared_pointer_deleter, pointerGetter, ocurredException),
      m_extraSignals()
{

}

jobject PointerContainerWithPendingExtraSignals::getExtraSignal(JNIEnv * env, const QSharedPointerToQObjectLink* link, const QMetaMethod& method) const{
    if(QObject* object = qobject()){
        QList<QMetaMethod> extraSignals = getExtraSignalsOfMetaObject(object->metaObject());
        if(extraSignals.contains(method)){
            QReadLocker rlocker(QtJambiLinkUserData::lock());
            if(!m_extraSignals.contains(method.methodIndex())){
                rlocker.unlock();
                JObjectWrapper signal = QtJambiMetaObject::resolveExtraSignal(env, link->getJavaObjectLocalRef(env), method);
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


PointerContainer::PointerContainer(JNIEnv* env, jobject jobj, const QMetaObject* metaObject, const QSharedPointer<QtJambiLink>& link, void* ptr_shared_pointer, bool isShell, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException):
    QObject(),
    m_ptr_shared_pointer(ptr_shared_pointer),
    m_isShell(isShell),
    m_shared_pointer_deleter(shared_pointer_deleter),
    m_shared_pointer_getter(pointerGetter),
    m_link(link.toWeakRef())
{
    if(!ocurredException.object()){
        QtJambiMetaObject::resolveSignals(env, jobj, metaObject, ocurredException);
        QObject* object = qobject();
        this->setObjectName(QString("PointerContainer-for-%1").arg(object->objectName()));
        if(object){
            if(object->thread())
                this->moveToThread(object->thread());
        }
    }
}

PointerContainer::~PointerContainer(){
    try{
        QSharedPointer<QtJambiLink> link = m_link.toStrongRef();
        QSharedPointerToQObjectLink* _link = reinterpret_cast<QSharedPointerToQObjectLink*>(link.data());
        if(m_ptr_shared_pointer){
            QTJAMBI_DEBUG_PRINT_WITH_ARGS("calling shared pointer deleter function")
            QTJAMBI_INCREASE_COUNTER(pointerContainerDestroyedCount, _link)
            m_shared_pointer_deleter(m_ptr_shared_pointer, m_isShell);
        }
        if(_link){
            _link->removePointerContainer();
        }
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
    } catch (...) {
    }
}

bool PointerContainer::isDebugMessagingDisabled()const{
    if(QSharedPointer<QtJambiLink> link = m_link.toStrongRef()){
        return link->isDebugMessagingDisabled();
    }
    return true;
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
                                                           void *ptr, bool created_by_java, bool is_shell, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : PointerToContainerLink(env, nativeLink, jobj,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, is_shell, containerAccess, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
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
                                                           void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : DeletablePointerToContainerLink(env, nativeLink, jobj,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, is_shell, destructor_function, containerAccess, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
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

DeletableOwnedPointerToContainerInterfaceLink::DeletableOwnedPointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                           LINK_NAME_ARG(const char *qt_name)
                                                           void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : DeletableOwnedPointerToContainerLink(env, nativeLink, jobj,
                          LINK_NAME_ARG(qt_name)
                          ptr, created_by_java, is_shell, destructor_function, ownerFunction, containerAccess, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
}

SmartPointerToObjectInterfaceLink::SmartPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                                         LINK_NAME_ARG(const char *qt_name)
                                                                         bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, JavaException& ocurredException)
    : SmartPointerToObjectLink(env, nativeLink, jobj,
                                 LINK_NAME_ARG(qt_name)
                                 created_by_java, is_shell, ownerFunction, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
    for(QMap<size_t, uint>::key_value_iterator i = m_interfaceOffsets.keyValueBegin(); i!=m_interfaceOffsets.keyValueEnd(); ++i){
        registerPointer(reinterpret_cast<void*>( quintptr(SmartPointerToObjectLink::pointer()) + i.base().value() ));
    }
}

QSharedPointerToContainerInterfaceLink::QSharedPointerToContainerInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                                                         LINK_NAME_ARG(const char *qt_name)
                                                                         bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, AbstractContainerAccess* containerAccess, JavaException& ocurredException)
    : QSharedPointerToContainerLink(env, nativeLink, jobj,
                                          LINK_NAME_ARG(qt_name)
                                          created_by_java, is_shell, ownerFunction, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, containerAccess, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
    for(QMap<size_t, uint>::key_value_iterator i = m_interfaceOffsets.keyValueBegin(); i!=m_interfaceOffsets.keyValueEnd(); ++i){
        registerPointer(reinterpret_cast<void*>( quintptr(SmartPointerToObjectLink::pointer()) + i.base().value() ));
    }
}

QSharedPointerToQObjectInterfaceLink::QSharedPointerToQObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces, QObject* object, bool created_by_java, bool is_shell, void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter, const QMetaObject* superTypeForCustomMetaObject, JavaException& ocurredException)
    : QSharedPointerToQObjectLink(env, nativeLink, jobj, object, created_by_java, is_shell, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, superTypeForCustomMetaObject, ocurredException), m_interfaceOffsets(interfaceOffsets), m_availableInterfaces(interfaces), m_inheritedInterfaces(inheritedInterfaces)
{
    for(QMap<size_t, uint>::key_value_iterator i = m_interfaceOffsets.keyValueBegin(); i!=m_interfaceOffsets.keyValueEnd(); ++i){
        registerPointer(reinterpret_cast<void*>( quintptr(QSharedPointerToQObjectLink::pointer()) + i.base().value() ));
    }
}

ExtendedLink::~ExtendedLink(){}

ExtendedDeletablePointerToObjectLink::ExtendedDeletablePointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                                                                             LINK_NAME_ARG(const char* qt_name)
                                                                             void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function,
                                                                             const QObject* extension, JavaException& ocurredException)
    : DeletablePointerToObjectLink(env, nativeLink, jobj,
                                   LINK_NAME_ARG(qt_name)
                                   ptr, created_by_java, is_shell, destructor_function, ocurredException),
    ExtendedLink(),
    m_extension(extension)
{}

DependentDeletablePointerToObjectLink::DependentDeletablePointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                                                           LINK_NAME_ARG(const char* qt_name)
                                                           void *ptr, bool created_by_java, bool is_shell, PtrDeleterFunction destructor_function, JavaException& ocurredException)
    : DeletablePointerToObjectLink(env, nativeLink, jobj,
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

DependentMetaTypedPointerToObjectLink::DependentMetaTypedPointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                                                                             void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, JavaException& ocurredException)
    : MetaTypedPointerToObjectLink(env, nativeLink, jobj, ptr, metaType, created_by_java, is_shell, ocurredException)
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

OwnedDependentMetaTypedPointerToObjectLink::OwnedDependentMetaTypedPointerToObjectLink(JNIEnv *env, jobject nativeLink, jobject jobj,
                    void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell,
                    PtrOwnerFunction ownerFunction, JavaException& ocurredException)
    : OwnedMetaTypedPointerToObjectLink(env, nativeLink, jobj,
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

DependentMetaTypedPointerToObjectInterfaceLink::DependentMetaTypedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                               void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, JavaException& ocurredException)
    : MetaTypedPointerToObjectInterfaceLink(env, nativeLink, jobj, interfaceOffsets, interfaces, inheritedInterfaces,
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

OwnedDependentMetaTypedPointerToObjectInterfaceLink::OwnedDependentMetaTypedPointerToObjectInterfaceLink(JNIEnv *env, jobject nativeLink, jobject jobj, const QMap<size_t, uint>& interfaceOffsets, const QSet<size_t>& interfaces, const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces,
                                               void *ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
    : OwnedMetaTypedPointerToObjectInterfaceLink(env, nativeLink, jobj, interfaceOffsets, interfaces, inheritedInterfaces,
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
INTERFACE_METHODS(QSharedPointerToQObjectInterfaceLink, QSharedPointerToQObjectLink)
INTERFACE_METHODS(SmartPointerToObjectInterfaceLink, SmartPointerToObjectLink)
INTERFACE_METHODS(QSharedPointerToContainerInterfaceLink, QSharedPointerToContainerLink)
INTERFACE_METHODS(PointerToContainerInterfaceLink, PointerToContainerLink)
INTERFACE_METHODS(MetaTypedPointerToContainerInterfaceLink, MetaTypedPointerToContainerLink)
INTERFACE_METHODS(DeletablePointerToContainerInterfaceLink, DeletablePointerToContainerLink)
INTERFACE_METHODS(DeletableOwnedPointerToContainerInterfaceLink, DeletableOwnedPointerToContainerLink)


QObject* PointerContainer::qobject() const{
    return m_ptr_shared_pointer && m_shared_pointer_getter ? reinterpret_cast<QObject*>(m_shared_pointer_getter(m_ptr_shared_pointer)) : nullptr;
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
            JniEnvironment env{200};
            link->invalidate(env);
        }
        // continue to delete
    }
    return QObject::event(e);
}

bool QtJambiDestructor::isDebugMessagingDisabled() const{
    if(QSharedPointer<QtJambiLink> link = m_link.toStrongRef()){
        return link->isDebugMessagingDisabled();
    }
    return true;
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

bool QtJambiMetaTypeDestructor::isDebugMessagingDisabled() const{
    if(QSharedPointer<QtJambiLink> link = m_link.toStrongRef()){
        return link->isDebugMessagingDisabled();
    }
    return true;
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
            JniEnvironment env{200};
            link->invalidate(env);
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
