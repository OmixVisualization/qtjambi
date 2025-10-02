/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBISHELL_H
#define QTJAMBISHELL_H

#include "global.h"
#include <typeinfo>

#include <QtCore/QObject>

class AbstractContainerAccess;
class QtJambiShellImpl;
enum class QtJambiNativeID : jlong;

namespace QtJambiAPI{
enum ConstructorOptions : int;
typedef void (*ConstructorFn)(void*, JNIEnv*, jobject, jvalue*, QtJambiAPI::ConstructorOptions);
}

class QTJAMBI_EXPORT QtJambiShell{
public:
    typedef QtJambiAPI::ConstructorFn ConstructorFunction;

    virtual void destructed(const std::type_info& typeId) = 0;
    virtual void constructed(const std::type_info& typeId) = 0;
    virtual void deleteShell() = 0;
    virtual class QtJambiScope* returnScope(JNIEnv *env, const std::type_info& typeId, uint index) = 0;
    bool tryDeleteShell(const std::type_info& typeId);
    const QMetaObject* metaObject() const;
    void warnForMethod(const char*) const;
    void warnForMethod(const char*, const QObject* object) const;
    jobject getJavaObjectLocalRef(JNIEnv *env) const;
    jclass javaClass() const;
    jmethodID javaMethod(const std::type_info& typeId, int pos) const;
    void * qt_metacast(const char *className, bool* ok = nullptr);
    const void * qt_metacast(const char *className, bool* ok = nullptr) const;
    int qt_metacall(QMetaObject::Call _c, int _id, void **_a);


    static void initialize(JNIEnv *env, jclass callingClass, jobject object, ConstructorFunction constructorFunction,
                           size_t size, const std::type_info& typeId, uint returnScopeRequired, bool isShell,
                           PtrDeleterFunction delete_function,
                           jvalue* arguments = nullptr);

    static void initialize(JNIEnv *env, jclass callingClass, jobject object, ConstructorFunction constructorFunction,
                           size_t size, const std::type_info& typeId, uint returnScopeRequired, bool isShell,
                           PtrDeleterFunction delete_function, PtrOwnerFunction ownerFunction,
                           jvalue* arguments = nullptr);

    static void initialize(JNIEnv *env, jclass callingClass, jobject object, ConstructorFunction constructorFunction,
                           size_t size, const std::type_info& typeId, uint returnScopeRequired, bool isShell,
                           jvalue* arguments = nullptr);

    static void initialize(JNIEnv *env, jclass callingClass, jobject object, ConstructorFunction constructorFunction,
                           size_t size, const std::type_info& typeId, uint returnScopeRequired, bool isShell,
                           PtrOwnerFunction ownerFunction,
                           jvalue* arguments = nullptr);

    static void initialize(JNIEnv *env, jclass callingClass, jobject object, ConstructorFunction constructorFunction,
                           size_t size, const std::type_info& typeId, uint returnScopeRequired, const QMetaObject& originalMetaObject,
                           bool isShell, bool hasCustomMetaObject, jvalue* arguments = nullptr);

    static void initialize(JNIEnv *env, jclass callingClass, jobject object, ConstructorFunction constructorFunction,
                           size_t size, const std::type_info& typeId, uint returnScopeRequired, bool isShell,
                           AbstractContainerAccess* containerAccess,
                           jvalue* arguments = nullptr, QtJambiNativeID owner = QtJambiNativeID(0));

    static void initialize(JNIEnv *env, jclass callingClass, jobject object, ConstructorFunction constructorFunction,
                           size_t size, const std::type_info& typeId, uint returnScopeRequired, bool isShell,
                           AbstractContainerAccess* containerAccess,
                           PtrDeleterFunction delete_function, jvalue* arguments = nullptr, QtJambiNativeID owner = QtJambiNativeID(0));

    static void initialize(JNIEnv *env, jclass callingClass, jobject object, ConstructorFunction constructorFunction,
                           size_t size, const std::type_info& typeId, uint returnScopeRequired, bool isShell,
                           AbstractContainerAccess* containerAccess,
                           PtrDeleterFunction delete_function, PtrOwnerFunction ownerFunction,
                           jvalue* arguments = nullptr, QtJambiNativeID owner = QtJambiNativeID(0));
private:
    QtJambiShell();
    virtual ~QtJambiShell();
    Q_DISABLE_COPY_MOVE(QtJambiShell)
    friend QtJambiShellImpl;
};

class QTJAMBI_EXPORT QtJambiShellInterface{
public:
    static jobject getJavaObjectLocalRef(JNIEnv *env, const QtJambiShellInterface* shellInterface);
protected:
    virtual ~QtJambiShellInterface();
private:
    virtual QtJambiShell* __shell() const = 0;
    friend QtJambiShellImpl;
};

#endif // QTJAMBISHELL_H
