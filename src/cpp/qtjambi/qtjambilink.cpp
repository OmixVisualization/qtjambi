/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "qtjambilink.h"

#include "qtjambi_cache.h"
#include "qtjambi_core.h"
#include "qtjambitypemanager_p.h"
#include "qtjambidestructorevent_p.h"

#include <QDebug>
#include <QHash>
#include <QReadWriteLock>
#include <QThread>
#include <QWriteLocker>
#include <QCoreApplication>

#if defined(QTJAMBI_DEBUG_TOOLS)
#  include "qtjambidebugtools_p.h"
#  include <QStringList>
#endif

// #define DEBUG_REFCOUNTING

#ifdef QT_NO_DEBUG
#define QTJAMBI_DEBUG_TRACE_WITH_THREAD(location)
#else
#define QTJAMBI_DEBUG_TRACE_WITH_THREAD(location)\
QTJAMBI_DEBUG_TRACE(qPrintable(QString("[THREAD 0x%1] %2").arg(QString::number((qint64)(qintptr)QThread::currentThreadId(), 16)).arg(location)))
#endif

#if defined(QTJAMBI_DEBUG_TOOLS)
/* static */ QtJambiLink *QtJambiLink::QtJambiLinkList_head;
/* static */ QtJambiLink *QtJambiLink::QtJambiLinkList_tail;
Q_GLOBAL_STATIC(QReadWriteLock, gStaticQtJambiLinkListLock);

void QtJambiLink::QtJambiLinkList_add()
{
    if(m_in_qtjambilink_list) {
        char xb[24];
        fprintf(stderr, "QtJambiLink(%p %s)::QtJambiLinkList_add() ERROR m_in_qtjambilink_list=%d\n", this, debugFlagsToString(xb), m_in_qtjambilink_list);
        return;
    }

    {
        QWriteLocker lock(gStaticQtJambiLinkListLock());

        next = 0;
        prev = QtJambiLink::QtJambiLinkList_tail;
        if(prev)
            prev->next = this;
        else
            QtJambiLink::QtJambiLinkList_head = this;
        QtJambiLink::QtJambiLinkList_tail = this;

        m_in_qtjambilink_list = true;
    }
}

void QtJambiLink::QtJambiLinkList_remove()
{
    if(!m_in_qtjambilink_list) {
        char xb[24];
        fprintf(stderr, "QtJambiLink(%p %s)::QtJambiLinkList_remove() ERROR m_in_qtjambilink_list=%d\n", this, debugFlagsToString(xb), m_in_qtjambilink_list);
        return;
    }

    {
        QWriteLocker lock(gStaticQtJambiLinkListLock());

        if(prev)
            prev->next = next;
        else
            QtJambiLink::QtJambiLinkList_head = next;
        if(next)
            next->prev = prev;
        else
            QtJambiLink::QtJambiLinkList_tail = prev;
        next = 0;
        prev = 0;

        m_in_qtjambilink_list = false;
    }
}

/* static */
int QtJambiLink::QtJambiLinkList_count()
{
    int count = 0;
    {
        QReadLocker lock(gStaticQtJambiLinkListLock());
        QtJambiLink *link = QtJambiLink::QtJambiLinkList_head;
        while(link) {
            count++;
            link = link->next;
        }
    }
    return count;
}

/* static */
int QtJambiLink::QtJambiLinkList_dump()
{
    char xb[24];
    int count = 0;
    {
        QReadLocker lock(gStaticQtJambiLinkListLock());
        QtJambiLink *link = QtJambiLink::QtJambiLinkList_head;
        while(link) {
            fprintf(stderr, "QtJambiLink(%p %s) ALIVE: { java_object=%p, global_ref=%d, is_qobject=%d, pointer=%p, delete_later=%d, pointer_zeroed=%d (%s) }\n",
                link, link->debugFlagsToString(xb), link->m_java_object, link->m_global_ref, link->m_is_qobject, link->m_pointer, link->m_delete_later, link->m_pointer_zeroed,
                qPrintable(link->m_className.isNull() ? QString("<unknown>") : link->m_className));
            count++;
            link = link->next;
        }
        fflush(stderr);
    }
    return count;
}

/* static */
bool QtJambiLink::QtJambiLinkList_check(QtJambiLink *find)
{
    bool found = false;
    if(find) {
        QReadLocker lock(gStaticQtJambiLinkListLock());
        QtJambiLink *link = QtJambiLink::QtJambiLinkList_head;
        while(link) {
            if(find == link) {
                found = true;
                break;
            }
            link = link->next;
        }
    }
    if(found == false)
        fprintf(stderr, "QtJambiLink::QtJambiLinkList_check(%p): FAILED\n", find);
    return found;
}

// used for memory checking
Q_GLOBAL_STATIC_WITH_ARGS(QMutex, g_magicLock, (QMutex::Recursive));

#endif

typedef QHash<const void *, QtJambiLink *> LinkHash;
// gStaticUserDataIdLock only protects allocation of qtjambi_user_data_id
Q_GLOBAL_STATIC(QReadWriteLock, gStaticUserDataIdLock);
// gUserObjectCacheLock only protects all access of gUserObjectCache
Q_GLOBAL_STATIC(QReadWriteLock, gUserObjectCacheLock);
Q_GLOBAL_STATIC(LinkHash, gUserObjectCache);

// FIXME: Make this atomicly read from memory, assist startup
//  by providing callbacks for each module loading and call
//  QtJambiLinkUserData::id() to cause initialization.
int qtjambi_user_data_id = -1;


static int user_data_id()
{
  {
    QReadLocker read_lock(gStaticUserDataIdLock());
    if (qtjambi_user_data_id >= 0)
      return qtjambi_user_data_id;
  }

  {
    QWriteLocker lock(gStaticUserDataIdLock());
    if (qtjambi_user_data_id == -1)
      qtjambi_user_data_id = QObject::registerUserData();
    return qtjambi_user_data_id;
  }

}

int QtJambiLinkUserData::id()
{
    return user_data_id();
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
        gUserObjectCache()->clear();
    }
    return 0;
}

int qtjambi_object_count_get(int arg0, int arg1)
{
    Q_UNUSED(arg0);
    Q_UNUSED(arg1);
    // QObject
    // QtJambiLink
    // QtJambiLinkUserData
    // QtJambiDestructorEvent
    // JObject
#if defined(QTJAMBI_DEBUG_TOOLS)
    if(arg0 == 1)
        fflush(stdout);
    else if(arg0 == 2)
        fflush(stderr);
    else if(arg0 == 3)  // FIXME: Throw NotImplemented exception
        return QtJambiLink::QtJambiLinkList_dump();
#endif
    return -1;
}

int qtjambi_object_cache_operation_count()
{
    int count;
    {
        QWriteLocker lock(gUserObjectCacheLock());
        count = gUserObjectCache()->count();
    }
    return count;
}

void QtJambiLink::registerSubObject(void *ptr) {
    QWriteLocker locker(gUserObjectCacheLock());
    Q_ASSERT(gUserObjectCache());
    gUserObjectCache()->insert(ptr, this);
}

void QtJambiLink::unregisterSubObject(void *ptr) {
    QWriteLocker locker(gUserObjectCacheLock());
    gUserObjectCache()->remove(ptr);
}

const char *QtJambiLink::debugFlagsToString(char *buf) const {
    char *s = buf;
    if(m_global_ref)
        *s++ = ((m_java_object)            ? 'J' : '.');
    else
        *s++ = ((m_java_weak)              ? 'W' : '.');
    *s++ = ((m_pointer)                ? 'P' : '.');
    *s++ = ((char)((int)m_ownership + '0'));
    *s++ = ((char)((int)m_last_ownership + '0'));
    *s++ = ((m_global_ref)             ? 'G' : '.');
    *s++ = ((m_is_qobject)             ? 'Q' : '.');
    *s++ = ((m_has_been_finalized)     ? 'F' : '.');
    *s++ = ((m_qobject_deleted)        ? 'D' : '.');
    *s++ = ((m_created_by_java)        ? 'C' : '.');
    *s++ = ((m_object_invalid)         ? 'I' : '.');
    *s++ = ((m_in_cache)               ? 'c' : '.');
    *s++ = ((m_connected_to_java)      ? 'j' : '.');
    *s++ = ((m_delete_in_main_thread)  ? 'd' : '.');
    *s++ = ((m_java_link_removed)      ? 'r' : '.');
    *s++ = ((m_link_cacheable)         ? 'c' : '.');
    *s++ = ((m_delete_later)           ? 'L' : '.');
    *s++ = ((m_pointer_zeroed)         ? '0' : '.');
    *s++ = ((m_pointer_zapped)         ? 'Z' : '.');
    *s++ = ((m_user_data_skip)         ? 'U' : '.');
    *s = 0;
    return buf;
}

