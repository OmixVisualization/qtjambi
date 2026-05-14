/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBITASKTREE_HASHES_H
#define QTJAMBITASKTREE_HASHES_H

#include <QtTaskTree/QTaskTree>
#include <QtJambi/Global>

#if defined(QTJAMBI_GENERATOR_RUNNING)
QtTaskTree::Group operator>>(const QtTaskTree::For &forItem, const QtTaskTree::Do &doItem);
QtTaskTree::Group operator>>(const QtTaskTree::When &forItem, const QtTaskTree::Do &doItem);
QtTaskTree::ThenItem operator>>(const QtTaskTree::If &ifItem, const QtTaskTree::Then &thenItem);
QtTaskTree::ElseItem operator>>(const QtTaskTree::ThenItem &thenItem, const QtTaskTree::Else &elseItem);
QtTaskTree::ElseIfItem operator>>(const QtTaskTree::ThenItem &thenItem, const QtTaskTree::ElseIf &elseIfItem);
QtTaskTree::ThenItem operator>>(const QtTaskTree::ElseIfItem &elseIfItem, const QtTaskTree::Then &thenItem);
#endif

bool operator==(const QtTaskTree::GroupItem& value1, const QtTaskTree::GroupItem& value2);

#endif // QTJAMBITASKTREE_HASHES_H
