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

#ifndef QTJAMBISHELL_H
#define QTJAMBISHELL_H

#include "qtjambi_global.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QVector>
#include <QtCore/QMetaType>
#include <QtCore/QEvent>
#if defined(QTJAMBI_DEBUG_TOOLS)
 #include <QtCore/QMutex>
 #include <QtCore/QAtomicInt>
#endif

class QtJambiLink;
class QtPointerJambiLink;
typedef void(* PointerDeleter)(void *);
typedef void*(* PointerCreator)(void *);

#define PACKAGEPATH "org/qtjambi/"
#define EXCEPTIONPATHN PACKAGEPATH"exceptions/"
#define EXCEPTIONPATH(CLASS) EXCEPTIONPATHN#CLASS
#define KERNELPATHN PACKAGEPATH"kernel/"
#define KERNELPATH(CLASS) KERNELPATHN#CLASS
#define QTPATHN PACKAGEPATH"qt/"
#define QTPATH(CLASS) QTPATHN#CLASS

// Allow useful truncation on 32bit
#define QTJAMBI_LINK_USER_DATA_MAGIC ((long)0xf6f700fff5f42f03ll)
#define QTJAMBI_LINK_MAGIC ((long)0xf6f700fff5f42f04ll)

struct QtJambiLinkUserData : public QObjectUserData
{
    QtJambiLinkUserData(QtJambiLink *link)
      :
#if defined(QTJAMBI_DEBUG_TOOLS)
        m_magic(QTJAMBI_LINK_USER_DATA_MAGIC),
#endif
        m_link(link),
        m_metaObject(0)
     { }
    virtual ~QtJambiLinkUserData();

    inline QtJambiLink *link() { return m_link; }
    inline void invalidateLink() { m_link = 0; }

    inline void setMetaObject(const QMetaObject *mo) { m_metaObject = mo; }
    inline const QMetaObject *metaObject() const { return m_metaObject; }

    static int id();

#if defined(QTJAMBI_DEBUG_TOOLS)
    void validateMagic_unlocked(const char *prefix, bool validate_children);
    void validateMagic(bool validate_children = true);
    int acquireMagic_unlocked(int source_id);
    void acquireMagic(int source_id, bool validate_children = true);
    int releaseMagic_unlocked(int source_id);
    void releaseMagic(int source_id, bool validate_children = true);
#endif

private:
#if defined(QTJAMBI_DEBUG_TOOLS)
    long m_magic;    // natural bit width
#endif
    QtJambiLink *m_link;
    const QMetaObject *m_metaObject;
#if defined(QTJAMBI_DEBUG_TOOLS)
    QMutex m_mutex;
    QAtomicInt m_atomic_int;
    void *m_pthread_id;
    long m_magic_source_id;	// gluttony
#endif
};

/*
  A QtJambiLink is owned by Java, it will be deleted when the
  java object is finalized.
 */
class QTJAMBI_EXPORT QtJambiLink
{
public:
    enum Type{
        NormalLink,
        PointerLink
    };

private:
    inline QtJambiLink(jobject jobj, bool global_ref, bool is_qobject, void *pointer)
        : 
#if defined(QTJAMBI_DEBUG_TOOLS)
          m_magic(QTJAMBI_LINK_MAGIC),
#endif
          //m_java.object(jobj),
          m_pointer(pointer),
          m_ownership(SplitOwnership), // Default to split, because it's safest
          m_last_ownership(InvalidOwnership),
          m_global_ref(global_ref),
          m_is_qobject(is_qobject),
          m_has_been_finalized(false),
          m_qobject_deleted(false),
          m_created_by_java(false),
          m_object_invalid(false),
          m_in_cache(false),
          m_connected_to_java(false),
          m_delete_in_main_thread(false),
          m_java_link_removed(false),
          m_link_cacheable(false),
          m_delete_later(false),
          m_pointer_zeroed(false),
          m_pointer_zapped(false),
          m_user_data_skip(false),
          m_reserved1(0),
#if defined(QTJAMBI_DEBUG_TOOLS)
          m_in_qtjambilink_list(0),
#else
          m_reserved2(0),
#endif
#if QT_VERSION < 0x050000
          m_meta_type(QMetaType::Void),
#else
          m_meta_type(QMetaType::UnknownType),
#endif
          m_wrapper(0),
          m_destructor_function(0),
          m_type(NormalLink)
#if defined(QTJAMBI_DEBUG_TOOLS)
          , m_qtJambiLinkUserData(0)
          , next(0)
          , prev(0)
#endif
    {
        m_java.object = jobj;
#if defined(QTJAMBI_DEBUG_TOOLS)  
        QtJambiLinkList_add();
#endif
    };

