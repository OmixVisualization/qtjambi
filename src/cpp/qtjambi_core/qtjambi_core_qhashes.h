/****************************************************************************
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
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

#ifndef QTJAMBI_CORE_QHASHES_H
#define QTJAMBI_CORE_QHASHES_H

#include <QtCore/QtCore>
#include <qtjambi_core/qtjambiqfuture.h>

class QWidget;

template<class T>
uint genericHash(const T &value, uint seed = 0)
{
    return qHashBits(&value, sizeof(T), seed);
}

inline uint qHash(const QSizeF &size)
{
    uint hashCode = qHash(quint64(size.width()));
    hashCode = hashCode * 31 + qHash(quint64(size.height()));
    return hashCode;
}

inline uint qHash(const QSize &size)
{
    uint hashCode = qHash(size.width());
    hashCode = hashCode * 31 + qHash(size.height());
    return hashCode;
}

inline uint qHash(const QPointF &point)
{
    uint hashCode = qHash(quint64(point.x()));
    hashCode = hashCode * 31 + qHash(quint64(point.y()));
    return hashCode;
}

inline uint qHash(const QFileInfo &fileInfo)
{
    return qHash(fileInfo.absoluteFilePath());
}

inline uint qHash(const QDir &dir)
{
    return qHash(dir.absolutePath());
}

inline uint qHash(const QRect &rect)
{
    uint hashCode = qHash(rect.left());
    hashCode = hashCode * 31 + qHash(rect.top());
    hashCode = hashCode * 31 + qHash(rect.right());
    hashCode = hashCode * 31 + qHash(rect.bottom());
    return hashCode;
}

inline uint qHash(const QRectF &rect)
{
    uint hashCode = qHash(rect.left());
    hashCode = hashCode * 31 + qHash(rect.top());
    hashCode = hashCode * 31 + qHash(rect.right());
    hashCode = hashCode * 31 + qHash(rect.bottom());
    return hashCode;
}

inline uint qHash(const QPoint &point)
{
    uint hashCode = qHash(point.x());
    hashCode = hashCode * 31 + qHash(point.y());
    return hashCode;
}

#if QT_VERSION < 0x050000
inline int qHash(const QDate &date)
{
    return date.toJulianDay();
}

inline int qHash(const QTime &time)
{
    int hashCode = time.hour();
    hashCode = hashCode * 31 + time.minute();
    hashCode = hashCode * 31 + time.second();
    hashCode = hashCode * 31 + time.msec();
    return hashCode;
}

inline int qHash(const QDateTime &dateTime)
{
    int hashCode = qHash(dateTime.date());
    hashCode = hashCode * 31 + qHash(dateTime.time());
    return hashCode;
}
#endif

inline uint qHash(const QLineF &line)
{
    uint hashCode = qHash(line.p1());
    hashCode = hashCode * 31 + qHash(line.p2());
    return hashCode;
}

inline uint qHash(const QItemSelection &value)
{
    uint hashCode = qHash(value.size());
    for(const QItemSelectionRange& r : value){
        hashCode = hashCode * 31 + qHash(r);
    }
    return hashCode;
}

#if QT_VERSION < QT_VERSION_CHECK(5, 12, 0)

uint qHash(const QJsonObject &value);

uint qHash(const QJsonArray &value);

inline uint qHash(const QJsonValue &value)
{
    uint hashCode = qHash(int(value.type()));
    switch(value.type()){
    case QJsonValue::Null:    hashCode = hashCode * 31; break;
    case QJsonValue::Bool:    hashCode = hashCode * 31 + qHash(value.toBool()); break;
    case QJsonValue::Double:    hashCode = hashCode * 31 + qHash(value.toDouble()); break;
    case QJsonValue::String:    hashCode = hashCode * 31 + qHash(value.toString()); break;
    case QJsonValue::Array:    hashCode = hashCode * 31 + qHash(value.toArray()); break;
    case QJsonValue::Object:    hashCode = hashCode * 31 + qHash(value.toObject()); break;
    default: break;
    }
    return hashCode;
}

inline uint qHash(const QJsonObject &value)
{
    uint hashCode = qHash(value.keys().size());
    for(const QString& key : value.keys()){
        hashCode = hashCode * 31 + qHash(key);
//        hashCode = hashCode * 31 + qHash(value[key]);
    }
    return hashCode;
}

inline uint qHash(const QJsonArray &value)
{
    uint hashCode = qHash(value.size());
    for(const QJsonValue& r : value){
        hashCode = hashCode * 31 + qHash(r);
    }
    return hashCode;
}
#endif

inline uint qHash(const QJsonDocument &value)
{
    uint hashCode = qHash(value.isArray());
    hashCode = hashCode * 31 + qHash(value.isObject());
    hashCode = hashCode * 31 + qHash(value.isNull());
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    if(value.isArray()){
        hashCode = hashCode * 31 + qHash(value.array());
    }else if(value.isObject()){
        hashCode = hashCode * 31 + qHash(value.object());
    }
    return hashCode;
}

inline uint qHash(const QEasingCurve &curve)
{
    uint hashCode = qHash(curve.amplitude());
    hashCode = hashCode * 31 + qHash(curve.period());
    hashCode = hashCode * 31 + qHash(curve.overshoot());
    hashCode = hashCode * 31 + qHash(int(curve.type()));
    if(curve.type()==QEasingCurve::Custom){
        hashCode = hashCode * 31 + qHash(qint64( reinterpret_cast<const void*>(curve.customType() )));
    }
    return hashCode;
}

inline uint qHash(const QLine &line)
{
    uint hashCode = qHash(line.p1());
    hashCode = hashCode * 31 + qHash(line.p2());
    return hashCode;
}

inline uint qHash(const QMargins &value)
{
    uint hashCode = qHash(value.top());
    hashCode = hashCode * 31 + qHash(value.right());
    hashCode = hashCode * 31 + qHash(value.bottom());
    hashCode = hashCode * 31 + qHash(value.left());
    return hashCode;
}

inline uint qHash(const QMarginsF &value)
{
    uint hashCode = qHash(value.top());
    hashCode = hashCode * 31 + qHash(value.right());
    hashCode = hashCode * 31 + qHash(value.bottom());
    hashCode = hashCode * 31 + qHash(value.left());
    return hashCode;
}

inline uint qHash(const QXmlStreamAttribute &value)
{
    uint hashCode = qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.namespaceUri());
    hashCode = hashCode * 31 + qHash(value.prefix());
    hashCode = hashCode * 31 + qHash(value.qualifiedName());
    hashCode = hashCode * 31 + qHash(value.value());
    hashCode = hashCode * 31 + qHash(value.isDefault());
    return hashCode;
}

inline uint qHash(const QXmlStreamAttributes &value)
{
    uint hashCode = qHash(value.size());
    for(const QXmlStreamAttribute& a : value){
        hashCode = hashCode * 31 + qHash(a);
    }
    return hashCode;
}

inline uint qHash(const QTimeZone &value)
{
    uint hashCode = qHash(value.id());
    return hashCode;
}

#if QT_CONFIG(processenvironment)
inline uint qHash(const QProcessEnvironment &value)
{
    uint hashCode = qHash(value.keys().size());
    for(const QString& key : value.keys()){
        hashCode = hashCode * 31 + qHash(key);
        hashCode = hashCode * 31 + qHash(value.value(key));
    }
    return hashCode;
}
#endif

inline uint qHash(const QXmlStreamEntityDeclaration &value)
{
    uint hashCode = qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.notationName());
    hashCode = hashCode * 31 + qHash(value.publicId());
    hashCode = hashCode * 31 + qHash(value.systemId());
    hashCode = hashCode * 31 + qHash(value.value());
    return hashCode;
}

inline uint qHash(const QXmlStreamNamespaceDeclaration &value)
{
    uint hashCode = qHash(value.namespaceUri());
    hashCode = hashCode * 31 + qHash(value.prefix());
    return hashCode;
}

inline uint qHash(const QXmlStreamNotationDeclaration &value)
{
    uint hashCode = qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.publicId());
    hashCode = hashCode * 31 + qHash(value.systemId());
    return hashCode;
}

inline uint qHash(const QStorageInfo &value)
{
    uint hashCode = qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.rootPath());
    hashCode = hashCode * 31 + qHash(value.device());
    hashCode = hashCode * 31 + qHash(value.isRoot());
    hashCode = hashCode * 31 + qHash(value.isReady());
    hashCode = hashCode * 31 + qHash(value.isValid());
    hashCode = hashCode * 31 + qHash(value.blockSize());
    hashCode = hashCode * 31 + qHash(value.subvolume());
    hashCode = hashCode * 31 + qHash(value.isReadOnly());
    hashCode = hashCode * 31 + qHash(value.displayName());
    return hashCode;
}

inline uint qHash(const QElapsedTimer &value)
{
    struct ElapsedTimer{
        qint64 t1;
        qint64 t2;
    };
    const ElapsedTimer* t = reinterpret_cast<const ElapsedTimer*>(&value);
    uint hashCode = qHash(t->t1);
    hashCode = hashCode * 31 + qHash(t->t2);
    return hashCode;
}

inline uint qHash(const QtJambiVoidFuture &value)
{
    return genericHash(value);
}

inline uint qHash(const QtJambiFuture &value)
{
    return genericHash(value);
}

#endif // QTJAMBI_CORE_QHASHES_H 
