#ifndef QTJAMBI_DATAVIS3D_HASHES_H
#define QTJAMBI_DATAVIS3D_HASHES_H

#include <qtjambi/qtjambi_core.h>
#include <qtjambi_core/qtjambi_core_qhashes.h>
#include <qtjambi_gui/qtjambi_gui_qhashes.h>
#include <QtCore/qhashfunctions.h>
#include <QtDataVisualization/QBarDataItem>
#include <QtDataVisualization/QScatterDataItem>
#include <QtDataVisualization/QSurfaceDataItem>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#ifdef QT_JAMBI_RUN
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

inline hash_type qHash(const QtDataVisualization::QBarDataItem &value){
    hash_type hashCode = qHash(value.value());
    hashCode = hashCode * 31 + qHash(value.rotation());
    return hashCode;
}

inline hash_type qHash(const QtDataVisualization::QScatterDataItem &value){
    hash_type hashCode = qHash(value.position());
    hashCode = hashCode * 31 + qHash(value.rotation());
    return hashCode;
}

inline hash_type qHash(const QtDataVisualization::QSurfaceDataItem &value){
    return qHash(value.position());
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

inline hash_type qHash(const QBarDataItem &value){
    hash_type hashCode = qHash(value.value());
    hashCode = hashCode * 31 + qHash(value.rotation());
    return hashCode;
}

inline hash_type qHash(const QScatterDataItem &value){
    hash_type hashCode = qHash(value.position());
    hashCode = hashCode * 31 + qHash(value.rotation());
    return hashCode;
}

inline hash_type qHash(const QSurfaceDataItem &value){
    return qHash(value.position());
}
#endif

#endif // QTJAMBI_DATAVIS3D_HASHES_H