    inline QtJambiLink(jobject jobj, bool is_qobject, void *pointer)
        : 
#if defined(QTJAMBI_DEBUG_TOOLS)
          m_magic(QTJAMBI_LINK_MAGIC),
#endif
          //m_java.object(jobj),
          m_pointer(pointer),
          m_ownership(SplitOwnership), // Default to split, because it's safest
          m_last_ownership(InvalidOwnership),
          m_global_ref(true),          // we are jobject
          m_is_qobject(is_qobject),
          m_has_been_finalized(false),
          m_qobject_deleted(false),
          m_created_by_java(false),
          m_object_invalid(false),
          m_in_cache(false),
          m_connected_to_java(false),
          m_delete_in_main_thread(false),
          m_java_link_removed(false),
          m_link_cacheable(false),
          m_delete_later(false),
          m_pointer_zeroed(false),
          m_pointer_zapped(false),
          m_user_data_skip(false),
          m_reserved1(0),
#if defined(QTJAMBI_DEBUG_TOOLS)
          m_in_qtjambilink_list(0),
#else
          m_reserved2(0),
#endif
#if QT_VERSION < 0x050000
          m_meta_type(QMetaType::Void),
#else
          m_meta_type(QMetaType::UnknownType),
#endif
          m_wrapper(0),
          m_destructor_function(0),
          m_type(NormalLink)
#if defined(QTJAMBI_DEBUG_TOOLS)
          , m_qtJambiLinkUserData(0)
          , next(0)
          , prev(0)
#endif
    {
        m_java.object = jobj;
#if defined(QTJAMBI_DEBUG_TOOLS)  
        QtJambiLinkList_add();
#endif
    };

#ifdef _CLASSPATH_VM_JNI_TYPES_DEFINED
    /* We are being built under GNU ClassPath */
    inline QtJambiLink(jweak jobj, bool is_qobject, void *pointer)
        : 
#if defined(QTJAMBI_DEBUG_TOOLS)
          m_magic(QTJAMBI_LINK_MAGIC),
#endif
          //m_java.weak(jobj),
          m_pointer(pointer),
          m_ownership(SplitOwnership), // Default to split, because it's safest
          m_last_ownership(InvalidOwnership),
          m_global_ref(false),         // we are jweak!
          m_is_qobject(is_qobject),
          m_has_been_finalized(false),
          m_qobject_deleted(false),
          m_created_by_java(false),
          m_object_invalid(false),
          m_in_cache(false),
          m_connected_to_java(false),
          m_delete_in_main_thread(false),
          m_java_link_removed(false),
          m_link_cacheable(false),
          m_delete_later(false),
          m_pointer_zeroed(false),
          m_pointer_zapped(false),
          m_user_data_skip(false),
          m_reserved1(0),
#if defined(QTJAMBI_DEBUG_TOOLS)
          m_in_qtjambilink_list(0),
#else
          m_reserved2(0),
#endif
#if QT_VERSION < 0x050000
          m_meta_type(QMetaType::Void),
#else
          m_meta_type(QMetaType::UnknownType),
#endif
          m_wrapper(0),
          m_destructor_function(0),
          m_type(NormalLink)
#if defined(QTJAMBI_DEBUG_TOOLS)
          , m_qtJambiLinkUserData(0)
          , next(0)
          , prev(0)
#endif
    {
        m_java.weak = jobj;
#if defined(QTJAMBI_DEBUG_TOOLS)  
        QtJambiLinkList_add();
#endif
    };
#endif  // _CLASSPATH_VM_JNI_TYPES_DEFINED

protected:
    inline QtJambiLink(jobject jobj)
        : 
#if defined(QTJAMBI_DEBUG_TOOLS)
          m_magic(QTJAMBI_LINK_MAGIC),
#endif
          //m_java.object(jobj),
          m_pointer(0),
          m_ownership(SplitOwnership), // Default to split, because it's safest
          m_last_ownership(InvalidOwnership),
          m_global_ref(true),          // we are jobject
          m_has_been_finalized(false),
          m_qobject_deleted(false),
          m_created_by_java(false),
          m_object_invalid(false),
          m_in_cache(false),
          m_connected_to_java(false),
          m_delete_in_main_thread(false),
          m_java_link_removed(false),
          m_link_cacheable(false),
          m_delete_later(false),
          m_pointer_zeroed(false),
          m_pointer_zapped(false),
          m_user_data_skip(false),
          m_reserved1(0),
#if defined(QTJAMBI_DEBUG_TOOLS)
          m_in_qtjambilink_list(0),
#else
          m_reserved2(0),
#endif
#if QT_VERSION < 0x050000
          m_meta_type(QMetaType::Void),
#else
          m_meta_type(QMetaType::UnknownType),
#endif
          m_wrapper(0),
          m_destructor_function(0),
          m_type(NormalLink)
#if defined(QTJAMBI_DEBUG_TOOLS)
          , m_qtJambiLinkUserData(0)
          , next(0)
          , prev(0)
#endif
    {
        m_java.object = jobj;
#if defined(QTJAMBI_DEBUG_TOOLS)  
        QtJambiLinkList_add();
#endif
    };

public:
    enum Ownership {
        JavaOwnership,   // Weak ref to java object, deleteNativeObject deletes c++ object
        CppOwnership,    // Strong ref to java object until c++ object is deleted, deleteNativeObject does *not* delete c++ obj.
        SplitOwnership,  // Weak ref to java object, deleteNativeObject does *not* delete c++ object. Only for objects not created by Java.
        InvalidOwnership // Only meaningful for m_last_ownership as null/not-set placeholder
    };

