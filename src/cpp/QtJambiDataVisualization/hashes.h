/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


#ifndef QTJAMBIDATAVIS3D_HASHES_H
#define QTJAMBIDATAVIS3D_HASHES_H

#include <QtDataVisualization/QBarDataItem>
#include <QtDataVisualization/QScatterDataItem>
#include <QtDataVisualization/QSurfaceDataItem>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtDataVisualization/QBarDataArray>
#include <QtDataVisualization/QScatterDataArray>
#include <QtDataVisualization/QSurfaceDataArray>
#include <QtJambi/RegistryAPI>

#ifndef QTJAMBI_GENERATOR_RUNNING
void registerHashFunction_QBarDataArray();
void registerHashFunction_QBarDataRow();
void registerHashFunction_QSurfaceDataArray();
void registerHashFunction_QSurfaceDataRow();
void registerHashFunction_QScatterDataArray();
#endif

namespace QtJambiPrivate {

template<>
struct RegistryHelper<QtDataVisualization::QBarDataArray,false>{
    static void registerHashFunction(){registerHashFunction_QBarDataArray();}
};

template<>
struct RegistryHelper<QtDataVisualization::QBarDataRow,false>{
    static void registerHashFunction(){registerHashFunction_QBarDataRow();}
};

template<>
struct RegistryHelper<QtDataVisualization::QSurfaceDataArray,false>{
    static void registerHashFunction(){registerHashFunction_QSurfaceDataArray();}
};

template<>
struct RegistryHelper<QtDataVisualization::QSurfaceDataRow,false>{
    static void registerHashFunction(){registerHashFunction_QSurfaceDataRow();}
};

template<>
struct RegistryHelper<QtDataVisualization::QScatterDataArray,false>{
    static void registerHashFunction(){registerHashFunction_QScatterDataArray();}
};
}
#endif

#include <QtJambi/QtJambiAPI>
#include <QtJambiGui/hashes.h>
#include <QtCore/qhashfunctions.h>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#ifdef QTJAMBI_GENERATOR_RUNNING
bool operator==(const QtDataVisualization::QBarDataItem& a, const QtDataVisualization::QBarDataItem& b);
bool operator==(const QtDataVisualization::QScatterDataItem& a, const QtDataVisualization::QScatterDataItem& b);
bool operator==(const QtDataVisualization::QSurfaceDataItem& a, const QtDataVisualization::QSurfaceDataItem& b);
#else
namespace QtDataVisualization{
inline bool operator==(const QBarDataItem& a, const QBarDataItem& b){
    return qFuzzyCompare(a.value(), b.value()) && qFuzzyCompare(a.rotation(), b.rotation());
}

inline bool operator==(const QScatterDataItem& a, const QScatterDataItem& b){
    return a.position()==b.position() && qFuzzyCompare(a.rotation(), b.rotation());
}

inline bool operator==(const QSurfaceDataItem& a, const QSurfaceDataItem& b){
    return a.position()==b.position();
}
}
#endif

inline hash_type qHash(const QtDataVisualization::QBarDataItem &value, hash_type seed = 0){
    hash_type hashCode = qHash(value.value(), seed);
    hashCode = hashCode * 31 + qHash(value.rotation(), hashCode);
    return hashCode;
}

inline hash_type qHash(const QtDataVisualization::QScatterDataItem &value, hash_type seed = 0){
    hash_type hashCode = qHash(value.position(), seed);
    hashCode = hashCode * 31 + qHash(value.rotation(), hashCode);
    return hashCode;
}

inline hash_type qHash(const QtDataVisualization::QSurfaceDataItem &value, hash_type seed = 0){
    return qHash(value.position(), seed);
}

namespace QtJambiPrivate {
struct QHashCombine {
    typedef uint result_type;
    template <typename T>
    Q_DECL_CONSTEXPR result_type operator()(uint seed, const T &t) const noexcept(noexcept(qHash(t)))
    // combiner taken from N3876 / boost::hash_combine
    { return seed ^ (qHash(t) + 0x9e3779b9 + (seed << 6) + (seed >> 2)) ; }
};
}

inline hash_type qHash(const QtDataVisualization::QBarDataRow &value, hash_type seed = 0){
    return std::accumulate(value.begin(), value.end(), seed, QtJambiPrivate::QHashCombine());
}

inline hash_type qHash(const QtDataVisualization::QSurfaceDataRow &value, hash_type seed = 0){
    return std::accumulate(value.begin(), value.end(), seed, QtJambiPrivate::QHashCombine());
}

inline hash_type qHash(const QtDataVisualization::QScatterDataArray &value, hash_type seed = 0){
    return std::accumulate(value.begin(), value.end(), seed, QtJambiPrivate::QHashCombine());
}
#else
inline bool operator==(const QBarDataItem& a, const QBarDataItem& b){
    return qFuzzyCompare(a.value(), b.value()) && qFuzzyCompare(a.rotation(), b.rotation());
}

inline bool operator==(const QScatterDataItem& a, const QScatterDataItem& b){
    return a.position()==b.position() && qFuzzyCompare(a.rotation(), b.rotation());
}

inline bool operator==(const QSurfaceDataItem& a, const QSurfaceDataItem& b){
    return a.position()==b.position();
}

inline hash_type qHash(const QBarDataItem &value, hash_type seed = 0){
    hash_type hashCode = qHash(value.value(), seed);
    hashCode = hashCode * 31 + qHash(value.rotation(), hashCode);
    return hashCode;
}

inline hash_type qHash(const QScatterDataItem &value, hash_type seed = 0){
    hash_type hashCode = qHash(value.position(), seed);
    hashCode = hashCode * 31 + qHash(value.rotation(), hashCode);
    return hashCode;
}

inline hash_type qHash(const QSurfaceDataItem &value, hash_type seed = 0){
    return qHash(value.position(), seed);
}
#endif

#endif // QTJAMBI_DATAVIS3D_HASHES_H
