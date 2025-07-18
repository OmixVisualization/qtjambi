/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED
#include "qtjambiapi.h"
#include <QtCore/private/qobject_p.h>
#include <QtCore/private/qcoreapplication_p.h>
#include <QtCore/QThread>
#include <QtCore/QAbstractItemModel>
#include "qtjambishell_p.h"
#include "qtjambilink_p.h"
#include "java_p.h"
#include "registryutil_p.h"
#include "containerapi.h"
#include "containeraccess_p.h"
#include "supertypeinfo_p.h"
#include "qtjambimetaobject_p.h"
#include "threadutils_p.h"
#include "testapi.h"
#include "utils_p.h"
#include "modelapi.h"
#include "threadapi.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
#define qAsConst std::as_const
#endif

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

class VTable
{
public:
    VTable(JNIEnv* env, jclass clazz);

    virtual jclass javaClass() const;

    virtual jmethodID javaMethod(const std::type_info& typeId, int pos) const;

    virtual const QMetaObject* metaObject() const;

    virtual bool isEmpty() const = 0;

    virtual ~VTable();
private:
    jclass m_class;
};

Q_GLOBAL_STATIC(QReadWriteLock, gTableLock)
typedef SecureContainer<QHash<int, QSharedPointer<const VTable>>, gTableLock> VTableHash;
Q_GLOBAL_STATIC(VTableHash, functionTableCache)

QtJambiShell::QtJambiShell(){}

QtJambiShell::~QtJambiShell(){}

const void * QtJambiShell::qt_metacast(const char *className, bool* ok) const
{
    return const_cast<QtJambiShell*>(this)->qt_metacast(className, ok);
}

void * QtJambiShell::qt_metacast(const char *className, bool* ok)
{
    if(ok) *ok = false;
    void *rv = nullptr;
    if (!className){
        if(ok) *ok = true;
    }else if (className==QtJambiShellImpl::id){
        rv = const_cast<QtJambiShellImpl*>(static_cast<const QtJambiShellImpl*>(this));
        if(ok) *ok = true;
    }else if(QSharedPointer<QtJambiLink> link = static_cast<const QtJambiShellImpl*>(this)->m_link){
        const std::type_info* qtType = getTypeByQtName(className);
        if(qtType && link->isInterfaceAvailable(*qtType)){
            rv = link->typedPointer(*qtType);
        }
        if (rv == nullptr) {
            qtType = getInterfaceTypeForIID(className);
            if(qtType && link->isInterfaceAvailable(*qtType)){
                rv = link->typedPointer(*qtType);
            }
        }
        if (rv == nullptr) {
            if(JniEnvironment env{1024}){
                if (jobject java_object = link->getJavaObjectLocalRef(env)) {
                    QString name = QtJambiAPI::getObjectClassName(env, java_object);
                    if(name==className
                            || QString(name).replace(".", "::").replace("$", "::")==className
                            || QString(name).replace(".", "/")==className)  // string compare
                        rv = link->pointer();  // reuse the Shell
                }
            }
        }
        if(ok) *ok = rv!=nullptr;
    }
    return rv;
}

int QtJambiShell::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    QTJAMBI_INTERNAL_INSTANCE_METHOD_CALL("QtJambiShell::qt_metacall(QMetaObject::Call _c, int _id, void **_a)", this)
    const QMetaObject *meta_object = metaObject();
    if(const QtJambiMetaObject *dynamic_meta_object = QtJambiMetaObject::cast(meta_object)){
        if (QSharedPointer<QtJambiLink> __link = static_cast<const QtJambiShellImpl*>(this)->m_link) {
            if(JniEnvironment __jni_env{1024}){
                  jobject __obj = __link->getJavaObjectLocalRef(__jni_env);
                  if(!__jni_env->IsSameObject(__obj, nullptr)){
                      switch (_c) {
                      case QMetaObject::CreateInstance:
                          _id = dynamic_meta_object->invokeConstructor(__jni_env, _id, _a); break;
                      case QMetaObject::InvokeMetaMethod:
                          _id = dynamic_meta_object->invokeSignalOrSlot(__jni_env, __obj, __link->isQObject() ? __link->qobject() : nullptr, _id, _a); break;
                      case QMetaObject::ReadProperty:
                          _id = dynamic_meta_object->readProperty(__jni_env, __obj, __link->isQObject() ? __link->qobject() : nullptr, _id, _a); break;
                      case QMetaObject::WriteProperty:
                          _id = dynamic_meta_object->writeProperty(__jni_env, __obj, __link->isQObject() ? __link->qobject() : nullptr, _id, _a); break;
                      case QMetaObject::ResetProperty:
                          _id = dynamic_meta_object->resetProperty(__jni_env, __obj, __link->isQObject() ? __link->qobject() : nullptr, _id, _a); break;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                      case QMetaObject::QueryPropertyDesignable:
                          _id = dynamic_meta_object->queryPropertyDesignable(__jni_env, __obj, __link->isQObject() ? __link->qobject() : nullptr, _id, _a); break;
                      case QMetaObject::QueryPropertyScriptable:
                          _id = dynamic_meta_object->queryPropertyScriptable(__jni_env, __obj, __link->isQObject() ? __link->qobject() : nullptr, _id, _a); break;
                      case QMetaObject::QueryPropertyUser:
                          _id = dynamic_meta_object->queryPropertyUser(__jni_env, __obj, __link->isQObject() ? __link->qobject() : nullptr, _id, _a); break;
                      case QMetaObject::QueryPropertyStored:
                          _id = dynamic_meta_object->queryPropertyStored(__jni_env, __obj, __link->isQObject() ? __link->qobject() : nullptr, _id, _a); break;
                      case QMetaObject::QueryPropertyEditable:
                          _id = dynamic_meta_object->queryPropertyEditable(__jni_env, __obj, __link->isQObject() ? __link->qobject() : nullptr, _id, _a); break;
#else
                      case QMetaObject::BindableProperty:
                          _id = dynamic_meta_object->bindableProperty(__jni_env, __obj, __link->isQObject() ? __link->qobject() : nullptr, _id, _a); break;
#endif
#if 0
                      case QMetaObject::IndexOfMethod:
                          printf("QMetaObject::IndexOfMethod\n");
                          break;
                      case QMetaObject::RegisterPropertyMetaType:
                          printf("QMetaObject::RegisterPropertyMetaType\n");
                          break;
                      case QMetaObject::RegisterMethodArgumentMetaType:
                          printf("QMetaObject::RegisterMethodArgumentMetaType\n");
                          break;
#endif
                      default:
                          break;
                      }
                  }else if(__link->pointer() && !__link->isDeleteLater()){
                      qCWarning(DebugAPI::internalCategory, "QtJambiMetaObject::metaCall: metaCall on incomplete object ot type %s",
                                dynamic_meta_object->className());
                  }
             }
        }
    }
    return _id;
}

void QtJambiShell::warnForMethod(const char* method) const
{
    if(QSharedPointer<QtJambiLink> lnk = static_cast<const QtJambiShellImpl*>(this)->m_link){
        qCWarning(DebugAPI::debugAPIJavaOverloadsCategory, "%s: The java object has been deleted prior to the native object [%s].", method, qPrintable(lnk->describe()));
    }else{
        qCWarning(DebugAPI::debugAPIJavaOverloadsCategory, "%s: The java object has been deleted prior to the native object [object link deleted].", method);
    }
}

void QtJambiShell::warnForMethod(const char* method, const QObject* object) const
{
    if(object){
        if(QSharedPointer<QtJambiLink> lnk = static_cast<const QtJambiShellImpl*>(this)->m_link){
            qCWarning(DebugAPI::debugAPIJavaOverloadsCategory, "%s: The java object has been deleted prior to the native object [class=\"%s\", objectName=\"%s\", %s].", method, qPrintable(object->metaObject()->className()), qPrintable(object->objectName()), qPrintable(lnk->describe()));
        }else{
            qCWarning(DebugAPI::debugAPIJavaOverloadsCategory, "%s: The java object has been deleted prior to the native object [class=\"%s\", objectName=\"%s\", object link deleted].", method, qPrintable(object->metaObject()->className()), qPrintable(object->objectName()));
        }
    }else{
        if(QSharedPointer<QtJambiLink> lnk = static_cast<const QtJambiShellImpl*>(this)->m_link){
            qCWarning(DebugAPI::debugAPIJavaOverloadsCategory, "%s: The java object has been deleted prior to the native object [%s].", method, qPrintable(lnk->describe()));
        }else{
            qCWarning(DebugAPI::debugAPIJavaOverloadsCategory, "%s: The java object has been deleted prior to the native object [object link deleted].", method);
        }
    }
}

jobject QtJambiShell::getJavaObjectLocalRef(JNIEnv *env) const
{
    if(QSharedPointer<QtJambiLink> lnk = static_cast<const QtJambiShellImpl*>(this)->m_link){
        return lnk->getJavaObjectLocalRef(env);
    }
    return nullptr;
}

jclass QtJambiShell::javaClass() const
{
    Q_ASSERT(static_cast<const QtJambiShellImpl*>(this)->m_vtable);
    return static_cast<const QtJambiShellImpl*>(this)->m_vtable->javaClass();
}

jmethodID QtJambiShell::javaMethod(const std::type_info& typeId, int pos) const
{
    Q_ASSERT(static_cast<const QtJambiShellImpl*>(this)->m_vtable);
    return static_cast<const QtJambiShellImpl*>(this)->m_vtable->javaMethod(typeId, pos);
}

bool QtJambiShell::tryDeleteShell(const std::type_info& typeId){
    if(enabledDanglingPointerCheck()){
        if(!checkedGetTypeInfo(RegistryAPI::Private::PolymorphicTypeInfoSupplier<QtJambiShell>::value, this)){
            qCWarning(DebugAPI::internalCategory, "Possible memory leak while deleting %s: Cannot delete shell at %p", qPrintable(QtJambiAPI::typeName(typeId)), this);
            return false;
        }
    }
    deleteShell();
    return true;
}

const QMetaObject* QtJambiShell::metaObject() const
{
    Q_ASSERT(static_cast<const QtJambiShellImpl*>(this)->m_vtable);
    return static_cast<const QtJambiShellImpl*>(this)->m_vtable->metaObject();
}

QtJambiShellImpl::QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
  : m_ptr(ptr),
    m_vtable(QtJambiShellImpl::setupVTable(__jni_env, objectClass, __jni_object, typeId, superTypeInfos, nullptr, false, ocurredException)),
    m_link(QtJambiLink::createLinkForNewObject(__jni_env, objectClass, nativeLink,
                                               __jni_object, typeId, ptr, superTypeInfos,
                                               created_by_java, is_shell, destructor_function, ownerFunction, ocurredException))
{
}

QtJambiShellImpl::QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            PtrOwnerFunction ownerFunction, JavaException& ocurredException)
  : m_ptr(ptr),
    m_vtable(QtJambiShellImpl::setupVTable(__jni_env, objectClass, __jni_object, typeId, superTypeInfos, nullptr, false, ocurredException)),
    m_link(QtJambiLink::createLinkForNewObject(__jni_env, objectClass, nativeLink,
                                               __jni_object, typeId, ptr, superTypeInfos, metaType,
                                               created_by_java, is_shell, ownerFunction, ocurredException))
{
}

QtJambiShellImpl::QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr,
                                   bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            AbstractContainerAccess* containerAccess, JavaException& ocurredException)
  : m_ptr(ptr),
    m_vtable(QtJambiShellImpl::setupVTable(__jni_env, objectClass, __jni_object, typeId, superTypeInfos, nullptr, false, ocurredException)),
    m_link(QtJambiLink::createLinkForNewObject(__jni_env, objectClass, nativeLink,
                                               __jni_object, typeId, ptr, superTypeInfos,
                                               created_by_java, is_shell, containerAccess, ocurredException))
{
}

QtJambiShellImpl::QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, JavaException& ocurredException)
  : m_ptr(ptr),
    m_vtable(QtJambiShellImpl::setupVTable(__jni_env, objectClass, __jni_object, typeId, superTypeInfos, nullptr, false, ocurredException)),
    m_link(QtJambiLink::createLinkForNewObject(__jni_env, objectClass, nativeLink,
                                               __jni_object, typeId, ptr, superTypeInfos,
                                               created_by_java, is_shell, containerAccess, destructor_function, ocurredException))
{
}

QtJambiShellImpl::QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
  : m_ptr(ptr),
    m_vtable(QtJambiShellImpl::setupVTable(__jni_env, objectClass, __jni_object, typeId, superTypeInfos, nullptr, false, ocurredException)),
    m_link(QtJambiLink::createLinkForNewObject(__jni_env, objectClass, nativeLink,
                                               __jni_object, typeId, ptr, superTypeInfos,
                                               created_by_java, is_shell, containerAccess, destructor_function, ownerFunction, ocurredException))
{
}

QtJambiShellImpl::QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            PtrDeleterFunction destructor_function, JavaException& ocurredException)
  : m_ptr(ptr),
    m_vtable(QtJambiShellImpl::setupVTable(__jni_env, objectClass, __jni_object, typeId, superTypeInfos, nullptr, false, ocurredException)),
    m_link(QtJambiLink::createLinkForNewObject(__jni_env, objectClass, nativeLink,
                                               __jni_object, typeId, ptr, superTypeInfos,
                                               created_by_java, is_shell, destructor_function, ocurredException))
{
}

QtJambiShellImpl::QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            JavaException& ocurredException)
  : m_ptr(ptr),
    m_vtable(QtJambiShellImpl::setupVTable(__jni_env, objectClass, __jni_object, typeId, superTypeInfos, nullptr, false, ocurredException)),
    m_link(QtJambiLink::createLinkForNewObject(__jni_env, objectClass, nativeLink,
                                               __jni_object, typeId, ptr, superTypeInfos,
                                               created_by_java, is_shell, ocurredException))
{
}

QtJambiShellImpl::QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            JavaException& ocurredException)
  : m_ptr(ptr),
    m_vtable(QtJambiShellImpl::setupVTable(__jni_env, objectClass, __jni_object, typeId, superTypeInfos, nullptr, false, ocurredException)),
    m_link(QtJambiLink::createLinkForNewObject(__jni_env, objectClass, nativeLink,
                                               __jni_object, typeId, ptr, superTypeInfos, metaType,
                                               created_by_java, is_shell, ocurredException))
{
}

QtJambiShellImpl::QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QObject* ptr, const QMetaObject* originalMetaObject, bool created_by_java, bool isDeclarativeCall, bool is_shell, bool hasCustomMetaObject, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException)
    : m_ptr(ptr),
      m_vtable(QtJambiShellImpl::setupVTable(__jni_env, objectClass, __jni_object, typeId, superTypeInfos, originalMetaObject, hasCustomMetaObject, ocurredException)),
      m_link(QtJambiLink::createLinkForNewQObject(__jni_env, objectClass, nativeLink, __jni_object, typeId, m_vtable ? m_vtable->metaObject() : originalMetaObject, ptr, superTypeInfos, created_by_java, isDeclarativeCall, is_shell, hasCustomMetaObject, ocurredException))
{
}

QtJambiShellImpl::QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QThread* ptr, const QMetaObject* originalMetaObject, bool created_by_java, bool isDeclarativeCall, bool is_shell, bool hasCustomMetaObject, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException)
    : m_ptr(ptr),
      m_vtable(QtJambiShellImpl::setupVTable(__jni_env, objectClass, __jni_object, typeId, superTypeInfos, originalMetaObject, hasCustomMetaObject, ocurredException)),
      m_link(QtJambiLink::createLinkForNewQObject(__jni_env, objectClass, nativeLink, __jni_object, typeId, m_vtable ? m_vtable->metaObject() : originalMetaObject, ptr, superTypeInfos, created_by_java, isDeclarativeCall, is_shell, hasCustomMetaObject, ocurredException))
{
}

QtJambiShellInterface::~QtJambiShellInterface(){}

jobject QtJambiShellInterface::getJavaObjectLocalRef(JNIEnv *env, const QtJambiShellInterface* shellInterface){
    return shellInterface ? shellInterface->__shell()->getJavaObjectLocalRef(env) : nullptr;
}

QtJambiShellImpl::~QtJambiShellImpl()
{
}

void QtJambiShellImpl::overrideLink(const QSharedPointer<QtJambiLink>& link){
    m_link = link.toWeakRef();
}

bool QtJambiShellImpl::hasEmptyVTable() const{
    return !m_vtable || m_vtable->isEmpty();
}

void QtJambiShellImpl::init(JNIEnv* env){
    if(QSharedPointer<QtJambiLink> lnk = m_link){
        lnk->init(env);
    }
}

QSharedPointer<QtJambiLink> QtJambiShellImpl::link() const
{
    return m_link.toStrongRef();
}

const char *const QtJambiShellImpl::id = "QtJambiShell";

void * QtJambiShellImpl::getPlacement(JNIEnv *env, jclass callingClass, jobject placement_object)
{
    Q_UNUSED(callingClass)
    Q_ASSERT(env);
    if(!placement_object)
        JavaException::raiseNullPointerException(env, "QDeclarativeConstructor is null." QTJAMBI_STACKTRACEINFO );
    jclass cls = Java::QtCore::QObject$QDeclarativeConstructor::cls(env, placement_object);
    void* placement = reinterpret_cast<void*>(Java::QtCore::QObject$QDeclarativeConstructor::placement(env, placement_object));
    if(!env->IsAssignableFrom(cls, callingClass) || !placement){
        Java::Runtime::IllegalAccessException::throwNew(env, "Illegal use of QDeclarativeConstructor parameter." QTJAMBI_STACKTRACEINFO );
    }
    Java::QtCore::QObject$QDeclarativeConstructor::set_placement(env, placement_object, jlong(0));
    return placement;
}

QtJambiScope* QtJambiShellImpl::returnScope(JNIEnv *env, const std::type_info&, uint){
    Java::Runtime::IllegalAccessException::throwNew(env, "Illegal use of QtJambiShell::returnScope." QTJAMBI_STACKTRACEINFO );
    throw;
}

namespace ThreadPrivate{
void purgeLink(QSharedPointer<QtJambiLink>&& link);
}

void QtJambiShellImpl::deleteShell(){
    QTJAMBI_DEBUG_METHOD_PRINT_LINKNAME(m_link, "QtJambiShellImpl::deleteShell()")
#if defined(QTJAMBI_DEBUG_TOOLS) || !defined(QT_NO_DEBUG)
    const char* qtTypeName = nullptr;
#endif
    if(QSharedPointer<QtJambiLink> link = m_link){
#if defined(QTJAMBI_DEBUG_TOOLS) || !defined(QT_NO_DEBUG)
        qtTypeName = link->qtTypeName();
#endif
        if(link->isQThread()){
            if(DefaultJniEnvironment env{200}){
                link->invalidate(env);
                link.reset();
            }
        }else{
            if(JniEnvironment env{200}){
                link->invalidate(env);
                link.reset();
            }
        }
        if(link)
            ThreadPrivate::purgeLink(std::move(link));
    }
    operator delete(m_ptr);
    QTJAMBI_INCREASE_COUNTER2(shellDestroyedCount, qtTypeName)
    delete this;
}

struct DestructorInfo{
    DestructorInfo() = default;
    DestructorInfo(const DestructorInfo&) = default;
    DestructorInfo(
            void* _ptr,
            RegistryAPI::DestructorFn _destructor,
            const std::type_info& _typeId,
            PtrOwnerFunction _ownerFunction
        );
    const std::type_info& typeId();
    void destruct() const;
    const QObject* owner() const;
private:
    void* ptr = nullptr;
    RegistryAPI::DestructorFn destructor = nullptr;
    const std::type_info* m_typeId = nullptr;
    PtrOwnerFunction ownerFunction = nullptr;
};

DestructorInfo::DestructorInfo(
        void* _ptr,
        RegistryAPI::DestructorFn _destructor,
        const std::type_info& _typeId,
        PtrOwnerFunction _ownerFunction
    ) : ptr(_ptr),
    destructor(_destructor),
    m_typeId(&_typeId),
    ownerFunction(_ownerFunction)
{}

const std::type_info& DestructorInfo::typeId(){
    Q_ASSERT(m_typeId);
    return *m_typeId;
}

void DestructorInfo::destruct() const{
    Q_ASSERT(destructor);
    destructor(ptr);
}

const QObject* DestructorInfo::owner() const{
    return ownerFunction ? ownerFunction(ptr) : nullptr;
}

class DestructionHelper : public QObject {
public:
    DestructionHelper(const QPointer<const QObject>& owner, QtJambiUtils::Runnable&& purge);
    bool event(QEvent * e) override;
private:
    QPointer<const QObject> m_owner;
    QtJambiUtils::Runnable m_purge;

    Q_DISABLE_COPY_MOVE(DestructionHelper)
};

DestructionHelper::DestructionHelper(const QPointer<const QObject>& owner, QtJambiUtils::Runnable&& purge)
    : QObject(),
      m_owner(owner),
      m_purge(std::move(purge))
{
    Q_ASSERT(owner);
    this->moveToThread(owner->thread());
}

bool DestructionHelper::event(QEvent * e){
    if(e->type()==QEvent::Type(QEvent::DeferredDelete)){
        if(m_owner && m_owner->thread()!=this->thread()){
            this->moveToThread(m_owner->thread());
            this->deleteLater();
            return true;
        }
        m_purge();
    }
    return QObject::event(e);
}

class SingleTypeShell : public QtJambiShellImpl{
public:
    using QtJambiShellImpl::QtJambiShellImpl;
    ~SingleTypeShell() override;
    void destructed(const std::type_info& typeId) override;
    void constructed(const std::type_info& typeId) override;
    void deleteShell() override;
private:
#ifdef QT_DEBUG
    bool m_isAlive = false;
#endif
    Q_DISABLE_COPY_MOVE(SingleTypeShell)
};

