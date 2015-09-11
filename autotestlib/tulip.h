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

#ifndef TULIP_H
#define TULIP_H

#include <QtCore/QtCore>

class Tulip
{
public:
    QList<int> do_QList_of_int(const QList<int> &l) { return l; }

    QStringList do_QStringList(const QStringList &l) { return l; }

    QLinkedList<int> do_QLinkedList_of_int(const QLinkedList<int> &l) { return l; }

    QVector<int> do_QVector_of_int(const QVector<int> &l) { return l; }

    QStack<int> do_QStack_of_int(const QStack<int> &s) { return s; }

    QQueue<int> do_QQueue_of_int(const QQueue<int> &q) { return q; }

    QSet<int> do_QSet_of_int(const QSet<int> &s) { return s; }

    QMap<QString, QString> do_QMap_of_strings(const QMap<QString, QString> &m) { return m; }

    QHash<QString, QString> do_QHash_of_strings(const QHash<QString, QString> &h) { return h; }

    QPair<int, int> do_QPair_of_ints(const QPair<int, int> &p) { return p; }
};

class QHash_int : public QHash<int, int> { };
class QLinkedList_int : public QLinkedList<int> { };
class QList_int : public QList<int> { };
class QMap_int : public QMap<int, int> { };
class QQueue_int : public QQueue<int> { };
class QSet_int : public QSet<int> { };
class QStack_int : public QStack<int> { };
class QVector_int : public QVector<int> { };

#endif
