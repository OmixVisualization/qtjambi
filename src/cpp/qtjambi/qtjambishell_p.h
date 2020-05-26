/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include "qtjambi_interfaces.h"
#include <typeindex>
#include <QtCore/QtCore>

typedef std::function<QObject*(void *)> PointerQObjectGetterFunction;

class QtJambiShellImpl : public QtJambiShell{
public:
    QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, int metaType, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QObject* ptr, const QMetaObject* originalMetaObject, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException);
    ~QtJambiShellImpl() override;
    void init(JNIEnv* env);
    void deleteShell() override;
    QSharedPointer<QtJambiLink> link() const;
    void warnForMethod(const char*) const final override;
    void warnForMethod(const char*, const QObject*) const final override;
    jobject getJavaObjectLocalRef(JNIEnv *env) const final override;
    jclass javaClass() const final override;
    jmethodID javaMethod(const std::type_info& typeId, int pos) const final override;
    const QMetaObject* metaObject() const override;
    static const char *const id;
protected:
    void overrideLink(const QSharedPointer<QtJambiLink>& link);
private:
    void* m_ptr;
    const QSharedPointer<const QtJambiFunctionTable> m_vtable;
    QWeakPointer<QtJambiLink> m_link;
    Q_DISABLE_COPY_MOVE(QtJambiShellImpl)
};

class SingleTypeShell : public QtJambiShellImpl{
public:
    SingleTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, int metaType, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    SingleTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QObject* ptr, const QMetaObject* originalMetaObject, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException);
    ~SingleTypeShell();
    void destructed(const std::type_info& typeId);
    void constructed(const std::type_info& typeId);
    void deleteShell();
private:
#ifdef QT_DEBUG
    bool m_isAlive;
#endif
    Q_DISABLE_COPY_MOVE(SingleTypeShell)
};

struct DestructorInfo{
public:
    DestructorInfo() : ptr(nullptr),
        destructor(nullptr),
        m_typeId(nullptr),
        ownerFunction(nullptr)
    {}

    DestructorInfo(const DestructorInfo& info) : ptr(info.ptr),
        destructor(info.destructor),
        m_typeId(info.m_typeId),
        ownerFunction(info.ownerFunction)
    {}

    DestructorInfo(
            void* _ptr,
            Destructor _destructor,
            const std::type_info& _typeId,
            PtrOwnerFunction _ownerFunction
        ) : ptr(_ptr),
        destructor(_destructor),
        m_typeId(&_typeId),
        ownerFunction(_ownerFunction)
    {}

    const std::type_info& typeId(){
        Q_ASSERT(m_typeId);
        return *m_typeId;
    }

    void destruct() const{
        Q_ASSERT(destructor);
        destructor(ptr);
    }

    const QObject* owner() const{
        return ownerFunction ? ownerFunction(ptr) : nullptr;
    }

private:
    void* ptr;
    Destructor destructor;
    const std::type_info* m_typeId;
    PtrOwnerFunction ownerFunction;
};

class MultiTypeShell : public QtJambiShellImpl{
public:
    MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, int metaType, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    MultiTypeShell(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QObject* ptr, const QMetaObject* originalMetaObject, const QHash<size_t,DestructorInfo>& destructorInfo, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException);
    ~MultiTypeShell();
    void destructed(const std::type_info& typeId);
    void constructed(const std::type_info& typeId);
    void deleteShell();
    void superDeleteShell();
private:
    QSet<size_t> m_constructedTypes;
    const QHash<size_t,DestructorInfo> m_destructorInfo;

    Q_DISABLE_COPY_MOVE(MultiTypeShell)
};

class DestructionHelper : public QObject {
public:
    DestructionHelper(const QPointer<const QObject>& owner, std::function<void()> purge);
    bool event(QEvent * e);
private:
    QPointer<const QObject> m_owner;
    std::function<void()> m_purge;

    Q_DISABLE_COPY_MOVE(DestructionHelper)
};

#endif // QTJAMBISHELL_H