class SingleTypeRSShell : public SingleTypeShell{
public:
    SingleTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                    PtrOwnerFunction ownerFunction, JavaException& ocurredException, uint returnScopeCount);
    SingleTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                    PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException, uint returnScopeCount);
    SingleTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                    JavaException& ocurredException, uint returnScopeCount);
    SingleTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                    PtrDeleterFunction destructor_function, JavaException& ocurredException, uint returnScopeCount);
    SingleTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException, uint returnScopeCount);
    SingleTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr,
                      bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                      AbstractContainerAccess* containerAccess, JavaException& ocurredException, uint returnScopeCount);
    SingleTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr,
                      bool created_by_java, bool is_shell,
                      const SuperTypeInfos* superTypeInfos,
                      AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, JavaException& ocurredException, uint returnScopeCount);
    SingleTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr,
                      bool created_by_java, bool is_shell,
                      const SuperTypeInfos* superTypeInfos,
                      AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException, uint returnScopeCount);
    SingleTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QObject* ptr, const QMetaObject* originalMetaObject, bool created_by_java, bool isDeclarativeCall, bool is_shell, bool hasCustomMetaObject, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException, uint returnScopeCount);
    SingleTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QThread* ptr, const QMetaObject* originalMetaObject, bool created_by_java, bool isDeclarativeCall, bool is_shell, bool hasCustomMetaObject, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException, uint returnScopeCount);
    ~SingleTypeRSShell() override;
    QtJambiScope* returnScope(JNIEnv *env, const std::type_info& typeId, uint index) override;
    void deleteShell() override;
private:
    QVector<QtJambiScope*> m_scopes;
    Q_DISABLE_COPY_MOVE(SingleTypeRSShell)
};

class MultiTypeShell : public QtJambiShellImpl{
public:
    MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                   PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                   PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                   PtrDeleterFunction destructor_function, JavaException& ocurredException);
    MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException);
    MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                   JavaException& ocurredException);
    MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr,
                   const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                   AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                   AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, JavaException& ocurredException);
    MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                   AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QObject* ptr, const QMetaObject* originalMetaObject, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool isDeclarativeCall, bool is_shell, bool hasCustomMetaObject, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException);
    MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QThread* ptr, const QMetaObject* originalMetaObject, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool isDeclarativeCall, bool is_shell, bool hasCustomMetaObject, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException);
    ~MultiTypeShell() override;
    void destructed(const std::type_info& typeId) override;
    void constructed(const std::type_info& typeId) override;
    void deleteShell() override;
    void superDeleteShell();
private:
    QSet<size_t> m_constructedTypes;
    const QHash<size_t,DestructorInfo> m_destructorInfo;

    Q_DISABLE_COPY_MOVE(MultiTypeShell)
};

class MultiTypeRSShell : public MultiTypeShell{
public:
    MultiTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                     PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException, QMap<size_t,QVector<QtJambiScope*>>&& returnScopes);
    MultiTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                     PtrOwnerFunction ownerFunction, JavaException& ocurredException, QMap<size_t,QVector<QtJambiScope*>>&& returnScopes);
    MultiTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                     PtrDeleterFunction destructor_function, JavaException& ocurredException, QMap<size_t,QVector<QtJambiScope*>>&& returnScopes);
    MultiTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException, QMap<size_t,QVector<QtJambiScope*>>&& returnScopes);
    MultiTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                     JavaException& ocurredException, QMap<size_t,QVector<QtJambiScope*>>&& returnScopes);
    MultiTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr,
                     const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                     AbstractContainerAccess* containerAccess, JavaException& ocurredException, QMap<size_t,QVector<QtJambiScope*>>&& returnScopes);
    MultiTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                     AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, JavaException& ocurredException, QMap<size_t,QVector<QtJambiScope*>>&& returnScopes);
    MultiTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                     AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException, QMap<size_t,QVector<QtJambiScope*>>&& returnScopes);
    MultiTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QObject* ptr, const QMetaObject* originalMetaObject, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool isDeclarativeCall, bool is_shell, bool hasCustomMetaObject, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException, QMap<size_t,QVector<QtJambiScope*>>&& returnScopes);
    MultiTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QThread* ptr, const QMetaObject* originalMetaObject, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool isDeclarativeCall, bool is_shell, bool hasCustomMetaObject, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException, QMap<size_t,QVector<QtJambiScope*>>&& returnScopes);
    ~MultiTypeRSShell() override;
    QtJambiScope* returnScope(JNIEnv *env, const std::type_info& typeId, uint index) override;
    void deleteShell() override;
private:
    QMap<size_t,QVector<QtJambiScope*>> m_scopes;
    Q_DISABLE_COPY_MOVE(MultiTypeRSShell)
};

SingleTypeShell::~SingleTypeShell(){}

MultiTypeShell::MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            PtrOwnerFunction ownerFunction, JavaException& ocurredException)
  : QtJambiShellImpl(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, metaType, created_by_java, is_shell, superTypeInfos, ownerFunction, ocurredException),
    m_constructedTypes(),
    m_destructorInfo(destructorInfo)
{
}

MultiTypeShell::MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
  : QtJambiShellImpl(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, created_by_java, is_shell, superTypeInfos, destructor_function, ownerFunction, ocurredException),
    m_constructedTypes(),
    m_destructorInfo(destructorInfo)
{
}

MultiTypeShell::MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            JavaException& ocurredException)
  : QtJambiShellImpl(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, metaType, created_by_java, is_shell, superTypeInfos, ocurredException),
    m_constructedTypes(),
    m_destructorInfo(destructorInfo)
{
}

MultiTypeShell::MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
  : QtJambiShellImpl(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, created_by_java, is_shell, superTypeInfos, containerAccess, destructor_function, ownerFunction, ocurredException),
    m_constructedTypes(),
    m_destructorInfo(destructorInfo)
{
}

MultiTypeShell::MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, JavaException& ocurredException)
  : QtJambiShellImpl(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, created_by_java, is_shell, superTypeInfos, containerAccess, destructor_function, ocurredException),
    m_constructedTypes(),
    m_destructorInfo(destructorInfo)
{
}

MultiTypeShell::MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr,
                               const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            AbstractContainerAccess* containerAccess, JavaException& ocurredException)
  : QtJambiShellImpl(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr,
                     created_by_java, is_shell, superTypeInfos, containerAccess, ocurredException),
    m_constructedTypes(),
    m_destructorInfo(destructorInfo)
{
}

MultiTypeShell::MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            PtrDeleterFunction destructor_function, JavaException& ocurredException)
  : QtJambiShellImpl(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, created_by_java, is_shell, superTypeInfos, destructor_function, ocurredException),
    m_constructedTypes(),
    m_destructorInfo(destructorInfo)
{
}

MultiTypeShell::MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException)
  : QtJambiShellImpl(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, created_by_java, is_shell, superTypeInfos, ocurredException),
    m_constructedTypes(),
    m_destructorInfo(destructorInfo)
{
}

MultiTypeShell::MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QObject* ptr, const QMetaObject* originalMetaObject, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool isDeclarativeCall, bool is_shell, bool hasCustomMetaObject, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException)
    : QtJambiShellImpl(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, originalMetaObject, created_by_java, isDeclarativeCall, is_shell, hasCustomMetaObject, superTypeInfos, ocurredException),
      m_constructedTypes(),
      m_destructorInfo(destructorInfo)
{
}

MultiTypeShell::MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QThread* ptr, const QMetaObject* originalMetaObject, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool isDeclarativeCall, bool is_shell, bool hasCustomMetaObject, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException)
    : QtJambiShellImpl(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, originalMetaObject, created_by_java, isDeclarativeCall, is_shell, hasCustomMetaObject, superTypeInfos, ocurredException),
      m_constructedTypes(),
      m_destructorInfo(destructorInfo)
{
}

MultiTypeShell::~MultiTypeShell(){}

void SingleTypeShell::constructed(const std::type_info& typeId){
#ifdef QT_DEBUG
    m_isAlive = true;
#endif
    if(QSharedPointer<QtJambiLink> lnk = link()){
        lnk->addInterface(typeId);
    }
}

void SingleTypeShell::destructed(const std::type_info& typeId){
#ifdef QT_DEBUG
    m_isAlive = false;
#endif
    if(QSharedPointer<QtJambiLink> lnk = link()){
        lnk->removeInterface(typeId);
        QTJAMBI_INCREASE_COUNTER(shellDestructorCalledCount, lnk)
    }
}

void SingleTypeShell::deleteShell(){
#ifdef QT_DEBUG
    Q_ASSERT(!m_isAlive);
#endif
    QtJambiShellImpl::deleteShell();
}

void MultiTypeShell::destructed(const std::type_info& typeId){
    m_constructedTypes.remove(unique_id(typeId));
    if(QSharedPointer<QtJambiLink> lnk = link()){
        lnk->removeInterface(typeId);
        QTJAMBI_INCREASE_COUNTER(shellDestructorCalledCount, lnk)
    }
}

void MultiTypeShell::constructed(const std::type_info& typeId){
    m_constructedTypes << unique_id(typeId);
    if(QSharedPointer<QtJambiLink> lnk = link()){
        lnk->addInterface(typeId);
    }
}

void MultiTypeShell::superDeleteShell(){
    QtJambiShellImpl::deleteShell();
}

void MultiTypeShell::deleteShell(){
    if(m_constructedTypes.isEmpty()){
        superDeleteShell();
    }else{
        QSharedPointer<QAtomicInt> ref(new QAtomicInt(0));
        QList<DestructionHelper*> destructionHelpers;
        QSet<size_t> constructedTypes(m_constructedTypes);
        for(size_t typeIdx : constructedTypes){
            const DestructorInfo& info = m_destructorInfo[typeIdx];
            QWriteLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            if(const QObject* obj = info.owner()){
                QPointer<const QObject> owner;
                if(obj){
                    const QObjectPrivate* p = QObjectPrivate::get(obj);
                    if(p && !p->wasDeleted){
                        owner = obj;
                    }
                }
                if(owner->thread()!=QThread::currentThread()){
                    ref->ref();
                    destructionHelpers << new DestructionHelper(owner, [this,typeIdx,ref](){
                        this->m_destructorInfo[typeIdx].destruct();
                        ref->deref();
                        if(ref->loadRelaxed()==0){
                            this->superDeleteShell();
                        }
                    });
                }else{
                    locker.unlock();
                    info.destruct();
                    locker.relock();
                }
            }else{
                locker.unlock();
                info.destruct();
                locker.relock();
            }
        }
        if(ref->loadRelaxed()==0){
            this->superDeleteShell();
        }
        for(DestructionHelper* destructionHelper : qAsConst(destructionHelpers)){
            destructionHelper->deleteLater();
        }
    }
}

SingleTypeRSShell::SingleTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                                 PtrOwnerFunction ownerFunction, JavaException& ocurredException, uint returnScopeCount)
    : SingleTypeShell(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, metaType, created_by_java, is_shell, superTypeInfos, ownerFunction, ocurredException)
    ,m_scopes(returnScopeCount)
{
}

SingleTypeRSShell::SingleTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                                 PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException, uint returnScopeCount)
    : SingleTypeShell(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, created_by_java, is_shell, superTypeInfos, destructor_function, ownerFunction, ocurredException)
    ,m_scopes(returnScopeCount)
{
}

SingleTypeRSShell::SingleTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                                 JavaException& ocurredException, uint returnScopeCount)
    : SingleTypeShell(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, metaType, created_by_java, is_shell, superTypeInfos, ocurredException)
    ,m_scopes(returnScopeCount)
{
}

SingleTypeRSShell::SingleTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                                 AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException, uint returnScopeCount)
    : SingleTypeShell(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, created_by_java, is_shell, superTypeInfos, containerAccess, destructor_function, ownerFunction, ocurredException)
    ,m_scopes(returnScopeCount)
{
}

SingleTypeRSShell::SingleTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                                 AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, JavaException& ocurredException, uint returnScopeCount)
    : SingleTypeShell(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, created_by_java, is_shell, superTypeInfos, containerAccess, destructor_function, ocurredException)
    ,m_scopes(returnScopeCount)
{
}

SingleTypeRSShell::SingleTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr,
                                     bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                                 AbstractContainerAccess* containerAccess, JavaException& ocurredException, uint returnScopeCount)
    : SingleTypeShell(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr,
                      created_by_java, is_shell, superTypeInfos, containerAccess, ocurredException)
    ,m_scopes(returnScopeCount)
{
}

SingleTypeRSShell::SingleTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                                 PtrDeleterFunction destructor_function, JavaException& ocurredException, uint returnScopeCount)
    : SingleTypeShell(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, created_by_java, is_shell, superTypeInfos, destructor_function, ocurredException)
    ,m_scopes(returnScopeCount)
{
}

SingleTypeRSShell::SingleTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                                 JavaException& ocurredException, uint returnScopeCount)
    : SingleTypeShell(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, created_by_java, is_shell, superTypeInfos, ocurredException)
    ,m_scopes(returnScopeCount)
{
}

SingleTypeRSShell::SingleTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QObject* ptr, const QMetaObject* originalMetaObject, bool created_by_java, bool isDeclarativeCall, bool is_shell, bool hasCustomMetaObject, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException, uint returnScopeCount)
    : SingleTypeShell(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, originalMetaObject, created_by_java, isDeclarativeCall, is_shell, hasCustomMetaObject, superTypeInfos, ocurredException)
    ,m_scopes(returnScopeCount)
{
}

SingleTypeRSShell::SingleTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QThread* ptr, const QMetaObject* originalMetaObject, bool created_by_java, bool isDeclarativeCall, bool is_shell, bool hasCustomMetaObject, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException, uint returnScopeCount)
    : SingleTypeShell(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, originalMetaObject, created_by_java, isDeclarativeCall, is_shell, hasCustomMetaObject, superTypeInfos, ocurredException)
    ,m_scopes(returnScopeCount)
{
}

SingleTypeRSShell::~SingleTypeRSShell(){
    while(!m_scopes.isEmpty()){
        QtJambiScope* scope = m_scopes.takeFirst();
        if(scope)
            delete scope;
    }
}

void SingleTypeRSShell::deleteShell(){
    while(!m_scopes.isEmpty()){
        QtJambiScope* scope = m_scopes.takeFirst();
        if(scope)
            delete scope;
    }
    SingleTypeShell::deleteShell();
}

QtJambiScope* SingleTypeRSShell::returnScope(JNIEnv *, const std::type_info&, uint index){
    QtJambiScope* result = new QtJambiScope(this);
    if(m_scopes[index])
        delete m_scopes[index];
    m_scopes[index] = result;
    return result;
}

MultiTypeRSShell::MultiTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                               PtrOwnerFunction ownerFunction, JavaException& ocurredException, QMap<size_t,QVector<QtJambiScope*>>&& returnScopes)
    : MultiTypeShell(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, metaType, destructorInfo, created_by_java, is_shell, superTypeInfos, ownerFunction, ocurredException),
    m_scopes(std::move(returnScopes))
{
}

MultiTypeRSShell::MultiTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                               PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException, QMap<size_t,QVector<QtJambiScope*>>&& returnScopes)
    : MultiTypeShell(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, destructorInfo, created_by_java, is_shell, superTypeInfos, destructor_function, ownerFunction, ocurredException),
    m_scopes(std::move(returnScopes))
{
}

MultiTypeRSShell::MultiTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                               JavaException& ocurredException, QMap<size_t,QVector<QtJambiScope*>>&& returnScopes)
    : MultiTypeShell(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, metaType, destructorInfo, created_by_java, is_shell, superTypeInfos, ocurredException),
    m_scopes(std::move(returnScopes))
{
}

MultiTypeRSShell::MultiTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                               AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException, QMap<size_t,QVector<QtJambiScope*>>&& returnScopes)
    : MultiTypeShell(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, destructorInfo, created_by_java, is_shell, superTypeInfos, containerAccess, destructor_function, ownerFunction, ocurredException),
    m_scopes(std::move(returnScopes))
{
}

MultiTypeRSShell::MultiTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                               AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, JavaException& ocurredException, QMap<size_t,QVector<QtJambiScope*>>&& returnScopes)
    : MultiTypeShell(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, destructorInfo, created_by_java, is_shell, superTypeInfos, containerAccess, destructor_function, ocurredException),
    m_scopes(std::move(returnScopes))
{
}

MultiTypeRSShell::MultiTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr,
                                   const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                               AbstractContainerAccess* containerAccess, JavaException& ocurredException, QMap<size_t,QVector<QtJambiScope*>>&& returnScopes)
    : MultiTypeShell(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr,
                     destructorInfo, created_by_java, is_shell, superTypeInfos, containerAccess, ocurredException),
    m_scopes(std::move(returnScopes))
{
}

MultiTypeRSShell::MultiTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                               PtrDeleterFunction destructor_function, JavaException& ocurredException, QMap<size_t,QVector<QtJambiScope*>>&& returnScopes)
    : MultiTypeShell(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, destructorInfo, created_by_java, is_shell, superTypeInfos, destructor_function, ocurredException),
    m_scopes(std::move(returnScopes))
{
}

MultiTypeRSShell::MultiTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException, QMap<size_t,QVector<QtJambiScope*>>&& returnScopes)
    : MultiTypeShell(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, destructorInfo, created_by_java, is_shell, superTypeInfos, ocurredException),
    m_scopes(std::move(returnScopes))
{
}

MultiTypeRSShell::MultiTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QObject* ptr, const QMetaObject* originalMetaObject, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool isDeclarativeCall, bool is_shell, bool hasCustomMetaObject, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException, QMap<size_t,QVector<QtJambiScope*>>&& returnScopes)
    : MultiTypeShell(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, originalMetaObject, destructorInfo, created_by_java, isDeclarativeCall, is_shell, hasCustomMetaObject, superTypeInfos, ocurredException),
    m_scopes(std::move(returnScopes))
{
}

MultiTypeRSShell::MultiTypeRSShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QThread* ptr, const QMetaObject* originalMetaObject, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool isDeclarativeCall, bool is_shell, bool hasCustomMetaObject, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException, QMap<size_t,QVector<QtJambiScope*>>&& returnScopes)
    : MultiTypeShell(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, originalMetaObject, destructorInfo, created_by_java, isDeclarativeCall, is_shell, hasCustomMetaObject, superTypeInfos, ocurredException),
    m_scopes(std::move(returnScopes))
{
}

MultiTypeRSShell::~MultiTypeRSShell(){
    while(!m_scopes.isEmpty()){
        QVector<QtJambiScope*> vec = m_scopes.take(m_scopes.firstKey());
        while(!vec.isEmpty()){
            QtJambiScope* scope = vec.takeFirst();
            if(scope)
                delete scope;
        }
    }
}

void MultiTypeRSShell::deleteShell(){
    while(!m_scopes.isEmpty()){
        QVector<QtJambiScope*> vec = m_scopes.take(m_scopes.firstKey());
        while(!vec.isEmpty()){
            QtJambiScope* scope = vec.takeFirst();
            if(scope)
                delete scope;
        }
    }
    MultiTypeShell::deleteShell();
}

QtJambiScope* MultiTypeRSShell::returnScope(JNIEnv *, const std::type_info& typeId, uint index){
    QtJambiScope* result = new QtJambiScope(this);
    auto uid = unique_id(typeId);
    if(m_scopes[uid][index])
        delete m_scopes[uid][index];
    m_scopes[uid][index] = result;
    return result;
}

class SingleTypeModelShell : public SingleTypeShell, public QtJambiModelShell{
public:
    using SingleTypeShell::SingleTypeShell;
    ModelData* modelData() override {return &m_modelData;}
private:
    ModelData m_modelData;
};

class SingleTypeRSModelShell : public SingleTypeRSShell, public QtJambiModelShell{
public:
    using SingleTypeRSShell::SingleTypeRSShell;
    ModelData* modelData() override {return &m_modelData;}
private:
    ModelData m_modelData;
};

class MultiTypeModelShell : public MultiTypeShell, public QtJambiModelShell{
public:
    using MultiTypeShell::MultiTypeShell;
    ModelData* modelData() override {return &m_modelData;}
private:
    ModelData m_modelData;
};

class MultiTypeRSModelShell : public MultiTypeRSShell, public QtJambiModelShell{
public:
    using MultiTypeRSShell::MultiTypeRSShell;
    ModelData* modelData() override {return &m_modelData;}
private:
    ModelData m_modelData;
};

void assignShell(const SuperTypeInfos& superTypeInfos, quintptr ptr, QtJambiShellImpl* shell){
    for(const SuperTypeInfo& info : superTypeInfos){
        *reinterpret_cast<QtJambiShellImpl**>(ptr + info.offset() + info.size()) = shell;
    }
}

