/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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
#include "qtjambi_core.h"
#include <QtCore/private/qobject_p.h>
#include <QtCore/QThread>
#include "qtjambishell_p.h"
#include "qtjambifunctiontable_p.h"
#include "qtjambilink_p.h"
#include "qtjambi_interfaces_p.h"
#include "qtjambi_repository_p.h"
#include "qtjambi_registry_p.h"
#include "qtjambidebugtools_p.h"
#include "qtjambi_containers.h"
#include "qtjambi_containeraccess_p.h"

DestructorInfo::DestructorInfo() : ptr(nullptr),
    destructor(nullptr),
    m_typeId(nullptr),
    ownerFunction(nullptr)
{}

DestructorInfo::DestructorInfo(const DestructorInfo& info) : ptr(info.ptr),
    destructor(info.destructor),
    m_typeId(info.m_typeId),
    ownerFunction(info.ownerFunction)
{}

DestructorInfo::DestructorInfo(DestructorInfo&& info) : ptr(std::move(info.ptr)),
    destructor(std::move(info.destructor)),
    m_typeId(std::move(info.m_typeId)),
    ownerFunction(std::move(info.ownerFunction))
{}

DestructorInfo::DestructorInfo(
        void* _ptr,
        Destructor _destructor,
        const std::type_info& _typeId,
        PtrOwnerFunction _ownerFunction
    ) : ptr(_ptr),
    destructor(_destructor),
    m_typeId(&_typeId),
    ownerFunction(_ownerFunction)
{}

void DestructorInfo::swap(DestructorInfo& info)
{
   qSwap(ptr, info.ptr);
   qSwap(destructor, info.destructor);
   qSwap(m_typeId, info.m_typeId);
   qSwap(ownerFunction, info.ownerFunction);
}

DestructorInfo& DestructorInfo::operator=(const DestructorInfo& info)
{
   ptr = info.ptr;
   destructor = info.destructor;
   m_typeId = info.m_typeId;
   ownerFunction = info.ownerFunction;
   return *this;
}

DestructorInfo& DestructorInfo::operator=(DestructorInfo&& info)
{
    ptr = std::move(info.ptr);
    destructor = std::move(info.destructor);
    m_typeId = std::move(info.m_typeId);
    ownerFunction = std::move(info.ownerFunction);
    return *this;
}

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

void swap(DestructorInfo& a, DestructorInfo& b) noexcept{
    a.swap(b);
}

JNIEnv *qtjambi_current_environment(bool initializeJavaThread);

const QSharedPointer<const QtJambiFunctionTable>&qtjambi_setup_vtable(JNIEnv *, jclass, jobject, const std::type_info&, const SuperTypeInfos*, const QMetaObject*, JavaException& ocurredException);

const char *const QtJambiShellImpl::id = "QtJambiShell";

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
            if(JNIEnv *env = qtjambi_current_environment()){
                QTJAMBI_JNI_LOCAL_FRAME(env, 1000)
                if (jobject java_object = link->getJavaObjectLocalRef(env)) {
                    QString name = qtjambi_object_class_name(env, java_object);
                    if(name==className
                            || QString(name).replace(".", "::")==className
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
    const QMetaObject *meta_object = metaObject();
    if(const QtJambiMetaObject *dynamic_meta_object = QtJambiMetaObject::cast(meta_object)){
        if (QSharedPointer<QtJambiLink> __link = static_cast<const QtJambiShellImpl*>(this)->m_link) {
             if(JNIEnv *__jni_env = qtjambi_current_environment()){
                  QTJAMBI_JNI_LOCAL_FRAME(__jni_env, 1000)
                  jobject __obj = __link->getJavaObjectLocalRef(__jni_env);
                  switch (_c) {
                  case QMetaObject::CreateInstance:
                      _id = dynamic_meta_object->invokeConstructor(__jni_env, _id, _a); break;
                  case QMetaObject::InvokeMetaMethod:
                      _id = dynamic_meta_object->invokeSignalOrSlot(__jni_env, __obj, _id, _a); break;
                  case QMetaObject::ReadProperty:
                      _id = dynamic_meta_object->readProperty(__jni_env, __obj, _id, _a); break;
                  case QMetaObject::WriteProperty:
                      _id = dynamic_meta_object->writeProperty(__jni_env, __obj, _id, _a); break;
                  case QMetaObject::ResetProperty:
                      _id = dynamic_meta_object->resetProperty(__jni_env, __obj, _id, _a); break;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                  case QMetaObject::QueryPropertyDesignable:
                      _id = dynamic_meta_object->queryPropertyDesignable(__jni_env, __obj, _id, _a); break;
                  case QMetaObject::QueryPropertyScriptable:
                      _id = dynamic_meta_object->queryPropertyScriptable(__jni_env, __obj, _id, _a); break;
                  case QMetaObject::QueryPropertyUser:
                      _id = dynamic_meta_object->queryPropertyUser(__jni_env, __obj, _id, _a); break;
                  case QMetaObject::QueryPropertyStored:
                      _id = dynamic_meta_object->queryPropertyStored(__jni_env, __obj, _id, _a); break;
                  case QMetaObject::QueryPropertyEditable:
                      _id = dynamic_meta_object->queryPropertyEditable(__jni_env, __obj, _id, _a); break;
#else
                  case QMetaObject::BindableProperty:
                      _id = dynamic_meta_object->bindableProperty(__jni_env, __obj, _id, _a); break;
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
             }
        }
    }
    return _id;
}

QtJambiShellImpl::QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
  : m_ptr(ptr),
    m_vtable(qtjambi_setup_vtable(__jni_env, objectClass, __jni_object, typeId, superTypeInfos, nullptr, ocurredException)),
    m_link(QtJambiLink::createLinkForNewObject(__jni_env, objectClass, nativeLink,
                                               __jni_object, typeId, ptr, superTypeInfos,
                                               created_by_java, is_shell, destructor_function, ownerFunction, ocurredException))
{
}

QtJambiShellImpl::QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            PtrOwnerFunction ownerFunction, JavaException& ocurredException)
  : m_ptr(ptr),
    m_vtable(qtjambi_setup_vtable(__jni_env, objectClass, __jni_object, typeId, superTypeInfos, nullptr, ocurredException)),
    m_link(QtJambiLink::createLinkForNewObject(__jni_env, objectClass, nativeLink,
                                               __jni_object, typeId, ptr, superTypeInfos, metaType,
                                               created_by_java, is_shell, ownerFunction, ocurredException))
{
}

QtJambiShellImpl::QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            AbstractContainerAccess* containerAccess, JavaException& ocurredException)
  : m_ptr(ptr),
    m_vtable(qtjambi_setup_vtable(__jni_env, objectClass, __jni_object, typeId, superTypeInfos, nullptr, ocurredException)),
    m_link(QtJambiLink::createLinkForNewContainer(__jni_env, objectClass, nativeLink,
                                               __jni_object, typeId, ptr, superTypeInfos, metaType,
                                               created_by_java, is_shell, containerAccess, ocurredException))
{
}

QtJambiShellImpl::QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            PtrDeleterFunction destructor_function, JavaException& ocurredException)
  : m_ptr(ptr),
    m_vtable(qtjambi_setup_vtable(__jni_env, objectClass, __jni_object, typeId, superTypeInfos, nullptr, ocurredException)),
    m_link(QtJambiLink::createLinkForNewObject(__jni_env, objectClass, nativeLink,
                                               __jni_object, typeId, ptr, superTypeInfos,
                                               created_by_java, is_shell, destructor_function, ocurredException))
{
}