    virtual ~QtJambiLink();

    /* Returns the pointer value, wether its a QObject or plain object */
    inline void *pointer() const { return m_pointer; }

    /* Returns the pointer value as an object, will assert if pointer
       is a QObject */
    inline void *object() const { Q_ASSERT(!isQObject()); return m_pointer; }
    void resetObject(JNIEnv *env);

    /* Returns the pointer value for the signal wrapper, will assert if pointer is not a QObject */
    inline QObject *signalWrapper() const { Q_ASSERT(isQObject()); return m_wrapper; }
    inline void setSignalWrapper(QObject *ptr) { m_wrapper = ptr; }

    inline jobject javaObject(JNIEnv *env) const;
    /* Obviously don't go changing it */
    inline void javaObjectDeleteIfLocalRef(JNIEnv *env, jobject o) const;
    inline void javaObjectDeleteIfLocalRef(JNIEnv *env, jobject o, bool global_ref) const;

    /* Returns the pointer value as a QObject, will assert if pointer
       is not a QObject */
    inline QObject *qobject() const { Q_ASSERT(isQObject()); return reinterpret_cast<QObject *>(m_pointer); }

    /* Sets the QObject user data's meta object pointer. Object must be a QObject */
    void setMetaObject(const QMetaObject *mo) const;

    inline int metaType() const { return m_meta_type; }
    void setMetaType(int metaType);

    inline bool isCached() const { return m_in_cache; }

    /* Returns true if this link holds a global reference to the java
       object, meaning that the java object will not be
       finalized. This is for widgets mostly. */
    inline bool isGlobalReference() const { return m_global_ref; }

    /* Returns true if the link has ownership over the data. */
    inline bool hasOwnership() const { return !isQObject() || !isGlobalReference(); }

    inline bool isQObject() const { return m_is_qobject; }

    /* Deletes any global references to the java object so that it can
       be finalized by the virtual machine.
       Returns disposition bitmask. */
    int releaseJavaObject(JNIEnv *env);

    /* Deletes the native object.
       Returns disposition bitmask. */
    virtual int deleteNativeObject(JNIEnv *env);

    /* Triggered by native jni functions when a java object has been
       finalized. */
    void javaObjectFinalized(JNIEnv *env);

    /* Called by the native jni fucntion when the java object has been
       disposed */
    void javaObjectDisposed(JNIEnv *env);

    /* Called by the shell destructor */
    void nativeShellObjectDestroyed(JNIEnv *env);

    /* Called when java object is invalidated */
    void javaObjectInvalidated(JNIEnv *env);

    void registerSubObject(void *);
    void unregisterSubObject(void *);

    inline bool hasBeenFinalized() const { return m_has_been_finalized; }
    inline bool qobjectDeleted() const { return m_qobject_deleted; }
    inline PtrDeleterFunction destructorFunction() const { return m_destructor_function; }
    inline bool connectedToJava() const { return m_connected_to_java; }
    inline bool deleteInMainThread() const { return m_delete_in_main_thread; }
    inline void setAsQObjectDeleted() { m_qobject_deleted = true; }
    inline void setAsFinalized() { m_has_been_finalized = true; }
    inline void setDestructorFunction(PtrDeleterFunction dfnc) { m_destructor_function = dfnc; }
    inline void setConnectedToJava(bool c) { m_connected_to_java = c; }
    inline void setDeleteInMainThread(bool c) { m_delete_in_main_thread = c; }