void QtJambiShell::initialize(JNIEnv *env, jclass callingClass, jobject object,
                                      ConstructorFunction constructorFunction, size_t size, const std::type_info& typeId, uint returnScopeRequired, bool isShell,
                                      PtrOwnerFunction ownerFunction, jvalue* arguments){
    if(!object)
        return;
    QTJAMBI_JNI_LOCAL_FRAME(env, 64);
    jclass objectClass = env->GetObjectClass(object);
    jobject nativeLink = QtJambiLink::getNativeLink(env, object);
    QMetaType metaType(registeredMetaTypeID(typeId));
    Q_ASSERT(metaType.isValid());
    if(env->IsSameObject(objectClass, callingClass)){
        if(isShell){
            size_t offset = size;
            size += sizeof(QtJambiShell*);
            void* ptr = operator new(size);
            memset(ptr, 0, size);
            try{
                JavaException ocurredException;
                SingleTypeShell* shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, true, isShell, nullptr, ownerFunction, ocurredException, returnScopeRequired)
                                                                 : new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, true, isShell, nullptr, ownerFunction, ocurredException);
                if(ocurredException){
                    delete shell;
                    ocurredException.raise();
                    return;
                }
                *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                try{
                    constructorFunction(ptr, env, object, arguments, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), false);
                }catch(...){
                    if(QSharedPointer<QtJambiLink> link = shell->link()){
                        link->invalidate(env);
                    }
                    delete shell;
                    throw;
                }
                shell->init(env);
            }catch(...){
                operator delete(ptr);
                throw;
            }
        }else{
            void* ptr = operator new(size);
            memset(ptr, 0, size);
            try{
                JavaException ocurredException;
                const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewObject(env, objectClass, nativeLink, object, typeId, ptr, nullptr, metaType,
                                                                                              true, isShell, ownerFunction, ocurredException);
                if(ocurredException){
                    ocurredException.raise();
                    return;
                }else{
                    try{
                        constructorFunction(ptr, env, object, arguments, false, false, false);
                    }catch(...){
                        if(link){
                            link->invalidate(env);
                        }
                        throw;
                    }
                    if(link){
                        link->init(env);
                    }
                }
            }catch(...){
                operator delete(ptr);
                throw;
            }
        }
    }else{
        const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, objectClass);
        switch(superTypeInfos.size()){
        case 0:
            JavaException::raiseError(env, "Cannot determine type information about object's class." QTJAMBI_STACKTRACEINFO );
            break;
        case 1:
            Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
            if(isShell){
                size_t offset = size;
                size += sizeof(QtJambiShell*);
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    SingleTypeShell* shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, metaType, true, isShell, &superTypeInfos, ownerFunction, ocurredException, returnScopeRequired)
                                                                     : new SingleTypeShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, metaType, true, isShell, &superTypeInfos, ownerFunction, ocurredException);
                    if(ocurredException){
                        delete shell;
                        ocurredException.raise();
                        return;
                    }
                    *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                    try{
                        constructorFunction(ptr, env, object, arguments, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), false);
                    }catch(...){
                        if(QSharedPointer<QtJambiLink> link = shell->link()){
                            link->invalidate(env);
                        }
                        delete shell;
                        throw;
                    }
                    shell->init(env);
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }else{
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewObject(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, &superTypeInfos, metaType,
                                                                                                  true, isShell, ownerFunction, ocurredException);
                    if(ocurredException){
                        ocurredException.raise();
                    }else{
                        try{
                            constructorFunction(ptr, env, object, arguments, false, false, false);
                        }catch(...){
                            if(link){
                                link->invalidate(env);
                            }
                            throw;
                        }
                        if(link){
                            link->init(env);
                        }
                    }
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }
            break;
        default:
            Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
            if(isShell || isInterface(typeId)){
                size_t totalSize = 0;
                totalSize += size;
                totalSize += sizeof(QtJambiShell*);

                QList<ConstructorFunction> constructorFunctions;
                constructorFunctions << constructorFunction;

                for(int i=1; i<superTypeInfos.size(); ++i){
                    const SuperTypeInfo& info = superTypeInfos[i];
                    if(info.constructorInfos().isEmpty()){
                        jthrowable t = Java::QtJambi::QInterfaceCannotBeSubclassedException::newInstance(env, info.javaClass());
                        JavaException(env, t).raise();
                        return;
                    }else{
                        totalSize += info.size();
                        totalSize += sizeof(QtJambiShell*);
                        ConstructorFunction foundConstructorFunction = nullptr;
                        for(int j=0; j<info.constructorInfos().size(); ++j){
                            const ResolvedConstructorInfo & constructorInfo = info.constructorInfos().at(j);
                            if(constructorInfo.argumentTypes.isEmpty()){
                                foundConstructorFunction = constructorInfo.constructorFunction;
                                break;
                            }
                        }
                        if(!foundConstructorFunction){
                            Java::Runtime::Error::throwNew(env, QStringLiteral("Cannot initialize interface %1 without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.").arg(QString(info.className()).replace("/", ".").replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                        }
                        constructorFunctions << foundConstructorFunction;
                    }
                }

                void* ptr = operator new(totalSize);
                memset(ptr, 0, totalSize);
                try{
                    QMap<size_t,QVector<QtJambiScope*>> returnScopeMap;
                    QHash<size_t,DestructorInfo> destructorInfo;
                    returnScopeRequired = 0;
                    for(const SuperTypeInfo& info : superTypeInfos){
                        void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                        destructorInfo[unique_id(info.typeId())] = DestructorInfo(iptr, info.destructor(), info.typeId(), info.ownerFunction());
                        auto rs = returnScopes(info.typeId());
                        returnScopeRequired += rs;
                        if(rs>0)
                            returnScopeMap[unique_id(info.typeId())].resize(rs);
                    }
                    JavaException ocurredException;
                    MultiTypeShell* shell = returnScopeRequired>0 ? new MultiTypeRSShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, metaType, destructorInfo, true, isShell, &superTypeInfos, ownerFunction, ocurredException, std::move(returnScopeMap))
                                                                    : new MultiTypeShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, metaType, destructorInfo, true, isShell, &superTypeInfos, ownerFunction, ocurredException);
                    if(ocurredException){
                        delete shell;
                        ocurredException.raise();
                        return;
                    }
                    assignShell(superTypeInfos, quintptr(ptr), shell);
                    auto i=superTypeInfos.size()-1;
                    try{
                        for(; i>=0; --i){
                            const SuperTypeInfo& info = superTypeInfos[i];
                            jvalue* args = i==0 ? arguments : nullptr;
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            constructorFunctions.at(i)(iptr, env, object, args, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), constructorFunction!=constructorFunctions.at(i));
                            if(!info.hasShell())
                                shell->constructed(info.typeId());
                        }
                    }catch(const JavaException& exn){
                        for(auto j=i+1; j<superTypeInfos.size(); ++j){
                            const SuperTypeInfo& info = superTypeInfos[j];
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            QTJAMBI_TRY{
                                info.destructor()(iptr);
                            }QTJAMBI_CATCH(const JavaException& exn){
                                exn.addSuppressed(env, exn);
                            }QTJAMBI_TRY_END
                            if(!info.hasShell())
                                shell->destructed(info.typeId());
                        }
                        if(QSharedPointer<QtJambiLink> link = shell->link()){
                            link->invalidate(env);
                        }
                        delete shell;
                        exn.raise();
                        return;
                    }
                    shell->init(env);
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }else{
                Java::Runtime::Error::throwNew(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg(QtJambiAPI::getClassName(env, callingClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
            break;
        }
    }
}

void QtJambiShell::initialize(JNIEnv *env, jclass callingClass, jobject object,
                                      ConstructorFunction constructorFunction, size_t size, const std::type_info& typeId, uint returnScopeRequired, bool isShell,
                                      jvalue* arguments){
    if(!object)
        return;
    QTJAMBI_JNI_LOCAL_FRAME(env, 64);
    jclass objectClass = env->GetObjectClass(object);
    jobject nativeLink = QtJambiLink::getNativeLink(env, object);
    QMetaType metaType(registeredMetaTypeID(typeId));
    Q_ASSERT(metaType.isValid());
    if(env->IsSameObject(objectClass, callingClass)){
        if(isShell){
            size_t offset = size;
            size += sizeof(QtJambiShell*);
            void* ptr = operator new(size);
            memset(ptr, 0, size);
            try{
                JavaException ocurredException;
                SingleTypeShell* shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, true, isShell, nullptr, ocurredException, returnScopeRequired)
                                                                 : new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, true, isShell, nullptr, ocurredException);
                if(ocurredException){
                    delete shell;
                    ocurredException.raise();
                    return;
                }
                *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                try{
                    constructorFunction(ptr, env, object, arguments, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), false);
                }catch(...){
                    if(QSharedPointer<QtJambiLink> link = shell->link()){
                        link->invalidate(env);
                    }
                    delete shell;
                    throw;
                }
                shell->init(env);
            }catch(...){
                operator delete(ptr);
                throw;
            }
        }else{
            void* ptr = operator new(size);
            memset(ptr, 0, size);
            try{
                JavaException ocurredException;
                const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewObject(env, objectClass, nativeLink, object, typeId, ptr, nullptr, metaType,
                                                                                              true, isShell, ocurredException);
                if(ocurredException){
                    ocurredException.raise();
                    return;
                }else{
                    try{
                        constructorFunction(ptr, env, object, arguments, false, false, false);
                    }catch(...){
                        if(link){
                            link->invalidate(env);
                        }
                        throw;
                    }
                    if(link){
                        link->init(env);
                    }
                }
            }catch(...){
                operator delete(ptr);
                throw;
            }
        }
    }else{
        const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, objectClass);
        switch(superTypeInfos.size()){
        case 0:
            JavaException::raiseError(env, "Cannot determine type information about object's class." QTJAMBI_STACKTRACEINFO );
            break;
        case 1:
            Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
            if(isShell){
                size_t offset = size;
                size += sizeof(QtJambiShell*);
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    SingleTypeShell* shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, metaType, true, isShell, &superTypeInfos, ocurredException, returnScopeRequired)
                                                                     : new SingleTypeShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, metaType, true, isShell, &superTypeInfos, ocurredException);
                    if(ocurredException){
                        delete shell;
                        ocurredException.raise();
                        return;
                    }
                    *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                    try{
                        constructorFunction(ptr, env, object, arguments, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), false);
                    }catch(...){
                        if(QSharedPointer<QtJambiLink> link = shell->link()){
                            link->invalidate(env);
                        }
                        delete shell;
                        throw;
                    }
                    shell->init(env);
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }else{
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewObject(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, &superTypeInfos, metaType,
                                                                                                  true, isShell, ocurredException);
                    if(ocurredException){
                        ocurredException.raise();
                        return;
                    }else{
                        try{
                            constructorFunction(ptr, env, object, arguments, false, false, false);
                        }catch(...){
                            if(link){
                                link->invalidate(env);
                            }
                            throw;
                        }
                        if(link){
                            link->init(env);
                        }
                    }
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }
            break;
        default:
            Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
            if(isShell || isInterface(typeId)){
                size_t totalSize = 0;
                totalSize += size;
                totalSize += sizeof(QtJambiShell*);

                QList<ConstructorFunction> constructorFunctions;
                constructorFunctions << constructorFunction;

                for(int i=1; i<superTypeInfos.size(); ++i){
                    const SuperTypeInfo& info = superTypeInfos[i];
                    if(info.constructorInfos().isEmpty()){
                        jthrowable t = Java::QtJambi::QInterfaceCannotBeSubclassedException::newInstance(env, info.javaClass());
                        JavaException(env, t).raise();
                        return;
                    }else{
                        totalSize += info.size();
                        totalSize += sizeof(QtJambiShell*);
                        ConstructorFunction foundConstructorFunction = nullptr;
                        for(int j=0; j<info.constructorInfos().size(); ++j){
                            const ResolvedConstructorInfo & constructorInfo = info.constructorInfos().at(j);
                            if(constructorInfo.argumentTypes.isEmpty()){
                                foundConstructorFunction = constructorInfo.constructorFunction;
                                break;
                            }
                        }
                        if(!foundConstructorFunction){
                            Java::Runtime::Error::throwNew(env, QStringLiteral("Cannot initialize interface %1 without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.").arg(QString(info.className()).replace("/", ".").replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                        }
                        constructorFunctions << foundConstructorFunction;
                    }
                }

                void* ptr = operator new(totalSize);
                memset(ptr, 0, totalSize);
                try{
                    QMap<size_t,QVector<QtJambiScope*>> returnScopeMap;
                    QHash<size_t,DestructorInfo> destructorInfo;
                    returnScopeRequired = 0;
                    for(const SuperTypeInfo& info : superTypeInfos){
                        void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                        destructorInfo[unique_id(info.typeId())] = DestructorInfo(iptr, info.destructor(), info.typeId(), info.ownerFunction());
                        auto rs = returnScopes(info.typeId());
                        returnScopeRequired += rs;
                        if(rs>0)
                            returnScopeMap[unique_id(info.typeId())].resize(rs);
                    }
                    JavaException ocurredException;
                    MultiTypeShell* shell = returnScopeRequired>0 ? new MultiTypeRSShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, metaType, destructorInfo, true, isShell, &superTypeInfos, ocurredException, std::move(returnScopeMap))
                                                                    : new MultiTypeShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, metaType, destructorInfo, true, isShell, &superTypeInfos, ocurredException);
                    if(ocurredException){
                        delete shell;
                        ocurredException.raise();
                        return;
                    }
                    assignShell(superTypeInfos, quintptr(ptr), shell);
                    auto i=superTypeInfos.size()-1;
                    try{
                        for(; i>=0; --i){
                            const SuperTypeInfo& info = superTypeInfos[i];
                            jvalue* args = i==0 ? arguments : nullptr;
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            constructorFunctions.at(i)(iptr, env, object, args, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), constructorFunction!=constructorFunctions.at(i));
                            if(!info.hasShell())
                                shell->constructed(info.typeId());
                        }
                    }catch(const JavaException& exn){
                        for(auto j=i+1; j<superTypeInfos.size(); ++j){
                            const SuperTypeInfo& info = superTypeInfos[j];
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            QTJAMBI_TRY{
                                info.destructor()(iptr);
                            }QTJAMBI_CATCH(const JavaException& exn){
                                exn.addSuppressed(env, exn);
                            }QTJAMBI_TRY_END
                            if(!info.hasShell())
                                shell->destructed(info.typeId());
                        }
                        if(QSharedPointer<QtJambiLink> link = shell->link()){
                            link->invalidate(env);
                        }
                        delete shell;
                        exn.raise();
                        return;
                    }
                    shell->init(env);
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }else{
                Java::Runtime::Error::throwNew(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg(QtJambiAPI::getClassName(env, callingClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
            break;
        }
    }
}

void QtJambiShell::initialize(JNIEnv *env, jclass callingClass, jobject object,
                                      ConstructorFunction constructorFunction, size_t size, const std::type_info& typeId, uint returnScopeRequired, bool isShell,
                                      AbstractContainerAccess* containerAccess,
                                      jvalue* arguments, QtJambiNativeID owner){
    if(!object)
        return;
    QTJAMBI_JNI_LOCAL_FRAME(env, 64);
    jclass objectClass = env->GetObjectClass(object);
    jobject nativeLink = QtJambiLink::getNativeLink(env, object);
#if defined(QTJAMBI_GENERIC_ACCESS)
    if(AbstractListAccess* _containerAccess = dynamic_cast<AbstractListAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
    }else if(AbstractSetAccess* _containerAccess = dynamic_cast<AbstractSetAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    }else if(AbstractLinkedListAccess* _containerAccess = dynamic_cast<AbstractLinkedListAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
    }else if(AbstractVectorAccess* _containerAccess = dynamic_cast<AbstractVectorAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
#endif
    }else if(AbstractHashAccess* _containerAccess = dynamic_cast<AbstractHashAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
    }else if(AbstractMapAccess* _containerAccess = dynamic_cast<AbstractMapAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
    }else if(AbstractMultiHashAccess* _containerAccess = dynamic_cast<AbstractMultiHashAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
    }else if(AbstractMultiMapAccess* _containerAccess = dynamic_cast<AbstractMultiMapAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
    }
#endif //defined(QTJAMBI_GENERIC_ACCESS)
    Q_ASSERT(containerAccess);

    if(env->IsSameObject(objectClass, callingClass)){
        if(isShell){
            size_t offset = size;
            size += sizeof(QtJambiShell*);
            void* ptr = operator new(size);
            memset(ptr, 0, size);
            try{
                JavaException ocurredException;
                SingleTypeShell* shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr,
                                                                                       true, isShell, nullptr, containerAccess, ocurredException, returnScopeRequired)
                                                                 : new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr,
                                                                                       true, isShell, nullptr, containerAccess, ocurredException);
                if(ocurredException){
                    delete shell;
                    ocurredException.raise();
                    return;
                }
                *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                try{
                    constructorFunction(ptr, env, object, arguments, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), false);
                }catch(...){
                    if(QSharedPointer<QtJambiLink> link = shell->link()){
                        link->invalidate(env);
                    }
                    delete shell;
                    throw;
                }
                shell->init(env);
                if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
                    _owner->registerDependentObject(shell->link());
                }
            }catch(...){
                operator delete(ptr);
                throw;
            }
        }else{
            void* ptr = operator new(size);
            memset(ptr, 0, size);
            try{
                JavaException ocurredException;
                const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewObject(env, objectClass, nativeLink, object, typeId, ptr, nullptr,
                                                                                              true, isShell, containerAccess, ocurredException);
                if(ocurredException){
                    ocurredException.raise();
                }else{
                    try{
                        constructorFunction(ptr, env, object, arguments, false, false, false);
                    }catch(...){
                        if(link){
                            link->invalidate(env);
                        }
                        throw;
                    }
                    if(link){
                        link->init(env);
                        if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
                            _owner->registerDependentObject(link);
                        }
                    }
                }
            }catch(...){
                operator delete(ptr);
                throw;
            }
        }
    }else{
        const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, objectClass);
        switch(superTypeInfos.size()){
        case 0:
            JavaException::raiseError(env, "Cannot determine type information about object's class." QTJAMBI_STACKTRACEINFO );
            break;
        case 1:
            Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
            if(isShell){
                size_t offset = size;
                size += sizeof(QtJambiShell*);
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    SingleTypeShell* shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr,
                                                                                           true, isShell, &superTypeInfos, containerAccess, ocurredException, returnScopeRequired)
                                                                     : new SingleTypeShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr,
                                                                                           true, isShell, &superTypeInfos, containerAccess, ocurredException);
                    if(ocurredException){
                        delete shell;
                        ocurredException.raise();
                        return;
                    }
                    *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                    try{
                        constructorFunction(ptr, env, object, arguments, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), false);
                    }catch(...){
                        if(QSharedPointer<QtJambiLink> link = shell->link()){
                            link->invalidate(env);
                        }
                        delete shell;
                        throw;
                    }
                    shell->init(env);
                    if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
                        _owner->registerDependentObject(shell->link());
                    }
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }else{
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewObject(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, &superTypeInfos,
                                                                                                  true, isShell, containerAccess, ocurredException);
                    if(ocurredException){
                        ocurredException.raise();
                    }else{
                        try{
                            constructorFunction(ptr, env, object, arguments, false, false, false);
                        }catch(...){
                            if(link){
                                link->invalidate(env);
                            }
                            throw;
                        }
                        if(link){
                            link->init(env);
                            if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
                                _owner->registerDependentObject(link);
                            }
                        }
                    }
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }
            break;
        default:
            Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
            if(isShell || isInterface(typeId)){
                size_t totalSize = 0;
                totalSize += size;
                totalSize += sizeof(QtJambiShell*);

                QList<ConstructorFunction> constructorFunctions;
                constructorFunctions << constructorFunction;

                for(int i=1; i<superTypeInfos.size(); ++i){
                    const SuperTypeInfo& info = superTypeInfos[i];
                    if(info.constructorInfos().isEmpty()){
                        jthrowable t = Java::QtJambi::QInterfaceCannotBeSubclassedException::newInstance(env, info.javaClass());
                        JavaException(env, t).raise();
                        return;
                    }else{
                        totalSize += info.size();
                        totalSize += sizeof(QtJambiShell*);
                        ConstructorFunction foundConstructorFunction = nullptr;
                        for(int j=0; j<info.constructorInfos().size(); ++j){
                            const ResolvedConstructorInfo & constructorInfo = info.constructorInfos().at(j);
                            if(constructorInfo.argumentTypes.isEmpty()){
                                foundConstructorFunction = constructorInfo.constructorFunction;
                                break;
                            }
                        }
                        if(!foundConstructorFunction){
                            Java::Runtime::Error::throwNew(env, QStringLiteral("Cannot initialize interface %1 without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.").arg(QString(info.className()).replace("/", ".").replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                        }
                        constructorFunctions << foundConstructorFunction;
                    }
                }

                void* ptr = operator new(totalSize);
                memset(ptr, 0, totalSize);
                try{
                    QMap<size_t,QVector<QtJambiScope*>> returnScopeMap;
                    QHash<size_t,DestructorInfo> destructorInfo;
                    returnScopeRequired = 0;
                    for(const SuperTypeInfo& info : superTypeInfos){
                        void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                        destructorInfo[unique_id(info.typeId())] = DestructorInfo(iptr, info.destructor(), info.typeId(), info.ownerFunction());
                        auto rs = returnScopes(info.typeId());
                        returnScopeRequired += rs;
                        if(rs>0)
                            returnScopeMap[unique_id(info.typeId())].resize(rs);
                    }
                    JavaException ocurredException;
                    MultiTypeShell* shell = returnScopeRequired>0 ? new MultiTypeRSShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr,
                                                                                         destructorInfo, true, isShell, &superTypeInfos, containerAccess, ocurredException, std::move(returnScopeMap))
                                                                    : new MultiTypeShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr,
                                                                                         destructorInfo, true, isShell, &superTypeInfos, containerAccess, ocurredException);
                    if(ocurredException){
                        delete shell;
                        ocurredException.raise();
                        return;
                    }
                    assignShell(superTypeInfos, quintptr(ptr), shell);
                    auto i=superTypeInfos.size()-1;
                    try{
                        for(; i>=0; --i){
                            const SuperTypeInfo& info = superTypeInfos[i];
                            jvalue* args = i==0 ? arguments : nullptr;
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            constructorFunctions.at(i)(iptr, env, object, args, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), constructorFunction!=constructorFunctions.at(i));
                            if(!info.hasShell())
                                shell->constructed(info.typeId());
                        }
                    }catch(const JavaException& exn){
                        for(auto j=i+1; j<superTypeInfos.size(); ++j){
                            const SuperTypeInfo& info = superTypeInfos[j];
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            QTJAMBI_TRY{
                                info.destructor()(iptr);
                            }QTJAMBI_CATCH(const JavaException& exn){
                                exn.addSuppressed(env, exn);
                            }QTJAMBI_TRY_END
                            if(!info.hasShell())
                                shell->destructed(info.typeId());
                        }
                        if(QSharedPointer<QtJambiLink> link = shell->link()){
                            link->invalidate(env);
                        }
                        delete shell;
                        exn.raise();
                        return;
                    }
                    shell->init(env);
                    if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
                        _owner->registerDependentObject(shell->link());
                    }
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }else{
                Java::Runtime::Error::throwNew(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg(QtJambiAPI::getClassName(env, callingClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
            break;
        }
    }
}

void QtJambiShell::initialize(JNIEnv *env, jclass callingClass, jobject object,
                                      ConstructorFunction constructorFunction, size_t size, const std::type_info& typeId, uint returnScopeRequired, bool isShell,
                                      AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function,
                                      jvalue* arguments, QtJambiNativeID owner){
    if(!object)
        return;
    QTJAMBI_JNI_LOCAL_FRAME(env, 64);
    jclass objectClass = env->GetObjectClass(object);
    jobject nativeLink = QtJambiLink::getNativeLink(env, object);
#if defined(QTJAMBI_GENERIC_ACCESS)
    if(AbstractListAccess* _containerAccess = dynamic_cast<AbstractListAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
    }else if(AbstractSetAccess* _containerAccess = dynamic_cast<AbstractSetAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    }else if(AbstractLinkedListAccess* _containerAccess = dynamic_cast<AbstractLinkedListAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
    }else if(AbstractVectorAccess* _containerAccess = dynamic_cast<AbstractVectorAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
#endif
    }else if(AbstractHashAccess* _containerAccess = dynamic_cast<AbstractHashAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
    }else if(AbstractMapAccess* _containerAccess = dynamic_cast<AbstractMapAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
    }else if(AbstractMultiHashAccess* _containerAccess = dynamic_cast<AbstractMultiHashAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
    }else if(AbstractMultiMapAccess* _containerAccess = dynamic_cast<AbstractMultiMapAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
    }
#endif //defined(QTJAMBI_GENERIC_ACCESS)
    Q_ASSERT(containerAccess);

    if(env->IsSameObject(objectClass, callingClass)){
        if(isShell){
            size_t offset = size;
            size += sizeof(QtJambiShell*);
            void* ptr = operator new(size);
            memset(ptr, 0, size);
            try{
                JavaException ocurredException;
                SingleTypeShell* shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, true, isShell, nullptr, containerAccess, destructor_function, ocurredException, returnScopeRequired)
                                                                 : new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, isShell, nullptr, containerAccess, destructor_function, ocurredException);
                if(ocurredException){
                    delete shell;
                    ocurredException.raise();
                    return;
                }
                *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                try{
                    constructorFunction(ptr, env, object, arguments, false, false, false);
                }catch(...){
                    if(QSharedPointer<QtJambiLink> link = shell->link()){
                        link->invalidate(env);
                    }
                    delete shell;
                    throw;
                }
                shell->init(env);
                if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
                    _owner->registerDependentObject(shell->link());
                }
            }catch(...){
                operator delete(ptr);
                throw;
            }
        }else{
            void* ptr = operator new(size);
            memset(ptr, 0, size);
            try{
                JavaException ocurredException;
                const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewObject(env, objectClass, nativeLink, object, typeId, ptr, nullptr,
                                                                                              true, isShell, containerAccess, destructor_function, ocurredException);
                if(ocurredException){
                    ocurredException.raise();
                }else{
                    try{
                        constructorFunction(ptr, env, object, arguments, false, false, false);
                    }catch(...){
                        if(link){
                            link->invalidate(env);
                        }
                        throw;
                    }
                    if(link){
                        link->init(env);
                        if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
                            _owner->registerDependentObject(link);
                        }
                    }
                }
            }catch(...){
                operator delete(ptr);
                throw;
            }
        }
    }else{
        const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, objectClass);
        switch(superTypeInfos.size()){
        case 0:
            JavaException::raiseError(env, "Cannot determine type information about object's class." QTJAMBI_STACKTRACEINFO );
            break;
        case 1:
            Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
            if(isShell){
                size_t offset = size;
                size += sizeof(QtJambiShell*);
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    SingleTypeShell* shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, true, isShell, &superTypeInfos, containerAccess, destructor_function, ocurredException, returnScopeRequired)
                                                                     : new SingleTypeShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, true, isShell, &superTypeInfos, containerAccess, destructor_function, ocurredException);
                    if(ocurredException){
                        delete shell;
                        ocurredException.raise();
                        return;
                    }
                    *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                    try{
                        constructorFunction(ptr, env, object, arguments, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), false);
                    }catch(...){
                        if(QSharedPointer<QtJambiLink> link = shell->link()){
                            link->invalidate(env);
                        }
                        delete shell;
                        throw;
                    }
                    shell->init(env);
                    if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
                        _owner->registerDependentObject(shell->link());
                    }
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }else{
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewObject(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, &superTypeInfos,
                                                                                                  true, isShell, containerAccess, destructor_function, ocurredException);
                    if(ocurredException){
                        ocurredException.raise();
                    }else{
                        try{
                            constructorFunction(ptr, env, object, arguments, false, false, false);
                        }catch(...){
                            if(link){
                                link->invalidate(env);
                            }
                            throw;
                        }
                        if(link){
                            link->init(env);
                            if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
                                _owner->registerDependentObject(link);
                            }
                        }
                    }
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }
            break;
        default:
            Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
            if(isShell || isInterface(typeId)){
                size_t totalSize = 0;
                totalSize += size;
                totalSize += sizeof(QtJambiShell*);

                QList<ConstructorFunction> constructorFunctions;
                constructorFunctions << constructorFunction;

                for(int i=1; i<superTypeInfos.size(); ++i){
                    const SuperTypeInfo& info = superTypeInfos[i];
                    if(info.constructorInfos().isEmpty()){
                        jthrowable t = Java::QtJambi::QInterfaceCannotBeSubclassedException::newInstance(env, info.javaClass());
                        JavaException(env, t).raise();
                        return;
                    }else{
                        totalSize += info.size();
                        totalSize += sizeof(QtJambiShell*);
                        ConstructorFunction foundConstructorFunction = nullptr;
                        for(int j=0; j<info.constructorInfos().size(); ++j){
                            const ResolvedConstructorInfo & constructorInfo = info.constructorInfos().at(j);
                            if(constructorInfo.argumentTypes.isEmpty()){
                                foundConstructorFunction = constructorInfo.constructorFunction;
                                break;
                            }
                        }
                        if(!foundConstructorFunction){
                            Java::Runtime::Error::throwNew(env, QStringLiteral("Cannot initialize interface %1 without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.").arg(QString(info.className()).replace("/", ".").replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                        }
                        constructorFunctions << foundConstructorFunction;
                    }
                }

                void* ptr = operator new(totalSize);
                memset(ptr, 0, totalSize);
                try{
                    QMap<size_t,QVector<QtJambiScope*>> returnScopeMap;
                    QHash<size_t,DestructorInfo> destructorInfo;
                    returnScopeRequired = 0;
                    for(const SuperTypeInfo& info : superTypeInfos){
                        void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                        destructorInfo[unique_id(info.typeId())] = DestructorInfo(iptr, info.destructor(), info.typeId(), info.ownerFunction());
                        auto rs = returnScopes(info.typeId());
                        returnScopeRequired += rs;
                        if(rs>0)
                            returnScopeMap[unique_id(info.typeId())].resize(rs);
                    }
                    JavaException ocurredException;
                    MultiTypeShell* shell = returnScopeRequired>0 ? new MultiTypeRSShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, destructorInfo, true, isShell, &superTypeInfos, containerAccess, destructor_function, ocurredException, std::move(returnScopeMap))
                                                                    : new MultiTypeShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, destructorInfo, true, isShell, &superTypeInfos, containerAccess, destructor_function, ocurredException);
                    if(ocurredException){
                        delete shell;
                        ocurredException.raise();
                        return;
                    }
                    assignShell(superTypeInfos, quintptr(ptr), shell);
                    auto i=superTypeInfos.size()-1;
                    try{
                        for(; i>=0; --i){
                            const SuperTypeInfo& info = superTypeInfos[i];
                            jvalue* args = i==0 ? arguments : nullptr;
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            constructorFunctions.at(i)(iptr, env, object, args, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), constructorFunction!=constructorFunctions.at(i));
                            if(!info.hasShell())
                                shell->constructed(info.typeId());
                        }
                    }catch(const JavaException& exn){
                        for(auto j=i+1; j<superTypeInfos.size(); ++j){
                            const SuperTypeInfo& info = superTypeInfos[j];
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            QTJAMBI_TRY{
                                info.destructor()(iptr);
                            }QTJAMBI_CATCH(const JavaException& exn){
                                exn.addSuppressed(env, exn);
                            }QTJAMBI_TRY_END
                            if(!info.hasShell())
                                shell->destructed(info.typeId());
                        }
                        if(QSharedPointer<QtJambiLink> link = shell->link()){
                            link->invalidate(env);
                        }
                        delete shell;
                        exn.raise();
                        return;
                    }
                    shell->init(env);
                    if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
                        _owner->registerDependentObject(shell->link());
                    }
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }else{
                Java::Runtime::Error::throwNew(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg(QtJambiAPI::getClassName(env, callingClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
            break;
        }
    }
}

void QtJambiShell::initialize(JNIEnv *env, jclass callingClass, jobject object,
                                      ConstructorFunction constructorFunction, size_t size, const std::type_info& typeId, uint returnScopeRequired, bool isShell,
                                      AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction,
                                      jvalue* arguments, QtJambiNativeID owner){
    if(!object)
        return;
    QTJAMBI_JNI_LOCAL_FRAME(env, 64);
    jclass objectClass = env->GetObjectClass(object);
    jobject nativeLink = QtJambiLink::getNativeLink(env, object);
#if defined(QTJAMBI_GENERIC_ACCESS)
    if(AbstractListAccess* _containerAccess = dynamic_cast<AbstractListAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
    }else if(AbstractSetAccess* _containerAccess = dynamic_cast<AbstractSetAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    }else if(AbstractLinkedListAccess* _containerAccess = dynamic_cast<AbstractLinkedListAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
    }else if(AbstractVectorAccess* _containerAccess = dynamic_cast<AbstractVectorAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
#endif
    }else if(AbstractHashAccess* _containerAccess = dynamic_cast<AbstractHashAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
    }else if(AbstractMapAccess* _containerAccess = dynamic_cast<AbstractMapAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
    }else if(AbstractMultiHashAccess* _containerAccess = dynamic_cast<AbstractMultiHashAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
    }else if(AbstractMultiMapAccess* _containerAccess = dynamic_cast<AbstractMultiMapAccess*>(containerAccess)){
        containerAccess = checkContainerAccess(env, _containerAccess);
    }
#endif //defined(QTJAMBI_GENERIC_ACCESS)
    Q_ASSERT(containerAccess);

    if(env->IsSameObject(objectClass, callingClass)){
        if(isShell){
            size_t offset = size;
            size += sizeof(QtJambiShell*);
            void* ptr = operator new(size);
            memset(ptr, 0, size);
            try{
                JavaException ocurredException;
                SingleTypeShell* shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, true, isShell, nullptr, containerAccess, destructor_function, ownerFunction, ocurredException, returnScopeRequired)
                                                                 : new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, isShell, nullptr, containerAccess, destructor_function, ownerFunction, ocurredException);
                if(ocurredException){
                    delete shell;
                    ocurredException.raise();
                    return;
                }
                *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                try{
                    constructorFunction(ptr, env, object, arguments, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), false);
                }catch(...){
                    if(QSharedPointer<QtJambiLink> link = shell->link()){
                        link->invalidate(env);
                    }
                    delete shell;
                    throw;
                }
                shell->init(env);
                if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
                    _owner->registerDependentObject(shell->link());
                }
            }catch(...){
                operator delete(ptr);
                throw;
            }
        }else{
            void* ptr = operator new(size);
            memset(ptr, 0, size);
            try{
                JavaException ocurredException;
                const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewObject(env, objectClass, nativeLink, object, typeId, ptr, nullptr,
                                                                                              true, isShell, containerAccess, destructor_function, ownerFunction, ocurredException);
                if(ocurredException){
                    ocurredException.raise();
                }else{
                    try{
                        constructorFunction(ptr, env, object, arguments, false, false, false);
                    }catch(...){
                        if(link){
                            link->invalidate(env);
                        }
                        throw;
                    }
                    if(link){
                        link->init(env);
                        if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
                            _owner->registerDependentObject(link);
                        }
                    }
                }
            }catch(...){
                operator delete(ptr);
                throw;
            }
        }
    }else{
        const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, objectClass);
        switch(superTypeInfos.size()){
        case 0:
            JavaException::raiseError(env, "Cannot determine type information about object's class." QTJAMBI_STACKTRACEINFO );
            break;
        case 1:
            Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
            if(isShell){
                size_t offset = size;
                size += sizeof(QtJambiShell*);
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    SingleTypeShell* shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, true, isShell, &superTypeInfos, containerAccess, destructor_function, ownerFunction, ocurredException, returnScopeRequired)
                                                                     : new SingleTypeShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, true, isShell, &superTypeInfos, containerAccess, destructor_function, ownerFunction, ocurredException);
                    if(ocurredException){
                        delete shell;
                        ocurredException.raise();
                        return;
                    }
                    *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                    try{
                        constructorFunction(ptr, env, object, arguments, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), false);
                    }catch(...){
                        if(QSharedPointer<QtJambiLink> link = shell->link()){
                            link->invalidate(env);
                        }
                        delete shell;
                        throw;
                    }
                    shell->init(env);
                    if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
                        _owner->registerDependentObject(shell->link());
                    }
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }else{
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewObject(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, &superTypeInfos,
                                                                                                  true, isShell, containerAccess, destructor_function, ownerFunction, ocurredException);
                    if(ocurredException){
                        ocurredException.raise();
                    }else{
                        try{
                            constructorFunction(ptr, env, object, arguments, false, false, false);
                        }catch(...){
                            if(link){
                                link->invalidate(env);
                            }
                            throw;
                        }
                        if(link){
                            link->init(env);
                            if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
                                _owner->registerDependentObject(link);
                            }
                        }
                    }
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }
            break;
        default:
            Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
            if(isShell || isInterface(typeId)){
                size_t totalSize = 0;
                totalSize += size;
                totalSize += sizeof(QtJambiShell*);

                QList<ConstructorFunction> constructorFunctions;
                constructorFunctions << constructorFunction;

                for(int i=1; i<superTypeInfos.size(); ++i){
                    const SuperTypeInfo& info = superTypeInfos[i];
                    if(info.constructorInfos().isEmpty()){
                        jthrowable t = Java::QtJambi::QInterfaceCannotBeSubclassedException::newInstance(env, info.javaClass());
                        JavaException(env, t).raise();
                        return;
                    }else{
                        totalSize += info.size();
                        totalSize += sizeof(QtJambiShell*);
                        ConstructorFunction foundConstructorFunction = nullptr;
                        for(int j=0; j<info.constructorInfos().size(); ++j){
                            const ResolvedConstructorInfo & constructorInfo = info.constructorInfos().at(j);
                            if(constructorInfo.argumentTypes.isEmpty()){
                                foundConstructorFunction = constructorInfo.constructorFunction;
                                break;
                            }
                        }
                        if(!foundConstructorFunction){
                            Java::Runtime::Error::throwNew(env, QStringLiteral("Cannot initialize interface %1 without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.").arg(QString(info.className()).replace("/", ".").replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                        }
                        constructorFunctions << foundConstructorFunction;
                    }
                }

                void* ptr = operator new(totalSize);
                memset(ptr, 0, totalSize);
                try{
                    QMap<size_t,QVector<QtJambiScope*>> returnScopeMap;
                    QHash<size_t,DestructorInfo> destructorInfo;
                    returnScopeRequired = 0;
                    for(const SuperTypeInfo& info : superTypeInfos){
                        void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                        destructorInfo[unique_id(info.typeId())] = DestructorInfo(iptr, info.destructor(), info.typeId(), info.ownerFunction());
                        auto rs = returnScopes(info.typeId());
                        returnScopeRequired += rs;
                        if(rs>0)
                            returnScopeMap[unique_id(info.typeId())].resize(rs);
                    }
                    JavaException ocurredException;
                    MultiTypeShell* shell = returnScopeRequired>0 ? new MultiTypeRSShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, destructorInfo, true, isShell, &superTypeInfos, containerAccess, destructor_function, ownerFunction, ocurredException, std::move(returnScopeMap))
                                                                    : new MultiTypeShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, destructorInfo, true, isShell, &superTypeInfos, containerAccess, destructor_function, ownerFunction, ocurredException);
                    if(ocurredException){
                        delete shell;
                        ocurredException.raise();
                        return;
                    }
                    assignShell(superTypeInfos, quintptr(ptr), shell);
                    auto i=superTypeInfos.size()-1;
                    try{
                        for(; i>=0; --i){
                            const SuperTypeInfo& info = superTypeInfos[i];
                            jvalue* args = i==0 ? arguments : nullptr;
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            constructorFunctions.at(i)(iptr, env, object, args, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), constructorFunction!=constructorFunctions.at(i));
                            if(!info.hasShell())
                                shell->constructed(info.typeId());
                        }
                    }catch(const JavaException& exn){
                        for(auto j=i+1; j<superTypeInfos.size(); ++j){
                            const SuperTypeInfo& info = superTypeInfos[j];
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            QTJAMBI_TRY{
                                info.destructor()(iptr);
                            }QTJAMBI_CATCH(const JavaException& exn){
                                exn.addSuppressed(env, exn);
                            }QTJAMBI_TRY_END
                            if(!info.hasShell())
                                shell->destructed(info.typeId());
                        }
                        if(QSharedPointer<QtJambiLink> link = shell->link()){
                            link->invalidate(env);
                        }
                        delete shell;
                        exn.raise();
                        return;
                    }
                    shell->init(env);
                    if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
                        _owner->registerDependentObject(shell->link());
                    }
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }else{
                Java::Runtime::Error::throwNew(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg(QtJambiAPI::getClassName(env, callingClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
            break;
        }
    }
}

void QtJambiShell::initialize(JNIEnv *env, jclass callingClass, jobject object,
                                      ConstructorFunction constructorFunction, size_t size, const std::type_info& typeId, uint returnScopeRequired, bool isShell,
                                      PtrDeleterFunction deleter_function, PtrOwnerFunction ownerFunction, jvalue* arguments){
    if(!object)
        return;
    QTJAMBI_JNI_LOCAL_FRAME(env, 64);
    jclass objectClass = env->GetObjectClass(object);
    jobject nativeLink = QtJambiLink::getNativeLink(env, object);
    if(env->IsSameObject(objectClass, callingClass)){
        if(isShell){
            size_t offset = size;
            size += sizeof(QtJambiShell*);
            void* ptr = operator new(size);
            memset(ptr, 0, size);
            try{
                JavaException ocurredException;
                SingleTypeShell* shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, true, isShell, nullptr, deleter_function, ownerFunction, ocurredException, returnScopeRequired)
                                                                 : new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, isShell, nullptr, deleter_function, ownerFunction, ocurredException);
                if(ocurredException){
                    delete shell;
                    ocurredException.raise();
                    return;
                }
                *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                try{
                    constructorFunction(ptr, env, object, arguments, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), false);
                }catch(...){
                    if(QSharedPointer<QtJambiLink> link = shell->link()){
                        link->invalidate(env);
                    }
                    delete shell;
                    throw;
                }
                shell->init(env);
            }catch(...){
                operator delete(ptr);
                throw;
            }
        }else{
            void* ptr = operator new(size);
            memset(ptr, 0, size);
            try{
                JavaException ocurredException;
                const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewObject(env, objectClass, nativeLink, object, typeId, ptr, nullptr,
                                                                                              true, isShell, deleter_function, ownerFunction, ocurredException);
                if(ocurredException){
                    ocurredException.raise();
                }else{
                    try{
                        constructorFunction(ptr, env, object, arguments, false, false, false);
                    }catch(...){
                        if(link){
                            link->invalidate(env);
                        }
                        throw;
                    }
                    if(link){
                        link->init(env);
                    }
                }
            }catch(...){
                operator delete(ptr);
                throw;
            }
        }
    }else{
        const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, objectClass);
        switch(superTypeInfos.size()){
        case 0:
            JavaException::raiseError(env, "Cannot determine type information about object's class." QTJAMBI_STACKTRACEINFO );
            break;
        case 1:
            Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
            if(isShell){
                size_t offset = size;
                size += sizeof(QtJambiShell*);
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    SingleTypeShell* shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, true, isShell, &superTypeInfos, deleter_function, ownerFunction, ocurredException, returnScopeRequired)
                                                                     : new SingleTypeShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, true, isShell, &superTypeInfos, deleter_function, ownerFunction, ocurredException);
                    if(ocurredException){
                        delete shell;
                        ocurredException.raise();
                        return;
                    }
                    *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                    try{
                        constructorFunction(ptr, env, object, arguments, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), false);
                    }catch(...){
                        if(QSharedPointer<QtJambiLink> link = shell->link()){
                            link->invalidate(env);
                        }
                        delete shell;
                        throw;
                    }
                    shell->init(env);
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }else{
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewObject(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, &superTypeInfos,
                                                                                                  true, isShell, deleter_function, ownerFunction, ocurredException);
                    if(ocurredException){
                        ocurredException.raise();
                    }else{
                        try{
                            constructorFunction(ptr, env, object, arguments, false, false, false);
                        }catch(...){
                            if(link){
                                link->invalidate(env);
                            }
                            throw;
                        }
                        if(link){
                            link->init(env);
                        }
                    }
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }
            break;
        default:
            Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
            if(isShell || isInterface(typeId)){
                size_t totalSize = 0;
                totalSize += size;
                totalSize += sizeof(QtJambiShell*);

                QList<ConstructorFunction> constructorFunctions;
                constructorFunctions << constructorFunction;

                for(int i=1; i<superTypeInfos.size(); ++i){
                    const SuperTypeInfo& info = superTypeInfos[i];
                    if(info.constructorInfos().isEmpty()){
                        jthrowable t = Java::QtJambi::QInterfaceCannotBeSubclassedException::newInstance(env, info.javaClass());
                        JavaException(env, t).raise();
                        return;
                    }else{
                        totalSize += info.size();
                        totalSize += sizeof(QtJambiShell*);
                        ConstructorFunction foundConstructorFunction = nullptr;
                        for(int j=0; j<info.constructorInfos().size(); ++j){
                            const ResolvedConstructorInfo & constructorInfo = info.constructorInfos().at(j);
                            if(constructorInfo.argumentTypes.isEmpty()){
                                foundConstructorFunction = constructorInfo.constructorFunction;
                                break;
                            }
                        }
                        if(!foundConstructorFunction){
                            Java::Runtime::Error::throwNew(env, QStringLiteral("Cannot initialize interface %1 without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.").arg(QString(info.className()).replace("/", ".").replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                        }
                        constructorFunctions << foundConstructorFunction;
                    }
                }

                void* ptr = operator new(totalSize);
                memset(ptr, 0, totalSize);
                try{
                    QMap<size_t,QVector<QtJambiScope*>> returnScopeMap;
                    QHash<size_t,DestructorInfo> destructorInfo;
                    returnScopeRequired = 0;
                    for(const SuperTypeInfo& info : superTypeInfos){
                        void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                        destructorInfo[unique_id(info.typeId())] = DestructorInfo(iptr, info.destructor(), info.typeId(), info.ownerFunction());
                        auto rs = returnScopes(info.typeId());
                        returnScopeRequired += rs;
                        if(rs>0)
                            returnScopeMap[unique_id(info.typeId())].resize(rs);
                    }
                    JavaException ocurredException;
                    MultiTypeShell* shell = returnScopeRequired>0 ? new MultiTypeRSShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, destructorInfo, true, isShell, &superTypeInfos, deleter_function, ownerFunction, ocurredException, std::move(returnScopeMap))
                                                                    : new MultiTypeShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, destructorInfo, true, isShell, &superTypeInfos, deleter_function, ownerFunction, ocurredException);
                    if(ocurredException){
                        delete shell;
                        ocurredException.raise();
                        return;
                    }
                    assignShell(superTypeInfos, quintptr(ptr), shell);
                    auto i=superTypeInfos.size()-1;
                    try{
                        for(; i>=0; --i){
                            const SuperTypeInfo& info = superTypeInfos[i];
                            jvalue* args = i==0 ? arguments : nullptr;
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            constructorFunctions.at(i)(iptr, env, object, args, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), constructorFunction!=constructorFunctions.at(i));
                            if(!info.hasShell())
                                shell->constructed(info.typeId());
                        }
                    }catch(const JavaException& exn){
                        for(auto j=i+1; j<superTypeInfos.size(); ++j){
                            const SuperTypeInfo& info = superTypeInfos[j];
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            QTJAMBI_TRY{
                                info.destructor()(iptr);
                            }QTJAMBI_CATCH(const JavaException& exn){
                                exn.addSuppressed(env, exn);
                            }QTJAMBI_TRY_END
                            if(!info.hasShell())
                                shell->destructed(info.typeId());
                        }
                        if(QSharedPointer<QtJambiLink> link = shell->link()){
                            link->invalidate(env);
                        }
                        delete shell;
                        exn.raise();
                        return;
                    }
                    shell->init(env);
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }else{
                Java::Runtime::Error::throwNew(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg(QtJambiAPI::getClassName(env, callingClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
            break;
        }
    }
}

void QtJambiShell::initialize(JNIEnv *env, jclass callingClass, jobject object,
                                      ConstructorFunction constructorFunction, size_t size, const std::type_info& typeId, uint returnScopeRequired, bool isShell,
                                      PtrDeleterFunction deleter_function, jvalue* arguments){
    if(!object)
        return;
    QTJAMBI_JNI_LOCAL_FRAME(env, 64);
    jclass objectClass = env->GetObjectClass(object);
    jobject nativeLink = QtJambiLink::getNativeLink(env, object);
    if(env->IsSameObject(objectClass, callingClass)){
        if(isShell){
            size_t offset = size;
            size += sizeof(QtJambiShell*);
            void* ptr = operator new(size);
            memset(ptr, 0, size);
            try{
                JavaException ocurredException;
                SingleTypeShell* shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, true, isShell, nullptr, deleter_function, ocurredException, returnScopeRequired)
                                                                 : new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, isShell, nullptr, deleter_function, ocurredException);
                if(ocurredException){
                    delete shell;
                    ocurredException.raise();
                    return;
                }
                *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                try{
                    constructorFunction(ptr, env, object, arguments, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), false);
                }catch(...){
                    if(QSharedPointer<QtJambiLink> link = shell->link()){
                        link->invalidate(env);
                    }
                    delete shell;
                    throw;
                }
                shell->init(env);
            }catch(...){
                operator delete(ptr);
                throw;
            }
        }else{
            void* ptr = operator new(size);
            memset(ptr, 0, size);
            try{
                JavaException ocurredException;
                const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewObject(env, objectClass, nativeLink, object, typeId, ptr, nullptr,
                                                                                              true, isShell, deleter_function, ocurredException);
                if(ocurredException){
                    ocurredException.raise();
                }else{
                    try{
                        constructorFunction(ptr, env, object, arguments, false, false, false);
                    }catch(...){
                        if(link){
                            link->invalidate(env);
                        }
                        throw;
                    }
                    if(link){
                        link->init(env);
                    }
                }
            }catch(...){
                operator delete(ptr);
                throw;
            }
        }
    }else{
        const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, objectClass);
        switch(superTypeInfos.size()){
        case 0:
            JavaException::raiseError(env, "Cannot determine type information about object's class." QTJAMBI_STACKTRACEINFO );
            break;
        case 1:
            Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
            if(isShell){
                size_t offset = size;
                size += sizeof(QtJambiShell*);
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    SingleTypeShell* shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, true, isShell, &superTypeInfos, deleter_function, ocurredException, returnScopeRequired)
                                                                     : new SingleTypeShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, true, isShell, &superTypeInfos, deleter_function, ocurredException);
                    if(ocurredException){
                        delete shell;
                        ocurredException.raise();
                        return;
                    }
                    *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                    try{
                        constructorFunction(ptr, env, object, arguments, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), false);
                    }catch(...){
                        if(QSharedPointer<QtJambiLink> link = shell->link()){
                            link->invalidate(env);
                        }
                        delete shell;
                        throw;
                    }
                    shell->init(env);
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }else{
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewObject(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, &superTypeInfos,
                                                                                                  true, isShell, deleter_function, ocurredException);
                    if(ocurredException){
                        ocurredException.raise();
                    }else{
                        try{
                            constructorFunction(ptr, env, object, arguments, false, false, false);
                        }catch(...){
                            if(link){
                                link->invalidate(env);
                            }
                            throw;
                        }
                        if(link){
                            link->init(env);
                        }
                    }
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }
            break;
        default:
            Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
            if(isShell || isInterface(typeId)){
                size_t totalSize = 0;
                totalSize += size;
                totalSize += sizeof(QtJambiShell*);

                QList<ConstructorFunction> constructorFunctions;
                constructorFunctions << constructorFunction;

                for(int i=1; i<superTypeInfos.size(); ++i){
                    const SuperTypeInfo& info = superTypeInfos[i];
                    if(info.constructorInfos().isEmpty()){
                        jthrowable t = Java::QtJambi::QInterfaceCannotBeSubclassedException::newInstance(env, info.javaClass());
                        JavaException(env, t).raise();
                        return;
                    }else{
                        totalSize += info.size();
                        totalSize += sizeof(QtJambiShell*);
                        ConstructorFunction foundConstructorFunction = nullptr;
                        for(int j=0; j<info.constructorInfos().size(); ++j){
                            const ResolvedConstructorInfo & constructorInfo = info.constructorInfos().at(j);
                            if(constructorInfo.argumentTypes.isEmpty()){
                                foundConstructorFunction = constructorInfo.constructorFunction;
                                break;
                            }
                        }
                        if(!foundConstructorFunction){
                            Java::Runtime::Error::throwNew(env, QStringLiteral("Cannot initialize interface %1 without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.").arg(QString(info.className()).replace("/", ".").replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                        }
                        constructorFunctions << foundConstructorFunction;
                    }
                }

                void* ptr = operator new(totalSize);
                memset(ptr, 0, totalSize);
                try{
                    QMap<size_t,QVector<QtJambiScope*>> returnScopeMap;
                    QHash<size_t,DestructorInfo> destructorInfo;
                    returnScopeRequired = 0;
                    for(const SuperTypeInfo& info : superTypeInfos){
                        void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                        destructorInfo[unique_id(info.typeId())] = DestructorInfo(iptr, info.destructor(), info.typeId(), info.ownerFunction());
                        auto rs = returnScopes(info.typeId());
                        returnScopeRequired += rs;
                        if(rs>0)
                            returnScopeMap[unique_id(info.typeId())].resize(rs);
                    }
                    JavaException ocurredException;
                    MultiTypeShell* shell = returnScopeRequired>0 ? new MultiTypeRSShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, destructorInfo, true, isShell, &superTypeInfos, deleter_function, ocurredException, std::move(returnScopeMap))
                                                                    : new MultiTypeShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, destructorInfo, true, isShell, &superTypeInfos, deleter_function, ocurredException);
                    if(ocurredException){
                        delete shell;
                        ocurredException.raise();
                        return;
                    }
                    assignShell(superTypeInfos, quintptr(ptr), shell);
                    auto i=superTypeInfos.size()-1;
                    try{
                        for(; i>=0; --i){
                            const SuperTypeInfo& info = superTypeInfos[i];
                            jvalue* args = i==0 ? arguments : nullptr;
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            constructorFunctions.at(i)(iptr, env, object, args, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), constructorFunction!=constructorFunctions.at(i));
                            if(!info.hasShell())
                                shell->constructed(info.typeId());
                        }
                    }catch(const JavaException& exn){
                        for(auto j=i+1; j<superTypeInfos.size(); ++j){
                            const SuperTypeInfo& info = superTypeInfos[j];
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            QTJAMBI_TRY{
                                info.destructor()(iptr);
                            }QTJAMBI_CATCH(const JavaException& exn){
                                exn.addSuppressed(env, exn);
                            }QTJAMBI_TRY_END
                            if(!info.hasShell())
                                shell->destructed(info.typeId());
                        }
                        if(QSharedPointer<QtJambiLink> link = shell->link()){
                            link->invalidate(env);
                        }
                        delete shell;
                        exn.raise();
                        return;
                    }
                    shell->init(env);
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }else{
                Java::Runtime::Error::throwNew(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg(QtJambiAPI::getClassName(env, callingClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
            break;
        }
    }
}

void QtJambiShell::initialize(JNIEnv *env, jclass callingClass, jobject object,
                                      ConstructorFunction constructorFunction, size_t size, const std::type_info& typeId, uint returnScopeRequired, PtrDeleterFunction delete_function){
    if(!object)
        return;
    QTJAMBI_JNI_LOCAL_FRAME(env, 64);
    jclass objectClass = env->GetObjectClass(object);
    jobject nativeLink = QtJambiLink::getNativeLink(env, object);
    if(env->IsSameObject(objectClass, callingClass)){
        size_t offset = size;
        size += sizeof(QtJambiShell*);
        void* ptr = operator new(size);
        memset(ptr, 0, size);
        try{
            JavaException ocurredException;
            SingleTypeShell* shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, true, true, nullptr, delete_function, ocurredException, returnScopeRequired)
                                                             : new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, true, nullptr, delete_function, ocurredException);
            if(ocurredException){
                delete shell;
                ocurredException.raise();
                return;
            }
            *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
            try{
                constructorFunction(ptr, env, object, nullptr, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), false);
            }catch(...){
                if(QSharedPointer<QtJambiLink> link = shell->link()){
                    link->invalidate(env);
                }
                delete shell;
                throw;
            }
            shell->init(env);
        }catch(...){
            operator delete(ptr);
            throw;
        }
    }else{
        const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, objectClass);
        switch(superTypeInfos.size()){
        case 0:
            JavaException::raiseError(env, "Cannot determine type information about object's class." QTJAMBI_STACKTRACEINFO );
            break;
        case 1:{
                Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
                size_t offset = size;
                size += sizeof(QtJambiShell*);
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    SingleTypeShell* shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, true, true, &superTypeInfos, delete_function, ocurredException, returnScopeRequired)
                                                                     : new SingleTypeShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, true, true, &superTypeInfos, delete_function, ocurredException);
                    if(ocurredException){
                        delete shell;
                        ocurredException.raise();
                        return;
                    }
                    *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                    try{
                        constructorFunction(ptr, env, object, nullptr, false, false, false);
                    }catch(...){
                        if(QSharedPointer<QtJambiLink> link = shell->link()){
                            link->invalidate(env);
                        }
                        delete shell;
                        throw;
                    }
                    shell->init(env);
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }
            break;
        default:
            Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
            {
                size_t totalSize = 0;
                totalSize += size;
                totalSize += sizeof(QtJambiShell*);

                QList<ConstructorFunction> constructorFunctions;
                constructorFunctions << constructorFunction;

                for(int i=1; i<superTypeInfos.size(); ++i){
                    const SuperTypeInfo& info = superTypeInfos[i];
                    if(info.constructorInfos().isEmpty()){
                        jthrowable t = Java::QtJambi::QInterfaceCannotBeSubclassedException::newInstance(env, info.javaClass());
                        JavaException(env, t).raise();
                        return;
                    }else{
                        totalSize += info.size();
                        totalSize += sizeof(QtJambiShell*);
                        ConstructorFunction foundConstructorFunction = nullptr;
                        for(int j=0; j<info.constructorInfos().size(); ++j){
                            const ResolvedConstructorInfo & constructorInfo = info.constructorInfos().at(j);
                            if(constructorInfo.argumentTypes.isEmpty()){
                                foundConstructorFunction = constructorInfo.constructorFunction;
                                break;
                            }
                        }
                        if(!foundConstructorFunction){
                            Java::Runtime::Error::throwNew(env, QStringLiteral("Cannot initialize interface %1 without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.").arg(QString(info.className()).replace("/", ".").replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                        }
                        constructorFunctions << foundConstructorFunction;
                    }
                }

                void* ptr = operator new(totalSize);
                memset(ptr, 0, totalSize);
                try{
                    QMap<size_t,QVector<QtJambiScope*>> returnScopeMap;
                    QHash<size_t,DestructorInfo> destructorInfo;
                    returnScopeRequired = 0;
                    for(const SuperTypeInfo& info : superTypeInfos){
                        void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                        destructorInfo[unique_id(info.typeId())] = DestructorInfo(iptr, info.destructor(), info.typeId(), info.ownerFunction());
                        auto rs = returnScopes(info.typeId());
                        returnScopeRequired += rs;
                        if(rs>0)
                            returnScopeMap[unique_id(info.typeId())].resize(rs);
                    }
                    JavaException ocurredException;
                    MultiTypeShell* shell = returnScopeRequired>0 ? new MultiTypeRSShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, destructorInfo, true, true, &superTypeInfos, delete_function, ocurredException, std::move(returnScopeMap))
                                                                    : new MultiTypeShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), ptr, destructorInfo, true, true, &superTypeInfos, delete_function, ocurredException);
                    if(ocurredException){
                        delete shell;
                        ocurredException.raise();
                        return;
                    }
                    assignShell(superTypeInfos, quintptr(ptr), shell);
                    auto i=superTypeInfos.size()-1;
                    try{
                        for(; i>=0; --i){
                            const SuperTypeInfo& info = superTypeInfos[i];
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            constructorFunctions.at(i)(iptr, env, object, nullptr, QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), constructorFunction!=constructorFunctions.at(i));
                            if(!info.hasShell())
                                shell->constructed(info.typeId());
                        }
                    }catch(const JavaException& exn){
                        for(auto j=i+1; j<superTypeInfos.size(); ++j){
                            const SuperTypeInfo& info = superTypeInfos[j];
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            QTJAMBI_TRY{
                                info.destructor()(iptr);
                            }QTJAMBI_CATCH(const JavaException& exn){
                                exn.addSuppressed(env, exn);
                            }QTJAMBI_TRY_END
                            if(!info.hasShell())
                                shell->destructed(info.typeId());
                        }
                        if(QSharedPointer<QtJambiLink> link = shell->link()){
                            link->invalidate(env);
                        }
                        delete shell;
                        exn.raise();
                        return;
                    }
                    shell->init(env);
                }catch(...){
                    operator delete(ptr);
                    throw;
                }
            }
            break;
        }
    }
}

