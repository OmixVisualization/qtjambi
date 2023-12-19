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

#ifndef QTJAMBICORE_HASHES_H
#define QTJAMBICORE_HASHES_H

#include <QtCore/QtCore>
#include <QtJambi/Global>
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0) && QT_CONFIG(permissions)
#include <QtCore/qpermissions.h>
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0) && defined(QTJAMBI_GENERATOR_RUNNING)
bool operator==(QByteArrayView, QByteArray);
bool operator<(QByteArrayView, QByteArray);
hash_type qHash(QSizePolicy);
#endif

class QWidget;

template<class T>
hash_type genericHash(const T &value, hash_type seed = 0)
{
    return qHashBits(&value, sizeof(T), seed);
}

inline hash_type qHash(const QAbstractEventDispatcher::TimerInfo &value, hash_type seed = 0)
{
    return genericHash(value, seed);
}

inline bool operator==(const QAbstractEventDispatcher::TimerInfo &v1, const QAbstractEventDispatcher::TimerInfo &v2){
    return v1.interval==v2.interval
            && v1.timerId==v2.timerId
            && v1.timerType==v2.timerType;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
inline hash_type qHash(const QMetaType &value, hash_type seed = 0)
{
    return qHash(value.id(), seed);
}
#endif

inline hash_type qHash(const QSizeF &size, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, qint64(size.width()));
    seed = hash(seed, qint64(size.height()));
    return seed;
}

inline hash_type qHash(const QByteArrayMatcher &value, hash_type seed = 0)
{
    return qHash(value.pattern(), seed);
}

inline bool operator==(const QByteArrayMatcher &value1, const QByteArrayMatcher &value2)
{
    return value1.pattern()==value2.pattern();
}

inline hash_type qHash(const QCalendar &value, hash_type seed = 0)
{
    return qHash(value.name(), seed);
}

inline bool operator==(const QCalendar &value1, const QCalendar &value2)
{
    return value1.name()==value2.name();
}

inline hash_type qHash(const QCalendar::YearMonthDay &value, hash_type seed = 0)
{
    return genericHash(value, seed);
}

inline bool operator==(const QCalendar::YearMonthDay &value1, const QCalendar::YearMonthDay &value2)
{
    return value1.year==value2.year && value1.month==value2.month && value1.day==value2.day;
}

inline hash_type qHash(const QCborError &value, hash_type seed = 0)
{
    return qHash(QCborError::Code(value), seed);
}

inline bool operator==(const QCborError &value1, const QCborError &value2)
{
    return QCborError::Code(value1)==QCborError::Code(value2);
}

inline hash_type qHash(const QCborParserError &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.offset);
    seed = hash(seed, value.error);
    return seed;
}

inline bool operator==(const QCborParserError &value1, const QCborParserError &value2)
{
    return value1.offset==value2.offset && value1.error==value2.error;
}

inline hash_type qHash(const QOperatingSystemVersion &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.type());
    seed = hash(seed, value.majorVersion());
    seed = hash(seed, value.minorVersion());
    seed = hash(seed, value.microVersion());
    return seed;
}

inline bool operator==(const QOperatingSystemVersion &value1, const QOperatingSystemVersion &value2)
{
    return value1.type()==value2.type()
            && value1.majorVersion()==value2.majorVersion()
            && value1.minorVersion()==value2.minorVersion()
            && value1.microVersion()==value2.microVersion();
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
inline hash_type qHash(const QOperatingSystemVersionBase &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.type());
    seed = hash(seed, value.majorVersion());
    seed = hash(seed, value.minorVersion());
    seed = hash(seed, value.microVersion());
    return seed;
}

inline bool operator==(const QOperatingSystemVersionBase &value1, const QOperatingSystemVersionBase &value2)
{
    return value1.type()==value2.type()
            && value1.majorVersion()==value2.majorVersion()
            && value1.minorVersion()==value2.minorVersion()
            && value1.microVersion()==value2.microVersion();
}

#ifdef QFACTORYLOADER_P_H
inline hash_type qHash(const QPluginParsedMetaData &value, hash_type seed = 0)
{
    return qHash(*reinterpret_cast<const QCborValue*>(&value), seed);
}

inline bool operator==(const QPluginParsedMetaData &value1, const QPluginParsedMetaData &value2)
{
    return *reinterpret_cast<const QCborValue*>(&value1)==*reinterpret_cast<const QCborValue*>(&value2);
}
#endif
#endif

