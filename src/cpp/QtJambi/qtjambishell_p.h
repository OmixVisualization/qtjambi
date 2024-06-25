/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBISHELL_P_H
#define QTJAMBISHELL_P_H

#include "qtjambishell.h"
#include "qtjambilink_p.h"
#include <typeindex>
#include <QtCore/QSharedPointer>
#include <QtCore/QHash>

class SuperTypeInfos;

class VTable;

typedef void*(*SmartPointerGetter)(const void *);
typedef std::function<void*(const void *)> SmartPointerGetterFunction;

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
                AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, JavaException& ocurredException);
    QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                AbstractContainerAccess* containerAccess, PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, void* ptr, bool created_by_java, bool is_shell, const SuperTypeInfos* superTypeInfos,
                PtrDeleterFunction destructor_function, PtrOwnerFunction ownerFunction, JavaException& ocurredException);
    QtJambiShellImpl(JNIEnv *__jni_env, jclass objectClass, jobject nativeLink, jobject __jni_object, const std::type_info& typeId, QObject* ptr, const QMetaObject* originalMetaObject, bool created_by_java, bool isDeclarativeCall, bool is_shell, bool hasCustomMetaObject, const SuperTypeInfos* superTypeInfos, JavaException& ocurredException);
    ~QtJambiShellImpl() override;
    void deleteShell() override;
    QtJambiScope* returnScope(JNIEnv *env, const std::type_info& typeId, uint index) override;
    void init(JNIEnv* env);
    QSharedPointer<QtJambiLink> link() const;
    bool hasEmptyVTable() const;
    void overrideLink(const QSharedPointer<QtJambiLink>& link);
    static void * getPlacement(JNIEnv *env, jclass callingClass, jobject placement_object);
    static const char *const id;
    static void initializeNativeInterface(JNIEnv *env, jclass callingClass, jobject object, jobject link, jobject arguments);
    static QtJambiShellImpl* get(const QtJambiShellInterface* shellInterface);
    static jmethodID javaMethod(const QtJambiShellInterface* shellInterface, const std::type_info& typeId, int pos);
    static QSharedPointer<QtJambiLink> link(const QtJambiShellInterface* shellInterface);
protected:
    static const QSharedPointer<const VTable>&setupVTable(JNIEnv *, jclass, jobject, const std::type_info&, const SuperTypeInfos*, const QMetaObject*, bool hasCustomMetaObject, JavaException& ocurredException);
private:
    void* m_ptr;
    const QSharedPointer<const VTable> m_vtable;
    QWeakPointer<QtJambiLink> m_link;
    Q_DISABLE_COPY_MOVE(QtJambiShellImpl)
    friend QtJambiShell;
};

#endif // QTJAMBISHELL_P_H