void QtJambiShell::initialize(JNIEnv *env, jclass callingClass, jobject object, ConstructorFunction constructorFunction, size_t size, const std::type_info& typeId, uint returnScopeRequired, const QMetaObject& originalMetaObject, bool isShell, bool hasCustomMetaObject, bool isDeclarativeCall, jvalue* arguments){
    try{
        if(!object)
            return;
        QTJAMBI_JNI_LOCAL_FRAME(env, 64);
        jclass objectClass = env->GetObjectClass(object);
        jobject nativeLink = QtJambiLink::getNativeLink(env, object);
        if(env->IsSameObject(objectClass, callingClass)){
            if(isShell){
                size_t offset = size;
                size += sizeof(QtJambiShell*);
                void* ptr;
                if(isDeclarativeCall){
                    Q_ASSERT(arguments);
                    ptr = QtJambiShellImpl::getPlacement(env, callingClass, arguments->l);
                }else{
                    ptr = operator new(size);
                    memset(ptr, 0, size);
                }
                try{
                    JavaException ocurredException;
                    SingleTypeShell* shell;
                    if(returnScopeRequired>0){
                        shell = new SingleTypeRSShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QObject*>(ptr), &originalMetaObject, true, isDeclarativeCall, isShell, hasCustomMetaObject, nullptr, ocurredException, returnScopeRequired);
                    }else{
                        shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QObject*>(ptr), &originalMetaObject, true, isDeclarativeCall, isShell, hasCustomMetaObject, nullptr, ocurredException);
                    }
                    if(ocurredException){
                        delete shell;
                        ocurredException.raise();
                        return;
                    }
                    *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                    QtJambiLink::registerQObjectInitialization(ptr, shell->link());
                    try{
                        constructorFunction(ptr, env, object, arguments, &originalMetaObject!=shell->metaObject(), !shell->hasEmptyVTable(), false);
                        QtJambiLink::unregisterQObjectInitialization(ptr);
                    }catch(...){
                        QtJambiLink::unregisterQObjectInitialization(ptr);
                        if(QSharedPointer<QtJambiLink> link = shell->link()){
                            link->invalidate(env);
                        }
                        delete shell;
                        throw;
                    }
                    shell->init(env);
                }catch(...){
                    if(!isDeclarativeCall){
                        operator delete(ptr);
                    }
                    throw;
                }
            }else{
                void* ptr;
                if(isDeclarativeCall){
                    Q_ASSERT(arguments);
                    ptr = QtJambiShellImpl::getPlacement(env, callingClass, arguments->l);
                }else{
                    ptr = operator new(size);
                    memset(ptr, 0, size);
                }
                try{
                    JavaException ocurredException;
                    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewQObject(env, objectClass, nativeLink, object, typeId, &originalMetaObject, reinterpret_cast<QObject*>(ptr), nullptr, true, isDeclarativeCall, isShell, hasCustomMetaObject, ocurredException);
                    if(ocurredException){
                        ocurredException.raise();
                    }else{
                        try{
                            QtJambiLink::registerQObjectInitialization(ptr, link);
                            constructorFunction(ptr, env, object, arguments, false, false, false);
                            QtJambiLink::unregisterQObjectInitialization(ptr);
                        }catch(...){
                            QtJambiLink::unregisterQObjectInitialization(ptr);
                            if(link){
                                link->invalidate(env);
                            }
                            throw;
                        }
                        if(link){
                            link->init(env);
                        }
                    }
                }catch(...){
                    if(!isDeclarativeCall){
                        operator delete(ptr);
                    }
                    throw;
                }
            }
        }else{
            if(!isShell){
                if(const QtJambiMetaObject* mo = QtJambiMetaObject::cast(CoreAPI::metaObjectForClass(env, objectClass, &originalMetaObject, false))){
                    if(mo->hasSignals()){
                        QString class_name = QtJambiAPI::getClassName(env, objectClass).replace("$", ".");
                        Java::Runtime::UnsupportedOperationException::throwNew(env, QStringLiteral("Cannot define signals in class %1 because it's meta object is final.").arg(class_name) QTJAMBI_STACKTRACEINFO );
                    }
                }
            }
            const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, objectClass);
            switch(superTypeInfos.size()){
            case 0:
                JavaException::raiseError(env, "Cannot determine type information about object's class." QTJAMBI_STACKTRACEINFO );
                break;
            case 1:
                Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
                if(isShell){
                    size_t offset = size;
                    size += sizeof(QtJambiShell*);
                    void* ptr;
                    if(isDeclarativeCall){
                        Q_ASSERT(arguments);
                        ptr = QtJambiShellImpl::getPlacement(env, callingClass, arguments->l);
                    }else{
                        ptr = operator new(size);
                        memset(ptr, 0, size);
                    }
                    try{
                        JavaException ocurredException;
                        SingleTypeShell* shell;
                        if(returnScopeRequired>0){
                            shell = new SingleTypeRSShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), reinterpret_cast<QObject*>(ptr), &originalMetaObject, true, isDeclarativeCall, isShell, hasCustomMetaObject, &superTypeInfos, ocurredException, returnScopeRequired);
                        }else{
                            shell = new SingleTypeShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), reinterpret_cast<QObject*>(ptr), &originalMetaObject, true, isDeclarativeCall, isShell, hasCustomMetaObject, &superTypeInfos, ocurredException);
                        }
                        if(ocurredException){
                            delete shell;
                            ocurredException.raise();
                            return;
                        }
                        *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                        try{
                            QtJambiLink::registerQObjectInitialization(ptr, shell->link());
                            constructorFunction(ptr, env, object, arguments, &originalMetaObject!=shell->metaObject(), !shell->hasEmptyVTable(), false);
                            QtJambiLink::unregisterQObjectInitialization(ptr);
                        }catch(...){
                            QtJambiLink::unregisterQObjectInitialization(ptr);
                            if(QSharedPointer<QtJambiLink> link = shell->link()){
                                link->invalidate(env);
                            }
                            delete shell;
                            throw;
                        }
                        shell->init(env);
                    }catch(...){
                        if(!isDeclarativeCall)
                            operator delete(ptr);
                        throw;
                    }
                }else{
                    void* ptr;
                    if(isDeclarativeCall){
                        Q_ASSERT(arguments);
                        ptr = QtJambiShellImpl::getPlacement(env, callingClass, arguments->l);
                    }else{
                        ptr = operator new(size);
                        memset(ptr, 0, size);
                    }
                    try{
                        JavaException ocurredException;
                        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewQObject(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), &originalMetaObject, reinterpret_cast<QObject*>(ptr), &superTypeInfos, true, isDeclarativeCall, isShell, hasCustomMetaObject, ocurredException);
                        if(ocurredException){
                            ocurredException.raise();
                        }else{
                            try{
                                QtJambiLink::registerQObjectInitialization(ptr, link);
                                constructorFunction(ptr, env, object, arguments, false, false, false);
                                QtJambiLink::unregisterQObjectInitialization(ptr);
                            }catch(...){
                                QtJambiLink::unregisterQObjectInitialization(ptr);
                                if(link){
                                    link->invalidate(env);
                                }
                                throw;
                            }
                            if(link){
                                link->init(env);
                            }
                        }
                    }catch(...){
                        if(isDeclarativeCall)
                            operator delete(ptr);
                        throw;
                    }
                }
                break;
            default:
                Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
                if(isShell){
                    size_t totalSize = 0;
                    totalSize += size;
                    totalSize += sizeof(QtJambiShell*);

                    QList<ConstructorFunction> constructorFunctions;
                    constructorFunctions << constructorFunction;

                    for(int i=1; i<superTypeInfos.size(); ++i){
                        const SuperTypeInfo& info = superTypeInfos[i];
                        if(info.constructorInfos().isEmpty()){
                            jthrowable t = Java::QtJambi::QInterfaceCannotBeSubclassedException::newInstance(env, info.javaClass());
                            JavaException(env, t).raise();
                            return;
                        }else{
                            totalSize += info.size();
                            totalSize += sizeof(QtJambiShell*);
                            ConstructorFunction foundConstructorFunction = nullptr;
                            for(int j=0; j<info.constructorInfos().size(); ++j){
                                const ResolvedConstructorInfo & constructorInfo = info.constructorInfos().at(j);
                                if(constructorInfo.argumentTypes.isEmpty()){
                                    foundConstructorFunction = constructorInfo.constructorFunction;
                                    break;
                                }
                            }
                            if(!foundConstructorFunction){
                                Java::Runtime::Error::throwNew(env, QStringLiteral("Cannot initialize interface %1 without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.").arg(QString(info.className()).replace("/", ".").replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                return;
                            }
                            constructorFunctions << foundConstructorFunction;
                        }
                    }

                    void* ptr;
                    if(isDeclarativeCall){
                        Q_ASSERT(arguments);
                        ptr = QtJambiShellImpl::getPlacement(env, callingClass, arguments->l);
                    }else{
                        ptr = operator new(totalSize);
                        memset(ptr, 0, totalSize);
                    }
                    try{
                        QMap<size_t,QVector<QtJambiScope*>> returnScopeMap;
                        QHash<size_t,DestructorInfo> destructorInfo;
                        returnScopeRequired = 0;
                        for(const SuperTypeInfo& info : superTypeInfos){
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            destructorInfo[unique_id(info.typeId())] = DestructorInfo(iptr, info.destructor(), info.typeId(), info.ownerFunction());
                            auto rs = returnScopes(info.typeId());
                            returnScopeRequired += rs;
                            if(rs>0)
                                returnScopeMap[unique_id(info.typeId())].resize(rs);
                        }
                        JavaException ocurredException;
                        MultiTypeShell* shell;
                        if(returnScopeRequired>0){
                            shell = new MultiTypeRSShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), reinterpret_cast<QObject*>(ptr), &originalMetaObject, destructorInfo, true, isDeclarativeCall, isShell, hasCustomMetaObject, &superTypeInfos, ocurredException, std::move(returnScopeMap));
                        }else{
                            shell = new MultiTypeShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), reinterpret_cast<QObject*>(ptr), &originalMetaObject, destructorInfo, true, isDeclarativeCall, isShell, hasCustomMetaObject, &superTypeInfos, ocurredException);
                        }
                        if(ocurredException){
                            delete shell;
                            ocurredException.raise();
                            return;
                        }
                        assignShell(superTypeInfos, quintptr(ptr), shell);
                        auto i=superTypeInfos.size()-1;
                        try{
                            QtJambiLink::registerQObjectInitialization(ptr, shell->link());
                            for(; i>=0; --i){
                                const SuperTypeInfo& info = superTypeInfos[i];
                                jvalue* args = i==0 ? arguments : nullptr;
                                void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                                constructorFunctions.at(i)(iptr, env, object, args, &originalMetaObject!=shell->metaObject(), !shell->hasEmptyVTable(), constructorFunction!=constructorFunctions.at(i));
                                if(!info.hasShell())
                                    shell->constructed(info.typeId());
                            }
                            QtJambiLink::unregisterQObjectInitialization(ptr);
                        }catch(const JavaException& exn){
                            for(auto j=i+1; j<superTypeInfos.size(); ++j){
                                const SuperTypeInfo& info = superTypeInfos[j];
                                void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                                QTJAMBI_TRY{
                                    info.destructor()(iptr);
                                }QTJAMBI_CATCH(const JavaException& exn){
                                    exn.addSuppressed(env, exn);
                                }QTJAMBI_TRY_END
                                if(!info.hasShell())
                                    shell->destructed(info.typeId());
                            }
                            QtJambiLink::unregisterQObjectInitialization(ptr);
                            if(QSharedPointer<QtJambiLink> link = shell->link()){
                                link->invalidate(env);
                            }
                            delete shell;
                            exn.raise();
                            return;
                        }
                        shell->init(env);
                    }catch(...){
                        if(isDeclarativeCall)
                            operator delete(ptr);
                        throw;
                    }
                }else{
                    Java::Runtime::Error::throwNew(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg(QtJambiAPI::getClassName(env, callingClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                }
                break;
            }
        }
    }catch(const JavaException& exn){
        if(isDeclarativeCall){
            try{
                Java::QtCore::QObject$QDeclarativeConstructor::set_placement(env, arguments->l, jlong(0xDDDDDDDDDDDDDDD));
            }catch(const JavaException& exn2){
                exn2.report(env);
            }
        }
        exn.raise();
    }
}

