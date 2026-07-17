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

#ifndef MAPFACTORY_H
#define MAPFACTORY_H

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

class MapFactory
{
public:
    MapFactory();
    static QSet<QRunnable*> createSetOfRunnables();
    static QHash<QString,QRunnable*> createStringHashOfRunnables();
    static QMultiHash<QString,QRunnable*> createStringMultiHashOfRunnables();
    static QMap<QString,QRunnable*> createStringMapOfRunnables();
    static QMultiMap<QString,QRunnable*> createStringMultiMapOfRunnables();
    static QSet<QObject*> createSetOfObjects();
    static QHash<QString,QObject*> createStringHashOfObjects();
    static QMultiHash<QString,QObject*> createStringMultiHashOfObjects();
    static QMap<QString,QObject*> createStringMapOfObjects();
    static QMultiMap<QString,QObject*> createStringMultiMapOfObjects();
};

#endif // MAPFACTORY_H
