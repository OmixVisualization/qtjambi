#ifndef QTJAMBI_LOCATION_HASHES_H
#define QTJAMBI_LOCATION_HASHES_H

#include <qtjambi_core/qtjambi_core_qhashes.h>
#include <QtCore/QtCore>
#include <QtLocation/QtLocation>
#include <qtjambi_positioning/qtjambi_positioning_hashes.h>

inline uint qHash(const QPlaceUser &value)
{
    uint hashCode = qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.userId());
    return hashCode;
}

inline uint qHash(const QPlaceContentRequest &value)
{
    uint hashCode = qHash(value.placeId());
    hashCode = hashCode * 31 + qHash(value.limit());
    hashCode = hashCode * 31 + qHash(value.contentType());
    return hashCode;
}

inline uint qHash(const QPlaceRatings &value)
{
    uint hashCode = qHash(value.isEmpty());
    hashCode = hashCode * 31 + qHash(value.count());
    hashCode = hashCode * 31 + qHash(value.average());
    hashCode = hashCode * 31 + qHash(value.maximum());
    return hashCode;
}

inline uint qHash(const QPlaceIcon &value)
{
    uint hashCode = qHash(value.isEmpty());
    hashCode = hashCode * 31 + qHash(value.url());
    return hashCode;
}

inline uint qHash(const QPlaceSearchResult &value)
{
    uint hashCode = qHash(value.icon());
    hashCode = hashCode * 31 + qHash(value.type());
    hashCode = hashCode * 31 + qHash(value.title());
    return hashCode;
}

inline uint qHash(const QPlaceContactDetail &value)
{
    uint hashCode = qHash(value.label());
    hashCode = hashCode * 31 + qHash(value.value());
    return hashCode;
}

inline uint qHash(const QPlaceCategory &value)
{
    uint hashCode = qHash(value.icon());
    hashCode = hashCode * 31 + qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    hashCode = hashCode * 31 + qHash(value.categoryId());
    hashCode = hashCode * 31 + qHash(value.visibility());
    return hashCode;
}

inline uint qHash(const QPlaceAttribute &value)
{
    uint hashCode = qHash(value.text());
    hashCode = hashCode * 31 + qHash(value.label());
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    return hashCode;
}

inline uint qHash(const QPlaceSupplier &value)
{
    uint hashCode = qHash(value.url());
    hashCode = hashCode * 31 + qHash(value.icon());
    hashCode = hashCode * 31 + qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.supplierId());
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    return hashCode;
}

inline uint qHash(const QGeoRouteRequest &value)
{
    uint hashCode = qHash(value.waypoints());
    hashCode = hashCode * 31 + qHash(value.travelModes());
    hashCode = hashCode * 31 + qHash(value.excludeAreas());
    hashCode = hashCode * 31 + qHash(value.featureTypes());
    hashCode = hashCode * 31 + qHash(value.departureTime());
    hashCode = hashCode * 31 + qHash(value.segmentDetail());
    hashCode = hashCode * 31 + qHash(value.maneuverDetail());
    hashCode = hashCode * 31 + qHash(value.routeOptimization());
    hashCode = hashCode * 31 + qHash(value.numberAlternativeRoutes());
    return hashCode;
}

inline uint qHash(const QPlace &value)
{
    uint hashCode = qHash(value.icon());
    hashCode = hashCode * 31 + qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    hashCode = hashCode * 31 + qHash(value.placeId());
    hashCode = hashCode * 31 + qHash(value.ratings());
    hashCode = hashCode * 31 + qHash(value.location());
    hashCode = hashCode * 31 + qHash(value.supplier());
    hashCode = hashCode * 31 + qHash(value.categories());
    hashCode = hashCode * 31 + qHash(value.primaryFax());
    hashCode = hashCode * 31 + qHash(value.visibility());
    hashCode = hashCode * 31 + qHash(value.attribution());
    hashCode = hashCode * 31 + qHash(value.contactTypes());
    hashCode = hashCode * 31 + qHash(value.primaryEmail());
    hashCode = hashCode * 31 + qHash(value.primaryPhone());
    hashCode = hashCode * 31 + qHash(value.detailsFetched());
    hashCode = hashCode * 31 + qHash(value.primaryWebsite());
    hashCode = hashCode * 31 + qHash(value.extendedAttributeTypes());
    return hashCode;
}

inline uint qHash(const QPlaceMatchRequest &value)
{
    uint hashCode = qHash(value.places());
    return hashCode;
}

inline uint qHash(const QPlaceSearchRequest &value)
{
    uint hashCode = qHash(value.limit());
    hashCode = hashCode * 31 + qHash(value.categories());
    hashCode = hashCode * 31 + qHash(value.searchArea());
    hashCode = hashCode * 31 + qHash(value.searchTerm());
    hashCode = hashCode * 31 + qHash(value.relevanceHint());
    hashCode = hashCode * 31 + qHash(value.visibilityScope());
    hashCode = hashCode * 31 + qHash(value.recommendationId());
    return hashCode;
}

inline uint qHash(const QPlaceContent &value)
{
    uint hashCode = qHash(value.type());
    hashCode = hashCode * 31 + qHash(value.user());
    hashCode = hashCode * 31 + qHash(value.supplier());
    hashCode = hashCode * 31 + qHash(value.attribution());
    return hashCode;
}

inline uint qHash(const QGeoManeuver &value)
{
    if(!value.isValid())
        return 0;
    uint hashCode = qHash(value.position());
    hashCode = hashCode * 31 + qHash(value.waypoint());
    hashCode = hashCode * 31 + qHash(value.instructionText());
    hashCode = hashCode * 31 + qHash(value.timeToNextInstruction());
    hashCode = hashCode * 31 + qHash(value.distanceToNextInstruction());
    return hashCode;
}

inline uint qHash(const QGeoRouteSegment &value)
{
    if(!value.isValid())
        return 0;
    uint hashCode = qHash(value.path());
    hashCode = hashCode * 31 + qHash(value.maneuver());
    hashCode = hashCode * 31 + qHash(value.travelTime());
    hashCode = hashCode * 31 + qHash(value.distance());
    hashCode = hashCode * 31 + qHash(value.isLegLastSegment());
    return hashCode;
}

inline uint qHash(const QGeoRoute &value)
{
    uint hashCode = qHash(value.path());
    hashCode = hashCode * 31 + qHash(value.bounds());
    hashCode = hashCode * 31 + qHash(value.request());
    hashCode = hashCode * 31 + qHash(value.routeId());
    hashCode = hashCode * 31 + qHash(value.distance());
    hashCode = hashCode * 31 + qHash(value.routeLegs());
    hashCode = hashCode * 31 + qHash(value.travelMode());
    hashCode = hashCode * 31 + qHash(value.travelTime());
    hashCode = hashCode * 31 + qHash(value.firstRouteSegment());
    return hashCode;
}

#endif // QTJAMBI_LOCATION_HASHES_H