inline hash_type qHash(const QCollator &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.locale());
    seed = hash(seed, value.caseSensitivity());
    seed = hash(seed, value.numericMode());
    seed = hash(seed, value.ignorePunctuation());
    return seed;
}

inline bool operator==(const QCollator &value1, const QCollator &value2)
{
    return value1.locale()==value2.locale()
            && value1.caseSensitivity()==value2.caseSensitivity()
            && value1.numericMode()==value2.numericMode()
            && value1.ignorePunctuation()==value2.ignorePunctuation();
}

inline hash_type qHash(const QDeadlineTimer &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.timerType());
#if QT_VERSION < QT_VERSION_CHECK(6, 6, 0)
    seed = hash(seed, value._q_data().first);
    seed = hash(seed, value._q_data().second);
#else
    const QPair<qint64,unsigned> &t = reinterpret_cast<const QPair<qint64,unsigned> &>(value);
    seed = hash(seed, t.first);
    seed = hash(seed, t.second);
#endif
    return seed;
}

#if defined(QTJAMBI_GENERATOR_RUNNING)
QDeadlineTimer operator+(QDeadlineTimer dt, qint64 msecs);
QDeadlineTimer& QDeadlineTimer::operator=(const QDeadlineTimer& other);
#endif

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline hash_type qHash(const QSize &size, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, size.width());
    seed = hash(seed, size.height());
    return seed;
}
#else

template<class T>
bool operator==(const QFuture<T> &f1, const QFuture<T> &f2) {
    struct Future{
        QFutureInterfaceBase d;
    };
    return reinterpret_cast<const Future*>(&f1)->d==reinterpret_cast<const Future*>(&f2)->d;
}
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
inline hash_type qHash(const QPointF &point, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, point.x());
    seed = hash(seed, point.y());
    return seed;
}
#else
inline hash_type qHash(const QPointF &point, QHashDummyValue, hash_type seed = 0)
{
    return qHashMulti(seed, point.x(), point.y());
}
#endif

inline hash_type qHash(const QFutureInterfaceBase &value, hash_type seed = 0)
{
    return qHash(quintptr(&value.resultStoreBase()), seed);
}

template<class T>
inline hash_type qHash(const QFuture<T> &value, hash_type seed = 0)
{
    struct Future{
        QFutureInterfaceBase d;
    };
    return qHash(reinterpret_cast<const Future*>(&value)->d, seed);
}

inline hash_type qHash(const QFileInfo &fileInfo, hash_type seed = 0)
{
    return qHash(fileInfo.absoluteFilePath(), seed);
}

inline hash_type qHash(const QDir &dir, hash_type seed = 0)
{
    return qHash(dir.absolutePath(), seed);
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline hash_type qHash(const QRect &rect, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, rect.left());
    seed = hash(seed, rect.top());
    seed = hash(seed, rect.right());
    seed = hash(seed, rect.bottom());
    return seed;
}
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

inline hash_type qHash(const QRectF &rect, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, rect.left());
    seed = hash(seed, rect.top());
    seed = hash(seed, rect.right());
    seed = hash(seed, rect.bottom());
    return seed;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline hash_type qHash(const QPoint &point, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, point.x());
    seed = hash(seed, point.y());
    return seed;
}
#endif

inline hash_type qHash(const QLineF &line, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, line.p1().x());
    seed = hash(seed, line.p1().y());
    seed = hash(seed, line.p2().x());
    seed = hash(seed, line.p2().y());
    return seed;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
inline hash_type qHash(const QItemSelectionRange &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.topLeft());
    seed = hash(seed, value.bottomRight());
    return seed;
}
#endif

inline hash_type qHash(const QJsonValueRef &value, hash_type seed = 0)
{
    switch (value.type()) {
    case QJsonValue::Null:
        return qHash(nullptr, seed);
    case QJsonValue::Bool:
        return qHash(value.toBool(), seed);
    case QJsonValue::Double:
        return qHash(value.toDouble(), seed);
    case QJsonValue::String:
        return qHash(value.toString(), seed);
    case QJsonValue::Array:
        return qHash(value.toArray(), seed);
    case QJsonValue::Object:
        return qHash(value.toObject(), seed);
    default: break;
    }
    return seed;
}

inline hash_type qHash(const QItemSelection &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.size());
    for(const QItemSelectionRange& r : value){
        seed = hash(seed, r);
    }
    return seed;
}

#if QT_VERSION < QT_VERSION_CHECK(5, 12, 0)
hash_type qHash(const QJsonObject &value, hash_type seed = 0);
hash_type qHash(const QJsonArray &value, hash_type seed = 0);

