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

#ifndef QTJAMBI_INTERFACES_H
#define QTJAMBI_INTERFACES_H

#include <QtCore/QVector>
#include <QtCore/QString>
#include "qtjambi_global.h"
#include "qtjambi_registry.h"
#include "qtjambi_jobjectwrapper.h"

QTJAMBI_EXPORT
int qtjambi_interface_offset(JNIEnv *env, jclass cls, const std::type_info& interfacetype);

struct ResolvedConstructorInfo{
    inline ResolvedConstructorInfo(): constructorFunction(nullptr),argumentTypes(){}
    inline ResolvedConstructorInfo(const ResolvedConstructorInfo& copy): constructorFunction(copy.constructorFunction),argumentTypes(copy.argumentTypes){}
    inline ResolvedConstructorInfo(ConstructorFunction _constructorFunction, QList<jclass> _argumentTypes): constructorFunction(_constructorFunction),argumentTypes(_argumentTypes){}
    ConstructorFunction constructorFunction;
    QList<jclass> argumentTypes;
};

struct SuperTypeInfo{
    SuperTypeInfo(  const char*const _qtName,
                    const QString _className,
                    jclass _javaClass,
                    const size_t _size,
                    bool _hasShell,
                    const size_t _offset,
                    const QVector<ResolvedConstructorInfo>& _constructorInfos,
                    Destructor _destructor,
                    PtrOwnerFunction _ownerFunction,
                    const std::type_info& typeId)
        : qtName(_qtName),
          className(_className),
          javaClass(_javaClass),
          size(_size),
          hasShell(_hasShell),
          offset(_offset),
          constructorInfos(_constructorInfos),
          destructor(_destructor),
          ownerFunction(_ownerFunction),
          m_typeId(&typeId)
    {}

    SuperTypeInfo()
        : qtName(nullptr),
          className(),
          javaClass(nullptr),
          size(0),
          hasShell(false),
          offset(0),
          constructorInfos(),
          destructor(nullptr),
          ownerFunction(nullptr),
          m_typeId(nullptr)
    {}

    SuperTypeInfo(  const SuperTypeInfo& other )
        : qtName(other.qtName),
          className(other.className),
          javaClass(other.javaClass),
          size(other.size),
          hasShell(other.hasShell),
          offset(other.offset),
          constructorInfos(other.constructorInfos),
          destructor(other.destructor),
          ownerFunction(other.ownerFunction),
          m_typeId(other.m_typeId)
    {}

    const std::type_info& typeId() const {
        Q_ASSERT(m_typeId);
        return *m_typeId;
    }
    const char*const qtName;
    const QString className;
    const jclass javaClass;
    const size_t size;
    const bool hasShell;
    const size_t offset;
    const QVector<ResolvedConstructorInfo> constructorInfos;
    const Destructor destructor;
    const PtrOwnerFunction ownerFunction;
private:
    const std::type_info* m_typeId;
};

class SuperTypeInfos : public QVector<SuperTypeInfo>{
public:
    SuperTypeInfos() = default;
    SuperTypeInfos(JNIEnv *env, jobject obj);
    jobject interfaceList(JNIEnv *env) const;
private:
    JObjectWrapper m_interfaceList;
};

const SuperTypeInfos& getSuperTypeInfos(JNIEnv *env, jclass cls);

#endif // QTJAMBI_INTERFACES_H
