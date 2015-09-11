/****************************************************************************
**
** Copyright (C) 2004-2009 Nokia. All rights reserved.
**
** This file is part of an example program for Qt.
** EDITIONS: NOLIMITS
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QStringListModel>

class DragDropListModel : public QStringListModel
{
    Q_OBJECT

public:
    DragDropListModel(const QStringList &strings, QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent);
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    QStringList mimeTypes() const;
    Qt::DropActions supportedDropActions() const;
};

#endif
