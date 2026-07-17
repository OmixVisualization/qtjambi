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

void containerCast(QList<bool>& results, JNIEnv * env, QtJambiScope& scope, jobject list){
    {
        QList<QObject const*> container;
        container << QCoreApplication::instance();
        container << QAbstractEventDispatcher::instance();
        container << QThread::currentThread();
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        results << container.isSharedWith(qtjambi_cast<QList<QObject const*>&>(env, scope, o));
    }

    {
        const QList<QVariant> container{8,7,6,5,4.3f};
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        results << container.isSharedWith(qtjambi_cast<const QList<QVariant>&>(env, scope, o));
    }
    {
        QSet<QString> container;
        container << "set";
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        results << (container==qtjambi_cast<QSet<QString>&>(env, scope, o));
    }

    {
        const QSet<QObject const*> container;
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        results << (qtjambi_cast<const QSet<QObject const*>&>(env, scope, o)==container);
    }

#ifndef QTJAMBI_NO_WIDGETS
    {
        QQueue<QGraphicsItem const*> container;
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        results << container.isSharedWith(qtjambi_cast<QQueue<QGraphicsItem const*>&>(env, scope, o));
    }
#endif

    {
        const QQueue<float> container;
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        results << container.isSharedWith(qtjambi_cast<const QQueue<float>&>(env, scope, o));
    }

#ifndef QTJAMBI_NO_WIDGETS
    {
        QVector<QGraphicsItem const*> container{nullptr, nullptr};
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        results << container.isSharedWith(qtjambi_cast<QVector<QGraphicsItem const*>&>(env, scope, o));
    }
#endif

    {
        const QVector<float> container{1, 2, 3, 4, 6};
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        results << container.isSharedWith(qtjambi_cast<const QVector<float>&>(env, scope, o));
    }

#ifndef QTJAMBI_NO_WIDGETS
    {
        QStack<QGraphicsItem const*> container;
        container << nullptr;
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        results << container.isSharedWith(qtjambi_cast<QStack<QGraphicsItem const*>&>(env, scope, o));
    }
#endif

    {
        const QStack<float> container;
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        results << container.isSharedWith(qtjambi_cast<const QStack<float>&>(env, scope, o));
    }
}
