/****************************************************************************
**
** Copyright (C) 2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_CAST_IMPL_DBUS_H
#define QTJAMBI_CAST_IMPL_DBUS_H

#ifdef QDBUSREPLY_H
#include <QtCore/QVariant>

template<class T>
struct QDBusReplyUtility{
    static QDBusReply<T> reverseCreateFrom(const QDBusReply<QVariant>& dBusReply)
    {
        if(dBusReply.isValid()){
            QDBusMessage message;
            message.setArguments({QVariant::fromValue<T>(dBusReply.value().value<T>())});
            return QDBusReply<T>(message);
        }else{
            return QDBusReply<T>(dBusReply.error());
        }
    }

    static QDBusReply<QVariant> createFrom(const QDBusReply<T>& dBusReply)
    {
        if(dBusReply.isValid()){
            QDBusMessage message;
            message.setArguments({QVariant::fromValue<QDBusVariant>(QDBusVariant(QVariant::fromValue<T>(dBusReply.value())))});
            return QDBusReply<QVariant>(message);
        }else{
            return QDBusReply<QVariant>(dBusReply.error());
        }
    }
};

template<>
struct QDBusReplyUtility<QVariant>{
    static QDBusReply<QVariant> reverseCreateFrom(const QDBusReply<QVariant>& dBusReply)
    {
        return dBusReply;
    }
    static QDBusReply<QVariant> createFrom(const QDBusReply<QVariant>& dBusReply)
    {
        return dBusReply;
    }
};

template<>
struct QDBusReplyUtility<void>{
    static QDBusReply<void> reverseCreateFrom(const QDBusReply<QVariant>& dBusReply)
    {
        if(dBusReply.isValid()){
            return QDBusReply<void>(QDBusMessage());
        }else{
            return QDBusReply<void>(dBusReply.error());
        }
    }
    static QDBusReply<QVariant> createFrom(const QDBusReply<void>& dBusReply)
    {
        if(dBusReply.isValid()){
            QDBusVariant dbusVariant(QVariant(QMetaType(QMetaType::Void), nullptr));
            QDBusMessage message;
            message.setArguments({QVariant::fromValue<QDBusVariant>(dbusVariant)});
            return QDBusReply<QVariant>(message);
        }else{
            return QDBusReply<QVariant>(dBusReply.error());
        }
    }
};
#endif // QDBUSREPLY_H

#endif // QTJAMBI_CAST_IMPL_DBUS_H
