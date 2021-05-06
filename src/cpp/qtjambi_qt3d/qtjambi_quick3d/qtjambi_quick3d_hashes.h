#ifndef QTJAMBI_QUICK3D_HASHES_H
#define QTJAMBI_QUICK3D_HASHES_H

#include <qtjambi/qtjambi_core.h>
#include <qtjambi_core/qtjambi_core_qhashes.h>
#include <qtjambi_gui/qtjambi_gui_qhashes.h>
#include <QtCore/qhashfunctions.h>
#include <QtQuick3D/QQuick3DGeometry>

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

#endif // QTJAMBI_QUICK3D_HASHES_H
