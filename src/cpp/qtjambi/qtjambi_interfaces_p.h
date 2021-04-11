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

#ifndef QTJAMBI_INTERFACES_H
#define QTJAMBI_INTERFACES_H

#include <QtCore/QVector>
#include <QtCore/QString>
#include "qtjambi_global.h"
#include "qtjambi_registry.h"
#include "qtjambi_jobjectwrapper.h"

struct ResolvedConstructorInfo{
    ConstructorFunction constructorFunction;
    QList<jclass> argumentTypes;
};

struct SuperTypeInfo{
    SuperTypeInfo(  const char* _qtName,
                    const QString& _className,
                    jclass _javaClass,
                    size_t _size,
                    bool _hasShell,
                    size_t _offset,
                    const QVector<ResolvedConstructorInfo>& _constructorInfos,
                    Destructor _destructor,
                    PtrOwnerFunction _ownerFunction,
                    const std::type_info& typeId);
    SuperTypeInfo();
    SuperTypeInfo(  const SuperTypeInfo& other );
    SuperTypeInfo(  SuperTypeInfo&& other );
    SuperTypeInfo& operator=(  const SuperTypeInfo& other );
    SuperTypeInfo& operator=(  SuperTypeInfo&& other );
    void swap(SuperTypeInfo& other);
    const std::type_info& typeId() const;
    const char* qtName() const;
    const QString& className() const;
    jclass javaClass() const;
    size_t size() const;
    bool hasShell() const;
    size_t offset() const;
    const QVector<ResolvedConstructorInfo>& constructorInfos() const;
    Destructor destructor() const;
    PtrOwnerFunction ownerFunction() const;
private:
    char const* m_qtName;
    QString m_className;
    jclass m_javaClass;
    size_t m_size;
    bool m_hasShell;
    size_t m_offset;
    QVector<ResolvedConstructorInfo> m_constructorInfos;
    Destructor m_destructor;
    PtrOwnerFunction m_ownerFunction;
    std::type_info const* m_typeId;
};

void swap(SuperTypeInfo& a, SuperTypeInfo& b) noexcept;

class SuperTypeInfos : public QVector<SuperTypeInfo>{
public:
    SuperTypeInfos() = default;
    SuperTypeInfos(JNIEnv *env, jobject obj);
    jobject interfaceList(JNIEnv *env) const;
private:
    JObjectWrapper m_interfaceList;
    friend void clear_supertypes_at_shutdown(JNIEnv *env);
};

const SuperTypeInfos& getSuperTypeInfos(JNIEnv *env, jclass cls);

#endif // QTJAMBI_INTERFACES_H
