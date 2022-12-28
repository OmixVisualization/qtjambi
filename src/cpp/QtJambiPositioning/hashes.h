/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBIPOSITIONING_HASHES_H
#define QTJAMBIPOSITIONING_HASHES_H

#include <QtCore/QtCore>
#include <QtPositioning/QtPositioning>
#include <QtJambi/Global>
#include <QtJambiCore/hashes.h>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
hash_type qHash(const QGeoAddress &value);
hash_type qHash(const QGeoShape &value);
#endif

hash_type qHash(const QGeoRectangle &value);
hash_type qHash(const QGeoPath &value);
hash_type qHash(const QGeoCircle &value);
hash_type qHash(const QGeoPolygon &value);

inline hash_type qHash(const QGeoCircle &value)
{
    if(!value.isValid())
        return 0;
    hash_type hashCode = qHash(value.radius());
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    hashCode = hashCode * 31 + qHash(value.center());
    return hashCode;
}

inline hash_type qHash(const QGeoPath &value)
{
    if(!value.isValid())
        return 0;
    hash_type hashCode = qHash(value.path());
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    hashCode = hashCode * 31 + qHash(value.center());
    return hashCode;
}

inline hash_type qHash(const QGeoPolygon &value)
{
    if(!value.isValid())
        return 0;
hash_type hashCode =
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        qHash(value.path());
#else
        qHash(value.perimeter());
#endif
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    hashCode = hashCode * 31 + qHash(value.center());
    return hashCode;
}

inline hash_type qHash(const QGeoRectangle &value)
{
    if(!value.isValid())
        return 0;
    hash_type hashCode = qHash(value.width());
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    hashCode = hashCode * 31 + qHash(value.center());
    hashCode = hashCode * 31 + qHash(value.height());
    return hashCode;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline hash_type qHash(const QGeoLocation &value)
{
    hash_type hashCode = qHash(value.address());
    hashCode = hashCode * 31 + qHash(value.coordinate());
    hashCode = hashCode * 31 + qHash(value.boundingBox());
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    return hashCode;
}

inline hash_type qHash(const QGeoAreaMonitorInfo &value)
{
    if(!value.isValid())
        return 0;
    hash_type hashCode = qHash(value.area());
    hashCode = hashCode * 31 + qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.expiration());
    hashCode = hashCode * 31 + qHash(value.identifier());
    hashCode = hashCode * 31 + qHash(value.isPersistent());
    return hashCode;
}

inline hash_type qHash(const QGeoSatelliteInfo &value)
{
    hash_type hashCode = qHash(value.signalStrength());
    hashCode = hashCode * 31 + qHash(value.satelliteSystem());
    hashCode = hashCode * 31 + qHash(value.satelliteIdentifier());
    for(int a=QGeoSatelliteInfo::Elevation; a<=QGeoSatelliteInfo::Azimuth; ++a){
        if(value.hasAttribute(QGeoSatelliteInfo::Attribute(a))){
            hashCode = hashCode * 31 + qHash(value.attribute(QGeoSatelliteInfo::Attribute(a)));
        }else{
            hashCode = hashCode * 31 + qHash(false);
        }
    }
    return hashCode;
}

inline hash_type qHash(const QGeoPositionInfo &value)
{
    if(!value.isValid())
        return 0;
    hash_type hashCode = qHash(value.timestamp());
    hashCode = hashCode * 31 + qHash(value.coordinate());
    for(int a=QGeoPositionInfo::Direction; a<=QGeoPositionInfo::VerticalAccuracy; ++a){
        if(value.hasAttribute(QGeoPositionInfo::Attribute(a))){
            hashCode = hashCode * 31 + qHash(value.attribute(QGeoPositionInfo::Attribute(a)));
        }else{
            hashCode = hashCode * 31 + qHash(false);
        }
    }
    return hashCode;
}

inline hash_type qHash(const QGeoShape &value)
{
    if(!value.isValid())
        return 0;
    switch(value.type()){
    case QGeoShape::RectangleType:
        return qHash(static_cast<const QGeoRectangle &>(value));
    case QGeoShape::CircleType:
        return qHash(static_cast<const QGeoCircle &>(value));
    case QGeoShape::PathType:
        return qHash(static_cast<const QGeoPath &>(value));
    case QGeoShape::PolygonType:
        return qHash(static_cast<const QGeoPolygon &>(value));
    default:
        hash_type hashCode = qHash(value.type());
        hashCode = hashCode * 31 + qHash(value.isEmpty());
        hashCode = hashCode * 31 + qHash(value.center());
        return hashCode;
    }
}

inline hash_type qHash(const QGeoAddress &value)
{
    hash_type hashCode = qHash(value.city());
    hashCode = hashCode * 31 + qHash(value.text());
    hashCode = hashCode * 31 + qHash(value.state());
    hashCode = hashCode * 31 + qHash(value.county());
    hashCode = hashCode * 31 + qHash(value.street());
    hashCode = hashCode * 31 + qHash(value.country());
    hashCode = hashCode * 31 + qHash(value.district());
    hashCode = hashCode * 31 + qHash(value.postalCode());
    hashCode = hashCode * 31 + qHash(value.countryCode());
    hashCode = hashCode * 31 + qHash(value.isTextGenerated());
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    return hashCode;
}
#endif

#endif // QTJAMBI_POSITIONING_HASHES_H
