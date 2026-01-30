/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "tulip.h"

QList<int> Tulip::do_QList_of_int(const QList<int> &l) { return l; }

QStringList Tulip::do_QStringList(const QStringList &l) { return l; }

QStack<int> Tulip::do_QStack_of_int(const QStack<int> &s) { return s; }

QQueue<int> Tulip::do_QQueue_of_int(const QQueue<int> &q) { return q; }

QSet<int> Tulip::do_QSet_of_int(const QSet<int> &s) { return s; }

QMap<QString, QString> Tulip::do_QMap_of_strings(const QMap<QString, QString> &m) { return m; }

QHash<QString, QString> Tulip::do_QHash_of_strings(const QHash<QString, QString> &h) { return h; }

QPair<int, int> Tulip::do_QPair_of_ints(const QPair<int, int> &p) { return p; }

const QList<QString>& Tulip::constList() const {return m_list;}
QList<QString>& Tulip::listRef() {return m_list;}

const QList<QList<float>>& Tulip::constListList() const {return m_listlist;}
QList<QList<float>>& Tulip::listListRef() {return m_listlist;}

const QQueue<QString>& Tulip::constQueue() const {return m_queue;}
QQueue<QString>& Tulip::queueRef() {return m_queue;}

const QSet<QString>& Tulip::constSet() const {return m_set;}
QSet<QString>& Tulip::setRef() {return m_set;}

const QStack<QPair<int,QString>>& Tulip::constStack() const {return m_stack;}
QStack<QPair<int,QString>>& Tulip::stackRef() {return m_stack;}

const QMap<QString,QPoint>& Tulip::constMap() const {return m_map;}
QMap<QString,QPoint>& Tulip::mapRef() {return m_map;}

const QHash<int,QPoint>& Tulip::constHash() const {return m_hash;}
QHash<int,QPoint>& Tulip::hashRef() {return m_hash;}

const QMultiMap<QString,QPoint>& Tulip::constMultiMap() const {return m_multimap;}
QMultiMap<QString,QPoint>& Tulip::multiMapRef() {return m_multimap;}

const QMultiHash<int,QPoint>& Tulip::constMultiHash() const {return m_multihash;}
QMultiHash<int,QPoint>& Tulip::multiHashRef() {return m_multihash;}

const QStringList& Tulip::constStringList() const {return m_stringlist;}
QStringList& Tulip::stringListRef() {return m_stringlist;}

const QByteArrayList& Tulip::constByteArrayList() const {return m_bytearraylist;}
QByteArrayList& Tulip::byteArrayListRef() {return m_bytearraylist;}

QList<QRunnable*> Tulip::createListOfRunnables() {return {};}

QList<QEasingCurve::EasingFunction> Tulip::createListOfEasingFunctions() {return {[](qreal r) -> qreal { return r; }};}
void Tulip::testEasingFunctions(const QList<QEasingCurve::EasingFunction>& functions){
    int i=0;
    for(QEasingCurve::EasingFunction fun : functions){
        if(fun)
            fun(i++);
    }
}
QList<Tulip::TestStdFunction> Tulip::createListOfStdFunctions() {return {[](int,bool,double){}};}
void Tulip::testStdFunctions(const QList<Tulip::TestStdFunction>& functions){
    int i=0;
    for(Tulip::TestStdFunction fun : functions){
        if(fun){
            fun(i, i%2==1, i);
            ++i;
        }
    }
}

void Tulip::consumeIntList(const QList<int>& list){list.size();};//for (int i = 0; i < list.size(); i++) {list.at(i);}}
void Tulip::consumeStringList(const QList<QString>& list){list.size();};//for (int i = 0; i < list.size(); i++) {list.at(i);}}
void Tulip::consumeColorList(const QList<QColor>& list){list.size();};//for (int i = 0; i < list.size(); i++) {list.at(i);}}
void Tulip::consumeQObjectList(const QList<QObject*>& list){list.size();};//for (int i = 0; i < list.size(); i++) {list.at(i);}}

QSet<QRunnable*> Tulip::createSetOfRunnables() {return {};}
QHash<QString,QRunnable*> Tulip::createStringHashOfRunnables() {return {};}
QMultiHash<QString,QRunnable*> Tulip::createStringMultiHashOfRunnables() {return {};}
QMap<QString,QRunnable*> Tulip::createStringMapOfRunnables() {return {};}
QMultiMap<QString,QRunnable*> Tulip::createStringMultiMapOfRunnables() {return {};}
QList<QObject*> Tulip::createListOfObjects() {return {};}
QSet<QObject*> Tulip::createSetOfObjects() {return {};}
QHash<QString,QObject*> Tulip::createStringHashOfObjects() {return {};}
QMultiHash<QString,QObject*> Tulip::createStringMultiHashOfObjects() {return {};}
QMap<QString,QObject*> Tulip::createStringMapOfObjects() {return {};}
QMultiMap<QString,QObject*> Tulip::createStringMultiMapOfObjects() {return {};}
QList<QList<QRunnable*>> Tulip::createListOfListOfRunnables() {return {};}
QList<QList<QObject*>> Tulip::createListOfListOfObjects() {return {};}

qint64 Tulip::fillIntList(qint32 capacity){
    QList<int> list;
    QTime t1 = QTime::currentTime();
    for (int i = 0; i < capacity; i++) {
        list.append(i);
    }
    QTime t2 = QTime::currentTime();
    return t1.msecsTo(t2);
}
