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

#include <QtCore/QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
size_t qHash(const std::pair<float,double>& value, size_t seed = 0);
#endif

#include "containers.h"
#include <QtCore/QPoint>
#include <QtJambi/QtJambiAPI>
#include <QtJambi/Cast>

#include <QtCore/QtCore>

#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
inline size_t qHash(const std::pair<float,double>& value, size_t seed){
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.first);
    seed = hash(seed, value.second);
    return seed;
}
#endif

namespace ContainerTest{

QSet<QString> valueAt(QList<QSet<QString>>& container, int position){
    return container[position];
}

void copyQListFDToJavaList(const QList<QPair<float,double>>& container, jobject results){
    if(JniEnvironment env{200}){
        for(QPair<float,double> f : container){
            QtJambiAPI::addToJavaCollection(env, results, QtJambiAPI::newQPair(env, QtJambiAPI::toJavaFloatObject(env, f.first), QtJambiAPI::toJavaDoubleObject(env, f.second)));
        }
    }
}

QHASHOUT getQListFDHash(const QList<QPair<float,double>>& container){
    return qHash(container);
}

void writeQListFD(QDataStream &s, const QList<QPair<float,double>>& container){
    s << container;
}

void readQListFD(QDataStream &s, QList<QPair<float,double>>& container){
    s >> container;
}

void copyQHashShortDoubleToJavaList(const QHash<short,double>& container, jobject results){
    if(JniEnvironment env{200}){
        for(short s : container){
            QtJambiAPI::putJavaMap(env, results, QtJambiAPI::toJavaShortObject(env, s), QtJambiAPI::toJavaDoubleObject(env, container[s]));
        }
    }
}

QHASHOUT getQHashShortDoubleHash(const QHash<short,double>& container){
    return qHash(container);
}

void writeQHashShortDouble(QDataStream &s, const QHash<short,double>& container){
    s << container;
}

void readQHashShortDouble(QDataStream &s, QHash<short,double>& container){
    s >> container;
}

void copyQSetQPointToJavaList(const QSet<QPoint>& container, jobject results){
    if(JniEnvironment env{200}){
        for(QPoint s : container){
            QtJambiAPI::addToJavaCollection(env, results, qtjambi_cast<jobject>(env, s));
        }
    }
}

QHASHOUT getQSetQPointHash(const QSet<QPoint>& container){
    return qHash(container);
}

void writeQSetQPoint(QDataStream &s, const QSet<QPoint>& container){
    s << container;
}

void readQSetQPoint(QDataStream &s, QSet<QPoint>& container){
    s >> container;
}

void copyQMapShortDoubleToJavaList(const QMap<short,double>& container, jobject results){
    if(JniEnvironment env{200}){
        for(short s : container){
            QtJambiAPI::putJavaMap(env, results, QtJambiAPI::toJavaShortObject(env, s), QtJambiAPI::toJavaDoubleObject(env, container[s]));
        }
    }
}

//size_t getQMapShortDoubleHash(const QMap<short,double>& container){
//    return qHash(container);
//}

void writeQMapShortDouble(QDataStream &s, const QMap<short,double>& container){
    s << container;
}

void readQMapShortDouble(QDataStream &s, QMap<short,double>& container){
    s >> container;
}

void copyQMultiHashShortDoubleToJavaList(const QMultiHash<short,double>& container, jobject results){
    if(JniEnvironment env{200}){
        for(short s : container){
            for(double d : container.values(s)){
                QtJambiAPI::putJavaMap(env, results, QtJambiAPI::toJavaShortObject(env, s), QtJambiAPI::toJavaDoubleObject(env, d));
            }
        }
    }
}

QHASHOUT getQMultiHashShortDoubleHash(const QMultiHash<short,double>& container){
    return qHash(container);
}

void writeQMultiHashShortDouble(QDataStream &s, const QMultiHash<short,double>& container){
    s << container;
}

void readQMultiHashShortDouble(QDataStream &s, QMultiHash<short,double>& container){
    s >> container;
}

void copyQMultiMapShortDoubleToJavaList(const QMultiMap<short,double>& container, jobject results){
    if(JniEnvironment env{200}){
        for(short s : container){
            for(double d : container.values(s)){
                QtJambiAPI::putJavaMap(env, results, QtJambiAPI::toJavaShortObject(env, s), QtJambiAPI::toJavaDoubleObject(env, d));
            }
        }
    }
}

//size_t getQMultiMapShortDoubleHash(const QMultiMap<short,double>& container){
//    return qHash(container);
//}

void writeQMultiMapShortDouble(QDataStream &s, const QMultiMap<short,double>& container){
    s << container;
}

void readQMultiMapShortDouble(QDataStream &s, QMultiMap<short,double>& container){
    s >> container;
}

#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
typedef QMetaSequence::Iterable SequentialIterable;
typedef QMetaAssociation::Iterable AssociativeIterable;
#else
typedef QSequentialIterable SequentialIterable;
typedef QAssociativeIterable AssociativeIterable;
#endif

void copyFromIterable(const QVariant& variant, jobject results){
    if(JniEnvironment env{200}){
        if (variant.canConvert<SequentialIterable>()) {
            const SequentialIterable iterable = variant.value<SequentialIterable>();
            auto begin = iterable.begin();
            auto end = iterable.end();
            while(begin!=end){
                QVariant value = *begin;
                QtJambiAPI::addToJavaCollection(env, results, QtJambiAPI::convertQVariantToJavaObject(env, value));
                ++begin;
            }
        }
        else if (variant.canConvert<AssociativeIterable>()) {
            const AssociativeIterable iterable = variant.value<AssociativeIterable>();
            auto begin = iterable.begin();
            auto end = iterable.end();
            while(begin!=end){
                QVariant key = begin.key();
                QVariant value = begin.value();
                QtJambiAPI::putJavaMap(env, results, QtJambiAPI::convertQVariantToJavaObject(env, key), QtJambiAPI::convertQVariantToJavaObject(env, value));
                ++begin;
            }
        }
    }
}

QVariant sequentialAt(const QVariant& variant, int index){
    if (variant.canConvert<SequentialIterable>()) {
        const SequentialIterable iterable = variant.value<SequentialIterable>();
        return iterable.at(index);
    }else{
        return QVariant(QString("cannot convert %1 to sequential iterable").arg(variant.metaType().name()));
    }
}

QVariant sequentialRemoveFirst(QVariant variant){
    if (variant.canConvert<SequentialIterable>()) {
        SequentialIterable iterable = variant.view<SequentialIterable>();
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
        iterable.removeFirst();
#else
        iterable.removeValue(SequentialIterable::AtBegin);
#endif
    }else{
        return QVariant(QString("cannot convert %1 to sequential iterable").arg(variant.metaType().name()));
    }
    return variant;
}

QVariant sequentialRemoveLast(QVariant variant){
    if (variant.canConvert<SequentialIterable>()) {
        SequentialIterable iterable = variant.view<SequentialIterable>();
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
        iterable.removeLast();
#else
        iterable.removeValue(SequentialIterable::AtEnd);
#endif
    }else{
        return QVariant(QString("cannot convert %1 to sequential iterable").arg(variant.metaType().name()));
    }
    return variant;
}

QVariant sequentialSetAt(QVariant variant, int index, const QVariant& value){
    if (variant.canConvert<SequentialIterable>()) {
        SequentialIterable iterable = variant.view<SequentialIterable>();
        iterable.set(index, value);
    }else{
        return QVariant(QString("cannot convert %1 to sequential iterable").arg(variant.metaType().name()));
    }
    return variant;
}

QVariant sequentialAppend(QVariant variant, const QVariant& value){
    if (variant.canConvert<SequentialIterable>()) {
        SequentialIterable iterable = variant.view<SequentialIterable>();
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
        iterable.append(value);
#else
        iterable.addValue(value, QSequentialIterable::AtEnd);
#endif
    }else{
        return QVariant(QString("cannot convert %1 to sequential iterable").arg(variant.metaType().name()));
    }
    return variant;
}

QVariant sequentialPrepend(QVariant variant, const QVariant& value){
    if (variant.canConvert<SequentialIterable>()) {
        SequentialIterable iterable = variant.view<SequentialIterable>();
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
        iterable.prepend(value);
#else
        iterable.addValue(value, QSequentialIterable::AtBegin);
#endif
    }else{
        return QVariant(QString("cannot convert %1 to sequential iterable").arg(variant.metaType().name()));
    }
    return variant;
}

QVariant associativeSetValue(QVariant variant, const QVariant& key, const QVariant& value){
    if (variant.canConvert<AssociativeIterable>()) {
        AssociativeIterable iterable = variant.view<AssociativeIterable>();
        iterable.setValue(key, value);
    }else{
        return QVariant(QString("cannot convert %1 to associative iterable").arg(variant.metaType().name()));
    }
    return variant;
}

QVariant associativeInsertKey(QVariant variant, const QVariant& key){
    if (variant.canConvert<AssociativeIterable>()) {
        AssociativeIterable iterable = variant.view<AssociativeIterable>();
        iterable.insertKey(key);
    }else{
        return QVariant(QString("cannot convert %1 to associative iterable").arg(variant.metaType().name()));
    }
    return variant;
}

QVariant associativeRemoveKey(QVariant variant, const QVariant& key){
    if (variant.canConvert<AssociativeIterable>()) {
        AssociativeIterable iterable = variant.view<AssociativeIterable>();
        iterable.removeKey(key);
    }else{
        return QVariant(QString("cannot convert %1 to associative iterable").arg(variant.metaType().name()));
    }
    return variant;
}

QPair<QVariant,QVariant> associativeFindAndReplace(QVariant variant, const QVariant& key, const QVariant& value){
    if (variant.canConvert<AssociativeIterable>()) {
        AssociativeIterable iterable = variant.view<AssociativeIterable>();
        auto it = iterable.mutableFind(key);
        if(it!=iterable.mutableEnd()){
            QVariant r = it.value();
            it.value() = value;
            return {variant,r};
        }else{
            if(!key.canConvert(iterable.metaContainer().keyMetaType())){
                return {variant,QVariant(QString("cannot convert %1 to %2").arg(key.metaType().name(), iterable.metaContainer().keyMetaType().name()))};
            }
            if(!iterable.metaContainer().canCreateIteratorAtKey()){
                return {variant,QVariant(QString("QMetaContainer of %1 cannot create iterator at key").arg(variant.metaType().name()))};
            }
            return {variant,QVariant(QString("found iterator == end"))};
        }
    }else{
        return {variant,QVariant(QString("cannot convert %1 to associative iterable").arg(variant.metaType().name()))};
    }
    return {variant,QVariant()};
}

bool associativeFind(const QVariant& variant, const QVariant& key){
    if (variant.canConvert<AssociativeIterable>()) {
        AssociativeIterable iterable = variant.value<AssociativeIterable>();
        return iterable.find(key) != iterable.end();
    }else{
        qWarning() << "cannot convert" << variant.metaType().name() << " to associative iterable";
    }
    return false;
}

QVariant associativeValue(const QVariant& variant, const QVariant& key){
    if (variant.canConvert<AssociativeIterable>()) {
        AssociativeIterable iterable = variant.value<AssociativeIterable>();
        return iterable.value(key);
    }else{
        qWarning() << "cannot convert" << variant.metaType().name() << " to associative iterable";
    }
    return QVariant();
}

qsizetype containerSize(const QVariant& variant){
    if (variant.canConvert<SequentialIterable>()) {
        const SequentialIterable iterable = variant.value<SequentialIterable>();
        return iterable.size();
    }else if (variant.canConvert<AssociativeIterable>()) {
        const AssociativeIterable iterable = variant.value<AssociativeIterable>();
        return iterable.size();
    }else{
        qWarning() << "cannot convert" << variant.metaType().name() << " to associative or sequential iterable";
        return 0;
    }
}
QList<QVariant> toQVariantList(const QVariant& variant){
    return variant.value<QVariantList>();
}

QHash<QString,QVariant> toQVariantHash(const QVariant& variant){
    return variant.value<QVariantHash>();
}

QMap<QString,QVariant> toQVariantMap(const QVariant& variant){
    return variant.value<QVariantMap>();
}

} // namespace ContainerTest
