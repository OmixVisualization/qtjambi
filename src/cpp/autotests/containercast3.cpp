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

void containerCast3(QList<bool>& results, JNIEnv * env, QtJambiScope& scope, jobject list){

    {
        std::initializer_list<int> container{1,2,3,4,5,6,7,8,9};
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        results << (qtjambi_cast<std::initializer_list<int>>(env, scope, o)==container);
    }
    {
        const std::initializer_list<QList<QMap<QString,int>>> container{
            /*QList*/{
                /*QMap*/{
                    {"A", 5}, {"B", 123}, {"C", 9}
                },
                /*QMap*/{
                    {"Y", 6}, {"Z", 39}
                }
            },
            /*QList*/{}
        };
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        results << (qtjambi_cast<std::initializer_list<QList<QMap<QString,int>>>>(env, scope, o)==container);
    }

    {
        static QStringList stringList{"A", "B", "C", "D"};
        QStringList* container = &stringList;
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        results << (qtjambi_cast<QStringList*>(env, scope, o)==container);
    }

    {
        static QQueue<QColor> q;
        QQueue<QColor>* container = &q;
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        results << (qtjambi_cast<QQueue<QColor>*>(env, scope, o)==container);
    }

    {
        QSharedPointer<QHash<QString,QString>> ptr(new QHash<QString,QString>{ {"A", "B"} });
        QSharedPointer<QHash<QString,QString>>* container = &ptr;
        jobject o = qtjambi_cast<jobject>(env, scope, container);
        QtJambiAPI::addToJavaCollection(env, list, o);
        QSharedPointer<QHash<QString,QString>>* ptr2 = qtjambi_cast<QSharedPointer<QHash<QString,QString>>*>(env, scope, o);
        results << (ptr2!=nullptr);
        if(ptr2){
            results << (ptr2->data()==ptr.data());
        }
    }
}
