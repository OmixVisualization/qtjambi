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

#ifndef QTJAMBIHTTPSERVER_HASHES_H
#define QTJAMBIHTTPSERVER_HASHES_H

#include <QtCore/QtCore>
#include <QtJambi/Global>
#include <QtJambiGui/hashes.h>
#include <QtJambiNetwork/hashes.h>
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)

#include <QtHttpServer/QHttpServerWebSocketUpgradeResponse>
inline size_t qHash(const QHttpServerWebSocketUpgradeResponse& value, size_t seed = 0){
    return qHashMulti(seed, value.type(), value.denyStatus(), value.denyMessage());
}

inline bool operator==(const QHttpServerWebSocketUpgradeResponse& value1, const QHttpServerWebSocketUpgradeResponse& value2){
    return value1.type()==value2.type() && value1.denyStatus()==value2.denyStatus() && value1.denyMessage()==value2.denyMessage();
}

#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)

#include <QtHttpServer/QHttpServerConfiguration>
inline size_t qHash(const QHttpServerConfiguration& value, size_t seed = 0){
    return qHashMulti(seed, value.rateLimitPerSecond());
}
#endif

#endif // QTJAMBIHTTPSERVER_HASHES_H
