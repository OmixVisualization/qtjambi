#ifndef QTJAMBI_DATAVIS3D_HASHES_H
#define QTJAMBI_DATAVIS3D_HASHES_H

#include <QtDataVisualization/QBarDataItem>
#include <QtDataVisualization/QScatterDataItem>
#include <QtDataVisualization/QSurfaceDataItem>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtDataVisualization/QBarDataArray>
#include <QtDataVisualization/QScatterDataArray>
#include <QtDataVisualization/QSurfaceDataArray>
#include <qtjambi/qtjambi_registry.h>

void registerHashFunction_QBarDataArray();
void registerHashFunction_QBarDataRow();
void registerHashFunction_QSurfaceDataArray();
void registerHashFunction_QSurfaceDataRow();
void registerHashFunction_QScatterDataArray();

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

#include <qtjambi/qtjambi_core.h>
#include <qtjambi_core/qtjambi_core_qhashes.h>
#include <qtjambi_gui/qtjambi_gui_qhashes.h>
#include <QtCore/qhashfunctions.h>

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

namespace QtJambiPrivate {
struct QHashCombine {
    typedef uint result_type;
    template <typename T>
    Q_DECL_CONSTEXPR result_type operator()(uint seed, const T &t) const noexcept(noexcept(qHash(t)))
    // combiner taken from N3876 / boost::hash_combine
    { return seed ^ (qHash(t) + 0x9e3779b9 + (seed << 6) + (seed >> 2)) ; }
};
}

inline hash_type qHash(const QtDataVisualization::QBarDataRow &value){
    return std::accumulate(value.begin(), value.end(), 0, QtJambiPrivate::QHashCombine());
}

inline hash_type qHash(const QtDataVisualization::QSurfaceDataRow &value){
    return std::accumulate(value.begin(), value.end(), 0, QtJambiPrivate::QHashCombine());
}

inline hash_type qHash(const QtDataVisualization::QScatterDataArray &value){
    return std::accumulate(value.begin(), value.end(), 0, QtJambiPrivate::QHashCombine());
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