void QtJambiThreadShell::initialize(JNIEnv *env, jclass callingClass, jobject object, ConstructorFunction constructorFunction, size_t size, const std::type_info& typeId, uint returnScopeRequired, const QMetaObject& originalMetaObject, bool isShell, bool hasCustomMetaObject, bool isDeclarativeCall, jvalue* arguments){
    try{
        if(!object)
            return;
        QTJAMBI_JNI_LOCAL_FRAME(env, 64);
        jclass objectClass = env->GetObjectClass(object);
        jobject nativeLink = QtJambiLink::getNativeLink(env, object);
        if(env->IsSameObject(objectClass, callingClass)){
            if(isShell){
                size_t offset = size;
                size += sizeof(QtJambiShell*);
                void* ptr;
                if(isDeclarativeCall){
                    Q_ASSERT(arguments);
                    ptr = QtJambiShellImpl::getPlacement(env, callingClass, arguments->l);
                }else{
                    ptr = operator new(size);
                    memset(ptr, 0, size);
                }
                try{
                    JavaException ocurredException;
                    SingleTypeShell* shell;
                    if(returnScopeRequired>0){
                        shell = new SingleTypeRSShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QThread*>(ptr), &originalMetaObject, true, isDeclarativeCall, isShell, hasCustomMetaObject, nullptr, ocurredException, returnScopeRequired);
                    }else{
                        shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QThread*>(ptr), &originalMetaObject, true, isDeclarativeCall, isShell, hasCustomMetaObject, nullptr, ocurredException);
                    }
                    if(ocurredException){
                        delete shell;
                        ocurredException.raise();
                        return;
                    }
                    *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                    QtJambiLink::registerQObjectInitialization(ptr, shell->link());
                    try{
                        constructorFunction(ptr, env, object, arguments, &originalMetaObject!=shell->metaObject(), !shell->hasEmptyVTable(), false);
                        QtJambiLink::unregisterQObjectInitialization(ptr);
                    }catch(...){
                        QtJambiLink::unregisterQObjectInitialization(ptr);
                        if(QSharedPointer<QtJambiLink> link = shell->link()){
                            link->invalidate(env);
                        }
                        delete shell;
                        throw;
                    }
                    shell->init(env);
                }catch(...){
                    if(!isDeclarativeCall){
                        operator delete(ptr);
                    }
                    throw;
                }
            }else{
                void* ptr;
                if(isDeclarativeCall){
                    Q_ASSERT(arguments);
                    ptr = QtJambiShellImpl::getPlacement(env, callingClass, arguments->l);
                }else{
                    ptr = operator new(size);
                    memset(ptr, 0, size);
                }
                try{
                    JavaException ocurredException;
                    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewQObject(env, objectClass, nativeLink, object, typeId, &originalMetaObject, reinterpret_cast<QThread*>(ptr), nullptr, true, isDeclarativeCall, isShell, hasCustomMetaObject, ocurredException);
                    if(ocurredException){
                        ocurredException.raise();
                    }else{
                        try{
                            QtJambiLink::registerQObjectInitialization(ptr, link);
                            constructorFunction(ptr, env, object, arguments, false, false, false);
                            QtJambiLink::unregisterQObjectInitialization(ptr);
                        }catch(...){
                            QtJambiLink::unregisterQObjectInitialization(ptr);
                            if(link){
                                link->invalidate(env);
                            }
                            throw;
                        }
                        if(link){
                            link->init(env);
                        }
                    }
                }catch(...){
                    if(!isDeclarativeCall){
                        operator delete(ptr);
                    }
                    throw;
                }
            }
        }else{
            if(!isShell){
                if(const QtJambiMetaObject* mo = QtJambiMetaObject::cast(CoreAPI::metaObjectForClass(env, objectClass, &originalMetaObject, false))){
                    if(mo->hasSignals()){
                        QString class_name = QtJambiAPI::getClassName(env, objectClass).replace("$", ".");
                        Java::Runtime::UnsupportedOperationException::throwNew(env, QStringLiteral("Cannot define signals in class %1 because it's meta object is final.").arg(class_name) QTJAMBI_STACKTRACEINFO );
                    }
                }
            }
            const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, objectClass);
            switch(superTypeInfos.size()){
            case 0:
                JavaException::raiseError(env, "Cannot determine type information about object's class." QTJAMBI_STACKTRACEINFO );
                break;
            case 1:
                Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
                if(isShell){
                    size_t offset = size;
                    size += sizeof(QtJambiShell*);
                    void* ptr;
                    if(isDeclarativeCall){
                        Q_ASSERT(arguments);
                        ptr = QtJambiShellImpl::getPlacement(env, callingClass, arguments->l);
                    }else{
                        ptr = operator new(size);
                        memset(ptr, 0, size);
                    }
                    try{
                        JavaException ocurredException;
                        SingleTypeShell* shell;
                        if(returnScopeRequired>0){
                            shell = new SingleTypeRSShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), reinterpret_cast<QThread*>(ptr), &originalMetaObject, true, isDeclarativeCall, isShell, hasCustomMetaObject, &superTypeInfos, ocurredException, returnScopeRequired);
                        }else{
                            shell = new SingleTypeShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), reinterpret_cast<QThread*>(ptr), &originalMetaObject, true, isDeclarativeCall, isShell, hasCustomMetaObject, &superTypeInfos, ocurredException);
                        }
                        if(ocurredException){
                            delete shell;
                            ocurredException.raise();
                            return;
                        }
                        *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                        try{
                            QtJambiLink::registerQObjectInitialization(ptr, shell->link());
                            constructorFunction(ptr, env, object, arguments, &originalMetaObject!=shell->metaObject(), !shell->hasEmptyVTable(), false);
                            QtJambiLink::unregisterQObjectInitialization(ptr);
                        }catch(...){
                            QtJambiLink::unregisterQObjectInitialization(ptr);
                            if(QSharedPointer<QtJambiLink> link = shell->link()){
                                link->invalidate(env);
                            }
                            delete shell;
                            throw;
                        }
                        shell->init(env);
                    }catch(...){
                        if(!isDeclarativeCall)
                            operator delete(ptr);
                        throw;
                    }
                }else{
                    void* ptr;
                    if(isDeclarativeCall){
                        Q_ASSERT(arguments);
                        ptr = QtJambiShellImpl::getPlacement(env, callingClass, arguments->l);
                    }else{
                        ptr = operator new(size);
                        memset(ptr, 0, size);
                    }
                    try{
                        JavaException ocurredException;
                        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewQObject(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), &originalMetaObject, reinterpret_cast<QThread*>(ptr), &superTypeInfos, true, isDeclarativeCall, isShell, hasCustomMetaObject, ocurredException);
                        if(ocurredException){
                            ocurredException.raise();
                        }else{
                            try{
                                QtJambiLink::registerQObjectInitialization(ptr, link);
                                constructorFunction(ptr, env, object, arguments, false, false, false);
                                QtJambiLink::unregisterQObjectInitialization(ptr);
                            }catch(...){
                                QtJambiLink::unregisterQObjectInitialization(ptr);
                                if(link){
                                    link->invalidate(env);
                                }
                                throw;
                            }
                            if(link){
                                link->init(env);
                            }
                        }
                    }catch(...){
                        if(isDeclarativeCall)
                            operator delete(ptr);
                        throw;
                    }
                }
                break;
            default:
                Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
                if(isShell){
                    size_t totalSize = 0;
                    totalSize += size;
                    totalSize += sizeof(QtJambiShell*);

                    QList<ConstructorFunction> constructorFunctions;
                    constructorFunctions << constructorFunction;

                    for(int i=1; i<superTypeInfos.size(); ++i){
                        const SuperTypeInfo& info = superTypeInfos[i];
                        if(info.constructorInfos().isEmpty()){
                            jthrowable t = Java::QtJambi::QInterfaceCannotBeSubclassedException::newInstance(env, info.javaClass());
                            JavaException(env, t).raise();
                            return;
                        }else{
                            totalSize += info.size();
                            totalSize += sizeof(QtJambiShell*);
                            ConstructorFunction foundConstructorFunction = nullptr;
                            for(int j=0; j<info.constructorInfos().size(); ++j){
                                const ResolvedConstructorInfo & constructorInfo = info.constructorInfos().at(j);
                                if(constructorInfo.argumentTypes.isEmpty()){
                                    foundConstructorFunction = constructorInfo.constructorFunction;
                                    break;
                                }
                            }
                            if(!foundConstructorFunction){
                                Java::Runtime::Error::throwNew(env, QStringLiteral("Cannot initialize interface %1 without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.").arg(QString(info.className()).replace("/", ".").replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                return;
                            }
                            constructorFunctions << foundConstructorFunction;
                        }
                    }

                    void* ptr;
                    if(isDeclarativeCall){
                        Q_ASSERT(arguments);
                        ptr = QtJambiShellImpl::getPlacement(env, callingClass, arguments->l);
                    }else{
                        ptr = operator new(totalSize);
                        memset(ptr, 0, totalSize);
                    }
                    try{
                        QMap<size_t,QVector<QtJambiScope*>> returnScopeMap;
                        QHash<size_t,DestructorInfo> destructorInfo;
                        returnScopeRequired = 0;
                        for(const SuperTypeInfo& info : superTypeInfos){
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            destructorInfo[unique_id(info.typeId())] = DestructorInfo(iptr, info.destructor(), info.typeId(), info.ownerFunction());
                            auto rs = returnScopes(info.typeId());
                            returnScopeRequired += rs;
                            if(rs>0)
                                returnScopeMap[unique_id(info.typeId())].resize(rs);
                        }
                        JavaException ocurredException;
                        MultiTypeShell* shell;
                        if(returnScopeRequired>0){
                            shell = new MultiTypeRSShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), reinterpret_cast<QThread*>(ptr), &originalMetaObject, destructorInfo, true, isDeclarativeCall, isShell, hasCustomMetaObject, &superTypeInfos, ocurredException, std::move(returnScopeMap));
                        }else{
                            shell = new MultiTypeShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), reinterpret_cast<QThread*>(ptr), &originalMetaObject, destructorInfo, true, isDeclarativeCall, isShell, hasCustomMetaObject, &superTypeInfos, ocurredException);
                        }
                        if(ocurredException){
                            delete shell;
                            ocurredException.raise();
                            return;
                        }
                        assignShell(superTypeInfos, quintptr(ptr), shell);
                        auto i=superTypeInfos.size()-1;
                        try{
                            QtJambiLink::registerQObjectInitialization(ptr, shell->link());
                            for(; i>=0; --i){
                                const SuperTypeInfo& info = superTypeInfos[i];
                                jvalue* args = i==0 ? arguments : nullptr;
                                void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                                constructorFunctions.at(i)(iptr, env, object, args, &originalMetaObject!=shell->metaObject(), !shell->hasEmptyVTable(), constructorFunction!=constructorFunctions.at(i));
                                if(!info.hasShell())
                                    shell->constructed(info.typeId());
                            }
                            QtJambiLink::unregisterQObjectInitialization(ptr);
                        }catch(const JavaException& exn){
                            for(auto j=i+1; j<superTypeInfos.size(); ++j){
                                const SuperTypeInfo& info = superTypeInfos[j];
                                void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                                QTJAMBI_TRY{
                                    info.destructor()(iptr);
                                }QTJAMBI_CATCH(const JavaException& exn){
                                    exn.addSuppressed(env, exn);
                                }QTJAMBI_TRY_END
                                if(!info.hasShell())
                                    shell->destructed(info.typeId());
                            }
                            QtJambiLink::unregisterQObjectInitialization(ptr);
                            if(QSharedPointer<QtJambiLink> link = shell->link()){
                                link->invalidate(env);
                            }
                            delete shell;
                            exn.raise();
                            return;
                        }
                        shell->init(env);
                    }catch(...){
                        if(isDeclarativeCall)
                            operator delete(ptr);
                        throw;
                    }
                }else{
                    Java::Runtime::Error::throwNew(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg(QtJambiAPI::getClassName(env, callingClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                }
                break;
            }
        }
    }catch(const JavaException& exn){
        if(isDeclarativeCall){
            try{
                Java::QtCore::QObject$QDeclarativeConstructor::set_placement(env, arguments->l, jlong(0xDDDDDDDDDDDDDDD));
            }catch(const JavaException& exn2){
                exn2.report(env);
            }
        }
        exn.raise();
    }
}

