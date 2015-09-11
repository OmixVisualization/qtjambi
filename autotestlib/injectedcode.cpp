/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "injectedcode.h"

void GraphicsSceneSubclass::drawItems(QPainter *painter,
                                     int numItems,
                                     QGraphicsItem *items[],
                                     const QStyleOptionGraphicsItem options[],
                                     QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(widget)
    this->numItems = numItems;

    firstBoundingRect = items[0]->boundingRect();
    secondBoundingRect = items[1]->boundingRect();

    firstStyleOptionType = options[0].type;
    firstStyleOptionVersion = options[0].version;

    secondStyleOptionType = options[1].type;
    secondStyleOptionVersion = options[1].version;

    firstItem = items[0];
    secondItem = items[1];

    firstStyleOption = options[0];
    secondStyleOption = options[1];
}

/*
void AccessibleTableInterfaceSubclass::cellAtIndex(int index, int *row, int *column, int *rowSpan,
                                                   int *columnSpan, bool *isSelected)
{
    if (row != 0) *row = index + 1;
    if (column != 0) *column = index + 2;
    if (rowSpan != 0) *rowSpan = index + 3;
    if (columnSpan != 0) *columnSpan = index + 4;
    if (isSelected != 0) *isSelected = (index % 2) == 0;
}

void AccessibleTableInterfaceSubclass::callCellAtIndex(AccessibleTableInterfaceSubclass *obj, int index, int *row, int *col, int *rowSpan, int *columnSpan, bool *isSelected)
{
    obj->cellAtIndex(index, row, col, rowSpan, columnSpan, isSelected);
}

int AccessibleTableInterfaceSubclass::selectedColumns(int maxColumns, QList<int> *columns)
{
    if (columns != 0)
        columns->append(maxColumns);
    return 0;
}

int AccessibleTableInterfaceSubclass::selectedRows(int maxRows, QList<int> *rows)
{
    if (rows != 0)
        rows->append(maxRows);
    return 0;
}

QList<int> AccessibleTableInterfaceSubclass::callSelectedColumns(AccessibleTableInterfaceSubclass *obj, int maxColumns, QList<int> columns)
{
    obj->selectedColumns(maxColumns, &columns);
    return columns;
}

QList<int> AccessibleTableInterfaceSubclass::callSelectedRows(AccessibleTableInterfaceSubclass *obj, int maxRows, QList<int> rows)
{
    obj->selectedRows(maxRows, &rows);
    return rows;
}
*/

void AbstractSocketSubclass::connectProxyAuthenticationRequired(QAbstractSocket *socket)
{
    connect(socket, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)), this, SLOT(aSlot(const QNetworkProxy &, QAuthenticator *)));
}

void AbstractSocketSubclass::emitProxyAuthenticationRequired(QAbstractSocket *socket, const QNetworkProxy &proxy, QAuthenticator *authenticator)
{
    ((AbstractSocketSubclass *) socket)->emitSignalAccessor(proxy, authenticator);
}

void AbstractSocketSubclass::aSlot(const QNetworkProxy &proxy, QAuthenticator *authenticator)
{
    if (authenticator != 0) {
        authenticator->setUser(proxy.user());
        authenticator->setPassword(proxy.password());
    }
}