QtJambiLink *QtJambiLink::createLinkForQObject(JNIEnv *env, jobject java, QObject *object, void* ptr_shared_pointer, PointerDeleter pointerDeleter)
{
    Q_ASSERT(env);
    Q_ASSERT(java);
    Q_ASSERT(object);

    // Initialize the link
    jweak weakGlobalRef = env->NewWeakGlobalRef(java);
    // QtJambiLink(jobject jobj, bool global_ref, bool is_qobject, void *pointer)
    QtJambiLink *link;
    if(ptr_shared_pointer && pointerDeleter){
        PointerContainer* pointerContainer = new PointerContainer(ptr_shared_pointer, pointerDeleter, object);
        link = new QtPointerJambiLink(weakGlobalRef, false, object, pointerContainer);
        link->setJavaOwnership(env, weakGlobalRef);
    }else{
        link = new QtJambiLink(weakGlobalRef, false, true, object);
    }

#if defined(QTJAMBI_DEBUG_TOOLS)
    link->m_className = QString::fromLatin1(object->metaObject()->className());
    qtjambi_increase_linkConstructedCount(link->m_className);
#endif

    // Fetch the user data id
    QtJambiLinkUserData *qjlud = new QtJambiLinkUserData(link);
    object->setUserData(user_data_id(), qjlud);
#if defined(QTJAMBI_DEBUG_TOOLS)
    link->m_qtJambiLinkUserData = qjlud;
#endif

    // Set the native__id field of the java object
    StaticCache *sc = StaticCache::instance();
    sc->resolveQtJambiObject();
    // We continue to use the hard local reference to java object in preference to the weak one we just created
    env->SetLongField(java, sc->QtJambiObject.native_id, reinterpret_cast<jlong>(link));
    // FIXME: I have no idea why we created a weakGlobalRef to then immediately upgrade it to a hardGlobalRef
    link->setCppOwnership(env, java);

    return link;
}


QtJambiLink *QtJambiLink::createWrapperForQObject(JNIEnv *env, QObject *object, const char *class_name,
                                                const char *package_name, void* ptr_shared_pointer, PointerDeleter pointerDeleter)
{
    Q_ASSERT(!object->userData(user_data_id()));

    jclass object_class = resolveClass(env, class_name, package_name);
    if (object_class == 0) {
        qWarning("createWrapperForQObject(), failed to resolve class %s.%s\n",
                 package_name, class_name);
        return 0;
    }

    jmethodID constructorId = resolveMethod(env, "<init>", "(Lorg/qtjambi/qt/internal/QtJambiObject$QPrivateConstructor;)V",
        class_name, package_name, false);
    Q_ASSERT(constructorId);

    jobject java_object = env->NewObject(object_class, constructorId, 0);
    QtJambiLink *link = createLinkForQObject(env, java_object, object, ptr_shared_pointer, pointerDeleter);
    link->setMetaObject(object->metaObject());
    return link;
}


QtJambiLink *QtJambiLink::createLinkForObject(JNIEnv *env, jobject java, void *ptr, const QString &java_name,
                                              bool enter_in_cache, void* ptr_shared_pointer, PointerDeleter pointerDeleter)
{
    Q_ASSERT(env);
    Q_ASSERT(java);
    Q_ASSERT(ptr);

    // Initialize the link
    jweak weakGlobalRef = env->NewWeakGlobalRef(java);
    // QtJambiLink(jobject jobj, bool global_ref, bool is_qobject, void *pointer)
    QtJambiLink *link;
    if(ptr_shared_pointer && pointerDeleter){
        link = new QtPointerJambiLink(weakGlobalRef, false, ptr, ptr_shared_pointer, pointerDeleter);
        link->setJavaOwnership(env, weakGlobalRef);
    }else{
        link = new QtJambiLink(weakGlobalRef, false, false, ptr);
    }

    link->m_destructor_function = java_name.isEmpty() ? 0 : deleter(java_name);

#if defined(QTJAMBI_DEBUG_TOOLS)
    link->m_className = java_name.indexOf("/") >= 0 ? java_name.split("/").last() : java_name;
    qtjambi_increase_linkConstructedCount(link->m_className);
#endif

    // If the object is created by Java, then we have control over its destructor, which means
    // we can cache the pointer. Otherwise, we do not have any control over when the memory
    // becomes free, so we cannot cache the pointer.
    if (enter_in_cache) {
        if(qtjambi_object_cache_mode_get() != 0) {
            QWriteLocker locker(gUserObjectCacheLock());
            Q_ASSERT(gUserObjectCache());
            gUserObjectCache()->insert(ptr, link);
            link->m_in_cache = true;
        }
        link->m_link_cacheable = true;
    }

    // Set the native__id field of the java object
    StaticCache *sc = StaticCache::instance();
    sc->resolveQtJambiObject();
    // We continue to use the hard local reference to java object in preference to the weak one we just created
    env->SetLongField(java, sc->QtJambiObject.native_id, reinterpret_cast<jlong>(link));

    return link;
}

QtJambiLink *QtJambiLink::findLinkForUserObject(const void *ptr)
{
    if (ptr == 0)
        return 0;

    QReadLocker locker(gUserObjectCacheLock());
    Q_ASSERT(gUserObjectCache());
    return gUserObjectCache()->value(ptr, 0);
}

QtJambiLink *QtJambiLink::findLinkForQObject(QObject *o)
{
    if (o == 0)
        return 0;

    QtJambiLinkUserData *p = static_cast<QtJambiLinkUserData *>(o->userData(user_data_id()));
    return p == 0 ? 0 : p->link();
}


QtJambiLink *QtJambiLink::findLink(JNIEnv *env, jobject java)
{
    if (java == 0)
        return 0;

    StaticCache *sc = StaticCache::instance();
    sc->resolveQtJambiObject();
#if defined(QTJAMBI_DEBUG_TOOLS)
    // This is a hack around the testcase org.qtjambi.autotests.TestMemoryManagementValueType
    if(env->IsInstanceOf(java, sc->QtJambiObject.class_ref) == 0) {
        fprintf(stderr, "QtJambiLink(static)::findLink(java=%p) WARNING the supplied argument fails (java instanceof QtJambiObject) test\n", java);
        fflush(stderr);
        return 0;
    }
    Q_ASSERT(env->IsInstanceOf(java, sc->QtJambiObject.class_ref));  // check the java object is right type
#endif
    QtJambiLink *link = reinterpret_cast<QtJambiLink *>(env->GetLongField(java, sc->QtJambiObject.native_id));
#if defined(QTJAMBI_DEBUG_TOOLS)
    Q_ASSERT(!link || QtJambiLink::QtJambiLinkList_check(link));  // check the C++ pointer is valid
#endif
    return link;
}


// This method will always cleanup QtJambiLink's m_java_object.  It will also fire the disposed() callback
//  and set the native_id=0 and removeFromCache() via aboutToMakeObjectInvalid().
int QtJambiLink::releaseJavaObject(JNIEnv *env)
{
    if (!m_java_object && !m_java_weak)
        return 0;

    int rv = 0;

    // The name of the method below isn't well chosen.  It is possible we
    // only want to relinquish the Java object but the C++ side will continue
    // to live on.  In this sense the object isn't going to an invalidate state.
    if(!m_object_invalid) {
        aboutToMakeObjectInvalid(env);
        rv |= 0x20000;    // invoked disposed() callback
    }

    if (isGlobalReference()) {
        env->DeleteGlobalRef(m_java_object);
        m_java_object = 0;
    } else {
        // Check if garbage collector has removed the object, hmm... it doesn't matter if the GC
        // has made the target null (by removing the object) because the GC still knows we are
        // holding a reference and it is this reference that has to be released by us here.  Now
        // if we were to try to access the object we'd need to obtain a NewLocalRef() first.
        //
        // I do not understand this pantomime, you only need to check IsSameObject when going to
        // access it not to delete it, since there will always be a WeakReference holder object
        // needing to be deallocated.
        //jobject localRef = env->NewLocalRef(m_java_weak);
        //bool bf = env->IsSameObject(localRef, 0);
        //if (env->IsSameObject(localRef, 0) == JNI_FALSE)  // localRef != null
            env->DeleteWeakGlobalRef(m_java_weak);
        //env->DeleteLocalRef(localRef);
        m_java_weak = 0;
    }

    rv |= 0x10000;	// deleted java object reference
    return rv;
}

