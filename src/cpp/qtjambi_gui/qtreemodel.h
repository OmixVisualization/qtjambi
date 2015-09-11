/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


#ifndef QTJAMBITREEMODEL_H
#define QTJAMBITREEMODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtCore/QHash>
#include <QtCore/QVariant>
#include <QtCore/QObject>

#include <qtjambi/qtjambi_global.h>

class Node;

struct JObject_key {
    jobject obj;
    uint hashCode;
};

uint qHash(const JObject_key &key);
bool operator==(const JObject_key &a, const JObject_key &b);

class QTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    QTreeModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &) const;
    QModelIndex index(int row, int, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;

    jobject indexToValue(const QModelIndex &index) const;
    QModelIndex valueToIndex(jobject object) const;

    void childrenRemoved(const QModelIndex &parent, int first, int last);
    void childrenInserted(const QModelIndex &parent, int first, int last);

    virtual int childCount(jobject parent) const = 0;
    virtual jobject child(jobject parent, int index) const = 0;

    virtual QVariant data(jobject value, int role) const;
    virtual QIcon icon(jobject value) const;
    virtual QString text(jobject value) const = 0;

public slots:
    void releaseChildren(const QModelIndex &index);

private slots:
    void wasReset();
    void wasChanged();
    void dataWasChanged(const QModelIndex &, const QModelIndex &);

private:
    void initializeNode(Node *n, const QModelIndex &index) const;
    void queryChildren(Node *parentNode, int start = -1, int length = -1) const;
    Node *node(const QModelIndex &index) const;
    Node *node(jobject object) const;

    QHash<JObject_key, Node *> m_nodes;
    Node *m_root;
    uint m_invalidation : 1;
};


#endif
