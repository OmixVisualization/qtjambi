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

#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <QtCore/QtGlobal>

#include <QtJambiCore/hashes.h>

#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QQueue>
#include <QtCore/QString>
#include <QtCore/QStack>
#include <QtCore/QStringList>
#include <QtCore/QSet>
#include <QtCore/QByteArray>

#include <QtJambi/global.h>
#include <QtJambi/typetests.h>

class QHash_int : public QHash<int, int> { };
class QList_int : public QList<int> { };
class QList_String : public QList<QString> { };
class QMap_int : public QMap<int, int> { };
class QMap_int_String : public QMap<int, QString> { };
class QMap_String_int : public QMap<QString, int> { };
class QMap_String : public QMap<QString, QString> { };
class QQueue_int : public QQueue<int> { };
class QQueue_String : public QQueue<QString> { };
class QSet_int : public QSet<int> { };
class QSet_String : public QSet<QString> { };
class QStack_int : public QStack<int> { };
class QStack_String : public QStack<QString> { };
class QMultiMap_int : public QMultiMap<int, int> { };
class QMultiMap_String_int : public QMultiMap<QString, int> { };
class QMultiMap_int_String : public QMultiMap<QString, int> { };
class QMultiMap_String : public QMultiMap<QString, QString> { };
class QMultiHash_int : public QMultiHash<int, int> { };
class QMultiHash_String_int : public QMultiHash<QString, int> { };
class QMultiHash_int_String : public QMultiHash<QString, int> { };
class QMultiHash_String : public QMultiHash<QString, QString> { };

#define QHASHOUT size_t

namespace ContainerTest{
    QSet<QString> valueAt(QList<QSet<QString>>& container, int position);
    void copyQListFDToJavaList(const QList<QPair<float,double>>& list, jobject results);
    QHASHOUT getQListFDHash(const QList<QPair<float,double>>& list);
    void readQListFD(QDataStream &s, QList<QPair<float,double>>& list);
    void writeQListFD(QDataStream &s, const QList<QPair<float,double>>& list);
    void copyQHashShortDoubleToJavaList(const QHash<short,double>& container, jobject results);
    QHASHOUT getQHashShortDoubleHash(const QHash<short,double>& container);
    void writeQHashShortDouble(QDataStream &s, const QHash<short,double>& container);
    void readQHashShortDouble(QDataStream &s, QHash<short,double>& container);
    void copyQSetQPointToJavaList(const QSet<QPoint>& container, jobject results);
    QHASHOUT getQSetQPointHash(const QSet<QPoint>& container);
    void writeQSetQPoint(QDataStream &s, const QSet<QPoint>& container);
    void readQSetQPoint(QDataStream &s, QSet<QPoint>& container);
    void copyQMapShortDoubleToJavaList(const QMap<short,double>& container, jobject results);
//    size_t getQMapShortDoubleHash(const QMap<short,double>& container);
    void writeQMapShortDouble(QDataStream &s, const QMap<short,double>& container);
    void readQMapShortDouble(QDataStream &s, QMap<short,double>& container);
    void copyQMultiHashShortDoubleToJavaList(const QMultiHash<short,double>& container, jobject results);
    QHASHOUT getQMultiHashShortDoubleHash(const QMultiHash<short,double>& container);
    void writeQMultiHashShortDouble(QDataStream &s, const QMultiHash<short,double>& container);
    void readQMultiHashShortDouble(QDataStream &s, QMultiHash<short,double>& container);
    void copyQMultiMapShortDoubleToJavaList(const QMultiMap<short,double>& container, jobject results);
//    size_t getQMultiMapShortDoubleHash(const QMultiMap<short,double>& container);
    void writeQMultiMapShortDouble(QDataStream &s, const QMultiMap<short,double>& container);
    void readQMultiMapShortDouble(QDataStream &s, QMultiMap<short,double>& container);
    void copyFromIterable(const QVariant& variant, jobject results);
    QList<QVariant> toQVariantList(const QVariant& variant);
    QHash<QString,QVariant> toQVariantHash(const QVariant& variant);
    QMap<QString,QVariant> toQVariantMap(const QVariant& variant);
    qsizetype containerSize(const QVariant& variant);
    QVariant associativeValue(const QVariant& variant, const QVariant& key);
    bool associativeFind(const QVariant& variant, const QVariant& key);
    QVariant sequentialAt(const QVariant& variant, int index);
    QVariant sequentialRemoveFirst(QVariant variant);
    QVariant sequentialRemoveLast(QVariant variant);
    QVariant sequentialSetAt(QVariant variant, int index, const QVariant& value);
    QVariant sequentialAppend(QVariant variant, const QVariant& value);
    QVariant sequentialPrepend(QVariant variant, const QVariant& value);
    QVariant associativeRemoveKey(QVariant variant, const QVariant& key);
    QVariant associativeInsertKey(QVariant variant, const QVariant& key);
    QVariant associativeSetValue(QVariant variant, const QVariant& key, const QVariant& value);
    QPair<QVariant,QVariant> associativeFindAndReplace(QVariant variant, const QVariant& key, const QVariant& value);
}

#endif // CONTAINERS_H
