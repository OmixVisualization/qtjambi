/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtGraphs/QBarDataItem>
#include <QtGraphs/QScatterDataItem>
#include <QtGraphs/QSurfaceDataItem>

#include <QtJambi/QtJambiAPI>
#include <QtJambiGui/hashes.h>
#include <QtCore/qhashfunctions.h>

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
    return qHashMulti(seed, value.value(), value.rotation());
}

inline hash_type qHash(const QScatterDataItem &value, hash_type seed = 0){
    return qHashMulti(seed, value.position(), value.rotation());
}

inline hash_type qHash(const QSurfaceDataItem &value, hash_type seed = 0){
    return qHash(value.position(), seed);
}

#endif // QTJAMBI_DATAVIS3D_HASHES_H
