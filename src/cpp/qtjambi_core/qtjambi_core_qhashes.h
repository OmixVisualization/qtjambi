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

#include <QtCore/QHash>
#include <QtCore/QRect>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QDateTime>
#include <QtCore/QRectF>
#include <QtCore/QDir>
#include <QtCore/QSize>
#include <QtCore/QSizeF>
#include <QtCore/QLine>
#include <QtCore/QLineF>

inline int qHash(const QSizeF &size)
{
    int hashCode = qHash(quint64(size.width()));
    hashCode = hashCode * 31 + qHash(quint64(size.height()));
    return hashCode;
}

inline int qHash(const QSize &size)
{
    int hashCode = qHash(size.width());
    hashCode = hashCode * 31 + qHash(size.height());
    return hashCode;
}

inline int qHash(const QPointF &point)
{
    int hashCode = qHash(quint64(point.x()));
    hashCode = hashCode * 31 + qHash(quint64(point.y()));
    return hashCode;
}

inline int qHash(const QFileInfo &fileInfo)
{
    return qHash(fileInfo.absoluteFilePath());
}

inline int qHash(const QDir &dir)
{
    return qHash(dir.absolutePath());
}

inline int qHash(const QRect &rect)
{
    int hashCode = rect.left();
    hashCode = hashCode * 31 + rect.top();
    hashCode = hashCode * 31 + rect.right();
    hashCode = hashCode * 31 + rect.bottom();
    return hashCode;
}

inline int qHash(const QRectF &rect)
{
    int hashCode = qHash(quint64(rect.left()));
    hashCode = hashCode * 31 + qHash(quint64(rect.top()));
    hashCode = hashCode * 31 + qHash(quint64(rect.right()));
    hashCode = hashCode * 31 + qHash(quint64(rect.bottom()));
    return hashCode;
}

inline int qHash(const QPoint &point)
{
    int hashCode = point.x();
    hashCode = hashCode * 31 + point.y();
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

inline int qHash(const QLineF &line)
{
    int hashCode = qHash(line.p1());
    hashCode = hashCode * 31 + qHash(line.p2());
    return hashCode;
}

inline int qHash(const QLine &line)
{
    int hashCode = qHash(line.p1());
    hashCode = hashCode * 31 + qHash(line.p2());
    return hashCode;
}

#endif // QTJAMBI_CORE_QHASHES_H 
