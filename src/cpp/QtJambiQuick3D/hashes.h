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

#ifndef QTJAMBIQUICK3D_HASHES_H
#define QTJAMBIQUICK3D_HASHES_H

#include <QtCore/qhashfunctions.h>
#include <QtQuick3D/QQuick3DGeometry>
#include <QtJambi/QtJambiAPI>
#include <QtJambiGui/hashes.h>

inline bool operator==(const QQuick3DGeometry::Attribute& a, const QQuick3DGeometry::Attribute& b){
    return a.semantic==b.semantic && a.offset==b.offset && a.componentType==b.componentType;
}

inline hash_type qHash(const QQuick3DGeometry::Attribute &value){
    hash_type hashCode = qHash(value.semantic);
    hashCode = hashCode * 31 + qHash(value.offset);
    hashCode = hashCode * 31 + qHash(value.componentType);
    return hashCode;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 1, 0)
#include <QtQuick3D/QQuick3DInstancing>
inline hash_type qHash(const QQuick3DInstancing::InstanceTableEntry &value){
    hash_type hashCode = qHash(value.row0);
    hashCode = hashCode * 31 + qHash(value.row1);
    hashCode = hashCode * 31 + qHash(value.row2);
    hashCode = hashCode * 31 + qHash(value.color);
    hashCode = hashCode * 31 + qHash(value.instanceData);
    return hashCode;
}
#endif

#endif // QTJAMBIQUICK3D_HASHES_H
