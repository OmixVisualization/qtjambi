/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#ifndef QTJAMBI_REMOTEOBJECTS_H
#define QTJAMBI_REMOTEOBJECTS_H
#include <qtjambi/qtjambi_global.h>
#include <QtRemoteObjects/qtremoteobjectglobal.h>

inline hash_type qHash(const QRemoteObjectSourceLocationInfo& info){
    hash_type hashCode = qHash(info.typeName);
    hashCode = hashCode * 31 + qHash(info.hostUrl);
    return hashCode;
}

#ifndef QT_JAMBI_RUN
void qtjambi_qRegisterRemoteObjectsClient(JNIEnv *env, jclass type, const QString& id);
void qtjambi_qRegisterRemoteObjectsServer(JNIEnv *env, jclass type, const QString& id);
void qtjambi_copyStoredProperties(JNIEnv *env, jclass type, jobject src, jobject dst);
void qtjambi_copyStoredProperties(JNIEnv *env, jclass type, QDataStream& src, jobject dst);
void qtjambi_copyStoredProperties(JNIEnv *env, jclass type, jobject src, QDataStream& dst);
#endif

#endif // QTJAMBI_REMOTEOBJECTS_H