#ifdef DEBUG_REFCOUNTING
typedef QHash<int, int> RefCountingHash;
Q_GLOBAL_STATIC(QReadWriteLock, gRefCountStaticLock);
Q_GLOBAL_STATIC(RefCountingHash, gRefCountHash);
#endif

// If the object is to be deleted in the main thread, then the gc thread will post an event
// to the main thread which will delete the link, and then proceed to alter the link object.
// If the event is delivered before the process of cleaning up the link in the gc thread is
// done, we will get crashes. Therefore, the lock guarantees that only one thread is cleaning
// up the link at any given time.
Q_GLOBAL_STATIC(QReadWriteLock, g_deleteLinkLock);

#if defined(QTJAMBI_DEBUG_TOOLS)
void
QtJambiLink::validateMagic_unlocked(const char *prefix, bool validate_children)
{
    if(m_magic != QTJAMBI_LINK_MAGIC) {
        char xb[24];
        fprintf(stderr, "QtJambiLink(%p %s) INVALID MAGIC %s found:0x%lx expected:0x%lx qtJambiLinkUserData=%p\n", this, debugFlagsToString(xb), (prefix == 0 ? "" : prefix), m_magic, QTJAMBI_LINK_MAGIC, m_qtJambiLinkUserData);
        fflush(stderr);
    }
    if(validate_children) {
        if(m_qtJambiLinkUserData)
            m_qtJambiLinkUserData->validateMagic_unlocked("child", false);
    }
}

void
QtJambiLink::validateMagic(bool validate_children)
{
    g_magicLock()->lock();
    validateMagic_unlocked(0, validate_children);
    g_magicLock()->unlock();
}

const char *
QtJambiLink::acquire_magic_source_id_to_string(int source_id)
{
    const char *kp;
    switch(source_id) {
    case QTJAMBI_MAGIC_QTJAMBILINK_DTOR:
        kp = "~QtJambiLink";
        break;

    case QTJAMBI_MAGIC_JAVAOBJECTFINALIZED:
        kp = "javaObjectFinalized";
        break;

    case QTJAMBI_MAGIC_JAVAOBJECTDISPOSED:
        kp = "javaObjectDisposed";
        break;

    case QTJAMBI_MAGIC_QTJAMBILINKUSERDATA_DTOR:
        kp = "~QtJambiLinkUserData";
        break;

    case QTJAMBI_MAGIC_QTJAMBILINKUSERDATA_DTOR2:
        kp = "~QtJambiLinkUserData#2";
        break;

    default:
        kp = 0;  // kp unintialized warning
        break;
    }
    return kp;
}

int
QtJambiLink::acquireMagic_unlocked(int source_id)
{
    int i = m_atomic_int.fetchAndAddAcquire(1);
    if(i != 0) {   // was 0, now 1
        // TID_SAME (usually means we reentered)
        char tidbuf[256];
        char xb[24];
        snprintf(tidbuf, sizeof(tidbuf), " tid=%p mytid=%p %s", m_pthread_id, THREAD_ID(), ((m_pthread_id == THREAD_ID()) ? "TID_SAME" : "TID_DIFF"));
        fprintf(stderr, "QtJambiLink(%p %s) INVALID ACQUIRE found:%d expected:%d%s them:%s us:%s\n", this, debugFlagsToString(xb), i, 0, tidbuf,
         acquire_magic_source_id_to_string(m_magic_source_id), acquire_magic_source_id_to_string(source_id));
        fflush(stderr);
    }
    return i;
}

void
QtJambiLink::acquireMagic(int source_id, bool validate_children)
{
    g_magicLock()->lock();
    validateMagic_unlocked(0, validate_children);
    if(acquireMagic_unlocked(source_id) == 0) {
        m_pthread_id = THREAD_ID();
        m_magic_source_id = source_id;
    }
    g_magicLock()->unlock();
}

int
QtJambiLink::releaseMagic_unlocked(int source_id)
{
    int i = m_atomic_int.fetchAndAddRelease(-1);
    if(i != 1) {   // was 1, now 0
        // TID_SAME (usually means we reentered)
        char tidbuf[256];
        char xb[24];
        snprintf(tidbuf, sizeof(tidbuf), " tid=%p mytid=%p %s", m_pthread_id, THREAD_ID(), ((m_pthread_id == THREAD_ID()) ? "TID_SAME" : "TID_DIFF"));
        fprintf(stderr, "QtJambiLink(%p %s) INVALID RELEASE found:%d expected:%d%s them:%s us:%s\n", this, debugFlagsToString(xb), i, 1, tidbuf,
         acquire_magic_source_id_to_string(m_magic_source_id), acquire_magic_source_id_to_string(source_id));
        fflush(stderr);
    }
    return i;
}

void
QtJambiLink::releaseMagic(int source_id, bool validate_children)
{
    g_magicLock()->lock();
    validateMagic_unlocked(0, validate_children);
    releaseMagic_unlocked(source_id);
    if(m_pthread_id == THREAD_ID())
        m_pthread_id = 0;
    g_magicLock()->unlock();
}
#endif

QtJambiLink::~QtJambiLink()
{
#if defined(QTJAMBI_DEBUG_TOOLS)
    acquireMagic(QTJAMBI_MAGIC_QTJAMBILINK_DTOR);
#endif
    int disp = 0;
    Q_UNUSED(disp)
    /* new block created to keep C++ goto usage happy */
    {
        JNIEnv *env = qtjambi_current_environment();   // moved from just before cleanUpAll(env) call to help debug messages
        bool i_am_locked;
        if (deleteInMainThread() || isDeleteLater() || isPointerZeroed()) {
            g_deleteLinkLock()->lockForWrite();
            i_am_locked = true;
        } else {
            i_am_locked = false;
        }

        disp = cleanUpAll(env);
#if defined(QTJAMBI_DEBUG_TOOLS)
        if((disp & 0x0001) != 0) {  // FIXME move up ?
            m_qtJambiLinkUserData = 0;
        }
#endif

#if defined(QTJAMBI_DEBUG_TOOLS)
        qtjambi_increase_linkDestroyedCount(m_className);
#endif

        if (i_am_locked) {
            g_deleteLinkLock()->unlock();
        }
    }

#if defined(QTJAMBI_DEBUG_TOOLS)
        QtJambiLinkList_remove();
#endif
#if defined(QTJAMBI_DEBUG_TOOLS)
        // when (disp & 0x0001)!=0 then we used 'delete' synchronously to destroy m_pointer, when we did this
        //  and it was a QObject it would have called QtJambiLinkUserData dtor.  Meaning we expect
        //  the MAGIC of that to now be invalid.
        releaseMagic(QTJAMBI_MAGIC_QTJAMBILINK_DTOR);
#endif
    return;

#if defined(QTJAMBI_DEBUG_TOOLS)
//release:  // label used by personal branch can remove someday
    releaseMagic(QTJAMBI_MAGIC_QTJAMBILINK_DTOR);
#endif
}


void QtJambiLink::aboutToMakeObjectInvalid(JNIEnv *env)
{
    Q_ASSERT(env);
    Q_ASSERT(!m_object_invalid);
    // FIXME: This is too many checks, the caller should be made responsible
    //  for many of these.  Verify visibility and use outside the DSO qtjambi.so.
    // FIXME: Consider renaming m_object_invalid => m_called_disposed
    // This used to care about (env != 0)
    // This used to care about (m_pointer != 0)
    // This used to care about (!m_object_invalid) not its an Q_ASSERT and caller checks

    jobject localRef = 0;
    jobject javaRefToInvalidate;
    if (!isGlobalReference()) {
        // Reconsile a non-null weak reference
        localRef = env->NewLocalRef(m_java_weak);
        if (env->IsSameObject(localRef, 0) != JNI_FALSE)
            goto done;  // weak ref is null so nothing to invalidate
        // We were a weak global reference, we now have a hard local reference to
        //  use for invalidation that is not 'null'.
        javaRefToInvalidate = localRef;
    } else {
        javaRefToInvalidate = m_java_object;
    }

    if (javaRefToInvalidate) {
        StaticCache *sc = StaticCache::instance();
        sc->resolveQtJambiObject();
        env->CallVoidMethod(javaRefToInvalidate, sc->QtJambiObject.disposed);
        qtjambi_exception_check(env);

#if defined(QTJAMBI_DEBUG_TOOLS)
        qtjambi_increase_objectInvalidatedCount(m_className);
#endif

        env->SetLongField(javaRefToInvalidate, sc->QtJambiObject.native_id, 0);
        if (m_in_cache)
            removeFromCache(env);
        QTJAMBI_EXCEPTION_CHECK(env);
        m_object_invalid = true;
    }

done:
    if(localRef)
        env->DeleteLocalRef(localRef);
}

