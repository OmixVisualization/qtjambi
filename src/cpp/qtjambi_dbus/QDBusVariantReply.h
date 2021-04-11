/****************************************************************************
**
** Copyright (C) 2016 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBIDBUSREPLY_H
#define QTJAMBIDBUSREPLY_H

#include <QtDBus/QDBusReply>
#include <QtCore/QVariant>
#include <QtDBus/QDBusError>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusPendingCall>

#ifndef QT_JAMBI_RUN

template<class T>
struct QDBusReplyUtility{
    static QDBusReply<T>* reverseCreateFrom(const QDBusReply<QVariant>& dBusReply)
    {
        if(dBusReply.isValid()){
            QDBusVariant dbusVariant(dBusReply.value());
            QDBusMessage message;
            message.setArguments({QVariant::fromValue<QDBusVariant>(dbusVariant)});
            return new QDBusReply<T>(message);
        }else{
            return new QDBusReply<T>(dBusReply.error());
        }
    }

    static QDBusReply<QVariant>* createFrom(const QDBusReply<T>& dBusReply)
    {
        if(dBusReply.isValid()){
            QDBusVariant dbusVariant(QVariant::fromValue<T>(dBusReply.value()));
            QDBusMessage message;
            message.setArguments({QVariant::fromValue<QDBusVariant>(dbusVariant)});
            return new QDBusReply<QVariant>(message);
        }else{
            return new QDBusReply<QVariant>(dBusReply.error());
        }
    }
};

template<>
struct QDBusReplyUtility<QVariant>{
    static QDBusReply<QVariant>* reverseCreateFrom(const QDBusReply<QVariant>& dBusReply)
    {
        return new QDBusReply<QVariant>(dBusReply);
    }
    static QDBusReply<QVariant>* createFrom(const QDBusReply<QVariant>& dBusReply)
    {
        return new QDBusReply<QVariant>(dBusReply);
    }
};

template<>
struct QDBusReplyUtility<void>{
    static QDBusReply<void>* reverseCreateFrom(const QDBusReply<QVariant>& dBusReply)
    {
        if(dBusReply.isValid()){
            return new QDBusReply<void>(QDBusMessage());
        }else{
            return new QDBusReply<void>(dBusReply.error());
        }
    }
    static QDBusReply<QVariant>* createFrom(const QDBusReply<void>& dBusReply)
    {
        if(dBusReply.isValid()){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            QDBusVariant dbusVariant(QVariant(QMetaType::Void, nullptr));
#else
            QDBusVariant dbusVariant(QVariant(QMetaType(QMetaType::Void), nullptr));
#endif
            QDBusMessage message;
            message.setArguments({QVariant::fromValue<QDBusVariant>(dbusVariant)});
            return new QDBusReply<QVariant>(message);
        }else{
            return new QDBusReply<QVariant>(dBusReply.error());
        }
    }
};
#endif //QT_JAMBI_RUN

#endif // QTJAMBIDBUSREPLY_H
