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

#ifndef QTJAMBILOCATION_HASHES_H
#define QTJAMBILOCATION_HASHES_H

#include <QtJambiCore/hashes.h>
#include <QtLocation/QtLocation>
#include <QtJambiPositioning/hashes.h>

inline hash_type qHash(const QPlaceUser &value, hash_type seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.name());
    seed = hash(seed, value.userId());
    return seed;
}

inline hash_type qHash(const QPlaceContentRequest &value, hash_type seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.placeId());
    seed = hash(seed, value.limit());
    seed = hash(seed, value.contentType());
    return seed;
}

inline hash_type qHash(const QPlaceRatings &value, hash_type seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.isEmpty());
    seed = hash(seed, value.count());
    seed = hash(seed, value.average());
    seed = hash(seed, value.maximum());
    return seed;
}

inline hash_type qHash(const QPlaceIcon &value, hash_type seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.isEmpty());
    seed = hash(seed, value.url());
    return seed;
}

inline hash_type qHash(const QPlaceSearchResult &value, hash_type seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.icon());
    seed = hash(seed, value.type());
    seed = hash(seed, value.title());
    return seed;
}

inline hash_type qHash(const QPlaceContactDetail &value, hash_type seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.label());
    seed = hash(seed, value.value());
    return seed;
}

inline hash_type qHash(const QPlaceCategory &value, hash_type seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.icon());
    seed = hash(seed, value.name());
    seed = hash(seed, value.isEmpty());
    seed = hash(seed, value.categoryId());
    seed = hash(seed, value.visibility());
    return seed;
}

inline hash_type qHash(const QPlaceAttribute &value, hash_type seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.text());
    seed = hash(seed, value.label());
    seed = hash(seed, value.isEmpty());
    return seed;
}

inline hash_type qHash(const QPlaceSupplier &value, hash_type seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.url());
    seed = hash(seed, value.icon());
    seed = hash(seed, value.name());
    seed = hash(seed, value.supplierId());
    seed = hash(seed, value.isEmpty());
    return seed;
}

inline hash_type qHash(const QGeoRouteRequest &value, hash_type seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.waypoints());
    seed = hash(seed, value.travelModes());
    seed = hash(seed, value.excludeAreas());
    seed = hash(seed, value.featureTypes());
    seed = hash(seed, value.departureTime());
    seed = hash(seed, value.segmentDetail());
    seed = hash(seed, value.maneuverDetail());
    seed = hash(seed, value.routeOptimization());
    seed = hash(seed, value.numberAlternativeRoutes());
    return seed;
}

inline hash_type qHash(const QPlace &value, hash_type seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.icon());
    seed = hash(seed, value.name());
    seed = hash(seed, value.isEmpty());
    seed = hash(seed, value.placeId());
    seed = hash(seed, value.ratings());
    seed = hash(seed, value.location());
    seed = hash(seed, value.supplier());
    seed = hash(seed, value.categories());
    seed = hash(seed, value.primaryFax());
    seed = hash(seed, value.visibility());
    seed = hash(seed, value.attribution());
    seed = hash(seed, value.contactTypes());
    seed = hash(seed, value.primaryEmail());
    seed = hash(seed, value.primaryPhone());
    seed = hash(seed, value.detailsFetched());
    seed = hash(seed, value.primaryWebsite());
    seed = hash(seed, value.extendedAttributeTypes());
    return seed;
}

inline hash_type qHash(const QPlaceMatchRequest &value, hash_type seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.places());
    return seed;
}

inline hash_type qHash(const QPlaceSearchRequest &value, hash_type seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.limit());
    seed = hash(seed, value.categories());
    seed = hash(seed, value.searchArea());
    seed = hash(seed, value.searchTerm());
    seed = hash(seed, value.relevanceHint());
    seed = hash(seed, value.visibilityScope());
    seed = hash(seed, value.recommendationId());
    return seed;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0) && !defined(QTJAMBI_GENERATOR_RUNNING)
namespace QtJambiLocationPrivate{
hash_type variantHash(const QVariant& v, hash_type seed = 0);
}
#endif

inline hash_type qHash(const QPlaceContent &value, hash_type seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.type());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    seed = hash(seed, value.user());
    seed = hash(seed, value.supplier());
    seed = hash(seed, value.attribution());
