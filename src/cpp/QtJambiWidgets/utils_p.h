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

#ifndef UTILS_P_H
#define UTILS_P_H

#include <QtWidgets/QtWidgets>
#include <QtJambi/Global>
#include <QtJambi/JavaAPI>

namespace Java{
namespace QtWidgets{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QGraphicsItem$BlockedByModalPanelInfo,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QFileDialog$Result,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QFormLayout$ItemInfo,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QGridLayout$ItemInfo,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QSplitter$Range,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QStyle,
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(findSubControl))
}
}

const QObject* getPointerOwner(const QGraphicsLayoutItem* __qt_this);
const QObject* getPointerOwner(const QGraphicsItem* __qt_this);
const QObject* getPointerOwner(const QTreeWidgetItemIterator* __qt_this);

#endif // UTILS_P_H