void QtJambiLink::setMetaObject(const QMetaObject *mo) const
{
    Q_ASSERT(isQObject());
    if (!isQObject())
        return;

    QObject *o = qobject();
    QtJambiLinkUserData *d = static_cast<QtJambiLinkUserData *>(o->userData(QtJambiLinkUserData::id()));
    if (d != 0)
        d->setMetaObject(mo);
    else
        qWarning("setMetaObject: No jambi user data in QObject, line %d in file '%s'", __LINE__, __FILE__);
}

void QtJambiLink::setGlobalRef(JNIEnv *env, bool global)
{
    if (global == m_global_ref)
        return;

    Q_ASSERT_X(m_java_object, "QtJambiLink::setGlobalRef()", "Java object required");

    if (!global) {
        // Delete the global reference and make it a weak one
        jweak weakGlobalRef = env->NewWeakGlobalRef(m_java_object); // CHECKME: m_global_ref ? m_java_object : m_java_weak

        env->DeleteGlobalRef(m_java_object);
        m_java_object = 0;  // PARANOIA

        m_global_ref = false;
        m_java_weak = weakGlobalRef;

    } else {
        // Delete the weak ref and replace it with a global ref
        jobject globalRef = env->NewGlobalRef(m_java_object); // CHECKME: m_global_ref ? m_java_object : m_java_weak

        env->DeleteWeakGlobalRef(m_java_weak);
        m_java_weak = 0;  // PARANOIA

        m_global_ref = true;
        m_java_object = globalRef;
    }
}

// We now return a disposition explaining what we did, the values need moving to
//  macros for better readibility.
// What we can say about this method in entry is that (m_pointer!=0 && m_delete_later==0)
//  or its a bug.
// What we can say about this method on exit is that (m_pointer==0 || m_delete_later!=0)
//  or its a bug.  Sometimes when m_pointer==0 then m_pointer_zeroed!=0.
int QtJambiLink::deleteNativeObject(JNIEnv *env)
{
    QTJAMBI_DEBUG_TRACE_WITH_THREAD("(native) entering: QtJambiLink::deleteNativeObject(JNIEnv *)");
#ifdef DEBUG_REFCOUNTING
    {
        QWriteLocker locker(gRefCountStaticLock());
        int currentCount = gRefCountHash()->value(m_meta_type, 0) - 1;
        gRefCountHash()->insert(m_meta_type, currentCount);
        qDebug("Deleting '%s' [count after: %d]", QMetaType::typeName(m_meta_type), currentCount);
    }
#endif
    Q_ASSERT(m_pointer);	// must be non-null
    Q_ASSERT(!m_delete_later);	// must be 0
    int rv = 0;

#if 0
    // I can only guess we call this from here to give the disposed() callback a
    // last chance to work with the native object before we destroy it.
    // NOPE, this appears to be have the problem in blowing away the ability
    //  to translate from m_pointer -> m_java_object when the Qt event system
    //  is trying to delivery events after the object has been GCed by Java and
    //  we use deleteLater() here.  In this case it is best to let the Qt system
    //  use the C++ dtor to cause this logic to happen since at that point in
    //  time we a guaranteed events will have been flushed.
    if(!m_object_invalid)
        aboutToMakeObjectInvalid(env);

    // As the native object is about to be deleted then there is no need for the
    // native object to pin the Java object in memory any longer.
    if (m_java_object && isGlobalReference()) {
        env->DeleteGlobalRef(m_java_object);
        m_java_object = 0;
    }
#endif

    if (isQObject() && m_ownership == JavaOwnership) {

        QObject *qobj = qobject();
        QThread *objectThread = qobj->thread();

        QThread *currentThread = QThread::currentThread();
        // Explicit dispose from current thread, delete object
        if (currentThread == objectThread) {
//             printf(" - straight up delete %s [%s]\n",
//                    qPrintable(qobj->objectName()),
//                    qobj->metaObject()->className());
            m_user_data_skip = 1;  // inhibit UserData
            QTJAMBI_DEBUG_TRACE_WITH_THREAD("call delete QObject");
            delete qobj;
            setAsQObjectDeleted();  // put here due to m_user_data_skip=1
            rv = 0x0001;    // called delete

        // We're in the main thread and we'll have an event loop
        // running, so its safe to call delete later.
        } else if (QCoreApplication::instance() &&
                   QCoreApplication::instance()->thread() == objectThread) {
//             printf(" - deleteLater in main thread %s [%s]\n",
//                    qPrintable(qobj->objectName()),
//                    qobj->metaObject()->className());
            m_delete_later = 1;
            QTJAMBI_DEBUG_TRACE_WITH_THREAD("call QObject::deleteLater()");
            qobj->deleteLater();
            rv = 0x0002;    // called deleteLater()

        // If the QObject is in a non-main thread, check if that
        // thread is a QThread, in which case it will run an eventloop
        // and thus, do cleanup, hence deleteLater() is safe;
        // Otherwise issue a warning.
        } else {
            jobject t = env->NewLocalRef(qtjambi_from_thread(env, objectThread));
            if (t) {
                QTJAMBI_EXCEPTION_CHECK(env);
//                printf("name %s, %s, %p", qPrintable(objectThread->objectName()), objectThread->metaObject()->className(), t);;
                jclass cl = env->GetObjectClass(t);
//                qDebug() << ".. au ..";

                if (qtjambi_class_name(env, cl) == QLatin1String("org.qtjambi.qt.QThread")) {
    //                 printf(" - delete later in QThread=%p %s [%s]\n",
    //                        objectThread,
    //                        qPrintable(qobj->objectName()),
    //                        qobj->metaObject()->className());
                    m_delete_later = 1;
                    qobj->deleteLater();
                    rv = 0x0002;    // called deleteLater()
                } else if (QCoreApplication::instance()) {
                    // Make we should explain to use moveToThread()
                    qWarning("QObjects can only be implicitly garbage collected when owned"
                            " by a QThread, native resource ('%s' [%s]) is leaked",
                            qPrintable(qobj->objectName()),
                            qobj->metaObject()->className());
                    rv = 0x1000;    // error? or not?
                }

    //             StaticCache *sc = StaticCache::instance();
    //             sc->resolveQThread();
    //             if (env->IsSameObject(cl, sc->QThread.class_ref)) {
    //                 qobj->deleteLater();

    //             } else {
    //                 // ## Resolve QThreadAffinityException...
    //                 // Message: "QObjects can only be implicitly garbage collected when owned by a QThread".
    //                 qWarning("something really bad happened...");
    //             }
            } else {
                m_user_data_skip = 1;  // inhibit UserData
                delete qobj;
                setAsQObjectDeleted();  // put here due to m_user_data_skip=1
                rv = 0x0001;    // called delete
            }
            env->DeleteLocalRef(t);
        }

        // If flagged that this object is now pending deleteLater() but we don't zap the m_pointer
        // as there maybe other events to process on the C++ object ahead of the deletion action.
        if(!m_delete_later)
            m_pointer = 0;

    } else {
        if (m_ownership == JavaOwnership && deleteInMainThread() && (QCoreApplication::instance() == 0 || QCoreApplication::instance()->thread() != QThread::currentThread())) {

            if (QCoreApplication::instance()) {
                QTJAMBI_DEBUG_TRACE_WITH_THREAD("use QtJambiDestructorEvent");
                m_pointer_zeroed = 1;    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                QtJambiDestructorEvent *qtde = new QtJambiDestructorEvent(this, m_pointer, m_meta_type, m_ownership, m_destructor_function);
                QCoreApplication::postEvent(QCoreApplication::instance(), qtde);
                rv = 0x0003;    // queued QtJambiDestructorEvent
            }

        } else if (m_ownership == JavaOwnership && m_pointer != 0 &&
           #if QT_VERSION < 0x050000
                   m_meta_type != QMetaType::Void &&
           #else
                   m_meta_type != QMetaType::UnknownType &&
           #endif
                   (QCoreApplication::instance() != 0
                   || (m_meta_type < QMetaType::FirstGuiType 
#if QT_VERSION < 0x050000
					|| m_meta_type > QMetaType::LastGuiType
#else
					|| m_meta_type > QMetaType::LastWidgetsType
#endif
				))) {

           QTJAMBI_DEBUG_TRACE_WITH_THREAD("call QMetaType::destroy()");
           QMetaType::destroy(m_meta_type, m_pointer);
           rv = 0x0004;    // used QMetaType::destroy

        } else if (m_ownership == JavaOwnership && m_destructor_function) {
            QTJAMBI_DEBUG_TRACE_WITH_THREAD("call destructor_function");
            m_destructor_function(m_pointer);
            rv = 0x0005;    // used m_destructor_function
        }

        m_pointer = 0;
    }
    QTJAMBI_DEBUG_TRACE_WITH_THREAD("(native) leaving: QtJambiLink::deleteNativeObject(JNIEnv *)");
    return rv;
}