#else
    for(QPlaceContent::DataTag tag : value.dataTags()){
        seed = hash(seed, tag);
        QVariant variant = value.value(tag);
        if(variant.isValid()){
            switch(tag){
            case QPlaceContent::ContentSupplier:
                seed = hash(seed, variant.value<QPlaceSupplier>());
                break;
            case QPlaceContent::ContentUser:
                seed = hash(seed, variant.value<QPlaceUser>());
                break;
            case QPlaceContent::ReviewTitle:
            case QPlaceContent::ReviewText:
            case QPlaceContent::ReviewLanguage:
            case QPlaceContent::ReviewRating:
            case QPlaceContent::ReviewId:
            case QPlaceContent::EditorialLanguage:
            case QPlaceContent::EditorialTitle:
            case QPlaceContent::EditorialText:
            case QPlaceContent::ImageId:
            case QPlaceContent::ImageUrl:
            case QPlaceContent::ImageMimeType:
            case QPlaceContent::ContentAttribution:
                seed = hash(seed, variant.value<QString>());
                break;
             break;
            case QPlaceContent::ReviewDateTime:
                seed = hash(seed, variant.value<QDateTime>());
                break;
            default:
                if(variant.metaType()==QMetaType::fromType<QString>()){
                    seed = hash(seed, variant.value<QString>());
                } else if(variant.metaType()==QMetaType::fromType<char>()){
                    seed = hash(seed, variant.value<char>());
                } else if(variant.metaType()==QMetaType::fromType<uchar>()){
                    seed = hash(seed, variant.value<uchar>());
                } else if(variant.metaType()==QMetaType::fromType<signed char>()){
                    seed = hash(seed, variant.value<signed char>());
                } else if(variant.metaType()==QMetaType::fromType<qint16>()){
                    seed = hash(seed, variant.value<qint16>());
                } else if(variant.metaType()==QMetaType::fromType<quint16>()){
                    seed = hash(seed, variant.value<quint16>());
                } else if(variant.metaType()==QMetaType::fromType<qint32>()){
                    seed = hash(seed, variant.value<qint32>());
                } else if(variant.metaType()==QMetaType::fromType<quint32>()){
                    seed = hash(seed, variant.value<quint32>());
                } else if(variant.metaType()==QMetaType::fromType<float>()){
                    seed = hash(seed, variant.value<float>());
                } else if(variant.metaType()==QMetaType::fromType<double>()){
                    seed = hash(seed, variant.value<double>());
                } else if(variant.metaType()==QMetaType::fromType<QChar>()){
                    seed = hash(seed, variant.value<QChar>());
                } else if(variant.metaType()==QMetaType::fromType<QDateTime>()){
                    seed = hash(seed, variant.value<QDateTime>());
                } else if(variant.metaType()==QMetaType::fromType<QDate>()){
                    seed = hash(seed, variant.value<QDate>());
                } else if(variant.metaType()==QMetaType::fromType<QTime>()){
                    seed = hash(seed, variant.value<QTime>());
                } else if(variant.metaType()==QMetaType::fromType<QUrl>()){
                    seed = hash(seed, variant.value<QUrl>());
                }else{
                    seed = QtJambiLocationPrivate::variantHash(variant, seed);
                }
                break;
            }
        }
    }
#endif
    return seed;
}

inline hash_type qHash(const QGeoManeuver &value, hash_type seed = 0)
{
    if(!value.isValid())
        return seed;
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.position());
    seed = hash(seed, value.waypoint());
    seed = hash(seed, value.instructionText());
    seed = hash(seed, value.timeToNextInstruction());
    seed = hash(seed, value.distanceToNextInstruction());
    return seed;
}

inline hash_type qHash(const QGeoRouteSegment &value, hash_type seed = 0)
{
    if(!value.isValid())
        return seed;
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.path());
    seed = hash(seed, value.maneuver());
    seed = hash(seed, value.travelTime());
    seed = hash(seed, value.distance());
    seed = hash(seed, value.isLegLastSegment());
    return seed;
}

inline hash_type qHash(const QGeoRoute &value, hash_type seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.path());
    seed = hash(seed, value.bounds());
    seed = hash(seed, value.request());
    seed = hash(seed, value.routeId());
    seed = hash(seed, value.distance());
    seed = hash(seed, value.routeLegs());
    seed = hash(seed, value.travelMode());
    seed = hash(seed, value.travelTime());
    seed = hash(seed, value.firstRouteSegment());
    return seed;
}

#endif // QTJAMBI_LOCATION_HASHES_H
