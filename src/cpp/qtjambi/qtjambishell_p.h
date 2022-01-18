/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBISHELL_H
#define QTJAMBISHELL_H

#include "qtjambi_core.h"
#include "qtjambilink_p.h"
#include <typeindex>
#include <QtCore/QSharedPointer>
#include <QtCore/QHash>

class SuperTypeInfos;

typedef std::function<QObject*(void *)> PointerQObjectGetterFunction;

class QtJambiShellImpl : public QtJambiShell{
public:
    QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                JavaException& ocurredException);
    QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                PtrDeleterFunction destructor_function, JavaException& ocurredException);
    QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException);
    QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QObject* ptr, const QMetaObject* originalMetaObject, bool created_by_java, bool isDeclarativeCall, bool is_shell, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException);
    ~QtJambiShellImpl() override;
    void deleteShell() override;
    void init(JNIEnv* env);
    QSharedPointer<QtJambiLink> link() const;
    static const char *const id;
protected:
    void overrideLink(const QSharedPointer<QtJambiLink>& link);
private:
    void* m_ptr;
    const QSharedPointer<const QtJambiFunctionTable> m_vtable;
    QWeakPointer<QtJambiLink> m_link;
    Q_DISABLE_COPY_MOVE(QtJambiShellImpl)
    friend QtJambiShell;
};

class SingleTypeShell : public QtJambiShellImpl{
public:
    SingleTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    SingleTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    SingleTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                JavaException& ocurredException);
    SingleTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                PtrDeleterFunction destructor_function, JavaException& ocurredException);
    SingleTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException);
    SingleTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    SingleTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QObject* ptr, const QMetaObject* originalMetaObject, bool created_by_java, bool isDeclarativeCall, bool is_shell, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException);
    ~SingleTypeShell() override;
    void destructed(const std::type_info& typeId) override;
    void constructed(const std::type_info& typeId) override;
    void deleteShell() override;
private:
#ifdef QT_DEBUG
    bool m_isAlive;
#endif
    Q_DISABLE_COPY_MOVE(SingleTypeShell)
};

struct DestructorInfo{
    DestructorInfo();
    DestructorInfo(const DestructorInfo& info);
    DestructorInfo(DestructorInfo&& info);
    DestructorInfo(
            void* _ptr,
            Destructor _destructor,
            const std::type_info& _typeId,
            PtrOwnerFunction _ownerFunction
        );

    void swap(DestructorInfo& info);
    DestructorInfo& operator=(const DestructorInfo& info);
    DestructorInfo& operator=(DestructorInfo&& info);
    const std::type_info& typeId();
    void destruct() const;
    const QObject* owner() const;
private:
    void* ptr;
    Destructor destructor;
    const std::type_info* m_typeId;
    PtrOwnerFunction ownerFunction;
};

void swap(DestructorInfo& a, DestructorInfo& b) noexcept;

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
    MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, const QMetaType& metaType, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                AbstractContainerAccess* containerAccess, JavaException& ocurredException);
    MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QObject* ptr, const QMetaObject* originalMetaObject, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool isDeclarativeCall, bool is_shell, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException);
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

class DestructionHelper : public QObject {
public:
    DestructionHelper(const QPointer<const QObject>& owner, std::function<void()> purge);
    bool event(QEvent * e) override;
private:
    QPointer<const QObject> m_owner;
    std::function<void()> m_purge;

    Q_DISABLE_COPY_MOVE(DestructionHelper)
};

#endif // QTJAMBISHELL_H