void QtJambiModelShell::initialize(JNIEnv *env, jclass callingClass, jobject object, ConstructorFunction constructorFunction, size_t size, const std::type_info& typeId, uint returnScopeRequired, const QMetaObject& originalMetaObject, bool isShell, bool hasCustomMetaObject, bool isDeclarativeCall, jvalue* arguments){
    try{
        static ResettableBoolFlag enableItemModelCache(env, "io.qt.experimental.item-model-cached-virtuals");
        if(!enableItemModelCache){
            QtJambiShell::initialize(env, callingClass, object, QtJambiShell::ConstructorFunction(constructorFunction), size, typeId, returnScopeRequired, originalMetaObject, isShell, hasCustomMetaObject, isDeclarativeCall, arguments);
            return;
        }
        if(!object)
            return;
        QTJAMBI_JNI_LOCAL_FRAME(env, 64);

        jclass objectClass = env->GetObjectClass(object);
        jobject nativeLink = QtJambiLink::getNativeLink(env, object);
        if(env->IsSameObject(objectClass, callingClass)){
            if(isShell){
                size_t offset = size;
                size += sizeof(QtJambiShell*);
                void* ptr;
                if(isDeclarativeCall){
                    Q_ASSERT(arguments);
                    ptr = QtJambiShellImpl::getPlacement(env, callingClass, arguments->l);
                }else{
                    ptr = operator new(size);
                    memset(ptr, 0, size);
                }
                try{
                    JavaException ocurredException;
                    SingleTypeShell* shell;
                    if(returnScopeRequired>0){
                        shell = new SingleTypeRSModelShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QObject*>(ptr), &originalMetaObject, true, isDeclarativeCall, isShell, hasCustomMetaObject, nullptr, ocurredException, returnScopeRequired);
                    }else{
                        shell = new SingleTypeModelShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QObject*>(ptr), &originalMetaObject, true, isDeclarativeCall, isShell, hasCustomMetaObject, nullptr, ocurredException);
                    }
                    if(ocurredException){
                        delete shell;
                        ocurredException.raise();
                        return;
                    }
                    *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                    QtJambiLink::registerQObjectInitialization(ptr, shell->link());
                    try{
                        constructorFunction(ptr, env, object, arguments, &originalMetaObject!=shell->metaObject(), !shell->hasEmptyVTable(), false);
                        QtJambiLink::unregisterQObjectInitialization(ptr);
                    }catch(...){
                        QtJambiLink::unregisterQObjectInitialization(ptr);
                        if(QSharedPointer<QtJambiLink> link = shell->link()){
                            link->invalidate(env);
                        }
                        delete shell;
                        throw;
                    }
                    shell->init(env);
                }catch(...){
                    if(!isDeclarativeCall){
                        operator delete(ptr);
                    }
                    throw;
                }
            }else{
                void* ptr;
                if(isDeclarativeCall){
                    Q_ASSERT(arguments);
                    ptr = QtJambiShellImpl::getPlacement(env, callingClass, arguments->l);
                }else{
                    ptr = operator new(size);
                    memset(ptr, 0, size);
                }
                try{
                    JavaException ocurredException;
                    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewQObject(env, objectClass, nativeLink, object, typeId, &originalMetaObject, reinterpret_cast<QObject*>(ptr), nullptr, true, isDeclarativeCall, isShell, hasCustomMetaObject, ocurredException);
                    if(ocurredException){
                        ocurredException.raise();
                    }else{
                        try{
                            QtJambiLink::registerQObjectInitialization(ptr, link);
                            constructorFunction(ptr, env, object, arguments, false, false, false);
                            QtJambiLink::unregisterQObjectInitialization(ptr);
                        }catch(...){
                            QtJambiLink::unregisterQObjectInitialization(ptr);
                            if(link){
                                link->invalidate(env);
                            }
                            throw;
                        }
                        if(link){
                            link->init(env);
                        }
                    }
                }catch(...){
                    if(!isDeclarativeCall){
                        operator delete(ptr);
                    }
                    throw;
                }
            }
        }else{
            if(!isShell){
                if(const QtJambiMetaObject* mo = QtJambiMetaObject::cast(CoreAPI::metaObjectForClass(env, objectClass, &originalMetaObject, false))){
                    if(mo->hasSignals()){
                        QString class_name = QtJambiAPI::getClassName(env, objectClass).replace("$", ".");
                        Java::Runtime::UnsupportedOperationException::throwNew(env, QStringLiteral("Cannot define signals in class %1 because it's meta object is final.").arg(class_name) QTJAMBI_STACKTRACEINFO );
                    }
                }
            }
            const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, objectClass);
            auto superTypeInfosCount = superTypeInfos.size();
            switch(superTypeInfosCount){
            case 0:
                JavaException::raiseError(env, "Cannot determine type information about object's class." QTJAMBI_STACKTRACEINFO );
                break;
            case 1:
                Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
                if(isShell){
                    size_t offset = size;
                    size += sizeof(QtJambiShell*);
                    void* ptr;
                    if(isDeclarativeCall){
                        Q_ASSERT(arguments);
                        ptr = QtJambiShellImpl::getPlacement(env, callingClass, arguments->l);
                    }else{
                        ptr = operator new(size);
                        memset(ptr, 0, size);
                    }
                    try{
                        JavaException ocurredException;
                        SingleTypeShell* shell;
                        if(returnScopeRequired>0){
                            shell = new SingleTypeRSModelShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), reinterpret_cast<QObject*>(ptr), &originalMetaObject, true, isDeclarativeCall, isShell, hasCustomMetaObject, &superTypeInfos, ocurredException, returnScopeRequired);
                        }else{
                            shell = new SingleTypeModelShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), reinterpret_cast<QObject*>(ptr), &originalMetaObject, true, isDeclarativeCall, isShell, hasCustomMetaObject, &superTypeInfos, ocurredException);
                        }
                        if(ocurredException){
                            delete shell;
                            ocurredException.raise();
                            return;
                        }
                        *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                        try{
                            QtJambiLink::registerQObjectInitialization(ptr, shell->link());
                            constructorFunction(ptr, env, object, arguments, &originalMetaObject!=shell->metaObject(), !shell->hasEmptyVTable(), false);
                            QtJambiLink::unregisterQObjectInitialization(ptr);
                        }catch(...){
                            QtJambiLink::unregisterQObjectInitialization(ptr);
                            if(QSharedPointer<QtJambiLink> link = shell->link()){
                                link->invalidate(env);
                            }
                            delete shell;
                            throw;
                        }
                        shell->init(env);
                    }catch(...){
                        if(!isDeclarativeCall)
                            operator delete(ptr);
                        throw;
                    }
                }else{
                    void* ptr;
                    if(isDeclarativeCall){
                        Q_ASSERT(arguments);
                        ptr = QtJambiShellImpl::getPlacement(env, callingClass, arguments->l);
                    }else{
                        ptr = operator new(size);
                        memset(ptr, 0, size);
                    }
                    try{
                        JavaException ocurredException;
                        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewQObject(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), &originalMetaObject, reinterpret_cast<QObject*>(ptr), &superTypeInfos, true, isDeclarativeCall, isShell, hasCustomMetaObject, ocurredException);
                        if(ocurredException){
                            ocurredException.raise();
                        }else{
                            try{
                                QtJambiLink::registerQObjectInitialization(ptr, link);
                                constructorFunction(ptr, env, object, arguments, false, false, false);
                                QtJambiLink::unregisterQObjectInitialization(ptr);
                            }catch(...){
                                QtJambiLink::unregisterQObjectInitialization(ptr);
                                if(link){
                                    link->invalidate(env);
                                }
                                throw;
                            }
                            if(link){
                                link->init(env);
                            }
                        }
                    }catch(...){
                        if(isDeclarativeCall)
                            operator delete(ptr);
                        throw;
                    }
                }
                break;
            default:
                Q_ASSERT(typeid_equals(typeId, superTypeInfos.at(0).typeId()) || typeid_equals(typeId, typeid(void)));
                if(isShell){
                    size_t totalSize = 0;
                    totalSize += size;
                    totalSize += sizeof(QtJambiShell*);

                    QList<ConstructorFunction> constructorFunctions;
                    constructorFunctions << constructorFunction;

                    for(int i=1; i<superTypeInfosCount; ++i){
                        const SuperTypeInfo& info = superTypeInfos[i];
                        if(info.constructorInfos().isEmpty()){
                            jthrowable t = Java::QtJambi::QInterfaceCannotBeSubclassedException::newInstance(env, info.javaClass());
                            JavaException(env, t).raise();
                            return;
                        }else{
                            totalSize += info.size();
                            totalSize += sizeof(QtJambiShell*);
                            ConstructorFunction foundConstructorFunction = nullptr;
                            for(int j=0; j<info.constructorInfos().size(); ++j){
                                const ResolvedConstructorInfo & constructorInfo = info.constructorInfos().at(j);
                                if(constructorInfo.argumentTypes.isEmpty()){
                                    foundConstructorFunction = constructorInfo.constructorFunction;
                                    break;
                                }
                            }
                            if(!foundConstructorFunction){
                                Java::Runtime::Error::throwNew(env, QStringLiteral("Cannot initialize interface %1 without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.").arg(QString(info.className()).replace("/", ".").replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                return;
                            }
                            constructorFunctions << foundConstructorFunction;
                        }
                    }

                    void* ptr;
                    if(isDeclarativeCall){
                        Q_ASSERT(arguments);
                        ptr = QtJambiShellImpl::getPlacement(env, callingClass, arguments->l);
                    }else{
                        ptr = operator new(totalSize);
                        memset(ptr, 0, totalSize);
                    }
                    try{
                        QMap<size_t,QVector<QtJambiScope*>> returnScopeMap;
                        QHash<size_t,DestructorInfo> destructorInfo;
                        returnScopeRequired = 0;
                        for(const SuperTypeInfo& info : superTypeInfos){
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            destructorInfo[unique_id(info.typeId())] = DestructorInfo(iptr, info.destructor(), info.typeId(), info.ownerFunction());
                            auto rs = returnScopes(info.typeId());
                            returnScopeRequired += rs;
                            if(rs>0)
                                returnScopeMap[unique_id(info.typeId())].resize(rs);
                        }
                        JavaException ocurredException;
                        MultiTypeShell* shell;
                        if(returnScopeRequired>0){
                            shell = new MultiTypeRSModelShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), reinterpret_cast<QObject*>(ptr), &originalMetaObject, destructorInfo, true, isDeclarativeCall, isShell, hasCustomMetaObject, &superTypeInfos, ocurredException, std::move(returnScopeMap));
                        }else{
                            shell = new MultiTypeModelShell(env, objectClass, nativeLink, object, superTypeInfos.at(0).typeId(), reinterpret_cast<QObject*>(ptr), &originalMetaObject, destructorInfo, true, isDeclarativeCall, isShell, hasCustomMetaObject, &superTypeInfos, ocurredException);
                        }
                        if(ocurredException){
                            delete shell;
                            ocurredException.raise();
                            return;
                        }
                        assignShell(superTypeInfos, quintptr(ptr), shell);
                        auto i=superTypeInfosCount-1;
                        try{
                            QtJambiLink::registerQObjectInitialization(ptr, shell->link());
                            for(; i>=0; --i){
                                const SuperTypeInfo& info = superTypeInfos[i];
                                jvalue* args = i==0 ? arguments : nullptr;
                                void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                                constructorFunctions.at(i)(iptr, env, object, args, &originalMetaObject!=shell->metaObject(), !shell->hasEmptyVTable(), constructorFunction!=constructorFunctions.at(i));
                                if(!info.hasShell())
                                    shell->constructed(info.typeId());
                            }
                            QtJambiLink::unregisterQObjectInitialization(ptr);
                        }catch(const JavaException& exn){
                            for(auto j=i+1; j<superTypeInfosCount; ++j){
                                const SuperTypeInfo& info = superTypeInfos[j];
                                void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                                QTJAMBI_TRY{
                                    info.destructor()(iptr);
                                }QTJAMBI_CATCH(const JavaException& exn){
                                    exn.addSuppressed(env, exn);
                                }QTJAMBI_TRY_END
                                if(!info.hasShell())
                                    shell->destructed(info.typeId());
                            }
                            QtJambiLink::unregisterQObjectInitialization(ptr);
                            if(QSharedPointer<QtJambiLink> link = shell->link()){
                                link->invalidate(env);
                            }
                            delete shell;
                            exn.raise();
                            return;
                        }
                        shell->init(env);
                    }catch(...){
                        if(isDeclarativeCall)
                            operator delete(ptr);
                        throw;
                    }
                }else{
                    Java::Runtime::Error::throwNew(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg(QtJambiAPI::getClassName(env, callingClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                }
                break;
            }
        }
    }catch(const JavaException& exn){
        if(isDeclarativeCall){
            try{
                Java::QtCore::QObject$QDeclarativeConstructor::set_placement(env, arguments->l, jlong(0xDDDDDDDDDDDDDDD));
            }catch(const JavaException& exn2){
                exn2.report(env);
            }
        }
        exn.raise();
    }
}

