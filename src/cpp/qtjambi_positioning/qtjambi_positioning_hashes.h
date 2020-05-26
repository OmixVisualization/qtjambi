#ifndef QTJAMBI_POSITIONING_HASHES_H
#define QTJAMBI_POSITIONING_HASHES_H

#include <qtjambi_core/qtjambi_core_qhashes.h>
#include <QtCore/QtCore>
#include <QtPositioning/QtPositioning>

inline uint qHash(const QGeoAddress &value)
{
    uint hashCode = qHash(value.city());
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

uint qHash(const QGeoRectangle &value);
uint qHash(const QGeoPath &value);
uint qHash(const QGeoCircle &value);
uint qHash(const QGeoPolygon &value);

inline uint qHash(const QGeoShape &value)
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
        uint hashCode = qHash(value.type());
        hashCode = hashCode * 31 + qHash(value.isEmpty());
        hashCode = hashCode * 31 + qHash(value.center());
        return hashCode;
    }
}

inline uint qHash(const QGeoCircle &value)
{
    if(!value.isValid())
        return 0;
    uint hashCode = qHash(value.radius());
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    hashCode = hashCode * 31 + qHash(value.center());
    return hashCode;
}

inline uint qHash(const QGeoPath &value)
{
    if(!value.isValid())
        return 0;
    uint hashCode = qHash(value.path());
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    hashCode = hashCode * 31 + qHash(value.center());
    return hashCode;
}

inline uint qHash(const QGeoPolygon &value)
{
    if(!value.isValid())
        return 0;
    uint hashCode = qHash(value.path());
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    hashCode = hashCode * 31 + qHash(value.center());
    return hashCode;
}

inline uint qHash(const QGeoRectangle &value)
{
    if(!value.isValid())
        return 0;
    uint hashCode = qHash(value.width());
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    hashCode = hashCode * 31 + qHash(value.center());
    hashCode = hashCode * 31 + qHash(value.height());
    return hashCode;
}

inline uint qHash(const QGeoLocation &value)
{
    uint hashCode = qHash(value.address());
    hashCode = hashCode * 31 + qHash(value.coordinate());
    hashCode = hashCode * 31 + qHash(value.boundingBox());
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    return hashCode;
}

inline uint qHash(const QGeoAreaMonitorInfo &value)
{
    if(!value.isValid())
        return 0;
    uint hashCode = qHash(value.area());
    hashCode = hashCode * 31 + qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.expiration());
    hashCode = hashCode * 31 + qHash(value.identifier());
    hashCode = hashCode * 31 + qHash(value.isPersistent());
    return hashCode;
}

inline uint qHash(const QGeoSatelliteInfo &value)
{
    uint hashCode = qHash(value.signalStrength());
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

inline uint qHash(const QGeoPositionInfo &value)
{
    if(!value.isValid())
        return 0;
    uint hashCode = qHash(value.timestamp());
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

#endif // QTJAMBI_POSITIONING_HASHES_H