inline hash_type qHash(const QJsonValue &value, hash_type seed = 0)
{
    switch (value.type()) {
    case QJsonValue::Null:
        return qHash(nullptr, seed);
    case QJsonValue::Bool:
        return qHash(value.toBool(), seed);
    case QJsonValue::Double:
        return qHash(value.toDouble(), seed);
    case QJsonValue::String:
        return qHash(value.toString(), seed);
    case QJsonValue::Array:
        return qHash(value.toArray(), seed);
    case QJsonValue::Object:
        return qHash(value.toObject(), seed);
    default: break;
    }
    return seed;
}

inline hash_type qHash(const QJsonObject &object, hash_type seed)
{
    QtPrivate::QHashCombine hash;
    for (auto it = object.begin(), end = object.end(); it != end; ++it) {
        const QString key = it.key();
        const QJsonValue value = it.value();
        seed = hash(seed, QPair<const QString&, const QJsonValue&>(key, value));
    }
    return seed;
}

inline hash_type qHash(const QJsonArray &array, hash_type seed)
{
    return qHashRange(array.begin(), array.end(), seed);
}
#endif

inline hash_type qHash(const QJsonDocument &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.isArray());
    seed = hash(seed, value.isObject());
    seed = hash(seed, value.isNull());
    seed = hash(seed, value.isEmpty());
    if(value.isArray()){
        seed = hash(seed, value.array());
    }else if(value.isObject()){
        seed = hash(seed, value.object());
    }
    return seed;
}

inline hash_type qHash(const QEasingCurve &curve, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, curve.amplitude());
    seed = hash(seed, curve.period());
    seed = hash(seed, curve.overshoot());
    seed = hash(seed, int(curve.type()));
    if(curve.type()==QEasingCurve::Custom){
        seed = hash(seed, qint64( reinterpret_cast<const void*>(curve.customType() )));
    }
    return seed;
}

inline hash_type qHash(const QLine &line, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, line.p1());
    seed = hash(seed, line.p2());
    return seed;
}

inline hash_type qHash(const QMargins &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.top());
    seed = hash(seed, value.right());
    seed = hash(seed, value.bottom());
    seed = hash(seed, value.left());
    return seed;
}

inline hash_type qHash(const QMarginsF &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.top());
    seed = hash(seed, value.right());
    seed = hash(seed, value.bottom());
    seed = hash(seed, value.left());
    return seed;
}

inline hash_type qHash(const QXmlStreamAttribute &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.name());
    seed = hash(seed, value.namespaceUri());
    seed = hash(seed, value.prefix());
    seed = hash(seed, value.qualifiedName());
    seed = hash(seed, value.value());
    seed = hash(seed, value.isDefault());
    return seed;
}

inline hash_type qHash(const QXmlStreamAttributes &value, hash_type seed = 0)
{
    QtPrivate::QHashCombineCommutative hash;
    seed = hash(seed, value.size());
    for(const QXmlStreamAttribute& a : value){
        seed = hash(seed, a);
    }
    return seed;
}

inline hash_type qHash(const QTimeZone &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.id());
    return seed;
}

#if QT_CONFIG(processenvironment)
inline hash_type qHash(const QProcessEnvironment &value, hash_type seed = 0)
{
    QtPrivate::QHashCombineCommutative hash;
    seed = hash(seed, value.keys().size());
    for(const QString& key : value.keys()){
        seed = hash(seed, key);
        seed = hash(seed, value.value(key));
    }
    return seed;
}
#endif

inline hash_type qHash(const QXmlStreamEntityDeclaration &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.name());
    seed = hash(seed, value.notationName());
    seed = hash(seed, value.publicId());
    seed = hash(seed, value.systemId());
    seed = hash(seed, value.value());
    return seed;
}

inline hash_type qHash(const QXmlStreamNamespaceDeclaration &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.namespaceUri());
    seed = hash(seed, value.prefix());
    return seed;
}

inline hash_type qHash(const QXmlStreamNotationDeclaration &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.name());
    seed = hash(seed, value.publicId());
    seed = hash(seed, value.systemId());
    return seed;
}

hash_type qHash(const QMetaMethod &value, hash_type seed = 0);

