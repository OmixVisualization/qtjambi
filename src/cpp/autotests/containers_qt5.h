/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef CONTAINERS_QT5_H
#define CONTAINERS_QT5_H

#include <QtCore/QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)

#include <QtCore/QLinkedList>
#include <QtCore/QVector>
#include <QtCore/QString>

#include <QtJambi/typetests.h>

class QLinkedList_int : public QLinkedList<int> { };
class QVector_int : public QVector<int> { };
class QVector_String : public QVector<QString> { };
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)

#endif // CONTAINERS_QT5_H
