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

#include "qtjambi_core.h"
#include "qtjambi_cache.h"
#include "qtjambifunctiontable.h"
#include "qtjambilink.h"
#include "qtjambitypemanager_p.h"
#include "qtjambidestructorevent_p.h"
#include "qtdynamicmetaobject.h"
#include "qtjambivariant_p.h"

#include <common/qnativepointer.h>

#include <qglobal.h>

#include <QtCore/QAbstractItemModel>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QEvent>
#include <QtCore/QFileInfo>
#include <QtCore/QLibrary>
#include <QtCore/QMetaMethod>
#include <QtCore/QMetaObject>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QThread>
#include <QtCore/private/qthread_p.h>
#include <QtCore/QVariant>
#include <QtCore/QTextStream>
#include <QtCore/QMutex>
#include <QtWidgets/QWidget>
#if QT_VERSION < 0x050000
#include <QtGui/QStyleOption>
#endif
#include <stdio.h>
#include <string.h>

#if defined(QTJAMBI_DEBUG_TOOLS)
 #include "qtjambidebugevent_p.h"
#endif
            
static QStringList locate_vm();

typedef jint (JNICALL *PtrGetDefaultJavaVMInitArgs)(void *);
typedef jint (JNICALL *PtrCreateJavaVM)(JavaVM **, void **, void *);
typedef jint (JNICALL *PtrGetCreatedJavaVMs)(JavaVM **vmBuf, jsize bufLen, jsize *nVMs);

static PtrGetDefaultJavaVMInitArgs ptrGetDefaultJavaVMInitArgs;
static PtrCreateJavaVM ptrCreateJavaVM;
static PtrGetCreatedJavaVMs ptrGetCreatedJavaVMs;

static AbortHook abort_hook;
static ExitHook exit_hook;
static VfPrintFHook vfprintf_hook;

extern "C" QTJAMBI_EXPORT void qtjambi_set_vm_abort_hook(AbortHook hook){
    abort_hook = hook;
}

extern "C" QTJAMBI_EXPORT void qtjambi_set_vm_exit_hook(ExitHook hook){
    exit_hook = hook;
}

extern "C" QTJAMBI_EXPORT void qtjambi_set_vm_vfprintf_hook(VfPrintFHook hook){
    vfprintf_hook = hook;
}

static QString vm_location_override;

// C-style wrapper for qInstallMsgHandler so the launcher can look it up dynamically
// without bothering with knowing the name mangling
//extern "C" QTJAMBI_EXPORT QtMsgHandler wrap_qInstallMsgHandler(QtMsgHandler handler)
//{
//    return qInstallMsgHandler(handler);
//}


// accessor for protected function receiver
class Qaffeine: public QObject
{
public:
    inline int receivers(const char *signal) { return QObject::receivers(signal); }
};

// #define JOBJECT_REFCOUNT

Q_GLOBAL_STATIC_WITH_ARGS(QMutex, qtjambi_vm_lock, (QMutex::NonRecursive));
static int qtjambi_vm_count = 0;
static JavaVM *qtjambi_vm = 0;
/*!
 * This is set when we're shutting down.
 * Use of volatile keyword to force compiler to already reload the value everytime
 * it is referenced and to always store the value at the sequence point indicated
 * by code path.
 * 0 = Qt is operational, 1 = Qt is shutting down, 2 = Qt has not started (important it is non-zero)
 */
static volatile int qtjambi_vm_shutdown = 2;

int qtjambi_vm_shutdown_get()
{
    return qtjambi_vm_shutdown;
}

void qtjambi_vm_shutdown_set(int new_value)
{
    qtjambi_vm_shutdown = new_value;
}

/*!
 * This function is called by Qt Jambi shutdown hook to indicate that
 * the library is being unloaded. It prevents any access to the
 * current JNIEnv *.
 */
void qtjambi_shutdown()
{
    qtjambi_vm = 0;
}

