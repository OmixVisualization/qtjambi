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

#ifndef QTJAMBIREMOTEOBJECTS_HASHES_H
#define QTJAMBIREMOTEOBJECTS_HASHES_H
#include <QtJambi/Global>
#include <QtRemoteObjects/qtremoteobjectglobal.h>

inline size_t qHash(const QRemoteObjectSourceLocationInfo& info, size_t seed = 0){
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, info.typeName);
    seed = hash(seed, info.hostUrl);
    return seed;
}

#endif // QTJAMBIREMOTEOBJECTS_HASHES_H