inline hash_type qHash(const QStorageInfo &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.name());
    seed = hash(seed, value.rootPath());
    seed = hash(seed, value.device());
    seed = hash(seed, value.isRoot());
    seed = hash(seed, value.isReady());
    seed = hash(seed, value.isValid());
    seed = hash(seed, value.blockSize());
    seed = hash(seed, value.subvolume());
    seed = hash(seed, value.isReadOnly());
    seed = hash(seed, value.displayName());
    return seed;
}

inline hash_type qHash(const QElapsedTimer &value, hash_type seed = 0)
{
    struct ElapsedTimer{
        qint64 t1;
        qint64 t2;
    };
    const ElapsedTimer* t = reinterpret_cast<const ElapsedTimer*>(&value);
    QtPrivate::QHashCombine hash;
    seed = hash(seed, t->t1);
    seed = hash(seed, t->t2);
    return seed;
}

inline hash_type qHash(const QMetaObject &value, hash_type seed = 0)
{
    return qHash(qintptr(&value), seed);
}

inline hash_type qHash(const QMetaEnum &value, hash_type seed = 0)
{
    if(!value.isValid())
        return seed;
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.enclosingMetaObject());
    seed = hash(seed, value.enclosingMetaObject()->indexOfEnumerator(value.name()));
    return seed;
}

inline hash_type qHash(const QMetaMethod &value, hash_type seed)
{
    if(!value.isValid())
        return seed;
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.enclosingMetaObject());
    seed = hash(seed, value.methodIndex());
    return seed;
}

inline hash_type qHash(const QMetaProperty &value, hash_type seed = 0)
{
    if(!value.isValid())
        return seed;
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.enclosingMetaObject());
    seed = hash(seed, value.propertyIndex());
    return seed;
}

inline bool operator==(const QMetaEnum &value1, const QMetaEnum &value2)
{
    return value1.enclosingMetaObject()==value2.enclosingMetaObject()
            && value1.enclosingMetaObject()->indexOfEnumerator(value1.name())==value2.enclosingMetaObject()->indexOfEnumerator(value2.name());
}

inline bool operator==(const QMetaProperty &value1, const QMetaProperty &value2)
{
    return value1.enclosingMetaObject()==value2.enclosingMetaObject()
            && value2.propertyIndex()==value2.propertyIndex();
}

#ifdef QTJAMBI_GENERATOR_RUNNING
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
QDebug operator<<(QDebug out, const QByteArrayView &);

struct QtJambiItemSelection{
    QtJambiItemSelection(std::initializer_list<QItemSelectionRange>);
    QtJambiItemSelection();
};
#endif

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline bool operator<(const QString &, const QString &) noexcept{return false;}
inline bool operator<(const QChar &, const QChar &) noexcept{return false;}
inline bool operator==(const QChar &, const QChar &) noexcept{return false;}
#endif

struct QtJambiStringList{
    QtJambiStringList() = delete;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QString join(const QString &sep);
    QString join(QChar sep);
    QStringList filter(const QString &str, Qt::CaseSensitivity cs);
    QStringList filter(const QRegularExpression  &re);
    int removeDuplicates();
    void replaceInStrings(const QString &before, const QString &after, Qt::CaseSensitivity cs);
    void replaceInStrings(const QRegularExpression &re, const QString &after);
    void sort(Qt::CaseSensitivity cs);
#else
    QtJambiStringList(std::initializer_list<QString>);
    QtJambiStringList(QList<QString>);
    void sort(Qt::CaseSensitivity cs);
    qsizetype removeDuplicates();
    QString join(QStringView sep);
    QString join(QChar sep);
    QStringList filter(const QStringView str, Qt::CaseSensitivity cs);
    void replaceInStrings(QStringView before, QStringView after, Qt::CaseSensitivity cs);
    bool contains(const QStringView str, Qt::CaseSensitivity cs) noexcept;
    #if QT_CONFIG(regularexpression)
    QStringList filter(const QRegularExpression &re);
    void replaceInStrings(const QRegularExpression &re, const QString &after);
    qsizetype indexOf(const QRegularExpression &re, qsizetype from);
    qsizetype lastIndexOf(const QRegularExpression &re, qsizetype from);
#endif // QT_CONFIG(regularexpression)
#endif
};
#endif //QTJAMBI_GENERATOR_RUNNING

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
#if QT_CONFIG(permissions)
inline size_t qHash(const QBluetoothPermission &, size_t seed = 0)
{
    return qHash(QString("QBluetoothPermission"), seed);
}
inline bool operator==(const QBluetoothPermission &, const QBluetoothPermission &){return true;}