// This is called from QtJambiLink dtor
int QtJambiLink::cleanUpAll(JNIEnv *env)
{
    int rv = 0;

    if (m_java_object != 0)
        rv |= releaseJavaObject(env);

    if (m_pointer && !m_delete_later)
        rv |= deleteNativeObject(env);

    if (m_wrapper) {
        delete m_wrapper;
        m_wrapper = 0;
        rv |= 0x0100;
    }

    return rv;
}

// Entry point for JVM finalization QtJambiObject#finalize()
void QtJambiLink::javaObjectFinalized(JNIEnv *env)
{
#if defined(QTJAMBI_DEBUG_TOOLS)
    acquireMagic(QTJAMBI_MAGIC_JAVAOBJECTFINALIZED);
#endif
    bool i_am_locked;
    if (deleteInMainThread() || isDeleteLater() || isPointerZeroed()) {
        g_deleteLinkLock()->lockForWrite();
        i_am_locked = true;
    } else {
        i_am_locked = false;
    }

    int disp = cleanUpAll(env);
#if defined(QTJAMBI_DEBUG_TOOLS)
    if((disp & 0x0001) != 0) {  // FIXME move up ?
        m_qtJambiLinkUserData = 0;
    }
#else
    Q_UNUSED(disp);
#endif
    setAsFinalized();

    // This is required for the user data destructor
    // can know it's time to delete the link object.
    m_java_link_removed = true;

    // Link deletion policy:
    //
    // 1. QObjects will be deleteLater'd, and they always have a link, so we must keep the link
    // 2. GUI objects with Java ownership will be deleted by event, the event will kill the link,
    //    so we must keep the link.
    // 3. For all other objects:
    //    A. If Java has ownership of the native object, it has been deleted, so we kill the link
    //    B. The last possible scenario is either C++ owns the Java object (it wouldn't have been finalized)
    //       or it's not created by Java. Since the Java object has been finalized, we know the ownership
    //       is split, and the object is not created by Java, and we can safely kill the link because
    //       the native object cannot have a link back.

    // Collect info before opening the lock, as the link may be deleted at any point after the
    // lock is released, so we can't call any functions on it.
    bool javaOwnership = ownership() == JavaOwnership;
    bool isQObject = this->isQObject();
    bool deleteInMainThread = this->deleteInMainThread();
    bool qobjectDeleted = this->qobjectDeleted();

    if (i_am_locked) {
        g_deleteLinkLock()->unlock();
    }

    // **** Link may be deleted at this point (QObjects and GUI objects that are Java owned)

#if defined(QTJAMBI_DEBUG_TOOLS)
    releaseMagic(QTJAMBI_MAGIC_JAVAOBJECTFINALIZED);
#endif
    if ((!isQObject || qobjectDeleted) && (!deleteInMainThread || !javaOwnership))
        delete this;
}

// This allows the Java language to attempt to expedite the disposal of the C++ object
//  (when that this possible), however its possible the C++ object will not be destroyed
//  as the Java side does not own it.
// Entry point for QtJambiObject#dispose() method
void QtJambiLink::javaObjectDisposed(JNIEnv *env)
{
#if defined(QTJAMBI_DEBUG_TOOLS)
    acquireMagic(QTJAMBI_MAGIC_JAVAOBJECTDISPOSED);
#endif
    bool i_am_locked = false;
    if (deleteInMainThread() || isDeleteLater() || isPointerZeroed()) {
        g_deleteLinkLock()->lockForWrite();
        i_am_locked = true;
    }

    if (m_pointer) {
        // Not sure why we do this here ?  Why are we messing with ownership and therefore the
        //  lifecycles.  And why is m_pointer!=0 important ?
        // I have a hunch this is forced to Java ownership due to QtJambiLinkUserData reentrancy as it would
        //  inhibit the 'delete m_link;'.  If this is the case I have understood and solved that matter a
        //  different way and there is no risk of having QtJambiLink destroyed from under us.
        // Alternatively this maybe to force a WeakReference back to Java from QtJambiLink because after
        //  all we are disposed now.
        setJavaOwnership(env, m_java_object);
        // We call this here to ensure it always gets called before we return from javaObjectDisposed()
        if(!m_object_invalid)
            aboutToMakeObjectInvalid(env);
        // This must be called when m_pointer!=0 && !m_delete_later
        if(!m_delete_later) {
            int disp;
//            if(isPointer()){
//                QtPointerJambiLink* pointerLink = dynamic_cast<QtPointerJambiLink*>(this);
//                disp = pointerLink->deleteNativeObject(env);
//            }else{
                disp = deleteNativeObject(env);
//            }
#if defined(QTJAMBI_DEBUG_TOOLS)
            if((disp & 0x0001) != 0) {  // FIXME move up ?
                m_qtJambiLinkUserData = 0;
            }
#else
            Q_UNUSED(disp);
#endif
        }

#if defined(QTJAMBI_DEBUG_TOOLS)
        qtjambi_increase_disposeCalledCount(m_className);
#endif

    }

    // Link has been severed from Java object. We can delete
    // it as soon as the C++ object severs its link.
    m_java_link_removed = true;

    // Link deletion policy:
    //
    // NOTE that dispose() forces Java ownership of the C++ object, so we
    // only need to consider this case.
    //
    // 1. QObjects that are in the wrong thread will be deleteLater'd, we must keep the link
    // 2. QObjects that are the right thread have been deleted, we can kill the link [qobjectDeleted is true in this case]
    // 3. GUI objects that are not in the GUI thread will be deleted later, we keep the link
    // 4. All other objects can kill the link now

    // Collect data about object before opening lock, as the link
    // can be deleted at any point once the lock is open
    bool isQObject = this->isQObject();
    bool qobjectDeleted = this->qobjectDeleted();
    bool deleteInMainThread = this->deleteInMainThread();
    bool isGUIThread = QCoreApplication::instance() == 0 || QCoreApplication::instance()->thread() == QThread::currentThread();

    if (i_am_locked) {
        g_deleteLinkLock()->unlock();
    }

    // **** Link may be deleted at this point (QObjects and GUI objects that are Java owned)

#if defined(QTJAMBI_DEBUG_TOOLS)
    releaseMagic(QTJAMBI_MAGIC_JAVAOBJECTDISPOSED);
#endif
    if ((!isQObject || qobjectDeleted) && (!deleteInMainThread || isGUIThread))
        delete this;
}

// Entry point for generated shell C++ destructor (must understand this better and what
//  the purpose of this extra method is for)
void QtJambiLink::nativeShellObjectDestroyed(JNIEnv *env)
{
    resetObject(env);

    // Link deletion policy:
    //
    // Either the object has Java ownership or C++ ownership [we can only come here if the object was created by Java]
    // We also know that this function is *never* called for QObjects, as they are memory managed differently.
    // 1. If it has Java ownership, it is currently being deleted, we should keep the link because we are inside one of
    //    the other deletion entry points, and the link will be deleted a little bit later.
    // 2. If it has C++ ownership, the link should be deleted, because the Java object will now have its link removed.

    if (ownership() != JavaOwnership)
        delete this;
}

void QtJambiLink::setMetaType(int metaType)
{
    m_meta_type = metaType;

#ifdef DEBUG_REFCOUNTING
    {
        QWriteLocker locker(gRefCountStaticLock());
        int currentCount = gRefCountHash()->value(m_meta_type, 0) + 1;
        gRefCountHash()->insert(m_meta_type, currentCount);
        qDebug("Creating '%s' [count after: %d]", QMetaType::typeName(m_meta_type), currentCount);
    }
#endif
}


