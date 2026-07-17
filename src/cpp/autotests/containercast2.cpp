/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "global.h"

#if QT_CONFIG(vulkan) && __has_include(<vulkan/vulkan.h>)
#include <QtGui/QVulkanInstance>
#endif
#ifndef QTJAMBI_NO_QUICK
#include <QtQuick/QQuickItem>
#endif
#ifndef QTJAMBI_NO_WIDGETS
#include <QtWidgets/QtWidgets>
#endif

#include <QtJambi/Cast>
#include <QtJambi/ModelCast>
#include <QtJambi/FutureCast>
#include <QtJambi/ArrayCast>
#include <QtJambi/EnumCast>
#include <QtJambi/ArithmeticCast>
#include <QtJambi/SmartPointerCast>
#include <QtJambi/Template1Cast>
#include <QtJambi/Template2Cast>
#include <QtJambi/Template3Cast>
#include <QtJambi/Template4Cast>
#include <QtJambi/Template5Cast>
#include <QtJambi/ContainerCast>
#include <QtJambi/StringAPI>
#include <QtJambi/BufferAPI>
#include <QtJambiQml/Cast>
#include <QtJambiCore/Cast>
#include <QtJambi/QList>
#include <QtJambi/QVariantList>
#include <QtJambi/QObjectList>
#include <QtJambi/QStringList>
#include <QtJambi/QByteArrayList>
#include <QtJambi/QPair>
#include <QtJambi/QSet>
#include <QtJambi/QMap>
#include <QtJambi/QHash>
#include <QtJambi/QMultiMap>
#include <QtJambi/QMultiHash>

void containerCast2(QList<bool>& results, JNIEnv * env, QtJambiScope& scope, jobject list){
    {
        const QMap<float,QVariant> container{{3.987f, "6"}, {8.f, "3"}};
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        results << container.isSharedWith(qtjambi_cast<const QMap<float,QVariant>&>(env, scope, o));
    }

    {
        QMap<QString,QVariant> container{{"6", 6}, {"3", 9}};
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        results << container.isSharedWith(qtjambi_cast<QMap<QString,QVariant>&>(env, scope, o));
    }

    {
        const QHash<QString,int> container{{"6", 6}, {"3", 9}};
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        results << container.isSharedWith(qtjambi_cast<const QHash<QString,int>&>(env, scope, o));
    }

#ifndef QTJAMBI_NO_WIDGETS
    {
        QHash<const QGraphicsItem*, int> container;
        container.insert(nullptr, 6);
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        results << container.isSharedWith(qtjambi_cast<QHash<const QGraphicsItem*, int>&>(env, scope, o));
    }
#endif
    {
        const QMultiHash<QString,int> container{{"6", 6}, {"3", 9}};
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        results << container.isSharedWith(qtjambi_cast<const QMultiHash<QString,int>&>(env, scope, o));
    }

#ifndef QTJAMBI_NO_WIDGETS
    {
        QMultiHash<const QGraphicsItem*, int> container;
        container.insert(nullptr, 5);
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        results << container.isSharedWith(qtjambi_cast<QMultiHash<const QGraphicsItem*, int>&>(env, scope, o));
    }
#endif
}
