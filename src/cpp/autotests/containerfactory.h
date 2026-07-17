/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef CONTAINERFACTORY_H
#define CONTAINERFACTORY_H

#include <QtCore/QtGlobal>

#if !defined(Q_CC_MSVC) && !defined(QTJAMBI_GENERATOR_RUNNING)
template<typename RET, typename... ARGS>
inline size_t qHash(RET (*value)(ARGS...));
#endif

#include <QtJambi/typetests.h>

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/global.h>
#endif
#include <QtCore/QtCore>

#include <QtGui/QColor>

class ContainerFactory
{
public:
    ContainerFactory();

    static QList<QObject*> createListOfObjects();
    static QList<QList<QRunnable*>> createListOfListOfRunnables();
    static QList<QList<QObject*>> createListOfListOfObjects();
    static QList<QRunnable*> createListOfRunnables();
    static QList<QEasingCurve::EasingFunction> createListOfEasingFunctions();
    static void testEasingFunctions(const QList<QEasingCurve::EasingFunction>& functions);
    typedef std::function<void(int,bool,double)> TestStdFunction;
    static QList<ContainerFactory::TestStdFunction> createListOfStdFunctions();
    static void testStdFunctions(const QList<ContainerFactory::TestStdFunction>& functions);

    static qint64 fillIntList(qint32 capacity);
    static void consumeIntList(const QList<int>& list);
    static void consumeStringList(const QList<QString>& list);
    static void consumeColorList(const QList<QColor>& list);
    static void consumeQObjectList(const QList<QObject*>& list);
};

#endif // CONTAINERFACTORY_H
