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

#ifndef CONTAINERREFERENCES_H
#define CONTAINERREFERENCES_H

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

class ContainerReferences
{
public:
    const QList<QString>& constList() const;
    QList<QString>& listRef();

    const QList<QList<float>>& constListList() const;
    QList<QList<float>>& listListRef();

    const QQueue<QString>& constQueue() const;
    QQueue<QString>& queueRef();

    const QSet<QString>& constSet() const;
    QSet<QString>& setRef();

    const QStack<QPair<int,QString>>& constStack() const;
    QStack<QPair<int,QString>>& stackRef();

    const QMap<QString,QPoint>& constMap() const;
    QMap<QString,QPoint>& mapRef();

    const QHash<int,QPoint>& constHash() const;
    QHash<int,QPoint>& hashRef();

    const QMultiMap<QString,QPoint>& constMultiMap() const;
    QMultiMap<QString,QPoint>& multiMapRef();

    const QMultiHash<int,QPoint>& constMultiHash() const;
    QMultiHash<int,QPoint>& multiHashRef();

    const QStringList& constStringList() const;
    QStringList& stringListRef();

    const QByteArrayList& constByteArrayList() const;
    QByteArrayList& byteArrayListRef();
public:
    QList<QString> m_list;
private:
    QList<QList<float>> m_listlist;
    QStringList m_stringlist;
    QByteArrayList m_bytearraylist;
    QQueue<QString> m_queue;
    QStack<QPair<int,QString>> m_stack;
    QMap<QString,QPoint> m_map;
    QHash<int,QPoint> m_hash;
    QMultiMap<QString,QPoint> m_multimap;
    QMultiHash<int,QPoint> m_multihash;
    QSet<QString> m_set;
};
#endif