QtJambiShellImpl* QtJambiShellImpl::get(const QtJambiShellInterface* shellInterface){
    return static_cast<QtJambiShellImpl*>(shellInterface->__shell());
}

jmethodID QtJambiShellImpl::javaMethod(const QtJambiShellInterface* shellInterface, const std::type_info& typeId, int pos){
    QtJambiShellImpl* impl = get(shellInterface);
    Q_ASSERT(impl->m_vtable);
    return impl->m_vtable->javaMethod(typeId, pos);
}

QSharedPointer<QtJambiLink> QtJambiShellImpl::link(const QtJambiShellInterface* shellInterface){
    return get(shellInterface)->m_link;
}

void QtJambiShellImpl::initializeNativeInterface(JNIEnv *env, jclass callingClass, jobject object, jobject nativeLink, jobject arguments){
    if(nativeLink && object){
        QTJAMBI_JNI_LOCAL_FRAME(env, 64);
        jclass objectClass = env->GetObjectClass(object);
        const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, objectClass);
        if(superTypeInfos.isEmpty())
            JavaException::raiseError(env, "Cannot determine type information about object's class." QTJAMBI_STACKTRACEINFO );
        if(superTypeInfos.size()>1 && !(isInterface(superTypeInfos.at(0).typeId()) || superTypeInfos.at(0).hasShell())){
            Java::Runtime::Error::throwNew(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg(QString(superTypeInfos.at(0).className()).replace("/", ".").replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
        size_t totalSize = 0;
        QList<QtJambiShell::ConstructorFunction> constructorFunctions;
        QList<QVector<jvalue>> constructorArguments;
        jobject declarativeCall = nullptr;
        for(int s=0; s<superTypeInfos.size(); ++s){
            const SuperTypeInfo& info = superTypeInfos.at(s);
            QList<jclass> givenArgumentTypes;
            QVector<jvalue> givenArguments;
//            QString className = QtJambiAPI::getClassName(env, info.javaClass());
            jobject argumentList = arguments ? Java::Runtime::Map::get(env, arguments, info.javaClass()) : nullptr;
            if(!env->IsSameObject(argumentList, nullptr)){
                givenArguments.reserve(QtJambiAPI::sizeOfJavaCollection(env, argumentList));
                jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, argumentList);
                while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                    jobject currentArg = QtJambiAPI::nextOfJavaIterator(env, iter);
                    jclass givenArgumentType = jclass(Java::Runtime::Map$Entry::getKey(env, currentArg));
                    jobject givenArgument = Java::Runtime::Map$Entry::getValue(env, currentArg);
                    givenArgumentTypes << givenArgumentType;
                    jvalue value;
                    if(Java::Runtime::Integer::isPrimitiveType(env,givenArgumentType)){
                        value.i = QtJambiAPI::fromJavaIntegerObject(env, givenArgument);
                    }else if(Java::Runtime::Long::isPrimitiveType(env, givenArgumentType)){
                        value.j = QtJambiAPI::fromJavaLongObject(env, givenArgument);
                    }else if(Java::Runtime::Short::isPrimitiveType(env, givenArgumentType)){
                        value.s = QtJambiAPI::fromJavaShortObject(env, givenArgument);
                    }else if(Java::Runtime::Byte::isPrimitiveType(env, givenArgumentType)){
                        value.b = QtJambiAPI::fromJavaByteObject(env, givenArgument);
                    }else if(Java::Runtime::Double::isPrimitiveType(env, givenArgumentType)){
                        value.d = QtJambiAPI::fromJavaDoubleObject(env, givenArgument);
                    }else if(Java::Runtime::Float::isPrimitiveType(env, givenArgumentType)){
                        value.f = QtJambiAPI::fromJavaFloatObject(env, givenArgument);
                    }else if(Java::Runtime::Character::isPrimitiveType(env, givenArgumentType)){
                        value.c = QtJambiAPI::fromJavaCharacterObject(env, givenArgument);
                    }else if(Java::Runtime::Boolean::isPrimitiveType(env, givenArgumentType)){
                        value.b = QtJambiAPI::fromJavaBooleanObject(env, givenArgument);
                    }else{
                        value.l = givenArgument;
                    }
                    givenArguments << value;
                }
            }
            QtJambiShell::ConstructorFunction foundConstructorFunction = nullptr;
            for(int j=0; j<info.constructorInfos().size(); ++j){
                const ResolvedConstructorInfo & constructorInfo = info.constructorInfos().at(j);
                if(constructorInfo.argumentTypes.size()==givenArgumentTypes.size()){
                    bool matches = true;
                    for(int a=0; a<constructorInfo.argumentTypes.size(); ++a){
                        if(!env->IsAssignableFrom(givenArgumentTypes[a], constructorInfo.argumentTypes[a])){
                            if(Java::Runtime::Collection::isSameClass(env, givenArgumentTypes[a])
                                    && Java::Runtime::Collection::isAssignableFrom(env, constructorInfo.argumentTypes[a])){
                                continue;
                            }else if(Java::Runtime::Map::isSameClass(env, givenArgumentTypes[a])
                                     && Java::Runtime::Map::isAssignableFrom(env, constructorInfo.argumentTypes[a])){
                                continue;
                            }else{
                                matches = false;
                                break;
                            }
                        }
                    }
                    if(matches){
                        foundConstructorFunction = constructorInfo.constructorFunction;
                        if(s==0 && constructorInfo.argumentTypes.size()==1
                                && Java::QtCore::QObject$QDeclarativeConstructor::isSameClass(env, constructorInfo.argumentTypes[0])){
                            declarativeCall = givenArguments[0].l;
                        }
                        break;
                    }
                }
            }
            if(!foundConstructorFunction){
                QString argumentStrg;
                for(jclass cls : qAsConst(givenArgumentTypes)){
                    if(!argumentStrg.isEmpty())
                        argumentStrg += ", ";
                    QString className = QtJambiAPI::getClassName(env, cls).replace("$", ".");
                    if(className.endsWith(";"))
                        className.chop(1);
                    while(className.startsWith("[")){
                        className = className.mid(1);
                        className += "[]";
                    }
                    argumentStrg += className;
                }
                Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Cannot find constructor %1(%2).").arg(QString(info.className()).replace("/", ".").replace("$", "."), qPrintable(argumentStrg)) QTJAMBI_STACKTRACEINFO );
            }else{
                constructorFunctions << foundConstructorFunction;
                constructorArguments << givenArguments;
            }
            totalSize += info.size();
            totalSize += sizeof(QtJambiShell*);
        }
        void* ptr;
        if(declarativeCall){
            Q_ASSERT(arguments);
            ptr = QtJambiShellImpl::getPlacement(env, callingClass, declarativeCall);
        }else{
            ptr = operator new(totalSize);
            memset(ptr, 0, totalSize);
        }
        try{
            QMap<size_t,QVector<QtJambiScope*>> returnScopeMap;
            QHash<size_t,DestructorInfo> destructorInfo;
            uint returnScopeRequired = 0;
            for(const SuperTypeInfo& info : superTypeInfos){
                void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                destructorInfo[unique_id(info.typeId())] = DestructorInfo(iptr, info.destructor(), info.typeId(), info.ownerFunction());
                auto rs = returnScopes(info.typeId());
                returnScopeRequired += rs;
                if(rs>0)
                    returnScopeMap[unique_id(info.typeId())].resize(rs);
            }
            QtJambiShellImpl* shell;
            Q_ASSERT(!superTypeInfos.isEmpty());
            const std::type_info& typeId = superTypeInfos[0].typeId();
            JavaException ocurredException;
            if(Java::QtCore::QObject::isInstanceOf(env, object)){
                if(Java::QtCore::QAbstractItemModel::isInstanceOf(env, object)){
                    if(superTypeInfos.size()==1){
                        if(returnScopeRequired>0){
                            shell = new SingleTypeRSModelShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QObject*>(ptr), registeredOriginalMetaObject(typeId), true, false, true, false, &superTypeInfos, ocurredException, returnScopeRequired);
                        }else{
                            shell = new SingleTypeModelShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QObject*>(ptr), registeredOriginalMetaObject(typeId), true, false, true, false, &superTypeInfos, ocurredException);
                        }
                    }else{
                        if(returnScopeRequired>0){
                            shell = new MultiTypeRSModelShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QObject*>(ptr), registeredOriginalMetaObject(typeId), destructorInfo, true, false, true, false, &superTypeInfos, ocurredException, std::move(returnScopeMap));
                        }else{
                            shell = new MultiTypeModelShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QObject*>(ptr), registeredOriginalMetaObject(typeId), destructorInfo, true, false, true, false, &superTypeInfos, ocurredException);
                        }
                    }
                }else{
                    if(Java::QtCore::QThread::isInstanceOf(env, object)){
                        if(superTypeInfos.size()==1){
                            shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QThread*>(ptr), registeredOriginalMetaObject(typeId), true, false, true, false, &superTypeInfos, ocurredException, returnScopeRequired)
                                                            : new SingleTypeShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QThread*>(ptr), registeredOriginalMetaObject(typeId), true, false, true, false, &superTypeInfos, ocurredException);
                        }else{
                            shell = returnScopeRequired>0 ? new MultiTypeRSShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QThread*>(ptr), registeredOriginalMetaObject(typeId), destructorInfo, true, false, true, false, &superTypeInfos, ocurredException, std::move(returnScopeMap))
                                                            : new MultiTypeShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QThread*>(ptr), registeredOriginalMetaObject(typeId), destructorInfo, true, false, true, false, &superTypeInfos, ocurredException);
                        }
                    }else{
                        if(superTypeInfos.size()==1){
                            shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QObject*>(ptr), registeredOriginalMetaObject(typeId), true, false, true, false, &superTypeInfos, ocurredException, returnScopeRequired)
                                                            : new SingleTypeShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QObject*>(ptr), registeredOriginalMetaObject(typeId), true, false, true, false, &superTypeInfos, ocurredException);
                        }else{
                            shell = returnScopeRequired>0 ? new MultiTypeRSShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QObject*>(ptr), registeredOriginalMetaObject(typeId), destructorInfo, true, false, true, false, &superTypeInfos, ocurredException, std::move(returnScopeMap))
                                                            : new MultiTypeShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QObject*>(ptr), registeredOriginalMetaObject(typeId), destructorInfo, true, false, true, false, &superTypeInfos, ocurredException);
                        }
                    }
                }
            }else if(isFunctional(typeId)){
                if(auto deleter_function = deleter(typeId)){
                    if(superTypeInfos.size()==1){
                        shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, true, true, &superTypeInfos, deleter_function, ocurredException, returnScopeRequired)
                                                        : new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, true, &superTypeInfos, deleter_function, ocurredException);
                    }else{
                        shell = returnScopeRequired>0 ? new MultiTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, destructorInfo, true, true, &superTypeInfos, deleter_function, ocurredException, std::move(returnScopeMap))
                                                        : new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, destructorInfo, true, true, &superTypeInfos, deleter_function, ocurredException);
                    }
                }else{
                    if(superTypeInfos.size()==1){
                        shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, true, true, &superTypeInfos, ocurredException, returnScopeRequired)
                                                        : new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, true, &superTypeInfos, ocurredException);
                    }else{
                        shell = returnScopeRequired>0 ? new MultiTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, destructorInfo, true, true, &superTypeInfos, ocurredException, std::move(returnScopeMap))
                                                        : new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, destructorInfo, true, true, &superTypeInfos, ocurredException);
                    }
                }
            }else{
                if(NewContainerAccessFunction containerAccessFactory = getSequentialContainerAccessFactory(typeId)){
                    if(superTypeInfos.size()==1){
                        shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr,
                                                                              true, true, &superTypeInfos, containerAccessFactory(), ocurredException, returnScopeRequired)
                                                        : new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr,
                                                                              true, true, &superTypeInfos, containerAccessFactory(), ocurredException);
                    }else{
                        shell = returnScopeRequired>0 ? new MultiTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr,
                                                                             destructorInfo, true, true, &superTypeInfos, containerAccessFactory(), ocurredException, std::move(returnScopeMap))
                                                        : new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr,
                                                                             destructorInfo, true, true, &superTypeInfos, containerAccessFactory(), ocurredException);
                    }
                }else{
                    QMetaType metaType(registeredMetaTypeID(typeId));
                    if(metaType.isValid()){
                        if(superTypeInfos.size()==1){
                            if(auto ownerFunction = registeredOwnerFunction(typeId)){
                                shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, true, true, &superTypeInfos, ownerFunction, ocurredException, returnScopeRequired)
                                                                : new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, true, true, &superTypeInfos, ownerFunction, ocurredException);
                            }else{
                                shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, true, true, &superTypeInfos, ocurredException, returnScopeRequired)
                                                                : new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, true, true, &superTypeInfos, ocurredException);
                            }
                        }else{
                            if(auto ownerFunction = registeredOwnerFunction(typeId)){
                                shell = returnScopeRequired>0 ? new MultiTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, destructorInfo, true, true, &superTypeInfos, ownerFunction, ocurredException, std::move(returnScopeMap))
                                                                : new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, destructorInfo, true, true, &superTypeInfos, ownerFunction, ocurredException);
                            }else{
                                shell = returnScopeRequired>0 ? new MultiTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, destructorInfo, true, true, &superTypeInfos, ocurredException, std::move(returnScopeMap))
                                                                : new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, destructorInfo, true, true, &superTypeInfos, ocurredException);
                            }
                        }
                    }else{
                        if(auto deleter_function = deleter(typeId)){
                            if(superTypeInfos.size()==1){
                                if(auto ownerFunction = registeredOwnerFunction(typeId)){
                                    shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, true, true, &superTypeInfos, deleter_function, ownerFunction, ocurredException, returnScopeRequired)
                                                                    : new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, true, &superTypeInfos, deleter_function, ownerFunction, ocurredException);
                                }else{
                                    shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, true, true, &superTypeInfos, deleter_function, ocurredException, returnScopeRequired)
                                                                    : new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, true, &superTypeInfos, deleter_function, ocurredException);
                                }
                            }else{
                                if(auto ownerFunction = registeredOwnerFunction(typeId)){
                                    shell = returnScopeRequired>0 ? new MultiTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, destructorInfo, true, true, &superTypeInfos, deleter_function, ownerFunction, ocurredException, std::move(returnScopeMap))
                                                                    : new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, destructorInfo, true, true, &superTypeInfos, deleter_function, ownerFunction, ocurredException);
                                }else{
                                    shell = returnScopeRequired>0 ? new MultiTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, destructorInfo, true, true, &superTypeInfos, deleter_function, ocurredException, std::move(returnScopeMap))
                                                                    : new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, destructorInfo, true, true, &superTypeInfos, deleter_function, ocurredException);
                                }
                            }
                        }else{
                            if(superTypeInfos.size()==1){
                                shell = returnScopeRequired>0 ? new SingleTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, true, true, &superTypeInfos, ocurredException, returnScopeRequired)
                                                                : new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, true, &superTypeInfos, ocurredException);
                            }else{
                                shell = returnScopeRequired>0 ? new MultiTypeRSShell(env, objectClass, nativeLink, object, typeId, ptr, destructorInfo, true, true, &superTypeInfos, ocurredException, std::move(returnScopeMap))
                                                                : new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, destructorInfo, true, true, &superTypeInfos, ocurredException);
                            }
                        }
                    }
                }
            }
            if(ocurredException){
                delete shell;
                ocurredException.raise();
                return;
            }
            assignShell(superTypeInfos, quintptr(ptr), shell);
            auto i=superTypeInfos.size()-1;
            try{
                for(; i>=0; --i){
                    const SuperTypeInfo& info = superTypeInfos[i];
                    void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                    constructorFunctions.at(i)(iptr, env, object, constructorArguments[i].data(), QtJambiMetaObject::isInstance(shell->metaObject()), !shell->hasEmptyVTable(), true);
                    if(!info.hasShell())
                        shell->constructed(info.typeId());
                }
            }catch(const JavaException& exn){
                for(auto j=i+1; j<superTypeInfos.size(); ++j){
                    const SuperTypeInfo& info = superTypeInfos[j];
                    void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                    QTJAMBI_TRY{
                        info.destructor()(iptr);
                    }QTJAMBI_CATCH(const JavaException& exn){
                        exn.addSuppressed(env, exn);
                    }QTJAMBI_TRY_END
                    if(!info.hasShell())
                        shell->destructed(info.typeId());
                }
                if(QSharedPointer<QtJambiLink> link = shell->link()){
                    link->invalidate(env);
                }
                delete shell;
                exn.raise();
                return;
            }
            shell->init(env);
        }catch(...){
            if(!declarativeCall)
                operator delete(ptr);
            throw;
        }
    }
}