void QtJambiLink::resetObject(JNIEnv *env) {
// FIXME: We want to add this check if needed
//    if (m_java_object != 0)
    releaseJavaObject(env);
    // FIXME: releaseJavaObject() already calls aboutToMakeObjectInvalid(env) when m_java_object!=0
    // redundant call to aboutToMakeObjectInvalid()
    //if(!m_object_invalid)
    //    aboutToMakeObjectInvalid(env);

    if(m_pointer) {
        // This flag means the object still exists in memory but its not our (QtJambi) responsbility anymore
        m_pointer_zapped = 1;
        m_pointer = 0;
    }
}

// This gets called when we hand a C++ object to C++ API and that API takes ownership of it,
// this puts the lifecycle out of QtJambi's (and Java's) control.  So we reliquish lifecycle
// management from the Java side of the object and then consider if we need to destroy the
// QtJambiLink instance itself.  Posting QEvent's is a common example of this pattern.
//
// Entry point for Java to disown its association with C++ (events, painters and other
//  temporarly valid objects presenting API to Java to use for callback)  This method
//  ensures that if the Java code was silly enough to hold onto a reference to the
//  temporary data then trying to access it later will fail gracefully (exceptions or no-op).
void QtJambiLink::javaObjectInvalidated(JNIEnv *env)
{
// FIXME: We want to add this check if needed
//    if (m_java_object != 0)  
    releaseJavaObject(env);

    // Link deletion policy
    //
    // We can only get to this point if the object is not Java owned, so that reduces the cases we need to look at.
    // 1. Either the native object has a link back (it's a QObject which has not been deleted,
    //    or a polymorphic object which has been created by Java.) We should keep the link in this case.
    // 2. or it doesn't (not created by Java and not QObject, or not polymorphic.) We should kill the link in this case.
    //    because there's no way of getting back to it.

    m_java_link_removed = true;
    if (qobjectDeleted() || (!isQObject() && !createdByJava()))
        delete this;
}

/*******************************************************************************
 * Convenience stuff...
 */

jmethodID QtJambiLink::findMethod(JNIEnv *env, jobject javaRef, const QString &method)
{
    Q_ASSERT(javaRef != 0);
    Q_ASSERT(env != 0);

    QString name;
    QString signature = QtJambiTypeManager::toJNISignature(method, &name);

    jclass clazz = env->GetObjectClass(javaRef);
    jmethodID id = 0;
    if (clazz != 0)
        id = resolveMethod(env, name.toLatin1(), signature.toLatin1(), clazz);

    if (id == 0) {
        qWarning("QtJambiLink::findMethod(), '%s' was not found (%s - %s)",
                 qPrintable(method),
                 qPrintable(name), qPrintable(signature));
    }

    return id;
}

void QtJambiLink::removeFromCache(JNIEnv *)
{
    QWriteLocker locker(gUserObjectCacheLock());
    if (m_pointer != 0 && gUserObjectCache() && gUserObjectCache()->contains(m_pointer)) {
        int count = gUserObjectCache()->remove(m_pointer);
        Q_ASSERT(count == 1);
        Q_UNUSED(count);
        m_in_cache = false;
    }
}

bool QtJambiLink::throwQtException(JNIEnv *env, const QString &extra, const QString &name)
{
    bool success = false;
    jclass cls = resolveClass(env, name.toUtf8().constData(), "org/qtjambi/qt/");
    QTJAMBI_EXCEPTION_CHECK(env);
    success = (env->ThrowNew(cls, extra.toUtf8()) == 0);
    return success;
}

QString QtJambiLink::nameForClass(JNIEnv *env, jclass clazz)
{
    QString returned;

    jmethodID methodId = resolveMethod(env, "getName", "()Ljava/lang/String;", "Class", "java/lang/");
    if (methodId != 0) {
        returned = qtjambi_to_qstring(env, reinterpret_cast<jstring>(env->CallObjectMethod(clazz, methodId)));
    }

    return returned;
}

bool QtJambiLink::stripQtPackageName(QString *className)
{
    bool altered = false;
    if (className->startsWith(QLatin1String(PACKAGEPATH))) {
        int idx = className->lastIndexOf("/");
        if (idx != -1) {
            (*className) = className->right(className->length() - idx - 1);
            altered = true;
        }
    }

    return altered;
}


int QtJambiLink::indexQtSignal(const QByteArray &signal) const
{
    Q_ASSERT(qobject() != 0);

    const QMetaObject *mo = qobject()->metaObject();
    QByteArray normalized = QMetaObject::normalizedSignature(signal.data());

    return mo->indexOfSignal(normalized.data() + 1);
}

int QtJambiLink::indexQtSlot(const QByteArray &slot) const
{
    Q_ASSERT(qobject() != 0);

    const QMetaObject *mo = qobject()->metaObject();
    QByteArray normalized = QMetaObject::normalizedSignature(slot.data());

    return mo->indexOfSlot(normalized.data() + 1);
}

// FIXME: We should use the save/restore of the original ownership here

void QtJambiLink::disableGarbageCollection(JNIEnv *env, jobject obj)
{
    setCppOwnership(env, obj);
}

void QtJambiLink::setCppOwnership(JNIEnv *env, jobject obj)
{
    if (!isGlobalReference()) {
        // The autotests case org.qtjambi.autotests.TestMemoryManagementNonPolymorphicObject#nativeDelete_NotCreatedInJava_CppOwnership()
        //  consistently fails, because the test before it #nativeDelete_CreatedInJava_CppOwnership() puts a reference in the cache
        //  that does not appear to get removed (even upon destruction).  Then by change the next test causes a new native QObject
        //  to be created that happens to have the same memory address as an item in the cache, this results in a unittest failure
        //  for: junit.framework.AssertionFailedError: linkConstructedCount expected:<1> but was:<0>
        // Reading the notes in QtJambiLink::createLinkForObject(...) around use of enter_in_cache==true it would appear that
        //  caching is only valid when Java has ownership.
        jobject globalRef = env->NewGlobalRef(obj);

        if (m_java_weak) {
            env->DeleteWeakGlobalRef(m_java_weak);
            m_java_weak = 0;  // PARANOIA
        }

        m_java_object = globalRef;
        m_global_ref = true;
    }
    m_ownership = CppOwnership;
}

void QtJambiLink::setDefaultOwnership(JNIEnv *env, jobject obj)
{
    if (createdByJava())
        setJavaOwnership(env, obj);
    else
        setSplitOwnership(env, obj);
}

void QtJambiLink::setJavaOwnership(JNIEnv *env, jobject obj)
{
    if (isGlobalReference()) {
        jobject weakGlobalRef = env->NewWeakGlobalRef(obj);

        if (m_java_object) {
            env->DeleteGlobalRef(m_java_object);
            m_java_object = 0;  // PARANOIA
        }

        m_java_weak = weakGlobalRef;
        m_global_ref = false;
    }
    m_ownership = JavaOwnership;
}

void QtJambiLink::setSplitOwnership(JNIEnv *env, jobject obj)
{
    if (isGlobalReference()) {
        jobject weakGlobalRef = env->NewWeakGlobalRef(obj);

        if (m_java_object) {
            env->DeleteGlobalRef(m_java_object);
            m_java_object = 0;  // PARANOIA
        }

        m_java_weak = weakGlobalRef;
        m_global_ref = false;
    }
    m_ownership = SplitOwnership;
}

#if defined(QTJAMBI_DEBUG_TOOLS)
void
QtJambiLinkUserData::validateMagic_unlocked(const char *prefix, bool validate_children)
{
    if(m_magic != QTJAMBI_LINK_USER_DATA_MAGIC) {
        fprintf(stderr, "QtJambiLinkUserData(%p) INVALID MAGIC %s found:0x%lx expected:0x%lx m_link=%p\n", this, (prefix == 0 ? "" : prefix), m_magic, QTJAMBI_LINK_USER_DATA_MAGIC, m_link);
        fflush(stderr);
    }
    if(validate_children) {
        if(m_link)
            m_link->validateMagic_unlocked("child", false);
    }
}

void
QtJambiLinkUserData::validateMagic(bool validate_children)
{
    g_magicLock()->lock();
    validateMagic_unlocked(0, validate_children);
    g_magicLock()->unlock();
}