    inline bool createdByJava() const { return m_created_by_java; }
    inline void setCreatedByJava(bool cbj) { m_created_by_java = cbj; }

    int indexQtSignal(const QByteArray &signal) const;
    int indexQtSlot(const QByteArray &slot) const;

    void disableGarbageCollection(JNIEnv *env, jobject java);

    virtual void setCppOwnership(JNIEnv *env, jobject java);
    void setJavaOwnership(JNIEnv *env, jobject java);
    virtual void setSplitOwnership(JNIEnv *env, jobject java);
    virtual void setDefaultOwnership(JNIEnv *env, jobject java);

    bool javaLinkRemoved() const { return m_java_link_removed; }

    Ownership ownership() const { return Ownership(m_ownership); }

    bool isPointer() const { return m_type==PointerLink; }

    void setGlobalRef(JNIEnv *env, bool global);

    bool isDeleteLater() const { return m_delete_later; }
    bool isPointerZeroed() const { return m_pointer_zeroed; }
    bool isUserDataSkip() const { return m_user_data_skip; }

    // For debug messages
    jobject getJavaObject() const { return m_java.object; }
    bool isPointerZapped() const { return m_pointer_zapped; }
    const char *debugFlagsToString(char *buf) const;

    static QtJambiLink *createLinkForObject(JNIEnv *env, jobject java, void *ptr, const QString &java_name,
        bool enter_in_cache, void* ptr_shared_pointer, PointerDeleter pointerDeleter);
    static QtJambiLink *createLinkForQObject(JNIEnv *env, jobject java, QObject *object, void* ptr_shared_pointer, PointerDeleter pointerDeleter);
    static QtJambiLink *createWrapperForQObject(JNIEnv *env, QObject *o, const char *class_name,
        const char *package_name, void* ptr_static_pointer, PointerDeleter pointerDeleter);

    static QtJambiLink *findLink(JNIEnv *env, jobject java);
    static inline QtJambiLink *findQObjectLink(JNIEnv *env, jobject java);

    static QtJambiLink *findLinkForQObject(QObject *qobject);
    static QtJambiLink *findLinkForUserObject(const void *ptr);

    static jmethodID findMethod(JNIEnv *env, jobject java, const QString &method);

    static QString nameForClass(JNIEnv *env, jclass clazz);
    static bool stripQtPackageName(QString *className);
    static bool throwQtException(JNIEnv *env, const QString &extra, const QString &name);

#if defined(QTJAMBI_DEBUG_TOOLS)
    void validateMagic_unlocked(const char *prefix, bool validate_children);
    void validateMagic(bool validate_children = true);
    int acquireMagic_unlocked(int source_id);
    void acquireMagic(int source_id, bool validate_children = true);
    int releaseMagic_unlocked(int source_id);
    void releaseMagic(int source_id, bool validate_children = true);
    // Method shared by QtJambiLinkUserData for description
    static const char *acquire_magic_source_id_to_string(int source_id);
#endif

private:
    void setNativeId();
    int cleanUpAll(JNIEnv *env);
    void removeFromCache(JNIEnv *env);
    void aboutToMakeObjectInvalid(JNIEnv *env);


#if defined(QTJAMBI_DEBUG_TOOLS)
    long m_magic;   // natural bit width
#endif

    // This union arrangement helps building on top of GNU ClassPath and maybe
    //  other JVMs using C++ with strict types where jweak != jobject.
    union {
        jobject object;
        jweak weak;
    } m_java;
    #define m_java_object m_java.object
    #define m_java_weak   m_java.weak

    void *m_pointer;

    uint m_ownership : 2;
    uint m_last_ownership : 2;         // used to track
    uint m_global_ref : 1;
    uint m_is_qobject : 1;
    uint m_has_been_finalized : 1;
    uint m_qobject_deleted : 1;
    uint m_created_by_java : 1;
    uint m_object_invalid : 1;
    uint m_in_cache : 1;
    uint m_connected_to_java : 1;
    uint m_delete_in_main_thread : 1;
    uint m_java_link_removed : 1;
    uint m_link_cacheable : 1;         // was this once upon a time: enter_in_cache==true
    uint m_delete_later : 1;           // used to track that QtJambiLink called deleteLater() so can expect event loop dtor to work on 'this'
    uint m_pointer_zeroed : 1;         // used to indicate that we have set m_pointer=0 but the original pointer may still be active and out there
    uint m_pointer_zapped : 1;         // used to indicate we set m_pointer=0 but have no clear idea how the C++ object will be freed
    uint m_user_data_skip : 1;         // used to make QtJambiLinkUserData skip m_link destruction
    uint m_reserved1 : 12;
#if defined(QTJAMBI_DEBUG_TOOLS)
    uint m_in_qtjambilink_list : 1;
#else
    uint m_reserved2 : 1;
#endif

