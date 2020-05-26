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

    const QList<QString>& constList() const {return m_list;}
    QList<QString>& listRef() {return m_list;}

    const QList<QList<float>>& constListList() const {return m_listlist;}
    QList<QList<float>>& listListRef() {return m_listlist;}

    const QQueue<QString>& constQueue() const {return m_queue;}
    QQueue<QString>& queueRef() {return m_queue;}

    const QSet<QString>& constSet() const {return m_set;}
    QSet<QString>& setRef() {return m_set;}

    const QVector<double>& constVector() const {return m_vector;}
    QVector<double>& vectorRef() {return m_vector;}

    const QStack<QPair<int,QString>>& constStack() const {return m_stack;}
    QStack<QPair<int,QString>>& stackRef() {return m_stack;}

    const QLinkedList<QPoint>& constLinkedList() const {return m_linkedlist;}
    QLinkedList<QPoint>& linkedListRef() {return m_linkedlist;}

    const QMap<QString,QPoint>& constMap() const {return m_map;}
    QMap<QString,QPoint>& mapRef() {return m_map;}

    const QHash<int,QPoint>& constHash() const {return m_hash;}
    QHash<int,QPoint>& hashRef() {return m_hash;}

    const QMultiMap<QString,QPoint>& constMultiMap() const {return m_multimap;}
    QMultiMap<QString,QPoint>& multiMapRef() {return m_multimap;}

    const QMultiHash<int,QPoint>& constMultiHash() const {return m_multihash;}
    QMultiHash<int,QPoint>& multiHashRef() {return m_multihash;}

    const QStringList& constStringList() const {return m_stringlist;}
    QStringList& stringListRef() {return m_stringlist;}

    const QByteArrayList& constByteArrayList() const {return m_bytearraylist;}
    QByteArrayList& byteArrayListRef() {return m_bytearraylist;}

public:
    QList<QString> m_list;
private:
    QList<QList<float>> m_listlist;
    QStringList m_stringlist;
    QByteArrayList m_bytearraylist;
    QQueue<QString> m_queue;
    QVector<double> m_vector;
    QStack<QPair<int,QString>> m_stack;
    QLinkedList<QPoint> m_linkedlist;
    QMap<QString,QPoint> m_map;
    QHash<int,QPoint> m_hash;
    QMultiMap<QString,QPoint> m_multimap;
    QMultiHash<int,QPoint> m_multihash;
    QSet<QString> m_set;
};

class QHash_int : public QHash<int, int> { };
class QLinkedList_int : public QLinkedList<int> { };
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
class QVector_int : public QVector<int> { };
class QVector_String : public QVector<QString> { };
class StringList : public QStringList { };
class ByteArrayList : public QByteArrayList { };
class QMultiMap_int : public QMultiMap<int, int> { };
class QMultiMap_String_int : public QMultiMap<QString, int> { };
class QMultiMap_int_String : public QMultiMap<QString, int> { };
class QMultiMap_String : public QMultiMap<QString, QString> { };
class QMultiHash_int : public QMultiHash<int, int> { };
class QMultiHash_String_int : public QMultiHash<QString, int> { };
class QMultiHash_int_String : public QMultiHash<QString, int> { };
class QMultiHash_String : public QMultiHash<QString, QString> { };

template<typename K,typename V>
uint qHashMap(const QMap<K,V>& value){
    uint hashCode = qHash(int(value.size()));
    for(const K& key : value.keys()){
        hashCode = hashCode * 31 + qHash(key);
        hashCode = hashCode * 31 + qHash(value.value(key));
    }
    return hashCode;
}

template<typename K,typename V>
uint qHashMap(const QHash<K,V>& value){
    uint hashCode = qHash(int(value.size()));
    for(const K& key : value.keys()){
        hashCode = hashCode * 31 + qHash(key);
        hashCode = hashCode * 31 + qHash(value.value(key));
    }
    return hashCode;
}

template<typename K,typename V>
uint qHashMultiMap(const QMultiMap<K,V>& value){
    uint hashCode = qHash(int(value.size()));
    for(const K& key : value.keys()){
        hashCode = hashCode * 31 + qHash(key);
        hashCode = hashCode * 31 + qHash(value.values(key));
    }
    return hashCode;
}