QtJambiShellImpl::QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            JavaException& ocurredException)
  : m_ptr(ptr),
    m_vtable(qtjambi_setup_vtable(__jni_env, objectClass, __jni_object, typeId, superTypeInfos, nullptr, ocurredException)),
    m_link(QtJambiLink::createLinkForNewObject(__jni_env, objectClass, nativeLink,
                                               __jni_object, typeId, ptr, superTypeInfos,
                                               created_by_java, is_shell, ocurredException))
{
}

QtJambiShellImpl::QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            JavaException& ocurredException)
  : m_ptr(ptr),
    m_vtable(qtjambi_setup_vtable(__jni_env, objectClass, __jni_object, typeId, superTypeInfos, nullptr, ocurredException)),
    m_link(QtJambiLink::createLinkForNewObject(__jni_env, objectClass, nativeLink,
                                               __jni_object, typeId, ptr, superTypeInfos, metaType,
                                               created_by_java, is_shell, ocurredException))
{
}

QtJambiShellImpl::QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QObject* ptr, const QMetaObject* originalMetaObject, bool created_by_java, bool isDeclarativeCall, bool is_shell, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException)
    : m_ptr(ptr),
      m_vtable(qtjambi_setup_vtable(__jni_env, objectClass, __jni_object, typeId, superTypeInfos, originalMetaObject, ocurredException)),
      m_link(QtJambiLink::createLinkForNewQObject(__jni_env, objectClass, nativeLink, __jni_object, typeId, m_vtable ? m_vtable->metaObject() : nullptr, ptr, superTypeInfos, created_by_java, isDeclarativeCall, is_shell, ocurredException))
{
}

QtJambiShell::QtJambiShell(){}

QtJambiShell::~QtJambiShell(){}

QtJambiShellInterface::~QtJambiShellInterface(){}

jobject QtJambiShellInterface::getJavaObjectLocalRef(JNIEnv *env, const QtJambiShellInterface* shellInterface){
    return shellInterface ? shellInterface->__shell()->getJavaObjectLocalRef(env) : nullptr;
}

QtJambiShellImpl::~QtJambiShellImpl()
{
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

void QtJambiShell::warnForMethod(const char* method) const
{
    if(QSharedPointer<QtJambiLink> lnk = static_cast<const QtJambiShellImpl*>(this)->m_link){
        qWarning("%s: The java object has been deleted prior to the native object [%s].", method, qPrintable(lnk->describe()));
    }else{
        qWarning("%s: The java object has been deleted prior to the native object [object link deleted].", method);
    }
}

void QtJambiShell::warnForMethod(const char* method, const QObject* object) const
{
    if(object){
        if(QSharedPointer<QtJambiLink> lnk = static_cast<const QtJambiShellImpl*>(this)->m_link){
            qWarning("%s: The java object has been deleted prior to the native object [class=\"%s\", objectName=\"%s\", %s].", method, qPrintable(object->metaObject()->className()), qPrintable(object->objectName()), qPrintable(lnk->describe()));
        }else{
            qWarning("%s: The java object has been deleted prior to the native object [class=\"%s\", objectName=\"%s\", object link deleted].", method, qPrintable(object->metaObject()->className()), qPrintable(object->objectName()));
        }
    }else{
        if(QSharedPointer<QtJambiLink> lnk = static_cast<const QtJambiShellImpl*>(this)->m_link){
            qWarning("%s: The java object has been deleted prior to the native object [%s].", method, qPrintable(lnk->describe()));
        }else{
            qWarning("%s: The java object has been deleted prior to the native object [object link deleted].", method);
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

void QtJambiShellImpl::overrideLink(const QSharedPointer<QtJambiLink>& link){
    m_link = link.toWeakRef();
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

const QMetaObject* QtJambiShell::metaObject() const
{
    Q_ASSERT(static_cast<const QtJambiShellImpl*>(this)->m_vtable);
    return static_cast<const QtJambiShellImpl*>(this)->m_vtable->metaObject();
}

void QtJambiShellImpl::deleteShell(){
#if defined(QTJAMBI_DEBUG_TOOLS) || !defined(QT_NO_DEBUG)
    const char* qtTypeName = nullptr;
#endif
    if(QSharedPointer<QtJambiLink> lnk = m_link){
#if defined(QTJAMBI_DEBUG_TOOLS) || !defined(QT_NO_DEBUG)
        qtTypeName = lnk->qtTypeName();
#endif
        lnk->invalidate(qtjambi_current_environment(lnk->noThreadInitializationOnPurge()));
    }
    operator delete(m_ptr);
    QTJAMBI_INCREASE_COUNTER2(shellDestroyedCount, qtTypeName)
    delete this;
}

SingleTypeShell::SingleTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            PtrOwnerFunction ownerFunction, JavaException& ocurredException)
  : QtJambiShellImpl(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, metaType, created_by_java, is_shell, superTypeInfos, ownerFunction, ocurredException)
#ifdef QT_DEBUG
    ,m_isAlive(false)
#endif
{
}

SingleTypeShell::SingleTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException)
  : QtJambiShellImpl(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, created_by_java, is_shell, superTypeInfos, destructor_function, ownerFunction, ocurredException)
#ifdef QT_DEBUG
    ,m_isAlive(false)
#endif
{
}

SingleTypeShell::SingleTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            JavaException& ocurredException)
  : QtJambiShellImpl(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, metaType, created_by_java, is_shell, superTypeInfos, ocurredException)
#ifdef QT_DEBUG
    ,m_isAlive(false)
#endif
{
}

SingleTypeShell::SingleTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            AbstractContainerAccess* containerAccess, JavaException& ocurredException)
  : QtJambiShellImpl(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, metaType, created_by_java, is_shell, superTypeInfos, containerAccess, ocurredException)
#ifdef QT_DEBUG
    ,m_isAlive(false)
#endif
{
}

SingleTypeShell::SingleTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            PtrDeleterFunction destructor_function, JavaException& ocurredException)
  : QtJambiShellImpl(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, created_by_java, is_shell, superTypeInfos, destructor_function, ocurredException)
#ifdef QT_DEBUG
    ,m_isAlive(false)
#endif
{
}

SingleTypeShell::SingleTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            JavaException& ocurredException)
  : QtJambiShellImpl(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, created_by_java, is_shell, superTypeInfos, ocurredException)
#ifdef QT_DEBUG
    ,m_isAlive(false)
#endif
{
}

SingleTypeShell::SingleTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QObject* ptr, const QMetaObject* originalMetaObject, bool created_by_java, bool isDeclarativeCall, bool is_shell, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException)
    : QtJambiShellImpl(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, originalMetaObject, created_by_java, isDeclarativeCall, is_shell, superTypeInfos, ocurredException)
#ifdef QT_DEBUG
    ,m_isAlive(false)
#endif
{
}

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

MultiTypeShell::MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
            AbstractContainerAccess* containerAccess, JavaException& ocurredException)
  : QtJambiShellImpl(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, metaType, created_by_java, is_shell, superTypeInfos, containerAccess, ocurredException),
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

MultiTypeShell::MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QObject* ptr, const QMetaObject* originalMetaObject, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool isDeclarativeCall, bool is_shell, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException)
    : QtJambiShellImpl(__jni_env, objectClass, nativeLink, __jni_object, typeId, ptr, originalMetaObject, created_by_java, isDeclarativeCall, is_shell, superTypeInfos, ocurredException),
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
    m_constructedTypes.remove(typeId.hash_code());
    if(QSharedPointer<QtJambiLink> lnk = link()){
        lnk->removeInterface(typeId);
        QTJAMBI_INCREASE_COUNTER(shellDestructorCalledCount, lnk)
    }
}

void MultiTypeShell::constructed(const std::type_info& typeId){
    m_constructedTypes << typeId.hash_code();
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
            QRecursiveMutexLocker locker(QtJambiLinkUserData::lock());
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
        for(DestructionHelper* destructionHelper : destructionHelpers){
            destructionHelper->deleteLater();
        }
    }
}