int
QtJambiLinkUserData::acquireMagic_unlocked(int source_id)
{
    int i = m_atomic_int.fetchAndAddAcquire(1);
    if(i != 0) {   // was 0, now 1
        // TID_SAME (usually means we reentered)
        char tidbuf[256];
        snprintf(tidbuf, sizeof(tidbuf), " tid=%p mytid=%p %s", m_pthread_id, THREAD_ID(), ((m_pthread_id == THREAD_ID()) ? "TID_SAME" : "TID_DIFF"));
        fprintf(stderr, "QtJambiLinkUserData(%p) INVALID ACQUIRE found:%d expected:%d%s them:%s us:%s\n", this, i, 0, tidbuf,
         QtJambiLink::acquire_magic_source_id_to_string(m_magic_source_id), QtJambiLink::acquire_magic_source_id_to_string(source_id));
        fflush(stderr);
    }
    return i;
}

void
QtJambiLinkUserData::acquireMagic(int source_id, bool validate_children)
{
    g_magicLock()->lock();
    validateMagic_unlocked(0, validate_children);
    if(acquireMagic_unlocked(source_id) == 0) {
        m_pthread_id = THREAD_ID();
        m_magic_source_id = source_id;
    }
    g_magicLock()->unlock();
}

int
QtJambiLinkUserData::releaseMagic_unlocked(int source_id)
{
    int i = m_atomic_int.fetchAndAddRelease(-1);
    if(i != 1) {   // was 1, now 0
        // TID_SAME (usually means we reentered)
        char tidbuf[256];
        snprintf(tidbuf, sizeof(tidbuf), " tid=%p mytid=%p %s", m_pthread_id, THREAD_ID(), ((m_pthread_id == THREAD_ID()) ? "TID_SAME" : "TID_DIFF"));
        fprintf(stderr, "QtJambiLinkUserData(%p) INVALID RELEASE found:%d expected:%d%s them:%s us:%s\n", this, i, 1, tidbuf,
         QtJambiLink::acquire_magic_source_id_to_string(m_magic_source_id), QtJambiLink::acquire_magic_source_id_to_string(source_id));
        fflush(stderr);
    }
    return i;
}

void
QtJambiLinkUserData::releaseMagic(int source_id, bool validate_children)
{
    g_magicLock()->lock();
    validateMagic_unlocked(0, validate_children);
    releaseMagic_unlocked(source_id);
    if(m_pthread_id == THREAD_ID())
        m_pthread_id = 0;
    g_magicLock()->unlock();
}
#endif

QtJambiLinkUserData::~QtJambiLinkUserData()
{
#if defined(QTJAMBI_DEBUG_TOOLS)
    acquireMagic(QTJAMBI_MAGIC_QTJAMBILINKUSERDATA_DTOR);
#endif
    if (m_link) {
#if defined(QTJAMBI_DEBUG_TOOLS)
        qtjambi_increase_userDataDestroyedCount(m_link->m_className);   // this derefs m_link
#endif
        JNIEnv *env = qtjambi_current_environment();
        if(m_link->isUserDataSkip()) {
            // This is set when QtJambiLink is calling the 'delete m_pointer;' and it doesn't want
            //  QtJambiLinkUserData to manage the QtJambiLink destruction.  This is in effect
            //  a reentrant barrier for a known scenario.
            // QtJambiLink::deleteNativeObject()  invokes "delete qobj;" and we end up here
            //  re-entering the QtJambi infrastructure code (we already have acquired the
            //  m_link->acquireMagic().  So we short circuit ~QtJambiLinkUserData() dtor.
#if defined(QTJAMBI_DEBUG_TOOLS)
            fprintf(stderr, "QtJambiLinkUserData(%p)::~QtJambiLinkUserData()  SKIP env=%p tid=%p;  m_link=%p, user_data_skip=%d\n", this, env, THREAD_ID(), m_link, m_link->isUserDataSkip());
#endif
            goto release;
        }
        // This typically happens when a QObject is destroyed after the vm shuts down,
        // in which case there is no way for us to properly clean up...
        if (!env)
            goto release;	// FIXME do better than this
#if defined(QTJAMBI_DEBUG_TOOLS)
        m_link->acquireMagic(QTJAMBI_MAGIC_QTJAMBILINKUSERDATA_DTOR2);  // if you move this up before isUserDataSkip() check you can observe the re-entering
#endif
        m_link->setAsQObjectDeleted();

        m_link->resetObject(env);

        // Link deletion policy
        //
        // 1. If there's Java ownership, java link removed: We are being deleteLater'd. Kill the link.
        // 2. Java ownership, java link not removed: We are currently being deleted. Keep the link. (dispose() basically)
        // 3. Cpp/Split ownership: Java object has been invalidated, so there's no link back. We can kill the link.

        if (m_link->ownership() != QtJambiLink::JavaOwnership || m_link->javaLinkRemoved()) {
#if defined(QTJAMBI_DEBUG_TOOLS)
            m_link->releaseMagic(QTJAMBI_MAGIC_QTJAMBILINKUSERDATA_DTOR2);
#endif
            delete m_link;
#if defined(QTJAMBI_DEBUG_TOOLS)
            invalidateLink();   // inhibit releaseMagic() at the end of this function from checking it
#endif
        }
    }

release:
#if defined(QTJAMBI_DEBUG_TOOLS)
    releaseMagic(QTJAMBI_MAGIC_QTJAMBILINKUSERDATA_DTOR);
#endif
    return;
}

QtPointerJambiLink::~QtPointerJambiLink(){
    QTJAMBI_DEBUG_TRACE_WITH_THREAD("(native) entering: QtPointerJambiLink::~QtPointerJambiLink()");
    if(m_pointerContainer){
        QThread *objectThread = m_pointerContainer->thread();
        QThread *currentThread = QThread::currentThread();
        if (currentThread == objectThread) {
            delete m_pointerContainer;
        } else if (QCoreApplication::instance() &&
                   QCoreApplication::instance()->thread() == objectThread) {
            m_pointerContainer->deleteLater();
        }else{
            qWarning("QObjects can only be implicitly garbage collected when owned"
                    " by a QThread, native resource ('%s' [%s]) is leaked",
                    qPrintable(m_pointerContainer->objectName()),
                    m_pointerContainer->metaObject()->className());
        }
        m_pointerContainer = 0;
        m_pointer = 0;
    }else if(m_ptr_shared_pointer){
        m_shared_pointer_deleter(m_pointerContainer);
        m_ptr_shared_pointer = 0;
        m_shared_pointer_deleter = 0;
        m_pointer = 0;
    }
    QTJAMBI_DEBUG_TRACE_WITH_THREAD("(native) -> leaving: QtPointerJambiLink::~QtPointerJambiLink()");
}