    int m_meta_type;

    QObject *m_wrapper;

    PtrDeleterFunction m_destructor_function;

    Type m_type;

#if defined(QTJAMBI_DEBUG_TOOLS)  
    // This is a temporary fix and the fallback solution to fine grained locking.
    // On Intel platforms we can use a 8bit atomic spinlock which will not increase
    //  the size of QtJambiLink and still protect the flags.
    QMutex m_mutex;
    QAtomicInt m_atomic_int;
    void *m_pthread_id;
    long m_magic_source_id;	// gluttony, but we're aligned gluttony
#endif

#if defined(QTJAMBI_DEBUG_TOOLS)
    static QtJambiLink *QtJambiLinkList_head;
    static QtJambiLink *QtJambiLinkList_tail;

    void QtJambiLinkList_add();
    void QtJambiLinkList_remove();

public:
    static int QtJambiLinkList_count();
    static int QtJambiLinkList_dump();
    static bool QtJambiLinkList_check(QtJambiLink *find);

    QtJambiLinkUserData *m_qtJambiLinkUserData;
    QString m_className;
    QtJambiLink *next;
    QtJambiLink *prev;
#endif
    friend QtPointerJambiLink;
};

class PointerContainer : public QObject{
    Q_OBJECT

public:
    explicit PointerContainer(void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, QObject* object);

    virtual ~PointerContainer();
    void* getPointerPointer();
public slots:
    void onDestroyed(QObject* object);
private:
    void* m_ptr_shared_pointer;
    PointerDeleter m_shared_pointer_deleter;
    QObject* m_object;
};

class QtPointerJambiLink : public QtJambiLink{
public:
    inline QtPointerJambiLink(jobject jobj, bool global_ref, void *pointer, PointerContainer* pointerContainer)
        : QtJambiLink(jobj, global_ref, true, pointer),
          m_pointerContainer(pointerContainer),
          m_ptr_shared_pointer(0),
          m_shared_pointer_deleter(0){
        m_type = PointerLink;
    }

    inline QtPointerJambiLink(jobject jobj, bool global_ref, void *pointer, void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter)
        : QtJambiLink(jobj, global_ref, false, pointer),
          m_pointerContainer(0),
          m_ptr_shared_pointer(ptr_shared_pointer),
          m_shared_pointer_deleter(shared_pointer_deleter){
        m_type = PointerLink;
    }

    ~QtPointerJambiLink();

    int deleteNativeObject(JNIEnv *env);

    void setCppOwnership(JNIEnv *env, jobject java);
    void setSplitOwnership(JNIEnv *env, jobject java);
    void setDefaultOwnership(JNIEnv *env, jobject java);

    void* getPointerPointer();
private:
    PointerContainer* m_pointerContainer;
    void* m_ptr_shared_pointer;
    PointerDeleter m_shared_pointer_deleter;
};

#if defined(QTJAMBI_DEBUG_TOOLS)
 // These are used with acquireMagic() methods for debugging so that the purpose of the original acquisition is recorded.
 #define QTJAMBI_MAGIC_QTJAMBILINK_DTOR 1
 #define QTJAMBI_MAGIC_JAVAOBJECTFINALIZED 2
 #define QTJAMBI_MAGIC_JAVAOBJECTDISPOSED 3
 #define QTJAMBI_MAGIC_QTJAMBILINKUSERDATA_DTOR 4
 #define QTJAMBI_MAGIC_QTJAMBILINKUSERDATA_DTOR2 5
#endif


inline jobject QtJambiLink::javaObject(JNIEnv *env) const
{
    if (m_global_ref)
        return m_java.object;
    else
        return env->NewLocalRef(m_java.object);	// this is null-safe
}

inline void QtJambiLink::javaObjectDeleteIfLocalRef(JNIEnv *env, jobject o, bool global_ref) const
{
    if (!global_ref)
        env->DeleteLocalRef(o);
    return;      // noop for global_ref==true
}

inline void QtJambiLink::javaObjectDeleteIfLocalRef(JNIEnv *env, jobject o) const
{
    if (!m_global_ref)
        env->DeleteLocalRef(o);
    return;      // noop for m_global_ref==true
}

inline QtJambiLink *QtJambiLink::findQObjectLink(JNIEnv *env, jobject java)
{
    QtJambiLink *link = findLink(env, java);
    return link && link->isQObject() ? link : 0;
}

#endif