/*!
 * This function is called by the Virtual Machine when it loads the
 * library. We need this to get a hold of the global VM pointer
 */
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(JNI_OnLoad)(JavaVM *vm, void *)
{
    qtjambi_vm_lock()->lock();
    if(qtjambi_vm_count == 0)
        qtjambi_vm = vm;
    qtjambi_vm_count++;
    qtjambi_vm_shutdown_set(0);		// Qt becomes operational
    qtjambi_vm_lock()->unlock();
    return JNI_VERSION_1_4;
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(JNI_OnUnload)(JavaVM *vm, void *reserved)
{
    Q_UNUSED(vm);
    Q_UNUSED(reserved);
    qtjambi_vm_lock()->lock();
    if(qtjambi_vm_count <= 1)
        qtjambi_shutdown();
    qtjambi_vm_count--;
    qtjambi_vm_lock()->unlock();
}

typedef QHash<QThread *, jobject> ThreadTable;

Q_GLOBAL_STATIC(ThreadTable, qtjambi_thread_table);
Q_GLOBAL_STATIC(QReadWriteLock, qtjambi_thread_table_lock);

void jobjectwrapper_save(QDataStream &stream, const void *_jObjectWrapper)
{
    JObjectWrapper *jObjectWrapper = (JObjectWrapper *) _jObjectWrapper;
    JNIEnv *env = qtjambi_current_environment();
    StaticCache *sc = StaticCache::instance();
    sc->resolveQtJambiInternal();

    jobject jstream = qtjambi_from_object(env, &stream, "QDataStream", "org/qtjambi/qt/core/", false);
	QTJAMBI_EXCEPTION_CHECK(env);

    env->CallStaticVoidMethod(sc->QtJambiInternal.class_ref, sc->QtJambiInternal.writeSerializableJavaObject, jstream, jObjectWrapper->object);
	QTJAMBI_EXCEPTION_CHECK(env);
}

void jobjectwrapper_load(QDataStream &stream, void *_jObjectWrapper)
{
    JObjectWrapper *jObjectWrapper = (JObjectWrapper *) _jObjectWrapper;
    JNIEnv *env = qtjambi_current_environment();
    StaticCache *sc = StaticCache::instance();
    sc->resolveQtJambiInternal();

    jobject jstream = qtjambi_from_object(env, &stream, "QDataStream", "org/qtjambi/qt/core/", false);
	QTJAMBI_EXCEPTION_CHECK(env);

    jobject res = env->CallStaticObjectMethod(sc->QtJambiInternal.class_ref, sc->QtJambiInternal.readSerializableJavaObject, jstream);
	QTJAMBI_EXCEPTION_CHECK(env);
    *jObjectWrapper = JObjectWrapper(env, res);
}


#undef qtjambi_exception_check
/*!  Checks for an exception in the virtual machine and returns true
  if there was one; otherwise returns false. The function will print
  the stack trace of any exception before clearing the exception state
  in the virtual machine.
*/
bool qtjambi_exception_check(JNIEnv *env, const char *fileName, int lineNumber)
{
    if (env->ExceptionCheck()) {
        if (fileName != 0)
            fprintf(stderr, "QtJambi: Exception pending in native code in file %s:%d\n", fileName, lineNumber);
        else
            fprintf(stderr, "QtJambi: Exception pending in native code\n");

        jmethodID mid = 0;  // used as flag to indicate we managed to call Throwable#printStackTrace()
        {
            jthrowable throbj = env->ExceptionOccurred();
            if(throbj) {
                // Need to clear the exception to get FindClass() to work.
                env->ExceptionClear();
                // FIXME: Cacheable
                // Can't call FindClass() with an exception pending (it will return 0)
                jclass clazz = env->FindClass("java/lang/Throwable");
                // IsInstanceOf might be PARANOIA ?
                if(clazz && env->IsInstanceOf(throbj, clazz)) {
                    mid = env->GetMethodID(clazz, "printStackTrace", "()V");
                    if(mid)
                        env->CallVoidMethod(throbj, mid);
                }
                // We cleanup all our local references here because:
                //  * exception handling is not expected for a performance code path so we can
                //    afford the performance due to extra code,
                //  * we don't know if the caller is using is in a loop or other long running
                //    code path,
                //  * we expect to be used multiple times in many APIs
                if(clazz)
                    env->DeleteLocalRef(clazz);
                env->DeleteLocalRef(throbj);
            }
        }
        // This isn't very useful as its not where the exception really occured a nested
        // exception however would be perfect (as long as we keep the original kind).
        // FIXME: This should go via MessageHandler / allowed to be rethrown and bubble upwards
        /*if(!mid)*/ {
            fprintf(stderr, "QtJambi: Exception pending in native code (detected at)\n");
            env->ExceptionDescribe();
        }
        env->ExceptionClear();
        return true;
    }
    return false;
}

bool qtjambi_exception_check(JNIEnv *env)
{
    return qtjambi_exception_check(env, 0, 0);
}


extern uint qHash(const char *);
struct CharPtr
{
    CharPtr(const char *_ptr) { ptr = _ptr; }
    bool operator==(CharPtr other) const { return strcmp(other.ptr, ptr) == 0; }
    const char *ptr;
};
uint qHash(CharPtr ptr) { return qHash(ptr.ptr); }


typedef QMultiHash<CharPtr, QtJambiPolymorphicHandler> PolymorphicIdHash;
Q_GLOBAL_STATIC(PolymorphicIdHash, g_polymorphic_ids);
Q_GLOBAL_STATIC(QReadWriteLock, g_polymorphic_id_lock);

void qtjambi_register_polymorphic_id(const char *lookup, QtJambiPolymorphicHandler handler)
{
    QWriteLocker locker(g_polymorphic_id_lock());
    g_polymorphic_ids()->insert(lookup, handler);
}

void qtjambi_resolve_polymorphic_id(const char *lookup, const void *object,
                                    const char **class_name, const char **package)
{
    QReadLocker locker(g_polymorphic_id_lock());
    QList<QtJambiPolymorphicHandler> handlers = g_polymorphic_ids()->values(lookup);
    for (int i=0; i<handlers.size(); ++i) {
        if (handlers[i](object, class_name, package))
            break;
    }
}

/*!
 * Fetches the current environment based on the global Virtual
 * Machine. This function will fail if the current thread is not
 * attached, but that should never happen.
 */
JNIEnv *qtjambi_current_environment()
{
    JavaVM *vm;
    JNIEnv *env;

    vm = qtjambi_vm;	// FIXME: Want this to be an atomic read
    if (vm == 0) {
        return 0;
    }
    // FIXME: We at least need to do an atomic read of qtjambi_vm and then test and
    //  reuse that value in this method.  Not keep accessing the global directly.
    int result = vm->GetEnv( (void **) (void *) (&env), JNI_VERSION_1_4);
    if (result == JNI_EDETACHED) {
        if (vm->AttachCurrentThreadAsDaemon((void **) (void *) (&env), 0) < 0) {
            qWarning("Failed attaching current thread");
            return 0;
        }
    } else {
        Q_ASSERT(result == JNI_OK);
    }
    return env;
}

QString qtjambi_class_name(JNIEnv *env, jclass java_class)
{
    Q_ASSERT(java_class);
    StaticCache *sc = StaticCache::instance();
    sc->resolveClass();
    Q_ASSERT(java_class);
    Q_ASSERT(env->IsInstanceOf(java_class, sc->Class.class_ref));  // check the java object is right type
    jstring name = (jstring) env->CallObjectMethod(java_class, sc->Class.getName);
    return qtjambi_to_qstring(env, name);
}

QString qtjambi_object_class_name(JNIEnv *env, jobject java_object)
{
    return qtjambi_class_name(env, env->GetObjectClass(java_object));
}

jobject qtjambi_from_qvariant(JNIEnv *env, const QVariant &qt_variant)
{
    int type = qMetaTypeId<JObjectWrapper>();

    StaticCache *sc = StaticCache::instance();

    switch (qt_variant.userType()) {
    case QVariant::Invalid: return 0;
    case QVariant::Int:
    case QVariant::UInt:
        sc->resolveInteger();
        return env->NewObject(sc->Integer.class_ref, sc->Integer.constructor, qt_variant.toInt());
    case QVariant::Double:
        sc->resolveDouble();
        return env->NewObject(sc->Double.class_ref, sc->Double.constructor, qt_variant.toDouble());
    case QVariant::String:
        sc->resolveString();
        return qtjambi_from_qstring(env, qt_variant.toString());
    case QVariant::LongLong:
    case QVariant::ULongLong:
        sc->resolveLong();
        return env->NewObject(sc->Long.class_ref, sc->Long.constructor, qt_variant.toLongLong());
    case QVariant::Bool:
        sc->resolveBoolean();
        return env->NewObject(sc->Boolean.class_ref, sc->Boolean.constructor, qt_variant.toBool());
    }


    // generic java object
    if (qt_variant.userType() == type) {
#if QT_VERSION < 0x050000
        JObjectWrapper wrapper = qVariantValue<JObjectWrapper>(qt_variant);
#else
		JObjectWrapper wrapper = qt_variant.value<JObjectWrapper>();
#endif
        return env->NewLocalRef(wrapper.object);
    } else {
        QString qtType = QLatin1String(qt_variant.typeName());

        QtJambiTypeManager manager(env);
        QString javaType = manager.getExternalTypeName(qtType, QtJambiTypeManager::ArgumentType);
        void *copy = 0;
        if(qt_variant.isNull())
            qWarning("qVariant is null");
        bool ok = manager.convertInternalToExternal(qt_variant.constData(), &copy,
            qtType, javaType, QtJambiTypeManager::ReturnType, true);

        jobject java_object = 0;
        if (ok && copy) {
            java_object = (reinterpret_cast<jvalue *>(copy))->l;
            manager.destroyExternal(copy, QtJambiTypeManager::ReturnType);
        }

        return java_object;
    }
}

QVariant qtjambi_to_qvariant(JNIEnv *env, jobject java_object)
{
    if (java_object == 0)
        return QVariant();

    jclass object_class = env->GetObjectClass(java_object);
    if (object_class == 0)
        return QVariant();

    // Test some quick ones first...
    StaticCache *sc = StaticCache::instance();
    sc->resolveString();
    sc->resolveInteger();
    sc->resolveDouble();
    sc->resolveLong();
    sc->resolveBoolean();
    if (env->IsSameObject(sc->String.class_ref, object_class)) {
        return qtjambi_to_qstring(env, static_cast<jstring>(java_object));
    } else if (env->IsSameObject(sc->Integer.class_ref, object_class)) {
        return (qint32) env->CallIntMethod(java_object, sc->Integer.intValue);
    } else if (env->IsSameObject(sc->Double.class_ref, object_class)) {
        return env->CallDoubleMethod(java_object, sc->Double.doubleValue);
    } else if (env->IsSameObject(sc->Long.class_ref, object_class)) {
        return (qint64) env->CallLongMethod(java_object, sc->Long.longValue);
    } else if (env->IsSameObject(sc->Boolean.class_ref, object_class)) {
        return QVariant((bool) env->CallBooleanMethod(java_object, sc->Boolean.booleanValue));
    }

    // Do the slightly slower fallback...
    QString fullName = qtjambi_class_name(env, object_class).replace(".", "/");

    JObjectWrapper wrapper(env, java_object);
    QtJambiTypeManager manager(env);
    // may it allow enum and flags?
    QString className = manager.getInternalTypeName(fullName, QtJambiTypeManager::ArgumentType);
    int type = !className.isEmpty()
        ? manager.metaTypeOfInternal(className, QtJambiTypeManager::ArgumentType)
        : (int)QVariant::Invalid;

    void *copy = 0;
    bool destroyCopy = false;
    if (type != QVariant::Invalid) {
        jvalue val;
        val.l = java_object;
        bool ok = manager.convertExternalToInternal(&val, &copy, fullName, className,
                                                    QtJambiTypeManager::ArgumentType);
        if (!ok)
            type = QVariant::Invalid;
        else
            destroyCopy = true;
    }

    if (type == QVariant::Invalid) {
        type = qMetaTypeId<JObjectWrapper>();
        copy = &wrapper;
    }

    QVariant returned = QVariant(type, copy);
    if (destroyCopy)
        manager.destroyInternal(copy, QtJambiTypeManager::ArgumentType);

//  qDebug() << fullName << className << returned.type() << returned.typeName();
    return returned;
}

void *qtjambi_to_object(JNIEnv *env, jobject java_object)
{
    if (QtJambiLink *link = QtJambiLink::findLink(env, java_object))
        return link->object();
    else
        return 0;
}

void *qtjambi_to_object(JNIEnv *env, jobject java_object, PointerCreator pointerCreator, PointerDeleter pointerDeleter)
{
    if(java_object){
        QtJambiLink *link = QtJambiLink::findLink(env, java_object);
        if(link){
            if(link->isPointer()){
                QtPointerJambiLink* pointerLink = static_cast<QtPointerJambiLink*>(link);
                return pointerLink->getPointerPointer();
            }else{
                void* pointer;
                if(link->isQObject()){
                    QObject* object = link->qobject();
                    pointer = pointerCreator((void*)object);
                    QtJambiLink::createLinkForQObject(env, java_object, object, pointer, pointerDeleter);
                }else{
                    bool createdByJava = link->createdByJava();
                    QString java_name = QtJambiLink::nameForClass(env, env->GetObjectClass(java_object));
                    void* object = link->object();
                    pointer = pointerCreator(object);
                    QtJambiLink::createLinkForObject(env, java_object, object, java_name, createdByJava, pointer, pointerDeleter);
                }
                link->releaseJavaObject(env);
                delete link;
                return pointer;
            }
        }
    }
    // this is a memory leak because this is not linked to java object!!!
    return pointerCreator(Q_NULLPTR);
}

QObject *qtjambi_to_qobject(JNIEnv *env, jobject java_object)
{
    if (java_object == 0)
        return 0;

    StaticCache *sc = StaticCache::instance();
    sc->resolveQtJambiObject();

    jlong id = env->GetLongField(java_object, sc->QtJambiObject.native_id);
    return id == 0
               ? 0
               : (reinterpret_cast<QtJambiLink *>(id))->qobject();
}

int qtjambi_to_enum(JNIEnv *env, jobject java_object)
{
    int returned = 0;
    Q_ASSERT(java_object);
    jclass clazz = env->GetObjectClass(java_object);
    if (clazz != 0) {
        jmethodID methodId = resolveMethod(env, "value", "()I", clazz);
        if (methodId == 0) {
            env->ExceptionClear();
//             qWarning("qtjambi_to_enum, method 'ordinal()' was not found");
            methodId = resolveMethod(env, "ordinal", "()I", clazz);
        }

        if (methodId != 0)
            returned = env->CallIntMethod(java_object, methodId);
    }

    return returned;
}

void *qtjambi_to_interface(JNIEnv *env,
                          QtJambiLink *link,
                          const char *interface_name,
                          const char *package_name,
                          const char *function_name)
{
    if (!link)
        return 0;
    jobject object = link->javaObject(env);
    jmethodID cast_id = resolveMethod(env, function_name, "(J)J", interface_name, package_name);
    jlong ret = env->CallLongMethod(object, cast_id, (jlong) link->pointer());
    QTJAMBI_EXCEPTION_CHECK(env);
    return reinterpret_cast<void *>(ret);
}

jobject qtjambi_from_object(JNIEnv *env, const void *qt_object, const char *className,
                            const char *packageName, const char *lookupName, bool makeCopyOfValueTypes,
                            void* ptr_shared_pointer, PointerDeleter sharedPointerDeleter)
{
    const char *class_name = className;
    const char *package = packageName;
    if (qt_object != 0)
        qtjambi_resolve_polymorphic_id(lookupName, qt_object, &class_name, &package);

    return qtjambi_from_object(env, qt_object, class_name, package, makeCopyOfValueTypes,
                               ptr_shared_pointer, sharedPointerDeleter);
}


jobject qtjambi_from_object(JNIEnv *env, const void *qt_object, const char *className,
                            const char *packageName, bool makeCopyOfValueTypes,
                            void* ptr_shared_pointer, PointerDeleter sharedPointerDeleter)
{
    if (qt_object == 0)
        return 0;

    QString java_full_name = QString::fromLatin1("%1%2").arg(packageName).arg(className);
    QString qt_name = getQtName(java_full_name);
    int metaType = QMetaType::type(qt_name.toLatin1());

    // If it's not a value type, we just link to the pointer directly.
    void *copy = 0;
    QString java_name = QLatin1String(packageName) + QLatin1String(className);
#if QT_VERSION < 0x050000
    if (!makeCopyOfValueTypes || metaType == QMetaType::Void) {
#else
    if (!makeCopyOfValueTypes || metaType == QMetaType::UnknownType) {
#endif
        // If the object is constructed in Java, then we can look it up
        QtJambiLink *link = QtJambiLink::findLinkForUserObject(qt_object);
        if (link != 0)
            return link->javaObject(env);

        // Otherwise we have to create it
        copy = const_cast<void *>(qt_object);
    } else {
#if QT_VERSION < 0x050000
        copy = QMetaType::construct(metaType, qt_object);
#else
        copy = QMetaType::create(metaType, qt_object);
#endif
        if (copy == 0)
            return 0;
    }

    jobject returned = 0;
    jclass clazz = resolveClass(env, className, packageName);
    QTJAMBI_EXCEPTION_CHECK(env);
    if (clazz != 0) {
        jmethodID constructorId = resolveMethod(env,
                                                "<init>",
                                                "(Lorg/qtjambi/qt/internal/QtJambiObject$QPrivateConstructor;)V",
                                                className,
                                                packageName,
                                                false);
        Q_ASSERT(constructorId);
        returned = env->NewObject(clazz, constructorId, 0);
    }

    QTJAMBI_EXCEPTION_CHECK(env);
    if (returned == 0)
        return 0;

    if (QtJambiLink *link = qtjambi_construct_object(env, returned, copy, metaType, java_name, false, ptr_shared_pointer, sharedPointerDeleter)) {
        // If the type is copied in, we own the pointer
#if QT_VERSION < 0x050000
        if (metaType != QMetaType::Void && makeCopyOfValueTypes)
#else
        if (metaType != QMetaType::UnknownType && makeCopyOfValueTypes)
#endif
            link->setJavaOwnership(env, link->javaObject(env));

        if (objectDeletionPolicy(java_name) == ObjectDeletionPolicyDeleteInMainThread)
            link->setDeleteInMainThread(true);
    } else {
#if QT_VERSION < 0x050000
        if (metaType != QMetaType::Void && copy != 0)
#else
        if (metaType != QMetaType::UnknownType && copy != 0)
#endif
            QMetaType::destroy(metaType, copy);

        returned = 0;
    }

    return returned;
}

void qtjambi_invalidate_object(JNIEnv *env, jobject java_object, bool checkJavaOwnership)
{
    QtJambiLink *link = QtJambiLink::findLink(env, java_object);
    if (link != 0 && (!checkJavaOwnership || (link->ownership() != QtJambiLink::JavaOwnership && !link->createdByJava()))) {
        link->javaObjectInvalidated(env);
    }
}

void qtjambi_invalidate_collection(JNIEnv *env, jobject java_collection, bool checkJavaOwnership)
{
    Q_ASSERT(java_collection);
    jobjectArray java_array = qtjambi_collection_toArray(env, java_collection);
    qtjambi_invalidate_array(env, java_array, checkJavaOwnership);
}

void qtjambi_invalidate_array(JNIEnv *env, jobjectArray java_array, bool checkJavaOwnership)
{
    Q_ASSERT(java_array);
    jsize array_size = env->GetArrayLength(java_array);
    for (int i=0; i<array_size; ++i) {
        jobject java_element = env->GetObjectArrayElement(java_array, i);
        if (java_element != 0) {
            qtjambi_invalidate_object(env, java_element, checkJavaOwnership);
        }
    }
}

static bool qtjambi_connect_callback(void **raw_data, bool slotMustBeGenerated);

// Find the first in meta_object's line of ancestors (including meta_object itself) which
// is a static meta object (hence, not one of Qt Jambi's fake meta objects)
static const QMetaObject *qtjambi_find_first_static_metaobject(const QMetaObject *meta_object)
{
    while (meta_object != 0 && qtjambi_metaobject_is_dynamic(meta_object))
        meta_object = meta_object->superClass();
    return meta_object;
}

/* Called the first time a C++ created QObject is converted into Java.
   Any Java emissions of signals that have previously been connected to something
   must then be connected to their corresponding C++ signal emissions.
*/
static void qtjambi_setup_connections(JNIEnv *, QtJambiLink *link)
{
    Q_ASSERT(link);

    if (link->connectedToJava())
        return;
    link->setConnectedToJava(true);

    const QObject *qobject = link->qobject();
    if(qobject == 0) {
        fprintf(stderr, "qtjambi_setup_connections() ASSERT(qobject=%p)  link=%p  java_object=%p\n", qobject, link, link->getJavaObject());
        // This return fixes the testcase org.qtjambi.autotests.TestClassFunctionality#run_callPrivateVirtualFunction()
        // there is some interaction between having the object finalized, the QObject marked deleteLater() and then
        // the qtjambi_disconnect_callback() and then qtjambi_connect_callback() which gets us here.
        // But the object is marked deleteLater().
        return;
    }

    Q_ASSERT(qobject);

    const QMetaObject *mo = qtjambi_find_first_static_metaobject(qobject->metaObject());
    Q_ASSERT(mo);

    for (int i=0; i<mo->methodCount(); ++i) {
        QMetaMethod m = mo->method(i);
        if (m.methodType() == QMetaMethod::Signal) {
#if QT_VERSION < 0x050000
            const char *signature = m.signature();
            QByteArray ba = QByteArray(signature);
#else
        	QByteArray ba = m.methodSignature();
#endif
            ba = QByteArray("2") + ba;

            // The trick of getting the correct connection from Java to C++ emits is
            // to call qtjambi_connect_callback() which is used to establish connections
            // across the language barriers, and pass in the signal as both signal and
            // slot. The slot with the signal's signature will not exist in C++ of course, and
            // the resolve-function will continue to resolve the equivalent Java method
            // and connect the whole thing in Java, which is what we want. We need to make sure
            // that when people implement a Java method which is named the same as the signal,
            // though, that we still find the correct generated method, which is why we pass in true
            // for the argument slotMustBeGenerated.
            Qt::ConnectionType type = Qt::AutoConnection;
            const void *cbdata[] = { qobject, ba.constData(), qobject, ba.constData(), &type };
            qtjambi_connect_callback((void **)cbdata, true);
        }
    }
}

jobject qtjambi_from_QObject(JNIEnv *env, QObject *qt_object) {
    return qtjambi_from_qobject(env, qt_object, "QObject", "org/qtjambi/qt/core/");
}

jobject qtjambi_from_QWidget(JNIEnv *env, QWidget *widget) {
#if QT_VERSION >= 5
    return qtjambi_from_qobject(env, widget, "QWidget", "org/qtjambi/qt/widgets/");
#else
    return qtjambi_from_qobject(env, widget, "QWidget", "org/qtjambi/qt/gui/");
#endif
}

jobject qtjambi_from_qobject(JNIEnv *env, QObject *qt_object, const char *className, const char *packageName,
                             void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter)
{
    if (qt_object == 0)
        return 0;

    QtJambiLink *link = QtJambiLink::findLinkForQObject(qt_object);

    // Since QObjects are created in a class based on virtual function calls,
    // if they at some point during their constructor are converted to Java,
    // the Java object will get the wrong class. In order to fix this as well
    // as possible, we replace the java object if it turns out it has previously
    // been created using a different metaObject than the current. This should
    // at least make the brokeness identical to that of C++, and we can't do this
    // better than C++ since we depend on C++ to do it.
    if (link != 0 && !link->createdByJava()) {
        QtJambiLinkUserData *p = static_cast<QtJambiLinkUserData *>(qt_object->userData(QtJambiLinkUserData::id()));
        if (p != 0 && p->metaObject() != qt_object->metaObject()) {
            // It should already be split ownership, but in case it has been changed, we need to make sure the c++
            // object isn't deleted.
            link->setSplitOwnership(env, link->javaObject(env));
            qt_object->setUserData(QtJambiLinkUserData::id(), 0);
            delete p;
            link = 0;
        }
    }

    if (!link) {
        const QMetaObject *mo = qtjambi_find_first_static_metaobject(qt_object->metaObject());

        QByteArray javaClassName;
        QByteArray javaPackageName;

        // Search the hierarchy of classes for a class that has been mapped to Java.
        // Prefer the requested class if no other can be found.
        // Only return objects of subclasses of the requested class
        // If the requested class is not in the object's hierarchy, then we prefer
        // the requested class (this means it's basically a proper subclass of the
        // requested class, and thus probably the concrete wrapper, but atleast a
        // more specific version than anything we can find)
        while (mo != 0) {
            // Never go further down the hierarchy than the requested class
            if (qstrcmp(className, mo->className()) == 0)
                break;

            QString javaName = getJavaName(QLatin1String(mo->className()));

            if (javaName.length() > 0) {
                javaClassName = QtJambiTypeManager::className(javaName).toLatin1();
                javaPackageName = QtJambiTypeManager::package(javaName).toLatin1();

                // Make sure the requested class is a superclass of this one
                while (mo != 0 && qstrcmp(mo->className(), className) == 0)
                    mo = mo->superClass();

                // If we found the requested class in the hierarchy, then choose the most
                // specialized class. Otherwise, just keep the requested class.
                if (mo != 0) {
                    className = javaClassName.constData();
                    packageName = javaPackageName.constData();
                    mo = 0;
                }


            } else {
                mo = mo->superClass();
            }
        }

        link = QtJambiLink::createWrapperForQObject(env, qt_object, className, packageName, ptr_shared_pointer, shared_pointer_deleter);
        if (link == 0) {
            qWarning("Qt Jambi: Couldn't created wrapper for class %s%s", packageName, className);
            return 0;
        }

        qtjambi_setup_connections(env, link);
    }

    return link->javaObject(env);
}

jobject qtjambi_from_enum(JNIEnv *env, int qt_enum, const char *className)
{
    Q_ASSERT(className);
    jclass cl = qtjambi_find_class(env, className);
#if defined(QTJAMBI_DEBUG_TOOLS)
    if(!cl) {
        fprintf(stderr, "qtjambi_find_class(): %s NOTFOUND", className);
        fflush(stderr);
        qWarning("qtjambi_find_class(): %s NOTFOUND", className);
    }
#endif
    Q_ASSERT(cl);

    jmethodID method = env->GetStaticMethodID(cl, "resolve_internal", "(I)Ljava/lang/Object;");
    Q_ASSERT(method);

    jobject obj = env->CallStaticObjectMethod(cl, method, qt_enum);

    env->DeleteLocalRef(cl);

    return obj;
}

jobject qtjambi_from_flags(JNIEnv *env, int qt_flags, const char *className)
{
    Q_ASSERT(className);
    jclass cl = qtjambi_find_class(env, className);
    Q_ASSERT(cl);

    jmethodID method = env->GetMethodID(cl, "<init>", "(I)V");
    Q_ASSERT(method);

    jobject obj = env->NewObject(cl, method, qt_flags);

    env->DeleteLocalRef(cl);

    return obj;
}

int qtjambi_to_enumerator(JNIEnv *env, jobject value)
{
    StaticCache *sc = StaticCache::instance();
    sc->resolveQtEnumerator();
    Q_ASSERT(value);
    Q_ASSERT(env->IsInstanceOf(value, sc->QtEnumerator.class_ref));  // check the java object is right type
    return env->CallIntMethod(value, sc->QtEnumerator.value);
}


QtJambiLink *qtjambi_construct_qobject(JNIEnv *env, jobject java_object, QObject *qobject,
                                       void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter)
{
    StaticCache *sc = StaticCache::instance();
    sc->resolveThread();
    jobject java_thread = env->CallStaticObjectMethod(sc->Thread.class_ref,
                                                      sc->Thread.currentThread);
    Q_ASSERT(java_thread);

    QThread *qt_thread = qobject->thread();
    Q_ASSERT(qt_thread == QThread::currentThread());
    Q_ASSERT(qt_thread);

    bool contained;
    {
        QReadLocker readLocker(qtjambi_thread_table_lock());
        contained = qtjambi_thread_table()->contains(qt_thread);
    }

    if (!contained) {
        QWriteLocker lock(qtjambi_thread_table_lock());
        if (!qtjambi_thread_table()->contains(qt_thread)) {
//             printf("(jni) inserting: %p for %s [%s] [%p]\n",
//                    qt_thread,
//                    qPrintable(qobject->objectName()),
//                    qobject->metaObject()->className(),
//                    qobject);
            qtjambi_thread_table()->insert(qt_thread, env->NewWeakGlobalRef(java_thread));
            callFunction(RefAdoptedThread, (void **) (void *) (&qt_thread));
        }
    }

    return QtJambiLink::createLinkForQObject(env, java_object, qobject, ptr_shared_pointer, shared_pointer_deleter);
}

QtJambiLink *qtjambi_construct_object(JNIEnv *env, jobject java_object, void *object,
                             int metaType, const QString &java_name, bool created_by_java,
                             void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter)
{
    QtJambiLink *link = QtJambiLink::createLinkForObject(env, java_object, object, java_name, created_by_java, ptr_shared_pointer, shared_pointer_deleter);
    link->setMetaType(metaType);
    return link;
}

QtJambiLink *qtjambi_construct_object(JNIEnv *env, jobject java_object, void *object,
                                      const char *className)
{
    int metaType = QMetaType::type(className);

#if QT_VERSION < 0x050000
    if (metaType != QMetaType::Void) {
#else
    if (metaType != QMetaType::UnknownType) {
#endif
        return qtjambi_construct_object(env, java_object, object, metaType

#if defined(QTJAMBI_DEBUG_TOOLS)
            , QString::fromLatin1(className)
#endif

        );

    } else {
        jclass cls_exc = env->FindClass("java/lang/Exception");
        env->ThrowNew(cls_exc, QString::fromLatin1("Qt Jambi failed to construct native instance"
                                              " of type %1").arg(className).toLatin1());
        env->DeleteLocalRef(cls_exc);  // non-performance path; be correct
        return 0;
    }
}

void *qtjambi_to_cpointer(JNIEnv *env, jobject java_object, int indirections)
{
    if (java_object == 0)
        return 0;

    StaticCache *sc = StaticCache::instance();
    sc->resolveNativePointer();
    Q_ASSERT(env->IsInstanceOf(java_object, sc->NativePointer.class_ref));  // check the java object is right type
    int object_indirections = env->GetIntField(java_object, sc->NativePointer.indirections);
    // What is this != test doing ?
    if (object_indirections != indirections) {
        jclass exception_class = resolveClass(env, "IllegalArgumentException", "java/lang/");
        Q_ASSERT(exception_class);
        env->ThrowNew(exception_class, "Illegal number of indirections");
        env->DeleteLocalRef(exception_class);  // non-performance path; be correct
        return 0;
    }
    void *ptr = reinterpret_cast<void *>(env->GetLongField(java_object, sc->NativePointer.ptr));

    return ptr;
}

jobject qtjambi_from_cpointer(JNIEnv *env, const void *qt_object, int type, int indirections)
{
    StaticCache *sc = StaticCache::instance();
    sc->resolveNativePointer();
    return env->CallStaticObjectMethod(sc->NativePointer.class_ref,
                                       sc->NativePointer.fromNative,
                                       reinterpret_cast<jlong>(qt_object), type, indirections);
}

jobject qtjambi_to_tablearea(JNIEnv *env, int row, int column, int rowCount, int columnCount)
{
    StaticCache *sc = StaticCache::instance();
    sc->resolveQTableArea();
    return env->NewObject(sc->QTableArea.class_ref, sc->QTableArea.constructor, row, column, rowCount, columnCount);
}

void qtjambi_from_tablearea(JNIEnv *env, jobject tableArea, int *row, int *column, int *rowCount, int *columnCount)
{
    StaticCache *sc = StaticCache::instance();
    sc->resolveQTableArea();
    Q_ASSERT(env->IsInstanceOf(tableArea, sc->QTableArea.class_ref));  // check the java object is right type
    if (row != 0)
        *row = tableArea != 0 ? env->GetIntField(tableArea, sc->QTableArea.row) : -1;
    if (column != 0)
        *column = tableArea != 0 ? env->GetIntField(tableArea, sc->QTableArea.column) : -1;
    if (rowCount != 0)
        *rowCount = tableArea != 0 ? env->GetIntField(tableArea, sc->QTableArea.rowCount) : -1;
    if (columnCount != 0)
        *columnCount = tableArea != 0 ? env->GetIntField(tableArea, sc->QTableArea.columnCount) : -1;
}

jobject qtjambi_to_resolvedentity(JNIEnv *env, bool success, void *inputSource)
{
    StaticCache *sc = StaticCache::instance();
    sc->resolveResolvedEntity();

    jobject java_inputSource = qtjambi_from_object(env, inputSource, "QXmlInputSource", "org/qtjambi/qt/xml/", true);
    return env->NewObject(sc->ResolvedEntity.class_ref, sc->ResolvedEntity.constructor, !success, java_inputSource);
}

bool qtjambi_from_resolvedentity(JNIEnv *env, void *&inputSource, jobject resolvedEntity)
{
    StaticCache *sc = StaticCache::instance();
    sc->resolveResolvedEntity();

    Q_ASSERT(resolvedEntity);
    Q_ASSERT(env->IsInstanceOf(resolvedEntity, sc->ResolvedEntity.class_ref));  // check the java object is right type
    jobject java_inputSource = env->GetObjectField(resolvedEntity, sc->ResolvedEntity.inputSource);
    inputSource = qtjambi_to_object(env, java_inputSource);

    return !env->GetBooleanField(resolvedEntity, sc->ResolvedEntity.error);
}

jobject qtjambi_to_cellatindex(JNIEnv *env, int row, int column, int rowCount, int columnCount, bool isSelected)
{
    StaticCache *sc = StaticCache::instance();
    sc->resolveCellAtIndex();
    return env->NewObject(sc->CellAtIndex.class_ref, sc->CellAtIndex.constructor, row, column, rowCount, columnCount, isSelected);
}

void qtjambi_from_cellatindex(JNIEnv *env, jobject cellAtIndex, int *row, int *column, int *rowCount, int *columnCount, bool *isSelected)
{
    Q_ASSERT(cellAtIndex);
    qtjambi_from_tablearea(env, cellAtIndex, row, column, rowCount, columnCount);

    StaticCache *sc = StaticCache::instance();
    sc->resolveCellAtIndex();

    if (isSelected != 0) {
        Q_ASSERT(env->IsInstanceOf(cellAtIndex, sc->CellAtIndex.class_ref));  // check the java object is right type
        *isSelected = cellAtIndex != 0 ? env->GetBooleanField(cellAtIndex, sc->CellAtIndex.isSelected) : false;
    }
}


jstring qtjambi_from_qstringref(JNIEnv *env, const QStringRef &s)
{
    QTJAMBI_EXCEPTION_CHECK(env);
    jstring str = env->NewString(reinterpret_cast<const jchar *>(s.constData()), s.length());
    Q_ASSERT(str != 0);

    return str;
}

jstring qtjambi_from_qstring(JNIEnv *env, const QString &s)
{
    QTJAMBI_EXCEPTION_CHECK(env);
    jstring str = env->NewString(reinterpret_cast<const jchar *>(s.constData()), s.length());
    Q_ASSERT(str != 0);

    return str;
}

QString qtjambi_to_qstring(JNIEnv *env, jstring java_string)
{
    if (java_string == 0)
        return QString();

    QString result;
    int length = env->GetStringLength(java_string);
    result.resize(length);
    env->GetStringRegion(java_string, 0, length, reinterpret_cast<ushort*>(result.data()));
    return result;
}

JBufferData::JBufferData(JNIEnv *env, jobject buffer_object) :
    m_env(env),
    m_buffer_object(buffer_object),
    m_data(Q_NULLPTR){

    if(m_buffer_object){
        StaticCache *sc = StaticCache::instance();
        sc->resolveBuffer();

        jboolean isDirect = m_env->CallBooleanMethod(m_buffer_object, sc->Buffer.isDirect);
        if(isDirect){
            m_data = m_env->GetDirectBufferAddress(m_buffer_object);
        }else{
            sc->resolveByteBuffer();
            sc->resolveIntBuffer();
            sc->resolveCharBuffer();
            sc->resolveShortBuffer();
            sc->resolveLongBuffer();
            sc->resolveDoubleBuffer();
            sc->resolveFloatBuffer();

            int position = m_env->CallIntMethod(m_buffer_object, sc->Buffer.position);
            int limit = m_env->CallIntMethod(m_buffer_object, sc->Buffer.limit);
            int length = limit - position;

            if(m_env->IsInstanceOf(m_buffer_object, sc->ByteBuffer.class_ref)){
                char* array = new char[length];
                for(int i=0; i<length; i++){
                    array[i] = m_env->CallByteMethod(m_buffer_object, sc->ByteBuffer.get, position + i);
                }
                m_data = array;
            }else if(m_env->IsInstanceOf(m_buffer_object, sc->IntBuffer.class_ref)){
                int* array = new int[length];
                for(int i=0; i<length; i++){
                    array[i] = m_env->CallIntMethod(m_buffer_object, sc->IntBuffer.get, position + i);
                }
                m_data = array;
            }else if(m_env->IsInstanceOf(m_buffer_object, sc->ShortBuffer.class_ref)){
                short* array = new short[length];
                for(int i=0; i<length; i++){
                    array[i] = m_env->CallShortMethod(m_buffer_object, sc->ShortBuffer.get, position + i);
                }
                m_data = array;
            }else if(m_env->IsInstanceOf(m_buffer_object, sc->CharBuffer.class_ref)){
                char* array = new char[length];
                for(int i=0; i<length; i++){
                    array[i] = m_env->CallCharMethod(m_buffer_object, sc->CharBuffer.get, position + i);
                }
                m_data = array;
            }else if(m_env->IsInstanceOf(m_buffer_object, sc->LongBuffer.class_ref)){
                long* array = new long[length];
                for(int i=0; i<length; i++){
                    array[i] = m_env->CallLongMethod(m_buffer_object, sc->LongBuffer.get, position + i);
                }
                m_data = array;
            }else if(m_env->IsInstanceOf(m_buffer_object, sc->FloatBuffer.class_ref)){
                float* array = new float[length];
                for(int i=0; i<length; i++){
                    array[i] = m_env->CallFloatMethod(m_buffer_object, sc->FloatBuffer.get, position + i);
                }
                m_data = array;
            }else if(m_env->IsInstanceOf(m_buffer_object, sc->DoubleBuffer.class_ref)){
                double* array = new double[length];
                for(int i=0; i<length; i++){
                    array[i] = m_env->CallDoubleMethod(m_buffer_object, sc->DoubleBuffer.get, position + i);
                }
                m_data = array;
            }
        }
    }
}

JBufferData::~JBufferData(){
    if(m_buffer_object){
        StaticCache *sc = StaticCache::instance();
        sc->resolveBuffer();
        jboolean isDirect = m_env->CallBooleanMethod(m_buffer_object, sc->Buffer.isDirect);
        jboolean isReadOnly = m_env->CallBooleanMethod(m_buffer_object, sc->Buffer.isReadOnly);
        if(!isDirect && !isReadOnly){
            sc->resolveByteBuffer();
            sc->resolveIntBuffer();
            sc->resolveCharBuffer();
            sc->resolveShortBuffer();
            sc->resolveLongBuffer();
            sc->resolveDoubleBuffer();
            sc->resolveFloatBuffer();

            int position = m_env->CallIntMethod(m_buffer_object, sc->Buffer.position);
            int limit = m_env->CallIntMethod(m_buffer_object, sc->Buffer.limit);
            int length = limit - position;

            if(m_env->IsInstanceOf(m_buffer_object, sc->ByteBuffer.class_ref)){
                char* array = (char*)m_data;
                for(int i=0; i<length; i++){
                    m_env->CallVoidMethod(m_buffer_object, sc->ByteBuffer.put, position + i, array[i]);
                }
                delete[] array;
            }else if(m_env->IsInstanceOf(m_buffer_object, sc->IntBuffer.class_ref)){
                int* array = (int*)m_data;
                for(int i=0; i<length; i++){
                    m_env->CallVoidMethod(m_buffer_object, sc->IntBuffer.put, position + i, array[i]);
                }
                delete[] array;
            }else if(m_env->IsInstanceOf(m_buffer_object, sc->ShortBuffer.class_ref)){
                short* array = (short*)m_data;
                for(int i=0; i<length; i++){
                    m_env->CallVoidMethod(m_buffer_object, sc->ShortBuffer.put, position + i, array[i]);
                }
                delete[] array;
            }else if(m_env->IsInstanceOf(m_buffer_object, sc->CharBuffer.class_ref)){
                char* array = (char*)m_data;
                for(int i=0; i<length; i++){
                    m_env->CallVoidMethod(m_buffer_object, sc->CharBuffer.put, position + i, array[i]);
                }
                delete[] array;
            }else if(m_env->IsInstanceOf(m_buffer_object, sc->LongBuffer.class_ref)){
                long* array = (long*)m_data;
                for(int i=0; i<length; i++){
                    m_env->CallVoidMethod(m_buffer_object, sc->LongBuffer.put, position + i, array[i]);
                }
                delete[] array;
            }else if(m_env->IsInstanceOf(m_buffer_object, sc->FloatBuffer.class_ref)){
                float* array = (float*)m_data;
                for(int i=0; i<length; i++){
                    m_env->CallVoidMethod(m_buffer_object, sc->FloatBuffer.put, position + i, array[i]);
                }
                delete[] array;
            }else if(m_env->IsInstanceOf(m_buffer_object, sc->DoubleBuffer.class_ref)){
                double* array = (double*)m_data;
                for(int i=0; i<length; i++){
                    m_env->CallVoidMethod(m_buffer_object, sc->DoubleBuffer.put, position + i, array[i]);
                }
                delete[] array;
            }
            m_data = Q_NULLPTR;
        }
    }
}

void* JBufferData::data(){
    return m_data;
}

QtJambiFunctionTable *qtjambi_setup_vtable(JNIEnv *env,
                                         jobject object,
                                         int inconsistentCount,
                                         const char **inconsistentNames,
                                         const char **inconsistentSignatures,
                                         int count,
                                         const char **names,
                                         const char **signatures)
{
    QTJAMBI_EXCEPTION_CHECK(env);
    Q_ASSERT(object);
    jclass object_class = env->GetObjectClass(object);

//     printf("vtable for: %s\n", qPrintable(qtjambi_class_name(env, object_class)));

    StaticCache *sc = StaticCache::instance();
    sc->resolveClass();
    sc->resolveObject();
    sc->resolveMethod();
    sc->resolveQtJambiInternal();

    QTJAMBI_EXCEPTION_CHECK(env);
    Q_ASSERT(object_class);
    Q_ASSERT(env->IsInstanceOf(object_class, sc->Class.class_ref));  // check the java object is right type
    jstring jclass_name = (jstring) env->CallObjectMethod(object_class, sc->Class.getName);
    Q_ASSERT(jclass_name);
    QString qclass_name = qtjambi_to_qstring(env, jclass_name);

    QtJambiFunctionTable *table = findFunctionTable(qclass_name);

    if (table) {

#ifdef QT_DEBUG
        table->ref();
#endif

        return table;
    } else {
        table = new QtJambiFunctionTable(qclass_name, count);
    }

    for (int i=0; i<count; ++i) {
        QTJAMBI_EXCEPTION_CHECK(env);
        jmethodID method_id = env->GetMethodID(object_class, names[i], signatures[i]);
        if (!method_id) {
            fprintf(stderr, "vtable setup failed: %s::%s %s\n",
                    qPrintable(qclass_name), names[i], signatures[i]);
            qtjambi_exception_check(env);
            continue;
        }

        jobject method_object = env->ToReflectedMethod(object_class, method_id, false);
        if (!method_object) {
            fprintf(stderr,
                    "vtable setup conversion to reflected method failed: %s::%s %s\n",
                    qPrintable(qclass_name), names[i], signatures[i]);
            qtjambi_exception_check(env);
            continue;
        }

        if (env->CallStaticBooleanMethod(sc->QtJambiInternal.class_ref,
                                         sc->QtJambiInternal.isImplementedInJava,
                                         method_object)) {
            table->setMethod(i, method_id);
//             printf("Implemented: %s::%s\n",
//                    qPrintable(qtjambi_class_name(env, object_class)),
//                    names[i]);
        }

        env->DeleteLocalRef(method_object);
    }

    QTJAMBI_EXCEPTION_CHECK(env);
    storeFunctionTable(qclass_name, table);

    for (int i=0; i<inconsistentCount; ++i) {
        QTJAMBI_EXCEPTION_CHECK(env);
        jmethodID method_id = env->GetMethodID(object_class, inconsistentNames[i],
                                               inconsistentSignatures[i]);
        if (!method_id) {
            fprintf(stderr, "inconsistent function setup failed: %s::%s %s\n",
                    qPrintable(qclass_name),
                    inconsistentNames[i],
                    inconsistentSignatures[i]);
            qtjambi_exception_check(env);
            continue;
        }

        QTJAMBI_EXCEPTION_CHECK(env);
        jobject method_object = env->ToReflectedMethod(object_class, method_id, false);
        if (!method_object) {
            fprintf(stderr,
                    "inconsistent function conversion to reflected method failed: "
                    "%s:%s %s\n",
                    qPrintable(qclass_name),
                    inconsistentNames[i],
                    inconsistentSignatures[i]);
            qtjambi_exception_check(env);
            continue;
        }

        QTJAMBI_EXCEPTION_CHECK(env);
        if (env->CallStaticBooleanMethod(sc->QtJambiInternal.class_ref,
                                         sc->QtJambiInternal.isImplementedInJava,
                                         method_object)) {
            QString errorMessage = QString("Function '%1' in class '%2'")
                                   .arg(inconsistentNames[i]).arg(qclass_name);
            QtJambiLink::throwQtException(env, errorMessage,
                                         QLatin1String("QNonVirtualOverridingException"));
            env->DeleteLocalRef(method_object);
            return 0;
        }

        env->DeleteLocalRef(method_object);
    }

    QTJAMBI_EXCEPTION_CHECK(env);
    return table;
}

jclass qtjambi_find_generated_superclass(JNIEnv *env,  jobject obj){
    StaticCache *sc = StaticCache::instance();
    sc->resolveQtJambiInternal();
    return (jclass)env->CallStaticObjectMethod(sc->QtJambiInternal.class_ref, sc->QtJambiInternal.findGeneratedSuperclass, obj);
}


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

void qtjambi_call_java_signal(JNIEnv *env, QtJambiSignalInfo signal_info, jvalue *args)
{
    StaticCache *sc = StaticCache::instance();
    sc->resolveAbstractSignal();

    // Check if signal has since been collected
    jobject object = env->NewLocalRef(signal_info.ref.weak);
    if (env->IsSameObject(object, 0) != JNI_FALSE) {    // it can be non-null (in C++) but null (in Java)
        goto done;     // Checks for non-null and for lost weak-reference
    }

    Q_ASSERT(env->IsInstanceOf(object, sc->AbstractSignal.class_ref));  // check the java object is right type
    // Don't recurse
    if (env->GetBooleanField(object, sc->AbstractSignal.inJavaEmission)) {
        goto done;
    }

    env->SetBooleanField(object, sc->AbstractSignal.inCppEmission, true);
    Q_ASSERT(object);	// redundant?
    Q_ASSERT(signal_info.methodId);
    if (args == 0)
        env->CallVoidMethod(object, signal_info.methodId);
    else
        env->CallVoidMethodA(object, signal_info.methodId, args);
    env->SetBooleanField(object, sc->AbstractSignal.inCppEmission, false);

done:
    if(object)
        env->DeleteLocalRef(object);
    return;
}


void qtjambi_resolve_signals(JNIEnv *env,
                             jobject java_object,
                             QtJambiSignalInfo *infos,
                             int count,
                             const char **names,
                             const int *argument_counts
#if defined(QTJAMBI_DEBUG_TOOLS)
                             ,
                             const char *java_class_name, /* is C++ static string const */
                             const char *java_signal_name /* needs to be copied if retained */
#endif
                             )
{
    Q_ASSERT(java_object);
    jclass clazz = qtjambi_find_generated_superclass(env,  java_object);

    QTJAMBI_EXCEPTION_CHECK(env);
    Q_ASSERT(clazz);

    char signal_class_name[]      = "org/qtjambi/qt/QSignalEmitter$AbstractPublicSignalX";
    char signal_field_signature[] = "Lorg/qtjambi/qt/QSignalEmitter$SignalX;";
    char defaultsignal_field_signature[] = "Lorg/qtjambi/qt/QSignalEmitter$SignalXDefaultX;";
    const int signal_pos = 50;
    const int signal_field_pos = 37;
    const int defaultsignal_field_pos = 45;

    char privatesignal_class_name[] = "org/qtjambi/qt/QSignalEmitter$PrivateSignalX";
    char privatesignal_field_signature[] = "Lorg/qtjambi/qt/QSignalEmitter$PrivateSignalX;";
    const int privatesignal_pos = 43;
    const int privatesignal_field_pos = 44;

    memset(infos, 0, sizeof(QtJambiSignalInfo) * count);

    for (int j=0; j<3; ++j) {
        char *class_name, *field_signature;
        int pos;
        int fpos;
        if (j == 0) {
            class_name = signal_class_name;
            field_signature = signal_field_signature;
            pos = signal_pos;
            fpos = signal_field_pos;
        } else if (j == 1) {
            class_name = signal_class_name;
            field_signature = defaultsignal_field_signature;
            pos = signal_pos;
            fpos = signal_field_pos;
        } else {
            class_name = privatesignal_class_name;
            field_signature = privatesignal_field_signature;
            pos = privatesignal_pos;
            fpos = privatesignal_field_pos;
        }

        for (int i=0; i<count; ++i) {
            if (infos[i].methodId) // Found amongst the normal signals...
                continue;
            int argCount = argument_counts[i];

            char argCountChar = '0' + argCount;
            // Swap the X for the right number...
            class_name[pos] = argCountChar;
            field_signature[fpos] = argCountChar;
            const char *signature = signatureTable[argCount];

            for(int k=1; (j != 1 && k==1) || k<=argCount;k++){
                if(j == 1){
                    field_signature[defaultsignal_field_pos] = ('0' + k);
                }
                jfieldID fieldId = env->GetFieldID(clazz, names[i], field_signature);
                if (env->ExceptionCheck()) {
                    env->ExceptionClear();
                    continue;
                }

                jobject signal = env->GetObjectField(java_object, fieldId);

                QTJAMBI_EXCEPTION_CHECK(env);
                Q_ASSERT(signal);
                if (env->ExceptionCheck()) {
                    env->ExceptionClear();
                    continue;
                }

                jclass cls = env->FindClass(class_name);

                Q_ASSERT(cls);
                if (env->ExceptionCheck()) {
                    env->ExceptionClear();
                    continue;
                }
                jmethodID emitMethodID = env->GetMethodID(cls, "emit", signature);

                Q_ASSERT(emitMethodID);
                if (env->ExceptionCheck()) {
                    env->ExceptionClear();
                    continue;
                }

                jweak signalWeakRef = env->NewWeakGlobalRef(signal);

                if (signalWeakRef && emitMethodID) {  // everything went well
                    if(infos[i].ref.weak)  // restartable
                        env->DeleteWeakGlobalRef(infos[i].ref.weak);
                    infos[i].ref.weak = signalWeakRef;
                    infos[i].methodId = emitMethodID;
#if defined(QTJAMBI_DEBUG_TOOLS)
                    infos[i].javaClassName = java_class_name;
                    if(infos[i].javaSignalName)
                        free((void *)infos[i].javaSignalName);
                    infos[i].javaSignalName = strdup(java_signal_name);
#endif
                } else {  // something went bad
                    if (env->ExceptionCheck()) {
                        env->ExceptionClear();
                    }
                    env->DeleteWeakGlobalRef(signalWeakRef);
                    infos[i].ref.weak = 0;
                    infos[i].methodId = 0;
#if defined(QTJAMBI_DEBUG_TOOLS)
                    infos[i].javaClassName = 0;
                    infos[i].javaSignalName = 0;
#endif
                }

                env->DeleteLocalRef(cls);
                env->DeleteLocalRef(signal);
            }
        }
    }
#ifndef QT_NO_DEBUG
    for (int i=0; i<count; ++i) {
        Q_ASSERT_X(infos[i].ref.weak, "qtjambi_resolve_signals", names[i]);
        Q_ASSERT_X(infos[i].methodId, "qtjambi_resolve_signals", names[i]);
    }
#endif
}

// Connects the emission of a C++ signal to the emit function in the corresponding
// signal in Java
bool qtjambi_connect_cpp_to_java(JNIEnv *,
                                 const QString &java_signal_name,
                                 QObject *sender,
                                 QObject *wrapper,
                                 const QString &java_class_name,
                                 const QString &signal_wrapper_prefix)
{
    Q_ASSERT(wrapper);

    QString cpp_signal_name = getQtName(java_class_name + QLatin1String(".") + java_signal_name);
    if (cpp_signal_name.isEmpty())
        return false;

    int paren_pos = cpp_signal_name.indexOf(QLatin1Char('('));
    cpp_signal_name = cpp_signal_name.mid(cpp_signal_name.lastIndexOf("::", paren_pos) + 2);
    QString cpp_slot_name = QString::number(QSLOT_CODE)
                            + signal_wrapper_prefix
                            + cpp_signal_name;


    cpp_signal_name = QString::number(QSIGNAL_CODE) + cpp_signal_name;

#ifdef QT_QTJAMBI_PORT
    if (!sender->connect(sender, cpp_signal_name.toLatin1().constData(),
                         wrapper, cpp_slot_name.toLatin1().constData())) {
#else

    // The trick of getting the correct connection from Java to C++ emits is
    // to call qtjambi_connect_callback() which is used to establish connections
    // across the language barriers, and pass in the signal as both signal and
    // slot. The slot with the signal's signature will not exist in C++ of course, and
    // the resolve-function will continue to resolve the equivalent Java method
    // and connect the whole thing in Java, which is what we want. We need to make sure
    // that when people implement a Java method which is named the same as the signal,
    // though, that we still find the correct generated method, which is why we pass in true
    // for the argument slotMustBeGenerated.
    Qt::ConnectionType type = Qt::AutoConnection;
    const void *cbdata[] = { sender, cpp_signal_name.toLatin1().constData(),
                             wrapper, cpp_slot_name.toLatin1().constData(), &type };

    /*
     * since it is no longer possible to connect callbacks for connection/disconnection
     * in Qt5, the Java signals cannot connected/disconnected from C++ by calling
     * QObject::connect() and QObject::disconnect().
     * Here, we have to do it manual. The Question is: How will this connection later get disconnected?
     */
    bool success_in_callback = qtjambi_connect_callback(&cbdata, false);
    QMetaObject::Connection native_connection = sender->connect(sender, cpp_signal_name.toLatin1().constData(),
                             wrapper, cpp_slot_name.toLatin1().constData());
    if (!success_in_callback && !native_connection) {
#endif
        qWarning("qtjambi_connect_cpp_to_java(): failed to connect '%s' in '%s' to wrapper '%s'",
                 qPrintable(cpp_signal_name), qPrintable(java_class_name), qPrintable(cpp_slot_name));
        return false;
    }

    return true;
}

jobject qtjambi_array_to_nativepointer(JNIEnv *env, jobjectArray array, int elementSize)
{
    Q_ASSERT(array);
    int len = env->GetArrayLength(array);
    if (len == 0)
        return 0;

    StaticCache *sc = StaticCache::instance();
    sc->resolveNativePointer();
    jobject nativePointer = env->NewObject(sc->NativePointer.class_ref, sc->NativePointer.constructor,
                                           jint(ByteType), elementSize * len, 1);

    char *buf = reinterpret_cast<char *>(qtjambi_to_cpointer(env, nativePointer, 1));
    for (int i=0; i<len; ++i) {
        jobject java_object = env->GetObjectArrayElement(array, i);

        void *ptr = 0;
        if (java_object != 0) {
            QtJambiLink *link = QtJambiLink::findLink(env, java_object);
            if (link != 0)
                ptr = link->pointer();
        }

        if (ptr != 0)
            memcpy(buf + i * elementSize, ptr, elementSize);
    }

    return nativePointer;
}

QThread *qtjambi_find_thread_in_table(JNIEnv *env, jobject thread)
{
    QReadLocker locker(qtjambi_thread_table_lock());
    ThreadTable *table = qtjambi_thread_table();
    for (ThreadTable::const_iterator it = table->begin(); it != table->end(); ++it) {
        if (env->IsSameObject(it.value(), thread))
            return it.key();
    }
    return 0;
}

QThread *qtjambi_to_thread(JNIEnv *env, jobject thread)
{
    if(thread==0)
        return 0;

    QThread *qt_thread = qtjambi_find_thread_in_table(env, thread);
    if (qt_thread)
        return qt_thread;

    // No thread found, need to create a "fake" thread and insert it
    // into the thread table for later mapping between Qt / Java.
    // This thread object is already ref'ed by Qt.
    callFunction(CreateThreadForAdoption, (void **) (void *) &qt_thread);
    Q_ASSERT_X(qt_thread, "qtjambi_to_thread", "Thread adoption failed, have to abort...");

    ThreadTable *table = qtjambi_thread_table();

    QWriteLocker writeLocker(qtjambi_thread_table_lock());
    table->insert(qt_thread, env->NewWeakGlobalRef(thread));
    return qt_thread;
}

jobject qtjambi_from_thread(JNIEnv *, QThread *thread)
{
    QReadLocker locker(qtjambi_thread_table_lock());
    ThreadTable *table = qtjambi_thread_table();
    jobject java_thread = table->value(thread, 0);
    return java_thread;
}

int qtjambi_release_threads(JNIEnv *env)
{
    QWriteLocker locker(qtjambi_thread_table_lock());
    int releaseCount = 0;
    ThreadTable *table = qtjambi_thread_table();
//     for (ThreadTable::const_iterator it = table->constBegin(); it != table->constEnd(); ++it) {
//         printf("   - qt=%p, java=%p\n", it.key(), it.value());
//     }
    for (ThreadTable::iterator it = table->begin(); it != table->end(); ) {
        jobject java_thread = it.value();
        Q_ASSERT(java_thread);

        if (env->IsSameObject(java_thread, 0)) {
            ++releaseCount;
            QThread *thread = it.key();
            it = table->erase(it);
//             locker.unlock();
            Q_ASSERT(thread);
            callFunction(DerefAdoptedThread, (void **) (void *) &thread);
//             locker.relock();
        } else {
            ++it;
        }
    }
    return releaseCount;
}

bool qtjambi_adopt_current_thread(void **args)
{
    JNIEnv *env = qtjambi_current_environment();
    if (env == 0)
      return false;

    StaticCache *sc = StaticCache::instance();
    sc->resolveThread();

    jobject java_thread = env->CallStaticObjectMethod(sc->Thread.class_ref,
                                                      sc->Thread.currentThread);

    QThread *qt_thread = qtjambi_find_thread_in_table(env, java_thread);
    if (!qt_thread)
        return false;

    *args = qt_thread;
    return true;
}

void qtjambi_metacall(JNIEnv *env, QEvent *event)
{
    // The hardcoded value for metatype id's
    Q_ASSERT(event->type() == 512);

    if (!env)
        env = qtjambi_current_environment();

    QtJambiLink *link = QtJambiLink::findLinkForUserObject(event);
    if(link == 0) {
#if defined(QTJAMBI_DEBUG_TOOLS)
        ::fprintf(stderr, "WARNING: qtjambi_metacall() event=%p link=%p\n", event, link);
#endif
        return;
    }
    Q_ASSERT(link);

    // The QEvent has done its jobs and got runtime execution back into
    // this method, while it was waiting in the Qt event queue the
    // associated Java object was taken as a global reference to stop
    // the JVM GCing the java data while it was waiting its turn to be
    // processed causing execution to end up here.
    // We always take a new (hard) local reference now because during the
    // execution of this method that hard reference will be convered back into
    // a weak reference which effectively goes out of scope during the calling
    // of the execute()V method below.  So to keep this code as quirk free as
    // possible we take our own hard reference during the time we want to keep
    // the java object alive in this method.
    jobject jEvent = env->NewLocalRef(link->getJavaObject());
    if(env->IsSameObject(jEvent, 0)) {
#if defined(QTJAMBI_DEBUG_TOOLS)
        ::fprintf(stderr, "WARNING: qtjambi_metacall() event=%p link=%p jEvent=%p\n", event, link, jEvent);
#endif
        return;
    }
    Q_ASSERT(jEvent);

    // We take charge of ensuring this is GCed, the jEvent is probably the
    // only hard reference left.  We do this early so that it gets done
    // regardless of any problems encountered below.
    // FIXME: Need to investigate the concern that application code might be
    // able to queue some other event of this type()==512 that is not what we
    // expect.  Maybe gracefully handling link==0 jEvent==0 will solve that.
    link->setDefaultOwnership(env, jEvent);

    jclass cls = env->GetObjectClass(jEvent);
    Q_ASSERT(cls);

    jmethodID id = env->GetMethodID(cls, "execute", "()V");
    Q_ASSERT(id);

    env->CallVoidMethod(jEvent, id);

    // Clear print out and clear any exceptions that occured during
    // the metacall...
    if (env->ExceptionCheck()) {
        // FIXME: Allow this error channel to be intercepted  QtJambiMetacallException()
        //  make the default interceptor print/clear exception, allow exception condx clearing
        //  only, allow propagation upwards (if relevant to caller).  i.e. does it make
        //  sense for this thread to receive exception ?
        // Emit loads of info here, event field data, class types.
        fprintf(stderr, "QtJambi: metacall failed\n");
        env->ExceptionDescribe();
        env->ExceptionClear();
    }

    env->DeleteLocalRef(cls);
    env->DeleteLocalRef(jEvent);
    // FIXME: Better failure scenarios (emit our own kind of exception/qWarning())
}


/*
FIXME:
We need some way of comparing the pointer to this type offsets and sizes
of the members against QModelIndex so any changes show up.
We have to do this in the first place because Qt provides no way to create
QModelIndex from public API. This is because application code should never
create them, but Jambi is closer Qt library code than an application
and hence needs to play with QModelIndex.
*/
typedef struct _QModelIndexAccessor {
    int r;
    int c;
    void *p;
    QAbstractItemModel *m;
} /*__attribute__((__may_alias__))*/ QModelIndexAccessor;

QModelIndex qtjambi_to_QModelIndex(JNIEnv *env, jobject index)
{
    if (!index)
        return QModelIndex();

    StaticCache *sc = StaticCache::instance();
    sc->resolveQModelIndex();

    QModelIndexAccessor mia = {
        env->GetIntField(index, sc->QModelIndex.field_row),
        env->GetIntField(index, sc->QModelIndex.field_column),
        (void *) env->GetLongField(index, sc->QModelIndex.field_internalId),
        (QAbstractItemModel *)
        qtjambi_to_qobject(env, env->GetObjectField(index, sc->QModelIndex.field_model))
    };
    QTJAMBI_EXCEPTION_CHECK(env);
    // Newer GCC show this as type-prunned pointer will break strict-aliasing rules
#if 1
    void *vp = (void *) &mia;
    QModelIndex *qmi = (QModelIndex *) vp;
    return *qmi;
#else
    return * (QModelIndex *) (void *) (&mia) ;
#endif

}

jobject qtjambi_from_QModelIndex(JNIEnv *env, const QModelIndex &index)
{
    if (!index.isValid())
        return 0;
    StaticCache *sc = StaticCache::instance();
    sc->resolveQModelIndex();

    jobject retVal = env->NewObject(sc->QModelIndex.class_ref, sc->QModelIndex.constructor,
                                    index.row(),
                                    index.column(),
                                    index.internalId(),
                                    qtjambi_from_qobject(env,
                                                        const_cast<QAbstractItemModel *>(index.model()),
                                                        "QAbstractItemModel$ConcreteWrapper",
                                                        "org/qtjambi/qt/core/"));
    QTJAMBI_EXCEPTION_CHECK(env);
    return retVal;
}

bool qtjambi_is_created_by_java(QObject *qobject)
{
    extern int qtjambi_user_data_id;
    QtJambiLinkUserData *userData = static_cast<QtJambiLinkUserData *>(qobject->userData(qtjambi_user_data_id));

    Q_ASSERT(!userData || userData->link());

    return userData && userData->link()->createdByJava();
}

static QString qtjambi_urlbase(JNIEnv *env) {
    StaticCache *sc = StaticCache::instance();
    sc->resolveSystem();
    jstring classPath = (jstring) env->CallStaticObjectMethod(sc->System.class_ref, sc->System.getProperty, qtjambi_from_qstring(env, "org.qtjambi.qtjambi.internal.urlbase"));
    if (classPath != 0)
        return qtjambi_to_qstring(env, classPath);
    else
        return QString();
}

Q_GLOBAL_STATIC_WITH_ARGS(jobject, gClassLoader, (0))
Q_GLOBAL_STATIC(QString, oldUrlBase)
Q_GLOBAL_STATIC(QReadWriteLock, gClassLoaderLock)
jclass qtjambi_find_class(JNIEnv *env, const char *qualifiedName)
{
    if (env->ExceptionCheck()) {
        // It is not allowed to call env->FindClass() with an exception pending, this is illegal JNI
        //  usage and must be the result of a software design error.
        qWarning("qtjambi_find_class(\"%s\") with Exception pending", qualifiedName);
        Q_ASSERT(false);
        return 0;
    }

    // This should do the trick when running outside Eclipse,
    // or in the context of a previously loaded class
    jclass returned = env->FindClass(qualifiedName);
    if (returned == 0) {
        jthrowable exception = env->ExceptionOccurred();
        env->ExceptionClear();

        // Check internal property which is set in Eclipse integration
        QString pathSeparator = ";";
        QString qtClassPath = qtjambi_urlbase(env);
        if (!qtClassPath.isEmpty()) {
            QString qtClassName = QString::fromLatin1(qualifiedName).replace('/', '.');
            StaticCache *sc = StaticCache::instance();
            sc->resolveClassLoader();

            jobject classLoader = 0;
            QString oldBase;
            {
                QReadLocker locker(gClassLoaderLock());
                classLoader = *gClassLoader();
                oldBase = *oldUrlBase();
            }

            if (classLoader == 0) {
                // No class loader instantiated?
                // We have to make a URL loader, because the default is created with
                // the wrong class path in Eclipse.

                QWriteLocker locker(gClassLoaderLock());
                classLoader = *gClassLoader();
                if (classLoader == 0) {
                    sc->resolveURLClassLoader();
                    sc->resolveURL();
                    sc->resolveThread();

                    jobject currentThread = env->CallStaticObjectMethod(sc->Thread.class_ref, sc->Thread.currentThread);
                    Q_ASSERT(currentThread);

                    jobject contextClassLoader = env->CallObjectMethod(currentThread, sc->Thread.getContextClassLoader);
                    Q_ASSERT(currentThread);

                    QStringList urlList = qtClassPath.split(pathSeparator);
                    jobjectArray urls = env->NewObjectArray(urlList.size(), sc->URL.class_ref, 0);
                    Q_ASSERT(urls);

                    for (int i=0; i<urlList.size(); ++i) {
                        jobject url = env->NewObject(sc->URL.class_ref, sc->URL.constructor, qtjambi_from_qstring(env, urlList.at(i)));
                        Q_ASSERT(url);

                        env->SetObjectArrayElement(urls, i, url);
                    }

                    classLoader = env->CallStaticObjectMethod(sc->URLClassLoader.class_ref, sc->URLClassLoader.newInstance, urls, contextClassLoader);
                    Q_ASSERT(classLoader);

                    classLoader = env->NewGlobalRef(classLoader);
                    Q_ASSERT(classLoader);

                    *gClassLoader() = classLoader;
                    *oldUrlBase() = qtClassPath;
                    oldBase = qtClassPath;
                }
            }

            if (classLoader != 0 && oldBase != qtClassPath) {
                // If the class path has changed after the class loader
                // was created, we need to update the class loader.
                // This is needed to load custom widgets in the current project.

                QWriteLocker locker(gClassLoaderLock());
                sc->resolveURLClassLoader();
                sc->resolveURL();

                QStringList oldUrlList = oldUrlBase()->split(pathSeparator);
                QStringList urlList = qtClassPath.split(pathSeparator);



                for (int i=0; i<urlList.size(); ++i) {
                    if (!oldUrlList.contains(urlList.at(i))) {
                        jobject url = env->NewObject(sc->URL.class_ref, sc->URL.constructor, qtjambi_from_qstring(env, urlList.at(i)));
                        Q_ASSERT(url);
                        env->CallVoidMethod(classLoader, sc->URLClassLoader.addURL, url);
                    }
                }

                *oldUrlBase() = qtClassPath;
            }

            // Look up the class in our custom class loader
            jstring className = qtjambi_from_qstring(env, qtClassName);
            Q_ASSERT(className);
            returned = (jclass) env->CallObjectMethod(classLoader, sc->ClassLoader.loadClass, className);
        } else {
            env->Throw(exception);
        }
    }

    return returned;
}

bool qtjambi_destroy_vm()
{
    JavaVM *vm;

    vm = qtjambi_vm;	// FIXME: Want this to be an atomic read
    if (!vm)
        return false;

    return vm->DestroyJavaVM() == 0;
}

bool qtjambi_initialize_vm(const QStringList & vm_parameters, const bool ignoreUnrecognizedOptions)
{
    if (qtjambi_vm)
        return true;

    QStringList libvm = locate_vm();
    if (libvm.isEmpty()) {
#ifdef Q_OS_DARWIN
        ptrCreateJavaVM = JNI_CreateJavaVM;
        ptrGetDefaultJavaVMInitArgs =  JNI_GetDefaultJavaVMInitArgs;
        ptrGetCreatedJavaVMs = JNI_GetCreatedJavaVMs;
#else
        qWarning("Jambi: failed to initialize...");
        return false;
#endif
    }else{
        QString errors;
        foreach(QString libpath , libvm){
            QLibrary lib(libpath);
            if (!lib.load()) {
                errors += QString("Jambi: failed to load '%1':\n%2\n").arg(libpath, lib.errorString());
            }else{
                ptrCreateJavaVM = (PtrCreateJavaVM) lib.resolve("JNI_CreateJavaVM");
                ptrGetDefaultJavaVMInitArgs = (PtrGetDefaultJavaVMInitArgs) lib.resolve("JNI_GetDefaultJavaVMInitArgs");
                ptrGetCreatedJavaVMs = (PtrGetCreatedJavaVMs) lib.resolve("JNI_GetCreatedJavaVMs");
                if((void*)ptrCreateJavaVM && (void*)ptrGetDefaultJavaVMInitArgs && (void*)ptrGetCreatedJavaVMs){
                    break;
                }else{
                    errors += QString("Jambi: symbols JNI_CreateJavaVM, JNI_GetDefaultJavaVMInitArgs and JNI_GetCreatedJavaVMs not found in library %1\n").arg(libpath);
                }
            }
        }
        if(!(void*)ptrCreateJavaVM || !(void*)ptrGetDefaultJavaVMInitArgs || !(void*)ptrGetCreatedJavaVMs){
            qWarning("%s", qPrintable(errors));
#ifdef Q_OS_DARWIN
            ptrCreateJavaVM = JNI_CreateJavaVM;
            ptrGetDefaultJavaVMInitArgs =  JNI_GetDefaultJavaVMInitArgs;
            ptrGetCreatedJavaVMs = JNI_GetCreatedJavaVMs;
#else
            return false;
#endif
        }
    }

    Q_ASSERT(ptrCreateJavaVM);
    Q_ASSERT(ptrGetDefaultJavaVMInitArgs);
    Q_ASSERT(ptrGetCreatedJavaVMs);

    QByteArray class_path = getenv("CLASSPATH");
    QList<QByteArray> options;
    bool classpathGiven = false;

    // check the given vm parameters for classpath option
    for (int i = 0; i < vm_parameters.size(); ++i) {
        QString param = vm_parameters.at(i);
        if(param.startsWith("-Djava.class.path=")){
            // now join option classpath with environment CLASSPATH variable
#ifdef Q_OS_WIN
            param.append(";");
#else
            param.append(":");
#endif
            param.append(class_path);
            classpathGiven = true;
            break;
        }
    }

    // if no classpath option is given use environment CLASSPATH variable
    if(!classpathGiven){
        class_path.prepend("-Djava.class.path=");
        options << class_path;
    }
#ifndef QT_NO_DEBUG
    //options << "-Xcheck:jni";
    options << "-Dorg.qtjambi.qt.debug=true";
#endif

#ifdef Q_OS_DARWIN
    // options << "-XstartOnFirstThread"; // never use this. it does not work in a stand alone process creating a vm.
    // options << "-Xdock:icon="; // never use this. it does not work in a stand alone process creating a vm.
    // options << "-Xdock:name="; // never use this. it does not work in a stand alone process creating a vm.
#endif

    // add the committed vm parameters
    for (int i = 0; i < vm_parameters.size(); ++i) {
        options << vm_parameters.at(i).toLatin1();
    }
    if((void*)abort_hook){
        options << "abort";
        qWarning("QtJambi add abort hook");
    }
    if((void*)exit_hook){
        options << "exit";
        qWarning("QtJambi add exit hook");
    }
    if((void*)vfprintf_hook){
        options << "vfprintf";
        qWarning("QtJambi add vfprintf hook");
    }

    JavaVMOption *vm_options = new JavaVMOption[options.size()];
    for (int i=0; i<options.size(); ++i){
        vm_options[i].optionString = options[i].data();
        if(options[i]=="exit"){
            vm_options[i].extraInfo = (void*)exit_hook;
        }else if(options[i]=="abort"){
            vm_options[i].extraInfo = (void*)abort_hook;
        }else if(options[i]=="vfprintf"){
            vm_options[i].extraInfo = (void*)vfprintf_hook;
        }
    }

    JavaVMInitArgs vm_args;
    vm_args.version = JNI_VERSION_1_8;
    if(ignoreUnrecognizedOptions){
        vm_args.ignoreUnrecognized = JNI_TRUE;
    }else{
        vm_args.ignoreUnrecognized = JNI_FALSE;
    }
    vm_args.nOptions = options.size();
    vm_args.options = vm_options;

    if (ptrGetDefaultJavaVMInitArgs(&vm_args)) {
        qWarning("QtJambi: failed to get vm arguments");
        delete [] vm_options;
        return false;
    }

    qtjambi_vm = 0;
    JavaVM *vms[256];
    jsize nVMs;
    JNIEnv *env;
    if (!ptrGetCreatedJavaVMs(vms, 256, &nVMs)) {
        if (nVMs > 0)
            qtjambi_vm = vms[0];
    }

    if (qtjambi_vm == 0) {
        int result = ptrCreateJavaVM(&qtjambi_vm, (void**) (void *) &env, &vm_args);
        // print details about the occuring error
        if(result!=JNI_OK){
            if(result==JNI_ERR){
                qWarning("QtJambi: failed to create vm due to unrecognized options.");
            }else if(result==JNI_EVERSION){
                qWarning("QtJambi: failed to create vm due to a version error.");
            }else if(result==JNI_ENOMEM){
                qWarning("QtJambi: failed to create vm. Not enough memory.");
            }else if(result==JNI_EINVAL){
                qWarning("QtJambi: failed to create vm due to invalid arguments.");
            }else{
                qWarning("QtJambi: failed to create vm.");
            }
            delete [] vm_options;
            return false;
        }
    }

    delete [] vm_options;

//     const char *initializers[] = {
//         "org/qtjambi/qt/QtJambi_LibraryInitializer",
//         "org/qtjambi/qt/core/QtJambi_LibraryInitializer",
//         "org/qtjambi/qt/gui/QtJambi_LibraryInitializer",
//         0
//     };

//     for (int i=0; initializers[i]; ++i) {
//         jclass cl = env->FindClass(initializers[i]);
//         if (qtjambi_exception_check(env)) {
//             qWarning("QtJambi: failed to initialize qt jambi java libraries");
//             break;
//         }
//     }

    return true;
}

#if defined(Q_OS_LINUX) || defined(Q_OS_SOLARIS) || defined(Q_OS_FREEBSD)
static QStringList locate_vm_linux_and_solaris()
{
    QStringList result;
    QString error;

    if (!vm_location_override.isEmpty()){
        QStringList jmachs;
#if defined(Q_OS_SOLARIS)
#  if defined(__sparc) || defined(__sparcv9)
#    ifdef _LP64
        jmachs << QLatin1String("sparcv9/server");
#    else
        jmachs << QLatin1String("sparc/client");
#    endif
#  else
#    ifdef _LP64
        jmachs << QLatin1String("amd64/server");
#    else // _LP64
        jmachs << QLatin1String("i386/client");
#    endif
#  endif
#else
#  ifdef _LP64
        jmachs << QLatin1String("amd64/server");
        jmachs << QLatin1String("amd64/client");
#  else
        jmachs << QLatin1String("i386/client");
        jmachs << QLatin1String("i386/server");
#  endif
#endif
        for (int k=0; k<jmachs.size(); ++k) {
            QString jpath = QString::fromLatin1("%1/lib/").arg(vm_location_override);
            QString jmach = jmachs.at(k);
            jpath += "/"+jmach;

            jpath = QDir::cleanPath(jpath);

            if (! jpath.endsWith(QLatin1Char('/')))
                jpath += QLatin1Char('/');

            QFileInfo file(jpath + "libjvm.so");
            if (file.exists())
                result << file.absoluteFilePath();
            else {
                error += QString::fromLatin1(" - failed to locate %1\n")
                         .arg(file.absoluteFilePath());
            }
        }
    }else{
        QStringList binDirectories;
        binDirectories << "/usr/bin/" << "/usr/sbin/" << "/usr/local/bin/" << "/usr/local/bin/" << "/bin/";
        for (int i = 0; i < binDirectories.size(); ++i) {
            QFileInfo javaExecutable(binDirectories.at(i)+"java");
            if(javaExecutable.exists() && javaExecutable.isFile() && javaExecutable.isExecutable()){
                QString canonicalFilePath = javaExecutable.canonicalFilePath();
                QDir canonicalPath = QFileInfo(canonicalFilePath).dir();
                canonicalPath.cdUp();
                canonicalPath.cd("lib");

                QStringList jmachs;
    #if defined(Q_OS_SOLARIS)
    #  if defined(__sparc) || defined(__sparcv9)
    #    ifdef _LP64
                jmachs << QLatin1String("sparcv9/server");
    #    else
                jmachs << QLatin1String("sparc/client");
    #    endif
    #  else
    #    ifdef _LP64
                jmachs << QLatin1String("amd64/server");
    #    else // _LP64
                jmachs << QLatin1String("i386/client");
    #    endif
    #  endif
    #else
    #  ifdef _LP64
                jmachs << QLatin1String("amd64/server");
                jmachs << QLatin1String("amd64/client");
    #  else
                jmachs << QLatin1String("i386/client");
                jmachs << QLatin1String("i386/server");
    #  endif
    #endif
                for (int k=0; k<jmachs.size(); ++k) {
                    QString jpath = canonicalPath.canonicalPath();
                    QString jmach = jmachs.at(k);
                    jpath += "/"+jmach;

                    jpath = QDir::cleanPath(jpath);

                    if (! jpath.endsWith(QLatin1Char('/')))
                        jpath += QLatin1Char('/');

                    QFileInfo file(jpath + "libjvm.so");
                    if (file.exists())
                        result << file.absoluteFilePath();
                    else {
                        error += QString::fromLatin1(" - failed to locate %1\n")
                                 .arg(file.absoluteFilePath());
                    }
                }
            }
        }

        QStringList envVariables;
        envVariables << "JAVADIR"  << "JAVAHOME" << "JDK_HOME" << "JAVA_HOME" << "JAVA_DIR";

        for (int i = 0; i < envVariables.size(); ++i) {

            QByteArray baEnv = qgetenv(envVariables.at(i).toLatin1());
            const char *env = baEnv.constData();
            if (!env || QByteArray(env).isEmpty())
                continue;

            error += QString::fromLatin1("Searched for VM in $%1\n").arg(envVariables.at(i));

            QStringList jpaths;
            jpaths << QString::fromLatin1("%1/jre/lib/").arg(env)
                   << QString::fromLatin1("%1/lib/").arg(env);

            QStringList jmachs;

    #if defined(Q_OS_SOLARIS)
    #  if defined(__sparc) || defined(__sparcv9)
    #    ifdef _LP64
            jmachs << QLatin1String("sparcv9/server");
    #    else
            jmachs << QLatin1String("sparc/client");
    #    endif
    #  else
    #    ifdef _LP64
            jmachs << QLatin1String("amd64/server");
    #    else // _LP64
            jmachs << QLatin1String("i386/client");
    #    endif
    #  endif
    #else
    #  ifdef _LP64
            jmachs << QLatin1String("amd64/server");
            jmachs << QLatin1String("amd64/client");
    #  else
            jmachs << QLatin1String("i386/client");
            jmachs << QLatin1String("i386/server");
    #  endif
    #endif

            for (int j=0; j<jpaths.size(); ++j) {
                QString jpath = jpaths.at(j);

                for (int k=0; k<jmachs.size(); ++k) {
                    QString jmach = jmachs.at(k);
                    jpath += jmach;

                    jpath = QDir::cleanPath(jpath);

                    if (! jpath.endsWith(QLatin1Char('/')))
                        jpath += QLatin1Char('/');

                    QFileInfo file(jpath + "libjvm.so");
                    if (file.exists())
                        result << file.absoluteFilePath();
                    else {
                        error += QString::fromLatin1(" - failed to locate %1\n")
                                 .arg(file.absoluteFilePath());
                    }
                }
            }
        }
    }

    if(result.isEmpty()){
        qWarning("QtJambi: failed to locate the JVM. Make sure JAVADIR, JAVA_DIR, JAVAHOME or JAVA_HOME is set, "
                 "and pointing to your Java installation.\n%s", qPrintable(error));
    }

    return result;
}
#endif // Q_OS_LINUX || Q_OS_SOLARIS


#if defined(Q_OS_SOLARIS)

#  if defined(_ILP32) && (_FILE_OFFSET_BITS != 32)
#    define FILE_OFFSET_BITS_HACK 1
#    undef _FILE_OFFSET_BITS
#    define _FILE_OFFSET_BITS 32
#  else
#    define FILE_OFFSET_BITS_HACK 0
#  endif

#  include <dlfcn.h>
#  include <link.h>
#  include <limits.h>

#  if ((FILE_OFFSET_BITS_HACK + 0x0) == 0x1)
#    undef _FILE_OFFSET_BITS
#    define _FILE_OFFSET_BITS 64
#  endif

static QStringList locate_vm()
{
    QStringList result;
    Link_map* lmap;

    if (dlinfo(RTLD_SELF, RTLD_DI_LINKMAP, (void **) &lmap) == -1) {
        qWarning("%s", dlerror());
        return QString();
    }

    while (lmap != NULL) {
        QString line = lmap->l_name;
        if (!line.isNull() && !line.isEmpty()) {
            if (line.endsWith("libjvm.so")) {
                result << QString(line.mid(line.indexOf('/')));
            }
        }
        lmap = lmap->l_next;
    }
    if(result.isEmpty()){
        result << locate_vm_linux_and_solaris();
    }
    return result;
}


#elif defined(Q_OS_LINUX)


static QStringList locate_vm()
{
    QStringList result;
    // If libjvm is already loaded, make sure we report the same jvm.
    QFile file("/proc/self/maps");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString line = in.readLine();
        while (!line.isNull()) {
            if(line.endsWith("libjvm.so")) {
                result << line.mid(line.indexOf('/'));
            }
            line = in.readLine();
        }
    }

    if(result.isEmpty()){
        result << locate_vm_linux_and_solaris();
    }
    return result;
}

#elif defined(Q_OS_FREEBSD)

static QStringList locate_vm()
{
    QStringList result;
    // If libjvm is already loaded, make sure we report the same jvm.
    QFile file("/proc/currproc/map");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString line = in.readLine();
        while (!line.isNull()) {
            if(line.endsWith("libjvm.so")) {
                result << line.mid(line.indexOf('/'));
            }
            line = in.readLine();
        }
    }

    if(result.isEmpty()){
        result << locate_vm_linux_and_solaris();
    }
    return result;
}

#elif defined(Q_OS_WIN)

// Windows version
static QStringList locate_vm()
{
    QStringList result;
    QString javaHome;

    QString error;

    if(!vm_location_override.isEmpty())
        javaHome = vm_location_override;

    if (javaHome.isEmpty()) {
        QStringList roots, locations;
        roots << "HKEY_LOCAL_MACHINE"
              << "HKEY_CURRENT_USER";

        locations << "Java Runtime Environment"
                  << "Java Development Kit";

        QString currentVersion("CurrentVersion");

        for (QStringList::const_iterator root = roots.constBegin();
             root < roots.constEnd() && javaHome.isEmpty(); ++root) {

            for (QStringList::const_iterator loc = locations.constBegin();
                 loc < locations.constEnd() && javaHome.isEmpty(); ++loc) {
                QString regKey = *root + "\\Software\\JavaSoft\\" + *loc;

                QSettings reg(regKey, QSettings::NativeFormat);
                if (reg.contains(currentVersion)) {
                    QString version = reg.value(currentVersion).toString();
                    reg.beginGroup(version);
                    javaHome = reg.value("JavaHome").toString();
                    reg.endGroup();
                }

                if (javaHome.isEmpty()) {
                    error += QString::fromLatin1("No VM under %1\n").arg(regKey);
                } else {
                    error += QString::fromLatin1("Using VM from %1: %2\n").arg(regKey).arg(javaHome);
                }
            }
        }


        if (javaHome.isEmpty()) {
            qWarning("Jambi: Failed to locate jvm.dll\n%s", qPrintable(error));
            return result;
        }
    }

    {
        QStringList libs;
        libs << "bin/client"
             << "lib/client"
             << "bin/server"
             << "lib/server"
             << "jre/bin/client"
             << "jre/lib/client"
             << "jre/bin/server"
             << "jre/lib/server";

        for (int i=0; i<libs.size(); ++i) {
            QFileInfo fi(javaHome + "/" + libs.at(i) + "/jvm.dll");
            if (fi.exists())
                result << fi.absoluteFilePath();
            else
                error += QString::fromLatin1(" - looked for jvm in %1, failed")
                         .arg(fi.absoluteFilePath());
        }
    }
    if(result.isEmpty()){
        qWarning("Jambi: Failed to locate jvm.dll\n%s", qPrintable(error));
    }
    return result;
}

#elif defined(Q_OS_MAC)
// Mac OSX version
static QStringList locate_vm()
{
    QStringList result;
    if(vm_location_override.isEmpty()){
        QProcess process;
        process.start("/usr/libexec/java_home", QStringList() << "-v" << "1.8" << "-d64");
        if (process.waitForFinished()){
            QByteArray data = process.readAll();
            vm_location_override = QString::fromUtf8(data.constData()).trimmed();
            if(!vm_location_override.startsWith("/")){
                vm_location_override = "";
            }
        }
    }
    if(vm_location_override.isEmpty()){
        QFileInfo appletJava("/Library/Internet Plug-Ins/JavaAppletPlugin.plugin/Contents/Home/bin/java");
        if(appletJava.isFile() && appletJava.isExecutable()){
            QProcess process;
            process.start(appletJava.absoluteFilePath(), QStringList() << "-version");
            if (process.waitForFinished()){
                QString data = QString::fromUtf8(process.readAll().constData());
                if(data.contains("java version \"1.8") && data.contains("64-Bit")){
                    QDir dir = appletJava.absoluteDir();
                    dir.cdUp();
                    vm_location_override = dir.canonicalPath();
                }
            }
        }
    }
    if(!vm_location_override.isEmpty()){
        QStringList libs;
        libs << "Libraries/libjvm.dylib"
             << "Contents/Libraries/libjvm.dylib"
             << "../Libraries/libjvm.dylib"
             << "lib/client/libjvm.dylib"
             << "lib/server/libjvm.dylib"
             << "../lib/client/libjvm.dylib"
             << "../lib/server/libjvm.dylib"
             << "jre/lib/client/libjvm.dylib"
             << "jre/lib/server/libjvm.dylib";

        for (int i=0; i<libs.size(); ++i) {
            QFileInfo fi(vm_location_override + "/" + libs.at(i));
            if (fi.exists()){
                result << fi.absoluteFilePath();
            }
        }
    }
    return result;
}

#endif // OS-switch

struct ResolvedConnectionData
{
    jobject java_sender;
    jobject java_receiver;
    jobject java_signal;
    jobject java_method;
};

struct BasicConnectionData
{
    QObject *sender;
    char *signal;
    QObject *receiver;
    char *method;
};

/* If we return 'true' we always populate 'resolved_data' */
static bool qtjambi_resolve_connection_data(JNIEnv *jni_env, const BasicConnectionData *data,
                                            ResolvedConnectionData *resolved_data,
                                            bool fail_on_cpp_resolve,
                                            bool create_java_objects,
                                            bool slot_must_be_generated)
{
    Q_ASSERT(jni_env);
    Q_ASSERT(data);
    Q_ASSERT(resolved_data);
    Q_ASSERT(data->sender);
    Q_ASSERT(data->signal);

    BasicConnectionData data_normalized =
        { data->sender, data->signal, data->receiver, data->method };
    QByteArray signal_normalized = QMetaObject::normalizedSignature(data->signal);
    data_normalized.signal = signal_normalized.data();

    QByteArray method_normalized;
    method_normalized = QMetaObject::normalizedSignature(data->method);
    data_normalized.method = data->method != 0 ? method_normalized.data() : 0;
    data = &data_normalized;

    QtJambiLink *sender_link = QtJambiLink::findLinkForQObject(data->sender);
    QtJambiLink *receiver_link = QtJambiLink::findLinkForQObject(data->receiver);

    // If neither object has a link to Java, nothing to resolve
    if (sender_link == 0 && receiver_link == 0) {

        int idx = qtjambi_find_first_static_metaobject(data->sender->metaObject())->indexOfSignal(data->signal + 1);

        // Signal only exists in Java, so we need to go through java.
        if (idx < 0 || signal_normalized.at(signal_normalized.length() - 1) != ')') {
            jobject jSender = qtjambi_from_QObject(jni_env, data->sender);
            sender_link = QtJambiLink::findLink(jni_env, jSender);
            jobject jReciever = qtjambi_from_QObject(jni_env, data->receiver);
            receiver_link = QtJambiLink::findLink(jni_env, jReciever);
        } else {
            return false; // don't meddle
        }
    }

    // If both the signal and the slot exists in C++ then we make a c++ connection,
    // but we will also have to make sure java emit makes c++ emit. This is both
    // an optimization (don't go via java for pure C++ connection) and also strictly needed
    // for connections made in superclass constructors because C++ builds the
    // virtual table in stages.
    if (fail_on_cpp_resolve
        && qtjambi_find_first_static_metaobject(data->sender->metaObject())->indexOfSignal(data->signal + 1) >= 0
        && data->receiver != 0
        && data->method != 0
        && qtjambi_find_first_static_metaobject(data->receiver->metaObject())->indexOfSlot(data->method + 1) >= 0) {
        if (sender_link != 0 && !sender_link->connectedToJava()) { // connect java to c++
            qtjambi_setup_connections(jni_env, sender_link);
        }
        return false; // do it in c++
    }

    if (receiver_link == 0 && data->receiver != 0) {
        if (!create_java_objects) // don't create new java objects for disconnects
            return false;         // because unless there's a java object there isn't
                                  // a connection in java

        receiver_link = QtJambiLink::findLink(jni_env,
                                              qtjambi_from_qobject(jni_env, data->receiver,
                                                                   "QObject", "org/qtjambi/qt/core/"));
    }

    if (sender_link == 0 && data->sender != 0) {
        if (!create_java_objects)
            return false;

        sender_link = QtJambiLink::findLink(jni_env,
                                            qtjambi_from_qobject(jni_env, data->sender,
                                                                 "QObject", "org/qtjambi/qt/core/"));
    }

    if (sender_link == 0) {
        qWarning("qtjambi_resolve_connection_data(): can't resolve sender '%s'",
                    qPrintable(data->sender->objectName()));
        return false;
    }

    resolved_data->java_sender = sender_link->javaObject(jni_env);
    if (resolved_data->java_sender == 0)
        return false;


    // Does the signal exist in C++? then we need to check if it's been renamed
    const QMetaObject *mo = qtjambi_find_first_static_metaobject(data->sender->metaObject());
    const char *class_name = 0;
    int mox = mo->indexOfSignal(data->signal + 1);
    while (mox >= 0 && mo != 0 && class_name == 0) {
        if (mox >= mo->methodOffset())
            class_name =  mo->className();
        mo = mo->superClass();
    }

    {
        QString signal_name = QString::fromLatin1(data->signal + 1);
        if (class_name != 0) {
            signal_name = QLatin1String(class_name) + QLatin1String("::") + signal_name;
            signal_name = getJavaName(signal_name);
            signal_name.chop(signal_name.size() - signal_name.indexOf(QLatin1Char('(')));
            signal_name = signal_name.mid(signal_name.lastIndexOf(QLatin1Char('.')) + 1);
        }

        int ltPos = signal_name.indexOf(QLatin1Char('<'));
        if (ltPos >= 1) {
            signal_name = signal_name.left(ltPos);
        }

        ltPos = signal_name.indexOf(QLatin1Char('('));
        if (ltPos >= 1) {
            signal_name = signal_name.left(ltPos);
        }

        if (signal_name.isEmpty()) // private and compat signals
            return false;

        StaticCache *sc = StaticCache::instance();
        sc->resolveQtJambiInternal();
        resolved_data->java_signal =
            jni_env->CallStaticObjectMethod(sc->QtJambiInternal.class_ref,
                                            sc->QtJambiInternal.lookupSignal,
                                            resolved_data->java_sender,
                                            qtjambi_from_qstring(jni_env, signal_name));

        QTJAMBI_EXCEPTION_CHECK(jni_env);
    }
    if (resolved_data->java_signal == 0)  // unmapped signal, cannot be resolved by us
        return false;

    if (receiver_link != 0) {
        resolved_data->java_receiver = receiver_link->javaObject(jni_env);
        if (resolved_data->java_receiver == 0) {
            QString object_name = data->receiver->objectName();
            qWarning("qtjambi_resolve_connection_data(): attempt to connect to finalized object '%s'",
                     qPrintable(object_name));
            return false;
        }
    } else {
        resolved_data->java_receiver = 0;
    }

    if (resolved_data->java_receiver != 0 && data->method != 0) {
        mo = qtjambi_find_first_static_metaobject(data->receiver->metaObject());
        class_name = 0;
        int mox = mo->indexOfMethod(data->method + 1);
        while (mo != 0 && class_name == 0) {
            if (mox >= mo->methodOffset())
                class_name = mo->className();
            mo = mo->superClass();
        }

        {
            QString slot_name = QString::fromLatin1(data->method + 1);
            if (class_name != 0) {
                slot_name = QLatin1String(class_name) + QLatin1String("::") + slot_name;
                slot_name = getJavaName(slot_name);
                int paren_pos = slot_name.indexOf(QLatin1Char('('));
                slot_name = slot_name.mid(slot_name.lastIndexOf(QLatin1Char('.'), paren_pos) + 1);
            }
            if (slot_name.isEmpty()) // Private and compat slots...
                return false;

            StaticCache *sc = StaticCache::instance();
            sc->resolveQtJambiInternal();
            sc->resolveAbstractSignal();

            // If the slot is actually a signal in Java, we must handle it by finding the
            // ref to the emit() method. Otherwise we find the function specified.
            if (class_name == 0 && data->method[0] - '0' == QSIGNAL_CODE) {

                int pos = slot_name.indexOf('(');
                if (pos >= 1)
                    slot_name = slot_name.left(pos);

                resolved_data->java_receiver =
                    jni_env->CallStaticObjectMethod(sc->QtJambiInternal.class_ref,
                                                    sc->QtJambiInternal.lookupSignal,
                                                    resolved_data->java_receiver,
                                                    qtjambi_from_qstring(jni_env, slot_name));
                QTJAMBI_EXCEPTION_CHECK(jni_env);
                if (resolved_data->java_receiver == 0)
                    return false;

                resolved_data->java_method =
                    jni_env->CallStaticObjectMethod(sc->QtJambiInternal.class_ref,
                                                    sc->QtJambiInternal.findEmitMethod,
                                                    resolved_data->java_receiver);
                QTJAMBI_EXCEPTION_CHECK(jni_env);
            } else {
                resolved_data->java_method =
                    jni_env->CallStaticObjectMethod(sc->QtJambiInternal.class_ref,
                                                    sc->QtJambiInternal.lookupSlot,
                                                    resolved_data->java_receiver,
                                                    qtjambi_from_qstring(jni_env, slot_name),
                                                    slot_must_be_generated);
                QTJAMBI_EXCEPTION_CHECK(jni_env);
            }
        }

        if (resolved_data->java_method == 0)
            return false;

    } else {
        resolved_data->java_method = 0;
    }

    return true;
}

static void qtjambi_disconnect_all(JNIEnv *jni_env, QObject *sender, QObject *receiver)
{
    QtJambiLink *java_link = QtJambiLink::findLinkForQObject(sender);
    if (java_link != 0) {
        jobject java_sender = java_link->javaObject(jni_env);
        if (!java_sender) {
            // This may for instance happens when a object has
            // disconnect() as part of its destructor
            return;
        }
        jobject java_receiver = receiver != 0
            ? qtjambi_from_qobject(jni_env, receiver, "QObject", "org/qtjambi/qt/core/")
            : 0;

        StaticCache *sc = StaticCache::instance();
        sc->resolveQSignalEmitter();
        // It is allowed for the argument java_receiver==0
        jni_env->CallVoidMethod(java_sender, sc->QSignalEmitter.disconnect, java_receiver);
    }
}

static bool qtjambi_disconnect_callback(void **raw_data)
{
    Q_ASSERT(raw_data != 0);

    // ### try to avoid...
    if (QCoreApplication::closingDown())
        return false;

    JNIEnv *jni_env = qtjambi_current_environment();
    Q_ASSERT(jni_env != 0);

    BasicConnectionData *data = reinterpret_cast<BasicConnectionData *>(raw_data);
    Q_ASSERT(data->sender);
    if (data->method == 0 && data->signal == 0) {
        qtjambi_disconnect_all(jni_env, data->sender, data->receiver);
        QTJAMBI_EXCEPTION_CHECK(jni_env);
    } else {

        ResolvedConnectionData resolved_data;
        if (!qtjambi_resolve_connection_data(jni_env, data, &resolved_data, false, false, false))
            return false;

        StaticCache *sc = StaticCache::instance();
        sc->resolveAbstractSignal();
        Q_ASSERT(resolved_data.java_signal);
        jni_env->CallBooleanMethod(resolved_data.java_signal,
                                sc->AbstractSignal.removeConnection,
                                resolved_data.java_receiver,
                                resolved_data.java_method);
        QTJAMBI_EXCEPTION_CHECK(jni_env);
    }

    return false; // always disconnect through default impl. as well
}

struct ConnectData: public BasicConnectionData
{
    Qt::ConnectionType *type;
};

static bool qtjambi_connect_callback(void **raw_data, bool slotMustBeGenerated)
{
    Q_ASSERT(raw_data != 0);

    JNIEnv *jni_env = qtjambi_current_environment();
    Q_ASSERT(jni_env != 0);

    ConnectData *data = reinterpret_cast<ConnectData *>(raw_data);
    if (data->sender == 0 || data->signal == 0 || data->receiver == 0
        || data->method == 0  || data->type == 0) {
        qWarning("qtjambi_connect_callback(): received unexpected null parameters");
        return false;
    }
//     printf("   --- (connect callback) %s::%s -> %s::%s, %d\n",
//            data->sender->metaObject()->className(), data->signal,
//            data->receiver->metaObject()->className(), data->method,
//            *data->type);

    ResolvedConnectionData resolved_data;
    if (!qtjambi_resolve_connection_data(jni_env, data, &resolved_data, true, true, slotMustBeGenerated))
        return false;

    StaticCache *sc = StaticCache::instance();
    sc->resolveAbstractSignal();
    Q_ASSERT(resolved_data.java_signal);
    jni_env->CallVoidMethod(resolved_data.java_signal,
                                             sc->AbstractSignal.connectSignalMethod,
                                             resolved_data.java_method,
                                             resolved_data.java_receiver,
                                             *data->type);
    if(qtjambi_exception_check(jni_env))
        return false;

    // If we succeeded in connecting in Java, we skip the C++ connection
    return true;
}

static bool qtjambi_connect_callback(void **raw_data)
{
    return qtjambi_connect_callback(raw_data, false);
}

static bool qtjambi_event_notify(void **data)
{
    QEvent *event = (QEvent *) data[1];
    bool *result = (bool *) data[2];

#if defined(QTJAMBI_DEBUG_TOOLS)
    char tmpbuf[512];
    const char *eventdesc = QtJambiDebugEvent::qtjambi_event_desc_to_string(tmpbuf, sizeof(tmpbuf), event);
    Q_UNUSED(eventdesc);
#endif

    switch ((int)event->type()) {
    case 513:
        // this is a delete event that has to happen in the gui thread.
        Q_ASSERT(QThread::currentThread() == QCoreApplication::instance()->thread());
        static_cast<QtJambiDestructorEvent *>(event)->callDestructor();
        return true;

    case 512:
        qtjambi_metacall(qtjambi_current_environment(), event);
        *result = true;
        return true;

    case QEvent::ChildAdded:
    case QEvent::ChildRemoved:
        {
            QChildEvent *e = static_cast<QChildEvent *>(event);

            QObject *parent = reinterpret_cast<QObject *>(data[0]);
//             printf("(child) [%s :  %s] %s [%s : %s]\n",
//                    qPrintable(e->child()->objectName()), e->child()->metaObject()->className(),
//                    e->added() ? "added to" : "removed from",
//                    qPrintable(parent->objectName()), parent->metaObject()->className());

            // We're not interested in objects that don't have a link as the GC won't
            // be interferring with them anyway.
            QtJambiLink *link = QtJambiLink::findLinkForQObject(e->child());
            if (link) {
                if (link->qobject()) {
                    JNIEnv *env = qtjambi_current_environment();
                    if (!env) // VM has shut down...
                        return false;
                    if (e->added())
                       link->setCppOwnership(env, link->javaObject(env));
                    else
                       link->setDefaultOwnership(env, link->javaObject(env));
                } else if (event->type() == QEvent::ChildAdded) {
                    qWarning("%s [%s] was garbage collected before it was reparented to %s [%s]",
                         qPrintable(e->child()->objectName()), e->child()->metaObject()->className(),
                         qPrintable(parent->objectName()), parent->metaObject()->className());
                }
            }
        }
        break;

    default:
        break;
    }

    return false;
}

void qtjambi_register_callbacks()
{
#if QT_VERSION >= 0x040300
    QInternal::registerCallback(QInternal::EventNotifyCallback, qtjambi_event_notify);
#endif

#if QT_VERSION < 0x050000
    /*
     * Since it is no longer possible to connect callbacks for connection/disconnection
     * in Qt5, the Java signals cannot connected/disconnected from C++ by calling
     * QObject::connect() and QObject::disconnect().
     *
     */
    QInternal::registerCallback(QInternal::ConnectCallback,    qtjambi_connect_callback);
    QInternal::registerCallback(QInternal::DisconnectCallback, qtjambi_disconnect_callback);
    QInternal::registerCallback(QInternal::AdoptCurrentThread, qtjambi_adopt_current_thread);
#else
#ifdef QT_QTJAMBI_PORT
    QInternal::registerCallback(QInternal::ConnectCallback,    qtjambi_connect_callback);
    QInternal::registerCallback(QInternal::DisconnectCallback, qtjambi_disconnect_callback);
    QInternal::registerCallback(QInternal::AdoptCurrentThread, qtjambi_adopt_current_thread);
#endif
#endif

    QMetaType::registerStreamOperators(QMetaType::typeName(qMetaTypeId<JObjectWrapper>()),
                       reinterpret_cast<QMetaType::SaveOperator>(jobjectwrapper_save),
                       reinterpret_cast<QMetaType::LoadOperator>(jobjectwrapper_load));
}

void qtjambi_unregister_callbacks() 
{
#if QT_VERSION < 0x050000
    {
        // unregisterType() will clear pointer returned by typeName() causing an
        // invalid memory read access during the unregisterType() call so we must
        // copy the string value to a local.
        const char *typenamestr = QMetaType::typeName(qMetaTypeId<JObjectWrapper>());
        size_t len = strlen(typenamestr);
        char *typenamebuf = new char[len+1];
        strcpy(typenamebuf, typenamestr);

        QMetaType::unregisterType(typenamebuf);

        delete[] typenamebuf;
    }
#endif

#if QT_VERSION < 0x050000
    /*
     * Since it is no longer possible to connect callbacks for connection/disconnection
     * in Qt5, the Java signals cannot connected/disconnected from C++ by calling
     * QObject::connect() and QObject::disconnect().
     *
     */
    QInternal::unregisterCallback(QInternal::AdoptCurrentThread, qtjambi_adopt_current_thread);
    QInternal::unregisterCallback(QInternal::DisconnectCallback, qtjambi_disconnect_callback);
    QInternal::unregisterCallback(QInternal::ConnectCallback,    qtjambi_connect_callback);
#else
#ifdef QT_QTJAMBI_PORT
    QInternal::unregisterCallback(QInternal::AdoptCurrentThread, qtjambi_adopt_current_thread);
    QInternal::unregisterCallback(QInternal::DisconnectCallback, qtjambi_disconnect_callback);
    QInternal::unregisterCallback(QInternal::ConnectCallback,    qtjambi_connect_callback);
#endif
#endif

#if QT_VERSION >= 0x040300
    QInternal::unregisterCallback(QInternal::EventNotifyCallback, qtjambi_event_notify);
#endif

}


void qtjambi_end_paint(JNIEnv *env, jobject widget)
{
    StaticCache *sc = StaticCache::instance();
    sc->resolveQtJambiGuiInternal();
    env->CallStaticVoidMethod(sc->QtJambiGuiInternal.class_ref, sc->QtJambiGuiInternal.endPaint, widget);
    QTJAMBI_EXCEPTION_CHECK(env);
}

// qtjambi_qt_metacast: this method is called from the generated shell code to implement
//    the cast, the implementation used to be unrolled into the shell but was moved here
//    to reduce code size (increase i-cache efficiency) as well as implement the slower
//    path relating to Java object lookup and manipulation.
// className: is the orignal argument passed by Qt QMetaObject API
// return value: -1 has special meaning to instruct caller to offer the request to the
//    superclass via WhateverSuperClass::qt_metacast(className).
//
// FIXME: This would appear to be another entry point that another thread might access QtJambiLink from?
void *qtjambishell_qt_metacast(const char *className, const char *shellClassName,
                                           QtJambiLink *link, void *shellMetaCast)
{
    void *rv = 0;

    Q_ASSERT(shellClassName);
    Q_ASSERT(link);
    Q_ASSERT(shellMetaCast);

    if (!className)
        goto done;

    {
        JNIEnv *env = qtjambi_current_environment();
        Q_ASSERT(env);

        jobject java_object = link->javaObject(env); // we should always take a local ref ?
        if (java_object) {
            const QString name = qtjambi_object_class_name(env, java_object).replace(".", "::");
            const QString wantedClassName = QString(className);
            // CHECKME should we be returning the signal wrapper at any time here ?
            if(wantedClassName == name)  // string compare
                rv = shellMetaCast;  // reuse the Shell
            // to the outside worlds point of view the shell is the Java object
            link->javaObjectDeleteIfLocalRef(env, java_object);
        }
    }

    if (rv == 0) {
        if (!strcmp(className, shellClassName))
            rv = shellMetaCast;
        else
            rv = (void *)-1;  // -1 has special meaning
    }

done:
    return rv;
}

void qtjambishell_throw_nullpointerexception(JNIEnv *env, const char *msg)
{
    StaticCache *sc = StaticCache::instance();
    sc->resolveNullPointerException();
    env->ThrowNew(sc->NullPointerException.class_ref, msg);
}

void qtjambi_debug_trace(const char *location, const char *file, int line)
{
    static int should = getenv("QTJAMBI_DEBUG_TRACE") != 0;
    if (should) {
        fprintf(stderr, "%s; ( %s:%d )\n", location, file, line);
        fflush(stderr);
    }
}

void qtjambi_set_vm_location_override(const QString &location)
{
    vm_location_override = location;
}

typedef QHash<QString, const QMetaObject *> MetaObjectHash;
Q_GLOBAL_STATIC(MetaObjectHash, metaObjects);
Q_GLOBAL_STATIC_WITH_ARGS(QMutex, metaObjectsLock, (QMutex::Recursive));

class StaticQtMetaObjectAccessor: public QObject
{
public:
    static const QMetaObject *illicitMetaObjectForQtNamespace() { return &staticQtMetaObject; }
};

const QMetaObject *qtjambi_metaobject_for_class(JNIEnv *env, jclass object_class, const QMetaObject *original_meta_object)
{
    StaticCache *sc = StaticCache::instance();
    sc->resolveQtJambiInternal();
    sc->resolveClass();
    sc->resolveQObject();
    if(!env->IsAssignableFrom(object_class, sc->QObject.class_ref)){
        return 0;
    }
    Q_ASSERT(object_class != 0);

    // If original_meta_object is null then we have to look it up

    QString class_name = qtjambi_class_name(env, object_class);
    Q_ASSERT(!class_name.isEmpty());

    const QMetaObject *returned = 0;
    {
        QMutexLocker locker(metaObjectsLock());
        returned = metaObjects()->value(class_name, 0);
        if (returned == 0) {
            // Return original meta object for generated classes, and
            // create a new dynamic meta object for subclasses
            if (env->CallStaticBooleanMethod(sc->QtJambiInternal.class_ref, sc->QtJambiInternal.isGeneratedClass, object_class)) {
                if (original_meta_object == 0) {
                    sc->resolveQt();
                    if (env->IsSameObject(sc->Qt.class_ref, object_class)) {
                        original_meta_object = StaticQtMetaObjectAccessor::illicitMetaObjectForQtNamespace();
                    } else {
                        jmethodID originalMetaObjectID = env->GetStaticMethodID(object_class, "originalMetaObject", "()J");
                        Q_ASSERT(originalMetaObjectID != 0);

                        // yee
                        original_meta_object = reinterpret_cast<const QMetaObject *>(env->CallStaticLongMethod(object_class, originalMetaObjectID));
                    }
                }
                returned = original_meta_object;
            } else {
                returned = new QtDynamicMetaObject(env, object_class, original_meta_object);
            }
            metaObjects()->insert(class_name, returned);
        }
    }

    return returned;
}

bool qtjambi_metaobject_is_dynamic(const QMetaObject *meta_object) {
    if (meta_object == 0) return false;

    int idx = meta_object->indexOfClassInfo("__qt__binding_shell_language");
    return (idx >= 0 && !strcmp(meta_object->classInfo(idx).value(), "Qt Jambi"));
}

bool JObjectWrapper::operator==(const JObjectWrapper &other) const
{
    if (object != 0) {
        JNIEnv *env = qtjambi_current_environment();
        StaticCache *sc = StaticCache::instance();
        sc->resolveObject();
        Q_ASSERT(env->IsInstanceOf(object, sc->Object.class_ref));  // check the java object is right type (objects on JVM don't have to inherit java.lang.Object)
        return env->CallBooleanMethod(object, sc->Object.equals, other.object);
    } else {
        return (object == other.object);
    }
}

void JObjectWrapper::initialize(JNIEnv *env, jobject obj)
{
    object = env->NewGlobalRef(obj);
}

JObjectWrapper::~JObjectWrapper()
{
    DEREF_JOBJECT;

    if (object){
        JNIEnv *env = qtjambi_current_environment();
        if(env){
            env->DeleteGlobalRef(object);
        }else{
	    // this may sometimes happen during shutdown.
            qWarning("Memory leak in QtJambi: Deleting global reference failed in thread %s[%s]",
                     QThread::currentThread()->objectName().toLatin1().constData(),
                     QString::number((long)QThread::currentThreadId()).toLatin1().constData());
        }
    }
}

QString qtjambi_enum_name_for_flags_name(JNIEnv *env, const QString &qualified_name)
{
    jclass flags_class = qtjambi_find_class(env, qualified_name.toUtf8());
    if (flags_class == 0) {
        qtjambi_exception_check(env);
        return QString();
    }

    StaticCache *sc = StaticCache::instance();
    sc->resolveMetaObjectTools();

    jclass enum_class = reinterpret_cast<jclass>(env->CallStaticObjectMethod(sc->MetaObjectTools.class_ref, sc->MetaObjectTools.getEnumForQFlags, flags_class));
    if (enum_class == 0) {
        qWarning("No enum type found for flags type '%s'", qPrintable(qualified_name));
        qtjambi_exception_check(env);
        return QString();
    }

    return qtjambi_class_name(env, enum_class);
}

void qtjambi_register_variant_handler()
{
    QtJambiVariant::registerHandler();
}

JObjectWrapper qtjambi_to_jobjectwrapper(JNIEnv *env, jobject java_object)
{
    return JObjectWrapper(env, java_object);
}

jobject qtjambi_from_jobjectwrapper(JNIEnv *env, const JObjectWrapper &wrapper)
{
    return env->NewLocalRef(wrapper.object);
}

QVarLengthArray<jvalue> qtjambi_from_jobjectArray(JNIEnv *env,
                                                  jobjectArray args,
                                                  jintArray _cnvTypes,
                                                  bool globalRefs)
{
    Q_ASSERT(_cnvTypes);
    int len = env->GetArrayLength(_cnvTypes);
    jint *cnvTypes = env->GetIntArrayElements(_cnvTypes, 0);
    QVarLengthArray<jvalue> argsArray(len);
    Q_ASSERT(args);
    for (int i=0; i<len; ++i) {
        jobject arg_object = env->GetObjectArrayElement(args, i);
        switch (cnvTypes[i]) {
        case 'L': argsArray[i].l = globalRefs ? env->NewGlobalRef(arg_object) : arg_object; break ;
        case 'Z': argsArray[i].z = qtjambi_to_boolean(env, arg_object); break ;
        case 'J': argsArray[i].j = qtjambi_to_long(env, arg_object); break ;
        case 'I': argsArray[i].i = qtjambi_to_int(env, arg_object); break ;
        case 'F': argsArray[i].f = qtjambi_to_float(env, arg_object); break ;
        case 'D': argsArray[i].d = qtjambi_to_double(env, arg_object); break ;
        case 'S': argsArray[i].s = qtjambi_to_short(env, arg_object); break ;
        case 'B': argsArray[i].b = qtjambi_to_byte(env, arg_object); break ;
        case 'C': argsArray[i].c = qtjambi_to_jchar(env, arg_object); break ;
        default:
            Q_ASSERT_X(false, "qtjambi_invoke_slot", "Error in conversion array");
        }
    }
    env->ReleaseIntArrayElements(_cnvTypes, cnvTypes, JNI_ABORT);

    return argsArray;
}

jobject qtjambi_invoke_method(JNIEnv *env,
                            jobject receiver,
                            jmethodID methodId,
                            jbyte returnType,
                            QVarLengthArray<jvalue> argsArray)
{
    switch (returnType)
    {
    case 'L': return env->CallObjectMethodA(receiver, methodId, argsArray.data());
    case 'V': env->CallVoidMethodA(receiver, methodId, argsArray.data()); return 0;
    case 'I': return qtjambi_from_int(env, env->CallIntMethodA(receiver, methodId, argsArray.data()));
    case 'J': return qtjambi_from_long(env, env->CallLongMethodA(receiver, methodId, argsArray.data()));
    case 'S': return qtjambi_from_short(env, env->CallShortMethodA(receiver, methodId, argsArray.data()));
    case 'Z': return qtjambi_from_boolean(env, env->CallBooleanMethodA(receiver, methodId, argsArray.data()));
    case 'F': return qtjambi_from_float(env, env->CallFloatMethodA(receiver, methodId, argsArray.data()));
    case 'D': return qtjambi_from_double(env, env->CallDoubleMethodA(receiver, methodId, argsArray.data()));
    case 'B': return qtjambi_from_byte(env, env->CallByteMethodA(receiver, methodId, argsArray.data()));
    case 'C': return qtjambi_from_char(env, env->CallCharMethodA(receiver, methodId, argsArray.data()));
    default:
        Q_ASSERT_X(false, "qtjambi_invoke_slot", "Invalid return type parameter");
        return 0;
    };
}

