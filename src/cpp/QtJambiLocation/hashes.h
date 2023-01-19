/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBILOCATION_HASHES_H
#define QTJAMBILOCATION_HASHES_H

#include <QtJambiCore/hashes.h>
#include <QtLocation/QtLocation>
#include <QtJambiPositioning/hashes.h>

inline hash_type qHash(const QPlaceUser &value)
{
    hash_type hashCode = qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.userId());
    return hashCode;
}

inline hash_type qHash(const QPlaceContentRequest &value)
{
    hash_type hashCode = qHash(value.placeId());
    hashCode = hashCode * 31 + qHash(value.limit());
    hashCode = hashCode * 31 + qHash(value.contentType());
    return hashCode;
}

inline hash_type qHash(const QPlaceRatings &value)
{
    hash_type hashCode = qHash(value.isEmpty());
    hashCode = hashCode * 31 + qHash(value.count());
    hashCode = hashCode * 31 + qHash(value.average());
    hashCode = hashCode * 31 + qHash(value.maximum());
    return hashCode;
}

inline hash_type qHash(const QPlaceIcon &value)
{
    hash_type hashCode = qHash(value.isEmpty());
    hashCode = hashCode * 31 + qHash(value.url());
    return hashCode;
}

inline hash_type qHash(const QPlaceSearchResult &value)
{
    hash_type hashCode = qHash(value.icon());
    hashCode = hashCode * 31 + qHash(value.type());
    hashCode = hashCode * 31 + qHash(value.title());
    return hashCode;
}

inline hash_type qHash(const QPlaceContactDetail &value)
{
    hash_type hashCode = qHash(value.label());
    hashCode = hashCode * 31 + qHash(value.value());
    return hashCode;
}

inline hash_type qHash(const QPlaceCategory &value)
{
    hash_type hashCode = qHash(value.icon());
    hashCode = hashCode * 31 + qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    hashCode = hashCode * 31 + qHash(value.categoryId());
    hashCode = hashCode * 31 + qHash(value.visibility());
    return hashCode;
}

inline hash_type qHash(const QPlaceAttribute &value)
{
    hash_type hashCode = qHash(value.text());
    hashCode = hashCode * 31 + qHash(value.label());
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    return hashCode;
}

inline hash_type qHash(const QPlaceSupplier &value)
{
    hash_type hashCode = qHash(value.url());
    hashCode = hashCode * 31 + qHash(value.icon());
    hashCode = hashCode * 31 + qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.supplierId());
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    return hashCode;
}

inline hash_type qHash(const QGeoRouteRequest &value)
{
    hash_type hashCode = qHash(value.waypoints());
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

inline hash_type qHash(const QPlace &value)
{
    hash_type hashCode = qHash(value.icon());
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

inline hash_type qHash(const QPlaceMatchRequest &value)
{
    hash_type hashCode = qHash(value.places());
    return hashCode;
}

inline hash_type qHash(const QPlaceSearchRequest &value)
{
    hash_type hashCode = qHash(value.limit());
    hashCode = hashCode * 31 + qHash(value.categories());
    hashCode = hashCode * 31 + qHash(value.searchArea());
    hashCode = hashCode * 31 + qHash(value.searchTerm());
    hashCode = hashCode * 31 + qHash(value.relevanceHint());
    hashCode = hashCode * 31 + qHash(value.visibilityScope());
    hashCode = hashCode * 31 + qHash(value.recommendationId());
    return hashCode;
}

inline hash_type qHash(const QPlaceContent &value)
{
    hash_type hashCode = qHash(value.type());
    hashCode = hashCode * 31 + qHash(value.user());
    hashCode = hashCode * 31 + qHash(value.supplier());
    hashCode = hashCode * 31 + qHash(value.attribution());
    return hashCode;
}

inline hash_type qHash(const QGeoManeuver &value)
{
    if(!value.isValid())
        return 0;
    hash_type hashCode = qHash(value.position());
    hashCode = hashCode * 31 + qHash(value.waypoint());
    hashCode = hashCode * 31 + qHash(value.instructionText());
    hashCode = hashCode * 31 + qHash(value.timeToNextInstruction());
    hashCode = hashCode * 31 + qHash(value.distanceToNextInstruction());
    return hashCode;
}

inline hash_type qHash(const QGeoRouteSegment &value)
{
    if(!value.isValid())
        return 0;
    hash_type hashCode = qHash(value.path());
    hashCode = hashCode * 31 + qHash(value.maneuver());
    hashCode = hashCode * 31 + qHash(value.travelTime());
    hashCode = hashCode * 31 + qHash(value.distance());
    hashCode = hashCode * 31 + qHash(value.isLegLastSegment());
    return hashCode;
}

inline hash_type qHash(const QGeoRoute &value)
{
    hash_type hashCode = qHash(value.path());
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