class RegularVTable : public VTable
{
public:
    RegularVTable(JNIEnv* env, jclass clazz, const QList<jmethodID> & methods);
    jmethodID javaMethod(const std::type_info& typeId, int pos) const override;
    bool isEmpty() const override;
private:
    QList<jmethodID> m_method_ids;
};

class EmptyVTable : public VTable
{
public:
    EmptyVTable(JNIEnv* env, jclass clazz);
    bool isEmpty() const override;
};

class InterfaceVTable : public VTable
{
public:
    InterfaceVTable(JNIEnv* env, jclass clazz, const QMap<std::type_index, QList<jmethodID>> & methods);
    jmethodID javaMethod(const std::type_info& typeId, int pos) const override;
    bool isEmpty() const override;
private:
    QMap<std::type_index, QList<jmethodID>> m_method_ids;
};

class MetaObjectVTable : public RegularVTable
{
public:
    MetaObjectVTable(JNIEnv* env, jclass clazz, const QList<jmethodID> & methods, const QMetaObject* metaObject);
    const QMetaObject* metaObject() const override;
private:
    const QMetaObject* m_metaObject;
};

class MetaObjectEmptyVTable : public EmptyVTable
{
public:
    MetaObjectEmptyVTable(JNIEnv* env, jclass clazz, const QMetaObject* metaObject);
    const QMetaObject* metaObject() const override;
private:
    const QMetaObject* m_metaObject;
};

class MetaObjectInterfaceVTable : public InterfaceVTable
{
public:
    MetaObjectInterfaceVTable(JNIEnv* env, jclass clazz, const QMap<std::type_index, QList<jmethodID>> & methods, const QMetaObject* metaObject);
    const QMetaObject* metaObject() const override;
private:
    const QMetaObject* m_metaObject;
};

class DynamicMetaObjectVTable : public RegularVTable
{
public:
    DynamicMetaObjectVTable(JNIEnv* env, jclass clazz, const QList<jmethodID> & methods, const QtJambiMetaObject* metaObject);
    const QMetaObject* metaObject() const override;
private:
    QSharedPointer<const QtJambiMetaObject> m_metaObject;
};

class DynamicMetaObjectInterfaceVTable : public InterfaceVTable
{
public:
    DynamicMetaObjectInterfaceVTable(JNIEnv* env, jclass clazz, const QMap<std::type_index, QList<jmethodID>> & methods, const QtJambiMetaObject* metaObject);
    const QMetaObject* metaObject() const override;
private:
    QSharedPointer<const QtJambiMetaObject> m_metaObject;
};

VTable::VTable(JNIEnv* env, jclass clazz)
    : m_class(getGlobalClassRef(env, clazz))
{
}

VTable::~VTable(){
}

jclass VTable::javaClass() const
{
    return m_class;
}

const QMetaObject* VTable::metaObject() const
{
    if(JniEnvironment env{200}){
        return CoreAPI::metaObjectForClass(env, m_class, nullptr);
    }else{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        return qt_getQtMetaObject();
#else
        return &Qt::staticMetaObject;
#endif
    }
}

jmethodID VTable::javaMethod(const std::type_info&, int) const
{
    return nullptr;
}

RegularVTable::RegularVTable(JNIEnv* env, jclass clazz,
                                         const QList<jmethodID> & methods)
    : VTable(env, clazz),
      m_method_ids(methods)
{
}

jmethodID RegularVTable::javaMethod(const std::type_info&, int pos) const
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos < m_method_ids.size());
    return pos >= 0 && pos < m_method_ids.size() ? m_method_ids.at(pos) : nullptr;
}

bool RegularVTable::isEmpty() const{
    return false;
}

InterfaceVTable::InterfaceVTable(JNIEnv* env, jclass clazz,
                                         const QMap<std::type_index, QList<jmethodID>> & methods)
    : VTable(env, clazz),
      m_method_ids(methods)
{
}

jmethodID InterfaceVTable::javaMethod(const std::type_info& typeId, int pos) const
{
    Q_ASSERT(m_method_ids.contains(typeId));
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos < m_method_ids[typeId].size());
    return pos >= 0 && pos < m_method_ids[typeId].size() ? m_method_ids[typeId].at(pos) : nullptr;
}

bool InterfaceVTable::isEmpty() const{
    return false;
}

EmptyVTable::EmptyVTable(JNIEnv* env, jclass clazz)
    : VTable(env, clazz)
{
}

bool EmptyVTable::isEmpty() const{
    return true;
}

MetaObjectEmptyVTable::MetaObjectEmptyVTable(JNIEnv* env, jclass clazz, const QMetaObject* metaObject)
: EmptyVTable(env, clazz), m_metaObject(metaObject) {}

const QMetaObject* MetaObjectEmptyVTable::metaObject() const { return m_metaObject; }

MetaObjectVTable::MetaObjectVTable(JNIEnv* env, jclass clazz, const QList<jmethodID> & methods, const QMetaObject* metaObject)
    : RegularVTable(env, clazz, methods), m_metaObject(metaObject) {}

const QMetaObject* MetaObjectVTable::metaObject() const { return m_metaObject; }

MetaObjectInterfaceVTable::MetaObjectInterfaceVTable(JNIEnv* env, jclass clazz, const QMap<std::type_index, QList<jmethodID>> & methods, const QMetaObject* metaObject)
    : InterfaceVTable(env, clazz, methods), m_metaObject(metaObject) {}

const QMetaObject* MetaObjectInterfaceVTable::metaObject() const { return m_metaObject; }

DynamicMetaObjectVTable::DynamicMetaObjectVTable(JNIEnv* env, jclass clazz, const QList<jmethodID> & methods, const QtJambiMetaObject* metaObject)
    : RegularVTable(env, clazz, methods), m_metaObject(metaObject->thisPointer()) {}

const QMetaObject* DynamicMetaObjectVTable::metaObject() const { return m_metaObject.get(); }

DynamicMetaObjectInterfaceVTable::DynamicMetaObjectInterfaceVTable(JNIEnv* env, jclass clazz, const QMap<std::type_index, QList<jmethodID>> & methods, const QtJambiMetaObject* metaObject)
    : InterfaceVTable(env, clazz, methods), m_metaObject(metaObject->thisPointer()) {}

const QMetaObject* DynamicMetaObjectInterfaceVTable::metaObject() const { return m_metaObject.get(); }

QString jniSignatureToJava(QString jniSignature, const QString& methodName){
    QStringList signature;
    QString array;
    QString returnType;
    if(jniSignature.startsWith('(')){
        jniSignature = jniSignature.mid(1);
        while(!jniSignature.isEmpty() && !jniSignature.startsWith(')')){
            if(jniSignature.startsWith('L')){
                jniSignature = jniSignature.mid(1);
                auto idx = jniSignature.indexOf(';');
                if(idx>=0){
                    signature << jniSignature.left(idx).replace('/', '.').replace('$', '.') + array;
                    array.clear();
                    jniSignature = jniSignature.mid(idx+1);
                }else{
                    jniSignature.clear();
                }
            }else{
                if(jniSignature.startsWith('V')){
                }else if(jniSignature.startsWith('I')){
                    signature << "int" + array;
                    array.clear();
                }else if(jniSignature.startsWith('J')){
                    signature << "long" + array;
                    array.clear();
                }else if(jniSignature.startsWith('S')){
                    signature << "short" + array;
                    array.clear();
                }else if(jniSignature.startsWith('B')){
                    signature << "byte" + array;
                    array.clear();
                }else if(jniSignature.startsWith('C')){
                    signature << "char" + array;
                    array.clear();
                }else if(jniSignature.startsWith('Z')){
                    signature << "boolean" + array;
                    array.clear();
                }else if(jniSignature.startsWith('D')){
                    signature << "double" + array;
                    array.clear();
                }else if(jniSignature.startsWith('F')){
                    signature << "float" + array;
                    array.clear();
                }else if(jniSignature.startsWith('[')){
                    array += "[]";
                }
                jniSignature = jniSignature.mid(1);
            }
        }
        if(jniSignature.startsWith(')')){
            jniSignature = jniSignature.mid(1);
            if(jniSignature.startsWith('L')){
                jniSignature = jniSignature.mid(1);
                auto idx = jniSignature.indexOf(';');
                if(idx>=0){
                    returnType = jniSignature.left(idx).replace('/', '.').replace('$', '.') + array;
                    jniSignature = jniSignature.mid(idx+1);
                }else{
                    jniSignature.clear();
                }
            }else{
                if(jniSignature.startsWith('V')){
                    returnType = "void";
                }else if(jniSignature.startsWith('I')){
                    returnType = "int" + array;
                }else if(jniSignature.startsWith('J')){
                    returnType = "long" + array;
                }else if(jniSignature.startsWith('S')){
                    returnType = "short" + array;
                }else if(jniSignature.startsWith('B')){
                    returnType = "byte" + array;
                }else if(jniSignature.startsWith('C')){
                    returnType = "char" + array;
                }else if(jniSignature.startsWith('Z')){
                    returnType = "boolean" + array;
                }else if(jniSignature.startsWith('D')){
                    returnType = "double" + array;
                }else if(jniSignature.startsWith('F')){
                    returnType = "float" + array;
                }else if(jniSignature.startsWith('[')){
                    array += "[]";
                }
                jniSignature = jniSignature.mid(1);
            }
        }
    }
    return QString("%1 %2(%3)").arg(returnType, methodName, signature.join(", "));
}

QString getClassDisplayName(JNIEnv *env, jclass object_class)
{
    QString name = QtJambiAPI::getClassName(env, object_class);//.replace('$', '.')
    auto idx = name.indexOf('$');
    QString _name;
    while(idx>=0 && idx<name.length()){
        _name += name.left(idx);
        name = name.mid(idx+1);
        if(name[0]=='$'
                || (name[0]>='0' && name[0]<='9')){
            _name += '$';
        }else{
            _name += '.';
        }
        idx = name.indexOf('$');
    }
    _name += name;
    return _name;
}

QList<jmethodID> getMethodIDs(JNIEnv *env, jclass object_class, const std::type_info& typeId)
{
    bool is_interface = isInterface(typeId) || isFunctional(typeId);
    QList<jmethodID> methods;
    QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId);
    Q_ASSERT(typeEntry);
    QVector<RegistryAPI::FunctionInfo> virtualFcts;
    if(typeEntry->isShellable()){
        const QVector<RegistryAPI::FunctionInfo>& virtualFcts = static_cast<const ShellableTypeEntry*>(typeEntry.data())->virtualFunctions();
        for (const RegistryAPI::FunctionInfo& info : virtualFcts) {
            if (jmethodID method_id = env->GetMethodID(object_class, info.name, info.signature)) {
                if (jobject method_object = env->ToReflectedMethod(object_class, method_id, false)) {
                    if (Java::QtJambi::ClassAnalyzerUtility::isImplementedInJava(env,
                                                     jboolean(info.flags & RegistryAPI::FunctionInfo::Abstract),
                                                     method_object, typeEntry->javaClass())) {
                        if(info.flags & RegistryAPI::FunctionInfo::Private){
                            bool isPrivateOverride = Java::Runtime::AccessibleObject::isAnnotationPresent(env, method_object, Java::QtJambi::QtPrivateOverride::getClass(env));
                            if(!isPrivateOverride){
                                QString message(QLatin1String("Method '%1' in class %2 misses @QtPrivateOverride annotation."));
                                message = message.arg(jniSignatureToJava(QLatin1String(info.signature), QLatin1String(info.name)));
                                message = message.arg(getClassDisplayName(env, object_class));
                                Java::QtJambi::QMissingVirtualOverridingException::throwNew(env, message QTJAMBI_STACKTRACEINFO );
                            }
                        }
                        methods << method_id;
                    }else if(info.flags & RegistryAPI::FunctionInfo::Abstract){
                        QString message(QLatin1String("Class %1 does not implement pure virtual function '%2' from Qt declared in %3."));
                        message = message.arg(getClassDisplayName(env, object_class));
                        message = message.arg(jniSignatureToJava(QLatin1String(info.signature), QLatin1String(info.name)));
                        message = message.arg(QLatin1String(typeEntry->qtName()));
                        Java::QtJambi::QMissingVirtualOverridingException::throwNew(env, message QTJAMBI_STACKTRACEINFO );
                    }else{
                        methods << nullptr;
                    }
                    env->DeleteLocalRef(method_object);
                }else{
                    jthrowable thr = nullptr;
                    if(env->ExceptionCheck()){
                        thr = env->ExceptionOccurred();
                        env->ExceptionClear();
                    }
                    fprintf(stderr,
                            "vtable setup conversion to reflected method failed: %s::%s %s\n",
                            qPrintable(getClassDisplayName(env, object_class)), info.name, info.signature);
                    if(thr)
                        JavaException(env, thr).raise();
                    methods << nullptr;
                }
            }else{
                if(is_interface){
                    if(env->ExceptionCheck()){
                        env->ExceptionClear();
                    }
                    if(info.flags & RegistryAPI::FunctionInfo::Abstract){
                        QString message;
                        if(isFunctional(typeId)){
                            message = QLatin1String("Class %1 does not implement functional interface method '%2' from Qt declared in %3.");
                        }else{
                            message = QLatin1String("Class %1 does not implement pure virtual function '%2' from Qt declared in %3.");
                        }
                        message = message.arg(getClassDisplayName(env, object_class));
                        message = message.arg(jniSignatureToJava(QLatin1String(info.signature), QLatin1String(info.name)));
                        message = message.arg(QLatin1String(typeEntry->qtName()));
                        Java::QtJambi::QMissingVirtualOverridingException::throwNew(env, message QTJAMBI_STACKTRACEINFO );
                    }
                }else if(info.flags & RegistryAPI::FunctionInfo::Private){
                    if(env->ExceptionCheck()){
                        env->ExceptionClear();
                    }
                    QString message(QLatin1String("Class %1 does not implement private pure virtual function '%2' from Qt declared in %3."));
                    message = message.arg(getClassDisplayName(env, object_class));
                    message = message.arg(jniSignatureToJava(QLatin1String(info.signature), QLatin1String(info.name)));
                    message = message.arg(QLatin1String(typeEntry->qtName()));
                    Java::QtJambi::QMissingVirtualOverridingException::throwNew(env, message QTJAMBI_STACKTRACEINFO );
                }else{
                    jthrowable thr = nullptr;
                    if(env->ExceptionCheck()){
                        thr = env->ExceptionOccurred();
                        env->ExceptionClear();
                    }
                    fprintf(stderr, "vtable setup failed: %s::%s %s\n",
                            qPrintable(getClassDisplayName(env, object_class)), info.name, info.signature);
                    if(thr)
                        JavaException(env, thr).raise();
                }
                methods << nullptr;
            }
        }
    }
    return methods;
}

const QSharedPointer<const VTable> &QtJambiShellImpl::setupVTable(JNIEnv *env, jclass object_class, jobject object, const std::type_info& typeId, const SuperTypeInfos* superTypeInfos, const QMetaObject* originalMetaObject, bool hasCustomMetaObject, JavaException& ocurredException)
{
    if(!ocurredException){
        try {
            Q_UNUSED(typeId)

            Q_ASSERT(object);
            Q_ASSERT(object_class);

            int classHashCode = Java::Runtime::Object::hashCode(env,object_class);
            {
                QReadLocker locker(gTableLock());
                Q_UNUSED(locker)
                Q_ASSERT(functionTableCache());
                if(functionTableCache->contains(classHashCode))
                    return (*functionTableCache)[classHashCode];
            }

            VTable * table;
            if(superTypeInfos && !superTypeInfos->isEmpty()){
                QMap<std::type_index, QList<jmethodID>> methodsByType;
                for(const SuperTypeInfo& info : *superTypeInfos){
                    if(!methodsByType.contains(info.typeId())){
                        methodsByType.insert(info.typeId(), getMethodIDs(env, object_class, info.typeId()));
                    }
                }
                if(methodsByType.size()==1){
                    if(originalMetaObject){
                        const QMetaObject* metaObject = originalMetaObject;
                        if(!env->IsSameObject(superTypeInfos->first().javaClass(), object_class))
                            metaObject = CoreAPI::metaObjectForClass(env, object_class, metaObject, hasCustomMetaObject);
                        if(!metaObject){
                            table = new RegularVTable(env, object_class, methodsByType.first());
                        }else if(const QtJambiMetaObject* mo = QtJambiMetaObject::cast(metaObject)){
                            table = new DynamicMetaObjectVTable(env, object_class, methodsByType.first(), mo);
                        }else{
                            table = new MetaObjectVTable(env, object_class, methodsByType.first(), metaObject);
                        }
                    }else{
                        table = new RegularVTable(env, object_class, methodsByType.first());
                    }
                }else{
                    if(originalMetaObject){
                        const QMetaObject* metaObject = originalMetaObject;
                        if(!env->IsSameObject(superTypeInfos->first().javaClass(), object_class))
                            metaObject = CoreAPI::metaObjectForClass(env, object_class, metaObject, hasCustomMetaObject);
                        if(!metaObject){
                            table = new InterfaceVTable(env, object_class, methodsByType);
                        }else if(const QtJambiMetaObject* mo = QtJambiMetaObject::cast(metaObject)){
                            table = new DynamicMetaObjectInterfaceVTable(env, object_class, methodsByType, mo);
                        }else{
                            table = new MetaObjectInterfaceVTable(env, object_class, methodsByType, metaObject);
                        }
                    }else{
                        table = new InterfaceVTable(env, object_class, methodsByType);
                    }
                }
            }else{
                // this should be exactly the qt type, so no virtual overrides.
                if(originalMetaObject){
                    Q_ASSERT(!QtJambiMetaObject::isInstance(originalMetaObject));
                    table = new MetaObjectEmptyVTable(env, object_class, originalMetaObject);
                }else{
                    table = new EmptyVTable(env, object_class);
                }
            }
            QWriteLocker locker(gTableLock());
            Q_UNUSED(locker)
            Q_ASSERT(functionTableCache());
            if(!functionTableCache->contains(classHashCode))
                functionTableCache->insert(classHashCode, QSharedPointer<const VTable>(table));
            else
                delete table;
            return (*functionTableCache)[classHashCode];
        }catch(const JavaException& exn){
            ocurredException.addSuppressed(env, exn);
        }
    }
    QWriteLocker locker(gTableLock());
    Q_UNUSED(locker)
    Q_ASSERT(functionTableCache());
    return (*functionTableCache)[0];
}


void clearVTablesAtShutdown(){
    QHash<int, QSharedPointer<const VTable>> cache;
    {
        QWriteLocker locker(gTableLock());
        Q_UNUSED(locker)
        if(functionTableCache())
            functionTableCache->swap(cache);
    }
}
