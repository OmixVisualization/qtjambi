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

#ifndef QTJAMBIPOSITIONING_HASHES_H
#define QTJAMBIPOSITIONING_HASHES_H

#include <QtCore/QtCore>
#include <QtPositioning/QtPositioning>
#include <QtJambi/Global>
#include <QtJambiCore/hashes.h>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
hash_type qHash(const QGeoAddress &value, hash_type seed = 0);
hash_type qHash(const QGeoShape &value, hash_type seed = 0);
#endif

hash_type qHash(const QGeoRectangle &value, hash_type seed = 0);
hash_type qHash(const QGeoPath &value, hash_type seed = 0);
hash_type qHash(const QGeoCircle &value, hash_type seed = 0);
hash_type qHash(const QGeoPolygon &value, hash_type seed = 0);

inline hash_type qHash(const QGeoCircle &value, hash_type seed)
{
    if(!value.isValid())
        return 0;
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.radius());
    seed = hash(seed, value.isEmpty());
    seed = hash(seed, value.center());
    return seed;
}

inline hash_type qHash(const QGeoPath &value, hash_type seed)
{
    if(!value.isValid())
        return 0;
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.path());
    seed = hash(seed, value.isEmpty());
    seed = hash(seed, value.center());
    return seed;
}

inline hash_type qHash(const QGeoPolygon &value, hash_type seed)
{
    if(!value.isValid())
        return 0;
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        value.path());
#else
        value.perimeter());
#endif
    seed = hash(seed, value.isEmpty());
    seed = hash(seed, value.center());
    return seed;
}

inline hash_type qHash(const QGeoRectangle &value, hash_type seed)
{
    if(!value.isValid())
        return 0;
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.width());
    seed = hash(seed, value.isEmpty());
    seed = hash(seed, value.center());
    seed = hash(seed, value.height());
    return seed;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline hash_type qHash(const QGeoLocation &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.address());
    seed = hash(seed, value.coordinate());
    seed = hash(seed, value.boundingBox());
    seed = hash(seed, value.isEmpty());
    return seed;
}

inline hash_type qHash(const QGeoAreaMonitorInfo &value, hash_type seed = 0)
{
    if(!value.isValid())
        return 0;
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.area());
    seed = hash(seed, value.name());
    seed = hash(seed, value.expiration());
    seed = hash(seed, value.identifier());
    seed = hash(seed, value.isPersistent());
    return seed;
}

inline hash_type qHash(const QGeoSatelliteInfo &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.signalStrength());
    seed = hash(seed, value.satelliteSystem());
    seed = hash(seed, value.satelliteIdentifier());
    for(int a=QGeoSatelliteInfo::Elevation; a<=QGeoSatelliteInfo::Azimuth; ++a){
        if(value.hasAttribute(QGeoSatelliteInfo::Attribute(a))){
            seed = hash(seed, value.attribute(QGeoSatelliteInfo::Attribute(a)));
        }else{
            seed = hash(seed, false);
        }
    }
    return seed;
}

inline hash_type qHash(const QGeoPositionInfo &value, hash_type seed = 0)
{
    if(!value.isValid())
        return 0;
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.timestamp());
    seed = hash(seed, value.coordinate());
    for(int a=QGeoPositionInfo::Direction; a<=QGeoPositionInfo::VerticalAccuracy; ++a){
        if(value.hasAttribute(QGeoPositionInfo::Attribute(a))){
            seed = hash(seed, value.attribute(QGeoPositionInfo::Attribute(a)));
        }else{
            seed = hash(seed, false);
        }
    }
    return seed;
}

inline hash_type qHash(const QGeoShape &value, hash_type seed)
{
    if(!value.isValid())
        return 0;
    switch(value.type()){
    case QGeoShape::RectangleType:
        return qHash(static_cast<const QGeoRectangle &>(value), seed);
    case QGeoShape::CircleType:
        return qHash(static_cast<const QGeoCircle &>(value), seed);
    case QGeoShape::PathType:
        return qHash(static_cast<const QGeoPath &>(value), seed);
    case QGeoShape::PolygonType:
        return qHash(static_cast<const QGeoPolygon &>(value), seed);
    default:
        QtPrivate::QHashCombine hash;
        seed = hash(seed, value.type());
        seed = hash(seed, value.isEmpty());
        seed = hash(seed, value.center());
        return seed;
    }
}

inline hash_type qHash(const QGeoAddress &value, hash_type seed)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.city());
    seed = hash(seed, value.text());
    seed = hash(seed, value.state());
    seed = hash(seed, value.county());
    seed = hash(seed, value.street());
    seed = hash(seed, value.country());
    seed = hash(seed, value.district());
    seed = hash(seed, value.postalCode());
    seed = hash(seed, value.countryCode());
    seed = hash(seed, value.isTextGenerated());
    seed = hash(seed, value.isEmpty());
    return seed;
}
#endif

#endif // QTJAMBI_POSITIONING_HASHES_H