int QtPointerJambiLink::deleteNativeObject(JNIEnv *env){
    int rv = 0;
    if(m_ptr_shared_pointer){
        QTJAMBI_DEBUG_TRACE_WITH_THREAD("(native) entering: QtPointerJambiLink::deleteNativeObject(JNIEnv *)");
    #ifdef DEBUG_REFCOUNTING
        {
            QWriteLocker locker(gRefCountStaticLock());
            int currentCount = gRefCountHash()->value(m_meta_type, 0) - 1;
            gRefCountHash()->insert(m_meta_type, currentCount);
            qDebug("Deleting '%s' [count after: %d]", QMetaType::typeName(m_meta_type), currentCount);
        }
    #endif
        Q_ASSERT(m_pointer);	// must be non-null
        Q_ASSERT(!m_delete_later);	// must be 0

        if (deleteInMainThread() && (QCoreApplication::instance() == 0 || QCoreApplication::instance()->thread() != QThread::currentThread())) {

            if (QCoreApplication::instance()) {
                QTJAMBI_DEBUG_TRACE_WITH_THREAD("use QtJambiDestructorEvent (calling shared pointer deleter function)");
                m_pointer_zeroed = 1;    // qobject still exists at the time we cut it away (and we have shoved dtor to event system)
                QtJambiDestructorEvent *qtde = new QtJambiDestructorEvent(
                            this,
                            m_ptr_shared_pointer,
            #if QT_VERSION < 0x050000
                            QMetaType::Void,
            #else
                            QMetaType::UnknownType,
            #endif
                            QtJambiLink::JavaOwnership,
                            m_shared_pointer_deleter);
                QCoreApplication::postEvent(QCoreApplication::instance(), qtde);
                rv = 0x0003;    // queued QtJambiDestructorEvent
            }

        } else {
            QTJAMBI_DEBUG_TRACE_WITH_THREAD("call shared pointer deleter function");
            m_shared_pointer_deleter(m_ptr_shared_pointer);
            rv = 0x0005;    // used m_destructor_function
        }

        m_pointer = 0;
        m_ptr_shared_pointer = 0;

        QTJAMBI_DEBUG_TRACE_WITH_THREAD("(native) leaving: -> QtPointerJambiLink::deleteNativeObject(JNIEnv *)");

    }else if(m_pointerContainer){

        QTJAMBI_DEBUG_TRACE_WITH_THREAD("(native) entering: QtPointerJambiLink::deleteNativeObject(JNIEnv *)");
#ifdef DEBUG_REFCOUNTING
        {
            QWriteLocker locker(gRefCountStaticLock());
            int currentCount = gRefCountHash()->value(m_meta_type, 0) - 1;
            gRefCountHash()->insert(m_meta_type, currentCount);
            qDebug("Deleting '%s' [count after: %d]", QMetaType::typeName(m_meta_type), currentCount);
        }
#endif
        Q_ASSERT(m_pointer);	// must be non-null
        Q_ASSERT(!m_delete_later);	// must be 0

    #if 0
        // I can only guess we call this from here to give the disposed() callback a
        // last chance to work with the native object before we destroy it.
        // NOPE, this appears to be have the problem in blowing away the ability
        //  to translate from m_pointer -> m_java_object when the Qt event system
        //  is trying to delivery events after the object has been GCed by Java and
        //  we use deleteLater() here.  In this case it is best to let the Qt system
        //  use the C++ dtor to cause this logic to happen since at that point in
        //  time we a guaranteed events will have been flushed.
        if(!m_object_invalid)
            aboutToMakeObjectInvalid(env);

        // As the native object is about to be deleted then there is no need for the
        // native object to pin the Java object in memory any longer.
        if (m_java_object && isGlobalReference()) {
            env->DeleteGlobalRef(m_java_object);
            m_java_object = 0;
        }
    #endif

        QThread *objectThread = m_pointerContainer->thread();

        QThread *currentThread = QThread::currentThread();
        // Explicit dispose from current thread, delete object
        if (currentThread == objectThread) {
//             printf(" - straight up delete %s [%s]\n",
//                    qPrintable(qobj->objectName()),
//                    qobj->metaObject()->className());
            m_user_data_skip = 1;  // inhibit UserData
            QTJAMBI_DEBUG_TRACE_WITH_THREAD("call delete PointerContainer");
            delete m_pointerContainer;
            m_pointerContainer = 0;
            setAsQObjectDeleted();  // put here due to m_user_data_skip=1
            rv = 0x0001;    // called delete

        // We're in the main thread and we'll have an event loop
        // running, so its safe to call delete later.
        } else if (QCoreApplication::instance() &&
                   QCoreApplication::instance()->thread() == objectThread) {
//             printf(" - deleteLater in main thread %s [%s]\n",
//                    qPrintable(qobj->objectName()),
//                    qobj->metaObject()->className());
            m_delete_later = 1;
            QTJAMBI_DEBUG_TRACE_WITH_THREAD("call PointerContainer::deleteLater()");
            m_pointerContainer->deleteLater();
            rv = 0x0002;    // called deleteLater()

        // If the QObject is in a non-main thread, check if that
        // thread is a QThread, in which case it will run an eventloop
        // and thus, do cleanup, hence deleteLater() is safe;
        // Otherwise issue a warning.
        } else {
            jobject t = env->NewLocalRef(qtjambi_from_thread(env, objectThread));
            if (t) {
                QTJAMBI_EXCEPTION_CHECK(env);
//                printf("name %s, %s, %p", qPrintable(objectThread->objectName()), objectThread->metaObject()->className(), t);;
                jclass cl = env->GetObjectClass(t);
//                qDebug() << ".. au ..";

                if (qtjambi_class_name(env, cl) == QLatin1String("org.qtjambi.qt.QThread")) {
    //                 printf(" - delete later in QThread=%p %s [%s]\n",
    //                        objectThread,
    //                        qPrintable(qobj->objectName()),
    //                        qobj->metaObject()->className());
                    m_delete_later = 1;
                    QTJAMBI_DEBUG_TRACE_WITH_THREAD("call PointerContainer::deleteLater()");
                    m_pointerContainer->deleteLater();
                    rv = 0x0002;    // called deleteLater()
                } else if (QCoreApplication::instance()) {
                    // Make we should explain to use moveToThread()
                    qWarning("QObjects can only be implicitly garbage collected when owned"
                            " by a QThread, native resource ('%s' [%s]) is leaked",
                            qPrintable(m_pointerContainer->objectName()),
                            m_pointerContainer->metaObject()->className());
                    rv = 0x1000;    // error? or not?
                }

    //             StaticCache *sc = StaticCache::instance();
    //             sc->resolveQThread();
    //             if (env->IsSameObject(cl, sc->QThread.class_ref)) {
    //                 qobj->deleteLater();

    //             } else {
    //                 // ## Resolve QThreadAffinityException...
    //                 // Message: "QObjects can only be implicitly garbage collected when owned by a QThread".
    //                 qWarning("something really bad happened...");
    //             }
            } else {
                m_user_data_skip = 1;  // inhibit UserData
                QTJAMBI_DEBUG_TRACE_WITH_THREAD("call delete PointerContainer");
                delete m_pointerContainer;
                m_ptr_shared_pointer = 0;
                setAsQObjectDeleted();  // put here due to m_user_data_skip=1
                rv = 0x0001;    // called delete
            }
            env->DeleteLocalRef(t);
        }

        // If flagged that this object is now pending deleteLater() but we don't zap the m_pointer
        // as there maybe other events to process on the C++ object ahead of the deletion action.
        if(!m_delete_later){
            m_pointer = 0;
        }
        m_pointerContainer = 0;

        QTJAMBI_DEBUG_TRACE_WITH_THREAD("(native) -> leaving: QtPointerJambiLink::deleteNativeObject(JNIEnv *)");
    }
    return rv;
}

void QtPointerJambiLink::setCppOwnership(JNIEnv *, jobject){}
void QtPointerJambiLink::setSplitOwnership(JNIEnv *, jobject){}
void QtPointerJambiLink::setDefaultOwnership(JNIEnv *, jobject){}

void* QtPointerJambiLink::getPointerPointer(){
    return m_pointerContainer ? m_pointerContainer->getPointerPointer() : m_ptr_shared_pointer;
}

void* PointerContainer::getPointerPointer(){
    return m_ptr_shared_pointer;
}

PointerContainer::PointerContainer(void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, QObject* object):
    QObject(),
    m_ptr_shared_pointer(ptr_shared_pointer),
    m_shared_pointer_deleter(shared_pointer_deleter),
    m_object(object)
{
    this->setObjectName(QString("PointerContainer-for-%1").arg(m_object->objectName()));
    if(m_object){
        this->moveToThread(m_object->thread());
        QObject::connect(m_object, SIGNAL(destroyed(QObject *)), this, SLOT(onDestroyed(QObject*)));
    }
}

PointerContainer::~PointerContainer(){
    QTJAMBI_DEBUG_TRACE_WITH_THREAD("(native) entering: PointerContainer::~PointerContainer()");
    if(m_object){
        QObject::disconnect(m_object, SIGNAL(destroyed(QObject *)), this, SLOT(onDestroyed(QObject*)));
        m_object = Q_NULLPTR;
    }
    if(m_ptr_shared_pointer){
        QTJAMBI_DEBUG_TRACE_WITH_THREAD("(native) calling shared pointer deleter function");
        m_shared_pointer_deleter(m_ptr_shared_pointer);
        m_ptr_shared_pointer = Q_NULLPTR;
    }
    QTJAMBI_DEBUG_TRACE_WITH_THREAD("(native) -> leaving: PointerContainer::~PointerContainer()");
}

void PointerContainer::onDestroyed(QObject* object){
    QTJAMBI_DEBUG_TRACE_WITH_THREAD("(native) entering: PointerContainer::onDestroyed(QObject* object)");
    if(object && object==m_object){
        if(m_object){
            QObject::disconnect(m_object, SIGNAL(destroyed(QObject *)), this, SLOT(onDestroyed(QObject*)));
            m_object = Q_NULLPTR;
        }
        if(m_ptr_shared_pointer){
            QTJAMBI_DEBUG_TRACE_WITH_THREAD("(native) calling shared pointer deleter function");
            m_shared_pointer_deleter(m_ptr_shared_pointer);
            m_ptr_shared_pointer = Q_NULLPTR;
        }
    }
    QTJAMBI_DEBUG_TRACE_WITH_THREAD("(native) -> leaving: PointerContainer::onDestroyed(QObject* object)");
}
