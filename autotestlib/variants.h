/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef VARIANTS_H
#define VARIANTS_H

#include <QtCore/QLine>
#include <QtCore/QLineF>
#include <QtCore/QVariant>
#include <QtGui/QPixmap>
#include <QtGui/QImage>
#include <QtWidgets>

class Variants
{
public:
    QVariant pushThrough(const QVariant &variant) {
        current = variant;
        return current;
    }

    qint64 currentQInt64() const { return *(qint64 *) current.constData(); }
    quint64 currentQUInt64() const { return *(quint64 *) current.constData(); }

    qint32 currentQInt32() const { return *(qint32 *) current.constData(); }
    quint32 currentQUInt32() const { return *(quint32 *) current.constData(); }

    qint16 currentQInt16() const { return *(qint16 *) current.constData(); }
    quint16 currentQUInt16() const { return *(quint16 *) current.constData(); }

    qint8 currentQInt8() const { return *(char *) current.constData(); }
    quint8 currentQUInt8() const { return *(char *) current.constData(); }

    float currentFloat() const { return *(float *) current.constData(); }
    double currentDouble() const { return *(double *) current.constData(); }

    bool currentBool() const { return *(bool *) current.constData(); }

    QString currentString() const { return *(QString *) current.constData(); }

    QVariantList currentQVariantList() const { return *(QVariantList *) current.constData(); }
    QVariantMap currentQVariantMap() const { return *(QVariantMap *) current.constData(); }

    QPixmap currentQPixmap() const { return *(QPixmap *) current.constData(); }
    QImage currentQImage() const { return *(QImage *) current.constData(); }

    QSize currentQSize() const { return current.toSize(); }
    QSizeF currentQSizeF() const { return current.toSizeF(); }

    QPoint currentQPoint() const { return current.toPoint(); }
    QPointF currentQPointF() const { return current.toPointF(); }

    QLine currentQLine() const { return current.toLine(); }
    QLineF currentQLineF() const { return current.toLineF(); }

    QRect currentQRect() const { return current.toRect(); }
    QRectF currentQRectF() const { return current.toRectF(); }

    int currentType() const { return current.type(); }

    bool isValid() const { return current.isValid(); }

    QString currentToString() { return current.toString(); }

    int currentToInt() { return current.toInt(); }

private:
    QVariant current;
};

#endif // VARIANTS_H
