/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBIWEBSOCKETS_HASHES_H
#define QTJAMBIWEBSOCKETS_HASHES_H

#include <QtJambi/Global>

#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
#include <QtWebSockets/QWebSocketHandshakeOptions>

#if defined(QTJAMBI_GENERATOR_RUNNING)
hash_type qHash(const QWebSocketHandshakeOptions& value, hash_type seed = 0);
#else
class QWebSocketHandshakeOptionsPrivate{
public:
    static hash_type hash(const QWebSocketHandshakeOptions& value, hash_type seed){
        return qHash(quintptr(value.d.data()), seed);
    }
};

inline hash_type qHash(const QWebSocketHandshakeOptions& value, hash_type seed = 0)
{
    return QWebSocketHandshakeOptionsPrivate::hash(value, seed);
}
#endif
#endif

#endif // QTJAMBIWEBSOCKETS_HASHES_H