inline size_t qHash(const QCameraPermission &, size_t seed = 0)
{
    return qHash(QString("QCameraPermission"), seed);
}
inline bool operator==(const QCameraPermission &, const QCameraPermission &){return true;}

inline size_t qHash(const QMicrophonePermission &, size_t seed = 0)
{
    return qHash(QString("QMicrophonePermission"), seed);
}
inline bool operator==(const QMicrophonePermission &, const QMicrophonePermission &){return true;}

inline size_t qHash(const QContactsPermission &value, size_t seed = 0)
{
    return qHashMulti(seed, QString("QContactsPermission"), value.accessMode());
}
inline bool operator==(const QContactsPermission &v1, const QContactsPermission &v2){return v1.accessMode()==v2.accessMode();}

inline size_t qHash(const QCalendarPermission &value, size_t seed = 0)
{
    return qHashMulti(seed, QString("QCalendarPermission"), value.accessMode());
}
inline bool operator==(const QCalendarPermission &v1, const QCalendarPermission &v2){return v1.accessMode()==v2.accessMode();}

inline size_t qHash(const QLocationPermission &value, size_t seed = 0)
{
    return qHashMulti(seed, QString("QLocationPermission"), value.accuracy(), value.availability());
}
inline bool operator==(const QLocationPermission &v1, const QLocationPermission &v2){
    return v1.accuracy()==v2.accuracy() && v1.availability()==v2.availability();
}

inline size_t qHash(const QPermission &value, size_t seed = 0)
{
    if(value.type()==QMetaType::fromType<QBluetoothPermission>()){
        seed = qHash(value.value<QBluetoothPermission>().value(), seed);
    }else if(value.type()==QMetaType::fromType<QCalendarPermission>()){
        seed = qHash(value.value<QCalendarPermission>().value(), seed);
    }else if(value.type()==QMetaType::fromType<QCameraPermission>()){
        seed = qHash(value.value<QCameraPermission>().value(), seed);
    }else if(value.type()==QMetaType::fromType<QContactsPermission>()){
        seed = qHash(value.value<QContactsPermission>().value(), seed);
    }else if(value.type()==QMetaType::fromType<QLocationPermission>()){
        seed = qHash(value.value<QLocationPermission>().value(), seed);
    }else if(value.type()==QMetaType::fromType<QMicrophonePermission>()){
        seed = qHash(value.value<QMicrophonePermission>().value(), seed);
    }
    return qHash(value.status(), seed);
}
inline bool operator==(const QPermission &v1, const QPermission &v2){
    if(v1.status()==v2.status()){
        if(v1.type()==QMetaType::fromType<QBluetoothPermission>() && v2.type()==QMetaType::fromType<QBluetoothPermission>()){
            return v1.value<QBluetoothPermission>().value()==v2.value<QBluetoothPermission>().value();
        }else if(v1.type()==QMetaType::fromType<QCalendarPermission>() && v2.type()==QMetaType::fromType<QCalendarPermission>()){
            return v1.value<QCalendarPermission>().value()==v2.value<QCalendarPermission>().value();
        }else if(v1.type()==QMetaType::fromType<QCameraPermission>() && v2.type()==QMetaType::fromType<QCameraPermission>()){
            return v1.value<QCameraPermission>().value()==v2.value<QCameraPermission>().value();
        }else if(v1.type()==QMetaType::fromType<QContactsPermission>() && v2.type()==QMetaType::fromType<QContactsPermission>()){
            return v1.value<QContactsPermission>().value()==v2.value<QContactsPermission>().value();
        }else if(v1.type()==QMetaType::fromType<QLocationPermission>() && v2.type()==QMetaType::fromType<QLocationPermission>()){
            return v1.value<QLocationPermission>().value()==v2.value<QLocationPermission>().value();
        }else if(v1.type()==QMetaType::fromType<QMicrophonePermission>() && v2.type()==QMetaType::fromType<QMicrophonePermission>()){
            return v1.value<QMicrophonePermission>().value()==v2.value<QMicrophonePermission>().value();
        }
    }
    return false;
}
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
inline bool operator==(const QUuid::Id128Bytes &v1, const QUuid::Id128Bytes &v2){
    return memcmp(&v1, &v2, sizeof(QUuid::Id128Bytes))==0;
}
inline size_t qHash(const QUuid::Id128Bytes &value, size_t seed = 0){
    return qHashBits(&value, sizeof(QUuid::Id128Bytes), seed);
}
#endif
#endif

#endif // QTJAMBICORE_HASHES_H