jobject getNativeLink(JNIEnv *env, jobject java);

void qtjambi_initialize_native_value(JNIEnv *env, jclass callingClass, jobject object,
                                      ConstructorFunction constructorFunction, size_t size, const std::type_info& typeId, bool isShell,
                                      PtrOwnerFunction ownerFunction, jvalue* arguments){
    jclass objectClass = env->GetObjectClass(object);
    jobject nativeLink = getNativeLink(env, object);
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
                SingleTypeShell* shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, true, isShell, nullptr, ownerFunction, ocurredException);
                if(ocurredException.object()){
                    delete shell;
                    ocurredException.raise();
                }
                *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                try{
                    constructorFunction(ptr, env, object, arguments);
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
                if(ocurredException.object()){
                    ocurredException.raise();
                }else{
                    try{
                        constructorFunction(ptr, env, object, arguments);
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
        SuperTypeInfos superTypeInfos = getSuperTypeInfos(env, objectClass);
        switch(superTypeInfos.size()){
        case 0:
            JavaException::raiseError(env, "Cannot determine type information about object's class." QTJAMBI_STACKTRACEINFO );
            break;
        case 1:
            Q_ASSERT(typeId == superTypeInfos.at(0).typeId());
            if(isShell){
                size_t offset = size;
                size += sizeof(QtJambiShell*);
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    SingleTypeShell* shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, true, isShell, &superTypeInfos, ownerFunction, ocurredException);
                    if(ocurredException.object()){
                        delete shell;
                        ocurredException.raise();
                    }
                    *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                    try{
                        constructorFunction(ptr, env, object, arguments);
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
                    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewObject(env, objectClass, nativeLink, object, typeId, ptr, &superTypeInfos, metaType,
                                                                                                  true, isShell, ownerFunction, ocurredException);
                    if(ocurredException.object()){
                        ocurredException.raise();
                    }else{
                        try{
                            constructorFunction(ptr, env, object, arguments);
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
            Q_ASSERT(typeId == superTypeInfos.at(0).typeId());
            if(isShell || isInterface(typeId)){
                size_t totalSize = 0;
                totalSize += size;
                totalSize += sizeof(QtJambiShell*);
                QHash<size_t,DestructorInfo> destructorInfo;

                QList<ConstructorFunction> constructorFunctions;
                constructorFunctions << constructorFunction;

                for(int i=1; i<superTypeInfos.size(); ++i){
                    const SuperTypeInfo& info = superTypeInfos[i];
                    if(info.constructorInfos().isEmpty()){
                        jthrowable t = Java::QtJambi::QInterfaceCannotBeSubclassedException::newInstance(env, info.javaClass());
                        JavaException(env, t).raise( QTJAMBI_STACKTRACEINFO_ENV(env) );
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
                            JavaException::raiseError(env, qPrintable( QString("Cannot initialize interface %1 without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.").arg(QString(info.className()).replace("/", ".").replace("$", ".")) ) QTJAMBI_STACKTRACEINFO );
                        }
                        constructorFunctions << foundConstructorFunction;
                    }
                }

                void* ptr = operator new(totalSize);
                memset(ptr, 0, totalSize);
                try{
                    for(const SuperTypeInfo& info : superTypeInfos){
                        void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                        destructorInfo[info.typeId().hash_code()] = DestructorInfo(iptr, info.destructor(), info.typeId(), info.ownerFunction());
                    }
                    JavaException ocurredException;
                    MultiTypeShell* shell = new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, destructorInfo, true, isShell, &superTypeInfos, ownerFunction, ocurredException);
                    if(ocurredException.object()){
                        delete shell;
                        ocurredException.raise();
                    }
                    for(const SuperTypeInfo& info : superTypeInfos){
                        *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + info.offset() + info.size()) = shell;
                    }
                    auto i=superTypeInfos.size()-1;
                    try{
                        for(; i>=0; --i){
                            const SuperTypeInfo& info = superTypeInfos[i];
                            jvalue* args = i==0 ? arguments : nullptr;
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            constructorFunctions.at(i)(iptr, env, object, args);
                            if(!info.hasShell())
                                shell->constructed(info.typeId());
                        }
                    }catch(const JavaException& exn){
                        for(auto j=i+1; j<superTypeInfos.size(); ++j){
                            const SuperTypeInfo& info = superTypeInfos[j];
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            qtjambi_suppress_exception(env, exn,
                                info.destructor()(iptr);
                            )
                            if(!info.hasShell())
                                shell->destructed(info.typeId());
                        }
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
                JavaException::raiseError(env, qPrintable( QString("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg(qtjambi_class_name(env, callingClass).replace("$", ".")) ) QTJAMBI_STACKTRACEINFO );
            }
            break;
        }
    }
}

void qtjambi_initialize_native_value(JNIEnv *env, jclass callingClass, jobject object,
                                      ConstructorFunction constructorFunction, size_t size, const std::type_info& typeId, bool isShell,
                                      jvalue* arguments){
    jclass objectClass = env->GetObjectClass(object);
    jobject nativeLink = getNativeLink(env, object);
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
                SingleTypeShell* shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, true, isShell, nullptr, ocurredException);
                if(ocurredException.object()){
                    delete shell;
                    ocurredException.raise();
                }
                *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                try{
                    constructorFunction(ptr, env, object, arguments);
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
                if(ocurredException.object()){
                    ocurredException.raise();
                }else{
                    try{
                        constructorFunction(ptr, env, object, arguments);
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
        SuperTypeInfos superTypeInfos = getSuperTypeInfos(env, objectClass);
        switch(superTypeInfos.size()){
        case 0:
            JavaException::raiseError(env, "Cannot determine type information about object's class." QTJAMBI_STACKTRACEINFO );
            break;
        case 1:
            Q_ASSERT(typeId == superTypeInfos.at(0).typeId());
            if(isShell){
                size_t offset = size;
                size += sizeof(QtJambiShell*);
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    SingleTypeShell* shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, true, isShell, &superTypeInfos, ocurredException);
                    if(ocurredException.object()){
                        delete shell;
                        ocurredException.raise();
                    }
                    *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                    try{
                        constructorFunction(ptr, env, object, arguments);
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
                    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewObject(env, objectClass, nativeLink, object, typeId, ptr, &superTypeInfos, metaType,
                                                                                                  true, isShell, ocurredException);
                    if(ocurredException.object()){
                        ocurredException.raise();
                    }else{
                        try{
                            constructorFunction(ptr, env, object, arguments);
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
            Q_ASSERT(typeId == superTypeInfos.at(0).typeId());
            if(isShell || isInterface(typeId)){
                size_t totalSize = 0;
                totalSize += size;
                totalSize += sizeof(QtJambiShell*);
                QHash<size_t,DestructorInfo> destructorInfo;

                QList<ConstructorFunction> constructorFunctions;
                constructorFunctions << constructorFunction;

                for(int i=1; i<superTypeInfos.size(); ++i){
                    const SuperTypeInfo& info = superTypeInfos[i];
                    if(info.constructorInfos().isEmpty()){
                        jthrowable t = Java::QtJambi::QInterfaceCannotBeSubclassedException::newInstance(env, info.javaClass());
                        JavaException(env, t).raise( QTJAMBI_STACKTRACEINFO_ENV(env) );
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
                            JavaException::raiseError(env, qPrintable( QString("Cannot initialize interface %1 without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.").arg(QString(info.className()).replace("/", ".").replace("$", ".")) ) QTJAMBI_STACKTRACEINFO );
                        }
                        constructorFunctions << foundConstructorFunction;
                    }
                }

                void* ptr = operator new(totalSize);
                memset(ptr, 0, totalSize);
                try{
                    for(const SuperTypeInfo& info : superTypeInfos){
                        void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                        destructorInfo[info.typeId().hash_code()] = DestructorInfo(iptr, info.destructor(), info.typeId(), info.ownerFunction());
                    }
                    JavaException ocurredException;
                    MultiTypeShell* shell = new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, destructorInfo, true, isShell, &superTypeInfos, ocurredException);
                    if(ocurredException.object()){
                        delete shell;
                        ocurredException.raise();
                    }
                    for(const SuperTypeInfo& info : superTypeInfos){
                        *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + info.offset() + info.size()) = shell;
                    }
                    auto i=superTypeInfos.size()-1;
                    try{
                        for(; i>=0; --i){
                            const SuperTypeInfo& info = superTypeInfos[i];
                            jvalue* args = i==0 ? arguments : nullptr;
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            constructorFunctions.at(i)(iptr, env, object, args);
                            if(!info.hasShell())
                                shell->constructed(info.typeId());
                        }
                    }catch(const JavaException& exn){
                        for(auto j=i+1; j<superTypeInfos.size(); ++j){
                            const SuperTypeInfo& info = superTypeInfos[j];
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            qtjambi_suppress_exception(env, exn,
                                info.destructor()(iptr);
                            )
                            if(!info.hasShell())
                                shell->destructed(info.typeId());
                        }
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
                JavaException::raiseError(env, qPrintable( QString("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg(qtjambi_class_name(env, callingClass).replace("$", ".")) ) QTJAMBI_STACKTRACEINFO );
            }
            break;
        }
    }
}

void qtjambi_initialize_native_container(JNIEnv *env, jclass callingClass, jobject object,
                                      ConstructorFunction constructorFunction, size_t size, const std::type_info& typeId, bool isShell,
                                      AbstractContainerAccess* containerAccess,
                                      jvalue* arguments){
    jclass objectClass = env->GetObjectClass(object);
    jobject nativeLink = getNativeLink(env, object);
    QMetaType metaType(registeredMetaTypeID(typeId));
    Q_ASSERT(metaType.isValid());
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
    Q_ASSERT(containerAccess);

    if(env->IsSameObject(objectClass, callingClass)){
        if(isShell){
            size_t offset = size;
            size += sizeof(QtJambiShell*);
            void* ptr = operator new(size);
            memset(ptr, 0, size);
            try{
                JavaException ocurredException;
                SingleTypeShell* shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, true, isShell, nullptr, containerAccess, ocurredException);
                if(ocurredException.object()){
                    delete shell;
                    ocurredException.raise();
                }
                *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                try{
                    constructorFunction(ptr, env, object, arguments);
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
                const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewContainer(env, objectClass, nativeLink, object, typeId, ptr, nullptr, metaType,
                                                                                              true, isShell, containerAccess, ocurredException);
                if(ocurredException.object()){
                    ocurredException.raise();
                }else{
                    try{
                        constructorFunction(ptr, env, object, arguments);
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
        SuperTypeInfos superTypeInfos = getSuperTypeInfos(env, objectClass);
        switch(superTypeInfos.size()){
        case 0:
            JavaException::raiseError(env, "Cannot determine type information about object's class." QTJAMBI_STACKTRACEINFO );
            break;
        case 1:
            Q_ASSERT(typeId == superTypeInfos.at(0).typeId());
            if(isShell){
                size_t offset = size;
                size += sizeof(QtJambiShell*);
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    SingleTypeShell* shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, true, isShell, &superTypeInfos, containerAccess, ocurredException);
                    if(ocurredException.object()){
                        delete shell;
                        ocurredException.raise();
                    }
                    *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                    try{
                        constructorFunction(ptr, env, object, arguments);
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
                    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewContainer(env, objectClass, nativeLink, object, typeId, ptr, &superTypeInfos, metaType,
                                                                                                  true, isShell, containerAccess, ocurredException);
                    if(ocurredException.object()){
                        ocurredException.raise();
                    }else{
                        try{
                            constructorFunction(ptr, env, object, arguments);
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
            Q_ASSERT(typeId == superTypeInfos.at(0).typeId());
            if(isShell || isInterface(typeId)){
                size_t totalSize = 0;
                totalSize += size;
                totalSize += sizeof(QtJambiShell*);
                QHash<size_t,DestructorInfo> destructorInfo;

                QList<ConstructorFunction> constructorFunctions;
                constructorFunctions << constructorFunction;

                for(int i=1; i<superTypeInfos.size(); ++i){
                    const SuperTypeInfo& info = superTypeInfos[i];
                    if(info.constructorInfos().isEmpty()){
                        jthrowable t = Java::QtJambi::QInterfaceCannotBeSubclassedException::newInstance(env, info.javaClass());
                        JavaException(env, t).raise( QTJAMBI_STACKTRACEINFO_ENV(env) );
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
                            JavaException::raiseError(env, qPrintable( QString("Cannot initialize interface %1 without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.").arg(QString(info.className()).replace("/", ".").replace("$", ".")) ) QTJAMBI_STACKTRACEINFO );
                        }
                        constructorFunctions << foundConstructorFunction;
                    }
                }

                void* ptr = operator new(totalSize);
                memset(ptr, 0, totalSize);
                try{
                    for(const SuperTypeInfo& info : superTypeInfos){
                        void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                        destructorInfo[info.typeId().hash_code()] = DestructorInfo(iptr, info.destructor(), info.typeId(), info.ownerFunction());
                    }
                    JavaException ocurredException;
                    MultiTypeShell* shell = new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, destructorInfo, true, isShell, &superTypeInfos, containerAccess, ocurredException);
                    if(ocurredException.object()){
                        delete shell;
                        ocurredException.raise();
                    }
                    for(const SuperTypeInfo& info : superTypeInfos){
                        *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + info.offset() + info.size()) = shell;
                    }
                    auto i=superTypeInfos.size()-1;
                    try{
                        for(; i>=0; --i){
                            const SuperTypeInfo& info = superTypeInfos[i];
                            jvalue* args = i==0 ? arguments : nullptr;
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            constructorFunctions.at(i)(iptr, env, object, args);
                            if(!info.hasShell())
                                shell->constructed(info.typeId());
                        }
                    }catch(const JavaException& exn){
                        for(auto j=i+1; j<superTypeInfos.size(); ++j){
                            const SuperTypeInfo& info = superTypeInfos[j];
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            qtjambi_suppress_exception(env, exn,
                                info.destructor()(iptr);
                            )
                            if(!info.hasShell())
                                shell->destructed(info.typeId());
                        }
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
                JavaException::raiseError(env, qPrintable( QString("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg(qtjambi_class_name(env, callingClass).replace("$", ".")) ) QTJAMBI_STACKTRACEINFO );
            }
            break;
        }
    }
}

void qtjambi_initialize_native_object(JNIEnv *env, jclass callingClass, jobject object,
                                      ConstructorFunction constructorFunction, size_t size, const std::type_info& typeId, bool isShell,
                                      PtrDeleterFunction deleter_function, PtrOwnerFunction ownerFunction, jvalue* arguments){
    jclass objectClass = env->GetObjectClass(object);
    jobject nativeLink = getNativeLink(env, object);
    if(env->IsSameObject(objectClass, callingClass)){
        if(isShell){
            size_t offset = size;
            size += sizeof(QtJambiShell*);
            void* ptr = operator new(size);
            memset(ptr, 0, size);
            try{
                JavaException ocurredException;
                SingleTypeShell* shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, isShell, nullptr, deleter_function, ownerFunction, ocurredException);
                if(ocurredException.object()){
                    delete shell;
                    ocurredException.raise();
                }
                *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                try{
                    constructorFunction(ptr, env, object, arguments);
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
                if(ocurredException.object()){
                    ocurredException.raise();
                }else{
                    try{
                        constructorFunction(ptr, env, object, arguments);
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
        SuperTypeInfos superTypeInfos = getSuperTypeInfos(env, objectClass);
        switch(superTypeInfos.size()){
        case 0:
            JavaException::raiseError(env, "Cannot determine type information about object's class." QTJAMBI_STACKTRACEINFO );
            break;
        case 1:
            Q_ASSERT(typeId == superTypeInfos.at(0).typeId());
            if(isShell){
                size_t offset = size;
                size += sizeof(QtJambiShell*);
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    SingleTypeShell* shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, isShell, &superTypeInfos, deleter_function, ownerFunction, ocurredException);
                    if(ocurredException.object()){
                        delete shell;
                        ocurredException.raise();
                    }
                    *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                    try{
                        constructorFunction(ptr, env, object, arguments);
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
                    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewObject(env, objectClass, nativeLink, object, typeId, ptr, &superTypeInfos,
                                                                                                  true, isShell, deleter_function, ownerFunction, ocurredException);
                    if(ocurredException.object()){
                        ocurredException.raise();
                    }else{
                        try{
                            constructorFunction(ptr, env, object, arguments);
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
            Q_ASSERT(typeId == superTypeInfos.at(0).typeId());
            if(isShell || isInterface(typeId)){
                size_t totalSize = 0;
                totalSize += size;
                totalSize += sizeof(QtJambiShell*);
                QHash<size_t,DestructorInfo> destructorInfo;

                QList<ConstructorFunction> constructorFunctions;
                constructorFunctions << constructorFunction;

                for(int i=1; i<superTypeInfos.size(); ++i){
                    const SuperTypeInfo& info = superTypeInfos[i];
                    if(info.constructorInfos().isEmpty()){
                        jthrowable t = Java::QtJambi::QInterfaceCannotBeSubclassedException::newInstance(env, info.javaClass());
                        JavaException(env, t).raise( QTJAMBI_STACKTRACEINFO_ENV(env) );
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
                            JavaException::raiseError(env, qPrintable( QString("Cannot initialize interface %1 without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.").arg(QString(info.className()).replace("/", ".").replace("$", ".")) ) QTJAMBI_STACKTRACEINFO );
                        }
                        constructorFunctions << foundConstructorFunction;
                    }
                }

                void* ptr = operator new(totalSize);
                memset(ptr, 0, totalSize);
                try{
                    for(const SuperTypeInfo& info : superTypeInfos){
                        void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                        destructorInfo[info.typeId().hash_code()] = DestructorInfo(iptr, info.destructor(), info.typeId(), info.ownerFunction());
                    }
                    JavaException ocurredException;
                    MultiTypeShell* shell = new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, destructorInfo, true, isShell, &superTypeInfos, deleter_function, ownerFunction, ocurredException);
                    if(ocurredException.object()){
                        delete shell;
                        ocurredException.raise();
                    }
                    for(const SuperTypeInfo& info : superTypeInfos){
                        *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + info.offset() + info.size()) = shell;
                    }
                    auto i=superTypeInfos.size()-1;
                    try{
                        for(; i>=0; --i){
                            const SuperTypeInfo& info = superTypeInfos[i];
                            jvalue* args = i==0 ? arguments : nullptr;
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            constructorFunctions.at(i)(iptr, env, object, args);
                            if(!info.hasShell())
                                shell->constructed(info.typeId());
                        }
                    }catch(const JavaException& exn){
                        for(auto j=i+1; j<superTypeInfos.size(); ++j){
                            const SuperTypeInfo& info = superTypeInfos[j];
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            qtjambi_suppress_exception(env, exn,
                                info.destructor()(iptr);
                            )
                            if(!info.hasShell())
                                shell->destructed(info.typeId());
                        }
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
                JavaException::raiseError(env, qPrintable( QString("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg(qtjambi_class_name(env, callingClass).replace("$", ".")) ) QTJAMBI_STACKTRACEINFO );
            }
            break;
        }
    }
}

void qtjambi_initialize_native_object(JNIEnv *env, jclass callingClass, jobject object,
                                      ConstructorFunction constructorFunction, size_t size, const std::type_info& typeId, bool isShell,
                                      PtrDeleterFunction deleter_function, jvalue* arguments){
    jclass objectClass = env->GetObjectClass(object);
    jobject nativeLink = getNativeLink(env, object);
    if(env->IsSameObject(objectClass, callingClass)){
        if(isShell){
            size_t offset = size;
            size += sizeof(QtJambiShell*);
            void* ptr = operator new(size);
            memset(ptr, 0, size);
            try{
                JavaException ocurredException;
                SingleTypeShell* shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, isShell, nullptr, deleter_function, ocurredException);
                if(ocurredException.object()){
                    delete shell;
                    ocurredException.raise();
                }
                *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                try{
                    constructorFunction(ptr, env, object, arguments);
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
                if(ocurredException.object()){
                    ocurredException.raise();
                }else{
                    try{
                        constructorFunction(ptr, env, object, arguments);
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
        SuperTypeInfos superTypeInfos = getSuperTypeInfos(env, objectClass);
        switch(superTypeInfos.size()){
        case 0:
            JavaException::raiseError(env, "Cannot determine type information about object's class." QTJAMBI_STACKTRACEINFO );
            break;
        case 1:
            Q_ASSERT(typeId == superTypeInfos.at(0).typeId());
            if(isShell){
                size_t offset = size;
                size += sizeof(QtJambiShell*);
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    SingleTypeShell* shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, isShell, &superTypeInfos, deleter_function, ocurredException);
                    if(ocurredException.object()){
                        delete shell;
                        ocurredException.raise();
                    }
                    *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                    try{
                        constructorFunction(ptr, env, object, arguments);
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
                    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewObject(env, objectClass, nativeLink, object, typeId, ptr, &superTypeInfos,
                                                                                                  true, isShell, deleter_function, ocurredException);
                    if(ocurredException.object()){
                        ocurredException.raise();
                    }else{
                        try{
                            constructorFunction(ptr, env, object, arguments);
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
            Q_ASSERT(typeId == superTypeInfos.at(0).typeId());
            if(isShell || isInterface(typeId)){
                size_t totalSize = 0;
                totalSize += size;
                totalSize += sizeof(QtJambiShell*);
                QHash<size_t,DestructorInfo> destructorInfo;

                QList<ConstructorFunction> constructorFunctions;
                constructorFunctions << constructorFunction;

                for(int i=1; i<superTypeInfos.size(); ++i){
                    const SuperTypeInfo& info = superTypeInfos[i];
                    if(info.constructorInfos().isEmpty()){
                        jthrowable t = Java::QtJambi::QInterfaceCannotBeSubclassedException::newInstance(env, info.javaClass());
                        JavaException(env, t).raise( QTJAMBI_STACKTRACEINFO_ENV(env) );
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
                            JavaException::raiseError(env, qPrintable( QString("Cannot initialize interface %1 without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.").arg(QString(info.className()).replace("/", ".").replace("$", ".")) ) QTJAMBI_STACKTRACEINFO );
                        }
                        constructorFunctions << foundConstructorFunction;
                    }
                }

                void* ptr = operator new(totalSize);
                memset(ptr, 0, totalSize);
                try{
                    for(const SuperTypeInfo& info : superTypeInfos){
                        void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                        destructorInfo[info.typeId().hash_code()] = DestructorInfo(iptr, info.destructor(), info.typeId(), info.ownerFunction());
                    }
                    JavaException ocurredException;
                    MultiTypeShell* shell = new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, destructorInfo, true, isShell, &superTypeInfos, deleter_function, ocurredException);
                    if(ocurredException.object()){
                        delete shell;
                        ocurredException.raise();
                    }
                    for(const SuperTypeInfo& info : superTypeInfos){
                        *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + info.offset() + info.size()) = shell;
                    }
                    auto i=superTypeInfos.size()-1;
                    try{
                        for(; i>=0; --i){
                            const SuperTypeInfo& info = superTypeInfos[i];
                            jvalue* args = i==0 ? arguments : nullptr;
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            constructorFunctions.at(i)(iptr, env, object, args);
                            if(!info.hasShell())
                                shell->constructed(info.typeId());
                        }
                    }catch(const JavaException& exn){
                        for(auto j=i+1; j<superTypeInfos.size(); ++j){
                            const SuperTypeInfo& info = superTypeInfos[j];
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            qtjambi_suppress_exception(env, exn,
                                info.destructor()(iptr);
                            )
                            if(!info.hasShell())
                                shell->destructed(info.typeId());
                        }
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
                JavaException::raiseError(env, qPrintable( QString("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg(qtjambi_class_name(env, callingClass).replace("$", ".")) ) QTJAMBI_STACKTRACEINFO );
            }
            break;
        }
    }
}

void qtjambi_initialize_native_functional(JNIEnv *env, jclass callingClass, jobject object,
                                      ConstructorFunction constructorFunction, size_t size, const std::type_info& typeId, PtrDeleterFunction delete_function){
    jclass objectClass = env->GetObjectClass(object);
    jobject nativeLink = getNativeLink(env, object);
    if(env->IsSameObject(objectClass, callingClass)){
        size_t offset = size;
        size += sizeof(QtJambiShell*);
        void* ptr = operator new(size);
        memset(ptr, 0, size);
        try{
            JavaException ocurredException;
            SingleTypeShell* shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, true, nullptr, delete_function, ocurredException);
            if(ocurredException.object()){
                delete shell;
                ocurredException.raise();
            }
            *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
            try{
                constructorFunction(ptr, env, object, nullptr);
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
        SuperTypeInfos superTypeInfos = getSuperTypeInfos(env, objectClass);
        switch(superTypeInfos.size()){
        case 0:
            JavaException::raiseError(env, "Cannot determine type information about object's class." QTJAMBI_STACKTRACEINFO );
            break;
        case 1:{
                Q_ASSERT(typeId == superTypeInfos.at(0).typeId());
                size_t offset = size;
                size += sizeof(QtJambiShell*);
                void* ptr = operator new(size);
                memset(ptr, 0, size);
                try{
                    JavaException ocurredException;
                    SingleTypeShell* shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, true, &superTypeInfos, delete_function, ocurredException);
                    if(ocurredException.object()){
                        delete shell;
                        ocurredException.raise();
                    }
                    *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                    try{
                        constructorFunction(ptr, env, object, nullptr);
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
            Q_ASSERT(typeId == superTypeInfos.at(0).typeId());
            {
                size_t totalSize = 0;
                totalSize += size;
                totalSize += sizeof(QtJambiShell*);
                QHash<size_t,DestructorInfo> destructorInfo;

                QList<ConstructorFunction> constructorFunctions;
                constructorFunctions << constructorFunction;

                for(int i=1; i<superTypeInfos.size(); ++i){
                    const SuperTypeInfo& info = superTypeInfos[i];
                    if(info.constructorInfos().isEmpty()){
                        jthrowable t = Java::QtJambi::QInterfaceCannotBeSubclassedException::newInstance(env, info.javaClass());
                        JavaException(env, t).raise( QTJAMBI_STACKTRACEINFO_ENV(env) );
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
                            JavaException::raiseError(env, qPrintable( QString("Cannot initialize interface %1 without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.").arg(QString(info.className()).replace("/", ".").replace("$", ".")) ) QTJAMBI_STACKTRACEINFO );
                        }
                        constructorFunctions << foundConstructorFunction;
                    }
                }

                void* ptr = operator new(totalSize);
                memset(ptr, 0, totalSize);
                try{
                    for(const SuperTypeInfo& info : superTypeInfos){
                        void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                        destructorInfo[info.typeId().hash_code()] = DestructorInfo(iptr, info.destructor(), info.typeId(), info.ownerFunction());
                    }
                    JavaException ocurredException;
                    MultiTypeShell* shell = new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, destructorInfo, true, true, &superTypeInfos, delete_function, ocurredException);
                    if(ocurredException.object()){
                        delete shell;
                        ocurredException.raise();
                    }
                    for(const SuperTypeInfo& info : superTypeInfos){
                        *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + info.offset() + info.size()) = shell;
                    }
                    auto i=superTypeInfos.size()-1;
                    try{
                        for(; i>=0; --i){
                            const SuperTypeInfo& info = superTypeInfos[i];
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            constructorFunctions.at(i)(iptr, env, object, nullptr);
                            if(!info.hasShell())
                                shell->constructed(info.typeId());
                        }
                    }catch(const JavaException& exn){
                        for(auto j=i+1; j<superTypeInfos.size(); ++j){
                            const SuperTypeInfo& info = superTypeInfos[j];
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            qtjambi_suppress_exception(env, exn,
                                info.destructor()(iptr);
                            )
                            if(!info.hasShell())
                                shell->destructed(info.typeId());
                        }
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
        }
    }
}

void * qtjambi_get_placement(JNIEnv *env, jclass callingClass, jobject placement_object);

void qtjambi_register_qobject_initialization(void *ptr, const QSharedPointer<QtJambiLink>& link);
void qtjambi_unregister_qobject_initialization(void *ptr);

void qtjambi_initialize_native_qobject(JNIEnv *env, jclass callingClass, jobject object, ConstructorFunction constructorFunction, size_t size, const std::type_info& typeId, const QMetaObject& originalMetaObject, bool isShell, bool isDeclarativeCall, jvalue* arguments){
    try{
        jclass objectClass = env->GetObjectClass(object);
        jobject nativeLink = getNativeLink(env, object);
        if(env->IsSameObject(objectClass, callingClass)){
            if(isShell){
                size_t offset = size;
                size += sizeof(QtJambiShell*);
                void* ptr;
                if(isDeclarativeCall){
                    Q_ASSERT(arguments);
                    ptr = qtjambi_get_placement(env, callingClass, arguments->l);
                }else{
                    ptr = operator new(size);
                    memset(ptr, 0, size);
                }
                try{
                    JavaException ocurredException;
                    SingleTypeShell* shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QObject*>(ptr), &originalMetaObject, true, isDeclarativeCall, isShell, nullptr, ocurredException);
                    if(ocurredException.object()){
                        delete shell;
                        ocurredException.raise();
                        return;
                    }
                    *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                    qtjambi_register_qobject_initialization(ptr, shell->link());
                    try{
                        constructorFunction(ptr, env, object, arguments);
                        qtjambi_unregister_qobject_initialization(ptr);
                    }catch(...){
                        qtjambi_unregister_qobject_initialization(ptr);
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
                    ptr = qtjambi_get_placement(env, callingClass, arguments->l);
                }else{
                    ptr = operator new(size);
                    memset(ptr, 0, size);
                }
                try{
                    JavaException ocurredException;
                    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewQObject(env, objectClass, nativeLink, object, typeId, &originalMetaObject, reinterpret_cast<QObject*>(ptr), nullptr, true, isDeclarativeCall, isShell, ocurredException);
                    if(ocurredException.object()){
                        ocurredException.raise();
                    }else{
                        try{
                            qtjambi_register_qobject_initialization(ptr, link);
                            constructorFunction(ptr, env, object, arguments);
                            qtjambi_unregister_qobject_initialization(ptr);
                        }catch(...){
                            qtjambi_unregister_qobject_initialization(ptr);
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
            SuperTypeInfos superTypeInfos = getSuperTypeInfos(env, objectClass);
            switch(superTypeInfos.size()){
            case 0:
                JavaException::raiseError(env, "Cannot determine type information about object's class." QTJAMBI_STACKTRACEINFO );
                break;
            case 1:
                Q_ASSERT(typeId == superTypeInfos.at(0).typeId());
                if(isShell){
                    size_t offset = size;
                    size += sizeof(QtJambiShell*);
                    void* ptr;
                    if(isDeclarativeCall){
                        Q_ASSERT(arguments);
                        ptr = qtjambi_get_placement(env, callingClass, arguments->l);
                    }else{
                        ptr = operator new(size);
                        memset(ptr, 0, size);
                    }
                    try{
                        JavaException ocurredException;
                        SingleTypeShell* shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QObject*>(ptr), &originalMetaObject, true, isDeclarativeCall, isShell, &superTypeInfos, ocurredException);
                        if(ocurredException.object()){
                            delete shell;
                            ocurredException.raise();
                        }
                        *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + offset) = shell;
                        try{
                            qtjambi_register_qobject_initialization(ptr, shell->link());
                            constructorFunction(ptr, env, object, arguments);
                            qtjambi_unregister_qobject_initialization(ptr);
                        }catch(...){
                            qtjambi_unregister_qobject_initialization(ptr);
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
                        ptr = qtjambi_get_placement(env, callingClass, arguments->l);
                    }else{
                        ptr = operator new(size);
                        memset(ptr, 0, size);
                    }
                    try{
                        JavaException ocurredException;
                        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNewQObject(env, objectClass, nativeLink, object, typeId, &originalMetaObject, reinterpret_cast<QObject*>(ptr), &superTypeInfos, true, isDeclarativeCall, isShell, ocurredException);
                        if(ocurredException.object()){
                            ocurredException.raise();
                        }else{
                            try{
                                qtjambi_register_qobject_initialization(ptr, link);
                                constructorFunction(ptr, env, object, arguments);
                                qtjambi_unregister_qobject_initialization(ptr);
                            }catch(...){
                                qtjambi_unregister_qobject_initialization(ptr);
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
                Q_ASSERT(typeId == superTypeInfos.at(0).typeId());
                if(isShell){
                    size_t totalSize = 0;
                    totalSize += size;
                    totalSize += sizeof(QtJambiShell*);
                    QHash<size_t,DestructorInfo> destructorInfo;

                    QList<ConstructorFunction> constructorFunctions;
                    constructorFunctions << constructorFunction;

                    for(int i=1; i<superTypeInfos.size(); ++i){
                        const SuperTypeInfo& info = superTypeInfos[i];
                        if(info.constructorInfos().isEmpty()){
                            jthrowable t = Java::QtJambi::QInterfaceCannotBeSubclassedException::newInstance(env, info.javaClass());
                            JavaException(env, t).raise( QTJAMBI_STACKTRACEINFO_ENV(env) );
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
                                JavaException::raiseError(env, qPrintable( QString("Cannot initialize interface %1 without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.").arg(QString(info.className()).replace("/", ".").replace("$", ".")) ) QTJAMBI_STACKTRACEINFO );
                                return;
                            }
                            constructorFunctions << foundConstructorFunction;
                        }
                    }

                    void* ptr;
                    if(isDeclarativeCall){
                        Q_ASSERT(arguments);
                        ptr = qtjambi_get_placement(env, callingClass, arguments->l);
                    }else{
                        ptr = operator new(totalSize);
                        memset(ptr, 0, totalSize);
                    }
                    try{
                        for(const SuperTypeInfo& info : superTypeInfos){
                            void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                            destructorInfo[info.typeId().hash_code()] = DestructorInfo(iptr, info.destructor(), info.typeId(), info.ownerFunction());
                        }
                        JavaException ocurredException;
                        MultiTypeShell* shell = new MultiTypeShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QObject*>(ptr), &originalMetaObject, destructorInfo, true, isDeclarativeCall, isShell, &superTypeInfos, ocurredException);
                        if(ocurredException.object()){
                            delete shell;
                            ocurredException.raise();
                        }
                        for(const SuperTypeInfo& info : superTypeInfos){
                            *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + info.offset() + info.size()) = shell;
                        }
                        auto i=superTypeInfos.size()-1;
                        try{
                            qtjambi_register_qobject_initialization(ptr, shell->link());
                            for(; i>=0; --i){
                                const SuperTypeInfo& info = superTypeInfos[i];
                                jvalue* args = i==0 ? arguments : nullptr;
                                void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                                constructorFunctions.at(i)(iptr, env, object, args);
                                if(!info.hasShell())
                                    shell->constructed(info.typeId());
                            }
                            qtjambi_unregister_qobject_initialization(ptr);
                        }catch(const JavaException& exn){
                            for(auto j=i+1; j<superTypeInfos.size(); ++j){
                                const SuperTypeInfo& info = superTypeInfos[j];
                                void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                                qtjambi_suppress_exception(env, exn,
                                    info.destructor()(iptr);
                                )
                                if(!info.hasShell())
                                    shell->destructed(info.typeId());
                            }
                            qtjambi_unregister_qobject_initialization(ptr);
                            if(QSharedPointer<QtJambiLink> link = shell->link()){
                                link->invalidate(env);
                            }
                            delete shell;
                            throw;
                        }
                        shell->init(env);
                    }catch(...){
                        if(isDeclarativeCall)
                            operator delete(ptr);
                        throw;
                    }
                }else{
                    JavaException::raiseError(env, qPrintable( QString("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg(qtjambi_class_name(env, callingClass).replace("$", ".")) ) QTJAMBI_STACKTRACEINFO );
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
        throw exn;
    }
}

void qtjambi_initialize_native_interface(JNIEnv *env, jclass callingClass, jobject object, jobject nativeLink, jobject arguments){
    if(nativeLink){
        jclass objectClass = env->GetObjectClass(object);
        SuperTypeInfos superTypeInfos = getSuperTypeInfos(env, objectClass);
        if(superTypeInfos.isEmpty())
            JavaException::raiseError(env, "Cannot determine type information about object's class." QTJAMBI_STACKTRACEINFO );
        if(superTypeInfos.size()>1 && !(isInterface(superTypeInfos.at(0).typeId()) || superTypeInfos.at(0).hasShell())){
            JavaException::raiseError(env, qPrintable( QString("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg(QString(superTypeInfos.at(0).className()).replace("/", ".").replace("$", ".")) ) QTJAMBI_STACKTRACEINFO );
        }
        size_t totalSize = 0;
        QList<ConstructorFunction> constructorFunctions;
        QList<QVector<jvalue>> constructorArguments;
        QHash<size_t,DestructorInfo> destructorInfo;
        jobject declarativeCall = nullptr;
        for(int s=0; s<superTypeInfos.size(); ++s){
            const SuperTypeInfo& info = superTypeInfos.at(s);
            QList<jclass> givenArgumentTypes;
            QVector<jvalue> givenArguments;
            QString className = qtjambi_class_name(env, info.javaClass());
            jobject argumentList = arguments ? Java::Runtime::Map::get(env, arguments, info.javaClass()) : nullptr;
            if(!env->IsSameObject(argumentList, nullptr)){
                givenArguments.reserve(qtjambi_collection_size(env, argumentList));
                jobject iter = qtjambi_collection_iterator(env, argumentList);
                while(qtjambi_iterator_has_next(env, iter)){
                    jobject currentArg = qtjambi_iterator_next(env, iter);
                    jclass givenArgumentType = Java::QtJambi::QtArgument$Stream$Arg::type(env, currentArg);
                    jobject givenArgument = Java::QtJambi::QtArgument$Stream$Arg::value(env, currentArg);
                    givenArgumentTypes << givenArgumentType;
                    jvalue value;
                    if(Java::Runtime::Integer::isPrimitiveType(env,givenArgumentType)){
                        value.i = qtjambi_to_int(env, givenArgument);
                    }else if(Java::Runtime::Long::isPrimitiveType(env, givenArgumentType)){
                        value.j = qtjambi_to_long(env, givenArgument);
                    }else if(Java::Runtime::Short::isPrimitiveType(env, givenArgumentType)){
                        value.s = qtjambi_to_short(env, givenArgument);
                    }else if(Java::Runtime::Byte::isPrimitiveType(env, givenArgumentType)){
                        value.b = qtjambi_to_byte(env, givenArgument);
                    }else if(Java::Runtime::Double::isPrimitiveType(env, givenArgumentType)){
                        value.d = qtjambi_to_double(env, givenArgument);
                    }else if(Java::Runtime::Float::isPrimitiveType(env, givenArgumentType)){
                        value.f = qtjambi_to_float(env, givenArgument);
                    }else if(Java::Runtime::Character::isPrimitiveType(env, givenArgumentType)){
                        value.c = qtjambi_to_char(env, givenArgument);
                    }else if(Java::Runtime::Boolean::isPrimitiveType(env, givenArgumentType)){
                        value.b = qtjambi_to_boolean(env, givenArgument);
                    }else{
                        value.l = givenArgument;
                    }
                    givenArguments << value;
                }
            }
            ConstructorFunction foundConstructorFunction = nullptr;
            for(int j=0; j<info.constructorInfos().size(); ++j){
                const ResolvedConstructorInfo & constructorInfo = info.constructorInfos().at(j);
                if(constructorInfo.argumentTypes.size()==givenArgumentTypes.size()){
                    bool matches = true;
                    for(int a=0; a<constructorInfo.argumentTypes.size(); ++a){
                        if(!env->IsAssignableFrom(givenArgumentTypes[a], constructorInfo.argumentTypes[a])){
                            matches = false;
                            break;
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
                for(jclass cls : givenArgumentTypes){
                    if(!argumentStrg.isEmpty())
                        argumentStrg += ", ";
                    QString className = qtjambi_class_name(env, cls).replace("$", ".");
                    if(className.endsWith(";"))
                        className.chop(1);
                    while(className.startsWith("[")){
                        className = className.mid(1);
                        className += "[]";
                    }
                    argumentStrg += className;
                }
                JavaException::raiseIllegalArgumentException(env, qPrintable( QString("Cannot find constructor %1(%2).").arg(QString(info.className()).replace("/", ".").replace("$", ".")).arg(qPrintable(argumentStrg)) ) QTJAMBI_STACKTRACEINFO );
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
            ptr = qtjambi_get_placement(env, callingClass, declarativeCall);
        }else{
            ptr = operator new(totalSize);
            memset(ptr, 0, totalSize);
        }
        try{
            for(const SuperTypeInfo& info : superTypeInfos){
                void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                destructorInfo[info.typeId().hash_code()] = DestructorInfo(iptr, info.destructor(), info.typeId(), info.ownerFunction());
            }
            QtJambiShellImpl* shell;
            Q_ASSERT(!superTypeInfos.isEmpty());
            const std::type_info& typeId = superTypeInfos[0].typeId();
            JavaException ocurredException;
            if(Java::QtCore::QObject::isInstanceOf(env, object)){
                if(superTypeInfos.size()==1){
                    shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QObject*>(ptr), registeredOriginalMetaObject(typeId), true, false, true, &superTypeInfos, ocurredException);
                }else{
                    shell = new MultiTypeShell(env, objectClass, nativeLink, object, typeId, reinterpret_cast<QObject*>(ptr), registeredOriginalMetaObject(typeId), destructorInfo, true, false, true, &superTypeInfos, ocurredException);
                }
            }else if(isFunctional(typeId)){
                if(auto deleter_function = deleter(typeId)){
                    if(superTypeInfos.size()==1){
                        shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, true, &superTypeInfos, deleter_function, ocurredException);
                    }else{
                        shell = new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, destructorInfo, true, true, &superTypeInfos, deleter_function, ocurredException);
                    }
                }else{
                    if(superTypeInfos.size()==1){
                        shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, true, &superTypeInfos, ocurredException);
                    }else{
                        shell = new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, destructorInfo, true, true, &superTypeInfos, ocurredException);
                    }
                }
            }else{
                QMetaType metaType(registeredMetaTypeID(typeId));
                if(metaType.isValid()){
                    if(superTypeInfos.size()==1){
                        if(NewContainerAccessFunction containerAccessFactory = getContainerAccessFactory(typeId)){
                            shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, true, true, &superTypeInfos, containerAccessFactory(), ocurredException);
                        }else if(auto ownerFunction = registeredOwnerFunction(typeId)){
                            shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, true, true, &superTypeInfos, ownerFunction, ocurredException);
                        }else{
                            shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, true, true, &superTypeInfos, ocurredException);
                        }
                    }else{
                        if(NewContainerAccessFunction containerAccessFactory = getContainerAccessFactory(typeId)){
                            shell = new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, destructorInfo, true, true, &superTypeInfos, containerAccessFactory(), ocurredException);
                        }else if(auto ownerFunction = registeredOwnerFunction(typeId)){
                            shell = new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, destructorInfo, true, true, &superTypeInfos, ownerFunction, ocurredException);
                        }else{
                            shell = new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, metaType, destructorInfo, true, true, &superTypeInfos, ocurredException);
                        }
                    }
                }else{
                    if(auto deleter_function = deleter(typeId)){
                        if(superTypeInfos.size()==1){
                            if(auto ownerFunction = registeredOwnerFunction(typeId)){
                                shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, true, &superTypeInfos, deleter_function, ownerFunction, ocurredException);
                            }else{
                                shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, true, &superTypeInfos, deleter_function, ocurredException);
                            }
                        }else{
                            if(auto ownerFunction = registeredOwnerFunction(typeId)){
                                shell = new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, destructorInfo, true, true, &superTypeInfos, deleter_function, ownerFunction, ocurredException);
                            }else{
                                shell = new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, destructorInfo, true, true, &superTypeInfos, deleter_function, ocurredException);
                            }
                        }
                    }else{
                        if(superTypeInfos.size()==1){
                            shell = new SingleTypeShell(env, objectClass, nativeLink, object, typeId, ptr, true, true, &superTypeInfos, ocurredException);
                        }else{
                            shell = new MultiTypeShell(env, objectClass, nativeLink, object, typeId, ptr, destructorInfo, true, true, &superTypeInfos, ocurredException);
                        }
                    }
                }
            }
            if(ocurredException.object()){
                delete shell;
                ocurredException.raise();
            }
            for(const SuperTypeInfo& info : superTypeInfos){
                *reinterpret_cast<QtJambiShell**>(quintptr(ptr) + info.offset() + info.size()) = shell;
            }
            auto i=superTypeInfos.size()-1;
            try{
                for(; i>=0; --i){
                    const SuperTypeInfo& info = superTypeInfos[i];
                    void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                    constructorFunctions.at(i)(iptr, env, object, constructorArguments[i].data());
                    if(!info.hasShell())
                        shell->constructed(info.typeId());
                }
            }catch(const JavaException& exn){
                for(auto j=i+1; j<superTypeInfos.size(); ++j){
                    const SuperTypeInfo& info = superTypeInfos[j];
                    void* iptr = reinterpret_cast<void*>(quintptr(ptr) + info.offset());
                    qtjambi_suppress_exception(env, exn,
                        info.destructor()(iptr);
                    )
                    if(!info.hasShell())
                        shell->destructed(info.typeId());
                }
                if(QSharedPointer<QtJambiLink> link = shell->link()){
                    link->invalidate(env);
                }
                delete shell;
                throw;
            }
            shell->init(env);
        }catch(...){
            if(!declarativeCall)
                operator delete(ptr);
            throw;
        }
    }
}

DestructionHelper::DestructionHelper(const QPointer<const QObject>& owner, std::function<void()> purge)
    : QObject(),
      m_owner(owner),
      m_purge(purge)
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
