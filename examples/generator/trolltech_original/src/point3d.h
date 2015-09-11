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

#ifndef POINT3D_H
#define POINT3D_H

#include <math.h>

class Point3D
{
public:
    inline Point3D() : m_x(0.0), m_y(0.0), m_z(1.0)
    {
    }

    inline Point3D(qreal _x, qreal _y, qreal _z) : m_x(_x), m_y(_y), m_z(_z)
    {
    }

    inline Point3D operator-(const Point3D &other)
    {
        return Point3D(x() - other.x(), y() - other.y(), z() - other.z());
    }

    inline qreal length()
    {
        return sqrt(x() * x() + y() * y() + z() * z());
    }

    qreal &rx() { return m_x; }
    qreal &ry() { return m_y; }
    qreal &rz() { return m_z; }

    void setX(qreal x) { m_x = x; }
    void setY(qreal y) { m_y = y; }
    void setZ(qreal z) { m_z = z; }

    qreal x() const { return m_x; }
    qreal y() const { return m_y; }
    qreal z() const { return m_z; }

private:
    qreal m_x;
    qreal m_y;
    qreal m_z;
};


#endif