template<typename K,typename V>
uint qHashMultiMap(const QMultiHash<K,V>& value){
    uint hashCode = qHash(int(value.size()));
    for(const K& key : value.keys()){
        hashCode = hashCode * 31 + qHash(key);
        hashCode = hashCode * 31 + qHash(value.values(key));
    }
    return hashCode;
}

template<typename E>
uint qHashLinkedList(const QLinkedList<E>& value){
    uint hashCode = qHash(int(value.size()));
    for(const E& v : value){
        hashCode = hashCode * 31 + qHash(v);
    }
    return hashCode;
}

inline uint qHash(const QHash_int& value){
    return qHashMap<int, int>(reinterpret_cast<const QHash<int, int>&>(value));
}

inline uint qHash(const QMap_String_int& value){
    return qHashMap<QString, int>(reinterpret_cast<const QMap<QString, int>&>(value));
}

inline uint qHash(const QMap_String& value){
    return qHashMap<QString, QString>(reinterpret_cast<const QMap<QString, QString>&>(value));
}

inline uint qHash(const QMap_int_String& value){
    return qHashMap<int, QString>(reinterpret_cast<const QMap<int, QString>&>(value));
}

inline uint qHash(const QMap_int& value){
    return qHashMap<int, int>(reinterpret_cast<const QMap<int, int>&>(value));
}

inline uint qHash(const QMultiMap_String_int& value){
    return qHashMultiMap<QString, int>(reinterpret_cast<const QMultiMap<QString, int>&>(value));
}

inline uint qHash(const QMultiMap_String& value){
    return qHashMultiMap<QString, QString>(reinterpret_cast<const QMultiMap<QString, QString>&>(value));
}

inline uint qHash(const QMultiMap_int_String& value){
    return qHashMultiMap<int, QString>(reinterpret_cast<const QMultiMap<int, QString>&>(value));
}

inline uint qHash(const QMultiMap_int& value){
    return qHashMultiMap<int, int>(reinterpret_cast<const QMultiMap<int, int>&>(value));
}

inline uint qHash(const QMultiHash_String_int& value){
    return qHashMultiMap<QString, int>(reinterpret_cast<const QMultiHash<QString, int>&>(value));
}

inline uint qHash(const QMultiHash_String& value){
    return qHashMultiMap<QString, QString>(reinterpret_cast<const QMultiHash<QString, QString>&>(value));
}

inline uint qHash(const QMultiHash_int_String& value){
    return qHashMultiMap<int, QString>(reinterpret_cast<const QMultiHash<int, QString>&>(value));
}

inline uint qHash(const QMultiHash_int& value){
    return qHashMultiMap<int, int>(reinterpret_cast<const QMultiHash<int, int>&>(value));
}

inline uint qHash(const QLinkedList_int& value){
    return qHashLinkedList(reinterpret_cast<const QLinkedList<int>&>(value));
}

inline uint qHash(const QList_int& value){
    return qHash(reinterpret_cast<const QList<int>&>(value));
}

inline uint qHash(const QList_String& value){
    return qHash(reinterpret_cast<const QList<QString>&>(value));
}

inline uint qHash(const QSet_int& value){
    return qHash(reinterpret_cast<const QSet<int>&>(value));
}

inline uint qHash(const QSet_String& value){
    return qHash(reinterpret_cast<const QSet<QString>&>(value));
}

inline uint qHash(const QVector_int& value){
    return qHash(reinterpret_cast<const QVector<int>&>(value));
}

inline uint qHash(const QVector_String& value){
    return qHash(reinterpret_cast<const QVector<QString>&>(value));
}

inline uint qHash(const QStack_int& value){
    return qHash(reinterpret_cast<const QVector<int>&>(value));
}

inline uint qHash(const QStack_String& value){
    return qHash(reinterpret_cast<const QVector<QString>&>(value));
}

inline uint qHash(const QQueue_int& value){
    return qHash(reinterpret_cast<const QList<int>&>(value));
}

inline uint qHash(const QQueue_String& value){
    return qHash(reinterpret_cast<const QList<QString>&>(value));
}

inline uint qHash(const StringList& value){
    return qHash(reinterpret_cast<const QList<QString>&>(value));
}

inline uint qHash(const ByteArrayList& value){
    return qHash(reinterpret_cast<const QList<QByteArray>&>(value));
}

#endif
