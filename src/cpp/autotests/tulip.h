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

#ifndef TULIP_H
#define TULIP_H

#include <QtCore/QtGlobal>

#if !defined(Q_CC_MSVC) && !defined(QTJAMBI_GENERATOR_RUNNING)
template<typename RET, typename... ARGS>
inline size_t qHash(RET (*value)(ARGS...));
#endif

#include <QtJambi/typetests.h>

#if !defined(Q_CC_MSVC) && !defined(QTJAMBI_GENERATOR_RUNNING)
template<typename RET, typename... ARGS>
inline size_t qHash(RET (*value)(ARGS...))
{
    return qHash(reinterpret_cast<quintptr>(value));
}
#endif

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/global.h>
#endif
#include <QtCore/QtCore>

#include <QtGui/QColor>

class Tulip
{
public:
    QList<int> do_QList_of_int(const QList<int> &l);

    QStringList do_QStringList(const QStringList &l);

    QStack<int> do_QStack_of_int(const QStack<int> &s);

    QQueue<int> do_QQueue_of_int(const QQueue<int> &q);

    QSet<int> do_QSet_of_int(const QSet<int> &s);

    QMap<QString, QString> do_QMap_of_strings(const QMap<QString, QString> &m);

    QHash<QString, QString> do_QHash_of_strings(const QHash<QString, QString> &h);

    QPair<int, int> do_QPair_of_ints(const QPair<int, int> &p);

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

    static QList<QRunnable*> createListOfRunnables();
    static QList<QEasingCurve::EasingFunction> createListOfEasingFunctions();
    static void testEasingFunctions(const QList<QEasingCurve::EasingFunction>& functions);
    typedef std::function<void(int,bool,double)> TestStdFunction;
    static QList<Tulip::TestStdFunction> createListOfStdFunctions();
    static void testStdFunctions(const QList<Tulip::TestStdFunction>& functions);
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

    static QList<QObject*> createListOfObjects();
    static QList<QList<QRunnable*>> createListOfListOfRunnables();
    static QList<QList<QObject*>> createListOfListOfObjects();
    static qint64 fillIntList(qint32 capacity);
    static void consumeIntList(const QList<int>& list);
    static void consumeStringList(const QList<QString>& list);
    static void consumeColorList(const QList<QColor>& list);
    static void consumeQObjectList(const QList<QObject*>& list);
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
